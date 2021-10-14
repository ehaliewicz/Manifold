
#include <genesis.h>
#include "colors.h"
#include "portal_map.h"
#include "vertex.h"

static s16 sectors[4*14] ={
    0, 0, 4, 0,
    5, 4, 4, 0,
    10, 8, 4,0,
    15, 12, 6, 0,
    22, 18, 4, 0,
    27, 22, 4, 0,
    32, 26, 4, 0,
    37, 30, 4, 0,
    42, 34, 4, 0,
    47, 38, 4, 0,
    52, 42, 5, 0,
    58, 47, 5, 0,
    64, 52, 6, 0,
    71, 58, 4, 0,
};

static u16 walls[76] = {
    //  0  0
    0, 1, 2, 3, 0, 
    //  1  5
    1, 4, 5, 2, 1, 
    //  2 10
    4, 6, 7, 5, 4, 
    //  3 15
    6, 8, 9, 10, 11, 7, 6, 

    //  4 22
    9, 12, 13, 10, 9, 
    //  5 27
    12, 14, 15, 13, 12, 
    //  6 32
    14, 16, 17, 15, 14, 
    //  7 37
    16, 18, 19, 17, 16, 
    //  8 42
    18, 20, 21, 19, 18, 
    //  9 47
    11, 10, 13, 22, 11, 
    // 10 52
    22, 13, 15, 17, 23, 22, 
    // 11 58
    8, 24, 14, 12, 9, 8, 
    // 12 64
    24, 25, 26, 27, 16, 14, 24, 
    // 13 71
    26, 28, 29, 27, 26, 
};

static s16 portals[62] = {
    // 0
    -1, 1, -1, -1, 
    // 1
    -1, 2, -1, 0, 
    // 2
    -1, 3, -1, 1, 
    // 3
    -1, 11, 4, 9, -1, 2, 
    // 4
    11, 5, 9, 3, 
    // 5
    11, 6, 10, 4, 
    // 6
    12, 7, 10, 5, 
    // 7
    -1, 8, -1, 6, 
    // 8
    -1, -1, -1, 7, 
    // 9
    3, 4, 10, -1, 
    // 10
    9, 5, 6, -1, -1, 
    // 11
    -1, 12, 5, 4, 3, 
    // 12
    -1, -1, 13, -1, 6, 11, 
    // 13
    -1, -1, -1, 12, 
};

static u8 wall_normal_quadrants[62] ={
    FACING_DOWN,
    QUADRANT_1,
    QUADRANT_0,
    FACING_RIGHT,
    QUADRANT_2,
    QUADRANT_1,
    QUADRANT_0,
    QUADRANT_3,
    QUADRANT_2,
    FACING_UP,
    QUADRANT_0,
    QUADRANT_3,
    QUADRANT_2,
    FACING_UP,
    FACING_UP,
    QUADRANT_0,
    QUADRANT_3,
    FACING_DOWN,
    QUADRANT_1,
    QUADRANT_1,
    QUADRANT_3,
    FACING_DOWN,
    QUADRANT_2,
    QUADRANT_1,
    QUADRANT_0,
    QUADRANT_3,
    QUADRANT_1,
    FACING_UP,
    QUADRANT_3,
    QUADRANT_3,
    FACING_LEFT,
    FACING_UP,
    FACING_RIGHT,
    FACING_DOWN,
    FACING_LEFT,
    FACING_UP,
    FACING_RIGHT,
    FACING_DOWN,
    QUADRANT_2,
    QUADRANT_1,
    QUADRANT_1,
    FACING_RIGHT,
    QUADRANT_3,
    QUADRANT_2,
    QUADRANT_1,
    FACING_UP,
    FACING_RIGHT,
    QUADRANT_2,
    QUADRANT_1,
    QUADRANT_0,
    QUADRANT_3,
    FACING_DOWN,
    QUADRANT_2,
    QUADRANT_1,
    QUADRANT_1,
    FACING_UP,
    QUADRANT_3,
    QUADRANT_3,
    FACING_LEFT,
    FACING_UP,
    FACING_RIGHT,
    QUADRANT_3,
};


// texture_idx, high_color, low_color, wall_color (deprecated)
static u8 wall_colors[62 * WALL_COLOR_NUM_PARAMS] = {
    0,0,0,0,
    0,1,1,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,1,1,0,
    0,0,0,0,
    0,1,1,0,
    0,0,0,0,
    0,1,1,0,
    0,0,0,0,
    0,1,1,0,
    0,0,0,0,
    0,1,1,0,
    0,1,1,0,
    0,1,1,0,
    0,0,0,0,
    0,3,1,0,
    0,1,1,0,
    0,1,1,0,
    0,1,1,0,
    0,1,1,0,
    0,1,1,0,
    0,1,1,0,
    0,1,1,0,
    0,1,1,0,
    0,1,1,0,
    0,3,1,0,
    0,1,1,0,
    0,1,1,0,
    0,0,0,0,
    0,1,1,0,
    0,0,0,0,
    0,1,1,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,1,1,0,
    0,9,8,0,
    0,1,8,0,
    0,1,1,0,
    0,0,0,0,
    0,1,1,0,
    0,1,8,0,
    0,1,8,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,1,1,0,
    0,1,8,0,
    0,1,8,0,
    0,3,8,0,
    0,0,0,0,
    0,0,0,0,
    0,1,3,0,
    0,0,0,0,
    0,1,8,0,
    0,1,1,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,1,1,0,
};

#define VERT(x1,y1) { .x = (x1 * 2), .y = ((-y1) * 2) } 
static vertex vertexes[30] = {
    VERT(140,120),
    VERT(270,120),
    VERT(240,200),
    VERT(140,170),
    VERT(340,150),
    VERT(310,210),
    VERT(390,220),
    VERT(330,220),
    VERT(460,270),
    VERT(380,270),
    VERT(340,270),
    VERT(270,240),
    VERT(350,310),
    VERT(310,320),
    VERT(430,350),
    VERT(380,360),
    VERT(380,420),
    VERT(330,420),
    VERT(380,460),
    VERT(330,460),
    VERT(380,560),
    VERT(330,560),
    VERT(270,330),
    VERT(270,420),
    VERT(480,290),
    VERT(500,370),
    VERT(490,410),
    VERT(450,420),
    VERT(490,460),
    VERT(450,460),
};

static u8 sector_types[14] = {
0,0,0,0,0,0,0,1,1,0,0,0,0,1,};

static s16 sector_params[NUM_SECTOR_PARAMS*14] = {
    // light orig_height, ticks_left, state, floor_height, ceil_height, floor_color, ceil_color
    0, 0<<4, 0, 0, 0<<4, 100<<4, 3, 3,
    0, 0<<4, 0, 0, 0<<4, 100<<4, 3, 3, 
    0, 0<<4, 0, 0, 0<<4, 100<<4, 3, 3, 
    0, 0<<4, 0, 0, 0<<4, 240<<4, 3, 3, 
    0, 0<<4, 0, 0, 0<<4, 240<<4, 3, 3,
    0, 0<<4, 0, 0, 0<<4, 240<<4, 3, 3,
    0, 0<<4, 0, 0, 0<<4, 240<<4, 3, 3,
    -2, 0<<4, 0, 0, 0<<4, 100<<4, 3, 3,
    -2, 0<<4, 0, 0, 0<<4, 100<<4, 3, 3,
    0, 0<<4, 0, 0, -30<<4, 240<<4, 1, 3,
    0, 0<<4, 0, 0, -30<<4, 240<<4, 1, 3,
    0, 0<<4, 0, 0, -30<<4, 240<<4, 1, 3,
    0, 0<<4, 0, 0, -30<<4, 240<<4, 1, 3,
    -2, 0<<4, 0, 0, 80<<4, 240<<4, 3, 3,
};

// pvs is an index for each sector, plus length
// raw pvs is wall lists

// TODO
// sector 2
// sector 3
// sector 4
// sector 5
// sector 6
// sector 7
// sector 8
// sector 9
// sector 10
// sector 11
// sector 12
// sector 13


static s16 pvs[14 * 2] = {
     0, 10,
    40, 14,
   100, 14,
   162, 16,
   233,  2,
     0, 10,
     0, 10,
     0, 10,
     0, 10,
     0, 10,
     0, 10,
     0, 10,
     0, 10,
     0, 10,
};

static u16 raw_pvs[512] = {
    // sector 0 pvs
    // sector index, num walls, N*2 wall + portal refs
     0, 3,  0,2,3,
     1, 2,  5,7,
     2, 3,  10,11,12,
     3, 2,  15,16,
     4, 1,  22,
    11, 2,  58,60,
     5, 1,  28,
     6, 1,  32,
    12, 3,  64,65,66,
    13, 2,  71,72,

    // sector 1 pvs, 40
     1, 2,  5,7,
     0, 3,  2,3,0,
     2, 3,  10,11,12,
     3, 3,  15,16,18,
     9, 1,  48,
     4, 1,  22,
     11,2,  58,60,
     5, 1,  29,
     10, 3, 54,55,56,
     6, 2,  32,33,
     12, 4, 64,65,66,67,
     13, 2, 71,72,
     7, 2,  37,39,
     8, 3,  42,43,44,

     // sector 2 pvs, 100
     2,3,   10,11,12,
     1,2,   5,7,
     0,3,   2,3,0,
     3,4,   15,16,18,19,
     9,2,   48,50,
     4,1,   22,
     11,2,  58,60,
     5, 1,  29,
     10,3,  54,55,56,
     6, 2,  32,33,
     12, 4, 64,65,66,67,
     7, 2,  37,39,
     8, 3,  42,43,44,
     13, 2, 71,72,

     // sector 3 pvs, 162
     3,5,    15,16,18,19,20,
     2,2,    10,12,
     1,2,    5,7,
     0,3,    2,3,0,
     3,4,   15,16,18,19,
     9,1,   48,
     4,2,   22,24,
     9,1,   50,
     11,2,  58,60,
     5, 1,  29,
     10,3,  54,55,56,
     6, 2,  32,33,
     12, 4, 64,65,66,67,
     7, 2,  37,39,
     8, 3,  42,43,44,
     13, 2, 71,72,

     // sector 4 pvs, 233
     4,1,   22,
     9,2,   47,50,

};



portal_map editor_test_map  = {
    .num_sectors = 14,
    .num_walls = 62,
    .num_verts = 30,
    .sectors = sectors,
    .walls = walls,
    .portals = portals,
    .vertexes = vertexes,
    .wall_colors = wall_colors,
    .wall_norm_quadrants = wall_normal_quadrants,
    .sector_params = sector_params,
    .sector_types = sector_types,
    .pvs = pvs,
    .raw_pvs = raw_pvs
};
