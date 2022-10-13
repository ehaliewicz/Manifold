#include <genesis.h>
#include "portal_map.h"
#include "maps.h"

#include "map_table.h"

#include "utils.h"



const uint32_t map_table[NUM_MAPS+3] = {
    0xDEADBEEF,
    3, // number of maps
    &overlapping_map,
    &editor_test_map, 
    &building_test_map, 
    0
    //&empty_map_1, //&empty_map_2, &empty_map_3,
};



// 2MB of space for map, texture, sprite, and palette data
const uint8_t wad_area[1024*1024*2] = { 
    'W','A','D','?',
    };
// 2048*1024



#define WAD_BASE_IDX 4

/*
typedef struct {
    u8 num_entries;
    dir_entry entries[32];
} wad_directory;

typedef struct {
    u8 namelen;
    char name[32];
    u32 len;
    u32 offset;
} dir_entry;
*/

u32 read_u32(u32 idx) {
    return ((wad_area[idx] << 24) |
            (wad_area[idx+1] << 16) |
            (wad_area[idx+2] << 8) |
            (wad_area[idx+3]));
}


void load_map(char* map_name) {
    u32 idx = WAD_BASE_IDX;
    u8 num_directory_entries = wad_area[idx++];
    for(int i = 0; i < num_directory_entries; i++) {
        u8 namelen = wad_area[idx++];
        if(strcmp(map_name, (char*)(&wad_area[idx+1])) == 0) {
            u32 len = read_u32(idx); idx+=4;
            u32 offset = read_u32(idx); idx+=4;

            //load_map_at_offset(offset, len);
            return;
        }
    }
    char buf[32];
    sprintf(buf, "Cannot find map '%s'", map_name);
    die(buf);
}