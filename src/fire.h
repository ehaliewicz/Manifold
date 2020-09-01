#ifndef FIRE_H
#define FIRE_H

//#define FIRE_WIDTH 64
#define FIRE_WIDTH 128 //64
#define FIRE_HEIGHT 55

void init_fire();
void start_fire_source();
void clear_fire_source();
void scroll_fire_a();
void scroll_fire_b();
void spread_and_draw_fire();
void spread_and_draw_fire_byte();
void copy_fire_buffer_portion();

#endif 