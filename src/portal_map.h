#ifndef PORTAL_MAP_H
#define PORTAL_MAP_H

#include <genesis.h>
#include "sector.h"
#include "texture.h"
#include "vertex.h"
#include "vis_range.h"

#define SECTOR_SIZE 8
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
    u8* texture;
    u8 lower_col;
} wall_col;

#define WALL_TEXTURE_IDX 0
#define WALL_HIGH_COLOR_IDX 1
#define WALL_LOW_COLOR_IDX 2

#define WALL_COLOR_NUM_PARAMS 4
#define WALL_COLOR_NUM_PARAMS_SHIFT 2

typedef struct {
    const u16 num_sectors;
    const u16 num_walls;
    const u16 num_verts;
    s16* sectors;
    const sector_type* sector_types;
    const s16* sector_params;
    const u16* walls;
    const s16* portals;
    const u8* wall_colors;
    const vertex* vertexes;
    const vis_range* wall_vis_ranges;
    const u8* wall_norm_quadrants;
    const u8* pvs;  // max of 32 sectors for now
    const u8* portal_contributes;
    const lit_texture** textures;
} portal_map;

s16* sector_data_start(s16 sector_idx, portal_map* mp);

s16 sector_wall_offset(s16 sector_idx, portal_map* mp);

s16 sector_portal_offset(s16 sector_idx, portal_map* mp);

s16 sector_num_walls(s16 sector_idx, portal_map* mp);

void set_sector_floor_height(s16 sector_idx, portal_map* mp, s16 height);
s16 sector_floor_height(s16 sector_idx, portal_map* mp);

void set_sector_ceil_height(s16 sector_idx, portal_map* mp, s16 height);
s16 sector_ceil_height(s16 sector_idx, portal_map* mp);

s16 sector_floor_color(s16 sector_idx, portal_map* mp);

s16 sector_ceil_color(s16 sector_idx, portal_map* mp);

u16 sector_flags(s16 sector_idx, portal_map* mp);

#endif
