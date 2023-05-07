#ifndef SECTOR_GROUP_H
#define SECTOR_GROUP_H

#include <genesis.h>
#include "portal_map.h"

#define NUM_SECTOR_GROUP_PARAMS 8
#define NUM_SECTOR_GROUP_PARAMS_SHIFT 3


// these params are actual sector group params
#define SECTOR_GROUP_PARAM_LIGHT_IDX 0
#define SECTOR_GROUP_PARAM_ORIG_HEIGHT_IDX 1
#define SECTOR_GROUP_PARAM_STATE_IDX 2
#define SECTOR_GROUP_PARAM_TICKS_LEFT_IDX 3
#define SECTOR_GROUP_PARAM_FLOOR_HEIGHT_IDX 4
#define SECTOR_GROUP_PARAM_CEIL_HEIGHT_IDX 5
#define SECTOR_GROUP_PARAM_FLOOR_COLOR_IDX 6
#define SECTOR_GROUP_PARAM_CEIL_COLOR_IDX 7

#define NO_TYPE 0
#define FLASHING 1
#define DOOR 2
#define LIFT 3
#define STAIRS 4
#define LOWERING_STAIRS 5

#define NUM_SECTOR_GROUP_TYPES 6


typedef enum {
    CLOSED,
    GOING_UP,
    OPEN,
    GOING_DOWN
} door_lift_state;

typedef enum {
    STAIRS_LOWERED = 0,
    STAIRS_MOVING = 1,
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
    LEVEL_END=4,
} trigger_type;

u16 get_sector_group_trigger_type(u16 sect_group);
s16 get_sector_group_trigger_target(u16 sect_group, u8 tgt_idx);


trigger_type activate_sector_group_enter_trigger(u16 sect_group);

#endif