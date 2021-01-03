#define SECTOR_SIZE 4
#define NUM_SECTORS 10

#define NUM_VERTS 27
#define VERT_SIZE 2

#define NUM_WALLS 52

int16_t sectors[SECTOR_SIZE*NUM_SECTORS] = {
    // sector 0 
    0, 6, floor, ceil,
    // sector 1
    6, 6, floor, ceil,
    // sector 2
    12, 6, floor, ceil,
    // sector 3
    18, 6, floor, ceil,
    // sector 4
    24, 6, floor, ceil,
    // sector 5
    30, 6, floor, ceil,
    // sector 6
    36, 4, floor, ceil,
    // sector 7
    40, 4, floor, ceil,
    // sector 8
    44, 4, floor, ceil,
    // sector 9
    48, 4, floor, ceil
};



int16_t walls[NUM_WALLS] = {
    // sector 0 walls 
    0, 1, 9, 8, 7, 6,
    // sector 1 walls
    1, 2, 12, 11, 10, 9,
    // sector 2 walls
    2, 3, 15, 14, 13, 12,
    // sector 3 walls
    3, 4, 18, 17, 16, 15,
    // sector 4 walls
    4, 5, 21, 20, 19, 18,
    // sector 5 walls
    5, 0, 6, 23, 22, 21,
    // sector 6 walls
    7, 8, 17, 16,
    // sector 7 walls
    25, 13, 14, 26,
    // sector 8 walls
    23, 24, 27, 22,
    // sector 9 walls
    10, 11, 19, 20
};


int16_t portals[NUM_WALLS] = {
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


vertex vertexes[NUM_VERTS] = {
			      
};

