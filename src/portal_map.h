#ifndef PORTAL_MAP_H
#define PORTAL_MAP_H

#include <genesis.h>

#include "object.h"
#include "sector_group.h"
#include "texture.h"
#include "vertex.h"

#define SECTOR_SIZE 4
#define VERT_SIZE 2

#define MAX_SECTORS      1024  // 4kB max
#define MAX_SECTOR_GROUPS 512
#define MAX_WALLS        1024  // 4kB max
#define MAX_VERTEXES     1024  // 8kB max
#define MAX_PORTALS       512  // 1kB max
#define MAX_TEXTURES       32  // 64 bytes, pointers into shared texture list

typedef enum {
    QUADRANT_0=0,
    QUADRANT_1=1,
    QUADRANT_2=2,
    QUADRANT_3=3,
    FACING_UP=4,
    FACING_LEFT=5,
    FACING_DOWN=6,
    FACING_RIGHT=7
} normal_quadrant;

typedef struct {
    u8 upper_col;
    u8* texture;
    u8 lower_col;
} wall_col;

#define WALL_TEXTURE_IDX 0
#define WALL_HIGH_COLOR_IDX 1
#define WALL_LOW_COLOR_IDX 2
#define WALL_SOLID_COLOR_IDX 3

#define WALL_COLOR_NUM_PARAMS 4
#define WALL_COLOR_NUM_PARAMS_SHIFT 2





#define NUM_PVS_PARAMS 2
#define PVS_SHIFT 1

typedef struct __attribute__((__packed__)) {
    const u16 bunch_offset;
    const u8 num_bunches;
} pvs_bunch_group;

typedef struct __attribute__((__packed__)) {
    const u16 sector_num;
    const u16 wall_offset;
    const u8 num_walls;
} pvs_bunch_entry;


// in total 1088 bytes

typedef struct __attribute__((__packed__)){
    const u16 num_sector_groups;
    const u16 num_sectors;
    const u16 num_walls;
    const u16 num_verts;
    const s16* sectors;
    const u8* sector_group_types;
    const s16* sector_group_params;

    // each sector has a list of 8 s16s
    // the first is the trigger type, and the remaining 7 are sector group targets
    const s16* sector_group_triggers;

    const u16* walls;

    //const u16* collision_hull_walls; // 
    
    //const s16* wall_dxs;
    //const s16* wall_dys;
    //const s16* wall_collision_hull;

    const s16* portals;
    const u8* wall_colors;
    const u8* wall_tex_repetitions;
    const vertex* vertexes;
    const vertex* collision_vertexes;
    const u8* wall_norm_quadrants;
    const u16 has_pvs;
    const u16* pvs;
    const u16* raw_pvs;

    const u16* pvs_offsets;
    const s16* sector_list_offsets;
    const u16* wall_pvs; // each index from above points to a list consisting of N:num_sectors, N 32-bit bitmaps


    const pvs_bunch_group* pvs_bunch_groups;
    const pvs_bunch_entry* pvs_bunch_entries;
    
    const u32* sector_pvs_offsets; // offsets rle-encoded sector pvs
    const s8* sector_pvs_entries; 
    const u32* sector_phs_offsets;
    const s8* sector_phs_entries;
    char* name;
    void* xgm_track;
    u16* palette;
    u16 num_things;
    map_object* things;
} portal_map;

s16* sector_data_start(s16 sector_idx, portal_map* mp);

s16 sector_wall_offset(s16 sector_idx, portal_map* mp);

s16 sector_portal_offset(s16 sector_idx, portal_map* mp);

s16 sector_num_walls(s16 sector_idx, portal_map* mp);

u16 sector_group(s16 sector_idx, portal_map* mp);

u8 sector_in_pvs(u16 src_sector, u16 check_sector, portal_map* mp);

u8 sector_in_phs(u16 src_sector, u16 check_sector, portal_map* mp);

void run_in_pvs(u16 src_sector, void (*sect_func)(u16), portal_map* mp);

void run_in_phs(u16 src_sector, void (*sect_func)(u16), portal_map* mp);

#endif
