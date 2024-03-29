#ifndef COLORS_H
#define COLORS_H

#include <genesis.h>

#define NEAR_DIST 50
#define FIX_0_16_INV_NEAR_DIST (65536/NEAR_DIST)
#define MID_DIST 150
#define FIX_0_16_INV_MID_DIST (65536/MID_DIST)
#define MID_DARK_DIST 250
#define FIX_0_16_INV_MID_DARK_DIST (65536/MID_DARK_DIST)
#define DARK_DIST 350
#define FIX_0_16_INV_DARK_DIST (65536/DARK_DIST)
#define FADE_DIST 400
#define FIX_0_16_INV_FADE_DIST (65536/FADE_DIST)

extern u8 dither_enabled;

#define TRANSPARENT_IDX  0x0
#define LIGHT_YELLOW_IDX 0x1
#define LIGHT_BLUE_IDX   0x2
#define LIGHT_GREEN_IDX  0x3 
#define LIGHT_RED_IDX    0x4
#define LIGHT_PURPLE_IDX 0x5
#define LIGHT_STEEL_IDX  0x6
#define YELLOW_IDX       0x7
#define BLUE_IDX         0x8
#define GREEN_IDX        0x9
#define RED_IDX          0xA
#define PURPLE_IDX       0xB
#define STEEL_IDX        0xC
#define DARK_YELLOW_IDX  0xD
#define DARK_BLUE_IDX    0xE
#define BLACK_IDX        0xF


//u32 get_dark_color(u8 col_idx, s8 light_level);
//u32 get_mid_dark_color(u8 col_idx, s8 light_level);
//u32 get_light_color(u8 col_idx, s8 light_level);

//extern const u32 color_calc_table[16*3*2];
//extern const u32 *color_calc_table; //[16*5*2];

extern const u32 long_color_table[16];

u32 get_dark_color(u8 col_idx, s8 light_level);
u32 get_light_color(u8 col_idx, s8 light_level);

void init_color_table();
void toggle_dither_mode();

#endif