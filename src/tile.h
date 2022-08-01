#ifndef TILE_H
#define TILE_H


typedef union {
    u8 bytes[8][4];
    u32 rows[8];
} tile;

#endif