#ifndef MATH3D_H
#define MATH3D_H

#include <genesis.h>
#include "texture.h"
#include "utils.h"

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
    UNCLIPPED             = 0b00000000,
    OFFSCREEN             = 0b00000001,
    LEFT_Z_CLIPPED        = 0b00000010,
    RIGHT_Z_CLIPPED       = 0b00000100,
    LEFT_FRUSTUM_CLIPPED  = 0b00001000,
    RIGHT_FRUSTUM_CLIPPED = 0b00010000,
} clip_result;

//#define NEAR_Z_32 (20 << FIX32_FRAC_BITS)
// NEAR_Z_16 10
#define NEAR_Z_16 10
//25 
#define NEAR_Z_FIX (NEAR_Z_16<<TRANS_Z_FRAC_BITS)

// translate and rotate map vertex according to player position
//Vect2D_f32 transform_map_vert(s16 x, s16 y);
#define TRANS_Z_FRAC_BITS 4
Vect2D_s16 transform_map_vert_16(s16 x, s16 y);

u16 get_texture_repetitions(s16 v1x, s16 v1y, s16 v2x, s16 v2y);

clip_result frustum_cull_vertex_16(Vect2D_s16* __restrict__ trans_v1, Vect2D_s16* __restrict__ trans_v2);
// clip pair of map vertexes against near z plane, modifies input pointers if necessary
//clip_result clip_map_vertex(Vect2D_f32* trans_v1, Vect2D_f32* trans_v2);
clip_result clip_map_vertex_16(Vect2D_s16* trans_v1, Vect2D_s16* trans_v2, texmap_params* tmap);

// project map vertex without height attributes
//s16 project_and_adjust_x(Vect2D_f32 trans_map_vert);

s16 project_and_adjust_x(s16 x, s16 z_recip); //Vect2D_s16 trans_map_vert);

//s16 project_and_adjust_y(Vect2D_f32 trans_map_vert, s16 y);

//s16 project_and_adjust_y_fix(Vect2D_f32 trans_map_vert, s16 y);

//s16 project_and_adjust_y_fix(Vect2D_s16 trans_map_vert, s16 y);

s16 project_and_adjust_y_fix(s16 y, s16 z_recip);

s16 project_and_adjust_y_fix_c(s16 y, s16 z);

s8 point_sign_int_vert(f32 x, f32 y, s16 v1_x, s16 v1_y, s16 v2_x, s16 v2_y);
s8 point_sign_int_vert_s16(s16 x, s16 y, s16 v1_x, s16 v1_y, s16 v2_x, s16 v2_y);
u8 within_frustum(s16 x1, s16 y1, s16 x2, s16 y2);

#endif