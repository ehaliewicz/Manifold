#ifndef SECTOR_H
#define SECTOR_H

#include <genesis.h>

typedef struct {
    s16 floor_height, ceil_height;
    char floor_texture[8], ceil_texture[8];
    s16 light_level;
    u16 sector_special, sector_tag;
} sector;

#endif