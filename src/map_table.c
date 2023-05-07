#include <genesis.h>
#include "portal_map.h"
#include "maps.h"

#include "map_table.h"

#include "utils.h"



const uint32_t map_table[MAX_MAPS+2] = {
    0xDEADBEEF,
    3, // number of maps
    &overlapping_map,
    &editor_test_map, 
    &building_test_map, 
    0, 0
    //&empty_map_1, //&empty_map_2, &empty_map_3,
};



// 2MB of space for map, texture, sprite, and palette data
const uint8_t wad_area[512*1024*2] = { 
    'W','A','D','?',
    };
// 2048*1024

