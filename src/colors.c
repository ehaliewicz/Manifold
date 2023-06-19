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


const u32 color_calc_table[16*5*2] = {
// far table
  // -2 light level
0, PIX(10), PIX(8), PIX(8), PIX(8), PIX(6), PIX(15), PIX(6), PIX(6), PIX(10), PIX(15), PIX(10), PIX(8), PIX(8), PIX(8), PIX(10), 
  // -1 light level
0, PIX(10), PIX(8), PIX(8), PIX(8), PIX(6), PIX(15), PIX(6), PIX(6), PIX(10), PIX(15), PIX(10), PIX(8), PIX(8), PIX(8), PIX(10), 
  // 0 light level
0, PIX(8), PIX(1), PIX(1), PIX(5), PIX(8), PIX(6), PIX(7), PIX(6), PIX(11), PIX(10), PIX(10), PIX(5), PIX(5), PIX(7), PIX(10), 
  // 1 light level
    0, PIX(1), PIX(2), PIX(3), PIX(4), PIX(5), PIX(6), PIX(7), PIX(8), PIX(9), PIX(10), PIX(11), PIX(12), PIX(13), PIX(14), PIX(15), 
  // 2 light level
0, PIX(3), PIX(3), PIX(3), PIX(3), PIX(4), PIX(8), PIX(14), PIX(1), PIX(14), PIX(10), PIX(11), PIX(4), PIX(3), PIX(14), PIX(10), 
// near table
  // -2 light level
0, PIX(10), PIX(8), PIX(8), PIX(8), PIX(6), PIX(15), PIX(6), PIX(6), PIX(10), PIX(15), PIX(10), PIX(8), PIX(8), PIX(8), PIX(10), 
  // -1 light level
0, PIX(8), PIX(1), PIX(1), PIX(5), PIX(8), PIX(6), PIX(7), PIX(6), PIX(11), PIX(10), PIX(10), PIX(5), PIX(5), PIX(7), PIX(10), 
  // 0 light level
0, PIX(1), PIX(2), PIX(3), PIX(4), PIX(5), PIX(6), PIX(7), PIX(8), PIX(9), PIX(10), PIX(11), PIX(12), PIX(13), PIX(14), PIX(15), 
  // 1 light level
0, PIX(2), PIX(3), PIX(3), PIX(4), PIX(12), PIX(6), PIX(7), PIX(5), PIX(9), PIX(10), PIX(11), PIX(4), PIX(13), PIX(14), PIX(10), 
  // 2 light level
0, PIX(3), PIX(3), PIX(3), PIX(3), PIX(4), PIX(8), PIX(14), PIX(1), PIX(14), PIX(10), PIX(11), PIX(4), PIX(3), PIX(14), PIX(10), 
};

/*

const u32 color_calc_table[16*5*2] = {
   // far dist lut [16*3] = {
      // -2 light level
      0, PIX(DARK_YELLOW_IDX), PIX(DARK_BLUE_IDX), PIX(GREEN_IDX), PIX(RED_IDX), PIX(PURPLE_IDX), PIX(STEEL_IDX),
         PIX(DARK_YELLOW_IDX), PIX(DARK_BLUE_IDX), PIX(GREEN_IDX), PIX(RED_IDX), PIX(PURPLE_IDX), PIX(STEEL_IDX),
         PIX(DARK_YELLOW_IDX), PIX(DARK_BLUE_IDX), PIX(BLACK_IDX),
      // -1 light level 
      0, PIX(YELLOW_IDX), PIX(BLUE_IDX), PIX(GREEN_IDX), PIX(RED_IDX), PIX(PURPLE_IDX), PIX(STEEL_IDX),
         PIX(YELLOW_IDX), PIX(BLUE_IDX), PIX(GREEN_IDX), PIX(RED_IDX), PIX(PURPLE_IDX), PIX(STEEL_IDX),
         PIX(DARK_YELLOW_IDX), PIX(DARK_BLUE_IDX), PIX(BLACK_IDX),
      // 0 light level 
      0, PIX(YELLOW_IDX), PIX(BLUE_IDX), PIX(GREEN_IDX), PIX(RED_IDX), PIX(PURPLE_IDX), PIX(STEEL_IDX),
         PIX(YELLOW_IDX), PIX(BLUE_IDX), PIX(GREEN_IDX), PIX(RED_IDX), PIX(PURPLE_IDX), PIX(STEEL_IDX),
         PIX(DARK_YELLOW_IDX), PIX(DARK_BLUE_IDX), PIX(BLACK_IDX),
      // 1 light level
      0, PIX(LIGHT_YELLOW_IDX), PIX(LIGHT_BLUE_IDX), PIX(LIGHT_GREEN_IDX), PIX(LIGHT_RED_IDX), PIX(LIGHT_PURPLE_IDX), PIX(LIGHT_STEEL_IDX),
         PIX(LIGHT_YELLOW_IDX), PIX(LIGHT_BLUE_IDX), PIX(LIGHT_GREEN_IDX), PIX(LIGHT_RED_IDX), PIX(LIGHT_PURPLE_IDX), PIX(LIGHT_STEEL_IDX),
         PIX(YELLOW_IDX), PIX(BLUE_IDX), PIX(BLACK_IDX),
      // 2 light level
      0, PIX(LIGHT_YELLOW_IDX), PIX(LIGHT_BLUE_IDX), PIX(LIGHT_GREEN_IDX), PIX(LIGHT_RED_IDX), PIX(LIGHT_PURPLE_IDX), PIX(LIGHT_STEEL_IDX),
         PIX(LIGHT_YELLOW_IDX), PIX(LIGHT_BLUE_IDX), PIX(LIGHT_GREEN_IDX), PIX(LIGHT_RED_IDX), PIX(LIGHT_PURPLE_IDX), PIX(LIGHT_STEEL_IDX),
         PIX(LIGHT_YELLOW_IDX), PIX(LIGHT_BLUE_IDX), PIX(BLACK_IDX),

   //}

   // near dist lut [16*3] = {
      // -2 light level
      0, PIX(DARK_YELLOW_IDX), PIX(DARK_BLUE_IDX), PIX(GREEN_IDX), PIX(RED_IDX), PIX(PURPLE_IDX), PIX(STEEL_IDX),
         PIX(DARK_YELLOW_IDX), PIX(DARK_BLUE_IDX), PIX(GREEN_IDX), PIX(RED_IDX), PIX(PURPLE_IDX), PIX(STEEL_IDX),
         PIX(DARK_YELLOW_IDX), PIX(DARK_BLUE_IDX), PIX(BLACK_IDX),
      // -1 light level
      0, PIX(YELLOW_IDX), PIX(BLUE_IDX), PIX(GREEN_IDX), PIX(RED_IDX), PIX(PURPLE_IDX), PIX(STEEL_IDX),
         PIX(YELLOW_IDX), PIX(BLUE_IDX), PIX(GREEN_IDX), PIX(RED_IDX), PIX(PURPLE_IDX), PIX(STEEL_IDX),
         PIX(DARK_YELLOW_IDX), PIX(DARK_BLUE_IDX), PIX(BLACK_IDX),
      // 0 light level 
      0, PIX(LIGHT_YELLOW_IDX), PIX(LIGHT_BLUE_IDX), PIX(LIGHT_GREEN_IDX), PIX(LIGHT_RED_IDX), PIX(LIGHT_PURPLE_IDX), PIX(LIGHT_STEEL_IDX),
         PIX(YELLOW_IDX), PIX(BLUE_IDX), PIX(GREEN_IDX), PIX(RED_IDX), PIX(PURPLE_IDX), PIX(STEEL_IDX),
         PIX(DARK_YELLOW_IDX), PIX(DARK_BLUE_IDX), PIX(BLACK_IDX),
      // 1 light level
      0, PIX(LIGHT_YELLOW_IDX), PIX(LIGHT_BLUE_IDX), PIX(LIGHT_GREEN_IDX), PIX(LIGHT_RED_IDX), PIX(LIGHT_PURPLE_IDX), PIX(LIGHT_STEEL_IDX),
         PIX(LIGHT_YELLOW_IDX), PIX(LIGHT_BLUE_IDX), PIX(LIGHT_GREEN_IDX), PIX(LIGHT_RED_IDX), PIX(LIGHT_PURPLE_IDX), PIX(LIGHT_STEEL_IDX),
         PIX(YELLOW_IDX), PIX(BLUE_IDX), PIX(BLACK_IDX),
      // 2 light level
      0, PIX(LIGHT_YELLOW_IDX), PIX(LIGHT_BLUE_IDX), PIX(LIGHT_GREEN_IDX), PIX(LIGHT_RED_IDX), PIX(LIGHT_PURPLE_IDX), PIX(LIGHT_STEEL_IDX),
         PIX(LIGHT_YELLOW_IDX), PIX(LIGHT_BLUE_IDX), PIX(LIGHT_GREEN_IDX), PIX(LIGHT_RED_IDX), PIX(LIGHT_PURPLE_IDX), PIX(LIGHT_STEEL_IDX),
         PIX(LIGHT_YELLOW_IDX), PIX(LIGHT_BLUE_IDX), PIX(BLACK_IDX),

   //}
};
*/

/*
const u32 color_calc_table[16*5*3] = {
//const u8 color_calc_table[16*5*3] = {

//const u8 dark_dist_lut[16*5] = {
  // -2
  0, PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),              PIX(DARK_PURPLE_IDX),                PIX(DARK_RED_IDX),
     PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),              PIX(DARK_PURPLE_IDX),                PIX(DARK_RED_IDX),
     PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),              PIX(DARK_PURPLE_IDX),                PIX(DARK_RED_IDX),
  // -1
  0, PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),              PIX(DARK_PURPLE_IDX),                PIX(DARK_RED_IDX),
     PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),              PIX(DARK_PURPLE_IDX),                PIX(DARK_RED_IDX),
     PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),              PIX(DARK_PURPLE_IDX),                PIX(DARK_RED_IDX),
  // 0
  0, PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),              PIX(DARK_PURPLE_IDX),                PIX(DARK_RED_IDX),
     PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),              PIX(DARK_PURPLE_IDX),                PIX(DARK_RED_IDX),
     PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),              PIX(DARK_PURPLE_IDX),                PIX(DARK_RED_IDX),
  // 1
  0, PIX(YELLOW_IDX),                    PIX(BLUE_IDX),                  PIX(GREEN_IDX),                   PIX(PURPLE_IDX),                     PIX(RED_IDX),
     PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),              PIX(DARK_PURPLE_IDX),                PIX(DARK_RED_IDX),
     PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),              PIX(DARK_PURPLE_IDX),                PIX(DARK_RED_IDX),
  // 2
  0, PIX(LIGHT_YELLOW_IDX),              PIX(LIGHT_BLUE_IDX),            PIX(LIGHT_GREEN_IDX),             PIX(LIGHT_PURPLE_IDX),               PIX(LIGHT_RED_IDX),
     PIX(YELLOW_IDX),                    PIX(BLUE_IDX),                  PIX(GREEN_IDX),                   PIX(PURPLE_IDX),                     PIX(RED_IDX),
     PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),              PIX(DARK_PURPLE_IDX),                PIX(DARK_RED_IDX),
//};

//const u8 mid_dist_lut[16*5] = {
  // -2
  0, PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),              PIX(DARK_PURPLE_IDX),                PIX(DARK_RED_IDX),
     PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),              PIX(DARK_PURPLE_IDX),                PIX(DARK_RED_IDX),
     PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),              PIX(DARK_PURPLE_IDX),                PIX(DARK_RED_IDX),
  // -1
  0, DPIX(YELLOW_IDX,DARK_YELLOW_IDX),   DPIX(BLUE_IDX,DARK_BLUE_IDX),   DPIX(GREEN_IDX, DARK_STEEL_IDX),  DPIX(PURPLE_IDX, DARK_PURPLE_IDX),   DPIX(RED_IDX, DARK_RED_IDX),
     PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),              PIX(DARK_PURPLE_IDX),                PIX(DARK_RED_IDX),
     PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),              PIX(DARK_PURPLE_IDX),                PIX(DARK_RED_IDX),
  // 0
  0, DPIX(LIGHT_YELLOW_IDX, YELLOW_IDX), DPIX(LIGHT_BLUE_IDX, BLUE_IDX), DPIX(LIGHT_GREEN_IDX, GREEN_IDX), DPIX(LIGHT_PURPLE_IDX, PURPLE_IDX),  DPIX(LIGHT_RED_IDX, RED_IDX),
     DPIX(YELLOW_IDX,DARK_YELLOW_IDX),   DPIX(BLUE_IDX,DARK_BLUE_IDX),   DPIX(GREEN_IDX, DARK_STEEL_IDX),  DPIX(PURPLE_IDX, DARK_PURPLE_IDX),   DPIX(RED_IDX, DARK_RED_IDX),
     PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),              PIX(DARK_PURPLE_IDX),                PIX(DARK_RED_IDX),
  // 1
  0, DPIX(LIGHT_YELLOW_IDX, YELLOW_IDX), DPIX(LIGHT_BLUE_IDX, BLUE_IDX), DPIX(LIGHT_GREEN_IDX, GREEN_IDX),  DPIX(LIGHT_PURPLE_IDX, PURPLE_IDX), DPIX(LIGHT_RED_IDX, RED_IDX),
     DPIX(LIGHT_YELLOW_IDX, YELLOW_IDX), DPIX(LIGHT_BLUE_IDX, BLUE_IDX), DPIX(LIGHT_GREEN_IDX, GREEN_IDX),  DPIX(LIGHT_PURPLE_IDX, PURPLE_IDX), DPIX(LIGHT_RED_IDX, RED_IDX),
     DPIX(YELLOW_IDX,DARK_YELLOW_IDX),   DPIX(BLUE_IDX,DARK_BLUE_IDX),   DPIX(GREEN_IDX, DARK_STEEL_IDX),   DPIX(PURPLE_IDX, DARK_PURPLE_IDX),  DPIX(RED_IDX, DARK_RED_IDX),
  // 2
  0, DPIX(LIGHT_YELLOW_IDX, YELLOW_IDX), DPIX(LIGHT_BLUE_IDX, BLUE_IDX), DPIX(LIGHT_GREEN_IDX, GREEN_IDX),  DPIX(LIGHT_PURPLE_IDX, PURPLE_IDX), DPIX(LIGHT_RED_IDX, RED_IDX),
     DPIX(LIGHT_YELLOW_IDX, YELLOW_IDX), DPIX(LIGHT_BLUE_IDX, BLUE_IDX), DPIX(LIGHT_GREEN_IDX, GREEN_IDX),  DPIX(LIGHT_PURPLE_IDX, PURPLE_IDX), DPIX(LIGHT_RED_IDX, RED_IDX),
     DPIX(LIGHT_YELLOW_IDX, YELLOW_IDX), DPIX(LIGHT_BLUE_IDX, BLUE_IDX), DPIX(LIGHT_GREEN_IDX, GREEN_IDX),  DPIX(LIGHT_PURPLE_IDX, PURPLE_IDX), DPIX(LIGHT_RED_IDX, RED_IDX),
//};

//const u8 near_dist[16*5] = {
  // -2
  0, PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),               PIX(DARK_PURPLE_IDX),               PIX(DARK_RED_IDX),
     PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),               PIX(DARK_PURPLE_IDX),               PIX(DARK_RED_IDX),
     PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),               PIX(DARK_PURPLE_IDX),               PIX(DARK_RED_IDX),
  // -1
  0, PIX(YELLOW_IDX),                    PIX(BLUE_IDX),                  PIX(GREEN_IDX),                    PIX(PURPLE_IDX),                    PIX(RED_IDX),
     PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),               PIX(DARK_PURPLE_IDX),               PIX(DARK_RED_IDX),
     PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),               PIX(DARK_PURPLE_IDX),               PIX(DARK_RED_IDX),
  // 0
  0, PIX(DARK_YELLOW_IDX),               PIX(LIGHT_BLUE_IDX),            PIX(LIGHT_GREEN_IDX),              PIX(LIGHT_PURPLE_IDX),              PIX(LIGHT_RED_IDX),
     PIX(YELLOW_IDX),                    PIX(BLUE_IDX),                  PIX(GREEN_IDX),                    PIX(PURPLE_IDX),                    PIX(RED_IDX),
     PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),               PIX(DARK_PURPLE_IDX),               PIX(DARK_RED_IDX),
  // 1
  0, PIX(YELLOW_IDX),                    PIX(LIGHT_BLUE_IDX),            PIX(LIGHT_GREEN_IDX),              PIX(LIGHT_PURPLE_IDX),              PIX(LIGHT_RED_IDX),
     PIX(YELLOW_IDX),                    PIX(LIGHT_BLUE_IDX),            PIX(LIGHT_GREEN_IDX),              PIX(LIGHT_PURPLE_IDX),              PIX(LIGHT_RED_IDX),
     PIX(DARK_YELLOW_IDX),               PIX(DARK_BLUE_IDX),             PIX(DARK_STEEL_IDX),               PIX(DARK_PURPLE_IDX),               PIX(DARK_RED_IDX),
  // 2
  0, PIX(DARK_YELLOW_IDX),               PIX(LIGHT_BLUE_IDX),            PIX(LIGHT_GREEN_IDX),              PIX(LIGHT_PURPLE_IDX),              PIX(LIGHT_RED_IDX),
     PIX(DARK_YELLOW_IDX),               PIX(LIGHT_BLUE_IDX),            PIX(LIGHT_GREEN_IDX),              PIX(LIGHT_PURPLE_IDX),              PIX(LIGHT_RED_IDX),
     PIX(DARK_YELLOW_IDX),               PIX(LIGHT_BLUE_IDX),            PIX(LIGHT_GREEN_IDX),              PIX(LIGHT_PURPLE_IDX),              PIX(LIGHT_RED_IDX),
};
*/

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
