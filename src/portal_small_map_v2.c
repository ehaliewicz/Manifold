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
    10, 11, 19, 20 10,
};

const vis_range vis_ranges[NUM_WALLS] = {
{.two_ranges = 1, .angles = { 0,507,1021,1023 } },
{.two_ranges = 0, .angles = { 399,909 } }, 
{.two_ranges = 1, .angles = { 0,0,513,1023 } },
{.two_ranges = 1, .angles = { 0,0,513,1023 } },
{.two_ranges = 1, .angles = { 0,0,513,1023 } },
{.two_ranges = 1, .angles = { 0,68,581,1023 } },
{.two_ranges = 1, .angles = { 0,507,1021,1023 } },
{.two_ranges = 0, .angles = { 110,621 } }, 
{.two_ranges = 0, .angles = { 500,1011 } }, 
{.two_ranges = 1, .angles = { 0,113,626,1023 } },
{.two_ranges = 1, .angles = { 0,128,641,1023 } },
{.two_ranges = 1, .angles = { 0,27,540,1023 } },
{.two_ranges = 1, .angles = { 0,398,911,1023 } },
{.two_ranges = 0, .angles = { 110,621 } }, 
{.two_ranges = 0, .angles = { 369,880 } }, 
{.two_ranges = 1, .angles = { 0,91,604,1023 } },
{.two_ranges = 1, .angles = { 0,384,897,1023 } },
{.two_ranges = 1, .angles = { 0,201,714,1023 } },
{.two_ranges = 1, .angles = { 0,384,897,1023 } },
{.two_ranges = 1, .angles = { 0,499,1012,1023 } },
{.two_ranges = 0, .angles = { 369,880 } }, 
{.two_ranges = 1, .angles = { 0,0,513,1023 } },
{.two_ranges = 1, .angles = { 0,410,923,1023 } },
{.two_ranges = 1, .angles = { 0,475,988,1023 } },
{.two_ranges = 0, .angles = { 0,512 } }, 
{.two_ranges = 0, .angles = { 0,512 } }, 
{.two_ranges = 0, .angles = { 92,603 } }, 
{.two_ranges = 1, .angles = { 0,0,513,1023 } },
{.two_ranges = 1, .angles = { 0,158,671,1023 } },
{.two_ranges = 0, .angles = { 68,579 } }, 
{.two_ranges = 0, .angles = { 152,662 } }, 
{.two_ranges = 0, .angles = { 152,662 } }, 
{.two_ranges = 0, .angles = { 181,691 } }, 
{.two_ranges = 0, .angles = { 410,922 } }, 
{.two_ranges = 1, .angles = { 0,158,671,1023 } },
{.two_ranges = 0, .angles = { 0,512 } }, 
{.two_ranges = 0, .angles = { 69,580 } }, 
{.two_ranges = 0, .angles = { 385,896 } }, 
{.two_ranges = 0, .angles = { 385,896 } }, 
{.two_ranges = 1, .angles = { 0,242,755,1023 } },
{.two_ranges = 1, .angles = { 0,0,513,1023 } },
{.two_ranges = 1, .angles = { 0,466,980,1023 } },
{.two_ranges = 0, .angles = { 0,512 } }, 
{.two_ranges = 0, .angles = { 465,975 } }, 
{.two_ranges = 0, .angles = { 0,512 } }, 
{.two_ranges = 1, .angles = { 0,195,708,1023 } },
{.two_ranges = 0, .angles = { 83,595 } }, 
{.two_ranges = 0, .angles = { 456,967 } }, 
{.two_ranges = 0, .angles = { 352,863 } }, 
{.two_ranges = 1, .angles = { 0,0,513,1023 } },
{.two_ranges = 1, .angles = { 0,384,897,1023 } },
{.two_ranges = 0, .angles = { 0,512 } }, 

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
