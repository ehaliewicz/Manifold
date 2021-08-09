#ifndef COLORS_H
#define COLORS_H

#include <genesis.h>

#define MID_DIST 150
#define FIX_0_16_INV_MID_DIST (65536/MID_DIST)
#define MID_DARK_DIST 250
#define FIX_0_16_INV_MID_DARK_DIST (65536/MID_DARK_DIST)
#define DARK_DIST 350
#define FIX_0_16_INV_DARK_DIST (65536/DARK_DIST)

/*
#define WHITE       RGB24_TO_VDPCOLOR(0xFFFFFF)
#define BLACK       RGB24_TO_VDPCOLOR(0x000000)
#define LIGHT_RED   RGB24_TO_VDPCOLOR(0xFF0000)
#define RED     RGB24_TO_VDPCOLOR(0x990000)
#define DARK_RED    RGB24_TO_VDPCOLOR(0x440000)
#define LIGHT_GREEN RGB24_TO_VDPCOLOR(0x00FF00)
#define GREEN   RGB24_TO_VDPCOLOR(0x009900)
#define DARK_GREEN  RGB24_TO_VDPCOLOR(0x004400)
#define LIGHT_BLUE  RGB24_TO_VDPCOLOR(0x0000FF)
#define BLUE    RGB24_TO_VDPCOLOR(0x000099)
#define DARK_BLUE   RGB24_TO_VDPCOLOR(0x000044)
#define BROWN   RGB24_TO_VDPCOLOR(0xA0522D)
#define DARK_BROWN   RGB24_TO_VDPCOLOR(0x654321)
*/

#define TRANSPARENT_IDX 0x0
#define LIGHT_GREEN_IDX 0x1 
#define LIGHT_RED_IDX   0x2
#define LIGHT_BLUE_IDX  0x3
#define LIGHT_BROWN_IDX 0x4
#define LIGHT_STEEL_IDX 0x5
#define GREEN_IDX       0x6
#define RED_IDX         0x7
#define BLUE_IDX        0x8
#define BROWN_IDX       0x9
#define STEEL_IDX       0xA
#define DARK_GREEN_IDX  0xB
#define DARK_RED_IDX    0xC
#define DARK_BLUE_IDX   0xD
#define DARK_BROWN_IDX  0xE
#define DARK_STEEL_IDX  0xF

/*
#define LIGHT_RED_IDX 0x11
#define RED_IDX 0x22
#define DARK_RED_IDX 0x33
#define LIGHT_GREEN_IDX 0x44
#define GREEN_IDX 0x55
#define DARK_GREEN_IDX 0x66
#define LIGHT_BLUE_IDX 0x77
#define BLUE_IDX 0x88
#define DARK_BLUE_IDX 0x99
#define BLUE_BLACK_IDX 0x9F
#define BROWN_IDX 0xAA
#define DARK_BROWN_IDX 0xBB
#define BLACK_IDX 0xFF
*/

//extern const u16 threeDPalette[16];

u8 calculate_color(u8 col_idx, u32 dist, s8 light_level);

u8 get_dark_color(u8 col_idx, s8 light_level);
u8 get_mid_dark_color(u8 col_idx, s8 light_level);
u8 get_light_color(u8 col_idx, s8 light_level);

u8 needs_dither(u32 dist);

#endif