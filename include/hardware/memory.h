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

#define DmaSet(dmaNum, src, dest, control)        \
{                                                 \
    vu32 *dmaRegs = (vu32 *)REG_ADDR_DMA##dmaNum; \
    dmaRegs[0] = (vu32)(src);                     \
    dmaRegs[1] = (vu32)(dest);                    \
    dmaRegs[2] = (vu32)(control);                 \
    dmaRegs[2];                                   \
}

#define DmaCopy(dmaNum, src, dest, size, bit)                                               \
    DmaSet(dmaNum,                                                                          \
           src,                                                                             \
           dest,                                                                            \
           (DMA_ENABLE | DMA_START_NOW | DMA_##bit##BIT | DMA_SRC_INC | DMA_DST_INC) << 16 \
         | ((size)/(bit/8)))

#define DmaCopy16(dmaNum, src, dest, size) DmaCopy(dmaNum, src, dest, size, 16)
#define DmaCopy32(dmaNum, src, dest, size) DmaCopy(dmaNum, src, dest, size, 32)

#define DmaCopyHBlank(src, dest, size, bit)                                                      \
    DmaSet(0,                                                                                    \
           src,                                                                                  \
           dest,                                                                                 \
           (DMA_ENABLE | DMA_START_HBLANK | DMA_##bit##BIT | DMA_SRC_INC | DMA_DST_INC) << 16 \
         | ((size)/(bit/8)))

#define DmaCopyHBlank16(src, dest, size) DmaCopyHBlank(src, dest, size, 16)
#define DmaCopyHBlank32(src, dest, size) DmaCopyHBlank(src, dest, size, 32)

#define DmaCopyHBlankDestFixed(src, dest, size, bit)                                             \
    DmaSet(0,                                                                                    \
           src,                                                                                  \
           dest,                                                                                 \
           (DMA_ENABLE | DMA_START_HBLANK | DMA_##bit##BIT | DMA_SRC_INC | DMA_DST_FIXED) << 16 \
         | ((size)/(bit/8)))

#define DmaCopyHBlankDestFixed16(src, dest, size) DmaCopyHBlankDestFixed(src, dest, size, 16)
#define DmaCopyHBlankDestFixed32(src, dest, size) DmaCopyHBlankDestFixed(src, dest, size, 32)

#define DmaFill(dmaNum, value, dest, size, bit)                                               \
{                                                                                             \
    vu##bit tmp = (vu##bit)(value);                                                           \
    DmaSet(dmaNum,                                                                            \
           &tmp,                                                                              \
           dest,                                                                              \
           (DMA_ENABLE | DMA_START_NOW | DMA_##bit##BIT | DMA_SRC_FIXED | DMA_DST_INC) << 16 \
         | ((size)/(bit/8)));                                                                 \
}

#define DmaFill16(dmaNum, value, dest, size) DmaFill(dmaNum, value, dest, size, 16)
#define DmaFill32(dmaNum, value, dest, size) DmaFill(dmaNum, value, dest, size, 32)

#define DmaClear(dmaNum, dest, size, bit)   \
{                                           \
    vu##bit *_DST = (vu##bit *)(dest);     \
    u32 _size = size;                       \
    DmaFill##bit(dmaNum, 0, _DST, _size);  \
}

#define DmaClear16(dmaNum, dest, size) DmaClear(dmaNum, dest, size, 16)
#define DmaClear32(dmaNum, dest, size) DmaClear(dmaNum, dest, size, 32)

#define DmaStop(dmaNum)                                         \
{                                                               \
    vu16 *dmaRegs = (vu16 *)REG_ADDR_DMA##dmaNum;               \
    dmaRegs[5] &= ~(DMA_START_MASK | DMA_DRQ_ENABLE  | DMA_REPEAT); \
    dmaRegs[5] &= ~DMA_ENABLE;                                  \
    dmaRegs[5];                                                 \
}                                                               

#define DmaStart(dmaNum)                                        \
{                                                               \
    vu16 *dmaRegs = (vu16 *)REG_ADDR_DMA##dmaNum;               \
    dmaRegs[5] &= ~(DMA_START_MASK | DMA_DRQ_ENABLE  | DMA_REPEAT); \
    dmaRegs[5] &= ~DMA_ENABLE;                                  \
    dmaRegs[5];                                                 \
}                                                               


#endif // GUARD_AGB_HARDWARE_MEMORY_H