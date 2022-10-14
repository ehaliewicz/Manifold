#ifndef OBJECT_H
#define OBJECT_H

#include <genesis.h>

#include "obj_sprite.h"

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

// 4+2+2+2+2+32+2 bytes 
// 46
typedef struct  __attribute__((__packed__)) {
    rle_sprite* sprite;
    uint16_t from_floor_draw_offset;
    uint16_t width;
    uint16_t height;
    uint16_t init_state;
    uint16_t speed; // 3 for claw guy?
    u16 is_player;
    char name[32];
} object_template;

extern const object_template object_types[];

typedef struct object object;

struct object {
    uint16_t id;
    uint16_t current_state;
    uint8_t object_type;
    uint32_t activate_tick;
    object_tgt tgt;
    object_pos pos;
    object *prev;
    object *next;
};

struct decoration_object {
    uint16_t id;
    uint8_t object_type;
    object_pos pos;
}; // doesn't even need a next and prev pointer?


void init_object_lists(int num_sectors);

void clear_object_lists();

typedef struct  __attribute__((__packed__)) {
    u16 sector_num;
    s16 x;
    s16 y; 
    s16 z;
    u16 type;
} map_object;

object* alloc_object_in_sector(u32 activate_tick, int sector_num, fix32 x, fix32 y, fix32 z, uint8_t object_type);

void free_object(object* obj);

object* objects_in_sector(int sector_num);

void process_all_objects(uint32_t cur_tick);


typedef struct {
    uint16_t next_state;
    uint16_t ticks;
    int (*action)(object*, uint16_t); // passed the current object as well as the sector
} obj_state;



#endif