#ifndef GUARD_AGB_GFX_BG_BMP_H
#define GUARD_AGB_GFX_BG_BMP_H

#include "agb.h"

void bg_bmp4_pixel_plot(vec2i a, uint8_t color, void *dst);
uint8_t bg_bmp4_pixel_get(vec2i a, void *src);
void bg_bmp4_line_plot(vec2i a, vec2i b, uint8_t color, void *dst);
void bg_bmp4_circle_plot(vec2i a, int32_t radius, uint8_t color, void *dst);

#endif // GUARD_AGB_GFX_BG_BMP_H