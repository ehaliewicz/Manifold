#ifndef SIDEDEF_H
#define SIDEDEF_H

#include <genesis.h>

typedef struct {
    s16 x_off, y_off;
    char upper_texture[8];
    char lower_texture[8];
    char middle_texture[8];
    u16 sector_ref;
} sidedef;

#endif