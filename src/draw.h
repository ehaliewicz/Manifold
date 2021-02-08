#ifndef DRAW_H
#define DRAW_H

#include <genesis.h>
#include "vertex.h"

typedef enum {
    GAME_WIREFRAME = 1,
    GAME_SOLID = 2 
} draw_mode;

extern s16* raster_buffer_0;
extern s16* raster_buffer_1;


void bresenham_line(u8 x0, u8 x1, u8 y0, u8 y1, u16 win_min, u16 win_max, u8 col);

void bresenham_wall(s16 x1, s16 x1y, s16 x2, s16 x2y, u16 window_min, u8 window_max, s16* output);
void draw_native_vertical_line_unrolled(s16 y0, s16 y1, u8 col, u8* col_ptr);
void draw_between_raster_spans(s16* top, s16* bot, u16 startx, u16 endx, u8 col,
                               uint8_t update_top_clip, uint8_t update_bot_clip);

void draw_from_top_to_raster_span(s16* top, u16 startx, u16 endx, u8 col, u8 update_top_clip);
void draw_from_raster_span_to_bot(s16* bot, u16 startx, u16 endx, u8 col, u8 update_bot_clip);

void draw_wall(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 window_min, u16 window_max,
              u8 wall_col);
              
void draw_line_to_buffer(s16 x1, s16 x1_y, s16 x2, s16 x2_y, 
                         u16 window_min, u16 window_max,
                         s16* out_buffer);

void draw_top_step(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 window_min, u16 window_max,
              u8 col);

void draw_bot_step(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 window_min, u16 window_max,
              u8 col);


void clear_2d_buffers();
void init_2d_buffers();
void release_2d_buffers();
#endif