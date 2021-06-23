#ifndef OBJECT_H
#define OBJECT_H

#include <genesis.h>

typedef struct {
    fix32 x;
    fix32 y;
    fix32 z;
    u16 ang;
    u16 cur_sector;
} object_pos;



typedef struct {
    uint8_t sprite_col;
    uint8_t size;
    uint16_t from_floor_draw_offset;
    uint8_t width;
    uint16_t height;
    uint16_t init_state;
} object_template;

extern const object_template object_types[];

typedef struct object object;

struct object {
    uint16_t id;
    uint16_t current_state;
    uint8_t object_type;
    uint32_t activate_tick;
    object_pos pos;
    object *prev;
    object *next;
};


void init_object_lists(int num_sectors);

void clear_object_lists();

object* alloc_object_in_sector(object_pos cur_player_pos, int sector_num, fix32 x, fix32 y, fix32 z, uint8_t object_type);

void free_object(object* obj);

object* objects_in_sector(int sector_num);

void process_all_objects(uint32_t cur_tick);


typedef struct {
    uint16_t next_state;
    uint16_t ticks;
    void (*action)(object*, uint16_t); // passed the current object as well as the sector
} obj_state;



#endif