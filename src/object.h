#ifndef OBJECT_H
#define OBJECT_H

#include <genesis.h>

#include "obj_sprite.h"

// 13 bytes
typedef struct {
    fix32 x;
    fix32 y;
    fix32 z;
    u8 ang;
} object_pos;

typedef struct {
    fix32 x;
    fix32 y;
    fix32 z;
    u16 cur_sector;
    u16 ang;
} player_pos;

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


// 64 active objects
#define NULL_OBJ_LINK 0b00111111
// 128 static objects
#define NULL_DEC_LINK 0b01111111

// if there's more than 32 objects to draw in a sector, too bad 
#define OBJ_SORT_BUF_SZ 32


typedef u8 object_link;

typedef u8 decoration_link;

// 127 objects! 
#define MAX_OBJECTS (NULL_OBJ_LINK) 
// 256 decorations!
#define MAX_DECORATIONS (NULL_DEC_LINK)

#define OBJ_LINK_DEREF(lnk) (objects[(lnk)])
#define DEC_LINK_DEREF(lnk) (decorations[(lnk)])

#define IDLE_STATE 0 
typedef struct object object;

// 22 bytes!, 2772 bytes for objects
struct object {
    
    fix32 x;
    fix32 y;
    s16 z;
    u8 ang;

    //object_pos pos; // 13 bytes
    object_link tgt; // top bit is fingerprint
    object_link prev; // top bit is fingerprint
    object_link next; // top bit is fingerprint

    uint8_t health;
    uint8_t current_state;
    uint8_t object_type;   // top 2 bits are fingerprint
    uint8_t activate_tick; // top 2 bits are fingerprint
    u8 fingerprint;
};

typedef struct decoration_object decoration_object;

// 8 bytes, 2040 bytes for all objects
struct decoration_object {
    uint8_t object_type;
    s16 x; s16 y; s16 z;
    decoration_link next;
    u8 fingerprint;
}; 

void init_object_lists(int num_sectors);

void clear_object_lists();

typedef struct  __attribute__((__packed__)) {
    u16 sector_num;
    s16 x;
    s16 y; 
    s16 z;
    u8 type;
} map_object;

object_link alloc_object_in_sector(u8 activate_tick, int sector_num, fix32 x, fix32 y, s16 z, uint8_t object_type);
decoration_link alloc_decoration_in_sector(int sector_num, s16 x, s16 y, s16 z, uint8_t object_type);

void free_object(object_link obj);

object_link objects_in_sector(int sector_num);

decoration_link decorations_in_sector(int sector_num);


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
extern decoration_object* decorations;

#endif