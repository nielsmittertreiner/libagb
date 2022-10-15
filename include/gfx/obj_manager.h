#ifndef GUARD_AGB_GFX_SPRITE_H
#define GUARD_AGB_GFX_SPRITE_H

#include "agb.h"

#define MAX_OAM_ENTRIES 128
#define MAX_OAM_MATRICES 32

typedef enum sprite_object_mode
{
    SPRITE_OBJECT_NORMAL,
    SPRITE_OBJECT_BLEND,
    SPRITE_OBJECT_WINDOW,
} sprite_object_mode;

typedef enum sprite_affine_mode
{
    SPRITE_AFFINE_OFF,
    SPRITE_AFFINE_NORMAL,
    SPRITE_AFFINE_ERASE,
    SPRITE_AFFINE_DOUBLE,
} sprite_affine_mode;

typedef enum sprite_size
{
    SPRITE_SIZE_SMALL,
    SPRITE_SIZE_NORNAL,
    SPRITE_SIZE_BIG,
    SPRITE_SIZE_HUGE,
} sprite_size;

typedef enum sprite_shape
{
    SPRITE_SHAPE_SQUARE,
    SPRITE_SHAPE_WIDE,
    SPRITE_SHAPE_TALL,
} sprite_shape;

// OAM Structure
typedef struct oam_data
{
    uint32_t y:8;           // Y Coordinate
    uint32_t affine_mode:2; // Affine Mode
    uint32_t object_mode:2; // OBJ Mode
    uint32_t mosaic:1;      // Mosaic
    uint32_t bpp:1;         // 16 colors/256 colors Select
    uint32_t shape:2;       // OBJ Shape
    uint32_t x:9;           // X Coordinate
    uint32_t matrix_num:5;  // Bits 4/5 are H-Flip, V-Flip
    uint32_t size:2;        // OBJ Size
    uint16_t tile_num:10;   // Character No.
    uint16_t priority:2;    // Display priority
    uint16_t palette_num:4; // Palette No.
    uint16_t affine_param;  // Affine Trasnformation Parameter
} oam_data;

typedef struct oam_matrix
{
    int16_t a;
    int16_t b;
    int16_t c;
    int16_t d;
} oam_matrix;

typedef struct sprite_object sprite_object;
typedef sprite_object *sprite_ptr;
typedef struct sprite_object
{
    vec2fp pos;
    sprite_affine_mode affine_mode:2;
    sprite_object_mode object_mode:2;
    uint32_t mosaic:1;
    bpp_mode bpp:1;
    sprite_shape shape:2;
    sprite_size size:2;
    uint32_t matrix_num:5;
    uint32_t tile_num:10;
    uint32_t priority:2;
    uint32_t palette_num:4;
    uint32_t affine_param;
    uint16_t sub_priority:5;
    uint16_t on_screen:1;
    uint16_t active:1;
    uint16_t centered:1;
    uint16_t v_flip:1;
    uint16_t h_flip:1;
    void (*func)(sprite_ptr);
} sprite_object;

typedef struct sprite_object_template
{
    const sprite_tiles *tiles;
    const sprite_palette *palette;
    sprite_affine_mode affine_mode:2;
    sprite_object_mode object_mode:2;
    sprite_shape shape:2;
    sprite_size size:2;
    uint8_t mosaic:1;
    uint8_t priority:2;
    uint8_t sub_priority:5;
    void (*func)(sprite_ptr);
} sprite_object_template;

extern oam_matrix g_oam_matrix_buffer[MAX_OAM_MATRICES];

sprite_ptr sprite_object_create(const sprite_object_template *template, vec2i pos);
void sprite_object_destroy(sprite_ptr sprite_ptr);
void sprite_objects_commit(void);

#endif // GUARD_AGB_GFX_SPRITE_H