#ifndef GAME_H
#define GAME_H

#include <genesis.h>
#include "draw.h"
#include "game_mode.h"
#include "vertex.h"

extern int cur_frame;
extern int automap_mode;
draw_mode render_mode;


typedef struct {
    fix32 x;
    fix32 y;
    fix32 z;
    fix32 ang;
} player_pos;


extern player_pos cur_player_pos;
extern fix16 playerXFrac4;
extern fix16 playerYFrac4;


#define ANGLE_90_DEGREES 256
 // 58 degrees from player viewpoint to top left of map
#define ANGLE_58_DEGREES 164


extern fix32 angleCos32;
extern fix32 angleSin32;
extern fix16 angleCos16;
extern fix16 angleSin16;
extern s16 angleSinFrac12;
extern s16 angleCosFrac12;



void init_game();
game_mode run_game();
void cleanup_game();


#endif