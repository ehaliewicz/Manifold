#ifndef COLLISION_H
#define COLLISION_H

#include <genesis.h>
#include "game.h"

typedef struct {
    Vect2D_f32 pos;
    u16 new_sector;
} collision_result;

#define PLAYER_COLLISION_DISTANCE 30

collision_result check_for_collision(fix32 curx, fix32 cury, fix32 newx, fix32 newy, u16 cur_sector);
collision_result check_for_collision_radius(fix32 curx, fix32 cury, fix32 newx, fix32 newy, u16 radius, u16 cur_sector);
u16 find_sector(object_pos cur_player_pos);

#endif