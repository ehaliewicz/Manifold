#include <genesis.h>

u16 counters[32];
u8 used_counters = 0;

u16 get_counter(u8 cnt_idx) {
    return counters[cnt_idx];
}

void reset_counter(u8 cnt_idx) {
    counters[cnt_idx] = 0;
}

void inc_counter(u8 cnt_idx, u16 inc) {
    counters[cnt_idx] += inc;
}
