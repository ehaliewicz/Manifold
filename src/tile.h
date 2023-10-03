#ifndef TILE_H
#define TILE_H


typedef union {
    u8 bytes[8][4];
    u32 rows[8];
} tile;

typedef struct {
    u8 bytes[4*8]; // 4 columns 8 rows tall each
} two_pix_col_tile;

#endif