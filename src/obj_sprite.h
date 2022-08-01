#ifndef OBJ_SPRITE_H
#define OBJ_SPRITE_H

#include "genesis.h"
#include "tile.h"

typedef struct {
    u16 num_spans;
    u8* spans;
    u16* texel_runs;
} column;

typedef struct {
    u16 num_columns;
    column columns[64];
} rle_object;


void render_object_to_tile_column(u32 tex_per_pix, u32* tex_per_pix_table_ptr, s16 start_y, u16 min_y, u16 max_y, column* col, tile* tile_buf);
void render_object_to_sprite(s16 left_x,s16 top_y, u16 scaled_size, rle_object* obj, tile* tile_buf);

#endif