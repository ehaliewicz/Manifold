#include <genesis.h>
#include "portal_map.h"
#include "maps.h"

#include "map_table.h"



const portal_map* map_table[NUM_MAPS+3] = {
    (portal_map*)"maps",
    (portal_map*)"list",
    1, // number of maps
    &overlapping_map, 0, 0, 0, 0, 0, 0, 0, // &editor_test_map
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

