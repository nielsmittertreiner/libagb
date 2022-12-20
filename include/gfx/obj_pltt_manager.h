#ifndef GUARD_AGB_GFX_OBJ_PLTT_H
#define GUARD_AGB_GFX_OBJ_PLTT_H

#include "agb.h"

uint16_t sprite_object_palette_alloc(const sprite_object_palette *palette);
void sprite_object_palette_free(uint16_t palette_num);

#endif // GUARD_AGB_GFX_OBJ_PLTT_H