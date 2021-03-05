#include <genesis.h>
#include <kdebug.h>
#include "draw.h"
#include "game.h"
#include "level.h"
#include "math3d.h"
#include "portal.h"




u8* yclip;


#define PIXEL_RIGHT_STEP 1
#define PIXEL_DOWN_STEP 4
//#define PIXEL_DOWN_STEP 2
#define TILE_RIGHT_STEP 4*SCREEN_HEIGHT //640

u16 get_index(u16 x, u16 y) {
  u16 x_col_offset = x & 1;
  u16 base_offset = 0;
  if(x & 0b10) {
    // use right half of framebuffer
    base_offset = (SCREEN_WIDTH/2)*SCREEN_HEIGHT;
  }
  u16 y_offset = y * 2;
  u16 x_num_pair_cols_offset = x >> 2;
  u16 x_cols_offset = x_num_pair_cols_offset * SCREEN_HEIGHT * 2;
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

  for(int x = 0; x < SCREEN_WIDTH; x++) {
    //u16 cur_off = getDMAWriteOffset(x, 0);
    u16 cur_off = getDMAWriteOffset(x, 0); // only get odd columns
    column_table[x] = cur_off; 
  }
}


void init_2d_buffers() {
    yclip = MEM_alloc(SCREEN_WIDTH*2*sizeof(u8));
    column_table = MEM_alloc(sizeof(u16) * SCREEN_WIDTH); //sizeof(16) * W);
    init_column_offset_table();
}

void clear_2d_buffers() {
    for(int i = 0; i < SCREEN_WIDTH; i++) {
        yclip[i*2] = 0;
        yclip[i*2+1] = SCREEN_HEIGHT-10; // 8; idk why this is messed up 
    }
}

void release_2d_buffers() {
    MEM_free(yclip);
    MEM_free(column_table);
}


// do texture map stuff :^)

__attribute__((noinline)) void draw_tex_column(col_params params) {
    u8 x0 = params.x;
    //u8 x1 = params.x1;
    u8 y0 = params.y0;
    u8 y1 = params.y1;
    u16 dy = y1 - y0;
    //u16 dx = x1 - x0;

    if(dy == 0) { return; } // || dx == 0) { return ;}
    Bitmap *b = params.bmp;
    s32 tex_size_fix = b->h/2 << 16;
    s32 dv_dy = tex_size_fix / dy; // 8.16
    u8* tex = params.bmp->image;
    s32 swapped_dv_dy = (dv_dy << 16) | (dv_dy>>16);

    #define TEX_SIZE 64
    /*
        roughly 100ms?
    for(int x = params.x; x < params.x + TEX_SIZE; x++) {
        u8* off = bmp_buffer_write + column_table[x] + (y0<<1);
        s32 v_fix = 0;
        for(int y = y0; y <= y1; y++) {
            char c = (tex[v_fix>>16]);
            *off = c;
            off += 2;
            v_fix += dv_dy;
        }
        // go to next row in texture (which is actually a column since it's rotated 90 degrees)
        tex += 128;
    }
    //return;
    */



    //addx texture map experiment code 
    // 5fps faster..
    // 66ms to texture map 91% of the screen, pretty fast

    for(int x = params.x; x < params.x + TEX_SIZE; x++) {
        u8* off = bmp_buffer_write + column_table[x] + (y0<<1);
        u32 v_fix = 0;

        // reset dy because the inline asm below clobbers it
        dy = (y1-y0);
        
        
        __asm volatile(
        " subq #1, %0\t\n\
        lp_%=:\t\n\
            move.b 0(%4, %2.w), (%1)\t\n\
            addx.l %3, %2\t\n\
            addq.l #2, %1\t\n\
            dbeq %0, lp_%="
            : "+d" (dy), "+a" (off),  "+d" (v_fix)
            : "d" (swapped_dv_dy), "a" (tex)
        );
        tex += 128;

        //tex += b->h;
    }
    

}


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


    u8* col_ptr = bmp_buffer_write + column_table[x<<1] + (y0<<1);
    u16* word_col_ptr = (u16*)col_ptr;
    u32 full_col = (col << 24) | (col << 16) | (col<<8) | col;

    u32 word_col = (col<<8) | col;

    u16 dy = (y1-y0);
    if(dy& 0b1) {
        *word_col_ptr++ = word_col;
    }
    u32* lw_col_ptr = word_col_ptr;

    dy>>=1;
    for(int y = 0; y < dy; y++) {
        *lw_col_ptr++ = full_col;
    }
    //for(int y = y0; y <= y1; y++) {
    //    *word_col_ptr++ = word_col;
    //}

    return;

    u16 dy_movep = dy>>2;
    
    if(dy_movep > 0) { 
        u16 extra_pix = dy&0b11;    
        if(extra_pix) {
            __asm volatile(
                "subq #1, %2\t\n\
                extra_pix_lp_%=:\t\n\
                move.b %1, (%0)\t\n\
                addq.l #2, %0\t\n\
                dbeq %2, extra_pix_lp_%="
                : "=a" (col_ptr)
                : "d" (col), "d" (extra_pix)
                );
        }
        /*
            movep.l %1, 312(%2) \t\n\
            movep.l %1, 304(%2) \t\n\
            movep.l %1, 296(%2) \t\n\
            movep.l %1, 288(%2) \t\n\
        */

        u16 jump_table_offset = ((SCREEN_HEIGHT/4)-dy_movep)<<2;
        __asm volatile(
           "jmp mvp_tbl_%=(%%pc, %0)\t\n\
           mvp_tbl_%=:\t\n\
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
    //if(JOY_readJoypad(JOY_1) & BUTTON_A) {
    //    request_flip();
    //    waitMs(500);
    //    BMP_clear();
    //}
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
    u8* yclip_ptr = &(yclip[x<<1]);

    for(;x <= endx; x++) {
        top_y_int = top_y_fix >> 16;
        bot_y_int = bot_y_fix >> 16;
        u8 min_drawable_y = *yclip_ptr++;
        u8 max_drawable_y = *yclip_ptr++;
        u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);
        u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);
        if(min_drawable_y < top_draw_y) {
            draw_native_vertical_line_unrolled(x, min_drawable_y, top_draw_y, ceil_col);
        }
        if(top_draw_y < bot_draw_y) {
            draw_native_vertical_line_unrolled(x, top_draw_y, bot_draw_y, wall_col);
        }
        if(bot_draw_y < max_drawable_y) {
            draw_native_vertical_line_unrolled(x, bot_draw_y, max_drawable_y, floor_col);
        }

        top_y_fix += top_dy_per_dx;
        bot_y_fix += bot_dy_per_dx;
    }
    
    flip(0);

    return; 
}



void draw_upper_step(s16 x1, s16 x1_ytop, s16 nx1_ytop, s16 x2, s16 x2_ytop, s16 nx2_ytop, u16 window_min, u16 window_max, u8 upper_color, u8 ceil_color) {
        // 4 subpixel bits here
    s16 top_dy_fix = x2_ytop - x1_ytop;
    s16 ntop_dy_fix = nx2_ytop - nx1_ytop;

    s16 dx = x2-x1;

    fix32 top_dy_per_dx = (top_dy_fix<<12) / dx;
    fix32 ntop_dy_per_dx = (ntop_dy_fix<<12) / dx;

    fix32 top_y_fix = x1_ytop<<12; 
    fix32 ntop_y_fix = nx1_ytop<<12;
    s16 top_y_int;
    s16 ntop_y_int;

    s16 beginx = max(x1, window_min);

    s16 skip_x = beginx - x1;

    if(skip_x > 0) {
        top_y_fix += (skip_x * top_dy_per_dx);
        ntop_y_fix += (skip_x * ntop_dy_per_dx);
    }


    s16 endx = min(window_max, x2);

    s16 x = beginx;
    u8* yclip_ptr = &(yclip[x<<1]);

    for(;x <= endx; x++) {
        top_y_int = top_y_fix >> 16;
        ntop_y_int = ntop_y_fix >> 16;
        u8 min_drawable_y = *yclip_ptr;
        u8 max_drawable_y = *(yclip_ptr+1);
        u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);
        u8 bot_draw_y = clamp(ntop_y_int, min_drawable_y, max_drawable_y);

        if(top_draw_y < bot_draw_y) {
            draw_native_vertical_line_unrolled(x, top_draw_y, bot_draw_y, upper_color);
            *yclip_ptr++ = bot_draw_y;
            yclip_ptr++;
        } else {
            yclip_ptr += 2;
        }
        if(min_drawable_y < top_draw_y) {
            draw_native_vertical_line_unrolled(x, min_drawable_y, top_draw_y, ceil_color);
        }

        top_y_fix += top_dy_per_dx;
        ntop_y_fix += ntop_dy_per_dx;
    }
    
    flip(0);

    return; 
}

void draw_lower_step(s16 x1, s16 x1_ybot, s16 nx1_ybot, s16 x2, s16 x2_ybot, s16 nx2_ybot, u16 window_min, u16 window_max, u8 lower_color, u8 floor_color) {
        // 4 subpixel bits here
    s16 bot_dy_fix = x2_ybot - x1_ybot;
    s16 nbot_dy_fix = nx2_ybot - nx1_ybot;

    s16 dx = x2-x1;

    fix32 bot_dy_per_dx = (bot_dy_fix<<12) / dx;
    fix32 nbot_dy_per_dx = (nbot_dy_fix<<12) / dx;

    fix32 bot_y_fix = x1_ybot<<12; 
    fix32 nbot_y_fix = nx1_ybot<<12;
    s16 bot_y_int;
    s16 nbot_y_int;

    s16 beginx = max(x1, window_min);

    s16 skip_x = beginx - x1;
    if(skip_x > 0) {
        bot_y_fix += (skip_x * bot_dy_per_dx);
        nbot_y_fix += (skip_x * nbot_dy_per_dx);
    }


    s16 endx = min(window_max, x2);

    s16 x = beginx;
    u8* yclip_ptr = &(yclip[x<<1]);

    for(;x <= endx; x++) {
        bot_y_int = bot_y_fix >> 16;
        nbot_y_int = nbot_y_fix >> 16;
        u8 min_drawable_y = *yclip_ptr;
        u8 max_drawable_y = *(yclip_ptr+1);
        u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);
        u8 top_draw_y = clamp(nbot_y_int, min_drawable_y, max_drawable_y);

        if(top_draw_y < bot_draw_y) {
            draw_native_vertical_line_unrolled(x, top_draw_y, bot_draw_y, lower_color);
            yclip_ptr++;
            *yclip_ptr++ = top_draw_y;
        } else {
            yclip_ptr += 2;
        }
        if(max_drawable_y > bot_draw_y) {
            draw_native_vertical_line_unrolled(x, bot_draw_y, max_drawable_y, floor_color);
        }

        bot_y_fix += bot_dy_per_dx;
        nbot_y_fix += nbot_dy_per_dx;
    }
    
    flip(0);

    return; 
}

void draw_ceiling_update_clip(s16 x1, s16 x1_ytop, s16 x2, s16 x2_ytop, u16 window_min, u16 window_max, u8 ceil_color) {
    // 4 subpixel bits here
    s16 top_dy_fix = x2_ytop - x1_ytop;

    s16 dx = x2-x1;

    fix32 top_dy_per_dx = (top_dy_fix<<12) / dx;

    fix32 top_y_fix = x1_ytop<<12; 
    s16 top_y_int;

    s16 beginx = max(x1, window_min);

    s16 skip_x = beginx - x1;
    if(skip_x > 0) {
        top_y_fix += (skip_x * top_dy_per_dx);
    }


    s16 endx = min(window_max, x2);

    s16 x = beginx;
    u8* yclip_ptr = &(yclip[x<<1]);

    for(;x <= endx; x++) {
        top_y_int = top_y_fix >> 16;
        u8 min_drawable_y = *yclip_ptr;
        u8 max_drawable_y = *(yclip_ptr+1);
        u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);

        if(min_drawable_y < top_draw_y) {
            draw_native_vertical_line_unrolled(x, min_drawable_y, top_draw_y, ceil_color);
            *yclip_ptr++ = top_draw_y;
            yclip_ptr++;
        } else {
            yclip_ptr += 2;
        }

        top_y_fix += top_dy_per_dx;
    }
    
    flip(0);

    return; 
}

void draw_floor_update_clip(s16 x1, s16 x1_ybot, s16 x2, s16 x2_ybot, u16 window_min, u16 window_max, u8 floor_color) {
        // 4 subpixel bits here
    s16 bot_dy_fix = x2_ybot - x1_ybot;

    s16 dx = x2-x1;

    fix32 bot_dy_per_dx = (bot_dy_fix<<12) / dx;

    fix32 bot_y_fix = x1_ybot<<12; 
    s16 bot_y_int;

    s16 beginx = max(x1, window_min);

    s16 skip_x = beginx - x1;
    if(skip_x > 0) {
        bot_y_fix += (skip_x * bot_dy_per_dx);
    }


    s16 endx = min(window_max, x2);

    s16 x = beginx;
    u8* yclip_ptr = &(yclip[x<<1]);

    for(;x <= endx; x++) {
        bot_y_int = bot_y_fix >> 16;
        u8 min_drawable_y = *yclip_ptr;
        u8 max_drawable_y = *(yclip_ptr+1);
        u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);

        if(max_drawable_y > bot_draw_y) {
            draw_native_vertical_line_unrolled(x, bot_draw_y, max_drawable_y, floor_color);
            yclip_ptr++;
            *yclip_ptr++ = bot_draw_y;
        } else {
            yclip_ptr += 2;
        }

        bot_y_fix += bot_dy_per_dx;
    }
    
    flip(0);

    return; 
}
