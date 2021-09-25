#include <genesis.h>
#include "colors.h"
#include "portal_map.h"
#include "vertex.h"

static const s16 sectors[24] ={
    0, 0, 4, 0<<4, 100<<4, 1, 1, 0,
    5, 4, 4, 0<<4, 100<<4, 1, 1, 0,
    10, 8, 4, 0<<4, 100<<4, 1, 1, 0,
};

static const u16 walls[15] = {
    0, 1, 2, 3, 0, 
    1, 4, 5, 2, 1, 
    4, 6, 7, 5, 4, 
};
static const s16 portals[12] ={
    -1, 1, -1, -1, 
    -1, 2, -1, 0, 
    -1, -1, -1, 1, 
};

static const u8 wall_normal_quadrants[12] ={
    QUADRANT_2,
    QUADRANT_1,
    QUADRANT_0,
    QUADRANT_3,
    QUADRANT_2,
    QUADRANT_1,
    FACING_UP,
    QUADRANT_3,
    QUADRANT_3,
    QUADRANT_2,
    QUADRANT_1,
    QUADRANT_3,
};

static const wall_col wall_colors[12] ={
    {.mid_col = 0x11},
    {.upper_col = 0x11, .lower_col = 0x11},
    {.mid_col = 0x11},
    {.mid_col = 0x11},
    {.mid_col = 0x11},
    {.upper_col = 0x11, .lower_col = 0x11},
    {.mid_col = 0x11},
    {.upper_col = 0x11, .lower_col = 0x11},
    {.mid_col = 0x11},
    {.mid_col = 0x11},
    {.mid_col = 0x11},
    {.upper_col = 0x11, .lower_col = 0x11},
};

#define VERT(x1,y1) { .x = (x1 * 2), .y = ((-y1) * 2) } 
static const vertex vertexes[8] = {
    VERT(260,190),
    VERT(410,220),
    VERT(390,370),
    VERT(200,320),
    VERT(560,240),
    VERT(530,380),
    VERT(720,120),
    VERT(740,210),
};
const portal_map blahblah  = {
    .num_sectors = 3,
    .num_walls = 12,
    .num_verts = 8,
    .sectors = sectors,
    .walls = walls,
    .portals = portals,
    .vertexes = vertexes,
    .wall_colors = wall_colors,
    .wall_norm_quadrants = wall_normal_quadrants
};