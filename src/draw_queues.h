#ifndef DRAW_QUEUES
#define DRAW_QUEUES

#include <genesis.h>

#define MAX_DRAW_SSECTORS 32



extern u16 draw_ssector_queue[MAX_DRAW_SSECTORS];
extern int num_draw_ssectors;

void clear_draw_ssector_queue();

int enqueue_draw_ssector(uint16_t ssector_idx);

#endif