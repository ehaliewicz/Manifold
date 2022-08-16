#include <genesis.h>
#include "math3d.h"
#include "portal_map.h"
#include "sector_group.h"

//const level* cur_level = NULL;
portal_map* cur_portal_map = NULL;


 void init_sector_parameters(portal_map* map) {
    
    u16 num_sector_groups = map->num_sector_groups;
    u16 num_bytes = num_sector_groups * sizeof(s16) * NUM_SECTOR_PARAMS;
    live_sector_group_parameters = MEM_alloc(num_bytes);
    memcpy(live_sector_group_parameters, cur_portal_map->sector_group_params, num_bytes);
 }

 void clean_sector_parameters() {
     MEM_free(live_sector_group_parameters);
     live_sector_group_parameters = NULL;
 }


u16 *wall_tex_repetitions;


void init_wall_tex_repetitions(portal_map* map) {
    u16 num_walls = map->num_walls;
    wall_tex_repetitions = MEM_alloc(sizeof(u16)*num_walls);
    u16 num_sectors = map->num_sectors;

    for(int sector = 0; sector < num_sectors; sector++) {
        u16 wall_offset = sector_wall_offset(sector, map);
        u16 sect_num_walls = sector_num_walls(sector, map);
        u16 portal_offset = sector_portal_offset(sector, map);
        for(int j = 0; j < sect_num_walls; j++) {
            u16 wall_idx = wall_offset+j;
            u16 portal_idx = portal_offset+j;

            u16 v1_idx = cur_portal_map->walls[wall_idx];
            u16 v2_idx = cur_portal_map->walls[wall_idx+1];

            vertex v1 = cur_portal_map->vertexes[v1_idx];
            vertex v2 = cur_portal_map->vertexes[v2_idx];
            u16 repetitions = get_texture_repetitions(v1.x, v1.y, v2.x, v2.y);


            wall_tex_repetitions[portal_idx] = repetitions;

        }
    }

}


void load_portal_map(portal_map* l) {
    cur_portal_map = l;
    init_sector_parameters(l);

    init_wall_tex_repetitions(l);

}

