
#ifndef PORTAL_MAP_H
#define PORTAL_MAP_H

#include <genesis.h>
#include "vertex.h"
#include "vis_range.h"


typedef struct {
    const int num_sectors;
    const int num_walls;
    const int num_verts;
    const s16* sectors;
    const u16* walls;
    const s16* portals;
    const vertex* vertexes;
    const vis_range wall_vis_ranges;
} portal_map;


s16 sector_wall_offset(s16 sector_idx, portal_map* mp);

s16 sector_num_walls(s16 sector_idx, portal_map* mp);

s16 sector_floor_height(s16 sector_idx, portal_map* mp);

s16 sector_ceil_height(s16 sector_idx, portal_map* mp);

#endif
