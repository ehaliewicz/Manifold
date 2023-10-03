#ifndef REAL_TIMER_H
#define REAL_TIMER_H

typedef enum {
    SIXTEENTH_SECOND,
    EIGHTH_SECOND,
    QUARTER_SECOND,
    HALF_SECOND,
    SECOND
} frame_timers;

extern u32 frames_for_counters[5];

void update_real_timer();
u32 ms_since_last_frame();
u32 total_vints();

#endif 