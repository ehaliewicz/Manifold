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


const u16 wall_normal_angles[NUM_WALLS] = {
					   771,
369,
256,
256,
256,
187,
771,
658,
268,
142,
128,
228,
881,
658,
399,
164,
896,
54,
896,
780,
399,
256,
869,
804,
768,
768,
676,
256,
97,
700,
616,
616,
587,
357,
97,
768,
699,
384,
384,
13,
256,
812,
768,
303,
768,
60,
684,
312,
416,
256,
896,
768,
};

const vis_range angle_ranges[NUM_WALLS] = {
{.two_ranges = 1, .angles = { 0,3,516,1023 } },
{.two_ranges = 0, .angles = { 114,625 } }, 
{.two_ranges = 0, .angles = { 0,512 } }, 
{.two_ranges = 0, .angles = { 0,512 } }, 
{.two_ranges = 0, .angles = { 0,512 } }, 
{.two_ranges = 1, .angles = { 0,443,956,1023 } },
{.two_ranges = 1, .angles = { 0,3,516,1023 } },
{.two_ranges = 0, .angles = { 403,914 } }, 
{.two_ranges = 0, .angles = { 13,524 } }, 
{.two_ranges = 1, .angles = { 0,398,911,1023 } },
{.two_ranges = 1, .angles = { 0,384,897,1023 } },
{.two_ranges = 1, .angles = { 0,484,997,1023 } },
{.two_ranges = 1, .angles = { 0,113,626,1023 } },
{.two_ranges = 0, .angles = { 403,914 } }, 
{.two_ranges = 0, .angles = { 144,655 } }, 
{.two_ranges = 1, .angles = { 0,420,933,1023 } },
{.two_ranges = 1, .angles = { 0,128,641,1023 } },
{.two_ranges = 1, .angles = { 0,310,823,1023 } },
{.two_ranges = 1, .angles = { 0,128,641,1023 } },
{.two_ranges = 1, .angles = { 0,12,525,1023 } },
{.two_ranges = 0, .angles = { 144,655 } }, 
{.two_ranges = 0, .angles = { 0,512 } }, 
{.two_ranges = 1, .angles = { 0,101,614,1023 } },
{.two_ranges = 1, .angles = { 0,36,549,1023 } },
{.two_ranges = 1, .angles = { 0,0,513,1023 } },
{.two_ranges = 1, .angles = { 0,0,513,1023 } },
{.two_ranges = 0, .angles = { 421,932 } }, 
{.two_ranges = 0, .angles = { 0,512 } }, 
{.two_ranges = 1, .angles = { 0,353,866,1023 } },
{.two_ranges = 0, .angles = { 445,956 } }, 
{.two_ranges = 0, .angles = { 361,872 } }, 
{.two_ranges = 0, .angles = { 361,872 } }, 
{.two_ranges = 0, .angles = { 332,843 } }, 
{.two_ranges = 0, .angles = { 102,613 } }, 
{.two_ranges = 1, .angles = { 0,353,866,1023 } },
{.two_ranges = 1, .angles = { 0,0,513,1023 } },
{.two_ranges = 0, .angles = { 444,955 } }, 
{.two_ranges = 0, .angles = { 129,640 } }, 
{.two_ranges = 0, .angles = { 129,640 } }, 
{.two_ranges = 1, .angles = { 0,269,782,1023 } },
{.two_ranges = 0, .angles = { 0,512 } }, 
{.two_ranges = 1, .angles = { 0,44,557,1023 } },
{.two_ranges = 1, .angles = { 0,0,513,1023 } },
{.two_ranges = 0, .angles = { 48,559 } }, 
{.two_ranges = 1, .angles = { 0,0,513,1023 } },
{.two_ranges = 1, .angles = { 0,316,829,1023 } },
{.two_ranges = 0, .angles = { 429,940 } }, 
{.two_ranges = 0, .angles = { 57,568 } }, 
{.two_ranges = 0, .angles = { 161,672 } }, 
{.two_ranges = 0, .angles = { 0,512 } }, 
{.two_ranges = 1, .angles = { 0,128,641,1023 } },
{.two_ranges = 1, .angles = { 0,0,513,1023 } },
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
