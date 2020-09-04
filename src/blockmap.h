#ifndef BLOCKMAP_H
#define BLOCKMAP_H

#include <genesis.h>

typedef struct {
    s16 x_origin;
    s16 y_origin;
    u16 num_columns;
    u16 num_rows;
    u16 num_offsets;
    u16 offsets_plus_table[];
} blockmap;

#endif