#include <genesis.h>
#include "portal_map.h"
#include "vertex.h"

// 7 sectors

#define SECTOR_SIZE 4
#define NUM_SECTORS 7

#define NUM_WALLS 32

#define NUM_VERTS 16
#define VERT_SIZE 2


// 56 bytes of wall reference data for sectors

// start wall, num walls, floor height, ceiling height
const s16 sectors[SECTOR_SIZE*NUM_SECTORS] = {
    // sector 0 6 walls
    0,  6, -80, 216,
    // sector 1, 4 walls
    6,  4, -80, 216,
    // sector 2, 4 walls
    10, 4, -80, 216,
    // sector 3, 6 walls
    14, 6, -80, 216,
    // sector 4, 4 walls
    20, 4, -80, 216,
    // sector 5, 4 walls
    24, 4, -80, 216,
    // sector 6, 4 walls
    28, 4, -80, 216
};


// 64 bytes of vertex references for walls
const u16 walls[32] = {
    // sector 0 walls
    0,1,7,13,12,6,
    // sector 1 walls
    1,2,8,7,
    // sector 2 walls
    2,3,9,8,
    // sector 3 walls
    3,4,10,14,15,9,
    // sector 4 walls
    4,5,11,10,
    // sector 5 walls
    5,0,6,11,
    // sector 6 walls
    12,13,15,14
};

// 64 bytes of sector references for walls

// -1 if not portal, otherwise an integer referencing another sector

const s16 portals[32] = {
    // sector 0 portal refs
    -1,1,-1,6,-1,5,
    // sector 1 portal refs
    -1,2,-1,0,
    // sector 2 portal refs
    -1,3,-1,1,
    // sector 3 portal refs
    -1,4,-1,6,-1,2,
    // sector 4 portal refs
    -1,5,-1,3,
    // sector 5 portal refs
    -1,0,-1,4,
    // sector 6 portal refs
    0,-1,3,-1
};



#define VERT(y1,x1) { .x = ((x1*3*10)), .y = (y1*4*10) }

// x needs to be divided by two
const vertex verts[NUM_VERTS*VERT_SIZE] = {
    VERT(14,46),		      
    VERT(13,90),
    VERT(29,109),
    VERT(49,96),
    VERT(46,48),
    VERT(27,35),
    VERT(18,55),
    VERT(18,84),
    VERT(30,97),
    VERT(41,88),
    VERT(41,55),
    VERT(27,46),
    VERT(18,67),
    VERT(18,76),
    VERT(41,67),
    VERT(41,76),    
};



const portal_map portal_level_1 = {
    .num_sectors = NUM_SECTORS,
    .num_walls = NUM_WALLS,
    .num_verts = NUM_VERTS,
    .sectors = sectors,
    .walls = walls,
    .portals = portals,
    .vertexes = verts,
};