#ifndef TEXTURE_H
#define TEXTURE_H

#include <genesis.h>

void draw_texture_vertical_line(s16 unclipped_y0, s16 y0, s16 unclipped_y1, s16 y1, u8* col_ptr, u16* tex_column);

#define TEX_HEIGHT 32
extern const u32 dark_texture[32*32];
extern const u32 mid_texture[32*32];
extern const u32 light_texture[32*32];

#endif