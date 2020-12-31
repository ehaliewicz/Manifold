#include <genesis.h>
#include "portal_map.h"


s16 sector_wall_offset(s16 sector_idx, portal_map* mp) {
    return mp->sectors[sector_idx*4];
}

s16 sector_num_walls(s16 sector_idx, portal_map* mp) {
    return mp->sectors[sector_idx*4+1];
}

s16 sector_floor_height(s16 sector_idx, portal_map* mp) {
    return mp->sectors[sector_idx*4+2];
}

s16 sector_ceil_height(s16 sector_idx, portal_map* mp) {
    return mp->sectors[sector_idx*4+3];
}

