#ifndef LINEDEF_H
#define LINEDEF_H
#include <genesis.h>

typedef struct {
    u16 v1, v2;
    u16 flags;
    u16 line_type;
    u16 sector_tag;
    u16 right_sidedef;
    u16 left_sidedef;
} linedef;

#endif