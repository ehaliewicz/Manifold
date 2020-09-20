#ifndef MATH3D_H
#define MATH3D_H

#include <genesis.h>


#define ASPECT_RATIO (BMP_WIDTH/BMP_HEIGHT)
#define SCALE 1
#define CONST1 128
#define CONST2 80
#define CONST3 80
#define CONST4 128

typedef struct {
    s16 x, yfloor, yceil;
} transformed_vert;


typedef enum {
    UNCLIPPED,
    LEFT_CLIPPED,
    RIGHT_CLIPPED,
    OFFSCREEN
} clip_result;


// translate and rotate map vertex according to player position
Vect2D_s32 transform_map_vert(s16 x, s16 y);

// clip pair of map vertexes against near z plane, modifies input pointers if necessary
clip_result clip_map_vertex(Vect2D_s32* trans_v1, Vect2D_s32* trans_v2);

// project map vertex without height attributes
s16 project_and_adjust_2d(Vect2D_s32 trans_map_vert);

int is_behind_near_plane(Vect2D_s32 transformed_map_vert);


// project map vertex with height attributes
transformed_vert project_and_adjust_3d(Vect2D_s32 trans_map_vert, s16 floor, s16 ceil);

#endif