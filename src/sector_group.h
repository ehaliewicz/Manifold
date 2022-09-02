#ifndef SECTOR_GROUP_H
#define SECTOR_GROUP_H

#include <genesis.h>
#include "portal_map.h"

#define NUM_SECTOR_TYPES 4

#define NO_TYPE 0
#define FLASHING 1
#define DOOR 2
#define LIFT 3
#define STAIRS 4



typedef enum {
    CLOSED,
    GOING_UP,
    OPEN,
    GOING_DOWN
} door_lift_state;

typedef enum {
    STAIRS_LOWERED = 0,
    STAIRS_RAISING = 1,
    STAIRS_RAISED = 2
} stair_state;

extern s16* live_sector_group_parameters;
//extern sector_param* live_sector_parameters;


s16* get_sector_group_pointer(u16 sect_group);

s16 get_sector_group_orig_height(u16 sect_idx);
s8 get_sector_group_light_level(u16 sect_idx);
u16 get_sector_group_ticks_left(u16 sect_idx);
u16 get_sector_group_state(u16 sect_idx);

s16 get_sector_group_floor_height(u16 sect_idx);
void set_sector_group_floor_height(u16 sector_idx, s16 height);

s16 get_sector_group_ceil_height(u16 sector_idx);
void set_sector_group_ceil_height(u16 sector_idx, s16 height);
u16 get_sector_group_floor_color(u16 sector_idx);
u16 get_sector_group_ceil_color(u16 sector_idx);

void run_sector_group_processes();

typedef enum {
    NO_TRIGGER=0,
    SET_SECTOR_DARK=1,
    SET_SECTOR_LIGHT=2,
    START_STAIRS=3,
} trigger_type;

u16 get_sector_group_trigger_type(u16 sect_group);
s16 get_sector_group_trigger_target(u16 sect_group, u8 tgt_idx);

void activate_sector_group_enter_trigger(u16 sect_group);

#endif