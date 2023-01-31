#ifndef GUARD_AGB_GFX_BG_BMP_H
#define GUARD_AGB_GFX_BG_BMP_H

#include "agb.h"

void bg_bmp4_pixel_plot(vec2i_t a, uint8_t color, void *dst);
uint8_t bg_bmp4_pixel_get(vec2i_t a, void *src);
void bg_bmp4_line_plot(vec2i_t a, vec2i_t b, uint8_t color, void *dst);
void bg_bmp4_circle_plot(vec2i_t a, int16_t radius, uint8_t color, void *dst);

#endif // GUARD_AGB_GFX_BG_BMP_H