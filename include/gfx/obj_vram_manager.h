#ifndef GUARD_AGB_UTILITY_OBJ_VRAM_MANAGER_H
#define GUARD_AGB_UTILITY_OBJ_VRAM_MANAGER_H

#include "agb.h"

typedef struct sprite_tiles
{
    const void *data;
    size_t size;
    compression_type compression;
    bpp_mode bpp;
} sprite_tiles;

typedef struct sprite_palette
{
    const void *data;
    size_t size;
    compression_type compression;
    bpp_mode bpp;
} sprite_palette;

uint16_t sprite_tiles_alloc(const sprite_tiles *data);
uint8_t sprite_tile_state_get(uint16_t tile_num);
void sprite_tile_state_set(uint16_t tile_num, uint8_t state);

#endif // GUARD_AGB_UTILITY_OBJ_VRAM_MANAGER_H