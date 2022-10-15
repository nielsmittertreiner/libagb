#include "agb.h"

void cpu_copy_16(const void *src, void *dst, size_t size)
{
    cpu_set(src, dst, CPU_SET_16BIT | (size / 2 & 0x1FFFFF));
}

void cpu_copy_32(const void *src, void *dst, size_t size)
{
    cpu_set(src, dst, CPU_SET_32BIT | (size / 4 & 0x1FFFFF));
}

void cpu_fill_16(uint16_t value, void *dst, size_t size)
{
    volatile uint16_t val = value;
    cpu_set((void *)&val, dst, CPU_SET_16BIT | CPU_SET_SRC_FIXED | ((size / 2) & 0x1FFFFF));
}

void cpu_fill_32(uint32_t value, void *dst, size_t size)
{
    volatile uint32_t val = value;
    cpu_set((void *)&val, dst, CPU_SET_32BIT | CPU_SET_SRC_FIXED | ((size / 4) & 0x1FFFFF));
}

void cpu_fast_copy(const void *src, void *dst, size_t size)
{
    cpu_fast_set(src, dst, ((size / 4) & 0x1FFFFF));
}

void cpu_fast_fill(uint32_t value, void *dst, size_t size)
{
    volatile uint32_t val = value;
    cpu_fast_set((void *)&val, dst, CPU_FAST_SET_SRC_FIXED | ((size / 4) & 0x1FFFFF));
}
