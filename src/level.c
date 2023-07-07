#include <genesis.h>
#include "collision.h"
#include "console.h"
#include "game.h"
#include "inventory.h"
#include "map_table.h"
#include "math3d.h"
#include "object.h"
#include "portal_map.h"
#include "sector_group.h"
#include "utils.h"

//const level* cur_level = NULL;
portal_map* cur_portal_map = NULL;


 void init_sector_parameters(portal_map* map) {
    
    u16 num_sector_groups = map->num_sector_groups;
    u16 num_bytes = num_sector_groups * sizeof(s16) * NUM_SECTOR_GROUP_PARAMS;
    live_sector_group_parameters = malloc(num_bytes, "live sector group params table");
    memcpy(live_sector_group_parameters, cur_portal_map->sector_group_params, num_bytes);
 }

 void clean_sector_parameters() {
    free(live_sector_group_parameters, "live sector group params table");
    live_sector_group_parameters = NULL;
 }



Vect2D_f32 get_sector_center(int i) {
    Vect2D_f32 res;
    int avg_sect_x = 0;
    int avg_sect_y = 0;
    int num_walls = sector_num_walls(i, (portal_map*)cur_portal_map);
    int wall_offset = sector_wall_offset(i, (portal_map*)cur_portal_map);

    for(int j = 0; j < num_walls; j++) {
        int vidx = cur_portal_map->walls[wall_offset+j];
        avg_sect_x += cur_portal_map->vertexes[vidx].x;
        avg_sect_y += cur_portal_map->vertexes[vidx].y;
    }
    avg_sect_x /= num_walls;
    avg_sect_y /= num_walls;
    res.x = intToFix32(avg_sect_x);
    res.y = intToFix32(avg_sect_y);
    
    return res;
}

void init_player_pos() {
    const int init_player_sector = 0;
    
    Vect2D_f32 sector_0_center = get_sector_center(init_player_sector); // get_sector_center(0);

    cur_player_pos.x = sector_0_center.x;
    cur_player_pos.y = sector_0_center.y;
    cur_player_pos.cur_sector = init_player_sector;

    u16 sect_group = sector_group(cur_player_pos.cur_sector, cur_portal_map);

    cur_player_pos.z = (get_sector_group_floor_height(sect_group)<<(FIX32_FRAC_BITS-4));// + FIX32(50);


    cur_player_pos.ang = 0;
}


void init_objects() {
    int got_player_thing = 0;
    map_object* player_thing = NULL;

    KLog_U1("allocating objects: ", cur_portal_map->num_things);

    for(int i = 0; i < cur_portal_map->num_things; i++) {
        map_object* thg = &cur_portal_map->things[i];
        //KLog_U1("thing type: ", thg->type);
        //KLog_U1("in sector: ", )
        u16 obj_sect_group = sector_group(thg->sector_num, cur_portal_map);

    //cur_player_pos.z = (cur_sector_height<<(FIX32_FRAC_BITS-4)) + FIX32(50);   
        volatile object_template* typ = &object_types[thg->type];
        if(thg->type == 0) { //typ->is_player) {
            //KLog("is player?");
            got_player_thing = 1;
            player_thing = thg;
        } else {
            //KLog_U1("type: ", thg->type);
            s16 cur_sector_height = get_sector_group_floor_height(obj_sect_group);
            if(typ->init_state == IDLE_STATE) {
                alloc_decoration_in_sector(thg->sector_num, thg->x, thg->y, cur_sector_height, thg->type);
            } else {
                alloc_object_in_sector(
                    i&1, // either 0 or 1, to spread the load
                    thg->sector_num,
                    thg->x<<FIX32_FRAC_BITS, 
                    thg->y<<FIX32_FRAC_BITS, 
                    cur_sector_height, //(cur_sector_height<<(FIX32_FRAC_BITS-4)), // + FIX32(50);   
                    //thg->z<<FIX32_FRAC_BITS, 
                    thg->type
                );
            }
        }
    }
}



void clean_portal_map() {
    clean_sector_parameters();
}

#define MIN_ACTIVATE_DIST 40
#define MIN_ACTIVATE_DIST_SQ FIX32((MIN_ACTIVATE_DIST*MIN_ACTIVATE_DIST))



int check_trigger_switch(player_pos* pos) {
    portal_map* map = cur_portal_map;
    u16 sector = pos->cur_sector;
    s16 wall_offset = sector_wall_offset(sector, map);
    s16 portal_offset = sector_portal_offset(sector, map);
    s16 num_walls = sector_num_walls(sector, map);
    s32 px = pos->x;
    s32 py = pos->y;

    int got_door = 0;
    s32 closest_dist = MAX_S32;

    u16 tgt_sect_group = 0;
    u8 tgt_sect_group_type = 0;

    for(int i = 0; i < num_walls; i++) {
        s16 portal = map->portals[portal_offset+i];
        if(portal == -1) {
            continue;
        }
        // check if this is a portal to a door or lift sector
        u16 next_sect_group = sector_group(portal, map);

        u8 next_sect_group_type = map->sector_group_types[next_sect_group];

        u8 next_sect_group_type_low_bits = GET_SECTOR_GROUP_TYPE(next_sect_group_type);

        if(next_sect_group_type_low_bits != DOOR && next_sect_group_type_low_bits != LIFT) {
            continue;
        }


        u16 v1_idx = map->walls[wall_offset+i];
        u16 v2_idx = map->walls[wall_offset+i+1];
        vertex v1 = map->vertexes[v1_idx];
        vertex v2 = map->vertexes[v2_idx];

        f32 dist_sqr = sq_shortest_dist_to_point(px, py, v1, v2);
        //KLog("dist sqr: ", dist_sqr);
        if(dist_sqr < closest_dist && dist_sqr < MIN_ACTIVATE_DIST_SQ) {
            got_door = 1;
            closest_dist = dist_sqr;
            tgt_sect_group = next_sect_group;
            tgt_sect_group_type = next_sect_group_type;
        }
    }

    if(got_door) {

        u8 tgt_sect_group_type_low_bits = GET_SECTOR_GROUP_TYPE(tgt_sect_group_type);
        switch (GET_SECTOR_GROUP_KEYCARD(tgt_sect_group_type)) {
            case BLUE_KEYCARD:
                if (!inventory_has_item(BLUE_KEY)) {
                    console_push_message("You need the blue key!", 22, 20);
                    return 0;
                }
                break;
            case RED_KEYCARD:
                if(!inventory_has_item(RED_KEY)) {
                    console_push_message("You need the red key!", 21, 20);
                    return 0;
                }
                break;
            case GREEN_KEYCARD:
                if(!inventory_has_item(GREEN_KEY)) {
                    console_push_message("You need the green key!", 23, 20);
                    return 0;
                }
                break;
            default:
                break;
        }

        if(tgt_sect_group_type_low_bits == DOOR) {
            set_sector_group_state(tgt_sect_group, GOING_UP);
        } else if (tgt_sect_group_type_low_bits == LIFT) {
            set_sector_group_state(tgt_sect_group, GOING_DOWN);
        }
    }
    return got_door;
}


void load_portal_map(portal_map* l) {
    cur_portal_map = l;

    init_sector_parameters(l);

    init_player_pos();    
   
    //if(cur_portal_map->palette != NULL) {
    //    KLog("LOADING MAP PALETTE");
    //    PAL_setPalette(PAL1, cur_portal_map->palette);
    //}
    
    init_object_lists(cur_portal_map->num_sectors);

    init_objects();

    if(cur_portal_map->xgm_track != NULL) {
        XGM_setLoopNumber(255);
        XGM_startPlay(cur_portal_map->xgm_track);
        XGM_setLoopNumber(255);
    } 
}