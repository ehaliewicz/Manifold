#include <genesis.h>
#include "portal_map.h"

s16* sector_data_start(s16 sector_idx, portal_map* mp) {
    return &mp->sectors[sector_idx*SECTOR_SIZE];
}

s16 sector_wall_offset(s16 sector_idx, portal_map* mp) {
    return mp->sectors[sector_idx*SECTOR_SIZE];
}

s16 sector_portal_offset(s16 sector_idx, portal_map* mp) {
    return mp->sectors[sector_idx*SECTOR_SIZE+1];
}

s16 sector_num_walls(s16 sector_idx, portal_map* mp) {
    return mp->sectors[sector_idx*SECTOR_SIZE+2];
}

s16 sector_floor_height(s16 sector_idx, portal_map* mp) {
    return mp->sectors[sector_idx*SECTOR_SIZE+3];
}

s16 sector_ceil_height(s16 sector_idx, portal_map* mp) {
    return mp->sectors[sector_idx*SECTOR_SIZE+4];
}

s16 sector_floor_color(s16 sector_idx, portal_map* mp) {
    return mp->sectors[sector_idx*SECTOR_SIZE+5];
}

s16 sector_ceil_color(s16 sector_idx, portal_map* mp) {
    return mp->sectors[sector_idx*SECTOR_SIZE+6];
}
