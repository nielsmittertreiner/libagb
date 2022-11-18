#ifndef GUARD_AGB_HARDWARE_MEMORY_H
#define GUARD_AGB_HARDWARE_MEMORY_H

#include "agb.h"

void cpu_copy_16(const void *src, void *dst, size_t size);
void cpu_copy_32(const void *src, void *dst, size_t size);
void cpu_fill_16(uint16_t value, void *dst, size_t size);
void cpu_fill_32(uint32_t value, void *dst, size_t size);
void cpu_fast_copy(const void *src, void *dst, size_t size);
void cpu_fast_fill(uint32_t value, void *dst, size_t size);
void dma_enable(uint32_t dma);
void dma_disable(uint32_t dma);
void dma_irq_enable(uint32_t dma);
void dma_irq_disable(uint32_t dma);
void dma_drq_enable(uint32_t dma);
void dma_drq_disable(uint32_t dma);
void dma_set(uint32_t dma, const void *src, void *dst, uint16_t count, uint16_t flags);
void dma_copy_16(const void *src, void *dst, size_t size);
void dma_copy_32(const void *src, void *dst, size_t size);
void dma_vblank_copy_16(const void *src, void *dst, size_t size);
void dma_vblank_copy_32(const void *src, void *dst, size_t size);
void dma_hblank_copy_16(const void *src, void *dst, size_t size);
void dma_hblank_copy_32(const void *src, void *dst, size_t size);
void dma_fifo_a(const void *src);
void dma_fifo_b(const void *src);

#endif // GUARD_AGB_HARDWARE_MEMORY_H