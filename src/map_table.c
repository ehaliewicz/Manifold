#include <genesis.h>
#include "portal_map.h"
#include "maps.h"

#include "empty_maps.h"
#include "map_table.h"



const __UINTPTR_TYPE__ map_table[NUM_MAPS+3] = {
    0xDEADBEEF,
    4, // number of maps
    NUM_MAPS,
    &overlapping_map, &editor_test_map, 0, 0//&empty_map_1, //&empty_map_2, &empty_map_3,
};


// 256KB of space for map data
const uint8_t map_data_bank[2048*1024] = { 0 };