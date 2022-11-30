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
typedef struct PACKED sprite_tile_alloc_node
{
    const sprite_object_tiles *tiles;
    uint16_t tile_start;
    uint8_t tile_count;
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

static sprite_object_node *sprite_objects_split(sprite_object_node *start, uint32_t size);
static void sprite_objects_merge(sprite_object_node *a, sprite_object_node *b);
static uint16_t sprite_object_tiles_alloc_data(const sprite_object_tiles *tiles, uint16_t tile_count);
static uint16_t sprite_object_tiles_alloc(const sprite_object_tiles *data);

EWRAM_DATA static sprite_object_node *s_sprite_object_list_head = NULL;
EWRAM_DATA static sprite_object_node *s_sprite_object_sort_sub_list = NULL;
EWRAM_DATA static sprite_object_node *s_sprite_object_list_sort_tail = NULL;
EWRAM_DATA static sprite_tile_alloc_node *s_sprite_tile_alloc_head = NULL;
EWRAM_DATA static sprite_palette_alloc_node *s_sprite_palette_alloc_head = NULL;
EWRAM_DATA static uint32_t s_sprite_matrix_bit_field;
EWRAM_DATA static uint16_t s_sprite_tiles_used = 0;
EWRAM_DATA static uint8_t s_sprite_object_count = 0;

static const size_t s_sprite_object_tiles_sizes[3][4] =
{
    [SPRITE_SHAPE_SQUARE] =
    {
        [SPRITE_SIZE_SMALL] = 32,
        [SPRITE_SIZE_NORNAL] = 128,
        [SPRITE_SIZE_BIG] = 512,
        [SPRITE_SIZE_HUGE] = 2048,
    },
    [SPRITE_SHAPE_WIDE] =
    {
        [SPRITE_SIZE_SMALL] = 64,
        [SPRITE_SIZE_NORNAL] = 128,
        [SPRITE_SIZE_BIG] = 256,
        [SPRITE_SIZE_HUGE] = 1024,
    },
    [SPRITE_SHAPE_TALL] =
    {
        [SPRITE_SIZE_SMALL] = 64,
        [SPRITE_SIZE_NORNAL] = 128,
        [SPRITE_SIZE_BIG] = 256,
        [SPRITE_SIZE_HUGE] = 1024,
    },
};

static const vec2i s_sprite_object_dimensions[3][4] =
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
    object.shape = template->tiles->sprite_shape;
    object.size = template->tiles->sprite_size;
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
    sprite_object_node *prev = s_sprite_object_list_head;
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
        prev = curr;
        curr = curr->next;
        index++;
    }

    AGB_ASSERT(curr, "sprite_ptr not in list!");

    s_sprite_object_count--;
    cpu_fast_fill(0, (void *)(OAM + (s_sprite_object_count * sizeof(oam_data))), sizeof(oam_data));
    prev->next = curr->next;
    free(curr);
}

void sprite_objects_update(void)
{
    sprite_object_node *curr = s_sprite_object_list_head;

    while (curr)
    {
        if (curr->object.func)
            curr->object.func(&curr->object);

        curr->object.pos =  sprite_object_pos_correct(&curr->object.pos);
        sprite_object_anim_play(&curr->object);

        curr = curr->next;
    }
}

ARM_CODE IWRAM_CODE static sprite_object_node *sprite_objects_split(sprite_object_node *start, uint32_t size)
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

ARM_CODE IWRAM_CODE static void sprite_objects_merge(sprite_object_node *a, sprite_object_node *b)
{
    sprite_object_node dummy_head;
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

static uint16_t sprite_object_tiles_alloc_data(const sprite_object_tiles *tiles, uint16_t tile_count)
{
    sprite_tile_alloc_node *curr = s_sprite_tile_alloc_head;
    sprite_tile_alloc_node *node;

    if (curr == NULL)
    {
        node = malloc(sizeof(sprite_tile_alloc_node));
        node->tiles = tiles;
        node->tile_start = sprite_object_tile_start_correct(1, tiles->bpp);
        node->tile_count = tile_count;
        node->next = NULL;
        s_sprite_tile_alloc_head = node;
        s_sprite_tiles_used += tile_count;
        return node->tile_start;
    }

    while (curr->next)
    {
        if ((curr->next->tile_start - (curr->tile_start + curr->tile_count - 1) - 1) >= tile_count)
        {
            node = malloc(sizeof(sprite_tile_alloc_node));
            node->tiles = tiles;
            node->tile_start = sprite_object_tile_start_correct(curr->tile_start + curr->tile_count, tiles->bpp);
            node->tile_count = tile_count;
            node->next = curr->next;
            curr->next = node;
            s_sprite_tiles_used += tile_count;
            return node->tile_start;
        }
        curr = curr->next;
    }

    if (sprite_tiles_free() >= tile_count)
    {
        node = malloc(sizeof(sprite_tile_alloc_node));
        node->tiles = tiles;
        node->tile_start = sprite_object_tile_start_correct(curr->tile_start + curr->tile_count, tiles->bpp);
        node->tile_count = tile_count;
        node->next = NULL;
        curr->next = node;
        s_sprite_tiles_used += tile_count;
        return node->tile_start;
    }

    return TILE_ALLOC_ERROR;
}

static uint16_t sprite_object_tiles_alloc(const sprite_object_tiles *tiles)
{
    size_t size = sprite_object_tiles_size(tiles->sprite_shape, tiles->sprite_size, tiles->bpp);
    uint16_t tile_num = sprite_object_tiles_alloc_data(tiles, (size >> 5));
    void *buffer = malloc(size);

    AGB_ASSERT(tiles->compression <= COMPRESSION_HUFF, "Invalid compression!");
    switch (tiles->compression)
    {
        case COMPRESSION_NONE:
            dma_copy_32(tiles->data, buffer, size);
            break;
        case COMPRESSION_LZ77:
            lz77_uncomp_wram(tiles->data, buffer);
            break;
        case COMPRESSION_RL:
            rl_uncomp_wram(tiles->data, buffer);
            break;
        case COMPRESSION_HUFF:
            huff_uncomp(tiles->data, buffer);
            break;
    }

    dma_copy_32(buffer, (void *)(OBJ_VRAM0 + (tile_num << 5)), size);
    free(buffer);
    return (tile_num);
}

const sprite_object_tiles *sprite_object_tiles_data(sprite_ptr sprite)
{
    sprite_tile_alloc_node *curr = s_sprite_tile_alloc_head;

    while (curr)
    {
        if (sprite->tile_num == curr->tile_start)
            return curr->tiles;

        curr = curr->next;
    }

    return NULL;
}

void sprite_object_tiles_free(uint16_t tile_num)
{

}

inline size_t sprite_object_tiles_size(sprite_shape shape, sprite_size size, bpp_mode bpp)
{
    return s_sprite_object_tiles_sizes[shape][size] << bpp;
}

inline vec2i sprite_object_dimensions(sprite_shape shape, sprite_size size)
{
    return s_sprite_object_dimensions[shape][size];
}

inline int32_t sprite_object_bottom_position(sprite_ptr sprite)
{
    return (vec2i_vec2fp(sprite->pos).y + sprite_object_dimensions(sprite->shape, sprite->size).y) & 255;
}

inline vec2fp sprite_object_pos_correct(vec2fp *pos)
{
    return (vec2fp){pos->x & BITS(21), pos->y & BITS(20)};
}

inline uint16_t sprite_object_tile_start_correct(uint16_t tile_start, bpp_mode bpp)
{
    return ((OBJ_TILE_COUNT >> 1) * ((REG_DISPCNT & 3) > DISPCNT_MODE_2)) + tile_start + (tile_start & bpp);
}

inline uint16_t sprite_tiles_free(void)
{
    return OBJ_TILE_COUNT - ((OBJ_TILE_COUNT >> 1) * ((REG_DISPCNT & 3) > DISPCNT_MODE_2)) - s_sprite_tiles_used;
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
