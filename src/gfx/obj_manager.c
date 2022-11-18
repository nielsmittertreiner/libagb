#include "agb.h"

#define TILE_ALLOC_ERROR    0xFFFF
#define MATRIX_ALLOC_ERROR  0x00FF
#define PLTT_ALLOC_ERROR    0x00FF

typedef struct sprite_object_node sprite_object_node;
typedef struct sprite_object_node
{
    sprite_object object;
    sprite_object_node *next;
} sprite_object_node;

typedef struct sprite_tile_alloc_node sprite_tile_alloc_node;
typedef struct sprite_tile_alloc_node
{
    uint16_t tile_start;
    uint16_t tile_count;
    sprite_tile_alloc_node *next;
} sprite_tile_alloc_node;

typedef struct sprite_palette_alloc_node sprite_palette_alloc_node;
typedef struct sprite_palette_alloc_node
{
    uint16_t palette_num;
    size_t size;
    uint8_t references;
    sprite_palette_alloc_node *next;
} sprite_palette_alloc_node;

static uint16_t sprite_object_tiles_alloc_data(uint16_t tile_count);
static uint16_t sprite_object_tiles_alloc(const sprite_object_gfx *data);

EWRAM_DATA static sprite_object_node *s_sprite_object_list_head = NULL;
EWRAM_DATA static sprite_object_node *s_sprite_object_sort_sub_list = NULL;
EWRAM_DATA static sprite_object_node *s_sprite_object_list_sort_tail = NULL;
EWRAM_DATA static sprite_tile_alloc_node *s_sprite_tile_alloc_head = NULL;
EWRAM_DATA static sprite_palette_alloc_node *s_sprite_palette_alloc_head = NULL;
EWRAM_DATA static uint32_t s_sprite_matrix_bit_field;
EWRAM_DATA static uint8_t s_sprite_object_count;

sprite_ptr sprite_object_create(const sprite_object_template *template, vec2i pos)
{
    sprite_object_node *node;
    sprite_object object;

    AGB_ASSERT(s_sprite_object_count <= MAX_OAM_ENTRIES, "OAM is full!");

    object.pos = vec2fp_vec2i(pos);
    object.affine = template->affine;
    object.double_size = template->double_size;
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
    node->next = s_sprite_object_list_head;
    s_sprite_object_list_head = node;
    s_sprite_object_count++;
    return &s_sprite_object_list_head->object;
}

void sprite_object_destroy(sprite_ptr sprite_ptr)
{
    sprite_object_node *curr = s_sprite_object_list_head;
    sprite_object_node *node = s_sprite_object_list_head;
    uint32_t index = 0;

    if (&curr->object == sprite_ptr)
    {
        s_sprite_object_count--;
        cpu_fast_fill(0, (void *)OAM, sizeof(oam_data));
        s_sprite_object_list_head = curr->next;
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
    cpu_fast_fill(0, (void *)(OAM + (s_sprite_object_count * sizeof(oam_data))), sizeof(oam_data));
    node->next = curr->next;
    free(curr);
}

void sprite_objects_update(void)
{
    sprite_object_node *curr = s_sprite_object_list_head;

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
    sprite_object_node *start = s_sprite_object_list_head;
    sprite_object_node dummy_head;

    if (s_sprite_object_list_head == NULL || s_sprite_object_list_head->next == NULL)
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

            sprite_objects_merge(start, sprite_objects_split(start, size));
            start = s_sprite_object_sort_sub_list;

        }
        start = dummy_head.next;
    }

    s_sprite_object_list_head = dummy_head.next;
}

void sprite_objects_commit(void)
{
    sprite_object_node *curr = s_sprite_object_list_head;
    oam_data oam;
    uint8_t i = 0;

    while (curr != NULL)
    {
        oam.y = vec2i_vec2fp(curr->object.pos).y;
        oam.affine = curr->object.affine;
        oam.double_size = curr->object.double_size;
        oam.object_mode = (3) ? SPRITE_OBJECT_NORMAL : curr->object.object_mode;
        oam.mosaic = curr->object.mosaic;
        oam.bpp = curr->object.bpp;
        oam.shape = curr->object.shape;
        oam.x = vec2i_vec2fp(curr->object.pos).x;
        oam.matrix_num = (curr->object.affine) ? curr->object.matrix_num : (curr->object.h_flip << 3) | (curr->object.v_flip << 4);
        oam.size = curr->object.size;
        oam.tile_num = curr->object.tile_num;
        oam.priority = curr->object.priority;
        oam.palette_num = curr->object.palette_num;

        dma_copy_32(&oam, (void *)(OAM + (i * sizeof(oam_data))), sizeof(oam_data));
        curr = curr->next;
        i++;
    }
}

static uint16_t sprite_object_tiles_alloc_data(uint16_t tile_count)
{
    sprite_tile_alloc_node *curr = s_sprite_tile_alloc_head;
    sprite_tile_alloc_node *node;

    if (curr == NULL)
    {
        node = malloc(sizeof(sprite_tile_alloc_node));
        node->tile_start = 1;
        node->tile_count = tile_count;
        node->next = NULL;
        s_sprite_tile_alloc_head = node;
        return node->tile_start;
    }

    while (curr->next)
    {
        if ((curr->next->tile_start - (curr->tile_start + curr->tile_count - 1) - 1) >= tile_count)
        {
            node = malloc(sizeof(sprite_tile_alloc_node));
            node->tile_start = curr->tile_start + curr->tile_count;
            node->tile_count = tile_count;
            node->next = curr->next;
            curr->next = node;
            return node->tile_start;
        }
        curr = curr->next;
    }

    if (OBJ_TILE_COUNT - (curr->tile_start + curr->tile_count - 1) >= tile_count)
    {
        node = malloc(sizeof(sprite_tile_alloc_node));
        node->tile_start = curr->tile_start + curr->tile_count;
        node->tile_count = tile_count;
        node->next = NULL;
        curr->next = node;
        return node->tile_start;
    }

    return TILE_ALLOC_ERROR;
}

static uint16_t sprite_object_tiles_alloc(const sprite_object_gfx *tiles)
{
    size_t tile_size = (tiles->bpp == BPP_4) ? TILE_SIZE_4BPP : TILE_SIZE_8BPP;
    uint16_t tile_num = sprite_object_tiles_alloc_data(tiles->size / tile_size);

    AGB_ASSERT(tiles->compression <= COMPRESSION_HUFF, "Invalid compression!");
    switch (tiles->compression)
    {
    case COMPRESSION_NONE:
        dma_copy_16(tiles->data, OBJ_VRAM0 + (tile_size * tile_num), tiles->size);
        break;
    case COMPRESSION_LZ77:
        lz77_uncomp_vram(tiles->data, OBJ_VRAM0 + (tile_size * tile_num));
        break;
    case COMPRESSION_RL:
        rl_uncomp_vram(tiles->data, OBJ_VRAM0 + (tile_size * tile_num));
        break;
    case COMPRESSION_HUFF:
        huff_uncomp(tiles->data, OBJ_VRAM0 + (tile_size * tile_num));
        break;
    }

    return ((tiles->bpp == BPP_4) ? tile_num : tile_num * 2);
}

void sprite_object_tiles_free(uint16_t tile_num)
{

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
