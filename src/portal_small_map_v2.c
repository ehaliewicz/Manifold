#include <genesis.h>
#include "portal_map.h"
#include "vertex.h"

#define NUM_SECTORS 10

#define NUM_VERTS 28

#define NUM_WALLS 52

const s16 sectors[SECTOR_SIZE*NUM_SECTORS] = {
    // sector 0   wall_offset, portal_offset, num_walls, floor_height, ceil_height
    0,  0, 6, -80, 216,
    // sector 1
    7,  6, 6, -80, 216,
    // sector 2
    14, 12, 6, -80, 216,
    // sector 3
    21, 18, 6, -80, 216,
    // sector 4
    28, 24, 6, -80, 216,
    // sector 5
    35, 30, 6, -80, 216,
    // sector 6
    42, 36, 4, -80, 216,
    // sector 7
    47, 40, 4, -20, 156,
    // sector 8
    52, 44, 4, -20, 156,
    // sector 9
    57, 48, 4, -80, 216
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
    10, 11, 19, 20, 10,
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


const vis_range vis_ranges[NUM_WALLS] = {
    {.two_ranges = 1, .angles = { 354,1024,0,110 } },
    {.two_ranges = 0, .angles = { 384,861 } },
    {.two_ranges = 0, .angles = { 287,768 } },
    {.two_ranges = 0, .angles = { 287,768 } },
    {.two_ranges = 0, .angles = { 287,768 } },
    {.two_ranges = 0, .angles = { 190,671 } },
    {.two_ranges = 1, .angles = { 0,256,799,989 } },
    {.two_ranges = 1, .angles = { 0,128,671,989 } },
    {.two_ranges = 0, .angles = { 287,768 } },
    {.two_ranges = 0, .angles = { 159,640 } },
    {.two_ranges = 0, .angles = { 159,640 } },
    {.two_ranges = 0, .angles = { 256,733 } },
    {.two_ranges = 1, .angles = { 0,349,896,989 } },
    {.two_ranges = 1, .angles = { 0,128,671,989 } },
    {.two_ranges = 0, .angles = { 415,896 } },
    {.two_ranges = 0, .angles = { 190,671 } },
    {.two_ranges = 1, .angles = { 0,384,927,989 } },
    {.two_ranges = 0, .angles = { 62,543 } },
    {.two_ranges = 1, .angles = { 0,384,927,989 } },
    {.two_ranges = 1, .angles = { 0,256,799,989 } },
    {.two_ranges = 0, .angles = { 415,896 } },
    {.two_ranges = 0, .angles = { 287,768 } },
    {.two_ranges = 1, .angles = { 0,349,896,989 } },
    {.two_ranges = 1, .angles = { 0,287,830,989 } },
    {.two_ranges = 1, .angles = { 0,256,799,989 } },
    {.two_ranges = 1, .angles = { 0,256,799,989 } },
    {.two_ranges = 1, .angles = { 0,159,702,989 } },
    {.two_ranges = 0, .angles = { 287,768 } },
    {.two_ranges = 0, .angles = { 128,605 } },
    {.two_ranges = 1, .angles = { 0,159,702,989 } },
    {.two_ranges = 1, .angles = { 0,93,640,989 } },
    {.two_ranges = 1, .angles = { 0,93,640,989 } },
    {.two_ranges = 1, .angles = { 0,62,605,989 } },
    {.two_ranges = 0, .angles = { 384,861 } },
    {.two_ranges = 0, .angles = { 128,605 } },
    {.two_ranges = 1, .angles = { 0,256,799,989 } },
    {.two_ranges = 1, .angles = { 0,159,702,989 } },
    {.two_ranges = 0, .angles = { 415,896 } },
    {.two_ranges = 0, .angles = { 415,896 } },
    {.two_ranges = 0, .angles = { 31,512 } },
    {.two_ranges = 0, .angles = { 287,768 } },
    {.two_ranges = 1, .angles = { 0,287,830,989 } },
    {.two_ranges = 1, .angles = { 0,256,799,989 } },
    {.two_ranges = 0, .angles = { 318,799 } },
    {.two_ranges = 1, .angles = { 0,256,799,989 } },
    {.two_ranges = 0, .angles = { 62,543 } },
    {.two_ranges = 1, .angles = { 0,159,702,989 } },
    {.two_ranges = 0, .angles = { 318,799 } },
    {.two_ranges = 0, .angles = { 446,927 } },
    {.two_ranges = 0, .angles = { 287,768 } },
    {.two_ranges = 1, .angles = { 0,384,927,989 } },
    {.two_ranges = 1, .angles = { 0,256,799,989 } },
}; 



#define VERT(y1,x1) { .x = (x1 * 30), .y = (y1*40) }

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
    .wall_vis_ranges = vis_ranges
};
