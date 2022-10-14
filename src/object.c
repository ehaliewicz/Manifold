#include <genesis.h>
#include "colors.h"
#include "collision.h"
#include "console.h"
#include "game.h"
#include "inventory.h"
#include "level.h"
#include "object.h"
#include "portal_map.h"
#include "utils.h"

static int next_obj_id = 0; 


#define MAX_OBJECTS 64

object* objects;

int *num_objs_in_sectors;



// setup objects
void init_object_lists(int num_sectors) {

    objects = malloc(sizeof(object)*MAX_OBJECTS, "object list");
    num_objs_in_sectors = malloc(sizeof(int)*num_sectors, "num objs in sectors");
    for(int i = 0; i < MAX_OBJECTS; i++) {
        objects[i].in_use = 0;
    }
    for(int i = 0; i < num_sectors; i++) {
        num_objs_in_sectors[i] = 0;
    }

} 


void clear_object_lists() {
    free(objects, "object list");
    free(num_objs_in_sectors, "num objs in sectors");
}

int object_sorts_smaller(object* a, object* b) {
    if (a->in_use && (!b->in_use)) {
        return 1;
    }
    if(a->pos.cur_sector < b->pos.cur_sector) {
        return 1;
    }
    return 0;
}

void sort_objects() {
    object obj_buf;
    for (int step = 1; step < MAX_OBJECTS; step++) {
        obj_buf = objects[step];
        int j = step - 1;

        // Compare key with each element on the left of it until an element smaller than
        // it is found.
        // For descending order, change key<array[j] to key>array[j].
        while (object_sorts_smaller(&obj_buf, &objects[j]) && j >= 0) {
            objects[j + 1] = objects[j];
            --j;
        }
        objects[j + 1] = obj_buf;
    }
}

void print_object_list() {
    for(int i = 0; i < MAX_OBJECTS; i++) {
        if(objects[i].in_use) {
            KLog_U2("obj: ", i, ", sector: ", objects[i].pos.cur_sector);
        }
    }
}


const object_template object_types[32+1] = {
    // first object type is player
    {.is_player = 1, .name = "player_object_type"},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    //{.init_state = 2,
    // .name = "blue keycard", 
    //.size = 20, .from_floor_draw_offset = 10<<4, .width=30, .height=46<<4},
    //{.init_state = 1,
    // .name = "red cube",
    //.size = 20, .from_floor_draw_offset = 20<<4, .width=12, .height=20<<4},
    //{.init_state = 1,
    // .name = "first enemy type ",
    //.size = 20, .from_floor_draw_offset = 10<<4, .width=46, .height=80<<4},
};

int look_for_player(object* cur_obj, uint16_t cur_sector);
int follow_player(object* cur_obj, uint16_t cur_sector);
int maybe_get_picked_up(object* cur_obj, uint16_t cur_sector);
int idle(object* cur_obj, uint16_t cur_sector);

obj_state object_state_machines[] = {
    {.next_state = 0, .ticks = 3, .action = &look_for_player, },
    {.next_state = 1, .ticks = 1, .action = &follow_player, },
    {.next_state = 2, .ticks = 1, .action = &maybe_get_picked_up, },
    {.next_state = 3, .ticks = 3, .action = &idle, },
};


fix32 dist_sqr(object_pos posa, object_pos origin) {
    //fix16Sqrt();
    fix16 dx1 = fix32ToFix16(posa.x - origin.x);
    fix16 dy1 = fix32ToFix16(posa.y - origin.y);
    return (dx1*dx1)+(dy1*dy1);
} 

// returns -1 if point 1 is closer than point 2, +1 if opposite, or 0 is they are the same distance
int is_closer(object_pos pos1, object_pos pos2, object_pos origin) {
  fix32 d1 = dist_sqr(pos1, origin);
  fix32 d2 = dist_sqr(pos2, origin);
  if(d1 == d2) {
    return 0;
  } else if (d1 < d2) {
    return -1;
  } else {
    return 1;
  }
} 

object* get_free_object() {
    for(int i = 0; i < MAX_OBJECTS; i++) {
        if(objects[i].in_use == 0) {
            return &objects[i];
        }
    }
    return NULL;
}

void free_object(object* o) {
    o->in_use = 0;
}

// inserts object in sorted position?
// not important
// first parameter used to be cur_player_pos
object* alloc_object_in_sector(u32 activate_tick, int sector_num, fix32 x, fix32 y, fix32 z, uint8_t object_type) {
    object* res = get_free_object();
    if (res == NULL) { 
        return NULL;
    }

    res->pos.cur_sector = sector_num;
    num_objs_in_sectors[sector_num]++;
    res->pos.x = x;
    res->pos.y = y;
    res->pos.z = z;
    res->id = next_obj_id++;
    volatile object_template* tmpl = &object_types[object_type];
    res->current_state = tmpl->init_state; // object_types[object_type].init_state;
    res->object_type = object_type;
    res->activate_tick = activate_tick;
    res->pos.cur_sector = sector_num;
    res->in_use = 1;

    return res;
} 


void move_object_to_sector(object* obj, u16 next_sector) {
    int old_sector = obj->pos.cur_sector;
    obj->pos.cur_sector = next_sector;
    num_objs_in_sectors[old_sector]--;
    num_objs_in_sectors[next_sector]++;
}


object* objects_in_sector(int sector_num) {
    if(num_objs_in_sectors[sector_num] == 0) {
        return NULL;
    }
    for(int i = 0; i < MAX_OBJECTS; i++) {
        if(objects[i].pos.cur_sector == sector_num) {
            return &objects[i];
        }
    }
    return NULL;
}

int idle(object* cur_obj, uint16_t cur_sector) {
    return 1; // stays alive
}

int look_for_player(object* cur_obj, uint16_t cur_sector) {
    if(cur_player_pos.cur_sector == cur_sector) {
        //cur_obj->object_type = 1;
        cur_obj->current_state++;
        cur_obj->tgt.sector = cur_sector;
        cur_obj->tgt.x = cur_player_pos.x;
        cur_obj->tgt.y = cur_player_pos.y;
        cur_obj->tgt.z = cur_player_pos.z;
    } else {
        //KLog("player not found");
    }
    return 1;
}



int maybe_get_picked_up(object* cur_obj, uint16_t cur_sector) {  
    object_pos pos = cur_obj->pos;
    int dx = fix32ToInt(cur_player_pos.x - pos.x);
    int dy = fix32ToInt(cur_player_pos.y - pos.y);
    
    u32 dist = fastLength(dx, dy);
    
    if(dist < 32) { 
        if(inventory_add_item(BLUE_KEY)) {
            char buf[50];
            int len = sprintf(buf, "picked up the %s!", object_types[cur_obj->object_type].name);
            console_push_message_high_priority(buf, len, 30);
        }

        return 0;
    }
    return 1;
}

int follow_player(object* cur_obj, uint16_t cur_sector) {
    object_pos pos = cur_obj->pos;
    // follow player
    //KLog("following player");

    int dx = fix32ToInt(cur_player_pos.x - pos.x);
    int dy = fix32ToInt(cur_player_pos.y - pos.y);


    //u32 dist = getApproximatedDistance(dx, dy);
    //if(dist < 8) { c }
    //int norm_dx = dx/dist;
    //int norm_dy = dy/dist;
    //dx = 7*norm_dx;
    //dy = 7*norm_dy;

    //int dz = fix32ToInt(cur_player_pos.z - pos.z);

    volatile object_template* obj_type = &object_types[cur_obj->object_type];

    s16 speed = obj_type->speed;
    if(dx > 0) {
        dx = min(speed, dx);
    } else if (dx < 0) {
        dx = max(-speed, dx);
    }

    if(dy > 0) {
        dy = min(speed, dy);
    } else {
        dy = max(-speed, dy);
    }
    

    /*
    int dist_sqr = (dx*dx)+(dy*dy);

    while (dist_sqr > 2) {
        dx >>= 1;
        dy >>= 1;
        dist_sqr = (dx*dx)+(dy*dy);
        KLog_U1("dist sqr: ", dist_sqr);
    }
    */

    fix32 dx32 = intToFix32(dx);
    fix32 dy32 = intToFix32(dy);

    //collision_result move_res = check_for_collision_radius(pos.x, pos.y, pos.x+dx32, pos.y+dy32, 8, cur_sector); // radius of 8 originally
    
    
    cur_obj->pos.x = pos.x+dx32;//move_res.pos.x;
    cur_obj->pos.y = pos.y+dy32;//move_res.pos.y;

    
    /*

    if(move_res.new_sector != cur_sector) {
        move_object_to_sector(cur_obj, move_res.new_sector);
        u16 sect = cur_obj->pos.cur_sector;
        u16 sect_group = sector_group(sect, cur_portal_map);
        cur_obj->pos.z = get_sector_group_floor_height(sect_group);
    }
    //u16 sect = cur_obj->pos.cur_sector;
    //u16 sect_group = sector_group(sect, cur_portal_map);
    //cur_obj->pos.z = get_sector_group_floor_height(sect_group);
    */
    return 1;
}



void process_all_objects(uint32_t cur_frame) {

   for(int i = 0; i < MAX_OBJECTS; i++) {
        if(objects[i].in_use) {
            object* cur_object = &objects[i];
            u16 sect = cur_object->pos.cur_sector;
            if(cur_object->activate_tick != 0) {
                    cur_object->activate_tick--;
            } else {
                KLog("processing object");
                uint16_t state_idx = cur_object->current_state;
                int live = object_state_machines[state_idx].action(cur_object, sect);
                if(!live) { 
                    free_object(cur_object);
                } else {
                    if(object_state_machines[state_idx].action == &idle) {
                        cur_object->activate_tick = 128;
                    } else {
                        cur_object->activate_tick = 1;
                    }
                }
            }
        }
   }
    
    KLog("done processing objects");
}