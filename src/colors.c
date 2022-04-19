#include <genesis.h>
#include "colors.h"


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


// #define PIX(p) ((p << 4) | p)
// #define DPIX(p,q) ((p << 4) | q)


#define BPIX(p) ((p << 4) | p)

#define BDPIX(p,q) ((p << 4) | q)

#define PIX(p) ((BPIX(p) << 24) | (BPIX(p) << 16) | (BPIX(p) << 8) | BPIX(p))
#define DPIX(p,q) ((BDPIX(p,q) << 24) | (BDPIX(p,q) << 16) | (BDPIX(p,q) << 8) | BDPIX(p,q))


// dark dist is -2 brightness
// mid dist is -1 brightness
// -2 is negative 2, etc

const u32 color_calc_table[16*5*3] = {
//const u8 color_calc_table[16*5*3] = {

//const u8 dark_dist_lut[16*5] = {
  // -2
  0, PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
     PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
     PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
  // -1
  0, PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
     PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
     PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
  // 0
  0, PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
     PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
     PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
  // 1
  0, PIX(GREEN_IDX), PIX(RED_IDX), PIX(BLUE_IDX), PIX(BROWN_IDX), PIX(STEEL_IDX),
     PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
     PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
  // 2
  0, PIX(LIGHT_GREEN_IDX), PIX(LIGHT_RED_IDX), PIX(LIGHT_BLUE_IDX), PIX(LIGHT_BROWN_IDX), PIX(LIGHT_STEEL_IDX),
     PIX(GREEN_IDX), PIX(RED_IDX), PIX(BLUE_IDX), PIX(BROWN_IDX), PIX(STEEL_IDX),
     PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
//};

//const u8 mid_dist_lut[16*5] = {
  // -2
  0, PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
     PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
     PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
  // -1
  0, DPIX(GREEN_IDX,DARK_GREEN_IDX), DPIX(RED_IDX,DARK_RED_IDX), DPIX(BLUE_IDX, DARK_BLUE_IDX), DPIX(BROWN_IDX, DARK_BROWN_IDX), DPIX(STEEL_IDX, DARK_STEEL_IDX),
     PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
     PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
  // 0
  0, DPIX(LIGHT_GREEN_IDX, GREEN_IDX), DPIX(LIGHT_RED_IDX, RED_IDX), DPIX(LIGHT_BLUE_IDX, BLUE_IDX), DPIX(LIGHT_BROWN_IDX, BROWN_IDX), DPIX(LIGHT_STEEL_IDX, STEEL_IDX),
     DPIX(GREEN_IDX,DARK_GREEN_IDX), DPIX(RED_IDX,DARK_RED_IDX), DPIX(BLUE_IDX, DARK_BLUE_IDX), DPIX(BROWN_IDX, DARK_BROWN_IDX), DPIX(STEEL_IDX, DARK_STEEL_IDX),
     PIX(DARK_GREEN_IDX),  PIX(DARK_RED_IDX),  PIX(DARK_BLUE_IDX),  PIX(DARK_BROWN_IDX),  PIX(DARK_STEEL_IDX),
  // 1
  0, DPIX(LIGHT_GREEN_IDX, GREEN_IDX), DPIX(LIGHT_RED_IDX, RED_IDX), DPIX(LIGHT_BLUE_IDX, BLUE_IDX), DPIX(LIGHT_BROWN_IDX, BROWN_IDX), DPIX(LIGHT_STEEL_IDX, STEEL_IDX),
     DPIX(LIGHT_GREEN_IDX, GREEN_IDX), DPIX(LIGHT_RED_IDX, RED_IDX), DPIX(LIGHT_BLUE_IDX, BLUE_IDX), DPIX(LIGHT_BROWN_IDX, BROWN_IDX), DPIX(LIGHT_STEEL_IDX, STEEL_IDX),
     DPIX(GREEN_IDX,DARK_GREEN_IDX), DPIX(RED_IDX,DARK_RED_IDX), DPIX(BLUE_IDX, DARK_BLUE_IDX), DPIX(BROWN_IDX, DARK_BROWN_IDX), DPIX(STEEL_IDX, DARK_STEEL_IDX),
  // 2
  0, DPIX(LIGHT_GREEN_IDX, GREEN_IDX), DPIX(LIGHT_RED_IDX, RED_IDX), DPIX(LIGHT_BLUE_IDX, BLUE_IDX), DPIX(LIGHT_BROWN_IDX, BROWN_IDX), DPIX(LIGHT_STEEL_IDX, STEEL_IDX),
     DPIX(LIGHT_GREEN_IDX, GREEN_IDX), DPIX(LIGHT_RED_IDX, RED_IDX), DPIX(LIGHT_BLUE_IDX, BLUE_IDX), DPIX(LIGHT_BROWN_IDX, BROWN_IDX), DPIX(LIGHT_STEEL_IDX, STEEL_IDX),
     DPIX(LIGHT_GREEN_IDX, GREEN_IDX), DPIX(LIGHT_RED_IDX, RED_IDX), DPIX(LIGHT_BLUE_IDX, BLUE_IDX), DPIX(LIGHT_BROWN_IDX, BROWN_IDX), DPIX(LIGHT_STEEL_IDX, STEEL_IDX),
//};

//const u8 near_dist[16*5] = {
  // -2
  0, PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
     PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
     PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
  // -1
  0, PIX(GREEN_IDX), PIX(RED_IDX), PIX(BLUE_IDX), PIX(BROWN_IDX), PIX(STEEL_IDX),
     PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
     PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
  // 0
  0, PIX(LIGHT_GREEN_IDX), PIX(LIGHT_RED_IDX), PIX(LIGHT_BLUE_IDX), PIX(LIGHT_BROWN_IDX), PIX(LIGHT_STEEL_IDX),
     PIX(GREEN_IDX), PIX(RED_IDX), PIX(BLUE_IDX), PIX(BROWN_IDX), PIX(STEEL_IDX),
     PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
  // 1
  0, PIX(LIGHT_GREEN_IDX), PIX(LIGHT_RED_IDX), PIX(LIGHT_BLUE_IDX), PIX(LIGHT_BROWN_IDX), PIX(LIGHT_STEEL_IDX),
     PIX(LIGHT_GREEN_IDX), PIX(LIGHT_RED_IDX), PIX(LIGHT_BLUE_IDX), PIX(LIGHT_BROWN_IDX), PIX(LIGHT_STEEL_IDX),
     PIX(DARK_GREEN_IDX), PIX(DARK_RED_IDX), PIX(DARK_BLUE_IDX), PIX(DARK_BROWN_IDX), PIX(DARK_STEEL_IDX),
  // 2
  0, PIX(LIGHT_GREEN_IDX), PIX(LIGHT_RED_IDX), PIX(LIGHT_BLUE_IDX), PIX(LIGHT_BROWN_IDX), PIX(LIGHT_STEEL_IDX),
     PIX(LIGHT_GREEN_IDX), PIX(LIGHT_RED_IDX), PIX(LIGHT_BLUE_IDX), PIX(LIGHT_BROWN_IDX), PIX(LIGHT_STEEL_IDX),
     PIX(LIGHT_GREEN_IDX), PIX(LIGHT_RED_IDX), PIX(LIGHT_BLUE_IDX), PIX(LIGHT_BROWN_IDX), PIX(LIGHT_STEEL_IDX),
};

/*
const u32 long_color_table[256] = {
 0x00000000, 0x01010101, 0x02020202, 0x03030303,
0x04040404, 0x05050505, 0x06060606, 0x07070707,
0x08080808, 0x09090909, 0x0a0a0a0a, 0x0b0b0b0b,
0x0c0c0c0c, 0x0d0d0d0d, 0x0e0e0e0e, 0x0f0f0f0f,
0x10101010, 0x11111111, 0x12121212, 0x13131313,
0x14141414, 0x15151515, 0x16161616, 0x17171717,
0x18181818, 0x19191919, 0x1a1a1a1a, 0x1b1b1b1b,
0x1c1c1c1c, 0x1d1d1d1d, 0x1e1e1e1e, 0x1f1f1f1f,
0x20202020, 0x21212121, 0x22222222, 0x23232323,
0x24242424, 0x25252525, 0x26262626, 0x27272727,
0x28282828, 0x29292929, 0x2a2a2a2a, 0x2b2b2b2b,
0x2c2c2c2c, 0x2d2d2d2d, 0x2e2e2e2e, 0x2f2f2f2f,
0x30303030, 0x31313131, 0x32323232, 0x33333333,
0x34343434, 0x35353535, 0x36363636, 0x37373737,
0x38383838, 0x39393939, 0x3a3a3a3a, 0x3b3b3b3b,
0x3c3c3c3c, 0x3d3d3d3d, 0x3e3e3e3e, 0x3f3f3f3f,
0x40404040, 0x41414141, 0x42424242, 0x43434343,
0x44444444, 0x45454545, 0x46464646, 0x47474747,
0x48484848, 0x49494949, 0x4a4a4a4a, 0x4b4b4b4b,
0x4c4c4c4c, 0x4d4d4d4d, 0x4e4e4e4e, 0x4f4f4f4f,
0x50505050, 0x51515151, 0x52525252, 0x53535353,
0x54545454, 0x55555555, 0x56565656, 0x57575757,
0x58585858, 0x59595959, 0x5a5a5a5a, 0x5b5b5b5b,
0x5c5c5c5c, 0x5d5d5d5d, 0x5e5e5e5e, 0x5f5f5f5f,
0x60606060, 0x61616161, 0x62626262, 0x63636363,
0x64646464, 0x65656565, 0x66666666, 0x67676767,
0x68686868, 0x69696969, 0x6a6a6a6a, 0x6b6b6b6b,
0x6c6c6c6c, 0x6d6d6d6d, 0x6e6e6e6e, 0x6f6f6f6f,
0x70707070, 0x71717171, 0x72727272, 0x73737373,
0x74747474, 0x75757575, 0x76767676, 0x77777777,
0x78787878, 0x79797979, 0x7a7a7a7a, 0x7b7b7b7b,
0x7c7c7c7c, 0x7d7d7d7d, 0x7e7e7e7e, 0x7f7f7f7f,
0x80808080, 0x81818181, 0x82828282, 0x83838383,
0x84848484, 0x85858585, 0x86868686, 0x87878787,
0x88888888, 0x89898989, 0x8a8a8a8a, 0x8b8b8b8b,
0x8c8c8c8c, 0x8d8d8d8d, 0x8e8e8e8e, 0x8f8f8f8f,
0x90909090, 0x91919191, 0x92929292, 0x93939393,
0x94949494, 0x95959595, 0x96969696, 0x97979797,
0x98989898, 0x99999999, 0x9a9a9a9a, 0x9b9b9b9b,
0x9c9c9c9c, 0x9d9d9d9d, 0x9e9e9e9e, 0x9f9f9f9f,
0xa0a0a0a0, 0xa1a1a1a1, 0xa2a2a2a2, 0xa3a3a3a3,
0xa4a4a4a4, 0xa5a5a5a5, 0xa6a6a6a6, 0xa7a7a7a7,
0xa8a8a8a8, 0xa9a9a9a9, 0xaaaaaaaa, 0xabababab,
0xacacacac, 0xadadadad, 0xaeaeaeae, 0xafafafaf,
0xb0b0b0b0, 0xb1b1b1b1, 0xb2b2b2b2, 0xb3b3b3b3,
0xb4b4b4b4, 0xb5b5b5b5, 0xb6b6b6b6, 0xb7b7b7b7,
0xb8b8b8b8, 0xb9b9b9b9, 0xbabababa, 0xbbbbbbbb,
0xbcbcbcbc, 0xbdbdbdbd, 0xbebebebe, 0xbfbfbfbf,
0xc0c0c0c0, 0xc1c1c1c1, 0xc2c2c2c2, 0xc3c3c3c3,
0xc4c4c4c4, 0xc5c5c5c5, 0xc6c6c6c6, 0xc7c7c7c7,
0xc8c8c8c8, 0xc9c9c9c9, 0xcacacaca, 0xcbcbcbcb,
0xcccccccc, 0xcdcdcdcd, 0xcececece, 0xcfcfcfcf,
0xd0d0d0d0, 0xd1d1d1d1, 0xd2d2d2d2, 0xd3d3d3d3,
0xd4d4d4d4, 0xd5d5d5d5, 0xd6d6d6d6, 0xd7d7d7d7,
0xd8d8d8d8, 0xd9d9d9d9, 0xdadadada, 0xdbdbdbdb,
0xdcdcdcdc, 0xdddddddd, 0xdededede, 0xdfdfdfdf,
0xe0e0e0e0, 0xe1e1e1e1, 0xe2e2e2e2, 0xe3e3e3e3,
0xe4e4e4e4, 0xe5e5e5e5, 0xe6e6e6e6, 0xe7e7e7e7,
0xe8e8e8e8, 0xe9e9e9e9, 0xeaeaeaea, 0xebebebeb,
0xecececec, 0xedededed, 0xeeeeeeee, 0xefefefef,
0xf0f0f0f0, 0xf1f1f1f1, 0xf2f2f2f2, 0xf3f3f3f3,
0xf4f4f4f4, 0xf5f5f5f5, 0xf6f6f6f6, 0xf7f7f7f7,
0xf8f8f8f8, 0xf9f9f9f9, 0xfafafafa, 0xfbfbfbfb,
0xfcfcfcfc, 0xfdfdfdfd, 0xfefefefe, 0xffffffff,
};
*/
// 960 bytes


u32 get_dark_color(u8 col_idx, s8 light_level) {
  u16 light_off = (light_level+2)<<4;
  return color_calc_table[light_off+col_idx];
}

u32 get_mid_dark_color(u8 col_idx, s8 light_level) {
  u16 light_off = (light_level+2)<<4;  
  u16 dist_off = (16*5);
  return color_calc_table[light_off+dist_off+col_idx];
}

u32 get_light_color(u8 col_idx, s8 light_level) {
  u16 light_off = (light_level+2)<<4;  
  u16 dist_off = (16*5*2);
  return color_calc_table[light_off+dist_off+col_idx];
}

u32 swizzled_color_calc_table[16*5*4];

void init_swizzled_color_calc_table() {
   for(int color = 0; color < 16; color++) {
      for(int light = -2; light <= 2; light++) {
         u32 dcol = get_dark_color(color, light);
         u32 mcol = get_mid_dark_color(color, light);
         u32 lcol = get_light_color(color, light);
         // color is up to 16
         // light is up to 5

         // organized in terms of light levels
         // i.e. light level 0
         // color 0
         // color_0_light_level_0_dist_0, ... color_0_light_level_0_dist_3,
         // 3 * 16

         u16 col_off = color * 4;
         u16 light_off = 4 * 16 * (light + 2);
         swizzled_color_calc_table[light_off + col_off + 0] = dcol;
         swizzled_color_calc_table[light_off + col_off + 1] = mcol;
         swizzled_color_calc_table[light_off + col_off + 2] = lcol;
      }
   }
}


u32* get_color_ptr(u16 color, s16 light_level) {
   u32* ret = swizzled_color_calc_table;

   __asm volatile(                         
      "addq #2, %2\t\n\
       lsl.w #6, %2\t\n\
       lsl.w #2, %1\t\n\
       add.w %1, %2\t\n\
       lsl.w #2, %2\t\n\
       add.l %2, %0\t\n\
       "
      : "+a" (ret)
      : "d" (color), "d" (light_level)
   );
   return ret;

   //u16 col_off = color * 4;
   //u16 light_off = 4 * 16 * (light_level + 2);
   //return swizzled_color_calc_table + light_off + col_off;
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