#ifndef GUARD_AGB_UTILITY_MATH_H
#define GUARD_AGB_UTILITY_MATH_H

#include "agb.h"

// fixed point helpers
#define FP16_PRECISION     8
#define FP16_SCALE         (1 << FP16_PRECISION)
#define FP32_PRECISION     12
#define FP32_SCALE         (1 << FP32_PRECISION)

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

fp16_t fp16_fp32(fp32_t n);
fp16_t fp16_uint(uint8_t n);
fp16_t fp16_int(int8_t n);
fp16_t fp16_float(float n);
fp16_t fp16_double(double n);
uint8_t uint_fp16(fp16_t n);
int8_t int_fp16(fp16_t n);
float float_fp16(fp16_t n);
double double_fp16(fp16_t n);
fp16_t fp16_mul(fp16_t a, fp16_t b);
fp16_t fp16_div(fp16_t a, fp16_t b);
fp16_t fp16_floor(fp16_t n);
fp16_t fp16_ceil(fp16_t n);
fp16_t fp16_round(fp16_t n);

fp32_t fp32_fp16(fp16_t n);
fp32_t fp32_uint(uint16_t n);
fp32_t fp32_int(int16_t n);
fp32_t fp32_float(float n);
fp32_t fp32_double(double n);
uint16_t uint_fp32(fp32_t n);
int16_t int_fp32(fp32_t n);
float float_fp32(fp32_t n);
double double_fp32(fp32_t n);
fp32_t fp32_mul(fp32_t a, fp32_t b);
fp32_t fp32_div(fp32_t a, fp32_t b);
fp32_t fp32_floor(fp32_t n);
fp32_t fp32_ceil(fp32_t n);
fp32_t fp32_round(fp32_t n);

vec2i_t vec2i_new(int16_t x, int16_t y);
vec2i_t vec2i_add(const vec2i_t *a, const vec2i_t *b);
vec2i_t vec2i_sub(const vec2i_t *a, const vec2i_t *b);
vec2i_t vec2i_mul(const vec2i_t *a, const vec2i_t *b);
vec2i_t vec2i_scale(const vec2i_t *a, uint16_t scale);
uint16_t vec2i_dot(const vec2i_t *a, const vec2i_t *b);
vec2i_t vec2i_vec2fp16(const vec2fp16_t *a);
vec2i_t vec2i_vec2fp32(const vec2fp32_t *a);

vec2fp16_t vec2fp16_new(fp16_t x, fp16_t y);
vec2fp16_t vec2fp16_add(const vec2fp16_t *a, const vec2fp16_t *b);
vec2fp16_t vec2fp16_sub(const vec2fp16_t *a, const vec2fp16_t *b);
vec2fp16_t vec2fp16_mul(const vec2fp16_t *a, const vec2fp16_t *b);
vec2fp16_t vec2fp16_scale(const vec2fp16_t *a, fp16_t scale);
fp16_t vec2fp16_dot(const vec2fp16_t *a, const vec2fp16_t *b);
vec2fp16_t vec2fp16_vec2i(const vec2i_t *a);
vec2fp16_t vec2fp16_vec2fp32(const vec2fp32_t *a);

vec2fp32_t vec2fp32_new(fp32_t x, fp32_t y);
vec2fp32_t vec2fp32_add(const vec2fp32_t *a, const vec2fp32_t *b);
vec2fp32_t vec2fp32_sub(const vec2fp32_t *a, const vec2fp32_t *b);
vec2fp32_t vec2fp32_mul(const vec2fp32_t *a, const vec2fp32_t *b);
vec2fp32_t vec2fp32_scale(const vec2fp32_t *a, fp32_t scale);
fp32_t vec2fp32_dot(const vec2fp32_t *a, const vec2fp32_t *b);
vec2fp32_t vec2fp32_vec2i(const vec2i_t *a);
vec2fp32_t vec2fp32_vec2fp16(const vec2fp16_t *a);

fp32_t lu_sin(uint16_t theta);
fp32_t lu_cos(uint16_t theta);

#endif // GUARD_AGB_UTILITY_MATH_H