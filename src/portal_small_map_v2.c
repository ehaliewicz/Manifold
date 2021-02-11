#include <genesis.h>
#include "colors.h"
#include "portal_map.h"
#include "vertex.h"

#define NUM_SECTORS 10

#define NUM_VERTS 28

#define NUM_WALLS 52

const s16 sectors[SECTOR_SIZE*NUM_SECTORS] = {
    // sector 0   wall_offset, portal_offset, num_walls, floor_height, ceil_height
    0,  0, 6, -80, 216, BLUE_BLACK_IDX, LIGHT_BLUE_IDX,
    // sector 1
    7,  6, 6, -80, 216, DARK_BLUE_IDX, LIGHT_BLUE_IDX,
    // sector 2
    14, 12, 6, -80, 216, DARK_BLUE_IDX, LIGHT_BLUE_IDX,
    // sector 3
    21, 18, 6, -80, 216, DARK_BLUE_IDX, LIGHT_BLUE_IDX,
    // sector 4
    28, 24, 6, -80, 216, DARK_BLUE_IDX, LIGHT_BLUE_IDX,
    // sector 5
    35, 30, 6, -80, 216, DARK_BLUE_IDX, LIGHT_BLUE_IDX,
    // sector 6
    42, 36, 4, -80, 216, DARK_BLUE_IDX, LIGHT_BLUE_IDX,
    // sector 7
    47, 40, 4, -40, 156, DARK_RED_IDX, LIGHT_RED_IDX,
    // sector 8
    52, 44, 4, -40, 156, DARK_RED_IDX, LIGHT_RED_IDX,
    // sector 9
    57, 48, 4, -80, 216, DARK_BLUE_IDX, LIGHT_BLUE_IDX
};



const u16 walls[NUM_WALLS+NUM_SECTORS] = {
    // sector 0 walls 
    0, 1, 9, 8, 7, 6, 0,
    // sector 1 walls
    1, 2, 12, 11, 10, 9, 1,
    // sector 2 walls
    2, 3, 15, 14, 13, 12, 2,
    // sector 3 walls
    3, 4, 18, 17, 16, 15, 3,
    // sector 4 walls
    4, 5, 21, 20, 19, 18, 4,
    // sector 5 walls
    5, 0, 6, 23, 22, 21, 5,
    // sector 6 walls
    7, 8, 16, 17, 7,
    // sector 7 walls
    25, 13, 14, 26, 25,
    // sector 8 walls
    23, 24, 27, 22, 23,
    // sector 9 walls
    10, 11, 19, 20, 10
};

const s16 portals[NUM_WALLS] = {
    // sector 0 portals
    -1, 1, -1, 6, -1, 5,
    // sector 1 portals
    -1, 2, -1, 9, -1, 0,
    // sector 2 portals
    -1, 3, -1, 7, -1, 1,
    // sector 3 portals
    -1, 4, -1, 6, -1, 2,
    // sector 4 portals
    -1, 5, -1, 9, -1, 3,
    // sector 5 portals
    -1, 0, -1, 8, -1, 4,
    // sector 6 portals
    0, -1, 3, -1,
    // sector 7 portals
    -1, 2, -1, -1,
    // sector 8 portals
    -1, -1, -1, 5,
    // sector 9 portals
    1, -1, 4, -1
};

const wall_col wall_colors[NUM_WALLS] = {
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = MED_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = MED_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = MED_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = MED_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = MED_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = MED_GREEN_IDX},
    {.mid_col = MED_GREEN_IDX},
    {.mid_col = MED_GREEN_IDX},
    {.mid_col = MED_GREEN_IDX},
    {.mid_col = MED_GREEN_IDX},
    {.mid_col = MED_GREEN_IDX},
    {.mid_col = MED_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = MED_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = MED_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = LIGHT_GREEN_IDX},
    {.mid_col = LIGHT_GREEN_IDX},
    {.mid_col = TRANSPARENT_IDX},
    {.mid_col = TRANSPARENT_IDX},
    {.mid_col = TRANSPARENT_IDX},
    {.mid_col = TRANSPARENT_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
    {.mid_col = DARK_GREEN_IDX},
};

const u8 wall_normal_quadrants[NUM_WALLS] = {
    // sector 0
    QUADRANT_3,
    QUADRANT_1,
    FACING_UP,
    FACING_UP,
    FACING_UP,
    QUADRANT_0,

    // sector 1
    QUADRANT_2,
    QUADRANT_1,
    QUADRANT_0,
    QUADRANT_0,
    QUADRANT_0,
    QUADRANT_3,

    // sector 2
    QUADRANT_1,
    QUADRANT_0,
    QUADRANT_3,
    QUADRANT_3,
    QUADRANT_3,
    QUADRANT_3,

    // sector 3
    FACING_UP,
    QUADRANT_3,
    FACING_DOWN,
    FACING_DOWN,
    FACING_DOWN,
    QUADRANT_2,

    // sector 4
    QUADRANT_0,
    FACING_DOWN,
    QUADRANT_2,
    QUADRANT_2,
    QUADRANT_2,
    QUADRANT_1,

    // sector 5
    QUADRANT_3,
    QUADRANT_2,
    QUADRANT_1,
    QUADRANT_1,
    QUADRANT_1,
    FACING_UP,

    // sector 6
    FACING_DOWN,
    FACING_LEFT,
    FACING_UP,
    FACING_RIGHT,

    // sector 7
    FACING_DOWN,
    QUADRANT_1,
    FACING_UP,
    QUADRANT_3,

    // sector 8
    FACING_DOWN,
    QUADRANT_1,
    FACING_UP,
    QUADRANT_3,

    // sector 9
    QUADRANT_2,
    QUADRANT_1,
    QUADRANT_0,
    QUADRANT_3

};

#define VERT(y1,x1) { .x = (x1 * 30), .y = ((110-y1)*40) }

const vertex vertexes[NUM_VERTS] = {
    VERT(14,46),			      
    VERT(13,90),
    VERT(29,110),
    VERT(46,96),
    VERT(46,48),
    VERT(27,35),
    VERT(18,55),
    VERT(18,67),
    VERT(18,76),
    VERT(18,84),
    VERT(21,87),
    VERT(25,91),
    VERT(30,97),
    VERT(32,95),
    VERT(38,91),
    VERT(41,88),
    VERT(41,76),
    VERT(41,67),
    VERT(41,55),
    VERT(35,52),
    VERT(31,49),
    VERT(27,46),
    VERT(26,47),
    VERT(20,53),
    VERT(20,56),
    VERT(32,92),
    VERT(38,86),
    VERT(26,50),
};


const portal_map portal_level_1 = {
    .num_sectors = NUM_SECTORS,
    .num_walls = NUM_WALLS,
    .num_verts = NUM_VERTS,
    .sectors = sectors,
    .walls = walls,
    .portals = portals,
    .vertexes = vertexes,
    .wall_norm_quadrants = wall_normal_quadrants,
    .wall_colors = wall_colors
};
