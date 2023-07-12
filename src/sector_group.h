#ifndef SECTOR_GROUP_H
#define SECTOR_GROUP_H

#include <genesis.h>
#include "portal_map.h"

#define NUM_SECTOR_GROUP_PARAMS 8
#define NUM_SECTOR_GROUP_PARAMS_SHIFT 3

typedef struct __attribute__((__packed__)) {
    s8 light_level;
    u8 floor_ceil_color;
} light_and_floor_ceil_color;

// these params are actual sector group params

// light level is only 3 bits, using 16 bits :(
// 00000 light_level:3 ceil_col:4 floor_col:4

#define SECTOR_GROUP_PARAM_LIGHT_FLOOR_CEIL_COLOR_IDX 0
#define SECTOR_GROUP_PARAM_ORIG_HEIGHT_IDX 1
// state is probably only 8 bits, could possibly be more than that eventually
#define SECTOR_GROUP_PARAM_STATE_IDX 2
#define SECTOR_GROUP_PARAM_TICKS_LEFT_IDX 3
#define SECTOR_GROUP_PARAM_FLOOR_HEIGHT_IDX 4
#define SECTOR_GROUP_PARAM_CEIL_HEIGHT_IDX 5
#define SECTOR_GROUP_PARAM_SCRATCH_ONE_IDX 6
#define SECTOR_GROUP_PARAM_SCRATCH_TWO_IDX 7

#define NO_TYPE 0
#define FLASHING 1
#define DOOR 2
#define LIFT 3
#define STAIRS 4
#define LOWERING_STAIRS 5

#define NUM_SECTOR_GROUP_TYPES 6

#define   NO_KEYCARD  0b00000000
#define BLUE_KEYCARD  0b00100000
#define GREEN_KEYCARD 0b01000000
#define RED_KEYCARD   0b01100000

// use the top three bits for keycard mask
// we can have up to 32 sector types?
#define SECTOR_GROUP_KEYCARD_MASK 0b11100000  
#define SECTOR_GROUP_TYPE_MASK 0b00011111

#define GET_SECTOR_GROUP_TYPE(typ_byte) ((typ_byte) & SECTOR_GROUP_TYPE_MASK)
#define GET_SECTOR_GROUP_KEYCARD(typ_byte) ((typ_byte) & SECTOR_GROUP_KEYCARD_MASK)

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
void set_sector_group_state(u16 sect_group, u16 state);

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