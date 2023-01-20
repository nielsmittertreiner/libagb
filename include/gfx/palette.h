#ifndef GUARD_AGB_GFX_PALETTE_H
#define GUARD_AGB_GFX_PALETTE_H

#include "agb.h"

#define PLTT_Y_MIN          0x00
#define PLTT_Y_MAX          0x20 
#define PLTT_SIZEOF_4BPP    pltt_sizeof(16)
#define PLTT_SIZEOF_8BPP    pltt_sizeof(256)

typedef enum bpp_mode
{
    BPP_4,
    BPP_8,
} bpp_mode;

void palette_load(const void *src, uint16_t offset, compression_type compression, size_t size);
void palette_fill(uint16_t color, uint16_t offset, size_t size);
void palette_shift(uint16_t offset, size_t size);
void palette_shift_fast(uint16_t offset, size_t size);
uint16_t color_blend(uint16_t color1, uint16_t color2, uint8_t y);
void palette_blend(uint16_t offset, uint16_t color, uint8_t y, size_t size);
uint8_t get_r(uint16_t color);
uint8_t get_g(uint16_t color);
uint8_t get_b(uint16_t color);
uint16_t rgb5(uint8_t r, uint8_t g, uint8_t b);
uint16_t rgb8(uint8_t r, uint8_t g, uint8_t b);
uint16_t pltt_offset(uint8_t n);
size_t pltt_sizeof(size_t n);

#endif // GUARD_AGB_GFX_PALETTE_H