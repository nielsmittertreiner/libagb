#include "agb.h"

//inline int32_t abs(int32_t n)
//{
//    int32_t mask = n >> 31;
//    return (n ^ mask) - mask;
//}

inline fp16_t fp16_uint(uint8_t n)
{
    return (n << FP16_PRECISION);
}

inline fp32_t fp32_uint(uint16_t n)
{
    return (n << FP32_PRECISION);
}

inline fp16_t fp16_int(int8_t n)
{
    return (n * FP16_SCALE);
}

inline fp32_t fp32_int(int16_t n)
{
    return (n * FP32_SCALE);
}

inline fp16_t fp16_float(float n)
{
    return ((fp16_t)(n * FP16_SCALE));
}

inline fp32_t fp32_float(float n)
{
    return ((fp32_t)(n * FP32_SCALE));
}

inline fp16_t fp16_double(double n)
{
    return ((fp16_t)(n * FP16_SCALE));
}

inline fp32_t fp32_double(double n)
{
    return ((fp32_t)(n * FP32_SCALE));
}

inline uint8_t uint_fp16(fp16_t n)
{
    return ((uint8_t)(n >> FP16_PRECISION));
}

inline uint16_t uint_fp32(fp32_t n)
{
    return ((uint16_t)(n >> FP32_PRECISION));
}

inline int8_t int_fp16(fp16_t n)
{
    return ((int8_t)(n / FP16_SCALE));
}

inline int16_t int_fp32(fp32_t n)
{
    return ((int16_t)(n / FP32_SCALE));
}

inline float float_fp16(fp16_t n)
{
    return ((float)(n / FP16_SCALE));
}

inline float float_fp32(fp32_t n)
{
    return ((float)(n / FP32_SCALE));
}

inline double double_fp16(fp16_t n)
{
    return ((double)(n / FP16_SCALE));
}

inline double double_fp32(fp32_t n)
{
    return ((double)(n / FP32_SCALE));
}

inline fp16_t fp16_mul(fp16_t a, fp16_t b)
{
    return (((a) * b) >> FP16_PRECISION);
}

inline fp32_t fp32_mul(fp32_t a, fp32_t b)
{
    return (((int64_t)(a) * b) >> FP32_PRECISION);
}

inline fp16_t fp16_div(fp16_t a, fp16_t b)
{
    return (((a) << FP16_PRECISION) / b);
}

inline fp32_t fp32_div(fp32_t a, fp32_t b)
{
    return ((((int64_t)a) << FP32_PRECISION) / b);
}

inline fp16_t fp16_floor(fp16_t n)
{
    return n & FP16_SCALE;
}

inline fp32_t fp32_floor(fp32_t n)
{
    return n & FP32_SCALE;
}

inline fp16_t fp16_ceil(fp16_t n)
{
    if (n & FP16_SCALE)
        return fp16_floor(n + fp16_uint(1));
    else
        return n;
}

inline fp32_t fp32_ceil(fp32_t n)
{
    if (n & FP32_SCALE)
        return fp32_floor(n + fp32_uint(1));
    else
        return n;
}

inline fp16_t fp16_round(fp16_t n)
{
    return fp16_floor(n + fp16_double(0.5));
}

inline fp32_t fp32_round(fp32_t n)
{
    return fp32_floor(n + fp32_double(0.5));
}

inline vec2i vec2i_new(int32_t x, int32_t y)
{
    return (vec2i){x, y};
}

inline vec2fp vec2fp_new(fp32_t x, fp32_t y)
{
    return (vec2fp){x, y};
}

inline vec2i vec2fp_to_vec2i(vec2fp a)
{
    return (vec2i){int_fp32(a.x), int_fp32(a.y)};
}

inline vec2fp vec2i_to_vec2fp(vec2i a)
{
    return (vec2fp){fp32_int(a.x), fp32_int(a.y)};
}
