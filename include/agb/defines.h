#ifndef GUARD_AGB_AGB_DEFINES_H
#define GUARD_AGB_AGB_DEFINES_H

#include "agb.h"

#define TRUE  1
#define FALSE 0

#define BIT(n)              (1 << (n))
#define BITS(n)             (BIT(n) - 1)

#define ARM_CODE            __attribute__((target("arm")))
#define THUMB_CODE          __attribute__((target("thumb")))
#define IWRAM_CODE          __attribute__((section(".iwram"), long_call))
#define EWRAM_CODE          __attribute__((section(".ewram"), long_call))
#define IWRAM_DATA          __attribute__((section(".iwram")))
#define EWRAM_DATA          __attribute__((section(".ewram")))
#define EWRAM_BSS           __attribute__((section(".sbss")))
#define ALIGNED(n)          __attribute__((aligned (n)))
#define PACKED              __attribute__((packed))

#define IRQ_CHECK           (*(uint16_t *)0x3007FF8)
#define IRQ_VECTOR          (*(void **)0x3007FFC)

// External Working RAM (256K)
#define EWRAM_START         ((void *)0x02000000)
#define EWRAM_SIZE          ((size_t)0x40000)

// Internal Working RAM (32K)
#define IWRAM_START         ((void *)0x03000000)
#define IWRAM_SIZE          ((size_t)0x8000)
#define IWRAM_END           ((void *)IWRAM_START + IWRAM_SIZE)

// Video Palette (1K)
#define PLTT                ((volatile uint16_t *)0x5000000)
#define PLTT_SIZE           ((size_t)0x400)

// Video RAM (96K)
#define VRAM                ((void *)0x6000000)
#define VRAM_SIZE           ((size_t)0x18000)

#define BG_VRAM             VRAM
#define BG_VRAM_SIZE        ((size_t)0x10000)
#define BG_CHAR_SIZE        ((size_t)0x4000)
#define BG_SCREEN_SIZE      ((size_t)0x800)
#define BG_CHAR_ADDR(n)     (BG_VRAM + (BG_CHAR_SIZE * (n)))
#define BG_SCREEN_ADDR(n)   (BG_VRAM + (BG_SCREEN_SIZE * (n)))

#define BG_TILE_H_FLIP(n)   (0x400 + (n))
#define BG_TILE_V_FLIP(n)   (0x800 + (n))

#define NUM_BACKGROUNDS     4

// text-mode BG
#define OBJ_VRAM0           (VRAM + 0x10000)
#define OBJ_VRAM0_SIZE      ((size_t)0x8000)

// bitmap-mode BG
#define OBJ_VRAM1           (VRAM + 0x14000)
#define OBJ_VRAM1_SIZE      ((size_t)0x4000)

#define OAM                 ((void *)0x7000000)
#define OAM_SIZE            ((size_t)0x400)

#define ROM_HEADER_SIZE     ((size_t)0xC0)

#define DISPLAY_WIDTH       ((size_t)240)
#define DISPLAY_HEIGHT      ((size_t)160)

#define TILE_SIZE_4BPP      ((size_t)32)
#define TILE_SIZE_8BPP      ((size_t)64)

#define TILE_OFFSET_4BPP(n) ((n) * TILE_SIZE_4BPP)
#define TILE_OFFSET_8BPP(n) ((n) * TILE_SIZE_8BPP)

#define OBJ_TILE_COUNT      ((size_t)1024)

#endif // GUARD_AGB_AGB_DEFINES_H
