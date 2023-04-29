
#include <genesis.h>
#include "colors.h"
#include "portal_map.h"
#include "sector_group.h"
#include "vertex.h"

// 9-12 are slime sectors
// wall offset, portal offset, number of walls, sector group
static const s16 sectors[4*14] ={
    0, 0, 4, 0,
    5, 4, 4, 0,
    10, 8, 4, 1,
    15, 12, 6, 2, 
    22, 18, 4, 2,
    27, 22, 4, 2,
    32, 26, 4, 2,
    37, 30, 4, 3,
    42, 34, 4, 3,
    47, 38, 4, 4, // slime sector
    52, 42, 5, 4,// slime sector
    58, 47, 5, 4,// slime sector
    64, 52, 6, 4,// slime sector
    71, 58, 4, 5,
};

static const u16 walls[76] = {
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
    //  9 47   slime sector
    11, 10, 13, 22, 11, 
    // 10 52   slime sector
    22, 13, 15, 17, 23, 22, 
    // 11 58   slime sector
    8, 24, 14, 12, 9, 8, 
    // 12 64   slime sector
    24, 25, 26, 27, 16, 14, 24, 
    // 13 71
    26, 28, 29, 27, 26, 
};

static const s16 portals[62] = {
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

static const u8 wall_normal_quadrants[62] ={
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
// 47 through 64
static const u8 wall_colors[62 * WALL_COLOR_NUM_PARAMS] = {
    // sector 0
    4,0,0,0,
    4,1,1,0,
    4,0,0,0,
    4,0,0,0,
    4,0,0,0,
    // sector 1
    4,1,1,0,
    4,0,0,0,
    4,1,1,0,
    4,0,0,0,
    4,1,1,0,
    // sector 2
    1,0,0,0,
    1,1,1,0,
    1,0,0,0,
    1,1,1,0,
    1,1,1,0,
    // sector 3
    1,1,1,0,
    1,0,0,0,
    1,12,1,0,
    1,1,1,0,
    1,1,1,0,
    1,1,1,0,
    1,1,1,0,
    // sector 4
    1,1,1,0,
    1,1,1,0,
    1,1,1,0,
    1,1,1,0,
    1,1,1,0,
    // sector 5
    1,12,9,0,
    1,1,1,0,
    1,1,1,0,
    1,0,0,0,
    1,1,1,0,
    // sector 6
    3,0,0,0,
    3,12,12,0,
    3,0,0,0,
    3,0,0,0,
    3,0,0,0,
    // sector 7
    2,1,9,0,
    2,9,9,0, // 0
    2,1,9,0, // 1
    2,1,9,0,
    2,0,0,0,
    // sector 8
    2,1,12,0,
    2,1,9,0, // 2
    2,1,9,0, // 3
    2,0,0,0,
    2,0,0,0,
    2,0,0,0,
    2,1,12,0,
    2,1,9,0, // 4
    2,1,9,0, // 5
    2,12,9,0, // 6
    2,0,0,0,
    2,0,0,0,
    2,1,9,0,
    2,0,0,0,
    2,1,9,0, // 8
    2,1,9,0,
    2,0,0,0,
    2,0,0,0,
    2,0,0,0,
    2,1,1,0,
};

#define VERT(x1,y1) { .x = (x1 * 2), .y = ((-y1) * 2) } 
static const vertex vertexes[30] = {
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

//static const u8 sector_types[14] = {
//0,0,0,0,0,0,0,1,1,0,0,0,0,1,
//};

static const s16 sector_group_params[NUM_SECTOR_GROUP_PARAMS*6] = {
    // light, orig_height, ticks_left, state, floor_height, ceil_height, floor_color, ceil_color

    // group 0 params
    -1, 0<<4, 0, 0, 0<<4, 100<<4, 12, 12,
    // group 1 params
    -1, 0<<4, 0, 0, 0<<4, 100<<4, 12, 12,
    // group 2 params
    0, 0<<4, 0, 0, -30<<4, 240<<4, 3, 6, // walkway group?
    // group 3 params
    // -2 light level
    -2, 0<<4, 0, 0, 0<<4, 100<<4, 6, 6, // hallway group
    // group 4 params
    0, 0<<4, 0, 0, -30<<4, 240<<4, 3, 6, // slime sector group
    // group 5 params
    // -2 light level
    -2, 0<<4, 0, 0, 80<<4, 240<<4, 6, 6, // upper room group
};

static const u8 sector_group_types[6] = {
    0, // group 0 // entryway
    0, // group 1 // outside hallway
    4, // group 2 // zigzag
    1, // group 3 // walkway
    0, // group 4 // slime
    1, // group 5 // upper balcony
};

static const s16 sector_group_triggers[6*8] = {
    0,0,0,0,0,0,0,0,
    3,2,-1,0,0,0,0,0, // start stairs trigger on group 2
    1,2,4,-1,0,0,0,0, // set sector groups 2,4 dark
    2,2,4,-1,0,0,0,0, // set sector groups 2,4 light
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

// pvs is an index for each sector, plus length
// raw pvs is wall lists


static const u16 pvs[14 * 2] = {
     0, 10,
    40, 14,
   100, 14,
   162, 15,
   225, 15,
   286, 16,
   350, 15,
   411, 12,
   459, 11,
   503, 14,
   565, 13,
   624, 16,
   697, 16,
   766, 12,
};

#define SECTOR_START_FLAG = 1<<15


static const u16 raw_pvs[1024] = {
    // sector 0 pvs
    // sector index, num walls, N walls

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
     9,2,   48,50,
     4,2,   22,24,
     11,2,  58,60,
     5, 1,  29,
     10,3,  54,55,56,
     6, 2,  32,33,
     12, 4, 64,65,66,67,
     7, 2,  37,39,
     8, 3,  42,43,44,
     13, 2, 71,72,

     // sector 4 pvs, 227
     4,2,   22,24,
     11,3,  58,60,62,
     9,2,   47,50,
     3,1,   18,
     3,3,   19,20,15,
     2,1,   10,
     1,1,   5,
     0,1,   0,
     5,1,   29,
     10,3,  54,55,56,
     6,2,   32,33,
     12, 4, 64,65,66,67,
     13, 2, 71,72,
     7, 2,  37,39,
     8, 3,  42,43,44,

    // sector 5 pvs, 287
    5,2,    27,29,
    6,1,    34,
    10,3,   54,55,56,
    6,2,    32,33,
    12,4,   64,65,66,67,
    7,2,    37,39,
    8,3,    42,43,44,
    4,1,    22,
    11,3,   58,61,62,
    13,2,   71,72,
    4,1,    24,
    9,2,    47,50,
    3,3,    19,20,15,
    2,1,    10,
    1,1,    5,
    0,1,    0,

    // sector 6 pvs, 351
    6,3,    32,33,34,
    5,1,    29,
    10,3,   53,55,56,
    5,1,    27,
    12,4,   64,65,66,67,
    7,2,    37,39,
    13,2,   71,72,
    8,3,    42,43,44,
    11,3,   58,61,62,
    4,1,    24,
    9,2,    50,47,
    3,3,    19,20,15,
    2,1,    10,
    1,1,    5,
    0,1,    0,

    // sector 7 pvs, 412
    7,3,    37,39,40,
    8,3,    42,43,44,
    6,2,    32,34,
    10,2,   56,53,
    5,1,    27,
    4,1,    24,
    12,2,   64,65,
    11,3,   58,61,62,
    9,2,    47,50,
    3,3,    19,20,15,
    2,1,    10,
    1,1,    5,

    // sector 8 pvs, 460
    8,3,    42,43,44,
    7,3,    37,39,40,
    6,2,    32,34,
    10,2,   56,53,
    5,1,    27,
    4,1,    24,
    11,3,   58,61,62,
    9,2,    47,50,
    3,3,    19,20,15,
    2,1,    10,
    1,1,    5,

    // sector 9 pvs, 504
    9,3,    47,48,50,
    4,3,    22,23,25,
    3,5,    15,16,17,19,20,
    2,1,    10,
    1,1,    5,
    5,1,    29,
    10,4,   53,54,55,56,
    5,1,    27,
    6,2,    32,33,
    7,2,    37,39,
    8,3,    42,43,44,
    11,2,   58,60,
    12,4,   64,65,66,67,
    13,2,   71,72,

    // sector 10 pvs, 566
    10,4,   53,54,55,56,
    5,3,    27,28,30,
    6,2,    32,33,
    7,2,    37,39,
    8,3,    42,43,44,
    12,4,   64,65,66,67,
    13,2,   71,72,
    9,3,    47,48,50,
    4,2,    22,25,
    11,3,   58,61,62,
    3,3,    15,19,20,
    2,1,    10,
    1,1,    5,

    // sector 11 pvs, 625
    11,4,   58,60,61,62,
    5,2,    28,30,
    4,3,    23,24,25,
    5,1,    29,
    10,4,   52,54,55,56,
    6,3,    32,33,34,
    12,4,   64,65,66,67,
    13,3,   71,72,73,
    7,2,    37,39,
    8,3,    42,43,44,
    3,1,    18,
    9,3,    47,49,50,
    3,3,    15,19,20,
    2,2,    10,12,
    1,1,    5,
    0,2,    0,3,

    // sector 12 pvs, 696 
    12,5,   64,65,66,67,68,
    11,1,   60,
    6,3,    33,34,35,
    7,1,    39,
    8,1,    44,
    13,3,   71,72,73,
    5,1,    27,
    11,3,   58,61,62,
    5,2,    28,29,
    10,4,   52,53,55,56,
    4,2,    24,25,
    9,2,    47,50,
    3,4,    15,18,19,20,
    2,2,    10,12,
    1,1,    5,
    0,2,    0,3,

    // sector 13 pvs, 765
    13,4,   71,72,73,74,
    12,4,   64,65,67,68,
    6,3,    33,34,35,
    10,4,   52,53,55,56,
    5,1,    27,
    4,1,    24,
    11,3,   58,61,62,
    9,2,    47,50,
    3,3,    15,19,20,
    2,1,    10,
    1,1,    5,
    0,1,    0,
    
    

};



const portal_map editor_test_map  = {
    .num_sector_groups = 6,
    .num_sectors = 14,
    .num_walls = 62,
    .num_verts = 30,
    .sectors = sectors,
    .walls = walls,
    .portals = portals,
    .vertexes = vertexes,
    .wall_colors = wall_colors,
    .wall_norm_quadrants = wall_normal_quadrants,
    .sector_group_params = sector_group_params,
    .sector_group_types = sector_group_types,
    .sector_group_triggers = sector_group_triggers,
    .has_pvs = 1,
    .pvs = pvs,
    .raw_pvs = raw_pvs,
    .name = "slime room test",
    .num_things = 0,
};
