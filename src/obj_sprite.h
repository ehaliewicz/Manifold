#ifndef OBJ_SPRITE_H
#define OBJ_SPRITE_H

#include "genesis.h"
#include "tile.h"

// 10 bytes
typedef struct  __attribute__((__packed__))  {
    const u16 num_spans;
    const u16* spans; // use words here so they can be pre-scaled to word indexes into scaled_sprite_runlengths
    const u8* texels;
} column;


// 642 bytes 
typedef struct  __attribute__((__packed__)) {
    // 2 bytes
    const u16 num_columns;
    // 640 bytes
    const column columns[64];
} rle_sprite;


void render_object_to_tile_column(u32 tex_per_pix, u32* tex_per_pix_table_ptr, s16 start_y, u16 min_y, u16 max_y, const column* col, u16* tile_buf_ptr);
u16 render_object_to_sprite(s16 left_x,s16 top_y, u16 scaled_size, const rle_sprite* obj);

void obj_sprite_init(u16 free_tile_loc);
const u32 scaled_sprite_run_lengths[64*513];
const u32 scaled_sprite_texel_per_pixel_lut[513];

const u16* const sprite_scale_coefficients_pointer_lut[513];

#endif