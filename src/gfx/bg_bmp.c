#include "agb.h"

IWRAM_CODE inline void bg_bmp4_pixel_plot(vec2i a, uint8_t color, void *dst)
{
    uint32_t shift = (a.x & 1) << 3;
    uint16_t *pixel = (uint16_t *)(dst + a.y * DISPLAY_WIDTH + (a.x & ~1));
    *pixel = ((0xFF00 >> shift) & *pixel) | (color << shift);
}

IWRAM_CODE inline uint8_t bg_bmp4_pixel_get(vec2i a, void *src)
{
    return *(uint16_t *)(src + a.y * DISPLAY_WIDTH + (a.x & ~1));
}

IWRAM_CODE void bg_bmp4_line_plot(vec2i a, vec2i b, uint8_t color, void *dst)
{
    vec2i d = {abs(b.x - a.x), -abs(b.y - a.y)};
    vec2i s = {a.x < b.x ? 1 : -1, a.y < b.y ? 1 : -1};
    int32_t err = d.x + d.y;
    int32_t err_xy;

    for(;;)
    {
        bg_bmp4_pixel_plot(a, color, dst);
        if (a.x == b.x && a.y == b.y)
        {
            break;
        }

        err_xy = err * 2;

        if (err_xy >= d.y)
        {
            err += d.y;
            a.x += s.x;
        }

        if (err_xy <= d.x)
        {
            err += d.x;
            a.y += s.y;
        }
   }
}

IWRAM_CODE void bg_bmp4_circle_plot(vec2i a, int32_t radius, uint8_t color, void *dst)
{
    vec2i p = {-radius, 0};
    int32_t err = 2 - 2 * radius;

    do
    {
        bg_bmp4_pixel_plot(vec2i_new(a.x - p.x, a.y + p.y), color, dst);
        bg_bmp4_pixel_plot(vec2i_new(a.x - p.y, a.y - p.x), color, dst);
        bg_bmp4_pixel_plot(vec2i_new(a.x + p.x, a.y - p.y), color, dst);
        bg_bmp4_pixel_plot(vec2i_new(a.x + p.y, a.y + p.x), color, dst);

        radius = err;
        if (radius <= p.y)
        {
            err += ++p.y * 2 + 1;
        }

        if (radius > p.x || err > p.y)
        {
            err += ++p.x * 2 + 1;
        }
    }
    while (p.x < 0);
}
