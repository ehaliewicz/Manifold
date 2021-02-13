#include <genesis.h>
#include <kdebug.h>
#include "draw.h"
#include "game.h"
#include "level.h"
#include "math3d.h"
#include "portal.h"




u8* yclip;
s16* raster_buffer_0;
s16* raster_buffer_1;


#define PIXEL_RIGHT_STEP 1
#define PIXEL_DOWN_STEP 4
//#define PIXEL_DOWN_STEP 2
#define TILE_RIGHT_STEP 640

u16 get_index(u16 x, u16 y) {
  u16 x_col_offset = x & 1;
  u16 base_offset = 0;
  if(x & 0b10) {
    // use right half of framebuffer
    base_offset = (W/2)*H;
  }
  u16 y_offset = y * 2;
  u16 x_num_pair_cols_offset = x >> 2;
  u16 x_cols_offset = x_num_pair_cols_offset * H * 2;
  return base_offset + y_offset + x_col_offset + x_cols_offset + 16;
}

//inline u8* getDMAWritePointer(u16 x, u16 y) {
    //return 
//  return &bmp_buffer_write[get_index(x, y)];
//}

inline u16 getDMAWriteOffset(u16 x, u16 y) {
    return get_index(x, y);
    //return 
  //return &bmp_buffer_write[get_index(x, y)];
}

u16* column_table;

void init_column_offset_table() {
  // offset from last column
  //column_table[0] = 0;
  //u8* last_ptr = getDMAWritePointer(0, 0);

  for(int x = 0; x < W; x++) {
    u16 cur_off = getDMAWriteOffset(x, 0);    
    column_table[x] = cur_off; //cur_ptr-last_ptr;
    //last_ptr = cur_ptr;
  }
}


void init_2d_buffers() {
    yclip = MEM_alloc(W*2*sizeof(u8));
    raster_buffer_0 = MEM_alloc(W*2*sizeof(s16));
    raster_buffer_1 = raster_buffer_0+W;
    column_table = MEM_alloc(sizeof(u16) * W); //sizeof(16) * W);
    init_column_offset_table();
}

void clear_2d_buffers() {
    for(int i = 0; i < W; i++) {
        yclip[i*2] = 8;
        yclip[i*2+1] = 151; //(H-8)-1;
    }
}

void release_2d_buffers() {
    MEM_free(yclip);
    MEM_free(raster_buffer_0);
    MEM_free(column_table);
}

#define SUBPIXEL_SHIFT 8


void draw_vertical_line(s16 y0, s16 y1, u8 col, u8* col_ptr) {
    u8* cur_ptr = col_ptr + y0*128;
    u8* end_ptr = col_ptr + y1*128;
    //u8 cnt = 1+y1-y0;
    while(cur_ptr <= end_ptr) {
        *cur_ptr = col;
        cur_ptr += 128;
    }
}



void draw_native_vertical_line_unrolled_inner(u16 jump_table_offset, u8 col, u8* col_ptr);
void vline_native_dither_movep(u8* buf, u8 extra_pix, s16 jump_table_offset, u32 col1_col2);


void draw_native_vertical_line_unrolled(s16 x, s16 y0, s16 y1, u8 col) {


    u8* col_ptr = bmp_buffer_write + column_table[x] + (y0<<1);
    u32 full_col = (col << 24) | (col << 16) | (col<<8) | col;

    u16 dy = (y1-y0);
    u16 dy_movep = dy>>2;
    
    if(dy_movep > 0) { 
        u16 extra_pix = dy&0b11;
        if(extra_pix) {
            __asm volatile(
                "subq #1, %2\t\n\
                extra_pix_lp:\t\n\
                move.b %0, (%1)\t\n\
                addq.l #2, %1\t\n\
                dbeq %2, extra_pix_lp"
                : 
                : "d" (col), "a" (col_ptr), "d" (extra_pix)
                );
        }

        u16 jump_table_offset = (40-dy_movep)<<2;
        __asm volatile(
           "jmp mvp_tbl_%=(%%pc, %0)\t\n\
           mvp_tbl_%=:\t\n\
            movep.l %1, 312(%2) \t\n\
            movep.l %1, 304(%2) \t\n\
            movep.l %1, 296(%2) \t\n\
            movep.l %1, 288(%2) \t\n\
            movep.l %1, 280(%2) \t\n\
            movep.l %1, 272(%2) \t\n\
            movep.l %1, 264(%2) \t\n\
            movep.l %1, 256(%2) \t\n\
            movep.l %1, 248(%2) \t\n\
            movep.l %1, 240(%2) \t\n\
            movep.l %1, 232(%2) \t\n\
            movep.l %1, 224(%2) \t\n\
            movep.l %1, 216(%2) \t\n\
            movep.l %1, 208(%2) \t\n\
            movep.l %1, 200(%2) \t\n\
            movep.l %1, 192(%2) \t\n\
            movep.l %1, 184(%2) \t\n\
            movep.l %1, 176(%2) \t\n\
            movep.l %1, 168(%2) \t\n\
            movep.l %1, 160(%2) \t\n\
            movep.l %1, 152(%2) \t\n\
            movep.l %1, 144(%2) \t\n\
            movep.l %1, 136(%2) \t\n\
            movep.l %1, 128(%2) \t\n\
            movep.l %1, 120(%2) \t\n\
            movep.l %1, 112(%2) \t\n\
            movep.l %1, 104(%2) \t\n\
            movep.l %1, 96(%2) \t\n\
            movep.l %1, 88(%2) \t\n\
            movep.l %1, 80(%2) \t\n\
            movep.l %1, 72(%2) \t\n\
            movep.l %1, 64(%2) \t\n\
            movep.l %1, 56(%2) \t\n\
            movep.l %1, 48(%2) \t\n\
            movep.l %1, 40(%2) \t\n\
            movep.l %1, 32(%2) \t\n\
            movep.l %1, 24(%2) \t\n\
            movep.l %1, 16(%2) \t\n\
            movep.l %1, 8(%2) \t\n\
            movep.l %1, 0(%2)"
            : 
            : "d"(jump_table_offset), "d" (full_col), "a" (col_ptr)
        );

    } else {
        __asm volatile(
            "subq #1, %2\t\n\
            small_col_lp_%=:\t\n\
            move.b %0, (%1)\t\n\
            addq.l #2, %1\t\n\
            dbeq %2, small_col_lp_%="
            : 
            : "d" (col), "a" (col_ptr), "d" (dy)
            );

        
    }
   
}

#define FLAT_COLOR

void flip() {
    /*
    if(JOY_readJoypad(JOY_1) & BUTTON_A) {
        request_flip();
        //BMP_flip(0, 1);
        //waitMs(200);
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
            draw_native_vertical_line_unrolled(x, top_draw_y, bot_draw_y, col); //, col_ptr);
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
    flip();
}


void draw_from_top_to_raster_span(s16* top, u16 startx, u16 endx, u8 col, u8 update_top_clip) {
    s16* top_ptr = top+startx;
    u8* yclip_ptr = &(yclip[startx<<1]);
    u8* col_ptr = BMP_getWritePointer(startx<<1, 0);

    for(u16 x = startx; x <= endx; x++) {
        u8 min_drawable_y = *yclip_ptr;
        u8 max_drawable_y = *(yclip_ptr+1);
        s16 wall_top_y = *top_ptr++;
        u8 top_draw_y = min_drawable_y;
        u8 wall_top_draw_y = clamp(wall_top_y, min_drawable_y, max_drawable_y);

        if(top_draw_y < wall_top_draw_y) {
            draw_native_vertical_line_unrolled(x, top_draw_y, wall_top_draw_y, col); //, col_ptr);
        }

        col_ptr++;
        if(update_top_clip) {
            *yclip_ptr++ = wall_top_draw_y;
            yclip_ptr++;
        } else {
            yclip_ptr += 2;
        }
    }
    flip();
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
        u8 bot_draw_y = max_drawable_y;

        if(wall_bot_draw_y < bot_draw_y) {
            draw_native_vertical_line_unrolled(x, wall_bot_draw_y, bot_draw_y, col); //, col_ptr);
        }

        col_ptr++;
        if(update_bot_clip) {
            yclip_ptr++;
            *yclip_ptr++ = wall_bot_draw_y;
        } else {
            yclip_ptr += 2;
        }
    }
    flip();
}

void draw_fix_line_to_buffer(s16 x1, fix32 x1_y, s16 x2, fix32 x2_y, 
                             u16 window_min, u16 window_max,
                             s16* out_buffer) {
    s16 dy_fix = x2_y - x1_y; // we have 4 subpixel bits
    s16 dx = (x2-x1)+1;

    fix32 dy_per_dx = (dy_fix<<12) / dx; // (dy_fix<<4) / dx; // 22.10

    fix32 y_fix = x1_y<<12; //4;   // 10 subpixel bits
    
    //s16 y_int;

    s16 beginx = max(x1, window_min);

    s16 skip_x = beginx - x1;
    if(skip_x > 0) {
        y_fix += (skip_x * dy_per_dx);
    }

    s16* col_ptr = &out_buffer[beginx];

    s16 endx = min(window_max, x2);
    s16 loop_dx = (endx+1)-beginx;


    if(dy_per_dx >= 0) {
        __asm volatile(
            "  subq #1, %0\t\n\
                add.w %1, %2\t\n\
                swap %1\t\n\
                swap %2\t\n\
            addx_fast_%=:\t\n\
                move.w %2, (%3)+\t\n\
                addx.l %1, %2\t\n\
                dbeq %0,  addx_fast_%="
            :
            : "d" (loop_dx), "d" (dy_per_dx), "d" (y_fix), "a" (col_ptr)
        );
    } else {

        __asm volatile(
        "  subq #1, %0\t\n\
            blah_%=:\t\n\
            swap %2\t\n\
            move.w %2, (%3)+\t\n\
            swap %2\t\n\
            add.l %1, %2\t\n\
            dbeq %0,  blah_%="
            :
            : "d" (loop_dx), "d" (dy_per_dx), "d" (y_fix), "a" (col_ptr)
        );
        

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

    return; 

}

void draw_wall(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 window_min, u16 window_max,
              u8 ceil_col, u8 wall_col, u8 floor_col) {


    // 4 subpixel bits here
    s16 top_dy_fix = x2_ytop - x1_ytop;
    s16 bot_dy_fix = x2_ybot - x1_ybot;

    s16 dx = x2-x1;

    fix32 top_dy_per_dx = (top_dy_fix<<12) / dx; // (dy_fix<<4) / dx; // 22.10
    fix32 bot_dy_per_dx = (bot_dy_fix<<12) / dx;

    fix32 top_y_fix = x1_ytop<<12; 
    fix32 bot_y_fix = x1_ybot<<12;
    s16 top_y_int;
    s16 bot_y_int;

    s16 beginx = max(x1, window_min);

    s16 skip_x = beginx - x1;
    if(skip_x > 0) {
        top_y_fix += (skip_x * top_dy_per_dx);
        bot_y_fix += (skip_x * bot_dy_per_dx);
    }


    s16 endx = min(window_max, x2);

    s16 x = beginx;
    u8* col_ptr = BMP_getWritePointer(x<<1, 0);
    u8* yclip_ptr = &(yclip[x*2]);

    for(;x <= endx; x++) {
        top_y_int = top_y_fix >> 16;
        bot_y_int = bot_y_fix >> 16;
        u8 min_drawable_y = *yclip_ptr++;
        u8 max_drawable_y = *yclip_ptr++;
        u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);
        u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);
        if(min_drawable_y < top_draw_y) {
            draw_native_vertical_line_unrolled(x, min_drawable_y, top_draw_y, ceil_col); //, col_ptr);
        }
        if(top_draw_y < bot_draw_y) {
            draw_native_vertical_line_unrolled(x, top_draw_y, bot_draw_y, wall_col); //, col_ptr);
        }
        if(bot_draw_y < max_drawable_y) {
            draw_native_vertical_line_unrolled(x, bot_draw_y, max_drawable_y, floor_col); //, col_ptr);
        }

        col_ptr++;
        top_y_fix += top_dy_per_dx;
        bot_y_fix += bot_dy_per_dx;
    }
    
    flip(0);

    return; 
}

