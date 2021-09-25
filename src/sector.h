#ifndef SECTOR_H
#define SECTOR_H

#include <genesis.h>

#define SECTOR_TYPES 3


typedef enum {
    NO_TYPE = 0,
    FLASHING = 1,
    DOOR = 2,
    LIFT = 3,
} sector_type;

typedef enum {
    CLOSED,
    GOING_UP,
    OPEN,
    GOING_DOWN
} door_lift_state;


typedef struct {
    s8 light;
    s16 par2;
    s8 stash;
    s16 orig_height;
    u8 frames_left;
    u8 state; // 0 closed, 1 going up, 2 open, 3 going on
} sector_param;


s8 get_sector_light_level(s16 sect_idx);
void run_sector_processes();

#endif