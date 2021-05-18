#ifndef GAME_H
#define GAME_H

#include <genesis.h>
#include "draw.h"
#include "game_mode.h"
#include "vertex.h"

extern int cur_frame;
extern int automap_mode;
extern draw_mode render_mode;
extern int debug_draw;

extern u32 last_frame_ticks;

typedef struct {
    fix32 x;
    fix32 y;
    fix32 z;
    u16 ang;
    u16 cur_sector;
} player_pos;

extern Vect2D_f32 *sector_centers;

extern player_pos cur_player_pos;
extern fix16 playerXFrac4;
extern fix16 playerYFrac4;
extern s16 playerZ12Frac4;

#define ANGLE_360_DEGREES 1024
#define ANGLE_180_DEGREES 512
#define ANGLE_90_DEGREES 256
 // 58 degrees from player viewpoint to top left of map
#define ANGLE_58_DEGREES 164


extern fix32 angleCos32;
extern fix32 angleSin32;
extern fix16 angleCos16;
extern fix16 angleSin16;
extern s16 angleSinFrac12;
extern s16 angleCosFrac12;
extern s16 playerXInt;
extern s16 playerYInt;
extern int subpixel;

void init_game();
game_mode run_game();
void cleanup_game();

void request_flip();

#endif