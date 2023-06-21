#ifndef COLLISION_H
#define COLLISION_H

#include <genesis.h>
#include "game.h"

typedef struct {
    Vect2D_f32 pos;
    u16 new_sector;
} collision_result;

#define PLAYER_COLLISION_DISTANCE 20
#define PLAYER_COLLISION_DISTANCE_SQR (PLAYER_COLLISION_DISTANCE*PLAYER_COLLISION_DISTANCE)

collision_result check_for_collision(fix32 curx, fix32 cury, fix32 newx, fix32 newy, u16 cur_sector);
collision_result check_for_collision_radius(fix32 curx, fix32 cury, fix32 curz, fix32 newx, fix32 newy, f32 radius_sqr, u16 cur_sector);
u16 find_sector(player_pos cur_player_pos);
s32 sq_shortest_dist_to_point(fix32 px, fix32 py, vertex a, vertex b);

#endif