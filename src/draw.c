#include <genesis.h>
#include <kdebug.h>
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

void bresenham_wall(s16 x1, s16 x1y, s16 x2, s16 x2y, u16 window_min, u8 window_max, s16* output) {
    /*
    const int32_t deltaX2 = deltaX * 2;
    const int32_t deltaY2 = deltaY * 2;
    int32_t error = deltaY2 - deltaX; 

    plot(x, y);

    // Step along the major x axis
    while(deltaX--)
    {
        if(error >= 0)
        {
            // Step along the minor y axis
            y++;
            error -= deltaX2;
        }
        
        error += deltaY2;
        x += dir;

        plot(x, y);
    }
    */
    

    int dx =  abs (x2 - x1);
    int dy = abs (x2y - x1y);
    int dy2 = dy<<1;
    int dx2 = dx<<1;
    int sy = (dy > 0) ? 1 : -1;

    // todo optimize perfectly horizontal lines

    //int err = dx + dy,
    int e2;  // error value e_xy 
    int err2 = dx2+dy2;
    int x = x1;
    int y = x1y;

    int beginx = max(x1, window_min);
    int endx = min(x2, window_max);
    while(x < beginx) {
        if (x == x2 && y == x2y) { break; }
        e2 = err2; //2 * err;
        if (e2 >= dy) { err2 += dy2; x++; } // e_xy+e_x > 0 
        if (e2 <= dx) { err2 += dx2; y += sy; } // e_xy+e_y < 0 
    }

    //s16* ptr = output+x;
    
    output[x] = y;
    while(x <= endx) {
        //if (x == endx) { break; }
        //*ptr = y;
        e2 = err2; //2 * err;
        if (e2 >= dy) { err2 += dy2; x++; output[x] = y; } // e_xy+e_x > 0 
        if (e2 <= dx) { err2 += dx2; y += sy; } // e_xy+e_y < 0 
    }
}



u8* yclip;
s16* raster_buffer_0;
s16* raster_buffer_1;

void init_2d_buffers() {
    yclip = MEM_alloc(W*2*sizeof(u8));
    raster_buffer_0 = MEM_alloc(W*2*sizeof(s16));
    raster_buffer_1 = raster_buffer_0+W;
}

void clear_2d_buffers() {
    for(int i = 0; i < W; i++) {
        yclip[i*2] = 0;
        yclip[i*2+1] = BMP_HEIGHT-1;
    }
}

void release_2d_buffers() {
    MEM_free(yclip);
    MEM_free(raster_buffer_0);
}

#define SUBPIXEL_SHIFT 8



    #define LOOP_GET_DRAW_PARAMS    do {                    \
        top_y_int = top_y_fix >> SUBPIXEL_SHIFT;                        \
        bot_y_int = bot_y_fix >> SUBPIXEL_SHIFT;                        \
        min_drawable_y = *yclip_ptr;                        \
        max_drawable_y = *(yclip_ptr+1);                    \
        top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);    \
        bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);    \
    } while(0)

    #define LOOP_SINGLE_GET_DRAW_PARAMS do { \
        y_int = y_fix >> SUBPIXEL_SHIFT;     \
    } while(0)

    #define LOOP_PREDRAW_NEXT_COLUMN do {    \
        top_y_fix += top_dy_per_dx;  \
        bot_y_fix += bot_dy_per_dx;  \
        x++;                         \
    } while(0) 

    #define LOOP_SINGLE_PREDRAW_NEXT_COLUMN do {    \
        y_fix += dy_per_dx;  \
        x++;                         \
    } while(0) 


    #define LOOP_NEXT_COLUMN do {   \
        LOOP_PREDRAW_NEXT_COLUMN;   \
        col_ptr++;                  \
        yclip_ptr+=2;               \
    } while(0)

    #define LOOP_SINGLE_NEXT_COLUMN do {   \
        LOOP_SINGLE_PREDRAW_NEXT_COLUMN;   \
        col_ptr++;                  \
    } while(0)

void draw_vertical_line(s16 y0, s16 y1, u8 col, u8* col_ptr) {
    u8* cur_ptr = col_ptr + y0*128;
    u8* end_ptr = col_ptr + y1*128;
    //u8 cnt = 1+y1-y0;
    while(cur_ptr <= end_ptr) {
        *cur_ptr = col;
        cur_ptr += 128;
    }
}


void draw_native_vertical_line(s16 y0, s16 y1, u8 col, u8* col_ptr);

//void draw_native_vertical_line_unrolled(s16 y0, s16 y1, u8 col, u8* col_ptr);
void draw_native_vertical_line_unrolled_inner(u16 jump_table_offset, u8 col, u8* col_ptr);

void draw_native_vertical_line_unrolled(s16 y0, s16 y1, u8 col, u8* col_ptr) {
    if(y0 >= y1) { return ;}
    col_ptr += (128 * y0);


    u8 dy = y1-y0;
    u16 jump_table_offset = (160-dy)*4;
    //KDebug_StartTimer();
    draw_native_vertical_line_unrolled_inner(jump_table_offset, col, col_ptr);
    //KDebug_StopTimer();
}

#define FLAT_COLOR

void flip() {
    /*
    if(JOY_readJoypad(JOY_1) & BUTTON_A) {
        
        BMP_flip(0);
        waitMs(200);
        BMP_clear();
    }
    */
}

void draw_between_raster_spans(s16* top, s16* bot, u16 startx, u16 endx, u8 col,
                               uint8_t update_top_clip, uint8_t update_bot_clip) {
    s16* top_ptr = top+startx;
    s16* bot_ptr = bot+startx;
    u8* yclip_ptr = &(yclip[startx<<1]);
    u8* col_ptr = BMP_getWritePointer(startx<<1, 0);
    for(u16 x = startx; x <= endx; x++) {
        u8 min_drawable_y = *yclip_ptr;
        u8 max_drawable_y = *(yclip_ptr+1);
        s16 top_y = *top_ptr++;
        s16 bot_y = *bot_ptr++;
        u8 top_draw_y = clamp(top_y, min_drawable_y, max_drawable_y);
        u8 bot_draw_y = clamp(bot_y, min_drawable_y, max_drawable_y);

        if(top_draw_y < bot_draw_y) {
            draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, col, col_ptr);
        }

        col_ptr++;
        if(update_top_clip) {
            *yclip_ptr++ = bot_draw_y;
            yclip_ptr++;
        } else if (update_bot_clip) {
            yclip_ptr++;
            *yclip_ptr++ = top_draw_y;
        } else {
            yclip_ptr += 2;
        }
    }
}


void draw_from_top_to_raster_span(s16* top, u16 startx, u16 endx, u8 col, u8 update_top_clip) {
    s16* top_ptr = top+startx;
    u8* yclip_ptr = &(yclip[startx<<1]);
    u8* col_ptr = BMP_getWritePointer(startx<<1, 0);

    for(u16 x = startx; x <= endx; x++) {
        u8 min_drawable_y = *yclip_ptr;
        u8 max_drawable_y = *(yclip_ptr+1);
        s16 wall_top_y = *top_ptr++;
        //u8 top_draw_y = clamp(top_y, min_drawable_y, max_drawable_y);
        u8 top_draw_y = clamp(0, min_drawable_y, max_drawable_y);
        u8 wall_top_draw_y = clamp(wall_top_y, min_drawable_y, max_drawable_y);

        if(top_draw_y < wall_top_draw_y) {
            draw_native_vertical_line_unrolled(top_draw_y, wall_top_draw_y, col, col_ptr);
        }

        col_ptr++;
        if(update_top_clip) {
            *yclip_ptr++ = wall_top_draw_y;
            yclip_ptr++;
        } else {
            yclip_ptr += 2;
        }
    }
}

void draw_from_raster_span_to_bot(s16* bot, u16 startx, u16 endx, u8 col, u8 update_bot_clip) {
    s16* bot_ptr = bot+startx;
    u8* yclip_ptr = &(yclip[startx<<1]);
    u8* col_ptr = BMP_getWritePointer(startx<<1, 0);

    for(u16 x = startx; x <= endx; x++) {
        u8 min_drawable_y = *yclip_ptr;
        u8 max_drawable_y = *(yclip_ptr+1);
        s16 wall_bot_y = *bot_ptr++;
        //u8 top_draw_y = clamp(top_y, min_drawable_y, max_drawable_y);
        u8 wall_bot_draw_y = clamp(wall_bot_y, min_drawable_y, max_drawable_y);
        u8 bot_draw_y = clamp(H-1, min_drawable_y, max_drawable_y);

        if(wall_bot_draw_y < bot_draw_y) {
            draw_native_vertical_line_unrolled(wall_bot_draw_y, bot_draw_y, col, col_ptr);
        }

        col_ptr++;
        if(update_bot_clip) {
            yclip_ptr++;
            *yclip_ptr++ = wall_bot_draw_y;
        } else {
            yclip_ptr += 2;
        }
    }
}

void draw_line_to_buffer(s16 x1, s16 x1_y, s16 x2, s16 x2_y, 
                         u16 window_min, u16 window_max,
                         s16* out_buffer) { 
    s16 dy = x2_y - x1_y;
    s16 dx = x2-x1;

    s32 dy_per_dx = (s16)(((s32)(dy<<SUBPIXEL_SHIFT)) / (s16)dx); // 12.8

    s32 y_fix = x1_y<<SUBPIXEL_SHIFT;

    s16 y_int;

    s16 beginx = max(x1, window_min);

    s16 skip_x = beginx - x1;
    if(skip_x > 0) {
        y_fix += (skip_x * dy_per_dx);
    }

    s16* col_ptr = &out_buffer[beginx];
    
    y_int = y_fix >> SUBPIXEL_SHIFT;
    *col_ptr++ = y_int;
    y_fix += dy_per_dx;


    s16 endx = min(window_max, x2);
    for(s16 x = beginx; x < endx; x++) {
        y_int = y_fix >> SUBPIXEL_SHIFT;
        *col_ptr++ = y_int;
        y_fix += dy_per_dx;
    }

    y_int = y_fix >> SUBPIXEL_SHIFT;
    *col_ptr = y_int;
    flip(0);

    return; 

}

void draw_wall(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 window_min, u16 window_max,
              u8 col) {
    
    s16 top_dy = x2_ytop - x1_ytop;
    s16 bot_dy = x2_ybot - x1_ybot;
    s16 dx = x2-x1;

    s32 top_dy_per_dx = (s16)(((s32)(top_dy<<SUBPIXEL_SHIFT)) / (s16)dx); // 12.8
    s32 bot_dy_per_dx = (s16)(((s32)(bot_dy<<SUBPIXEL_SHIFT)) / (s16)dx); // 12.8

    s32 top_y_fix = x1_ytop<<SUBPIXEL_SHIFT;
    s32 bot_y_fix = x1_ybot<<SUBPIXEL_SHIFT; // 9.7

    s16 top_y_int, bot_y_int;
    u8 min_drawable_y, max_drawable_y;
    
    u8 top_draw_y, bot_draw_y;

    s16 beginx = max(x1, window_min);
    s16 x = x1;

    s16 skip_x = beginx - x1;
    if(skip_x) {
        top_y_fix += (skip_x * top_dy_per_dx);
        bot_y_fix += (skip_x * bot_dy_per_dx);
        x = beginx;
    }

    u8* col_ptr = BMP_getWritePointer(x<<1, 0);
    u8* yclip_ptr = &(yclip[x*2]);
    
    LOOP_GET_DRAW_PARAMS;
    
    if(top_draw_y <= bot_draw_y) {
        draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, col, col_ptr);
        //draw_vertical_line(top_draw_y, bot_draw_y, col, col_ptr);
    }

    LOOP_NEXT_COLUMN;

    s16 endx = min(window_max, x2);
    while(x < endx) {

        LOOP_GET_DRAW_PARAMS;

        if(top_draw_y <= bot_draw_y) { 
            #ifdef FLAT_COLOR
            draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, col, col_ptr);
            //draw_vertical_line(top_draw_y, bot_draw_y, col, col_ptr);
            #else
            *(col_ptr+(top_draw_y*128)) = col;
            *(col_ptr+(bot_draw_y*128)) = col; 
            #endif
         }
        
        LOOP_NEXT_COLUMN;

    }

    LOOP_GET_DRAW_PARAMS;
    if(top_draw_y <= bot_draw_y) {
        draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, col, col_ptr);
        //draw_vertical_line(top_draw_y, bot_draw_y, col, col_ptr);
    }

    flip(0);

    return; 
}


void draw_top_step(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 window_min, u16 window_max,
              u8 col) {
    
    s16 top_dy = x2_ytop - x1_ytop;
    s16 bot_dy = x2_ybot - x1_ybot;
    s16 dx = x2-x1;

    s32 top_dy_per_dx = (top_dy<<SUBPIXEL_SHIFT) / dx; // 12.8
    s32 bot_dy_per_dx = (bot_dy<<SUBPIXEL_SHIFT) / dx; // 12.8

    s32 top_y_fix = x1_ytop<<SUBPIXEL_SHIFT;
    s32 bot_y_fix = x1_ybot<<SUBPIXEL_SHIFT; // 9.7

    s16 top_y_int, bot_y_int;
    u8 min_drawable_y, max_drawable_y;
    u8 top_draw_y, bot_draw_y;

    u16 beginx = max(x1, window_min);
    u16 x = beginx;

    s16 skip_x = beginx - x1;
    if(skip_x) {
        top_y_fix += (skip_x * top_dy_per_dx);
        bot_y_fix += (skip_x * bot_dy_per_dx);
        x = beginx;
    }


    u8* col_ptr = BMP_getWritePointer(x<<1, 0);
    u8* yclip_ptr = &(yclip[x<<1]);

    LOOP_GET_DRAW_PARAMS;
    
    if(top_draw_y <= bot_draw_y) {
        draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, col, col_ptr);
        //draw_vertical_line(top_draw_y, bot_draw_y, col, col_ptr);
        *yclip_ptr = bot_draw_y;
    }

    LOOP_NEXT_COLUMN;

    s16 endx = min(window_max, x2);
    while(x < endx) {

        LOOP_GET_DRAW_PARAMS;

        if(top_draw_y < bot_draw_y) { 
            #ifdef FLAT_COLOR
            draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, col, col_ptr);
            //draw_vertical_line(top_draw_y, bot_draw_y, col, col_ptr);
            #else
            *(col_ptr+(top_draw_y*128)) = col;
            *(col_ptr+(bot_draw_y*128)) = col; 
            #endif
            *yclip_ptr = bot_draw_y;
         }
        LOOP_NEXT_COLUMN;

    }

    LOOP_GET_DRAW_PARAMS;
    if(top_draw_y <= bot_draw_y) {
        draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, col, col_ptr);
        //draw_vertical_line(top_draw_y, bot_draw_y, col, col_ptr);
        *yclip_ptr = bot_draw_y;
        //*yclip_ptr = clamp(bot_draw_y, min_drawable_y, max_drawable_y);
        //ytop[x] = bot_draw_y;
        //ytop[x] =  clamp(bot_draw_y, min_drawable_y, max_drawable_y);
    }

    flip();
    return; 
}

void draw_bot_step(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 window_min, u16 window_max,
              u8 col) {
    
    //u8 col = 0x11;

    s16 top_dy = x2_ytop - x1_ytop;
    s16 bot_dy = x2_ybot - x1_ybot;
    s16 dx = x2-x1;

    s32 top_dy_per_dx = (top_dy<<SUBPIXEL_SHIFT) / dx; // 12.8
    s32 bot_dy_per_dx = (bot_dy<<SUBPIXEL_SHIFT) / dx; // 12.8

    s32 top_y_fix = x1_ytop<<SUBPIXEL_SHIFT;
    s32 bot_y_fix = x1_ybot<<SUBPIXEL_SHIFT; // 9.7

    s16 top_y_int, bot_y_int;
    u8 min_drawable_y, max_drawable_y;
    u8 top_draw_y, bot_draw_y;

    s16 beginx = max(x1, window_min);
    s16 x = beginx;

    s16 skip_x = beginx - x1;
    if(skip_x) {
        top_y_fix += (skip_x * top_dy_per_dx);
        bot_y_fix += (skip_x * bot_dy_per_dx);
        x = beginx;
    }

    u8* col_ptr = BMP_getWritePointer(x<<1, 0);
    u8* yclip_ptr = &(yclip[x<<1]);

    LOOP_GET_DRAW_PARAMS;
    
    if(top_draw_y <= bot_draw_y) {
        draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, col, col_ptr);
        //draw_vertical_line(top_draw_y, bot_draw_y, col, col_ptr);

    }
    *(yclip_ptr+1) = top_draw_y;

    LOOP_NEXT_COLUMN;

    s16 endx = min(window_max, x2);
    while(x < endx) {

        LOOP_GET_DRAW_PARAMS;

        if(top_draw_y <= bot_draw_y) { 
            #ifdef FLAT_COLOR
            draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, col, col_ptr);
            //draw_vertical_line(top_draw_y, bot_draw_y, col, col_ptr);
            #else
            *(col_ptr+(top_draw_y*128)) = col;
            *(col_ptr+(bot_draw_y*128)) = col; 
            #endif
            *(yclip_ptr+1) = top_draw_y;
         }
        LOOP_NEXT_COLUMN;

    }

    LOOP_GET_DRAW_PARAMS;
    if(top_draw_y <= bot_draw_y) {
        draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, col, col_ptr);
        //draw_vertical_line(top_draw_y, bot_draw_y, col, col_ptr);
        *(yclip_ptr+1) = top_draw_y;
    }

    flip();
    return; 
}