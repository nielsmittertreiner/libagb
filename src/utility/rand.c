#include "agb.h"

static EWRAM_DATA uint32_t s[4] = {0, 0, 0, 1};

static inline uint32_t rotl(const uint32_t x, uint32_t k)
{
	return (x << k) | (x >> (32 - k));
}

inline uint32_t rand_seed(uint32_t seed)
{
    s[0] = rotl(seed, seed & 0x1F);
    return s[0];
}

uint32_t rand_next(void)
{
    const uint32_t r = rotl(s[0] + s[3], 7) + s[0];
    const uint32_t t = s[0] << 9;

    s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];
	s[2] ^= t;
    s[3] = rotl(s[3], 11);

    return r;
}