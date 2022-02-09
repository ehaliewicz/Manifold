#include <genesis.h>
#include "portal_map.h"
#include "sector.h"

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


void load_portal_map(portal_map* l) {
    cur_portal_map = l;
    init_sector_parameters(l);
}

