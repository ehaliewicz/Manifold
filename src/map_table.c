#include <genesis.h>
#include "portal_map.h"
#include "maps.h"

#define NUM_MAPS 32


const portal_map* map_table[NUM_MAPS+2] = {
    (portal_map*)0xDEADBEEF,
    1, // number of maps
    &overlapping_map, &editor_test_map, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

