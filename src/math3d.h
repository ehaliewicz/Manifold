#ifndef MATH3D_H
#define MATH3D_H

#include <genesis.h>
#include "texture.h"

#define SCREEN_WIDTH BMP_PITCH
#define SCREEN_HEIGHT 144 // BMP_HEIGHT   // H

// .888
#define ASPECT_RATIO (SCREEN_WIDTH / SCREEN_HEIGHT) // 0.44

// 3d->2d projection constants 

/*
#define SCALE 1
#define CONST1 64 // (SCREEN_WIDTH/2)
#define CONST2 60 // ((SCREEN_WIDTH/2) * SCALE / min(1, ASPECT_RATIO)) 
#define CONST3 72 //(SCREEN_HEIGHT/2)
#define CONST4 72 //(SCREEN_HEIGHT/2 * SCALE / max(1, ASPECT_RATIO))
*/

#define SCALE 1
#define CONST1 32
#define CONST2 32 //72
#define CONST3 72
#define CONST4 72 


typedef struct {
    s16 x, yfloor, yceil;
} transformed_vert;


typedef enum {
    UNCLIPPED,
    LEFT_CLIPPED,
    RIGHT_CLIPPED,
    OFFSCREEN
} clip_result;

//#define NEAR_Z_32 (20 << FIX32_FRAC_BITS)
#define NEAR_Z_16 10 

// translate and rotate map vertex according to player position
//Vect2D_f32 transform_map_vert(s16 x, s16 y);
Vect2D_s16 transform_map_vert_16(s16 x, s16 y);

// clip pair of map vertexes against near z plane, modifies input pointers if necessary
//clip_result clip_map_vertex(Vect2D_f32* trans_v1, Vect2D_f32* trans_v2);
clip_result clip_map_vertex_16(Vect2D_s16* trans_v1, Vect2D_s16* trans_v2, texmap_info* tmap, u32 wall_len);

// project map vertex without height attributes
//s16 project_and_adjust_x(Vect2D_f32 trans_map_vert);

s16 project_and_adjust_x(s16 x, s16 z_recip); //Vect2D_s16 trans_map_vert);

//s16 project_and_adjust_y(Vect2D_f32 trans_map_vert, s16 y);

//s16 project_and_adjust_y_fix(Vect2D_f32 trans_map_vert, s16 y);

//s16 project_and_adjust_y_fix(Vect2D_s16 trans_map_vert, s16 y);

s16 project_and_adjust_y_fix(s16 y, s16 z_recip);

s16 project_and_adjust_y_fix_c(s16 y, s16 z);

#endif