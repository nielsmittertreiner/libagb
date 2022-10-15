#ifndef GUARD_AGB_BIOS_CPUSET_H
#define GUARD_AGB_BIOS_CPUSET_H

#include "agb.h"

typedef enum
{
    CPU_SET_SRC_FIXED   = BIT(24),
    CPU_SET_32BIT       = BIT(26),
    CPU_SET_16BIT       = !CPU_SET_32BIT,
} cpu_set_mode;

typedef enum
{
    CPU_FAST_SET_SRC_FIXED = BIT(24),
} cpu_fast_set_mode;

extern void cpu_set(const void *src, void *dst, uint32_t mode);
extern void cpu_fast_set(const void *src, void *dst, uint32_t mode);

#endif // GUARD_AGB_BIOS_CPUSET_H