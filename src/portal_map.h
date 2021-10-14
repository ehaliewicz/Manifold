#ifndef PORTAL_MAP_H
#define PORTAL_MAP_H

#include <genesis.h>
#include "sector.h"
#include "texture.h"
#include "vertex.h"

#define SECTOR_SIZE 4
#define VERT_SIZE 2

#define MAX_SECTORS   128  // 4kB max
#define MAX_WALLS     256  // 4kB max
#define MAX_VERTEXES  256  // 8kB max
#define MAX_PORTALS   128  // 1kB max
#define MAX_TEXTURES   16  // 64 bytes, pointers into shared texture list
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


#define SECTOR_PARAM_LIGHT_IDX 0
#define SECTOR_PARAM_ORIG_HEIGHT_IDX 1
#define SECTOR_PARAM_STATE_IDX 2
#define SECTOR_PARAM_TICKS_LEFT_IDX 3
#define SECTOR_PARAM_FLOOR_HEIGHT_IDX 4
#define SECTOR_PARAM_CEIL_HEIGHT_IDX 5
#define SECTOR_PARAM_FLOOR_COLOR_IDX 6
#define SECTOR_PARAM_CEIL_COLOR_IDX 7

#define NUM_SECTOR_PARAMS 8
#define NUM_SECTOR_PARAMS_SHIFT 3

#define NUM_PVS_PARAMS 2
#define PVS_SHIFT 1

typedef struct {
    u16 num_sectors;
     u16 num_walls;
     u16 num_verts;
     s16* sectors;
     u8* sector_types;
     s16* sector_params;
     u16* walls;
     s16* portals;
     u8* wall_colors;
     vertex* vertexes;
     u8* wall_norm_quadrants;
     s16* pvs;
     u16* raw_pvs;
} portal_map;

s16* sector_data_start(s16 sector_idx, portal_map* mp);

s16 sector_wall_offset(s16 sector_idx, portal_map* mp);

s16 sector_portal_offset(s16 sector_idx, portal_map* mp);

s16 sector_num_walls(s16 sector_idx, portal_map* mp);

u16 sector_flags(s16 sector_idx, portal_map* mp);

#endif
