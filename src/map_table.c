#include <genesis.h>
#include "portal_map.h"
#include "maps.h"

#include "map_table.h"



const u32 map_table[NUM_MAPS+2] = {
    0xDEADBEEF,
    1, // number of maps
    (u32)&overlapping_map, 0, 0, 0, 0, 0, 0, 0, // &editor_test_map
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};