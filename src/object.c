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

static u8 next_object_fingerprint = 0; 


object* objects;
decoration_object* decorations;

static object_link object_free_list = NULL_OBJ_LINK; 
static decoration_link decoration_free_list = NULL_DEC_LINK;

static object_link* sector_object_lists;
static decoration_link* sector_decoration_lists;

static int num_sector_lists;

z_buf_obj *z_sort_buf;//[64];
buf_obj *obj_sort_buf;//[64];

object_link new_object_link() {
  object_link head = object_free_list;
  if(head == NULL_OBJ_LINK) {
    return NULL_OBJ_LINK;
  }

  object_link next = OBJ_LINK_DEREF(head).next;
  if(next != NULL_OBJ_LINK) {
    OBJ_LINK_DEREF(next).prev = NULL_OBJ_LINK;
  }

  object_free_list = next; // set list to next

  OBJ_LINK_DEREF(head).next = NULL_OBJ_LINK;
  OBJ_LINK_DEREF(head).prev = NULL_OBJ_LINK;
  return head;
}

decoration_link new_decoration_link() {
  decoration_link head = decoration_free_list;
  if(head == NULL_DEC_LINK) {
    return NULL_DEC_LINK;
  }

  decoration_link next = DEC_LINK_DEREF(head).next;
  decoration_free_list = next; // set list to next

  DEC_LINK_DEREF(head).next = NULL_DEC_LINK;
  return head;
}

void push_object_to_front(object_link new_head, object_link* lst) {
    object_link prev_head = *lst;
    OBJ_LINK_DEREF(new_head).next = prev_head;
    //KLog_U2("attaching object: ", new_head, " to previous head link: ", prev_head);
    if(prev_head != NULL_OBJ_LINK) {
        OBJ_LINK_DEREF(prev_head).prev = new_head;
    }
    *lst = new_head;
}

void push_decoration_to_front(decoration_link new_head, decoration_link* lst) {
    decoration_link prev_head = *lst;
    //KLog_U2("attaching decoration: ", new_head, " to previous head link: ", prev_head);
    DEC_LINK_DEREF(new_head).next = prev_head;
    *lst = new_head;
}

// setup objects
void init_object_lists(int num_sectors) {


    num_sector_lists = num_sectors;
    //KLog_U1("size of object: ", sizeof(object));
    objects = malloc(sizeof(object)*MAX_OBJECTS, "object list");
    sector_object_lists = malloc(sizeof(object_link)*num_sectors, "sector object lists");
    for(int i = 0; i < MAX_OBJECTS; i++) {
        objects[i].prev = (i == 0 ? NULL_OBJ_LINK : i-1);
        objects[i].next = (i == (MAX_OBJECTS-1) ? NULL_OBJ_LINK : i+1);
    }
    object_free_list = 0; // start object link

    decorations = malloc(sizeof(decoration_object)*MAX_DECORATIONS, "decoration list");
    sector_decoration_lists = malloc(sizeof(decoration_link)*num_sectors, "sector decoration lists");
    for(int i = 0; i < MAX_DECORATIONS; i++) {
        decorations[i].next = (i == (MAX_DECORATIONS-1) ? NULL_DEC_LINK : i+1);
    }
    decoration_free_list = 0; // start decoration link :^)

    for(int i = 0; i < num_sectors; i++) {
        sector_object_lists[i] = NULL_OBJ_LINK;
        sector_decoration_lists[i] = NULL_DEC_LINK;
    }

    z_sort_buf = malloc(sizeof(z_buf_obj) * OBJ_SORT_BUF_SZ, "object z sort buffer");
    obj_sort_buf = malloc(sizeof(buf_obj) * OBJ_SORT_BUF_SZ, "object sort buffer");
} 


void clean_object_lists() {
    free(objects, "object list");
    free(decorations, "decoration list");
    free(sector_object_lists, "sector object lists");
    free(sector_decoration_lists, "sector decoration lists");
    free(z_sort_buf, "object z sort buffer");
    free(obj_sort_buf, "object sort buffer");
}



const object_template object_types[32+1] = {
    // first object type is player
    {.is_player = 0, .name = "player_object_type", .type = OBJECT},
    {.init_state = 0,
     .name = "claw guy", //.sprite=&claw_guy_sprite,
    .from_floor_draw_offset = 10<<4, .width=46, .height=80<<4,
    .type = OBJECT},
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

int look_for_player(object_link cur_obj, uint16_t cur_sector);
int follow_player(object_link cur_obj, uint16_t cur_sector);
int maybe_get_picked_up(object_link cur_obj, uint16_t cur_sector);
int idle(object_link cur_obj, uint16_t cur_sector);

obj_state object_state_machines[] = {
    {.next_state = 0, .ticks = 3, .action = &idle, },
    {.next_state = 1, .ticks = 3, .action = &look_for_player, },
    {.next_state = 2, .ticks = 1, .action = &follow_player, },
    {.next_state = 3, .ticks = 1, .action = &maybe_get_picked_up, },
};


fix32 dist_sqr(player_pos posa, player_pos origin) {
    //fix16Sqrt();
    fix16 dx1 = fix32ToFix16(posa.x - origin.x);
    fix16 dy1 = fix32ToFix16(posa.y - origin.y);
    return (dx1*dx1)+(dy1*dy1);
} 

// returns -1 if point 1 is closer than point 2, +1 if opposite, or 0 is they are the same distance
int is_closer(player_pos pos1, player_pos pos2, player_pos origin) {
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


object_link alloc_object_in_sector(u8 activate_tick, int sector_num, fix32 x, fix32 y, s16 z, uint8_t object_type) {
    KLog_U1("allocating object in sector: ", sector_num);
    object_link res = new_object_link();
    if(res == NULL_OBJ_LINK) {
        KLog("free list pop is null? wtf");
        return NULL_OBJ_LINK;
    }

    OBJ_LINK_DEREF(res).x = x;
    OBJ_LINK_DEREF(res).y = y;
    OBJ_LINK_DEREF(res).z = z;
    //OBJ_LINK_DEREF(res).fingerprint = next_object_fingerprint++;

    volatile object_template* tmpl = &object_types[object_type];
    OBJ_LINK_DEREF(res).current_state = tmpl->init_state; // object_types[object_type].init_state;
    OBJ_LINK_DEREF(res).object_type = object_type;
    OBJ_LINK_DEREF(res).activate_tick = activate_tick;


    object_link* sector_list_ptr = &sector_object_lists[sector_num];

    push_object_to_front(res, sector_list_ptr);
    return res;
} 

decoration_link alloc_decoration_in_sector(int sector_num, s16 x, s16 y, s16 z, uint8_t object_type) {
    //KLog_U1("allocating decoration in sector: ", sector_num);
    decoration_link res = new_decoration_link();
    if(res == NULL_DEC_LINK) {
        KLog("free list pop is null? wtf");
        return NULL_DEC_LINK;
    }

    DEC_LINK_DEREF(res).x = x;
    DEC_LINK_DEREF(res).y = y;
    DEC_LINK_DEREF(res).z = z;
    //OBJ_LINK_DEREF(res).fingerprint = next_object_fingerprint++;

    DEC_LINK_DEREF(res).object_type = object_type;


    decoration_link* sector_list_ptr = &sector_decoration_lists[sector_num];

    push_decoration_to_front(res, sector_list_ptr);
    return res;
} 

void free_object(object_link link, u16 object_sector) {
    if(link == NULL_OBJ_LINK) {
        KLog("its null?");
        return;
    }
    object_link next = OBJ_LINK_DEREF(link).next;
    object_link prev = OBJ_LINK_DEREF(link).prev;
    // patch next object if exists
    if(next != NULL_OBJ_LINK) {
        OBJ_LINK_DEREF(next).prev = prev;
    }
    // patch prev object if exists
    if(prev != NULL_OBJ_LINK) {
        OBJ_LINK_DEREF(prev).next = next;
    } else {

        sector_object_lists[object_sector] = next;
    }
    // clear prev link
    OBJ_LINK_DEREF(link).prev = NULL_OBJ_LINK;
    // attach to head of free list
    OBJ_LINK_DEREF(link).next = object_free_list;
    object_free_list = link;
}


void free_decoration(decoration_link link, u16 deco_sector) {
    if(link == NULL_DEC_LINK) {
        return;
    }
    if (sector_decoration_lists[deco_sector] == link) {
        sector_decoration_lists[deco_sector] = DEC_LINK_DEREF(link).next;
    }
    // attach to head of free list
    DEC_LINK_DEREF(link).next = decoration_free_list;
    decoration_free_list = link;
}


void move_object_to_sector(object_link obj, u16 old_sector, u16 next_sector) {


    object_link next = OBJ_LINK_DEREF(obj).next;
    object_link prev = OBJ_LINK_DEREF(obj).prev;
    OBJ_LINK_DEREF(obj).prev = NULL_OBJ_LINK;
    OBJ_LINK_DEREF(obj).next = NULL_OBJ_LINK;
    //OBJ_LINK_DEREF(obj).pos.cur_sector = next_sector;
    
    if(next != NULL_OBJ_LINK) {
        OBJ_LINK_DEREF(next).prev = prev;
    }
    if(prev != NULL_OBJ_LINK) {
        OBJ_LINK_DEREF(prev).next = next;
    }

    if(prev == NULL_OBJ_LINK) {
        sector_object_lists[old_sector] = next;
    }

    OBJ_LINK_DEREF(obj).next = sector_object_lists[next_sector];
    object_link nxt = OBJ_LINK_DEREF(obj).next;
    if(nxt != NULL_OBJ_LINK) {
        OBJ_LINK_DEREF(nxt).prev = obj;
    }
    sector_object_lists[next_sector] = obj;

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
        sector_object_lists[old_sector] = next;
    }
    object* next_free = free_object_list;
    if(next_free != NULL) {
        next_free->prev = obj;
    }
    obj->next = next_free;
    free_object_list = obj;
}
*/

object_link objects_in_sector(int sector_num) {
    //return NULL; 
    return sector_object_lists[sector_num];
}

decoration_link decorations_in_sector(int sector_num) {
    //return NULL; 
    return sector_decoration_lists[sector_num];
}

int idle(object_link cur_obj, uint16_t cur_sector) {
    return 1; // stays alive
}

void print_object_list(object_link lst) {
    KLog("printing object list");
    while(lst != NULL_OBJ_LINK) {
        KLog_U1("link: ", lst);
        lst = OBJ_LINK_DEREF(lst).next;
    }
    KLog_U1("terminated with: ", lst);
}

int look_for_player(object_link cur_obj, uint16_t cur_sector) {
    return 1;
    if(cur_player_pos.cur_sector == cur_sector) {
        //cur_obj->object_type = 1;
        OBJ_LINK_DEREF(cur_obj).current_state++;

        //cur_obj->tgt.sector = cur_sector;
        //cur_obj->tgt.x = cur_player_pos.x;
        //cur_obj->tgt.y = cur_player_pos.y;
        //cur_obj->tgt.z = cur_player_pos.z;

    } else {
        //KLog("player not found");
    }
    return 1;
}



int maybe_get_picked_up(object_link cur_obj, uint16_t cur_sector) {  
    fix32 obj_x = OBJ_LINK_DEREF(cur_obj).x;
    fix32 obj_y = OBJ_LINK_DEREF(cur_obj).y;

    int dx = fix32ToInt(cur_player_pos.x - obj_x);
    int dy = fix32ToInt(cur_player_pos.y - obj_y);
    
    u32 dist = fastLength(dx, dy);
    
    if(dist < 32) { 
        if(inventory_add_item(BLUE_KEY)) {
            char buf[50];
            int len = sprintf(buf, "picked up the %s!", object_types[OBJ_LINK_DEREF(cur_obj).object_type].name);
            console_push_message_high_priority(buf, len, 30);
        }

        return 0;
    }
    return 1;
}

int follow_player(object_link cur_obj, uint16_t cur_sector) {
    return 1;
    fix32 obj_x = OBJ_LINK_DEREF(cur_obj).x;
    fix32 obj_y = OBJ_LINK_DEREF(cur_obj).y;
    // follow player
    //KLog("following player");

    int dx = fix32ToInt(cur_player_pos.x - obj_x);
    int dy = fix32ToInt(cur_player_pos.y - obj_y);


    //u32 dist = getApproximatedDistance(dx, dy);
    //if(dist < 8) { c }
    //int norm_dx = dx/dist;
    //int norm_dy = dy/dist;
    //dx = 7*norm_dx;
    //dy = 7*norm_dy;

    //int dz = fix32ToInt(cur_player_pos.z - pos.z);

    volatile object_template* obj_type = &object_types[OBJ_LINK_DEREF(cur_obj).object_type];

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
    
    
    OBJ_LINK_DEREF(cur_obj).x = obj_x+dx32;//move_res.pos.x;
    OBJ_LINK_DEREF(cur_obj).y = obj_y+dy32;//move_res.pos.y;

    
    /*

    if(move_res.new_sector != cur_sector) {
        move_object_to_sector(cur_obj, cur_sector, move_res.new_sector);
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
    //return;
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
        object_link cur_object = sector_object_lists[sect];
        while(cur_object != NULL_OBJ_LINK) {
            if(OBJ_LINK_DEREF(cur_object).activate_tick != 0) {
                OBJ_LINK_DEREF(cur_object).activate_tick--;
            
            } else {
                //KLog("processing object");
                uint16_t state_idx = OBJ_LINK_DEREF(cur_object).current_state;
                int live = object_state_machines[state_idx].action(cur_object, sect);
                if(!live) { 
                    // TODO: make work with more than one object
                    // this will only work with a single object

                    // free object needs to remove the object and patch next and prev links if necessary
                    // and place it on the free list
                    //if(sector_object_lists[sect] == cur_object) {
                    //    sector_object_lists[sect] = OBJ_LINK_DEREF(cur_object).next;
                    //}
                    free_object(cur_object, sect); 

                } else {
                    if(object_state_machines[state_idx].action == &idle) {
                        OBJ_LINK_DEREF(cur_object).activate_tick = 128;
                    } else {
                        OBJ_LINK_DEREF(cur_object).activate_tick = 1;
                    }
                }
            }
            cur_object = OBJ_LINK_DEREF(cur_object).next;
            //break;
        }
    }
    //KLog("done processing objects");
}