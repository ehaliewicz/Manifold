#ifndef OBJECT_H
#define OBJECT_H

#include <genesis.h>

typedef struct {
    fix32 x;
    fix32 y;
    fix32 z;
    u16 ang;
    u16 cur_sector;
} object_pos;


typedef struct {
    uint8_t object_type;
    uint8_t sprite;
} object_template;


typedef struct {
    object_template template;
    object_pos pos;
} object;

#endif