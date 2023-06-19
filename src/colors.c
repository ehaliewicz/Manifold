#include <genesis.h>
#include "colors.h"


// #define PIX(p) ((p << 4) | p)
// #define DPIX(p,q) ((p << 4) | q)


#define BPIX(p) ((p << 4) | p)

#define BDPIX(p,q) ((p << 4) | q)

#define PIX(p) ((BPIX(p) << 24) | (BPIX(p) << 16) | (BPIX(p) << 8) | BPIX(p))
#define DPIX(p,q) ((BDPIX(p,q) << 24) | (BDPIX(p,q) << 16) | (BDPIX(p,q) << 8) | BDPIX(p,q))

//               DISTANCE
// LIGHT LEVEL      NEAR       FAR
// 1                LIGHT      LIGHT
// 0                LIGHT      DARK
// -1               DARK       DARK

//             LIGHT LEVEL
// DISTANCE     1     0     -1
// FAR          LIGHT DARK DARK 
// NEAR         LIGHT LIGHT DARK

// dark dist is -2 brightness
// mid dist is -1 brightness
// -2 is negative 2, etc

 //split byte colors 
const u32 dither_color_calc_table[16*5*2] = {
   // far table
   // -2 light level
   0, DPIX(10,10), DPIX(9,9), DPIX(9,9), DPIX(6,6), DPIX(6,6), DPIX(6,6), DPIX(6,6), DPIX(6,6), DPIX(10,10), DPIX(10,10), DPIX(10,10), DPIX(6,6), DPIX(6,6), DPIX(11,11), DPIX(15,15),
   // -1 light level
   0, DPIX(9,10), DPIX(1,9), DPIX(2,9), DPIX(7,6), DPIX(5,6), DPIX(6,6), DPIX(7,6), DPIX(8,6), DPIX(11,10), DPIX(10,10), DPIX(11,10), DPIX(13,6), DPIX(13,6), DPIX(9,11), DPIX(15,15),
   // 0 light level
   0, DPIX(9,9), DPIX(1,1), DPIX(2,2), DPIX(7,7), DPIX(5,6), DPIX(6,6), DPIX(7,6), DPIX(8,6), DPIX(11,11), DPIX(10,10), DPIX(11,10), DPIX(13,13), DPIX(13,6), DPIX(9,9), DPIX(15,15),
   // 1 light level
   0, DPIX(1,1), DPIX(2,2), DPIX(3,3), DPIX(4,4), DPIX(5,5), DPIX(6,6), DPIX(7,7), DPIX(8,8), DPIX(9,9), DPIX(10,10), DPIX(11,11), DPIX(12,12), DPIX(13,13), DPIX(14,14), DPIX(15,15),
   // 2 light level
   0, DPIX(3,3), DPIX(3,3), DPIX(3,3), DPIX(4,4), DPIX(13,13), DPIX(13,13), DPIX(4,4), DPIX(13,13), DPIX(3,3), DPIX(9,9), DPIX(9,9), DPIX(4,4), DPIX(4,4), DPIX(14,14), DPIX(15,15),
   // near table
   // -2 light level
   0, DPIX(10,10), DPIX(9,9), DPIX(9,9), DPIX(6,6), DPIX(6,6), DPIX(6,6), DPIX(6,6), DPIX(6,6), DPIX(10,10), DPIX(10,10), DPIX(10,10), DPIX(6,6), DPIX(6,6), DPIX(11,11), DPIX(15,15),
   // -1 light level
   0, DPIX(9,9), DPIX(1,1), DPIX(2,2), DPIX(7,7), DPIX(5,6), DPIX(6,6), DPIX(7,6), DPIX(8,6), DPIX(11,11), DPIX(10,10), DPIX(11,10), DPIX(13,13), DPIX(6,6), DPIX(9,9), DPIX(15,15),
   // 0 light level
   0, DPIX(1,1), DPIX(2,2), DPIX(3,3), DPIX(4,4), DPIX(5,5), DPIX(6,6), DPIX(7,7), DPIX(8,8), DPIX(9,9), DPIX(10,10), DPIX(11,11), DPIX(12,12), DPIX(13,13), DPIX(14,14), DPIX(15,15),
   // 1 light level
   0, DPIX(2,2), DPIX(3,2), DPIX(3,2), DPIX(4,4), DPIX(13,5), DPIX(5,5), DPIX(4,7), DPIX(5,5), DPIX(2,2), DPIX(11,11), DPIX(9,11), DPIX(4,12), DPIX(12,12), DPIX(14,14), DPIX(15,15),
   // 2 light level
   0, DPIX(3,3), DPIX(3,3), DPIX(3,3), DPIX(4,4), DPIX(13,13), DPIX(13,13), DPIX(4,4), DPIX(13,13), DPIX(3,3), DPIX(9,9), DPIX(9,9), DPIX(4,4), DPIX(4,4), DPIX(14,14), DPIX(15,15),
};


const u32 no_dither_color_calc_table[16*5*2] = {
   // far table
   // -2 light level
   0, DPIX(10,10), DPIX(9,9), DPIX(9,9), DPIX(6,6), DPIX(6,6), DPIX(6,6), DPIX(6,6), DPIX(6,6), DPIX(10,10), DPIX(10,10), DPIX(10,10), DPIX(6,6), DPIX(6,6), DPIX(11,11), DPIX(15,15),
   // -1 light level
   0, DPIX(10,10), DPIX(9,9), DPIX(9,9), DPIX(6,6), DPIX(6,6), DPIX(6,6), DPIX(6,6), DPIX(6,6), DPIX(10,10), DPIX(10,10), DPIX(10,10), DPIX(6,6), DPIX(6,6), DPIX(11,11), DPIX(15,15),
   // 0 light level
   0, DPIX(9,9), DPIX(1,1), DPIX(2,2), DPIX(7,7), DPIX(6,6), DPIX(6,6), DPIX(6,6), DPIX(6,6), DPIX(11,11), DPIX(10,10), DPIX(10,10), DPIX(13,13), DPIX(6,6), DPIX(9,9), DPIX(15,15),
   // 1 light level
   0, DPIX(1,1), DPIX(2,2), DPIX(3,3), DPIX(4,4), DPIX(5,5), DPIX(6,6), DPIX(7,7), DPIX(8,8), DPIX(9,9), DPIX(10,10), DPIX(11,11), DPIX(12,12), DPIX(13,13), DPIX(14,14), DPIX(15,15),
   // 2 light level
   0, DPIX(3,3), DPIX(3,3), DPIX(3,3), DPIX(4,4), DPIX(13,13), DPIX(13,13), DPIX(4,4), DPIX(13,13), DPIX(3,3), DPIX(9,9), DPIX(9,9), DPIX(4,4), DPIX(4,4), DPIX(14,14), DPIX(15,15),
   // near table
   // -2 light level
   0, DPIX(10,10), DPIX(9,9), DPIX(9,9), DPIX(6,6), DPIX(6,6), DPIX(6,6), DPIX(6,6), DPIX(6,6), DPIX(10,10), DPIX(10,10), DPIX(10,10), DPIX(6,6), DPIX(6,6), DPIX(11,11), DPIX(15,15),
   // -1 light level
   0, DPIX(9,9), DPIX(1,1), DPIX(2,2), DPIX(7,7), DPIX(6,6), DPIX(6,6), DPIX(6,6), DPIX(6,6), DPIX(11,11), DPIX(10,10), DPIX(10,10), DPIX(13,13), DPIX(6,6), DPIX(9,9), DPIX(15,15),
   // 0 light level
   0, DPIX(1,1), DPIX(2,2), DPIX(3,3), DPIX(4,4), DPIX(5,5), DPIX(6,6), DPIX(7,7), DPIX(8,8), DPIX(9,9), DPIX(10,10), DPIX(11,11), DPIX(12,12), DPIX(13,13), DPIX(14,14), DPIX(15,15),
   // 1 light level
   0, DPIX(2,2), DPIX(3,3), DPIX(3,3), DPIX(4,4), DPIX(13,13), DPIX(5,5), DPIX(4,4), DPIX(5,5), DPIX(2,2), DPIX(11,11), DPIX(9,9), DPIX(12,12), DPIX(12,12), DPIX(14,14), DPIX(15,15),
   // 2 light level
   0, DPIX(3,3), DPIX(3,3), DPIX(3,3), DPIX(4,4), DPIX(13,13), DPIX(13,13), DPIX(4,4), DPIX(13,13), DPIX(3,3), DPIX(9,9), DPIX(9,9), DPIX(4,4), DPIX(4,4), DPIX(14,14), DPIX(15,15),
};


u8 dither_enabled = 0;
u32* color_calc_table = (u32*)no_dither_color_calc_table;

void toggle_dither_mode() {
   if(dither_enabled) {
      dither_enabled = 0;
      color_calc_table = (u32*)no_dither_color_calc_table;
   } else {
      dither_enabled = 1;
      color_calc_table = (u32*)dither_color_calc_table;
   }
}


void init_color_table() {
   if(dither_enabled) {
      color_calc_table = dither_color_calc_table;
   } else {
      color_calc_table = no_dither_color_calc_table;
   }
}

// 960 bytes

u32 get_dark_color(u8 col_idx, s8 light_level) {
  u16 light_off = (light_level+2)<<4;
  return color_calc_table[light_off+col_idx];
}

//inline u32 get_mid_dark_color(u8 col_idx, s8 light_level) {
//  u16 light_off = (light_level+1)<<4;  
//  u16 dist_off = (16*5);
//  return color_calc_table[light_off+dist_off+col_idx];
//}

u32 get_light_color(u8 col_idx, s8 light_level) {
  u16 light_off = (light_level+2)<<4;  
  //u16 dist_off = (16*5*2);
  u16 dist_off = (16*5);
  return color_calc_table[light_off+dist_off+col_idx];
}
