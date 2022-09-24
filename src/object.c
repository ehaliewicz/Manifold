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

static object* objects;
static object *free_list = NULL; 

static object** sector_lists;

static int num_sector_lists;

object* pop(object** lst) {
  object* head = *lst;
  if(head == NULL) {
    return NULL;
  }

  object* next = head->next;
  if(next != NULL) {
    next->prev = NULL;
  }


  *lst = next; // set list to next

  head->next = NULL;
  head->prev = NULL;
  return head;
}

void push_to_front(object* new_head, object** lst) {
    object* prev_head = *lst;
    new_head->next = prev_head;
    if(prev_head != NULL) {
        prev_head->prev = new_head;
    }
    *lst = new_head;
}

void push(object* new_head, object** lst) {
  object* head = *lst;
  new_head->next = head;
  if(head != NULL) {

    // this handles if we're pushing in the middle of a list
    if(head->prev != NULL) {
      new_head->prev = head->prev;
      head->prev->next = new_head;
    }

    head->prev = new_head;
  }
  *lst = new_head;
}

void swap_with_prev(object* o) {
  object* prev = o->prev;
  object* next = o->next;

  o->prev = NULL;
  o->next = o;


  if(prev != NULL) {
    object* prev_prev = prev_prev;
    o->prev = prev_prev;
    if(prev_prev != NULL) {
      prev->prev->next = o;
    }
    prev->prev = o;
    prev->next = next;
  }


  if(next != NULL) {
    next->prev = prev;
  }
} 

// setup objects
void init_object_lists(int num_sectors) {

    num_sector_lists = num_sectors;
    objects = malloc(sizeof(object)*MAX_OBJECTS, "object list");

    sector_lists = malloc(sizeof(object*)*num_sectors, "sector object lists");
    for(int i = 0; i < MAX_OBJECTS; i++) {
        objects[i].prev = (i == 0 ? NULL : &(objects[i-1]));
        objects[i].next = (i == (MAX_OBJECTS-1) ? NULL : &(objects[i+1]));
    }
    free_list = &objects[0];

    for(int i = 0; i < num_sectors; i++) {
        sector_lists[i] = NULL;
    }
} 


void clear_object_lists() {
    free(objects, "object list");
    free(sector_lists, "sector object lists");
}



const object_template object_types[3] = {
    {.init_state = 2,
     .sprite_col = ((1 << 4) | 1), 
     .name = "blue keycard", 
    .size = 20, .from_floor_draw_offset = 10<<4, .width=30, .height=46<<4},
    {.init_state = 1,
     .sprite_col = ((RED_IDX << 4) | RED_IDX), 
     .name = "red cube",
    .size = 20, .from_floor_draw_offset = 20<<4, .width=12, .height=20<<4},
    {.init_state = 1,
     .sprite_col = ((RED_IDX << 4) | RED_IDX), 
     .name = "first enemy type ",
    .size = 20, .from_floor_draw_offset = 10<<4, .width=46, .height=80<<4},
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


// inserts object in sorted position
object* alloc_object_in_sector(object_pos cur_player_pos, int sector_num, fix32 x, fix32 y, fix32 z, uint8_t object_type) {
    object* res = pop(&free_list);
    if(res == NULL) {
        return NULL;
    }


    res->pos.cur_sector = sector_num;
    res->pos.x = x;
    res->pos.y = y;
    res->pos.z = z;
    res->id = next_obj_id++;

    res->current_state = object_types[object_type].init_state;
    res->object_type = object_type;
    res->activate_tick = 0;
    res->pos.cur_sector = sector_num;

    object** sector_list_obj = &sector_lists[sector_num];
    /*
    while((*sector_list_obj) != NULL) {
        break; // TODO why does this cause such a performance hit if it's executed just once?
        object* cur_sect_obj = *sector_list_obj;
        if(is_closer(res->pos, cur_sect_obj->pos, cur_player_pos) == -1) {
        // this object is the closest
            break;
        }
        sector_list_obj = &(cur_sect_obj->next);
    }
    */
    //return NULL;

  push_to_front(res, sector_list_obj);
  return res;
} 


void move_object_to_sector(object* obj, u16 next_sector) {

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


/*
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
*/

object* objects_in_sector(int sector_num) {
    return sector_lists[sector_num];
}


int look_for_player(object* cur_obj, uint16_t cur_sector) {
    if(cur_player_pos.cur_sector == cur_sector) {
        cur_obj->object_type = 1;
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

    
    if(dx > 0) {
        dx = min(3, dx);
    } else if (dx < 0) {
        dx = max(-3, dx);
    }

    if(dy > 0) {
        dy = min(3, dy);
    } else {
        dy = max(-3, dy);
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

    collision_result move_res = check_for_collision_radius(pos.x, pos.y, pos.x+dx32, pos.y+dy32, 8, cur_sector); // radius of 8 originally
    
    
    cur_obj->pos.x = move_res.pos.x;
    cur_obj->pos.y = move_res.pos.y;


    //if(dz > 0) {
    //    cur_obj->pos.z -= FIX32(.1); // intToFix32(min(abs(dz), 4));
    //} else if (dz < 0) {
    //    cur_obj->pos.z += FIX32(.1); // intToFix32(min(abs(dz), 4));
    //}

    if(move_res.new_sector != cur_sector) {
        move_object_to_sector(cur_obj, move_res.new_sector);
    }
    u16 sect = cur_obj->pos.cur_sector;
    u16 sect_group = sector_group(sect, cur_portal_map);
    cur_obj->pos.z = get_sector_group_floor_height(sect_group);
    return 1;
}


obj_state object_state_machines[] = {
    {.next_state = 0, .ticks = 3, .action = &look_for_player, },
    {.next_state = 1, .ticks = 1, .action = &follow_player, },
    {.next_state = 2, .ticks = 1, .action = &maybe_get_picked_up, },
};


void process_all_objects(uint32_t cur_frame) {
    /*
    for(int sect = 0; sect < num_sector_lists; sect++) {
        object* cur_object = sector_lists[sect];
        if(cur_object != NULL) {
            KLog_U2("got object: ", cur_object->id, " in sector: ", sect);
            KLog_U2("object reported sector: ", cur_object->pos.cur_sector, ", ", 0);
        }
    }
    */
    for(int sect = 0; sect < num_sector_lists; sect++) {
        object* cur_object = sector_lists[sect];
        while(cur_object != NULL) {
            uint16_t state_idx = cur_object->current_state;
            int live = object_state_machines[state_idx].action(cur_object, sect);
            if(!live) { 
                // TODO: make work with more than one object
                // this will only work with a single object
                push_to_front(cur_object, &free_list);
                sector_lists[sect] = NULL;
            }
            cur_object = cur_object->next;
            break;
        }
    }
}