#ifndef FONT_H
#define FONT_H

#include <genesis.h>
#include "common.h"

#define NUM_CHARS 96
#define PAIRS_IN_TILE 4

typedef union {
    u8 bytes[8][4];
    u32 rows[8];
} tile;

typedef struct {
    char chr;
    int width; // width in 2-pixel pairs (bytes), always a minimum of two, so we can write words at once
    tile bitmap;
} char_entry;

extern char_entry charmap[96];


void vwf_init();
int vwf_render_tiles(char* string, int len, tile* tiles, int num_tiles);
int vwf_count_tiles(char* string, int len);
void vwf_cleanup();

#endif