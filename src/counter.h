#ifndef COUNTER_H
#define COUNTER_H

#include <genesis.h>


#define LIGHT_LOOPS_COUNTER_IDX 0
#define NON_PERSP_WALLS_COUNTER_IDX 1
#define PERSP_WALLS_COUNTER_IDX 2
#define PRE_PROJ_FRUSTUM_CULL_COUNTER 3
#define NEAR_Z_CULL_COUNTER 4
#define COARSE_BACKFACE_CULL_COUNTER 5
#define POST_PROJ_BACKFACE_CULL_COUNTER 6

u16 get_counter(u8 cnt_idx);
void reset_counter(u8 cnt_idx);
void inc_counter(u8 cnv_idx, u16 inc);

#endif