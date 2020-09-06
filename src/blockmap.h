#ifndef BLOCKMAP_H
#define BLOCKMAP_H

#include <genesis.h>

#define BLOCKMAP_CELL_SIZE 128

typedef struct {
    s16 x_origin;
    s16 y_origin;
    u16 num_columns;
    u16 num_rows;
    u16 num_offsets;
    const u16 offsets_plus_table[];
} blockmap;

#endif