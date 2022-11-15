#ifndef GUARD_AGB_UTILITY_MATH_H
#define GUARD_AGB_UTILITY_MATH_H

#include "agb.h"

// fixed point helpers
#define FP32_PRECISION     12
#define FP16_PRECISION     8
#define FP32_SCALE         (1 << FP32_PRECISION)
#define FP16_SCALE         (1 << FP16_PRECISION)

// math helpers
#define ARRAY_COUNT(array)              ((size_t)(sizeof(array) / sizeof((array)[0])))
#define HIHALF(n)                       (((n) & 0xFFFF0000) >> 16)
#define LOHALF(n)                       ((n) & 0xFFFF)
#define MIN(a, b)                       ((a) < (b) ? (a) : (b))
#define MAX(a, b)                       ((a) >= (b) ? (a) : (b))
#define ABS(n)                          (((n) < 0) ? -(n) : (n))

#define SWAP(a, b, temp)    \
{                           \
    temp = a;               \
    a = b;                  \
    b = temp;               \
}

//int32_t abs(int32_t n);

fp16_t fp16_uint(uint8_t n);
fp32_t fp32_uint(uint16_t n);
fp16_t fp16_int(int8_t n);
fp32_t fp32_int(int16_t n);
fp16_t fp16_float(float n);
fp32_t fp32_float(float n);
fp16_t fp16_double(double n);
fp32_t fp32_double(double n);

uint8_t uint_fp16(fp16_t n);
uint16_t uint_fp32(fp32_t n);
int8_t int_fp16(fp16_t n);
int16_t int_fp32(fp32_t n);
float float_fp16(fp16_t n);
float float_fp32(fp32_t n);
double double_fp16(fp16_t n);
double double_fp32(fp32_t n);

fp16_t fp16_mul(fp16_t a, fp16_t b);
fp32_t fp32_mul(fp32_t a, fp32_t b);
fp16_t fp16_div(fp16_t a, fp16_t b);
fp32_t fp32_div(fp32_t a, fp32_t b);
fp16_t fp16_floor(fp16_t n);
fp32_t fp32_floor(fp32_t n);
fp16_t fp16_ceil(fp16_t n);
fp32_t fp32_ceil(fp32_t n);
fp16_t fp16_round(fp16_t n);
fp32_t fp32_round(fp32_t n);

vec2i vec2i_new(int32_t x, int32_t y);
vec2fp vec2fp_new(fp32_t x, fp32_t y);
vec2i vec2i_vec2fp(vec2fp a);
vec2fp vec2fp_vec2i(vec2i a);

#endif // GUARD_AGB_UTILITY_BENCHMARK_H