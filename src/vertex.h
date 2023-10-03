#ifndef VERTEX_H
#define VERTEX_H

#include <genesis.h>

typedef struct __attribute__((__packed__)) {
    s16 x, y;
} vertex;


typedef struct __attribute__((__packed__)) {
    fix32 x, y;
} vertex_f32;

#endif