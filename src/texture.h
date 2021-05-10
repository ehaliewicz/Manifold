#ifndef TEXTURE_H
#define TEXTURE_H

#include <genesis.h>

void draw_texture_vertical_line(s16 unclipped_y0, s16 y0, s16 unclipped_y1, s16 y1, u8* col_ptr, u8 tex_column);
void init_textures();

#endif