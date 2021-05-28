
#ifndef PORTAL_MAP_H
#define PORTAL_MAP_H

#include <genesis.h>
#include "vertex.h"
#include "vis_range.h"

#define SECTOR_SIZE 7
#define VERT_SIZE 2

typedef enum {
    QUADRANT_0,
    QUADRANT_1,
    QUADRANT_2,
    QUADRANT_3,
    FACING_UP,
    FACING_LEFT,
    FACING_DOWN,
    FACING_RIGHT
} normal_quadrant;

typedef struct {
    u8 upper_col;
    u8 mid_col;
    u8 lower_col;
} wall_col;

typedef enum {
    NO_TYPE = 0,
    FLASHING = 1,
} sector_type;

typedef struct {
    s8 light;
    s16 par2;
    s8 stash;
} sector_param;

typedef struct {
    const int num_sectors;
    const int num_walls;
    const int num_verts;
    const s16* sectors;
    const sector_type* sector_types;
    sector_param* sector_params;
    const u16* walls;
    const s16* portals;
    const wall_col* wall_colors;
    const vertex* vertexes;
    const vis_range* wall_vis_ranges;
    const u8* wall_norm_quadrants;
    const s16* floor_slopes;
    const s16* ceil_slopes;
} portal_map;

s16* sector_data_start(s16 sector_idx, portal_map* mp);

s16 sector_wall_offset(s16 sector_idx, portal_map* mp);

s16 sector_portal_offset(s16 sector_idx, portal_map* mp);

s16 sector_num_walls(s16 sector_idx, portal_map* mp);

s16 sector_floor_height(s16 sector_idx, portal_map* mp);

s16 sector_ceil_height(s16 sector_idx, portal_map* mp);

s16 sector_floor_color(s16 sector_idx, portal_map* mp);

s16 sector_ceil_color(s16 sector_idx, portal_map* mp);

s16 sector_sloped_start_wall_idx(s16 sector_idx, portal_map* mp);

s16 sector_floor_slope_start_wall_idx(s16 sector_idx, portal_map* mp);

// if -1, no slope
// if >=0, it designates the portal who's neighboring sector this sector is sloped towards
s16 sector_floor_slope_end_wall_idx(s16 sector_idx, portal_map* mp);

s16 sector_ceil_slope_start_wall_idx(s16 sector_idx, portal_map* mp);

s16 sector_ceil_slope_end_wall_idx(s16 sector_idx, portal_map* mp);

#endif
