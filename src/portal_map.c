#include <genesis.h>
#include "portal_map.h"

s16* sector_data_start(s16 sector_idx, portal_map* mp) {
    return (s16*)(&mp->sectors[sector_idx*SECTOR_SIZE]);
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

u16 sector_group(s16 sector_idx, portal_map* mp) {
    return mp->sectors[sector_idx*SECTOR_SIZE+3];
}

