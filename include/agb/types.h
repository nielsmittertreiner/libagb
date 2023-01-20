#ifndef GUARD_AGB_AGB_TYPES_H
#define GUARD_AGB_AGB_TYPES_H

typedef int16_t fp16_t;
typedef int32_t fp32_t;

typedef struct vec2i_t
{
    int16_t x;
    int16_t y;
} vec2i_t;

typedef struct vec2fp16_t
{
    fp16_t x;
    fp16_t y;
} vec2fp16_t;

typedef struct vec2fp32_t
{
    fp32_t x;
    fp32_t y;
} vec2fp32_t;

// Display Control Structure
typedef struct display_control
{
    uint16_t mode:3;                 // BG Mode Select
    uint16_t cgb_mode:1;             // CGB Mode Select
    uint16_t bitmap_frame:1;         // Bitmap Mode Display Frame Select
    uint16_t obj_hblank_interval:1;  // OBJ Processing in H Blank OFF
    uint16_t obj_char_mapping:1;     // OBJ Character Data Mapping Type
    uint16_t forced_blank:1;         // LCDC OFF
    uint16_t bg0:1;                  // BG 0 ON
    uint16_t bg1:1;                  // BG 1 ON
    uint16_t bg2:1;                  // BG 2 ON
    uint16_t bg3:1;                  // BG 3 ON
    uint16_t obj:1;                  // OBJ ON
    uint16_t win0:1;                 // Window 0 ON
    uint16_t win1:1;                 // Window 1 ON
    uint16_t obj_win:1;              // OBJ Window ON
} display_control;

// Display Status Control Structure
typedef struct display_status
{
    uint16_t vblank:1;              // During V-Blank
    uint16_t hblank:1;              // During H-Blank
    uint16_t vcount:1;              // V-Counter Match
    uint16_t vblank_interrupt:1;    // V-Blank Interrupt
    uint16_t hblank_interrupt:1;    // H-Blank Interrupt
    uint16_t vcount_interrupt:1;    // V-Counter Match Interrupt
    uint16_t unused_6_7:2;
    uint16_t vcount_setting:8;      // V Counter Comparison Value
} display_status;

// Mosaic Control Structure
typedef struct mosaic_control
{
    uint16_t bg_horizontal:4;   // BG Mosaic Horizontal Size
    uint16_t bg_vertical:4;     // BG Mosaic Vertical Size
    uint16_t obj_horizontal:4;  // OBJ Mosaic Horizontal Size
    uint16_t obj_vertical:4;    // OBJ Mosaic Vertical Size
} mosaic_control;

// Blend Target Control Structure
typedef struct blend_target_control
{
    uint16_t target_1_bg0:1;    // 1st Pixel
    uint16_t target_1_bg1:1;
    uint16_t target_1_bg2:1;
    uint16_t target_1_bg3:1;
    uint16_t target_1_obj:1;
    uint16_t target_1_bd:1;
    uint16_t mode:2;            // Blend Mode
    uint16_t target_2_bg0:1;    // 2nd Pixel
    uint16_t target_2_bg1:1;
    uint16_t target_2_bg2:1;
    uint16_t target_2_bg3:1;
    uint16_t target_2_obj:1;
    uint16_t target_2_bd:1;
} blend_target_control;

// Blend Coeffs Control Structure
typedef struct blend_coeffs_control
{
    uint16_t eva:5;             // Parameter A
    uint16_t unused_21_23:3;
    uint16_t evb:5;             // Parameter B
    uint16_t unused_29_31:3;
} blend_coeffs_control;

// Multi-player SIO Control Structure
typedef struct sio_multiplayer_control
{
    uint16_t baud_rate:2;           // baud rate
    uint16_t si:1;                  // SI terminal
    uint16_t sd:1;                  // SD terminal
    uint16_t id:2;                  // ID
    uint16_t error:1;               // error flag
    uint16_t enable:1;              // SIO enable
    uint16_t unused_8_11:4;
    uint16_t mode:2;                // communication mode (should equal 2)
    uint16_t interrupt_enable:1;    // IRQ enable
    uint16_t unused_15:1;
    uint16_t data;                  // data
} sio_multiplayer_control;

// DMA Register Control Structure
typedef struct dma_control
{
	const void *src;
	void *dst;
	uint16_t cnt_lo;    // transfer count
	uint16_t cnt_hi;    // flags
} dma_control;


#endif // GUARD_AGB_AGB_TYPES_H
