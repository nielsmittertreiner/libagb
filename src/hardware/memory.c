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

inline void dma_enable(uint32_t dma)
{
    REG_DMA[dma].cnt_hi |= DMA_ENABLE;
}

inline void dma_disable(uint32_t dma)
{
    REG_DMA[dma].cnt_hi &= ~DMA_ENABLE;
}

inline void dma_irq_enable(uint32_t dma)
{
    REG_DMA[dma].cnt_hi |= DMA_IRQ_ENABLE;
}

inline void dma_irq_disable(uint32_t dma)
{
    REG_DMA[dma].cnt_hi &= ~DMA_IRQ_ENABLE;
}

inline void dma_drq_enable(uint32_t dma)
{
    REG_DMA[dma].cnt_hi |= DMA_DRQ_ENABLE;
}

inline void dma_drq_disable(uint32_t dma)
{
    REG_DMA[dma].cnt_hi &= ~DMA_DRQ_ENABLE;
}

inline void dma_set(uint32_t dma, const void *src, void *dst, uint16_t count, uint16_t flags)
{
    REG_DMA[dma].src = src;
    REG_DMA[dma].dst = dst;
    REG_DMA[dma].cnt_lo = count;
    REG_DMA[dma].cnt_hi = flags;
}

void dma_copy_16(const void *src, void *dst, size_t size)
{
    dma_set(3, src, dst, size / 2, DMA_ENABLE | DMA_16BIT | DMA_START_NOW | DMA_SRC_INC | DMA_DST_INC);
}

void dma_copy_32(const void *src, void *dst, size_t size)
{
    dma_set(3, src, dst, size / 4, DMA_ENABLE | DMA_32BIT | DMA_START_NOW | DMA_SRC_INC | DMA_DST_INC);
}

void dma_vblank_copy_16(const void *src, void *dst, size_t size)
{
    dma_set(3, src, dst, size / 2, DMA_ENABLE | DMA_16BIT | DMA_START_VBLANK | DMA_SRC_INC | DMA_DST_INC);
}

void dma_vblank_copy_32(const void *src, void *dst, size_t size)
{
    dma_set(3, src, dst, size / 4, DMA_ENABLE | DMA_32BIT | DMA_START_VBLANK | DMA_SRC_INC | DMA_DST_INC);
}

void dma_hblank_copy_16(const void *src, void *dst, size_t size)
{
    dma_set(0, src, dst, size / 2, DMA_ENABLE | DMA_16BIT | DMA_START_HBLANK | DMA_REPEAT | DMA_DST_RELOAD | DMA_SRC_INC | DMA_DST_INC);
}

void dma_hblank_copy_32(const void *src, void *dst, size_t size)
{
    dma_set(0, src, dst, size / 4, DMA_ENABLE | DMA_32BIT | DMA_START_HBLANK | DMA_REPEAT | DMA_DST_RELOAD | DMA_SRC_INC | DMA_DST_INC);
}

void dma_fifo_a(const void *src)
{
    dma_set(1, src, (void *)REG_ADDR_FIFO_A, 4, DMA_ENABLE | DMA_32BIT | DMA_START_SPECIAL | DMA_REPEAT | DMA_SRC_INC | DMA_DST_FIXED);
}

void dma_fifo_b(const void *src)
{
    dma_set(2, src, (void *)REG_ADDR_FIFO_B, 4, DMA_ENABLE | DMA_32BIT | DMA_START_SPECIAL | DMA_REPEAT | DMA_SRC_INC | DMA_DST_FIXED);
}
