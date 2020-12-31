#ifndef COLLISION_H
#define COLLISION_H

#include <genesis.h>
#include "game.h"

typedef enum {
    NO_COLLISION,
    COLLIDED,
    SECTOR_TRANSITION
} collision_type;

typedef struct {
    collision_type type;
    union {
        player_pos new_player_pos;
        u16 new_sector;
    };
} collision_result;

collision_result check_for_collision(s16 cur_sector, player_pos position);

#endif