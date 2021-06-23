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

// this is fast and efficient when each column is mostly empty space
void draw_rle_monochrome_col(uint8_t *rle_column, uint8_t *col_ptr, uint16_t y_top, uint16_t y_bot, uint16_t col, s16 y0, s16 y1) {

  // scale is 16.16 scale factor, basically, it is 1/z in fixed point

  if(y1 <= y0) { return; }
  if(y_bot <= y_top) { return; }




  u8 num_runs = *rle_column++;
  if(num_runs == 0) { return; }

  u8 byte_col = (col<<4) | col;
  u16 word_col = (byte_col<<8) | byte_col;
  s16 pix = y1-y0;

  s32 cur_y_fix = y0<<5;
  s16 pixels_per_texel = (pix<<5) >> 6;// / 64;

  s32 y_top_fix = (y_top << 5);
  s32 y_bot_fix = (y_bot << 5);

  // load the first span out of the loop
  //u8 texels_skipped = *rle_column++;
  //u8 texels_length = *rle_column++;
  //if(texels_skipped) {
  //
  //}


  for(int i = 0; i < num_runs; i++) {
    s16 texels_skipped = *rle_column++;
    s16 texels_length = *rle_column++;

    if(texels_skipped) {
        s32 pixels_skipped_fix = pixels_per_texel;
           __asm volatile(
            "muls.w %1, %0"
            :  "+d" (pixels_skipped_fix), "+d" (texels_skipped) // output
            : // input
        );
    
        cur_y_fix += pixels_skipped_fix;
    }
    if(cur_y_fix >= y_bot_fix) { return; }

    s32 num_pixels_fix = pixels_per_texel;
        __asm volatile(
        "muls.w %1, %0"
        :  "+d" (num_pixels_fix), "+d" (texels_length) // output
        : // input
    );
    
    //s32 num_pixels_fix = texels_length * pixels_per_texel;
    s32 end_y_fix = (cur_y_fix + num_pixels_fix);

    if(end_y_fix >= y_top_fix) {
        
        s16 span_top_y = cur_y_fix>>5;
        span_top_y = max(y_top, min(span_top_y, y_bot));
        s16 span_bot_y = end_y_fix>>5;

        span_bot_y = max(y_top, min(span_bot_y, y_bot));
        s16 dy = span_bot_y - span_top_y;
        u16* col_span_ptr = &col_ptr[span_top_y<<1];
        for(int j = 0; j < dy; j++) {
            *col_span_ptr++ = word_col;
        }
        //draw_native_vertical_line_unrolled(span_top_y, span_bot_y, col, col_ptr);
    }



    cur_y_fix = end_y_fix;
    if(cur_y_fix >= y_bot_fix) { return; }
    


  }
} 

#define FLAT_COLOR

void flip() {
    
    /*if(JOY_readJoypad(JOY_1) & BUTTON_A) {
        request_flip();
        waitMs(500);
        //BMP_clear();
    }*/
}

/*
uint8_t rle_texture[5] = {
    2, // 2 runs
    23, // skip 23 texels
    2, // draw 2 pixels
    27, // skip 27 more texels
    1, // draw 1 more texel
};
*/


uint8_t rle_glass[102] = {
    0, // num runs in column 0
    1, // num runs in column 1
    44, // skip 44 texels
    1, // draw 1 texels
    1, // num runs in column 2
    41, // skip 41 texels
    3, // draw 3 texels
    2, // num runs in column 3
    22, // skip 22 texels
    2, // draw 2 texels
    14, // skip 14 texels
    3, // draw 3 texels
    2, // num runs in column 4
    20, // skip 20 texels
    3, // draw 3 texels
    13, // skip 13 texels
    3, // draw 3 texels
    2, // num runs in column 5
    18, // skip 18 texels
    3, // draw 3 texels
    14, // skip 14 texels
    1, // draw 1 texels
    1, // num runs in column 6
    16, // skip 16 texels
    3, // draw 3 texels
    1, // num runs in column 7
    15, // skip 15 texels
    2, // draw 2 texels
    1, // num runs in column 8
    13, // skip 13 texels
    3, // draw 3 texels
    1, // num runs in column 9
    11, // skip 11 texels
    3, // draw 3 texels
    1, // num runs in column 10
    9, // skip 9 texels
    3, // draw 3 texels
    2, // num runs in column 11
    7, // skip 7 texels
    3, // draw 3 texels
    40, // skip 40 texels
    1, // draw 1 texels
    2, // num runs in column 12
    7, // skip 7 texels
    1, // draw 1 texels
    40, // skip 40 texels
    2, // draw 2 texels
    1, // num runs in column 13
    47, // skip 47 texels
    3, // draw 3 texels
    1, // num runs in column 14
    45, // skip 45 texels
    3, // draw 3 texels
    1, // num runs in column 15
    43, // skip 43 texels
    3, // draw 3 texels
    1, // num runs in column 16
    41, // skip 41 texels
    3, // draw 3 texels
    1, // num runs in column 17
    39, // skip 39 texels
    3, // draw 3 texels
    1, // num runs in column 18
    37, // skip 37 texels
    3, // draw 3 texels
    1, // num runs in column 19
    35, // skip 35 texels
    3, // draw 3 texels
    1, // num runs in column 20
    34, // skip 34 texels
    1, // draw 1 texels
    1, // num runs in column 21
    18, // skip 18 texels
    1, // draw 1 texels
    1, // num runs in column 22
    16, // skip 16 texels
    2, // draw 2 texels
    1, // num runs in column 23
    14, // skip 14 texels
    3, // draw 3 texels
    2, // num runs in column 24
    12, // skip 12 texels
    3, // draw 3 texels
    23, // skip 23 texels
    2, // draw 2 texels
    2, // num runs in column 25
    10, // skip 10 texels
    3, // draw 3 texels
    23, // skip 23 texels
    2, // draw 2 texels
    2, // num runs in column 26
    9, // skip 9 texels
    2, // draw 2 texels
    23, // skip 23 texels
    2, // draw 2 texels
    1, // num runs in column 27
    34, // skip 34 texels
    1, // draw 1 texels
    0, // num runs in column 28
    0, // num runs in column 29
    0, // num runs in column 30
    0, // num runs in column 31
};
uint16_t rle_glass_column_indexes[32] = {
    0,
    1,
    4,
    7,
    12,
    17,
    22,
    25,
    28,
    31,
    34,
    37,
    42,
    47,
    50,
    53,
    56,
    59,
    62,
    65,
    68,
    71,
    74,
    77,
    80,
    85,
    90,
    95,
    98,
    99,
    100,
    101,
};




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

    s32 tex_per_pix_fix = (32<<16)/dx;
    s32 tex_col_fix = 0;

    if(skip_x > 0) {
        top_y_fix += (skip_x * top_dy_per_dx);
        bot_y_fix += (skip_x * bot_dy_per_dx);
        tex_col_fix +=  (skip_x * tex_per_pix_fix);
    }


    s16 endx = min(window_max, x2);

    s16 x = beginx;
    u8* yclip_ptr = &(clipping_buffer->clip_buffer[x<<1]);

    u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    
    u8 tex_col_int;

    for(;x < endx; x++) {
        u8 min_drawable_y = *yclip_ptr++;
        u8 max_drawable_y = *yclip_ptr++;
        u8 top_draw_y = min_drawable_y; 
        u8 bot_draw_y = max_drawable_y; 

        u8* col_ptr = *offset_ptr++;
        if(top_draw_y < bot_draw_y) {
            top_y_int = top_y_fix >> 16;
            bot_y_int = bot_y_fix >> 16;
            tex_col_int = tex_col_fix>>16;


            //draw_texture_vertical_line(top_y_int, top_draw_y, bot_y_int, bot_draw_y, col_ptr, tex_col&(32-1));
            
            uint16_t rle_glass_index = rle_glass_column_indexes[tex_col_int];
            tex_col_fix += tex_per_pix_fix;

            draw_rle_monochrome_col(&rle_glass[rle_glass_index], col_ptr, min_drawable_y, max_drawable_y, (LIGHT_STEEL_IDX<<4)|LIGHT_STEEL_IDX, top_y_int, bot_y_int);
            //draw_rle_monochrome_col(rle_texture, col_ptr, min_drawable_y, max_drawable_y, (LIGHT_STEEL_IDX<<4)|LIGHT_STEEL_IDX, top_y_int, bot_y_int);
            
            //draw_native_vertical_transparent_line_unrolled(top_draw_y, bot_draw_y, wall_col, col_ptr, x&1);

        }
        //if(bot_draw_y < max_drawable_y) {
        //    draw_native_vertical_line_unrolled(bot_draw_y, max_drawable_y, floor_col, col_ptr);
        //}

        top_y_fix += top_dy_per_dx;
        bot_y_fix += bot_dy_per_dx;
    }
    
    flip(0);

    return; 
}


void draw_masked(s16 x1, s16 x1_ytop, s16 x1_ybot,
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

    s32 tex_per_pix_fix = (32<<16)/dx;
    s32 tex_col_fix = 0;

    if(skip_x > 0) {
        top_y_fix += (skip_x * top_dy_per_dx);
        bot_y_fix += (skip_x * bot_dy_per_dx);
        tex_col_fix +=  (skip_x * tex_per_pix_fix);
    }


    s16 endx = min(window_max, x2);

    s16 x = beginx;
    u8* yclip_ptr = &(clipping_buffer->clip_buffer[x<<1]);

    u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    
    u8 tex_col_int;

    for(;x < endx; x++) {
        u8 min_drawable_y = *yclip_ptr++;
        u8 max_drawable_y = *yclip_ptr++;
        u8 top_draw_y = min_drawable_y; 
        u8 bot_draw_y = max_drawable_y; 

        u8* col_ptr = *offset_ptr++;
        if(top_draw_y < bot_draw_y) {
            top_y_int = top_y_fix >> 16;
            bot_y_int = bot_y_fix >> 16;
            u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);
            u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);

            if(top_draw_y < bot_draw_y) {
                draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, wall_col, col_ptr);
            }


        }

        top_y_fix += top_dy_per_dx;
        bot_y_fix += bot_dy_per_dx;
    }
    
    flip(0);

    return; 
}


void draw_forcefield(s16 x1, s16 x2,
                     u16 window_min, u16 window_max,
                     clip_buf* clipping_buffer,
                     u8 wall_col) {


    // 4 subpixel bits here
    s16 beginx = max(x1, window_min);
    s16 endx = min(window_max, x2);

    s16 x = beginx;

    u8* yclip_ptr = &(clipping_buffer->clip_buffer[x<<1]);

    u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    
    u8 tex_col_int;

    for(;x < endx; x++) {
        u8 min_drawable_y = *yclip_ptr++;
        u8 max_drawable_y = *yclip_ptr++;
        u8 top_draw_y = min_drawable_y; 
        u8 bot_draw_y = max_drawable_y; 

        u8* col_ptr = *offset_ptr++;
        if(top_draw_y < bot_draw_y) {
            draw_native_vertical_transparent_line_unrolled(top_draw_y, bot_draw_y, wall_col, col_ptr, x&1);
            
        }
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

