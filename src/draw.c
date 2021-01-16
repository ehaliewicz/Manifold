#include "draw.h"
#include "game.h"
#include "level.h"
#include "math3d.h"
#include "portal.h"


//u32 column_buffer[128] = { 0 };

void draw_vertical_line_no_clip(u8 x, u8 y0, u8 y1, u8 col) {
    u8* ptr = BMP_getWritePointer(x<<1, y0);
    u8 pix = y1-y0;
    if(y1 <= y0) { return; }
    while(pix--) {
        *ptr = col;
        ptr += 128;
    }
}

void draw_horizontal_line(u8 x0, u8 x1, u8 y, u8 col) {
    u8* ptr = BMP_getWritePointer(x0<<1, y);
    u8 pix = x1-x0;
    while(pix--) {
        *ptr++ = col;
    }
}


void bresenham_line(u8 x0, u8 x1, u8 y0, u8 y1, u16 win_min, u16 win_max, u8 col) {
    //col = col<<4 |
    
    if(x0 == x1) {
        if(x0 >= win_min && x0 <= win_max) {
            return draw_vertical_line_no_clip(x0, y0, y1, col); 
        }   
    } else if (y0 == y1) {
        x0 = max(x0, win_min);
        x1 = min(x1, win_max);
        return draw_horizontal_line(x0, x1, y0, col) ;
    }
    

    int dx =  abs (x1 - x0);
    int dy = -abs (y1 - y0);
    int dy2 = dy<<1;
    int dx2 = dx<<1;
    int sy = (y0 < y1) ? 1 : -1;
    int dptr_y = sy << 7;

    //int err = dx + dy,
    int e2;  // error value e_xy 
    int err2 = dx2+dy2;
    while(x0 < win_min) {
        if (x0 == x1 && y0 == y1) { break; }
        e2 = err2; //2 * err;
        if (e2 >= dy) { err2 += dy2; x0++; } // e_xy+e_x > 0 
        if (e2 <= dx) { err2 += dx2; y0 += sy; } // e_xy+e_y < 0 
    }

    u8* ptr = BMP_getWritePointer(x0<<1, y0);

    for (;;){ 
        if(x0 > win_max) { break; }
        *ptr = col;
                //BMP_setPixel(x0<<1, y0, col);
        if (x0 == x1 && y0 == y1) { break; }
        e2 = err2; //2 * err;
        if (e2 >= dy) { err2 += dy2; x0++; ptr++; } // e_xy+e_x > 0 
        if (e2 <= dx) { err2 += dx2; y0 += sy; ptr += dptr_y; } // e_xy+e_y < 0 
        
    }
    
}

/*
typedef struct {
    u8 has_two_ranges:1;
    u8 rng_0_min; u8 rng_0_max;
    u8 rng_1_min; u8 rng_1_max;
} backfacing_entry;

backfacing_entry wall_backfacing_table[32];

int is_backfacing(u16 wall_id, fix32 cur_player_angle) {
    u8 ply_ang = quantize_angle(cur_player_angle);
    backfacing_entry wall_entry = wall_backfacing_table[wall_id];
    if(ply_ang >= wall_entry.rng_0_min && ply_ang <= wall_entry.rng_0_max) {
        return 0;
    }

    if(wall_entry.has_two_ranges && ply_ang >= wall_entry.rng_1_min && ply_ang <= wall_entry.rng_1_max) {
        return 0;
    }
    return 1;
}
*/

// TODO: optimize backface culling!
// 

/*

    FULLY_NEAR_CLIPPED,
    NEAR_CLIPPED_LEFT,
    NEAR_CLIPPED_RIGHT,
    OUTSIDE_FRUSTUM,
    PARTIALLY_FRUSTUM_CLIPPED,
    FULLY_VISIBLE,
*/
/*
vis_query_result is_visible(volatile Vect2D_s32 trans_v1, volatile Vect2D_s32 trans_v2, s16 floor, s16 ceil, u16 window_min, u16 window_max) {
    vis_query_result res; 


    // with optimized backface culling
    //if(is_backfacing(wall.backface_angle)

    //if(trans_v1.x <= trans_v2.x) { return res;}

    
    s16 x1 = project_and_adjust_x(trans_v1);
    s16 x2 = project_and_adjust_x(trans_v2);

    if(x1 >= x2) {
        res.visible = 0;
        return res;
    }

    res.visible = 1;

    if(x1 < window_min) {
        res.draw_x1 = window_min;
    } else {
        res.draw_x1 = x1;
    }

    if(x2 > window_max) {
        res.draw_x1 = x1;
        res.draw_x2 = window_max;
    }
    
    if(clipped == LEFT_CLIPPED) {
        res.window_x1 = window_min;
        res.window_x2 = res.draw_x2-1;
    } else if (clipped == RIGHT_CLIPPED) {
        res.window_x1 = res.draw_x1+2;
        res.window_x2 = window_max;
    } else {
        res.window_x1 = res.draw_x1+2;
        res.window_x2 = res.draw_x2-1;
    }

    res.x1_ytop = project_and_adjust_y(trans_v1, ceil);
    res.x1_ybot = project_and_adjust_y(trans_v1, floor);
    res.x2_ytop = project_and_adjust_y(trans_v2, ceil);
    res.x2_ybot = project_and_adjust_y(trans_v2, floor);

    return res;
}
*/

s16 *ytop; //[W];
s16 *ybot; //[W];

void init_2d_buffers() {
    ytop = MEM_alloc(W*sizeof(s16));
    ybot = MEM_alloc(W*sizeof(s16));
}

void clear_2d_buffers() {
    for(int i = 0; i < W; i++) {
        ytop[i] = 1<<7;
        ybot[i] = 127<<7;
    }
}

void release_2d_buffers() {
    MEM_free(ytop);
    MEM_free(ybot);
}

    #define LOOP_GET_DRAW_PARAMS    do {                \
        top_y_int = top_y_fix&0b1111111110000000;       \
        bot_y_int = bot_y_fix&0b1111111110000000;       \
        min_drawable_y = ytop[x];                        \
        max_drawable_y = ybot[x];                        \
        top_draw_y = max(min_drawable_y, top_y_int);    \
        bot_draw_y = min(max_drawable_y, bot_y_int);    \
    } while(0)

    #define LOOP_PREDRAW_NEXT_COLUMN do {    \
        top_y_fix += top_dy_per_dx;  \
        bot_y_fix += bot_dy_per_dx;  \     
        x++;                         \
    } while(0) 

    #define LOOP_NEXT_COLUMN do {   \
        LOOP_PREDRAW_NEXT_COLUMN;   \
        col_ptr++;                  \
    } while(0)


void draw_scaled_vertical_line(s16 x, s16 scaled_y0, s16 scaled_y1, u8 col, u8* col_ptr) {
    u8* cur_ptr = col_ptr + scaled_y0;
    u8* end_ptr = col_ptr + scaled_y1;
    while(cur_ptr <= end_ptr) {
        *cur_ptr = col;
        cur_ptr += 128;
    }
}

void draw_wall(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 window_min, u16 window_max) {
    
    u8 col = 0x11;

    s16 top_dy = x2_ytop - x1_ytop;
    s16 bot_dy = x2_ybot - x1_ybot;
    s16 dx = x2-x1;

    s16 top_dy_per_dx = (top_dy<<7) / dx; // 12.8
    s16 bot_dy_per_dx = (bot_dy<<7) / dx; // 12.8

    s16 top_y_fix = x1_ytop<<7;
    s16 bot_y_fix = x1_ybot<<7; // 9.7

    s16 top_y_int, bot_y_int;
    s16 min_drawable_y, max_drawable_y;
    s16 top_draw_y, bot_draw_y;

    s16 x = x1;
    s16 beginx = max(x1, window_min);

    while(x < beginx) {
        LOOP_PREDRAW_NEXT_COLUMN;
    }


    u8* col_ptr = BMP_getWritePointer(x<<1, 0);

    LOOP_GET_DRAW_PARAMS;
    
    if(top_draw_y < bot_draw_y) {
        draw_scaled_vertical_line(x, top_draw_y, bot_draw_y, col, col_ptr);
    }

    LOOP_NEXT_COLUMN;

    s16 endx = min(window_max, x2);
    while(x < endx) {

        LOOP_GET_DRAW_PARAMS;

        if(top_draw_y < bot_draw_y) { 
            *(col_ptr+(top_draw_y)) = col;
            *(col_ptr+(bot_draw_y)) = col;
         }
        
        LOOP_NEXT_COLUMN;

    }

    LOOP_GET_DRAW_PARAMS;
    if(top_draw_y < bot_draw_y) {
        draw_scaled_vertical_line(x, top_draw_y, bot_draw_y, col, col_ptr);
    }

    return; 
}



void draw_top_step(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 window_min, u16 window_max) {
    
    u8 col = 0x11;

    s16 top_dy = x2_ytop - x1_ytop;
    s16 bot_dy = x2_ybot - x1_ybot;
    s16 dx = x2-x1;

    s16 top_dy_per_dx = (top_dy<<7) / dx; // 12.8
    s16 bot_dy_per_dx = (bot_dy<<7) / dx; // 12.8

    s16 top_y_fix = x1_ytop<<7;
    s16 bot_y_fix = x1_ybot<<7; // 9.7

    s16 top_y_int, bot_y_int;
    s16 min_drawable_y, max_drawable_y;
    s16 top_draw_y, bot_draw_y;


    s16 x = x1;
    s16 beginx = max(x1, window_min);

    while(x < beginx) {
        LOOP_PREDRAW_NEXT_COLUMN;
    }




    u8* col_ptr = BMP_getWritePointer(x<<1, 0);

    LOOP_GET_DRAW_PARAMS;
    
    if(top_draw_y < bot_draw_y) {
        draw_scaled_vertical_line(x, top_draw_y, bot_draw_y, col, col_ptr);
    }

    LOOP_NEXT_COLUMN;

    s16 endx = min(window_max, x2);
    while(x < endx) {

        LOOP_GET_DRAW_PARAMS;

        if(top_draw_y < bot_draw_y) { 
            *(col_ptr+(top_draw_y)) = col;
            *(col_ptr+(bot_draw_y)) = col;
         }
        if(top_draw_y > min_drawable_y) { ytop[x] = top_draw_y+128; }

        
        LOOP_NEXT_COLUMN;

    }

    LOOP_GET_DRAW_PARAMS;
    if(top_draw_y < bot_draw_y) {
        draw_scaled_vertical_line(x, top_draw_y, bot_draw_y, col, col_ptr);
    }

    return; 
}

void draw_bot_step(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 window_min, u16 window_max) {
    
    u8 col = 0x11;

    s16 top_dy = x2_ytop - x1_ytop;
    s16 bot_dy = x2_ybot - x1_ybot;
    s16 dx = x2-x1;

    s16 top_dy_per_dx = (top_dy<<7) / dx; // 12.8
    s16 bot_dy_per_dx = (bot_dy<<7) / dx; // 12.8

    s16 top_y_fix = x1_ytop<<7;
    s16 bot_y_fix = x1_ybot<<7; // 9.7

    s16 top_y_int, bot_y_int;
    s16 min_drawable_y, max_drawable_y;
    s16 top_draw_y, bot_draw_y;

    s16 x = x1;
    s16 beginx = max(x1, window_min);

    while(x < beginx) {
        LOOP_PREDRAW_NEXT_COLUMN;
    }


    u8* col_ptr = BMP_getWritePointer(x<<1, 0);

    LOOP_GET_DRAW_PARAMS;
    
    if(top_draw_y < bot_draw_y) {
        draw_scaled_vertical_line(x, top_draw_y, bot_draw_y, col, col_ptr);
    }

    LOOP_NEXT_COLUMN;

    s16 endx = min(window_max, x2);
    while(x < endx) {

        LOOP_GET_DRAW_PARAMS;

        if(top_draw_y < bot_draw_y) { 
            *(col_ptr+(top_draw_y)) = col;
            *(col_ptr+(bot_draw_y)) = col;
         }

        if(bot_draw_y < max_drawable_y) { ybot[x] = bot_draw_y-128; }
        
        LOOP_NEXT_COLUMN;

    }

    LOOP_GET_DRAW_PARAMS;
    if(top_draw_y < bot_draw_y) {
        draw_scaled_vertical_line(x, top_draw_y, bot_draw_y, col, col_ptr);
    }

    return; 
}

