#ifndef SECTOR_H
#define SECTOR_H

#include <genesis.h>
#include "portal_map.h"

#define NUM_SECTOR_TYPES 4

#define NO_TYPE 0
#define FLASHING 1
#define DOOR 2
#define LIFT 3



typedef enum {
    CLOSED,
    GOING_UP,
    OPEN,
    GOING_DOWN
} door_lift_state;



extern s16* live_sector_parameters;
//extern sector_param* live_sector_parameters;


s16 get_sector_orig_height(u16 sect_idx);
s8 get_sector_light_level(u16 sect_idx);
u16 get_sector_ticks_left(u16 sect_idx);
u16 get_sector_state(u16 sect_idx);

s16 get_sector_floor_height(u16 sect_idx);
void set_sector_floor_height(u16 sector_idx, s16 height);

s16 get_sector_ceil_height(u16 sector_idx);
void set_sector_ceil_height(u16 sector_idx, s16 height);
s16 get_sector_floor_color(u16 sector_idx);
s16 get_sector_ceil_color(u16 sector_idx);

void run_sector_processes();

#endif