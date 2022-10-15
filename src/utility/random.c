#include "agb.h"

#define HIBIT(n)       ((n) & 0x80000000U)    // mask all but highest bit of n
#define LOBIT(n)       ((n) & 0x00000001U)    // mask all but lowest bit of n
#define LOBITS(n)      ((n) & 0x7FFFFFFFU)    // mask the highest bit of n
#define MIXBITS(x, y)  (HIBIT(x) | LOBITS(y)) // move hi bit of x to hi bit of y

// .bss
static EWRAM_DATA u32 sStateVector[N];
static EWRAM_DATA u32 *sMersenneIndex;
static EWRAM_DATA s32 sMersenneLeft;
static EWRAM_DATA u8 sReloadRNG;

// .text
void SeedRNG(u32 seed)
{
    u32 x = (seed | 0x1 ) & 0xFFFFFFFF;
    u32 *state = sStateVector;
    u32 j = N;

    sMersenneLeft = 0;
    *state++ = x;

    while (--j)
    {
        x *= 69069;
        *state = ( x & 0xFFFFFFFF );
        state++;
    }

    sReloadRNG = TRUE;
}

u32 ReloadRNG(void)
{
   u32 *p0 = sStateVector;
   u32 *p2 = sStateVector + 2;
   u32 *pM = sStateVector + M;
   u32 s0;
   u32 s1;
   u32 j;

    // if this is the first time through seed the algorithm.
    if (sMersenneLeft < -1)
        SeedRNG(5489);

    sMersenneLeft = N - 1;
    sMersenneIndex = &(sStateVector[1]);

    for (s0 = sStateVector[0], s1 = sStateVector[1], j = (N - M + 1); --j; s0 = s1, s1 = *p2++)
    {
        *p0++ = (*pM++ ^ (MIXBITS(s0, s1) >> 1) ^ (LOBIT(s1) ? K : 0));
    }

    for (pM = sStateVector, j = M; --j; s0 = s1, s1 = *p2++)
    {
        *p0++ = (*pM++ ^ (MIXBITS(s0, s1) >> 1 ) ^ (LOBIT(s1) ? K : 0));
    }

    s1 = sStateVector[0];
    *p0 = (*pM ^ (MIXBITS(s0, s1) >> 1) ^ (LOBIT(s1) ? K : 0));
    s1 ^= (s1 >> 11);
    s1 ^= (s1 << 7) & 0x9D2C5680U;
    s1 ^= (s1 << 15) & 0xEFC60000U;
    return (s1 ^ (s1 >> 18));
}

u32 Random(void)
{
   u32 y;

   sMersenneLeft--;

    if (sMersenneLeft < 0) 
    {
        if (sReloadRNG == TRUE)
        {
            return ReloadRNG();
        }
        else
        {
            sMersenneLeft = N - 1;
            sMersenneIndex = &(sStateVector[1]);
        }
    }

    y  = *sMersenneIndex;
    sMersenneIndex++;
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9D2C5680;
    y ^= (y << 15) & 0xEFC60000;
    y ^= (y >> 18);
    return y;
}
