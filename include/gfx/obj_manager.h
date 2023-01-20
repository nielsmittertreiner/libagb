#ifndef GUARD_AGB_GFX_OBJ_H
#define GUARD_AGB_GFX_OBJ_H

#include "agb.h"

#define MAX_OAM_ENTRIES 128
#define MAX_OAM_MATRICES 32

typedef enum sprite_object_mode
{
    SPRITE_OBJECT_NORMAL,
    SPRITE_OBJECT_BLEND,
    SPRITE_OBJECT_WINDOW,
} sprite_object_mode;

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

// OAM Data Structure
typedef struct oam_data
{
    uint32_t y:8;           // Y Coordinate
    uint32_t affine:1;      // Affine
    uint32_t double_size:1; // Double Size
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
} oam_data;

// OAM Matrix Structure
typedef struct oam_matrix
{
    int16_t a;
    int16_t b;
    int16_t c;
    int16_t d;
} oam_matrix;

// Sprite Object Structure
typedef struct sprite_object sprite_object;
typedef sprite_object *sprite_ptr;
typedef struct sprite_object
{
    vec2fp32_t pos;
    uint32_t affine:1;
    uint32_t double_size:1;
    uint32_t object_mode:2;
    uint32_t shape:2;
    uint32_t size:2;
    uint32_t bpp:1;
    uint32_t mosaic:1;
    uint32_t matrix_num:5;
    uint32_t tile_num:10;
    uint32_t priority:2;
    uint32_t palette_num:4;
    uint16_t sub_priority:5;
    uint16_t centered:1;
    uint16_t v_flip:1;
    uint16_t h_flip:1;
    void (*func)(sprite_ptr);
} sprite_object;

// Sprite Object Tiles Structure
typedef struct sprite_object_tiles
{
    const void *data;
    compression_type compression;
    sprite_shape sprite_shape;
    sprite_size sprite_size;
    bpp_mode bpp;
    size_t size;
} sprite_object_tiles;

// Sprite Object Palette Structure
typedef struct sprite_object_palette
{
    const void *data;
    size_t size;
    compression_type compression;
    bpp_mode bpp;
} sprite_object_palette;

// Sprite Object Template Structure
typedef struct sprite_object_template
{
    const sprite_object_tiles *tiles;
    const sprite_object_palette *palette;
    uint16_t affine:1;
    uint16_t double_size:1;
    uint16_t object_mode:2;
    uint16_t mosaic:1;
    uint16_t priority:2;
    uint16_t sub_priority:5;
    void (*func)(sprite_ptr);
} sprite_object_template;

extern oam_matrix g_oam_matrix_buffer[MAX_OAM_MATRICES];

sprite_ptr sprite_object_create(const sprite_object_template *template, vec2i_t pos);
void sprite_object_destroy(sprite_ptr sprite);
void sprite_objects_update(void);
void sprite_objects_sort(void);
void sprite_objects_commit(void);
const vec2i_t *sprite_object_dimensions(sprite_shape shape, sprite_size size);
uint32_t sprite_object_bottom_position(sprite_ptr sprite);
vec2fp32_t sprite_object_pos_correct(vec2fp32_t *pos);

#endif // GUARD_AGB_GFX_OBJ_H