#include <genesis.h>
#include "colors.h"
#include "portal_map.h"
#include "textures.h"
#include "vertex.h"




// wall offset, portal offset, number of walls, flag
static const s16 sectors[17*SECTOR_SIZE] = {
    0, 0, 6, 0, // floor normally 100 here
    7, 6, 5, 1,
    13, 11, 4, 2, //SECTOR_FLOOR_SLOPED|SECTOR_CEIL_SLOPED, // floor normally 100 here
    18, 15, 4, 3, //SECTOR_FLOOR_SLOPED|SECTOR_CEIL_SLOPED,
    23, 19, 4, 4, //SECTOR_FLOOR_SLOPED|SECTOR_CEIL_SLOPED,
    28, 23, 4, 5, //SECTOR_FLOOR_SLOPED|SECTOR_CEIL_SLOPED,
    33, 27, 4, 6, //SECTOR_FLOOR_SLOPED|SECTOR_CEIL_SLOPED,
    38, 31, 5, 7,
    44, 36, 4, 8,
    49, 40, 4, 9,
    54, 44, 4, 10,
    59, 48, 4, 11,
    64, 52, 4, 12,
    69, 56, 4, 13,
    74, 60, 4, 14,
    79, 64, 5, 15,
    85, 69, 4, 16,
};


static const s16 sector_group_params[NUM_SECTOR_PARAMS*17] = {
    // light, orig_height, ticks_left, state, floor_height, ceil_height, floor_color, ceil_color
    0, 0, 0, 0,  60<<4, 200<<4, BLUE_IDX, BLUE_IDX, // only LIGHT is relevant here

    0, 200<<4, CLOSED, 30, 60<<4, 60<<4,  BLUE_IDX, BLUE_IDX, 
    
    0, 60<<4, CLOSED, 30, 200<<4, 200<<4, BLUE_IDX, BLUE_IDX,
    
    -1, 0, 0, 0, 120<<4, 240<<4, BLUE_IDX, LIGHT_BLUE_IDX,
    
    -1, 0, 0, 0, 140<<4, 280<<4, BLUE_IDX, BLUE_IDX,
    
    -1, 0, 0, 0, 160<<4, 320<<4, BLUE_IDX, LIGHT_BLUE_IDX,
    
    -1, 0, 0, 0, 180<<4, 350<<4, BLUE_IDX, BLUE_IDX,
    
    -1, 0, 0, 0, 200<<4, 350<<4, BLUE_IDX, LIGHT_BLUE_IDX,
    
    -1, 0, 0, 0,  60<<4, 200<<4, BLUE_IDX, BLUE_IDX,
    
    -2, 0, 0, 0, 100<<4, 200<<4, BLUE_IDX, BLUE_IDX,
    
    -2, 0, 0, 0, 200<<4, 350<<4, BLUE_IDX, TRANSPARENT_IDX,
    
    -1, 0, 0, 0,  60<<4, 180<<4, RED_IDX,  LIGHT_RED_IDX, 
    
    -1, 0, 0, 0,  60<<4, 160<<4, BLUE_IDX, BLUE_IDX,
    
    -1, 0, 0, 0,  40<<4, 140<<4, RED_IDX,  LIGHT_RED_IDX,
    
    -1, 0, 0, 0,  20<<4, 120<<4, BLUE_IDX, BLUE_IDX,
    
    -2, 0, 0, 0,   0<<4, 100<<4, RED_IDX,  LIGHT_RED_IDX,

    -2, 0, 0, 0,   0<<4, 100<<4, RED_IDX,  RED_IDX,
};

static const u16 sector_group_triggers[17*8] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};



static const u16 walls[90] = {
     0,  1,  2,  3,  4,  5,  0, 
     2,  6,  7, 21,  3,  2, 
     4,  8,  9,  5,  4, 
     8, 10, 11,  9,  8, 
    10, 12, 13, 11, 10, 
    12, 14, 15, 13, 12, 
    14, 16, 17, 15, 14, 
    16, 18, 19, 20, 17, 16, 
    21, 22, 23,  3, 21, 
    22, 24, 25, 23, 22, 
    18, 26, 27, 19, 18, 
     6, 28, 29,  7,  6, 
    28, 30, 31, 29, 28, 
    30, 32, 33, 31, 30, 
    32, 34, 35, 33, 32, 
    34, 36, 37, 38, 35, 34, 
    36, 39, 40, 37, 36, 
};

static const s16 portals[73] = {
    -1, -1, 1, -1, 2, -1, 
    -1, 11, -1, 8, 0, 
    -1, 3, -1, 0, 
    -1, 4, -1, 2, 
    -1, 5, -1, 3, 
    -1, 6, -1, 4, 
    -1, 7, -1, 5, 
    -1, 10, -1, -1, 6, 
    -1, 9, -1, 1, 
    -1, -1, -1, 8, 
    -1, -1, -1, 7, 
    -1, 12, -1, 1, 
    -1, 13, -1, 11, 
    -1, 14, -1, 12, 
    -1, 15, -1, 13, 
    -1, 16, -1, -1, 14, 
    -1, -1, -1, 15, 
};

static const u8 wall_normal_quadrants[73] ={
    FACING_DOWN,
    FACING_LEFT,
    FACING_LEFT,
    FACING_UP,
    FACING_UP,
    FACING_RIGHT,
    FACING_DOWN,
    FACING_LEFT,
    FACING_UP,
    FACING_UP,
    FACING_RIGHT,
    FACING_LEFT,
    FACING_UP,
    FACING_RIGHT,
    FACING_DOWN,
    QUADRANT_2,
    QUADRANT_1,
    QUADRANT_0,
    FACING_DOWN,
    QUADRANT_2,
    QUADRANT_1,
    QUADRANT_0,
    QUADRANT_3,
    FACING_DOWN,
    FACING_LEFT,
    QUADRANT_0,
    QUADRANT_3,
    FACING_DOWN,
    FACING_LEFT,
    FACING_UP,
    FACING_RIGHT,
    QUADRANT_3,
    FACING_LEFT,
    FACING_UP,
    FACING_RIGHT,
    FACING_RIGHT,
    FACING_LEFT,
    FACING_UP,
    FACING_RIGHT,
    FACING_DOWN,
    QUADRANT_2,
    FACING_UP,
    QUADRANT_3,
    FACING_DOWN,
    QUADRANT_3,
    FACING_LEFT,
    QUADRANT_0,
    FACING_RIGHT,
    QUADRANT_2,
    QUADRANT_1,
    QUADRANT_0,
    FACING_RIGHT,
    QUADRANT_2,
    QUADRANT_1,
    QUADRANT_0,
    QUADRANT_3,
    QUADRANT_1,
    QUADRANT_0,
    QUADRANT_3,
    QUADRANT_3,
    QUADRANT_1,
    QUADRANT_0,
    QUADRANT_3,
    QUADRANT_2,
    QUADRANT_1,
    QUADRANT_0,
    QUADRANT_3,
    QUADRANT_2,
    QUADRANT_2,
    FACING_LEFT,
    QUADRANT_0,
    QUADRANT_3,
    QUADRANT_2,
};

static const u8 wall_colors[73 * WALL_COLOR_NUM_PARAMS] = {
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, LIGHT_GREEN_IDX, GREEN_IDX, 0,
    0, 0, 0, 0,
    0, LIGHT_GREEN_IDX, LIGHT_GREEN_IDX, 0,
    0, 0, 0, 0,
    // sector 1 walls
    0, 0, 0, 0,
    0, GREEN_IDX, GREEN_IDX, 0,
    0, 0, 0, 0,
    0, GREEN_IDX, GREEN_IDX, 0,
    0, GREEN_IDX, GREEN_IDX, 0,
    // sector 2 walls
    0, 0, 0, 0,
    0, LIGHT_GREEN_IDX, LIGHT_GREEN_IDX, 0,
    0, 0, 0, 0,
    0, LIGHT_GREEN_IDX, LIGHT_GREEN_IDX, 0,
    // sector 3 walls
    0, 0, 0, 0,
    0, LIGHT_GREEN_IDX, LIGHT_GREEN_IDX, 0,
    0, 0, 0, 0,
    0, GREEN_IDX, GREEN_IDX, 0,
    // sector 4 walls
    0, 0, 0, 0,
    0, LIGHT_GREEN_IDX, LIGHT_GREEN_IDX, 0,
    0, 0, 0, 0,
    0, GREEN_IDX, GREEN_IDX, 0,
    // sector 5 walls
    0, 0, 0, 0,
    0, LIGHT_GREEN_IDX, LIGHT_GREEN_IDX, 0,
    0, 0, 0, 0,
    0, GREEN_IDX, GREEN_IDX, 0,
    // sector 6 walls
    0, 0, 0, 0,
    0, GREEN_IDX, GREEN_IDX, 0,
    0, 0, 0, 0,
    0, GREEN_IDX, GREEN_IDX, 0,
    // sector 7 walls
    0, 0, 0, 0,
    0, GREEN_IDX, GREEN_IDX, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, GREEN_IDX, GREEN_IDX, 0,
    // sector 8 walls
    0, 0, 0, 0,
    0, LIGHT_GREEN_IDX, LIGHT_GREEN_IDX, 0,
    0, 0, 0, 0,
    0, GREEN_IDX, GREEN_IDX, 0,
    // sector 9 walls
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, GREEN_IDX, GREEN_IDX, 0,
    // sector 10 walls
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, GREEN_IDX, GREEN_IDX, 0,
    // sector 11 walls
    0, 0, 0, 0,
    0, GREEN_IDX, GREEN_IDX, 0,
    0, 0, 0, 0,
    0, GREEN_IDX, GREEN_IDX, 0,
    // sector 12 walls
    0, 0, 0, 0,
    0, LIGHT_GREEN_IDX, LIGHT_GREEN_IDX, 0,
    0, 0, 0, 0,
    0, GREEN_IDX, GREEN_IDX, 0,
    // sector 13 walls
    0, 0, 0, 0,
    0, LIGHT_GREEN_IDX, LIGHT_GREEN_IDX, 0,
    0, 0, 0, 0,
    0, GREEN_IDX, GREEN_IDX, 0,
    // sector 14 walls
    0, 0, 0, 0,
    0, LIGHT_GREEN_IDX, LIGHT_GREEN_IDX, 0,
    0, 0, 0, 0,
    0, GREEN_IDX, GREEN_IDX, 0,
    // sector 15 walls
    0, 0, 0, 0,
    0, LIGHT_GREEN_IDX, LIGHT_GREEN_IDX, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, GREEN_IDX, GREEN_IDX, 0,
    // sector 16
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, GREEN_IDX, GREEN_IDX, 0,
};

// #define VERT(x1,y1) { .x = (x1 * 6), .y = ((-y1) * 6) } 

#define VERT(x1,y1) { .x = (x1 * 2), .y = ((-y1) * 2) }
static const vertex vertexes[42] = {
    VERT(70,60),
    VERT(170,60),
    VERT(170,110),
    VERT(170,160),
    VERT(120,160),
    VERT(70,160),
    VERT(310,110),
    VERT(310,160),
    VERT(120,270),
    VERT(70,270),
    VERT(140,300),
    VERT(80,320),
    VERT(160,320),
    VERT(110,350),
    VERT(170,320),
    VERT(170,360),
    VERT(190,320),
    VERT(190,360),
    VERT(260,310),
    VERT(260,390),
    VERT(190,390),
    VERT(220,160),
    VERT(220,450),
    VERT(170,450),
    VERT(320,570),
    VERT(60,570),
    VERT(360,250),
    VERT(360,470),
    VERT(360,130),
    VERT(330,170),
    VERT(380,180),
    VERT(340,190),
    VERT(370,220),
    VERT(330,210),
    VERT(350,250),
    VERT(310,230),
    VERT(320,300),
    VERT(240,260),
    VERT(270,210),
    VERT(320,400),
    VERT(140,320),
    VERT(0,0),
};


static const u8 sector_group_types[17] = {
    NO_TYPE,
    DOOR,
    LIFT,
    NO_TYPE,
    NO_TYPE,
    NO_TYPE,
    NO_TYPE,
    NO_TYPE,
    NO_TYPE,
    FLASHING,
    FLASHING,
    NO_TYPE,
    NO_TYPE,
    NO_TYPE,
    NO_TYPE,
    NO_TYPE,
    FLASHING
};

const portal_map overlapping_map = {
    .num_sector_groups = 17,
    .num_sectors = 17,
    .num_walls = 73,
    .num_verts = 42,
    .sectors = sectors,
    .sector_group_types = sector_group_types,
    .sector_group_params = sector_group_params,
    .sector_group_triggers = sector_group_triggers,
    .walls = walls,
    .portals = portals,
    .vertexes = vertexes,
    .wall_colors = wall_colors,
    .wall_norm_quadrants = wall_normal_quadrants,
    .has_pvs = 0,
};
