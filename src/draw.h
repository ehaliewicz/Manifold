#ifndef DRAW_H
#define DRAW_H

#include <genesis.h>
#include "clip_buf.h"
#include "texture.h"
#include "vertex.h"

typedef enum {
    GAME_WIREFRAME = 1,
    GAME_SOLID = 2 
} draw_mode;



typedef struct {
    u8 x;
    u8 y0;
    u8 y1;
    u8 clip_y0;
    u8 clip_y1;
    Bitmap *bmp;
} col_params;

void run_texture_test();

//void draw_native_vertical_line_unrolled(s16 y0, s16 y1, u8 col, u8* col_ptr);
void draw_native_vertical_line_unrolled(s16 y0, s16 y1, u32 full_col,  u8* col_ptr);
void copy_2d_buffer(u16 left, u16 right, clip_buf* dest);
void draw_native_vertical_transparent_line_unrolled(s16 y0, s16 y1, u8 col, u8* col_ptr, u8 odd);


void cache_floor_light_params(s16 rel_floor_height, u8 floor_col, s8 light_level, u32 avg_dist, u8 sloped);
void cache_ceil_light_params(s16 rel_ceil_height, u8 ceil_col, s8 light_level, u32 avg_dist, u8 sloped);

void draw_wall(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 z1,     u16 z2,
              u16 inv_z1, u16 inv_z2,
              u16 window_min, u16 window_max,
              u8 wall_col, s8 light_level, texmap_info tmap_info);

void draw_upper_step(s16 x1, s16 x1_ytop, s16 nx1_ytop, s16 x2, s16 x2_ytop, s16 nx2_ytop, 
                     u16 inv_z1, u16 inv_z2,
                     u16 window_min, u16 window_max, u8 upper_color, u8 ceil_color, s8 light_level);

void draw_ceiling_update_clip(s16 x1, s16 x1_ytop, s16 x2, s16 x2_ytop, 
                              s16 max_z,
                              u16 window_min, u16 window_max, u8 ceil_color);

//void draw_lower_step(s16 x1, s16 x1_ybot, s16 nx1_ybot, s16 x2, s16 x2_ybot, s16 nx2_ybot, u16 window_min, u16 window_max, u8 lower_color, u8 floor_color);
void draw_lower_step(s16 x1, s16 x1_ybot, s16 nx1_ybot, s16 x2, s16 x2_ybot, s16 nx2_ybot, 
                     u16 inv_z1, u16 inv_z2,
                     u16 window_min, u16 window_max, u8 lower_color, u8 floor_col, s8 light_level);

void draw_floor_update_clip(s16 x1, s16 x1_ybot, s16 x2, s16 x2_ybot, 
                            s16 max_z,
                            u16 window_min, u16 window_max, u8 floor_color);


void draw_transparent_wall(s16 x1, s16 x1_ytop, s16 x1_ybot,
                            s16 x2, s16 x2_ytop, s16 x2_ybot,
                            u16 window_min, u16 window_max,
                            clip_buf* clipping_buffer,
                            u8 wall_col);
                            
void draw_masked(s16 x1, s16 x1_ytop, s16 x1_ybot,
                 s16 x2, s16 x2_ytop, s16 x2_ybot,
                 u16 window_min, u16 window_max,
                 clip_buf* clipping_buffer,
                 u8 wall_col);

void draw_forcefield(s16 x1, s16 x2,
                     u16 window_min, u16 window_max,
                     clip_buf* clipping_buffer,
                     u8 wall_col);


void clear_2d_buffers();
void init_2d_buffers();
void release_2d_buffers();
#endif