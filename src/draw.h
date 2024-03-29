#ifndef DRAW_H
#define DRAW_H

#include <genesis.h>
#include "clip_buf.h"
#include "obj_sprite.h"
#include "object.h"
#include "texture.h"
#include "vertex.h"

#define WALLS_DIST_LIGHTING 
#define FLATS_DIST_LIGHTING


typedef struct {
    u8 x;
    u8 y0;
    u8 y1;
    u8 clip_y0;
    u8 clip_y1;
    Bitmap *bmp;
} col_params;

extern int debug_draw_cleared;


//void draw_native_vertical_line_unrolled(s16 y0, s16 y1, u8 col, u8* col_ptr);
void draw_native_double_vertical_line_unrolled(s16 y0, s16 y1, s16 y2, u32 full_col1, u32 full_col2, u8* col_ptr);
void draw_native_vertical_line_unrolled(s16 y0, s16 y1, u32 full_col,  u8* col_ptr);
void copy_2d_buffer(u16 left, u16 right, clip_buf* dest);
void draw_native_vertical_transparent_line_unrolled(s16 y0, s16 y1, u8 col, u8* col_ptr, u8 odd);


typedef struct {
    u8 needs_lighting;
    u8 mid_y;
    u8 dark_y;
    u8 fade_y;
    u32 light_color;
    u32 mid_color;
    u32 dark_color;
} light_params;

void clear_light_cache();
void cache_floor_light_params(s16 rel_floor_height, u8 floor_col, s8 light_level, light_params* params);
void cache_ceil_light_params(s16 rel_ceil_height, u8 ceil_col, s8 light_level, light_params* params);

void draw_solid_color_wall(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 inv_z1, u16 inv_z2,
              u16 window_min, u16 window_max,  s8 light_level,
              u8 wall_color, 
              light_params* floor_params, light_params* ceil_params);

void draw_wall(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 z1,     u16 z2,
              u16 inv_z1, u16 inv_z2,
              u16 window_min, u16 window_max, s8 light_level, 
              texmap_params* tmap_info, 
              light_params* floor_params, light_params* ceil_params);
              

void draw_wireframe_wall(
                   s16 x1, s16 x1_ytop, s16 x1_ybot, 
                   s16 x2, s16 x2_ytop, s16 x2_ybot,
                   u16 window_min, u16 window_max);
void draw_wireframe_lower_step(
                   s16 x1, s16 x1_ytop, s16 x1_ybot, 
                   s16 x2, s16 x2_ytop, s16 x2_ybot,
                   u16 window_min, u16 window_max);


void draw_top_pegged_wall(s16 x1, s16 x1_ytop, s16 x1_ybot,
                          s16 x2, s16 x2_ytop, s16 x2_ybot,
                          u16 z1_12_4,     u16 z2_12_4,
                          u16 inv_z1, u16 inv_z2,
                          u16 window_min, u16 window_max, s8 light_level,
                          texmap_params *tmap_info, 
                          light_params* floor_params, light_params* ceil_params,
                          s16 x1_pegged_top, s16 x2_pegged_top);

void draw_bot_pegged_wall(s16 x1, s16 x1_ytop, s16 x1_ybot,
                          s16 x2, s16 x2_ytop, s16 x2_ybot,
                          u16 z1_12_4,     u16 z2_12_4,
                          u16 inv_z1, u16 inv_z2,
                          u16 window_min, u16 window_max, s8 light_level,
                          texmap_params* tmap_info, 
                          light_params* floor_params, light_params* ceil_params,
                          s16 x1_pegged_top, s16 x2_pegged_top);

void draw_top_pegged_textured_upper_step(s16 x1, s16 x1_ytop, s16 nx1_ytop, s16 x2, s16 x2_ytop, s16 nx2_ytop,
                                         u16 z1_12_4,     u16 z2_12_4,
                                         u16 inv_z1, u16 inv_z2,
                                         u16 window_min, u16 window_max, s8 light_level,
                                         texmap_params* tmap_info, light_params* params,
                                         s16 x1_pegged_top, s16 x2_pegged_top);

void draw_upper_step(s16 x1, s16 x1_ytop, s16 nx1_ytop, s16 x2, s16 x2_ytop, s16 nx2_ytop, 
                     u16 inv_z1, u16 inv_z2,
                     u16 window_min, u16 window_max, u8 upper_color, s8 light_level, light_params* params);

void draw_ceiling_update_clip(s16 x1, s16 x1_ytop, s16 x2, s16 x2_ytop, 
                              u16 far_inv_z,
                              u16 window_min, u16 window_max, light_params* params);


void draw_bottom_pegged_textured_lower_step(s16 x1, s16 x1_ybot, s16 nx1_ybot, s16 x2, s16 x2_ybot, s16 nx2_ybot, 
                                            u16 z1_12_4,     u16 z2_12_4,
                                            u16 inv_z1, u16 inv_z2,
                                            u16 window_min, u16 window_max, s8 light_level,
                                            texmap_params* tmap_info, light_params* params,
                                            s16 x1_pegged_bot, s16 x2_pegged_bot);

void draw_lower_step(s16 x1, s16 x1_ybot, s16 nx1_ybot, s16 x2, s16 x2_ybot, s16 nx2_ybot, 
                     u16 inv_z1, u16 inv_z2,
                     u16 window_min, u16 window_max, u8 lower_color, s8 light_level, light_params* params);

void draw_floor_update_clip(s16 x1, s16 x1_ybot, s16 x2, s16 x2_ybot, 
                            u16 far_inv_z,
                            u16 window_min, u16 window_max, light_params* params);


void init_sprite_draw_cache();


extern obj_type drawn_to_center_cols;
extern object_link sprite_on_center_col;
extern u16 center_object_sector;

// reset whether a sprite was drawn to the center of the screen
void reset_sprite_hit_info(); 

void draw_rle_sprite(s16 x1, s16 x2, s16 ytop, s16 ybot,
                 u16 window_min, u16 window_max,
                 clip_buf* clipping_buffer,
                 const rle_sprite* obj, 
                 object_link obj_link, obj_type obj_type, u16 sector);

void draw_forcefield(s16 x1, s16 x2,
                     u16 window_min, u16 window_max,
                     clip_buf* clipping_buffer,
                     u8 wall_col);


void clear_2d_buffers();
void init_2d_buffers();
void release_2d_buffers();




#endif