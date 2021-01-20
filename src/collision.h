#ifndef COLLISION_H
#define COLLISION_H

#include <genesis.h>
#include "game.h"

typedef struct {
    Vect2D_f32 pos;
    u16 new_sector;
} collision_result;

collision_result check_for_collision(fix32 curx, fix32 cury, fix32 newx, fix32 newy, u16 cur_sector);
u16 find_sector(player_pos cur_player_pos);
u8 on_backfacing_side_of_wall(fix32 x, fix32 y, u16 wall_idx);

#endif