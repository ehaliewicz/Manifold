#include <genesis.h>
#include "draw_queues.h"

u16 draw_ssector_queue[MAX_DRAW_SSECTORS];
int num_draw_ssectors;

void clear_draw_ssector_queue() {
    num_draw_ssectors = 0;
}

int enqueue_draw_ssector(uint16_t ssector_idx) {
    if(num_draw_ssectors < MAX_DRAW_SSECTORS) {
        draw_ssector_queue[num_draw_ssectors++] = ssector_idx;
    } 
    return num_draw_ssectors >= MAX_DRAW_SSECTORS;
}