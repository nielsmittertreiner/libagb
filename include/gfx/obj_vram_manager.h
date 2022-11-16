#ifndef GUARD_AGB_UTILITY_OBJ_VRAM_MANAGER_H
#define GUARD_AGB_UTILITY_OBJ_VRAM_MANAGER_H

#include "agb.h"

typedef struct sprite_object_gfx
{
    const void *data;
    size_t size;
    compression_type compression;
    bpp_mode bpp;
} sprite_object_gfx;

uint16_t sprite_object_tiles_alloc(const sprite_object_gfx *data);

#endif // GUARD_AGB_UTILITY_OBJ_VRAM_MANAGER_H