#include <genesis.h>
#include "colors.h"
#include "portal_map.h"
#include "vertex.h"


    //
static const s16 sectors[35] = {
    0, 0, 12,  -80<<4, 216<<4, BLUE_IDX, LIGHT_BLUE_IDX,
    13, 12, 4, -80<<4, 216<<4, BLUE_IDX, LIGHT_BLUE_IDX,
    18, 16, 4, -80<<4, 216<<4, BLUE_IDX, LIGHT_BLUE_IDX,
    23, 20, 4, -80<<4, 216<<4, BLUE_IDX, LIGHT_BLUE_IDX,
    28, 24, 4, -80<<4, 216<<4, BLUE_IDX, LIGHT_BLUE_IDX,
};

static const u16 walls[33] ={
    1, 2, 3, 4, 7, 8, 9, 10, 11, 0, 5, 6, 1, 
    5, 12, 13, 6, 5, 
    2, 15, 16, 3, 2, 
    7, 17, 18, 8, 7, 
    10, 19, 20, 11, 10, 
};
static const s16 portals[28] = {
    -1, 2, -1, -1, 3, -1, -1, 4, -1, -1, 1, -1, 
    -1, -1, -1, 0, 
    -1, -1, -1, 0, 
    -1, -1, -1, 0, 
    -1, -1, -1, 0, 
};

static const wall_col wall_colors[28] = {
    {.mid_col = 0x55},
    {.upper_col = 0x55, .lower_col = 0x55},
    {.mid_col = 0x55},
    {.mid_col = 0x55},
    {.upper_col = 0x55, .lower_col = 0x55},
    {.mid_col = 0x55},
    {.mid_col = 0x55},
    {.upper_col = 0x55, .lower_col = 0x55},
    {.mid_col = 0x55},
    {.mid_col = 0x55},
    {.upper_col = 0x55, .lower_col = 0x55},
    {.mid_col = 0x55},
    {.mid_col = 0x55},
    {.mid_col = 0x55},
    {.mid_col = 0x55},
    {.upper_col = 0x55, .lower_col = 0x55},
    {.mid_col = 0x55},
    {.mid_col = 0x55},
    {.mid_col = 0x55},
    {.upper_col = 0x55, .lower_col = 0x55},
    {.mid_col = 0x55},
    {.mid_col = 0x55},
    {.mid_col = 0x55},
    {.upper_col = 0x55, .lower_col = 0x55},
    {.mid_col = 0x55},
    {.mid_col = 0x55},
    {.mid_col = 0x55},
    {.upper_col = 0x55, .lower_col = 0x55},
};

#define VERT(x1,y1) { .x = (x1 * 6), .y = ((-y1)*6) }
//#define VERT(x1,y1) { .x = (3*x1), .y = (4*y1)}
 //(x1 * 30), .y = ((110-y1)*40) }

static const vertex vertexes[22] = {
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

const portal_map editor_test_map = {
    .num_sectors = 5,
    .num_walls = 28,
    .num_verts = 22,
    .sectors = sectors,
    .walls = walls,
    .portals = portals,
    .vertexes = vertexes,
    .wall_colors = wall_colors,
    .wall_norm_quadrants = NULL
};
