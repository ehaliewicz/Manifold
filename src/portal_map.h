
#ifndef PORTAL_MAP_H
#define PORTAL_MAP_H

#include <genesis.h>
#include "vertex.h"


typedef struct {
    int num_sectors;
    int num_walls;
    int num_verts;
    s16* sectors;
    u16* walls;
    s16* portals;
    const vertex* vertexes;
} portal_map;


s16 sector_wall_offset(s16 sector_idx, portal_map* mp);

s16 sector_num_walls(s16 sector_idx, portal_map* mp);

s16 sector_floor_height(s16 sector_idx, portal_map* mp);

s16 sector_ceil_height(s16 sector_idx, portal_map* mp);

#endif