#include "agb.h"  

extern fp32_t sin_lut[512];

//inline int32_t abs(int32_t n)
//{
//    int32_t mask = n >> 31;
//    return (n ^ mask) - mask;
//}


// fp16_t function for 8.8 fixed point math

inline fp16_t fp16_fp32(fp32_t n)
{
    return n >> 4;
}

inline fp16_t fp16_uint(uint8_t n)
{
    return (n << FP16_PRECISION);
}

inline fp16_t fp16_int(int8_t n)
{
    return (n * FP16_SCALE);
}

inline fp16_t fp16_float(float n)
{
    return ((fp16_t)(n * FP16_SCALE));
}

inline fp16_t fp16_double(double n)
{
    return ((fp16_t)(n * FP16_SCALE));
}

inline uint8_t uint_fp16(fp16_t n)
{
    return ((uint8_t)(n >> FP16_PRECISION));
}

inline int8_t int_fp16(fp16_t n)
{
    return ((int8_t)(n / FP16_SCALE));
}

inline float float_fp16(fp16_t n)
{
    return ((float)(n / FP16_SCALE));
}

inline double double_fp16(fp16_t n)
{
    return ((double)(n / FP16_SCALE));
}

inline fp16_t fp16_mul(fp16_t a, fp16_t b)
{
    return (((a) * b) >> FP16_PRECISION);
}

inline fp16_t fp16_div(fp16_t a, fp16_t b)
{
    return (((a) << FP16_PRECISION) / b);
}

inline fp16_t fp16_floor(fp16_t n)
{
    return n & FP16_SCALE;
}

inline fp16_t fp16_ceil(fp16_t n)
{
    if (n & FP16_SCALE)
        return fp16_floor(n + fp16_uint(1));
    else
        return n;
}

inline fp16_t fp16_round(fp16_t n)
{
    return fp16_floor(n + fp16_float(0.5f));
}


// fp32_t function for 20.12 fixed point math

inline fp32_t fp32_fp16(fp16_t n)
{
    return n << 4;
}

inline fp32_t fp32_uint(uint16_t n)
{
    return (n << FP32_PRECISION);
}

inline fp32_t fp32_int(int16_t n)
{
    return (n * FP32_SCALE);
}

inline fp32_t fp32_float(float n)
{
    return ((fp32_t)(n * FP32_SCALE));
}

inline fp32_t fp32_double(double n)
{
    return ((fp32_t)(n * FP32_SCALE));
}

inline uint16_t uint_fp32(fp32_t n)
{
    return ((uint16_t)(n >> FP32_PRECISION));
}

inline int16_t int_fp32(fp32_t n)
{
    return ((int16_t)(n / FP32_SCALE));
}

inline float float_fp32(fp32_t n)
{
    return ((float)(n / FP32_SCALE));
}

inline double double_fp32(fp32_t n)
{
    return ((double)(n / FP32_SCALE));
}

inline fp32_t fp32_mul(fp32_t a, fp32_t b)
{
    return (((int64_t)(a) * b) >> FP32_PRECISION);
}

inline fp32_t fp32_div(fp32_t a, fp32_t b)
{
    return ((((int64_t)a) << FP32_PRECISION) / b);
}

inline fp32_t fp32_floor(fp32_t n)
{
    return n & FP32_SCALE;
}

inline fp32_t fp32_ceil(fp32_t n)
{
    if (n & FP32_SCALE)
        return fp32_floor(n + fp32_uint(1));
    else
        return n;
}

inline fp32_t fp32_round(fp32_t n)
{
    return fp32_floor(n + fp32_float(0.5f));
}


// vec2i functions for integer vector math

inline vec2i_t vec2i_new(int16_t x, int16_t y)
{
    return (vec2i_t){x, y};
}

inline vec2i_t vec2i_add(const vec2i_t *a, const vec2i_t *b)
{
    return (vec2i_t){a->x + b->x, a->y + b->y};
}

inline vec2i_t vec2i_sub(const vec2i_t *a, const vec2i_t *b)
{
    return (vec2i_t){a->x - b->x, a->y - b->y};
}

inline vec2i_t vec2i_mul(const vec2i_t *a, const vec2i_t *b)
{
    return (vec2i_t){a->x * b->x, a->y * b->y};
}

inline vec2i_t vec2i_scale(const vec2i_t *a, uint16_t scale)
{
    return (vec2i_t){a->x * scale, a->y * scale};
}

inline uint16_t vec2i_dot(const vec2i_t *a, const vec2i_t *b)
{
    return (a->x * b->x) + (a->y * b->y);
}

inline vec2i_t vec2i_vec2fp16(const vec2fp16_t *a)
{
    return (vec2i_t){int_fp16(a->x), int_fp16(a->y)};
}

inline vec2i_t vec2i_vec2fp32(const vec2fp32_t *a)
{
    return (vec2i_t){int_fp32(a->x), int_fp32(a->y)};
}


// vec2fp16 functions for 8.8 fixed point vector math

vec2fp16_t vec2fp16_new(fp16_t x, fp16_t y)
{
    return (vec2fp16_t){x, y};
}

vec2fp16_t vec2fp16_add(const vec2fp16_t *a, const vec2fp16_t *b)
{
    return (vec2fp16_t){a->x + b->x, a->y + b->y};
}

vec2fp16_t vec2fp16_sub(const vec2fp16_t *a, const vec2fp16_t *b)
{
    return (vec2fp16_t){a->x - b->x, a->y - b->y};
}

vec2fp16_t vec2fp16_mul(const vec2fp16_t *a, const vec2fp16_t *b)
{
    return (vec2fp16_t){fp16_mul(a->x, b->x), fp16_mul(a->y, b->y)};
}

vec2fp16_t vec2fp16_scale(const vec2fp16_t *a, fp16_t scale)
{
    return (vec2fp16_t){fp16_mul(a->x, scale), fp16_mul(a->y, scale)};
}

fp16_t vec2fp16_dot(const vec2fp16_t *a, const vec2fp16_t *b)
{
    return (fp16_t)fp16_mul(a->x, b->x) + fp16_mul(a->y, b->y);
}

vec2fp16_t vec2fp16_vec2i(const vec2i_t *a)
{
    return (vec2fp16_t){fp16_int(a->x), fp16_int(a->y)};
}

vec2fp16_t vec2fp16_vec2fp32(const vec2fp32_t *a)
{
    return (vec2fp16_t){fp16_fp32(a->x), fp16_fp32(a->y)};
}


// vec2fp32 functions for 20.12 fixed point vector math

inline vec2fp32_t vec2fp32_new(fp32_t x, fp32_t y)
{
    return (vec2fp32_t){x, y};
}

inline vec2fp32_t vec2fp32_add(const vec2fp32_t *a, const vec2fp32_t *b)
{
    return (vec2fp32_t){a->x + b->x, a->y + b->y};
}

inline vec2fp32_t vec2fp32_sub(const vec2fp32_t *a, const vec2fp32_t *b)
{
    return (vec2fp32_t){a->x - b->x, a->y - b->y};
}

inline vec2fp32_t vec2fp32_mul(const vec2fp32_t *a, const vec2fp32_t *b)
{
    return (vec2fp32_t){fp32_mul(a->x, b->x), fp32_mul(a->y, b->y)};
}

inline vec2fp32_t vec2fp32_scale(const vec2fp32_t *a, fp32_t scale)
{
    return (vec2fp32_t){fp32_mul(a->x, scale), fp32_mul(a->y, scale)};
}

inline fp32_t vec2fp32_dot(const vec2fp32_t *a, const vec2fp32_t *b)
{
    return (fp32_t)fp32_mul(a->x, b->x) + fp32_mul(a->y, b->y);
}

inline vec2fp32_t vec2fp32_vec2i(const vec2i_t *a)
{
    return (vec2fp32_t){fp32_int(a->x), fp32_int(a->y)};
}

inline vec2fp32_t vec2fp32_vec2fp16(const vec2fp16_t *a)
{
    return (vec2fp32_t){fp32_fp16(a->x), fp32_fp16(a->y)};
}

inline fp32_t lu_sin(uint16_t theta)
{
    return sin_lut[theta % 0x1FF];
}

inline fp32_t lu_cos(uint16_t theta)
{
    return sin_lut[(theta + 128) % 0x1FF];
}
