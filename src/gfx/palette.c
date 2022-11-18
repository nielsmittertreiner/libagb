#include "agb.h"

ALIGNED(4) EWRAM_DATA static uint16_t s_pltt_buffer[PLTT_BUFF_SIZE];
ALIGNED(4) EWRAM_DATA static uint8_t s_pltt_uncomp_buffer[PLTT_BUFF_SIZE * 2];

void palette_load_compressed(const uint32_t *src, uint16_t offset, size_t size)
{
    lz77_uncomp_wram(src, s_pltt_uncomp_buffer);
    cpu_fast_copy(s_pltt_uncomp_buffer, &s_pltt_buffer[offset], size);
    cpu_fast_copy(s_pltt_uncomp_buffer, (uint16_t *)PLTT + offset, size);
}

void palette_load(const void *src, uint16_t offset, compression_type compression, size_t size)
{
    switch (compression)
    {
        case COMPRESSION_NONE:
            cpu_fast_copy(src, &s_pltt_buffer[offset], size);
            cpu_fast_copy(src, (uint16_t *)PLTT + offset, size);
            break;
        case COMPRESSION_LZ77:
            lz77_uncomp_wram(src, (uint8_t *)&s_pltt_buffer[offset]);
            lz77_uncomp_wram(src, (uint16_t *)PLTT + offset);
            break;
        case COMPRESSION_RL:
            rl_uncomp_vram(src, (uint8_t *)&s_pltt_buffer[offset]);
            rl_uncomp_vram(src, (uint16_t *)PLTT + offset);
            break;
        case COMPRESSION_HUFF:
            huff_uncomp(src, (uint8_t *)&s_pltt_buffer[offset]);
            huff_uncomp(src, (uint16_t *)PLTT + offset);
            break;
    }

    //cpu_fast_copy(src, &s_pltt_buffer[offset], size);
    //cpu_fast_copy(src, (uint16_t *)PLTT + offset, size);
}

void palette_fill(uint16_t color, uint16_t offset, size_t size)
{
    cpu_fast_fill(((color << 16) | (color)), &s_pltt_buffer[offset], size);
    cpu_fast_fill(((color << 16) | (color)), (uint16_t *)PLTT + offset, size);
}

void palette_shift(uint16_t offset, size_t size)
{
    uint16_t slots = size >> 1;
    uint16_t buffer[slots];

    cpu_copy_16((uint16_t *)PLTT + offset, buffer, size);

    for (uint32_t i = 0; i < slots; i++)
    {
        s_pltt_buffer[offset + i] = s_pltt_buffer[offset + (i + 1)];
        buffer[i] = buffer[i + 1];
    }

    s_pltt_buffer[offset + slots - 1] = s_pltt_buffer[offset];
    buffer[slots - 1] = *(uint16_t *)(PLTT + offset);

    cpu_copy_16(buffer, (uint16_t *)PLTT + offset, size);
}

void palette_shift_fast(uint16_t offset, size_t size)
{
    uint16_t slots = size >> 1;
    uint16_t buffer[slots];

    cpu_fast_copy((uint16_t *)PLTT + offset, buffer, size);

    for (uint32_t i = 0; i < slots; i++)
    {
        s_pltt_buffer[offset + i] = s_pltt_buffer[offset + (i + 1)];
        buffer[i] = buffer[i + 1];
    }

    s_pltt_buffer[offset + slots - 1] = s_pltt_buffer[offset];
    buffer[slots - 1] = *(uint16_t *)(PLTT + offset);

    cpu_fast_copy(buffer, (void *)PLTT + offset, size);
}

uint16_t color_blend(uint16_t color1, uint16_t color2, uint8_t y)
{
    uint8_t r1 = get_r(color1);
    uint8_t g1 = get_g(color1);
    uint8_t b1 = get_b(color1);

    uint8_t r2 = get_r(color2);
    uint8_t g2 = get_g(color2);
    uint8_t b2 = get_b(color2);

    return rgb5((r1 + (((r2 - r1) * y) >> 5)),
                (g1 + (((g2 - g1) * y) >> 5)),
                (b1 + (((b2 - b1) * y) >> 5)));
}

void palette_blend(uint16_t offset, uint16_t color, uint8_t y, size_t size)
{
    uint16_t *dest = (uint16_t *)PLTT + offset;

    for (uint32_t i = 0; i < size; i++, dest++)
        *dest = color_blend(s_pltt_buffer[offset + i], color, y);
}

inline uint8_t get_r(uint16_t color)
{
    return ((color) & 0x1F);
}

inline uint8_t get_g(uint16_t color)
{
    return ((color >> 5) & 0x1F);
}

inline uint8_t get_b(uint16_t color)
{
    return ((color >> 10) & 0x1F);
}

inline uint16_t rgb5(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r) | (g << 5) | (b << 10));
}

inline uint16_t rgb8(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10));
}

inline uint16_t pltt_offset(uint8_t n)
{
    return (n << 4);
}

inline size_t pltt_sizeof(size_t n)
{
    return (n << 1);
}



