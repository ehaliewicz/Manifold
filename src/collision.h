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

Vect2D_f32 check_for_collision(fix32 curx, fix32 cury, fix32 newx, fix32 newy, u16 cur_sector);

u16 find_sector(player_pos cur_player_pos);
#endif