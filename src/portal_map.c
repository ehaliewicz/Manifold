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

u16 sector_flags(s16 sector_idx, portal_map* mp) {
    return mp->sectors[sector_idx*SECTOR_SIZE+7];
}

s16 sector_floor_slope_start_wall_idx(s16 sector_idx, portal_map* mp) {
    return mp->floor_slopes[sector_idx*2];
}

s16 sector_floor_slope_end_wall_idx(s16 sector_idx, portal_map* mp) {
    return mp->floor_slopes[sector_idx*2+1];
}

s16 sector_ceil_slope_start_wall_idx(s16 sector_idx, portal_map* mp) {
    return mp->ceil_slopes[sector_idx*2];
}

s16 sector_ceil_slope_end_wall_idx(s16 sector_idx, portal_map* mp) {
    return mp->ceil_slopes[sector_idx*2+1];
}