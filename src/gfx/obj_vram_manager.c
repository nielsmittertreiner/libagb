#include "agb.h"

#define TILE_ALLOC_ERROR    0xFFFF
#define MATRIX_ALLOC_ERROR  0x00FF
#define PLTT_ALLOC_ERROR    0x00FF

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

EWRAM_DATA static sprite_tile_alloc_node *s_sprite_tile_alloc_head = NULL;
EWRAM_DATA static sprite_palette_alloc_node *s_sprite_palette_alloc_head = NULL;
EWRAM_DATA static uint32_t s_sprite_matrix_bit_field;

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

    node = malloc(sizeof(sprite_tile_alloc_node));
    node->tile_start = curr->tile_start + curr->tile_count;
    node->tile_count = tile_count;
    node->next = NULL;
    curr->next = node;

    return node->tile_start;
}

uint16_t sprite_object_tiles_alloc(const sprite_object_gfx *tiles)
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
