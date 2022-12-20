#ifndef GUARD_AGB_GFX_OBJ_VRAM_H
#define GUARD_AGB_GFX_OBJ_VRAM_H

#include "agb.h"

uint16_t sprite_object_tiles_alloc(const sprite_object_tiles *tiles);
void sprite_object_tiles_free(uint16_t tile_num);
size_t sprite_object_tiles_size(sprite_shape shape, sprite_size size, bpp_mode bpp);
uint16_t sprite_object_tile_start_correct(uint16_t tile_start, bpp_mode bpp);
const sprite_object_tiles *sprite_object_tiles_data(sprite_ptr sprite);
uint16_t sprite_tiles_free(void);

#endif // GUARD_AGB_GFX_OBJ_VRAM_H