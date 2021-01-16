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
    player_pos new_player_pos;
    u16 collided_with;
} collision_result;

collision_result check_for_collision(player_pos cur_position, player_pos new_position);

u16 find_sector(player_pos cur_player_pos);
#endif