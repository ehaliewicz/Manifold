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
    s16 draw_x1; s16 draw_x2;
    s16 window_x1; s16 window_x2;
    s16 x1_ytop; s16 x1_ybot;
    s16 x2_ytop; s16 x2_ybot;
} vis_query_result;

// returns 1 if drawn, 0 otherwise (backfacing or off-screen)
//vis_query_result is_visible(Vect2D_s32 trans_v1, Vect2D_s32 trans_v2, s16 floor, s16 ceil, u16 window_min, u16 window_max);

//int draw_wall_from_verts(Vect2D_s32 trans_v1, Vect2D_s32 trans_v2, s16 ceil_height, s16 floor_height, u16 window_min, u16 window_max);
//int draw_wall_from_verts(vis_query_result query_res, u16 window_min, u16 window_max);

void draw_wall(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 window_min, u16 window_max);

void draw_top_step(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 window_min, u16 window_max);

void draw_bot_step(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 window_min, u16 window_max);

void clear_2d_buffers();
void init_2d_buffers();
void release_2d_buffers();
#endif