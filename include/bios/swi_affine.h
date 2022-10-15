#ifndef GUARD_AGB_BIOS_AFFINE_H
#define GUARD_AGB_BIOS_AFFINE_H

#include "agb.h"

// BG Affine Transformation Source Data Structure
typedef struct bg_affine_src_data
{
    int32_t src_center_x;       // Source Data Center Coordinate  << 8
    int32_t src_center_y;
    int16_t display_center_x;   // Display Center Coordinate
    int16_t display_center_y;
    int16_t ratio_x;             // Scaling Ratio << 8
    int16_t ratio_y;
    uint16_t theta;              // Rotation Angle (0 - 255) << 8
} bg_affine_src_data;

// BG Affine Transformation Destination Data Structure
typedef struct bg_affine_dst_data
{
    int16_t horizontal_diff_x;  // Line Direction X Coordinate Difference
    int16_t vertical_diff_x;    // Vertical Direction X Coordinate Difference
    int16_t horizontal_diff_y;  // Line Direction Y Coordinate Difference
    int16_t vertical_diff_y;    // Vertical Direction Y Coordinate Difference
    int32_t start_x;            // Start X Coordinate
    int32_t start_y;            // Start Y Coordinate
} bg_affine_dst_data;

// OBJ Affine Transformation Source Data Structure
typedef struct obj_affine_src_data
{
    int16_t ratio_x;             // Scaling Ratio
    int16_t ratio_y;
    uint16_t theta;              // Rotation Angle (0 - 255) << 8
} obj_affine_src_data;

// OBJ Affine Transformation Destination Data Structure
typedef struct obj_affine_dst_data
{
    int16_t horizontal_diff_x;  // Line Direction X Coordinate Difference
    int16_t vertical_diff_x;    // Vertical Direction X Coordinate Difference
    int16_t horizontal_diff_y;  // Line Direction Y Coordinate Difference
    int16_t vertical_diff_y;    // Vertical Direction Y Coordinate Difference
} obj_affine_dst_data;

extern void bg_affine_set(bg_affine_src_data *src, bg_affine_dst_data *dst, int32_t count);
extern void obj_affine_set(obj_affine_src_data *src, void *dst, int32_t count, int32_t offset);

#endif // GUARD_AGB_BIOS_AFFINE_H