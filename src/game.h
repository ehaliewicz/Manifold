#ifndef GAME_H
#define GAME_H

#include <genesis.h>
#include "draw.h"
#include "game_mode.h"
#include "object.h"
#include "vertex.h"

extern int cur_frame;

extern u32 last_frame_ticks;
extern u16 rot_speed;
extern u32 move_speed;

extern u8* bmp_buffer_0;
extern u8* bmp_buffer_1;

extern Vect2D_f32 *sector_centers;

extern player_pos cur_player_pos;
extern fix16 playerXFrac4;
extern fix16 playerYFrac4;
extern s16 playerZCam12Frac4;

#define ANGLE_360_DEGREES 1024
#define ANGLE_180_DEGREES 512
#define ANGLE_90_DEGREES 256
 // 58 degrees from player viewpoint to top left of map
#define ANGLE_58_DEGREES 164

#define RENDER_WIDTH 64


extern fix32 angleCos32;
extern fix32 angleSin32;
extern fix16 angleCos16;
extern fix16 angleSin16;
extern s16 angleSinFrac12;
extern s16 angleCosFrac12;
extern s16 playerXInt;
extern s16 playerYInt;


void init_game();
game_mode run_game();
void cleanup_game();

void request_flip();

extern u8 render_mode;

typedef enum {
    RENDER_SOLID,
    RENDER_WIREFRAME
} render_modes;

#endif