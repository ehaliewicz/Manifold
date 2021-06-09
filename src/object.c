#include <genesis.h>
#include "colors.h"
#include "collision.h"
#include "debug.h"
#include "game.h"
#include "object.h"


object* free_object_list;

int num_sector_lists;
object** sector_lists;

void look_for_player(object* cur_obj, uint16_t cur_sector) {
    if(cur_player_pos.cur_sector == cur_sector) {
        cur_obj->object_type = 1;
        cur_obj->current_state++;
    }
}

void follow_player(object* cur_obj, uint16_t cur_sector) {
    object_pos pos = cur_obj->pos;
    // follow player
    
    int dx = fix32ToInt(cur_player_pos.x - pos.x);
    int dy = fix32ToInt(cur_player_pos.y - pos.y);
    //int dz = fix32ToInt(cur_player_pos.z - pos.z);

    if(dx > 0) {
        dx = min(4, dx);
    } else if (dx < 0) {
        dx = max(-4, dx);
    }

    if(dy > 0) {
        dy = min(4, dy);
    } else {
        dy = max(-4, dy);
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

    collision_result move_res = check_for_collision_radius(pos.x, pos.y, pos.x+dx32, pos.y+dy32, 10, cur_sector);
    
    cur_obj->pos.x = move_res.pos.x;
    cur_obj->pos.y = move_res.pos.y;


    //if(dz > 0) {
    //    cur_obj->pos.z -= FIX32(.1); // intToFix32(min(abs(dz), 4));
    //} else if (dz < 0) {
    //    cur_obj->pos.z += FIX32(.1); // intToFix32(min(abs(dz), 4));
    //}

    if(move_res.new_sector != cur_sector) {
        move_object_to_sector(cur_obj, move_res.new_sector);
        cur_obj->pos.cur_sector = cur_sector;
    }
}


const object_template object_types[2] = {
    {.init_state = 0,
     .sprite_col = ((BLUE_IDX << 4) | BLUE_IDX), 
    .size = 20, .from_floor_draw_offset = 20<<4, .width=12, .height=20<<4},
    {.init_state = 1,
     .sprite_col = ((RED_IDX << 4) | RED_IDX), 
    .size = 20, .from_floor_draw_offset = 20<<4, .width=12, .height=20<<4},
};

obj_state object_state_machines[] = {
    {.next_state = 0, .ticks = 3, &look_for_player, },
    {.next_state = 1, .ticks = 1, &follow_player, }
};


#define MAX_NUM_OBJECTS 128

void init_object_lists(int num_sectors) {
    num_sector_lists = num_sectors;
    free_object_list = MEM_alloc(sizeof(object)*MAX_NUM_OBJECTS);

    sector_lists = MEM_alloc(sizeof(object*)*num_sectors);

    for(int i = 0; i < num_sectors; i++) {
        sector_lists[i] = NULL;
    }


    for(int i = 0; i < MAX_NUM_OBJECTS; i++) {
        object* prev = (i == 0) ? NULL : &(free_object_list[i-1]);
        object* next = (i == (MAX_NUM_OBJECTS-1)) ? NULL : &(free_object_list[i+1]);
        free_object_list[i].prev = prev;
        free_object_list[i].next = next;
    }
}

void clear_object_lists() {
    MEM_free(free_object_list);
    MEM_free(sector_lists);
}

object* alloc_object_in_sector(int sector_num, uint8_t object_type) {
    if(free_object_list == NULL) {
        return NULL;
    }
    object* res = free_object_list;
    object* next_free = res->next;
    if(next_free != NULL) { next_free->prev = NULL; }
    free_object_list = next_free;

    res->next = sector_lists[sector_num];
    res->prev = NULL;
    if(res->next != NULL) {
        res->next->prev = res;
    }

    res->current_state = object_types[object_type].init_state;
    res->object_type = object_type;
    res->activate_tick = 0;
    res->pos.cur_sector = sector_num;

    sector_lists[sector_num] = res;


    return res;
}


void move_object_to_sector(object* obj, int next_sector) {
    int old_sector = obj->pos.cur_sector;

    object* next = obj->next;
    object* prev = obj->prev;
    obj->prev = NULL;
    obj->next = NULL;
    obj->pos.cur_sector = next_sector;
    
    if(next != NULL) {
        next->prev = prev;
    }
    if(prev != NULL) {
        prev->next = next;
    }

    if(prev == NULL) {
        sector_lists[old_sector] = next;
    }
    obj->next = sector_lists[next_sector];
    if(obj->next != NULL) {
        obj->next->prev = obj;
    }
    sector_lists[next_sector] = obj;

    //char buf[32];
    //sprintf(buf, "move obj to sect %i ", next_sector);
    //VDP_drawTextBG(BG_B, buf, 10, 10);
    //sprintf(buf, "old sect %i",  )
    //die(buf);

}

void free_object(object* obj) {
    int old_sector = obj->pos.cur_sector;
    object* next = obj->next;
    object* prev = obj->prev;
    if(next != NULL) {
        next->prev = prev;
    }
    if(prev != NULL) {
        prev->next = next;
    } else {    
        sector_lists[old_sector] = next;
    }
    object* next_free = free_object_list;
    if(next_free != NULL) {
        next_free->prev = obj;
    }
    obj->next = next_free;
    free_object_list = obj;
}

object* objects_in_sector(int sector_num) {
    return sector_lists[sector_num];
}

void process_all_objects(uint32_t cur_frame) {
    for(int sect = 0; sect < num_sector_lists; sect++) {
        object* cur_object = sector_lists[sect];
        while(cur_object != NULL) {
            uint16_t state_idx = cur_object->current_state;
            object_state_machines[state_idx].action(cur_object, sect);
            //cur_object = cur_object->next;
            break;
        }
    }
}