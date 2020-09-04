#ifndef FIRE_H
#define FIRE_H

#include "game_mode.h"

//#define FIRE_WIDTH 64
#define FIRE_WIDTH 128 //64
#define FIRE_HEIGHT 55
#define BASE_FRAMEBUFFER_OFFSET (160-FIRE_HEIGHT)-1

void init_fire();
void start_fire_source();
void clear_fire_source();
void spread_and_draw_fire();
void spread_and_draw_fire_byte();
void copy_fire_buffer_portion();

void init_fire();
game_mode run_fire();
void cleanup_fire();


#endif 