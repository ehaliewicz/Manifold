#ifndef OBJECT_H
#define OBJECT_H

#include <genesis.h>

#include "obj_sprite.h"

// 16 bytes!
typedef struct {
    fix32 x;
    fix32 y;
    fix32 z;
    u16 ang;
    u16 cur_sector;
} object_pos;

typedef struct {
    fix32 x;
    fix32 y;
    fix32 z;
    u16 sector;
} object_tgt;

// 4+2+2+2+1+32+2 bytes 
// 46


typedef struct  __attribute__((__packed__)) {
    rle_sprite* sprite;
    uint16_t from_floor_draw_offset;
    uint16_t width;
    uint16_t height;
    uint8_t init_state;
    uint16_t speed; // 3 for claw guy?
    u16 is_player;
    char name[32];
} object_template;

extern const object_template object_types[];

typedef struct object object;

#define      LINK_MASK (0b0111111)
#define LINK_NULL_MASK (0b10000000)
#define OBJ_LINK_IS_NULL(lnk) (LINK_MASK & (lnk))
#define NULL_LINK LINK_NULL_MASK

#define LINK_DEREF(lnk) (objects[(lnk)&LINK_MASK])

typedef u8 object_link;

// 37 or 38 bytes, holy crap
// with links instead, 31/32 bytes
// with 16-bit activate tick, 27/28 bytes

// 25/26 bytes ?
struct object {
    uint16_t id;
    uint8_t current_state;
    uint8_t object_type;
    uint16_t activate_tick;
    //object_tgt tgt;
    uint8_t tgt;
    object_pos pos; // 16 bytes
    uint8_t prev;
    uint8_t next;
    //object *prev;
    //object *next;
};

typedef struct decoration_object decoration_object;

// 13 or 14 bytes
struct decoration_object {
    uint16_t object_type;
    s16 x; s16 y; s16 z;
    u16 cur_sector;
    decoration_object* next;
}; // doesn't even need a next and prev pointer?


void init_object_lists(int num_sectors);

void clear_object_lists();

typedef struct  __attribute__((__packed__)) {
    u16 sector_num;
    s16 x;
    s16 y; 
    s16 z;
    u8 type;
} map_object;

object_link alloc_object_in_sector(u16 activate_tick, int sector_num, fix32 x, fix32 y, fix32 z, uint8_t object_type);

void free_object(object_link obj);

object_link objects_in_sector(int sector_num);

void process_all_objects(uint32_t cur_tick);


typedef struct {
    uint16_t next_state;
    uint16_t ticks;
    int (*action)(object_link, uint16_t); // passed the current object as well as the sector
} obj_state;


// 12 bytes
typedef struct {
    s16 x;
    u8 obj_type;
    s16 ybot;
    s16 ytop;
} buf_obj;

// 6 bytes
typedef struct {
    u16 z_recip;
    u8 buf_idx;
    s16 height;
    //s16 x;
    //u8 obj_type;
    //s16 ybot;
    //s16 ytop;
} z_buf_obj;


extern z_buf_obj *z_sort_buf;//[64];
extern buf_obj *obj_sort_buf;//[64];

extern object* objects;

#endif