#include "agb.h"
#include "gfx/obj_vram_manager.h"
#include "gfx/obj_pltt_manager.h"

typedef struct sprite_object_node sprite_object_node;
typedef struct sprite_object_node
{
    sprite_object object;
    sprite_object_node *next;
} sprite_object_node;

static sprite_object_node *sprite_objects_split(sprite_object_node *start, size_t size);
static void sprite_objects_merge(sprite_object_node *a, sprite_object_node *b);

EWRAM_DATA static sprite_object_node *sprite_object_head = NULL;
EWRAM_DATA static sprite_object_node *sprite_object_sort_sub_head = NULL;
EWRAM_DATA static sprite_object_node *sprite_object_sort_tail = NULL;
EWRAM_DATA static uint32_t sprite_matrix_bit_field;
IWRAM_DATA static uint32_t sprite_object_count;

static const vec2i_t s_sprite_object_dimensions[3][4] =
{
    [SPRITE_SHAPE_SQUARE] =
    {
        [SPRITE_SIZE_SMALL] = {8, 8},
        [SPRITE_SIZE_NORNAL] = {16, 16},
        [SPRITE_SIZE_BIG] = {32, 32},
        [SPRITE_SIZE_HUGE] = {64, 64},
    },
    [SPRITE_SHAPE_WIDE] =
    {
        [SPRITE_SIZE_SMALL] = {16, 8},
        [SPRITE_SIZE_NORNAL] = {32, 8},
        [SPRITE_SIZE_BIG] = {32, 16},
        [SPRITE_SIZE_HUGE] = {64, 32},
    },
    [SPRITE_SHAPE_TALL] =
    {
        [SPRITE_SIZE_SMALL] = {8, 16},
        [SPRITE_SIZE_NORNAL] = {8, 32},
        [SPRITE_SIZE_BIG] = {16, 32},
        [SPRITE_SIZE_HUGE] = {32, 64},
    },
};

sprite_ptr sprite_object_create(const sprite_object_template *template, vec2i_t pos)
{
    sprite_object_node *node;

    AGB_ASSERT(sprite_object_count <= MAX_OAM_ENTRIES, "OAM is full!");
    node = malloc(sizeof(sprite_object_node));
    AGB_ASSERT(node, "Not enough memory for sprite_object");

    node->object.pos = vec2fp32_vec2i(&pos);
    node->object.affine = template->affine;
    node->object.double_size = template->double_size;
    node->object.object_mode = template->object_mode;
    node->object.mosaic = template->mosaic;
    node->object.bpp = template->tiles->bpp;
    node->object.shape = template->tiles->sprite_shape;
    node->object.size = template->tiles->sprite_size;
    node->object.matrix_num = 0; // if affine, allocate matrix, maybe increase usage counter
    node->object.tile_num = sprite_object_tiles_alloc(template->tiles);
    node->object.palette_num = sprite_object_palette_alloc(template->palette);
    node->object.priority = template->priority;
    node->object.sub_priority = template->sub_priority;
    node->object.centered = FALSE;
    node->object.v_flip = FALSE;
    node->object.h_flip = FALSE;
    node->object.func = template->func;

    node->next = sprite_object_head;
    sprite_object_head = node;
    sprite_object_count++;
    return &sprite_object_head->object;
}

void sprite_object_destroy(sprite_ptr sprite)
{
    sprite_object_node *curr = sprite_object_head;
    sprite_object_node *prev = sprite_object_head;
    uint32_t index = 0;

    if (&curr->object == sprite)
    {
        sprite_object_count--;
        cpu_fill_16(0, (void *)OAM, sizeof(oam_data) - sizeof(uint16_t));
        sprite_object_head = curr->next;
        free(curr);
        return;
    }

    while (curr && (&curr->object != sprite))
    {
        prev = curr;
        curr = curr->next;
        index++;
    }

    AGB_ASSERT(curr, "sprite not in list!");

    sprite_object_count--;
    cpu_fill_16(0, (void *)(OAM + (index * sizeof(oam_data))), sizeof(oam_data) - sizeof(uint16_t));
    prev->next = curr->next;
    free(curr);
}

ARM_CODE IWRAM_CODE void sprite_objects_update(void)
{
    sprite_object_node *curr = sprite_object_head;

    while (curr)
    {
        if (curr->object.func)
            curr->object.func(&curr->object);

        curr->object.pos =  sprite_object_pos_correct(&curr->object.pos);
        sprite_object_anim_play(&curr->object);

        curr = curr->next;
    }
}

ARM_CODE IWRAM_CODE void sprite_objects_sort(void)
{
    sprite_object_node *start = sprite_object_head;
    sprite_object_node dummy_head = {0};

    if (sprite_object_head == NULL || sprite_object_head->next == NULL)
    {
        return;
    }

    for (uint32_t size = 1; size < sprite_object_count; size *= 2)
    {
        sprite_object_sort_tail = &dummy_head;

        while (start)
        {
            if (!start->next)
            {
                sprite_object_sort_tail->next = start;
                break;
            }

            sprite_objects_merge(start, sprite_objects_split(start, size));
            start = sprite_object_sort_sub_head;

        }
        start = dummy_head.next;
    }

    sprite_object_head = dummy_head.next;
}

ARM_CODE IWRAM_CODE static sprite_object_node *sprite_objects_split(sprite_object_node *start, size_t size)
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
    sprite_object_sort_sub_head = end->next;
    mid_prev->next = NULL;
    end->next = NULL;
    return mid;
}

ARM_CODE IWRAM_CODE static void sprite_objects_merge(sprite_object_node *a, sprite_object_node *b)
{
    sprite_object_node dummy_head = {0};
    sprite_object_node *new_tail = &dummy_head;
    uint8_t a_priority;
    uint8_t b_priority;

    while (a && b)
    {
        a_priority = (a->object.sub_priority | (a->object.priority << 5));
        b_priority = (b->object.sub_priority | (b->object.priority << 5));

        if ((a_priority < b_priority) || (a_priority == b_priority && sprite_object_bottom_position(&a->object) >= sprite_object_bottom_position(&b->object)))
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

    sprite_object_sort_tail->next = dummy_head.next;
    sprite_object_sort_tail = new_tail;
}

ARM_CODE IWRAM_CODE void sprite_objects_commit(void)
{
    sprite_object_node *curr = sprite_object_head;
    oam_data oam;
    vec2i_t pos;
    uint8_t i = 0;

    while (curr != NULL)
    {
        pos = vec2i_vec2fp32(&curr->object.pos);
        if (curr->object.centered)
        {
            const vec2i_t *dim = sprite_object_dimensions(curr->object.shape, curr->object.size);
            const vec2i_t mid = vec2i_new(dim->x / 2, dim->y / 2);
            pos = vec2i_sub(&pos, &mid);
        }

        oam.y = pos.y;
        oam.affine = curr->object.affine;
        oam.double_size = curr->object.double_size;
        oam.object_mode = (3) ? SPRITE_OBJECT_NORMAL : curr->object.object_mode;
        oam.mosaic = curr->object.mosaic;
        oam.bpp = curr->object.bpp;
        oam.shape = curr->object.shape;
        oam.x = pos.x;
        oam.matrix_num = (curr->object.affine) ? curr->object.matrix_num : (curr->object.h_flip << 3) | (curr->object.v_flip << 4);
        oam.size = curr->object.size;
        oam.tile_num = curr->object.tile_num;
        oam.priority = curr->object.priority;
        oam.palette_num = curr->object.palette_num;

        *(OAM + i) = oam;
        curr = curr->next;
        i++;
    }
}

inline const vec2i_t *sprite_object_dimensions(sprite_shape shape, sprite_size size)
{
    return &s_sprite_object_dimensions[shape][size];
}

inline uint32_t sprite_object_bottom_position(sprite_ptr sprite)
{
    return (vec2i_vec2fp32(&sprite->pos).y + sprite_object_dimensions(sprite->shape, sprite->size)->y) & BITS(8);
}

inline vec2fp32_t sprite_object_pos_correct(vec2fp32_t *pos)
{
    return (vec2fp32_t){pos->x & BITS(21), pos->y & BITS(20)};
}
