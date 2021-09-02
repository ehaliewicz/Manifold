#ifndef TEXTURE_H
#define TEXTURE_H

#include <genesis.h>

void draw_texture_vertical_line(s16 unclipped_y0, s16 y0, s16 unclipped_y1, s16 y1, u8* col_ptr, u16* tex_column);

//#define TEX_HEIGHT 128
//#define TEX_HEIGHT_SHIFT 7

//#define TEX_WIDTH 64
//#define TEX_WIDTH_SHIFT 6
#define TEX_WIDTH 32
#define TEX_WIDTH_SHIFT 5

#define TEX_HEIGHT 64
#define TEX_HEIGHT_SHIFT 6
//#define TEX_WIDTH 32
//#define TEX_WIDTH_SHIFT 5

extern u16* dark_texture;
extern u16* mid_texture;
extern u16* light_texture;
void tick_texture();


typedef struct {
    s32 left_u;    // 16.16
    s16 left_z;
    s32 right_u;   // 16.16
    s16 right_z;
    s32 du_over_dz;
    u8 needs_perspective;
} texmap_info;
#endif