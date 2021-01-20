#ifndef MATH3D_H
#define MATH3D_H

#include <genesis.h>

#define SCREEN_WIDTH BMP_PITCH
#define SCREEN_HEIGHT BMP_HEIGHT
#define ASPECT_RATIO (SCREEN_WIDTH /SCREEN_HEIGHT)
#define SCALE 1
#define CONST1 64 // SCREEN_WIDTH/2 (SCREEN_WIDTH / 2)
#define CONST2 80 // SCREEN_WIDTH/2 * SCALE / MIN(1, ASPECT_RATIO)
#define CONST3 80 // SCREEN_HEIGHT/2
#define CONST4 80 // SCREEN_HEIGHT/2 * SCALE / MAX(1, ASPECT_RATIO)

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
Vect2D_f32 transform_map_vert(s16 x, s16 y);

// clip pair of map vertexes against near z plane, modifies input pointers if necessary
clip_result clip_map_vertex(Vect2D_f32* trans_v1, Vect2D_f32* trans_v2);

// project map vertex without height attributes
s16 project_and_adjust_x(Vect2D_f32 trans_map_vert);
s16 project_and_adjust_y(Vect2D_f32 trans_map_vert, s16 y);

int is_behind_near_plane(Vect2D_f32 transformed_map_vert);


// project map vertex with height attributes
transformed_vert project_and_adjust_3d(Vect2D_f32 trans_map_vert, s16 floor, s16 ceil);

#endif