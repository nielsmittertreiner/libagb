#include "agb.h"

typedef struct sprite_object_node sprite_object_node;
typedef struct sprite_object_node
{
    sprite_object object;
    sprite_object_node *next;
} sprite_object_node;

EWRAM_DATA static sprite_object_node *s_sprite_object_list = NULL;
EWRAM_DATA static sprite_object_node *s_sprite_object_sort_sub_list = NULL;
EWRAM_DATA static sprite_object_node *s_sprite_object_list_sort_tail = NULL;
EWRAM_DATA uint8_t s_sprite_object_count = 0;

sprite_ptr sprite_object_create(const sprite_object_template *template, vec2i pos)
{
    sprite_object_node *node;
    sprite_object object;

    AGB_ASSERT(s_sprite_object_count <= MAX_OAM_ENTRIES, "OAM is full!");

    object.pos = vec2fp_vec2i(pos);
    object.affine_mode = template->affine_mode;
    object.object_mode = template->object_mode;
    object.mosaic = template->mosaic;
    object.bpp = template->tiles->bpp;
    object.shape = template->shape;
    object.size = template->size;
    object.matrix_num = 0; // if affine, allocate matrix, maybe increase usage counter
    object.tile_num = sprite_object_tiles_alloc(template->tiles);
    object.palette_num = 0; // allocate palette, increase usage counter
    object.priority = template->priority;
    object.sub_priority = template->sub_priority;
    object.func = template->func;

    node = malloc(sizeof(sprite_object_node));
    node->object = object;
    node->next = s_sprite_object_list;
    s_sprite_object_list = node;
    s_sprite_object_count++;
    return &s_sprite_object_list->object;
}

void sprite_object_destroy(sprite_ptr sprite_ptr)
{
    sprite_object_node *curr = s_sprite_object_list;
    sprite_object_node *node = s_sprite_object_list;
    uint32_t index = 0;

    if (&curr->object == sprite_ptr)
    {
        s_sprite_object_count--;
        DmaClear16(3, OAM, sizeof(oam_data));
        s_sprite_object_list = curr->next;
        free(curr);
        return;
    }

    while (curr && (&curr->object != sprite_ptr))
    {
        node = curr;
        curr = curr->next;
        index++;
    }

    AGB_ASSERT(curr, "sprite_ptr not in list!");

    s_sprite_object_count--;
    DmaClear16(3, OAM + s_sprite_object_count, sizeof(oam_data));
    node->next = curr->next;
    free(curr);
}

void sprite_objects_update(void)
{
    sprite_object_node *curr = s_sprite_object_list;

    while (curr)
    {
        if (curr->object.func)
            curr->object.func(&curr->object);

        curr = curr->next;
    }
}

ARM_CODE IWRAM_CODE sprite_object_node *sprite_objects_split(sprite_object_node *start, uint32_t size)
{
    sprite_object_node *mid_prev = start;
    sprite_object_node *end = start->next;
    sprite_object_node *mid;

    for (uint32_t i = 1; i < size && (mid_prev->next || end->next); i++)
    {
        if (end->next)
        {
            end = (end->next->next) ? end->next->next : end->next;
        }
        if (mid_prev->next)
        {
            mid_prev = mid_prev->next;
        }
    }

    mid = mid_prev->next;
    s_sprite_object_sort_sub_list = end->next;
    mid_prev->next = NULL;
    end->next = NULL;
    return mid;
}

ARM_CODE IWRAM_CODE void sprite_objects_merge(sprite_object_node *a, sprite_object_node *b)
{
    sprite_object_node dummy_head;
    sprite_object_node *new_tail = &dummy_head;
    uint8_t a_priority;
    uint8_t b_priority;

    while (a && b)
    {
        a_priority = (a->object.sub_priority | (a->object.priority << 5));
        b_priority = (b->object.sub_priority | (b->object.priority << 5));

        if ((a_priority < b_priority) || (a_priority == b_priority && vec2i_vec2fp(a->object.pos).y >= vec2i_vec2fp(b->object.pos).y))
        {
            new_tail->next = a;
            a = a->next;
            new_tail = new_tail->next;
        }
        else
        {
            new_tail->next = b;
            b = b->next;
            new_tail = new_tail->next;
        }
    }

    new_tail->next = (a) ? a : b;

    while (new_tail->next)
    {
        new_tail = new_tail->next;
    }

    s_sprite_object_list_sort_tail->next = dummy_head.next;
    s_sprite_object_list_sort_tail = new_tail;
}

ARM_CODE IWRAM_CODE void sprite_objects_sort(void)
{
    sprite_object_node *start = s_sprite_object_list;
    sprite_object_node *mid;
    sprite_object_node dummy_head;

    if (s_sprite_object_list == NULL || s_sprite_object_list->next == NULL)
    {
        return;
    }

    for (uint32_t size = 1; size < s_sprite_object_count; size *= 2)
    {
        s_sprite_object_list_sort_tail = &dummy_head;

        while (start)
        {
            if (!start->next)
            {
                s_sprite_object_list_sort_tail->next = start;
                break;
            }

            mid = sprite_objects_split(start, size);
            sprite_objects_merge(start, mid);
            start = s_sprite_object_sort_sub_list;

        }
        
        start = dummy_head.next;
    }

    s_sprite_object_list = dummy_head.next;
}

void sprite_objects_commit(void)
{
    sprite_object_node *curr = s_sprite_object_list;
    oam_data oam;
    uint32_t i = 0;

    while (curr != NULL)
    {
        oam.x = vec2i_vec2fp(curr->object.pos).x;
        oam.y = vec2i_vec2fp(curr->object.pos).y;
        oam.affine_mode = curr->object.affine_mode;
        oam.object_mode = curr->object.object_mode;
        oam.mosaic = curr->object.mosaic;
        oam.bpp = curr->object.bpp;
        oam.shape = curr->object.shape;
        oam.size = curr->object.size;
        oam.matrix_num = (curr->object.matrix_num | (curr->object.h_flip << 3) | (curr->object.v_flip << 4));
        oam.tile_num = curr->object.tile_num;
        oam.priority = curr->object.priority;
        oam.palette_num = curr->object.palette_num;

        dma_copy_32(&oam, (oam_data *)OAM + i, sizeof(oam_data));
        curr = curr->next;
        i++;
    }
}

//void copy_oam_matrix_buffer_to_oam_buffer(void)
//{
//    uint32_t base;
//
//    for (uint32_t i = 0; i < MAX_OAM_MATRICES; i++)
//    {
//        base = i << 2;
//        g_oam_buffer[base + 0].affine_param = g_oam_matrix_buffer[i].a;
//        g_oam_buffer[base + 1].affine_param = g_oam_matrix_buffer[i].b;
//        g_oam_buffer[base + 2].affine_param = g_oam_matrix_buffer[i].c;
//        g_oam_buffer[base + 3].affine_param = g_oam_matrix_buffer[i].d;
//    }
//}
