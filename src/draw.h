#ifndef DRAW_H
#define DRAW_H

#include <genesis.h>
#include "vertex.h"

typedef enum {
    AUTOMAP = 0,
    GAME_WIREFRAME = 1,
    GAME_SOLID = 2 
} draw_mode;

typedef struct {
    u8 visible;
    u16 x1;
    u16 x2;
} vis_query_result;

// returns 1 if drawn, 0 otherwise (backfacing or off-screen)
vis_query_result is_visible(Vect2D_s32 trans_v1, Vect2D_s32 trans_v2, u16 window_min, u16 window_max);
int draw_wall_from_verts(Vect2D_s32 trans_v1, Vect2D_s32 trans_v2, s16 ceil_height, s16 floor_height, u16 window_min, u16 window_max);

#endif