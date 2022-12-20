#include "agb.h"

#define MAX_OBJ_TILE_COUNT  1024
#define TILE_ALLOC_ERROR    0xFFFF
#define MATRIX_ALLOC_ERROR  0x00FF

typedef struct sprite_tile_alloc_node sprite_tile_alloc_node;
typedef struct sprite_tile_alloc_node
{
    const sprite_object_tiles *tiles;
    uint16_t tile_start;
    uint8_t tile_count;
    sprite_tile_alloc_node *next;
} sprite_tile_alloc_node;

typedef struct sprite_tile_buffer_node sprite_tile_buffer_node;
typedef struct sprite_tile_buffer_node
{
    const sprite_object_tiles *tiles;
    void *data;
    uint16_t references;
    sprite_tile_buffer_node *next;
} sprite_tile_buffer_node;

static uint16_t sprite_object_tiles_alloc_data(const sprite_object_tiles *tiles, uint16_t tile_count);
static void *sprite_object_tiles_buffer_alloc(const sprite_object_tiles *tiles);
static void sprite_object_tiles_buffer_free(const sprite_object_tiles *tiles);

EWRAM_DATA static sprite_tile_alloc_node *s_sprite_tile_alloc_head = NULL;
EWRAM_DATA static sprite_tile_buffer_node *s_sprite_tile_buffer_head = NULL;
EWRAM_DATA static uint16_t s_sprite_tiles_used = 0;

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

uint16_t sprite_object_tiles_alloc(const sprite_object_tiles *tiles)
{
    size_t frame_size = sprite_object_tiles_size(tiles->sprite_shape, tiles->sprite_size, tiles->bpp);
    uint16_t tile_num = sprite_object_tiles_alloc_data(tiles, (frame_size >> 5));
    const void *buffer = sprite_object_tiles_buffer_alloc(tiles);

    dma_copy_32(buffer, (void *)(OBJ_VRAM0 + (tile_num << 5)), frame_size);
    return tile_num;
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

static void *sprite_object_tiles_buffer_alloc(const sprite_object_tiles *tiles)
{
    sprite_tile_buffer_node *curr = s_sprite_tile_buffer_head;
    sprite_tile_buffer_node *node;

    while (curr)
    {
        if (curr->tiles == tiles)
        {
            curr->references++;
            return curr->data;
        }

        curr = curr->next;
    }

    node = malloc(sizeof(sprite_tile_buffer_node));
    node->tiles = tiles;
    node->data = malloc(tiles->size);
    node->references = 1;
    node->next = s_sprite_tile_buffer_head;
    s_sprite_tile_buffer_head = node;

    switch (tiles->compression)
    {
        case COMPRESSION_NONE:
            dma_copy_32(tiles->data, node->data, tiles->size);
            break;
        case COMPRESSION_LZ77:
            lz77_uncomp_wram(tiles->data, node->data);
            break;
        case COMPRESSION_RL:
            rl_uncomp_wram(tiles->data, node->data);
            break;
        case COMPRESSION_HUFF:
            huff_uncomp(tiles->data, node->data);
            break;
    }

    return node->data;
}

void sprite_object_tiles_free(uint16_t tile_num)
{

}

static void sprite_object_tiles_buffer_free(const sprite_object_tiles *tiles)
{
    sprite_tile_buffer_node *curr = s_sprite_tile_buffer_head;
    sprite_tile_buffer_node *prev = s_sprite_tile_buffer_head;

    while (curr)
    {
        if (curr->tiles == tiles)
        {   
            curr->references--;
            if (curr->references == 0)
            {
                prev->next = curr->next;
                free(curr);
            }
            return;
        }

        prev = curr;
        curr = curr->next;
    }
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

inline size_t sprite_object_tiles_size(sprite_shape shape, sprite_size size, bpp_mode bpp)
{
    return s_sprite_object_tiles_sizes[shape][size] << bpp;
}

inline uint16_t sprite_object_tile_start_correct(uint16_t tile_start, bpp_mode bpp)
{
    return ((MAX_OBJ_TILE_COUNT >> 1) * ((REG_DISPCNT & 3) > DISPCNT_MODE_2)) + tile_start + (tile_start & bpp);
}

inline uint16_t sprite_tiles_free(void)
{
    return MAX_OBJ_TILE_COUNT - ((MAX_OBJ_TILE_COUNT >> 1) * ((REG_DISPCNT & 3) > DISPCNT_MODE_2)) - s_sprite_tiles_used;
}
