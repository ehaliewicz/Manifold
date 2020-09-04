#ifndef SEG_H
#define SEG_H

#include <genesis.h>

typedef struct {
    u16 begin_vert, end_vert;
    s16 angle;
    u16 linedef;
    s16 direction;
    s16 offset;
} seg;

#endif