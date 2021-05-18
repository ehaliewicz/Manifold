#include <genesis.h>
#include <kdebug.h>
#include "colors.h"
#include "clip_buf.h"
#include "draw.h"
#include "game.h"
#include "level.h"
#include "math3d.h"
#include "portal.h"
#include "texture.h"
#include "utils.h"




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
  return base_offset + y_offset + x_col_offset + x_cols_offset; // + 16;
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

//u16* column_table;
//u16* column_offset_table;
u8** buf_0_column_offset_table;
u8** buf_1_column_offset_table;

void init_column_offset_table() {
  // offset from last column
  //column_table[0] = 0;
  //u8* last_ptr = getDMAWritePointer(0, 0);

  for(int x = 0; x < SCREEN_WIDTH/2; x++) {  
    u16 cur_off = getDMAWriteOffset(x<<1, 0);
    buf_0_column_offset_table[x] = bmp_buffer_0 + cur_off;
    buf_1_column_offset_table[x] = bmp_buffer_1 + cur_off;
    //column_table[x] = cur_off; 
  }

  //column_offset_table[0] = 0;
  //u16 last_offset = 0;
  //for(int x = 1; x < SCREEN_WIDTH/2; x++) {
      //u16 last_col_offset = getDMAWriteOffset((x-1)<<1, 0);
  //    u16 cur_col_offset = getDMAWriteOffset(x, 0);
  //    column_offset_table[x] = cur_col_offset - last_offset; //column_table[x<<1];
  //    last_offset = cur_col_offset;
  //}
}


void init_2d_buffers() {
    yclip = MEM_alloc(128); //256); //SCREEN_WIDTH*2*sizeof(u8));
    //column_table = MEM_alloc(sizeof(u16) * SCREEN_WIDTH); //sizeof(16) * W);
    //double_column_table = MEM_alloc(sizeof(u16) * SCREEN_WIDTH/2);
    //column_offset_table = MEM_alloc(sizeof(u16) * SCREEN_WIDTH);
    buf_0_column_offset_table = MEM_alloc(sizeof(u8*) * SCREEN_WIDTH/2);
    buf_1_column_offset_table = MEM_alloc(sizeof(u8*) * SCREEN_WIDTH/2);
    init_column_offset_table();
}

void clear_2d_buffers() {
    for(int i = 0; i < 64; i++) { //SCREEN_WIDTH; i++) {
        yclip[i*2] = 0;
        yclip[i*2+1] = SCREEN_HEIGHT-1; // 8; idk why this is messed up 
    }
}

void copy_2d_buffer(u16 left, u16 right, clip_buf* dest) {
    int bytes_to_copy = ((right+1)-left)*2;
    //for(int x = left; x <= right; x++) {
    //    dest->clip_buffer[x*2] = yclip[x*2];
    //    dest->clip_buffer[(x*2)+1] = yclip[(x*2)+1];
    //}
    
    memcpy(&(dest->clip_buffer[left<<1]), &yclip[left<<1], bytes_to_copy); //bytes_to_copy); //128);
}

void release_2d_buffers() {
    MEM_free(yclip);
    //MEM_free(column_table);
    //MEM_free(column_offset_table);
    MEM_free(buf_1_column_offset_table);
    MEM_free(buf_1_column_offset_table);
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

    /*
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
    */
    

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


void draw_native_vertical_transparent_line_unrolled(s16 y0, s16 y1, u8 col, u8* col_ptr, u8 odd) {

    col_ptr = col_ptr + (y0<<1);
    u16* word_col_ptr = (u16*)col_ptr;

    u32 word_col = (col<<8) | col;


    u32 full_col = (col << 24) | (col << 16) | (col<<8) | col;
    
    u16 dy = (y1-y0);
    // odd skips the first pixel

    u32* lw_col_ptr;

    
   if(odd) {
        switch (dy & 0b111) {
            case 7:
                word_col_ptr++;
                lw_col_ptr = word_col_ptr;

                *lw_col_ptr++ = full_col;
                lw_col_ptr++;
                *lw_col_ptr++ = full_col;
                break;
            case 6:
                lw_col_ptr = word_col_ptr;

                *lw_col_ptr++ = full_col;
                lw_col_ptr++;
                *lw_col_ptr++ = full_col;
                break;
            case 5:
                *word_col_ptr++ = word_col;
                lw_col_ptr = word_col_ptr;
                lw_col_ptr++;
                *lw_col_ptr++ = full_col;
                break;
            case 4:
                lw_col_ptr = word_col_ptr;
                lw_col_ptr++;
                *lw_col_ptr++ = full_col;
                break;
            case 3:
                word_col_ptr++;
                lw_col_ptr = word_col_ptr;
                *lw_col_ptr++ = full_col;
                break;
            case 2: 
                lw_col_ptr = word_col_ptr;
                *lw_col_ptr++ = full_col;
                break;
            case 1:
                *word_col_ptr++ = word_col;
                lw_col_ptr = word_col_ptr;
                break;
            case 0:
                lw_col_ptr = word_col_ptr;
                break;
        }
    } else { 
        switch (dy & 0b111) {
            case 7:
                *word_col_ptr++ = word_col;
                lw_col_ptr = (u32*)word_col_ptr;
                lw_col_ptr++;
                *lw_col_ptr++ = full_col;
                lw_col_ptr++;
                break;
            case 6:
                lw_col_ptr = word_col_ptr;
                lw_col_ptr++;
                *lw_col_ptr++ = full_col;
                lw_col_ptr++;
                break;
            case 5:
                word_col_ptr++;
                lw_col_ptr = word_col_ptr;
                *lw_col_ptr++ = full_col;
                lw_col_ptr++;
                break;
            case 4:
                lw_col_ptr = word_col_ptr;
                *lw_col_ptr++ = full_col;
                lw_col_ptr++;
                break;
            case 3:
                *word_col_ptr++ = word_col;
                lw_col_ptr = word_col_ptr;
                lw_col_ptr++;
                break;
            case 2: 
                lw_col_ptr = word_col_ptr;
                lw_col_ptr++;
                break;
            case 1:
                word_col_ptr++;
                lw_col_ptr = word_col_ptr;
                break;
            case 0:
                lw_col_ptr = word_col_ptr;
                break;
        }
    }
    

    dy >>= 3;


    if(odd) {
        while(dy--) {
            __asm volatile(
                "addq #4, %0\t\n\
                move.l %1, (%0)+\t\n\
                addq #4, %0\t\n\
                move.l %1, (%0)+"
                : "+a" (lw_col_ptr)
                : "d" (full_col)
                );
            
        }
    } else {
        while(dy--) {
            
            __asm volatile(
                "move.l %1, (%0)+\t\n\
                addq #4, %0\t\n\
                move.l %1, (%0)+\t\n\
                addq #4, %0"
                : "+a" (lw_col_ptr)
                : "d" (full_col)
                );
            

        }
    }
    
    return;
}


void draw_native_vertical_line_unrolled(s16 y0, s16 y1, u8 col,  u8* col_ptr) {

    col_ptr = col_ptr + (y0<<1);
    u16* word_col_ptr = (u16*)col_ptr;

    u16 word_col = (col<<8) | col;    

    u32 full_col = (word_col << 16) | word_col; //(col << 24) | (col << 16) | (col<<8) | col;

    u16 dy = (y1-y0);
    if(dy & 0b1) {
        *word_col_ptr++ = word_col;
        dy--;
    }
    u32* lw_col_ptr = (u32*)word_col_ptr;


    dy>>=1;
    if(dy & 0b1) {
        *lw_col_ptr++ = full_col;
    }

    dy>>=1;
    if(dy & 0b1) {
        *lw_col_ptr++ = full_col;
        *lw_col_ptr++ = full_col;
    }

    dy>>=1;
    //u8* char_ptr = (u8*)lw_col_ptr;
    //char_ptr++;
    //lw_col_ptr = (u32*)char_ptr;
    while(dy--) {
        __asm volatile(
            "move.l %1, (%0)+\t\n\
            move.l %1, (%0)+\t\n\
            move.l %1, (%0)+\t\n\
            move.l %1, (%0)+"
            : "+a" (lw_col_ptr)
            : "d" (full_col)
            );

        //*lw_col_ptr++ = full_col;
        //*lw_col_ptr++ = full_col;
        //*lw_col_ptr++ = full_col;
        //*lw_col_ptr++ = full_col;
    }

    return;
}

#define FLAT_COLOR

void flip() {
    
    /*if(JOY_readJoypad(JOY_1) & BUTTON_A) {
        request_flip();
        waitMs(500);
        //BMP_clear();
    }*/
}


void draw_transparent_wall(s16 x1, s16 x1_ytop, s16 x1_ybot,
                            s16 x2, s16 x2_ytop, s16 x2_ybot,
                            u16 window_min, u16 window_max,
                            clip_buf* clipping_buffer,
                            u8 wall_col) {


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
    u8* yclip_ptr = &(clipping_buffer->clip_buffer[x<<1]);
    //u8* yclip_ptr = &(clipping_buffer->clip_buffer[0]); 
    //yclip_ptr =&(yclip[x<<1]);

    u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    //u8 tex_col = 0;
    for(;x < endx; x++) {
        //top_y_int = top_y_fix >> 16;
        //bot_y_int = bot_y_fix >> 16;
        u8 min_drawable_y = *yclip_ptr++;
        u8 max_drawable_y = *yclip_ptr++;
        u8 top_draw_y = min_drawable_y; 
        //u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);
        u8 bot_draw_y = max_drawable_y; 
        //u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);

        u8* col_ptr = *offset_ptr++;
        //if(min_drawable_y < top_draw_y) {
        //    draw_native_vertical_line_unrolled(min_drawable_y, top_draw_y, ceil_col, col_ptr);
        //}
        if(top_draw_y < bot_draw_y) {
            //draw_texture_vertical_line(top_y_int, top_draw_y, bot_y_int, bot_draw_y, col_ptr, tex_col&(32-1));
            //tex_col++;
            draw_native_vertical_transparent_line_unrolled(top_draw_y, bot_draw_y, wall_col, col_ptr, x&1);
        }
        //if(bot_draw_y < max_drawable_y) {
        //    draw_native_vertical_line_unrolled(bot_draw_y, max_drawable_y, floor_col, col_ptr);
        //}

        //top_y_fix += top_dy_per_dx;
        //bot_y_fix += bot_dy_per_dx;
    }
    
    flip(0);

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
    u8* yclip_ptr = &(yclip[x<<1]);

    u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    //u8 tex_col = 0;
    for(;x < endx; x++) {
        top_y_int = top_y_fix >> 16;
        bot_y_int = bot_y_fix >> 16;
        u8 min_drawable_y = *yclip_ptr++;
        u8 max_drawable_y = *yclip_ptr++;
        u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);
        u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);

        u8* col_ptr = *offset_ptr++;
        if(min_drawable_y < top_draw_y) {
            draw_native_vertical_line_unrolled(min_drawable_y, top_draw_y, ceil_col, col_ptr);
        }
        if(top_draw_y < bot_draw_y) {
            //draw_texture_vertical_line(top_y_int, top_draw_y, bot_y_int, bot_draw_y, col_ptr, tex_col&(32-1));
            //tex_col++;
            draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, wall_col, col_ptr);
        }
        if(bot_draw_y < max_drawable_y) {
            draw_native_vertical_line_unrolled(bot_draw_y, max_drawable_y, floor_col, col_ptr);
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

    u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    for(;x < endx; x++) {
        top_y_int = top_y_fix >> 16;
        ntop_y_int = ntop_y_fix >> 16;
        u8 min_drawable_y = *yclip_ptr;
        u8 max_drawable_y = *(yclip_ptr+1);
        u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);
        u8 bot_draw_y = clamp(ntop_y_int, min_drawable_y, max_drawable_y);


        u8* col_ptr = *offset_ptr++;
        if(top_draw_y < bot_draw_y) {
            draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, upper_color, col_ptr);
            *yclip_ptr++ = bot_draw_y;
            yclip_ptr++;
        } else {
            yclip_ptr += 2;
        }
        if(min_drawable_y < top_draw_y) {
            draw_native_vertical_line_unrolled(min_drawable_y, top_draw_y, ceil_color, col_ptr);
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

    u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    for(;x < endx; x++) {
        bot_y_int = bot_y_fix >> 16;
        nbot_y_int = nbot_y_fix >> 16;
        u8 min_drawable_y = *yclip_ptr;
        u8 max_drawable_y = *(yclip_ptr+1);
        u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);
        u8 top_draw_y = clamp(nbot_y_int, min_drawable_y, max_drawable_y);

        u8* col_ptr = *offset_ptr++;
        if(top_draw_y < bot_draw_y) {
            draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, lower_color, col_ptr);
            yclip_ptr++;
            *yclip_ptr++ = top_draw_y;
        } else {
            yclip_ptr += 2;
        }
        if(max_drawable_y > bot_draw_y) {
            draw_native_vertical_line_unrolled(bot_draw_y, max_drawable_y, floor_color, col_ptr);
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

    u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    for(;x < endx; x++) { 
        top_y_int = top_y_fix >> 16;
        u8 min_drawable_y = *yclip_ptr;
        u8 max_drawable_y = *(yclip_ptr+1);
        u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);

        u8* col_ptr = *offset_ptr++;

        if(min_drawable_y < top_draw_y) {
            draw_native_vertical_line_unrolled(min_drawable_y, top_draw_y, ceil_color, col_ptr);
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

    u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    for(;x < endx; x++) {
        bot_y_int = bot_y_fix >> 16;
        u8 min_drawable_y = *yclip_ptr;
        u8 max_drawable_y = *(yclip_ptr+1);
        u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);

        u8* col_ptr = *offset_ptr++;
        if(max_drawable_y > bot_draw_y) {
            draw_native_vertical_line_unrolled(bot_draw_y, max_drawable_y, floor_color, (u8*)((u32)col_ptr&~0b1));
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


void draw_monochrome_sprite_no_vclip(uint8_t* sprite, uint8_t start_x, uint8_t start_y,  s16 z, u16 window_left, u16 window_right) {

    u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[start_x]) : (&buf_1_column_offset_table[start_x]);
    

    uint8_t num_columns = *sprite++;


    s16 start_x_fix = start_x<<4;
    s16 screen_pixels_fix = 8<<4/z;  //8.4
    s16 end_x_fix = start_x_fix+screen_pixels_fix;

    s16 end_x_int = end_x_fix>>4;

    s16 start_y_fix = start_y<<4;
    s16 end_y_fix = start_y_fix+screen_pixels_fix;
    s16 end_y_int = end_x_fix>>4;

    //for(int x = start_x; x < end_x_int; x++) {
    //    u8* col_ptr = *offset_ptr++;
    //    draw_native_vertical_line_unrolled(start_y, end_y_int, ((RED_IDX << 4) | RED_IDX), col_ptr);
    //}

    
    //return;

    s16 size_multiplier;
    if(z > 100) {
        size_multiplier = 1;
    } else if (z > 50) {
        size_multiplier = 2;
    } else {
        size_multiplier = 4;
    }

    uint8_t dx = start_x;
    uint8_t col = 0;
    while(dx < window_left) {
        uint8_t num_runs = *sprite++;
        uint8_t bytes_to_skip = num_runs * 3;
        sprite += bytes_to_skip;
        dx++;
        col++;
        offset_ptr++;
    }

    for(; col < num_columns; col++) {

        u8* col_sprite_ptr = sprite;
        uint8_t num_runs = *col_sprite_ptr++;
        u8* start_offset_ptr = offset_ptr;

        for(int i = 0; i < size_multiplier; i++) {
            if(dx >= window_right) {
                return;
            }
            uint8_t dy = start_y;
            u8* col_ptr = *offset_ptr++;
            for(uint8_t run = 0; run < num_runs; run++) {
                uint8_t skip = *col_sprite_ptr++;
                dy += skip;
                uint8_t num_pixels = (*col_sprite_ptr++)*size_multiplier;
                uint8_t col = *col_sprite_ptr++;
                draw_native_vertical_line_unrolled(dy, dy+num_pixels, col, col_ptr);
                dy += num_pixels;
            }
            dx++;
        }


        /*
        if(dx >= window_right) {
            return;
        }
        uint8_t dy = start_y;
        u8* col_ptr = *offset_ptr++

        for(uint8_t run = 0; run < num_runs; run++) {
            uint8_t skip = *sprite++;
            dy += skip;
            uint8_t num_pixels = (*sprite++)*size_multiplier;
            uint8_t col = *sprite++;
            draw_native_vertical_line_unrolled(dy, dy+num_pixels, col, col_ptr);
            dy += num_pixels;
        }
        //dx++;
        dx += size_multiplier;
        */

    }
    flip(0);
}
