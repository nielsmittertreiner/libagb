#ifndef GUARD_AGB_GFX_BG_H
#define GUARD_AGB_GFX_BG_H

#include "agb.h"

typedef enum
{
	BG_MODE_0,
	BG_MODE_1,
	BG_MODE_2,
	BG_MODE_3,
	BG_MODE_4,
	BG_MODE_5,
} bg_mode;

typedef enum
{
	BG_PALETTE_MODE_4BPP,
	BG_PALETTE_MODE_8BPP,
} bg_palette_mode;

typedef enum
{
	BG_SCREEN_SIZE_256x256,
	BG_SCREEN_SIZE_512x256,
	BG_SCREEN_SIZE_256x512,
	BG_SCREEN_SIZE_512x512,
} bg_screen_size;

typedef enum
{
	BG_TYPE_TEXT,
	BG_TYPE_AFFINE,
} bg_type;

// Background Control Structure
typedef struct background_control
{
    uint16_t priority:2;    // Display Priority
    uint16_t char_base:2;   // Character Base Address
    uint16_t unused_4_5:2;
    uint16_t mosaic:1;      // Mosaic
    uint16_t bpp:1;         // 16 colors/ 256 colors Select
    uint16_t screen_base:5; // Screen Base Address
    uint16_t wraparound:1;  // Wraparound
    uint16_t size:2;        // Screen Size
} background_control;

typedef struct background_template
{
    uint32_t bg:2;
    uint32_t char_base:2;
    uint32_t screen_base:5;
    uint32_t size:2;
    uint32_t bpp:1;
    uint32_t priority:2;
    uint32_t tile_base:10;
} background_template;

#endif // GUARD_AGB_GFX_BG_H