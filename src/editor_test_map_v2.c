#include <genesis.h>
#include "colors.h"
#include "portal_map.h"
#include "vertex.h"

static const s16 sectors[35] ={
    0, 0, 12, -80, 216, 1, 1
    13, 12, 4, -40, 175, 1, 1
    18, 16, 4, -40, 175, 1, 1
    23, 20, 4, -40, 175, 1, 1
    28, 24, 4, -40, 175, 1, 1
};

static const u16 walls[33] ={
    1, 2, 3, 4, 7, 8, 9, 10, 11, 0, 5, 6, 1, 
    5, 12, 13, 6, 5, 
    2, 15, 16, 3, 2, 
    7, 17, 18, 8, 7, 
    10, 19, 20, 11, 10, 
};
static const s16 portals[28] ={
    -1, 2, -1, -1, 3, -1, -1, 4, -1, -1, 1, -1, 
    -1, -1, -1, 0, 
    -1, -1, -1, 0, 
    -1, -1, -1, 0, 
    -1, -1, -1, 0, 
};

static const wall_col wall_colors[28] ={
    {.mid_col = 1},
    {.upper_col = 1, lower_col = 1},
    {.mid_col = 1},
    {.mid_col = 1},
    {.upper_col = 1, lower_col = 1},
    {.mid_col = 1},
    {.mid_col = 1},
    {.upper_col = 1, lower_col = 1},
    {.mid_col = 1},
    {.mid_col = 1},
    {.upper_col = 1, lower_col = 1},
    {.mid_col = 1},
    {.mid_col = 1},
    {.mid_col = 1},
    {.mid_col = 1},
    {.upper_col = 1, lower_col = 1},
    {.mid_col = 1},
    {.mid_col = 1},
    {.mid_col = 1},
    {.upper_col = 1, lower_col = 1},
    {.mid_col = 1},
    {.mid_col = 1},
    {.mid_col = 1},
    {.upper_col = 1, lower_col = 1},
    {.mid_col = 1},
    {.mid_col = 1},
    {.mid_col = 1},
    {.upper_col = 1, lower_col = 1},
};

VERT(x1,y1) { .x = (x1 * 6), .y = (y1 * 6) } 
static const vertex vertexes[22] ={
    VERT(270,140),
    VERT(430,140),
    VERT(430,190),
    VERT(430,230),
    VERT(430,280),
    VERT(320,140),
    VERT(380,140),
    VERT(380,280),
    VERT(320,280),
    VERT(270,280),
    VERT(270,230),
    VERT(270,190),
    VERT(310,100),
    VERT(390,100),
    VERT(0,0),
    VERT(470,180),
    VERT(470,240),
    VERT(390,320),
    VERT(310,320),
    VERT(230,240),
    VERT(230,180),
    VERT(0,0),
};
const portal_map placeholder_name {
    .num_sectors = 5,
    .num_walls = 28,
    .num_verts = 22,
    .sectors = sectors,
    .walls = walls,
    .portals = portals,
    .vertexes = vertexes,
    .wall_colors = wall_colors,
    .wall_normal_quadrants = NULL
};
