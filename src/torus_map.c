/*
#include <genesis.h>
#include "colors.h"
#include "portal_map.h"
#include "vertex.h"

static const s16 sectors[32] ={
    0, 0, 4, 0<<4, 100<<4, 1, 1, 0,
    5, 4, 4, 0<<4, 100<<4, 1, 1, 0,
    10, 8, 4, 0<<4, 100<<4, 1, 1, 0,
    15, 12, 4, 0<<4, 100<<4, 1, 1, 0,
};

static const u16 walls[20] = {
    0, 1, 2, 3, 0, 
    1, 4, 5, 2, 1, 
    4, 6, 7, 5, 4, 
    6, 0, 3, 7, 6, 
};
static const s16 portals[16] ={
    -1, 1, -1, 3, 
    -1, 2, -1, 0, 
    -1, 3, -1, 1, 
    -1, 0, -1, 2, 
};

static const u8 wall_normal_quadrants[16] = {
    FACING_DOWN,
    QUADRANT_1,
    FACING_UP,
    QUADRANT_0,
    FACING_LEFT,
    QUADRANT_0,
    FACING_RIGHT,
    QUADRANT_3,
    FACING_UP,
    QUADRANT_3,
    FACING_DOWN,
    QUADRANT_2,
    FACING_RIGHT,
    QUADRANT_2,
    FACING_LEFT,
    QUADRANT_1,
};

static const wall_col wall_colors[16] ={
    {.mid_col = 0x11},
    {.upper_col = 0x11, .lower_col = 0x11},
    {.mid_col = 0x11},
    {.upper_col = 0x11, .lower_col = 0x11},
    {.mid_col = 0x11},
    {.upper_col = 0x11, .lower_col = 0x11},
    {.mid_col = 0x11},
    {.upper_col = 0x11, .lower_col = 0x11},
    {.mid_col = 0x11},
    {.upper_col = 0x11, .lower_col = 0x11},
    {.mid_col = 0x11},
    {.upper_col = 0x11, .lower_col = 0x11},
    {.mid_col = 0x11},
    {.upper_col = 0x11, .lower_col = 0x11},
    {.mid_col = 0x11},
    {.upper_col = 0x11, .lower_col = 0x11},
};


static u8 sector_types[3] = {
    NO_TYPE,
    NO_TYPE,
    NO_TYPE,
    NO_TYPE
};
static sector_param sector_params[4] = {
    {.light = 0},
    {.light = -1},
    {.light = 0},
    {.light = -1}
};

#define VERT(x1,y1) { .x = (x1 * 2), .y = ((-y1) * 2) } 
static const vertex vertexes[8] = {
    VERT(110,120),
    VERT(380,120),
    VERT(320,200),
    VERT(170,200),
    VERT(380,390),
    VERT(320,320),
    VERT(110,390),
    VERT(170,320),
};
const portal_map torus_map  = {
    .num_sectors = 4,
    .num_walls = 16,
    .num_verts = 8,
    .sectors = sectors,
    .walls = walls,
    .portals = portals,
    .vertexes = vertexes,
    .wall_colors = wall_colors,
    .wall_norm_quadrants = wall_normal_quadrants,
    .sector_types = sector_types,
    .sector_params = sector_params,
};
*/