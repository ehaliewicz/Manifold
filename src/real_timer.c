#include "genesis.h"
#include "my_bmp.h"
#include "real_timer.h"



#define BASE_SHIFT 2

// 0b0000000000000001 16ms
// 0b0000000000000010 32ms
// 0b0000000000000100 64ms

//u32 frame_timer_masks[4] = {
//    ~0b0000000000000111, // sixteenth second
//    ~0b0000000000001111, // eighth second
//    ~0b0000000000011111, // quarter second
//    ~0b0000000000111111, // half second
//    ~0b0000000001111111, // second (1024ms)
//};

u32 frames_for_counters[5] = {
    0,0,0,0,0
};

static u32 ms;
void update_real_timer() {
    u32 vints = end_of_frame_vints;
    u32 vints_tmp = vints>>BASE_SHIFT;
    u32* frame_cnt_ptr = frames_for_counters;
    for(int i = 0; i < 5; i++) {
        *frame_cnt_ptr++ = vints_tmp;
        vints_tmp >>= 1;
    }    
    
    u32 dvints = (end_of_frame_vints - prev_end_of_frame_vints);

    if(SYS_isNTSC()) {
        ms = dvints << 4;
    } else {
        u32 four_dvints = dvints <<2;
        u32 sixteen_dvints = dvints << 4;
        ms = four_dvints + sixteen_dvints;
    }
    if(ms > 1000) { ms = 1000; }
    
}

u32 ms_since_last_frame() {
    return ms;
}

u32 total_vints() {
    return end_of_frame_vints;
}