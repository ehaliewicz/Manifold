#ifndef DRAW_H
#define DRAW_H

#include <genesis.h>
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

void draw_tex_column(col_params params);
//void draw_native_vertical_line_unrolled(s16 y0, s16 y1, u8 col, u8* col_ptr);
void draw_native_vertical_line_unrolled(s16 x, s16 y0, s16 y1, u8 col);

void draw_between_raster_spans_update_top_clip(s16* top, s16* bot, u16 startx, u16 endx, u8 col);
void draw_between_raster_spans_update_bottom_clip(s16* top, s16* bot, u16 startx, u16 endx, u8 col);

void draw_from_top_to_raster_span_no_update_clip(s16* top, u16 startx, u16 endx, u8 col);
void draw_from_top_to_raster_span_update_clip(s16* top, u16 startx, u16 endx, u8 col);
void draw_from_raster_span_to_bot_no_update_clip(s16* bot, u16 startx, u16 endx, u8 col);
void draw_from_raster_span_to_bot_update_clip(s16* bot, u16 startx, u16 endx, u8 col);

void draw_fix_line_to_buffer(s16 x1, s16 x1_y, s16 x2, s16 x2_y, 
                             u16 window_min, u16 window_max,
                             s16* out_buffer);

void draw_line_to_buffer(s16 x1, s16 x1_y, s16 x2, s16 x2_y, 
                         u16 window_min, u16 window_max,
                         s16* out_buffer); 
                         
void draw_wall(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 window_min, u16 window_max,
              u8 ceil_col, u8 wall_col, u8 floor_col);

void draw_upper_step(s16 x1, s16 x1_ytop, s16 nx1_ytop, s16 x2, s16 x2_ytop, s16 nx2_ytop, u16 window_min, u16 window_max, u8 upper_color, u8 ceil_color);
void draw_ceiling_update_clip(s16 x1, s16 x1_ytop, s16 x2, s16 x2_ytop, u16 window_min, u16 window_max, u8 ceil_color);
void draw_lower_step(s16 x1, s16 x1_ybot, s16 nx1_ybot, s16 x2, s16 x2_ybot, s16 nx2_ybot, u16 window_min, u16 window_max, u8 lower_color, u8 floor_color);
void draw_floor_update_clip(s16 x1, s16 x1_ybot, s16 x2, s16 x2_ybot, u16 window_min, u16 window_max, u8 floor_color);


void clear_2d_buffers();
void init_2d_buffers();
void release_2d_buffers();
#endif