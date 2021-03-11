#include <genesis.h>
#include "colors.h"

/*
const u16 threeDPalette[16] = {
    WHITE,
    LIGHT_RED,
    RED,
    DARK_RED,
    LIGHT_GREEN,
    GREEN,
    DARK_GREEN,
    LIGHT_BLUE,
    BLUE,
    DARK_BLUE,
    DARK_BROWN,
    BROWN,
    BLACK,
    BLACK,
    BLACK,
    BLACK
};
*/

/*
const u16 threeDPalette[16] = {
  RGB24_TO_VDPCOLOR(0x000000),   // 0 transparent
  RGB24_TO_VDPCOLOR(0x216a14),   // 1 green
  RGB24_TO_VDPCOLOR(0xe9d805),   // 2 yellow
  RGB24_TO_VDPCOLOR(0xa00018),   // 3 red
  RGB24_TO_VDPCOLOR(0x5d0f13),   // 4 dark red
  RGB24_TO_VDPCOLOR(0x726a8a),   // 5 steel
  RGB24_TO_VDPCOLOR(0x1a0067),   // 6 blue
  RGB24_TO_VDPCOLOR(0x986300),   // 7 orange
  RGB24_TO_VDPCOLOR(0x7f4d2a),   // 8 brown
  RGB24_TO_VDPCOLOR(0x552605),   // 9 dark brown
  RGB24_TO_VDPCOLOR(0x424247),   // A dark steel
  RGB24_TO_VDPCOLOR(0x0d0051),   // B dark blue
  RGB24_TO_VDPCOLOR(0x746C02),   // C dark yellow
  RGB24_TO_VDPCOLOR(0x173910),   // D dark green
  RGB24_TO_VDPCOLOR(0x000000),   // E black
  RGB24_TO_VDPCOLOR(0xFFFFFF)
};
*/

const u8 dark_col_lut[16] = {
  0, GREEN_IDX,      RED_IDX,      BLUE_IDX,      BROWN_IDX,      STEEL_IDX,
     DARK_GREEN_IDX, DARK_RED_IDX, DARK_BLUE_IDX, DARK_BROWN_IDX, DARK_STEEL_IDX,
     DARK_GREEN_IDX, DARK_RED_IDX, DARK_BLUE_IDX, DARK_BROWN_IDX, DARK_STEEL_IDX
};

const u8 light_col_lut[16] = {
  0, LIGHT_GREEN_IDX, LIGHT_RED_IDX, LIGHT_BLUE_IDX, LIGHT_BROWN_IDX, LIGHT_STEEL_IDX,
     LIGHT_GREEN_IDX, LIGHT_RED_IDX, LIGHT_BLUE_IDX, LIGHT_BROWN_IDX, LIGHT_STEEL_IDX,
     GREEN_IDX,      RED_IDX,      BLUE_IDX,      BROWN_IDX,      STEEL_IDX,
};


u8 calculate_color(u8 col_idx, u32 dist, s8 light_level) {
  switch(light_level) {
    case -2:
      col_idx = dark_col_lut[col_idx];
    case -1:
      col_idx = dark_col_lut[col_idx];
      break;
    case 0:
      break;
    case 2:
      col_idx = light_col_lut[col_idx];
    case 1:
      col_idx = light_col_lut[col_idx];
      break;
  }
  if(light_level == 2) {
    return (col_idx << 4) | col_idx;
  }
  if(dist >= DARK_DIST) {
    return (dark_col_lut[col_idx] << 4) | dark_col_lut[col_idx];
  } else if (dist >= MID_DARK_DIST) {
    return (col_idx << 4) | dark_col_lut[col_idx];
  } else { // if (dist >= MID_DIST) {
    return col_idx << 4 | col_idx;
  } //else {

    //return col_idx << 4 | light_col_lut[col_idx];
  //}
}

u8 needs_dither(u32 dist) {
  if(dist >= DARK_DIST) {
    return 0;
  } else if (dist >= MID_DARK_DIST) {
    return 1;
  } else if (dist >= MID_DIST) {
    return 0;
  } else {
    return 1;
  }
}