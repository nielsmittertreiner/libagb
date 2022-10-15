#ifndef GUARD_AGB_UTILITY_RANDOM_H
#define GUARD_AGB_UTILITY_RANDOM_H

#include "agb.h"

#define N 624         // length of state vector
#define M 397         // a period parameter
#define K 0x9908B0DFU // a magic constant

void SeedRNG(u32 seed);
u32 ReloadRNG(void);
u32 Random(void);

#endif // GUARD_AGB_UTILITY_RANDOM_H
