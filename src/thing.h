#ifndef THING_H
#define THING_H

#include <genesis.h>

typedef struct {
    s16 x;
    s16 y;
    u16 angle;
    u16 type;
    u16 flags;
} thing;

#endif