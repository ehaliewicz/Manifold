#include <genesis.h>
#include <kdebug.h>
#include "my_bmp.h"
#include "clip_buf.h"
#include "colors.h"
#include "div_lut.h"
#include "draw.h"
#include "game.h"
#include "level.h"
#include "math3d.h"
#include "obj_sprite.h"
#include "portal.h"
#include "texture.h"
#include "textures.h"
#include "utils.h"


static u8* yclip;


//static u8** buf_0_column_offset_table;
//static u8** buf_1_column_offset_table;

// hardcoded offsets for left/right column split framebuffer
// hardcoded to 144 pixel height
static const u16 buf_column_offset_table[RENDER_WIDTH] = {
    0,9216,288,9504,576,9792,864,10080,
    1152,10368,1440,10656,1728,10944,2016,11232,
    2304,11520,2592,11808,2880,12096,3168,12384,
    3456,12672,3744,12960,4032,13248,4320,13536,
    4608,13824,4896,14112,5184,14400,5472,14688,
    5760,14976,6048,15264,6336,15552,6624,15840,
    6912,16128,7200,16416,7488,16704,7776,16992,
    8064,17280,8352,17568,8640,17856,8928,18144
};

//static u16* buf_1_column_offset_table;

void init_column_offset_table() {
  // offset from last column


  //for(int x = 0; x < SCREEN_WIDTH/2; x++) {  
  //  u16 cur_off = bmp_get_dma_write_offset(x<<1, 0);
  //  buf_column_offset_table[x] = cur_off;
  //  KLog_U1("", cur_off);
  //  //buf_1_column_offset_table[x] = cur_off;
  //}
}

#define GET_COLUMN_PTR(offset_ptr, base_bmp)  &base_bmp[*offset_ptr++]

void init_2d_buffers() {
    // 128 bytes
    yclip = malloc(RENDER_WIDTH*2, "yclip buffer");
}

void release_2d_buffers() {
    free(yclip, "yclip buffer");
    //free(buf_column_offset_table);
    //free(buf_1_column_offset_table);
}


void clear_2d_buffers() {
    u8* yclip_ptr = yclip;
    //u8* drawn_buf_ptr = drawn_buf;

    // 0:8 screen_height:8 0:8 screen_height:8
    u32 u32val = (SCREEN_HEIGHT << 16) | SCREEN_HEIGHT;

    // 128 bytes need to be moved
    

    
    // these are slightly faster than the memset calls below.
    __asm volatile(
        "move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+"
        : "+a" (yclip_ptr)
        : "d" (u32val)
    ); 


    /*
    __asm volatile(
        "move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        move.l %1, (%0)+\t\n\
        "
        : "+a" (drawn_buf_ptr)
        : "d" (u32zero)
    );
    */
    

    //memsetU32(yclip_ptr_u32, u32val, RENDER_WIDTH>>1);
    //memsetU32(drawn_buf_ptr, u32zero, RENDER_WIDTH>>2);

    /*
    int cnt = RENDER_WIDTH;

    while(cnt--) {
        *yclip_ptr++ = 0;
        *yclip_ptr++ = SCREEN_HEIGHT;
        *drawn_buf_ptr++ = 0;
    }
    */
}

void set_column_drawn(u8* after_column_yclip_ptr) {
    __asm volatile(
        "move.w #0, -2(%0)"
        :
        : "a" (after_column_yclip_ptr)
    ); 
}

u8 is_column_drawn(u8 min_drawable_y, u8 max_drawable_y) {
    return min_drawable_y >= max_drawable_y;
}


void copy_2d_buffer(u16 left, u16 right, clip_buf* dest) {
    int bytes_to_copy = ((right+1)-left)*2;
    memcpy(&(dest->y_clip_buffer[left<<1]), &yclip[left<<1], bytes_to_copy); //bytes_to_copy); //128);
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
                lw_col_ptr = (u32*)word_col_ptr;

                __asm volatile(
                    "move.l %1, (%0)+"
                    : "+a" (lw_col_ptr)
                    : "d" (full_col)
                ); // *lw_col_ptr++ = full_col;
                lw_col_ptr++;
                __asm volatile(
                    "move.l %1, (%0)+"
                    : "+a" (lw_col_ptr)
                    : "d" (full_col)
                ); // *lw_col_ptr++ = full_col;
                break;
            case 6:
                lw_col_ptr = (u32*)word_col_ptr;

                __asm volatile(
                    "move.l %1, (%0)+"
                    : "+a" (lw_col_ptr)
                    : "d" (full_col)
                ); // *lw_col_ptr++ = full_col;
                lw_col_ptr++;
                __asm volatile(
                    "move.l %1, (%0)+"
                    : "+a" (lw_col_ptr)
                    : "d" (full_col)
                ); // *lw_col_ptr++ = full_col;
                break;
            case 5:
                __asm volatile(
                    "move.w %1, (%0)+"
                    : "+a" (word_col_ptr)
                    : "d" (word_col)
                );  // *word_col_ptr++ = word_col;
                lw_col_ptr = (u32*)word_col_ptr;
                lw_col_ptr++;
                __asm volatile(
                    "move.l %1, (%0)+"
                    : "+a" (lw_col_ptr)
                    : "d" (full_col)
                ); // *lw_col_ptr++ = full_col;
                break;
            case 4:
                lw_col_ptr = (u32*)word_col_ptr;
                lw_col_ptr++;
                __asm volatile(
                    "move.l %1, (%0)+"
                    : "+a" (lw_col_ptr)
                    : "d" (full_col)
                ); // *lw_col_ptr++ = full_col;
                break;
            case 3:
                word_col_ptr++;
                lw_col_ptr = (u32*)word_col_ptr;
                __asm volatile(
                    "move.l %1, (%0)+"
                    : "+a" (lw_col_ptr)
                    : "d" (full_col)
                ); // *lw_col_ptr++ = full_col;
                break;
            case 2: 
                lw_col_ptr = (u32*)word_col_ptr;
                __asm volatile(
                    "move.l %1, (%0)+"
                    : "+a" (lw_col_ptr)
                    : "d" (full_col)
                ); // *lw_col_ptr++ = full_col;
                break;
            case 1:
                __asm volatile(
                    "move.w %1, (%0)+"
                    : "+a" (word_col_ptr)
                    : "d" (word_col)
                );  // *word_col_ptr++ = word_col;
                lw_col_ptr = (u32*)word_col_ptr;
                break;
            case 0:
                lw_col_ptr = (u32*)word_col_ptr;
                break;
        }
    } else { 
        switch (dy & 0b111) {
            case 7:
                __asm volatile(
                    "move.w %1, (%0)+"
                    : "+a" (word_col_ptr)
                    : "d" (word_col)
                );  // *word_col_ptr++ = word_col;
                lw_col_ptr = (u32*)word_col_ptr;
                lw_col_ptr++;
                __asm volatile(
                    "move.l %1, (%0)+"
                    : "+a" (lw_col_ptr)
                    : "d" (full_col)
                ); // *lw_col_ptr++ = full_col;
                lw_col_ptr++;
                break;
            case 6:
                lw_col_ptr = (u32*)word_col_ptr;
                lw_col_ptr++;
                __asm volatile(
                    "move.l %1, (%0)+"
                    : "+a" (lw_col_ptr)
                    : "d" (full_col)
                ); //*lw_col_ptr++ = full_col;
                lw_col_ptr++;
                break;
            case 5:
                word_col_ptr++;
                lw_col_ptr = (u32*)word_col_ptr;
                __asm volatile(
                    "move.l %1, (%0)+"
                    : "+a" (lw_col_ptr)
                    : "d" (full_col)
                ); //*lw_col_ptr++ = full_col;
                lw_col_ptr++;
                break;
            case 4:
                lw_col_ptr = (u32*)word_col_ptr;
                __asm volatile(
                    "move.l %1, (%0)+"
                    : "+a" (lw_col_ptr)
                    : "d" (full_col)
                ); //*lw_col_ptr++ = full_col;
                lw_col_ptr++;
                break;
            case 3:
                __asm volatile(
                    "move.w %1, (%0)+"
                    : "+a" (word_col_ptr)
                    : "d" (word_col)
                );  // *word_col_ptr++ = word_col;
                lw_col_ptr = (u32*)word_col_ptr;
                lw_col_ptr++;
                break;
            case 2: 
                lw_col_ptr = (u32*)word_col_ptr;
                lw_col_ptr++;
                break;
            case 1:
                word_col_ptr++;
                lw_col_ptr = (u32*)word_col_ptr;
                break;
            case 0:
                lw_col_ptr = (u32*)word_col_ptr;
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




void draw_native_double_vertical_line_unrolled(s16 y0, s16 y1, s16 y2, u32 full_col1, u32 full_col2, u8* col_ptr) {
    col_ptr = col_ptr + (y0 << 1);
    u16 dy1 = (y1-y0);
    u16 dy2 = (y2-y1);

    u16 word_col = full_col1;

    if(dy1 & 0b1) {
        __asm volatile(
            "move.w %1, (%0)+"
            : "+a" (col_ptr)
            : "d" (word_col)
        );
        dy1--;
    }

    
    __asm volatile(
        "sub.w #158, %1\t\n\
         neg.w %1\t\n\
         jmp movel_draw_table_%=(%%pc, %1.W)\t\n\
movel_draw_table_%=:\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+"
      : "+a" (col_ptr)
      : "d" (dy1), "d" (full_col1)
    );

    
    word_col = full_col2;

    if(dy2 & 0b1) {
        __asm volatile(
            "move.w %1, (%0)+"
            : "+a" (col_ptr)
            : "d" (word_col)
        );
        dy2--;
    }

    __asm volatile(
        "sub.w #158, %1\t\n\
         neg.w %1\t\n\
         jmp movel_draw_table_%=(%%pc, %1.W)\t\n\
movel_draw_table_%=:\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+\t\n\
      move.l %2, (%0)+"
      : "+a" (col_ptr)
      : "d" (dy2), "d" (full_col2)
    );

}


void draw_native_vertical_line_unrolled(s16 y0, s16 y1, u32 full_col,  u8* col_ptr) {


    col_ptr = col_ptr + (y0<<1);

    u16 word_col = full_col;

    u16 dy = (y1-y0);
    //KLog_U1("dy: ", dy);
    if(dy & 0b1) {
        __asm volatile(
            "move.w %1, (%0)+"
            : "+a" (col_ptr)
            : "d" (word_col)
        );
        dy--;
    }

    __asm volatile(
        "sub.w #158, %1\t\n\
        neg.w %1\t\n\
        jmp movel_draw_table_%=(%%pc, %1.W)\t\n\
movel_draw_table_%=:\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+\t\n\
    move.l %2, (%0)+"
    : "+a" (col_ptr)
    : "d" (dy), "d" (full_col)
    );
    


}


#define FLAT_COLOR

int debug_draw_cleared = 0;

int last_pressed_a = 0;
void flip() {  
    return;
  //u8* dst_buf = (bmp_buffer_write == bmp_buffer_0) ? bmp_buffer_1 : bmp_buffer_0;
  //return;
  if(JOY_readJoypad(JOY_1) & BUTTON_A) {
    if(!last_pressed_a) {
        if(!debug_draw_cleared) { return; }
        bmp_vertical_clear();
        // just copy written buffer to next buffer
        //memcpy(bmp_buffer_read, bmp_buffer_write, 256*144/2);
        request_flip(); // draw buffer with one wall
        u8* tmp = bmp_buffer_write;
        bmp_buffer_write = bmp_buffer_read;
        bmp_buffer_read = tmp;
        last_pressed_a = 1;
    } else {
        memcpy(bmp_buffer_read, bmp_buffer_write, 256*144/2);
        request_flip(); // draw buffer with one wall
        u8* tmp = bmp_buffer_write;
        bmp_buffer_write = bmp_buffer_read;
        bmp_buffer_read = tmp;
    }
    waitMs(125);
  } else {
      last_pressed_a = 0;
  }
}

#define MAX_CACHE_INSTRUCTIONS 256
// 1536+2 bytes
u8 sprite_draw_cache[256*6+2]; // [(400*6)+2];//[3072+2]; // extra 2 bytes are for a RTS at the end // 0x4E75;
extern const u8* sprite_draw_cache_lookup[512];
extern const u16 sprite_draw_cache_size_lookup[512];

#define RTS_OPCODE 0x4E75 
#define SPRITE_MOVE_OPCODE 0x1368

void init_sprite_draw_cache() {
    // sprite_draw_cache is a table of 256 move.b X(a0), Y(a1) instructions
    // Y increments by 2 every instruction, so that it draws to the next byte lower in the framebuffer
    u16* ptr = (u16*)sprite_draw_cache;
    for(int i = 0; i < MAX_CACHE_INSTRUCTIONS; i++) {
        *ptr++ = SPRITE_MOVE_OPCODE; // move.b X(a0), X(a1)
        *ptr++ = 0;      // texel offset initialized to 0, will be patched in set_up_scale_routine
        *ptr++ = (i<<1); // set up offset to column pointer
    }
    *ptr++ = RTS_OPCODE; // terminate cache with return instruction
}

// the encoding of `move.b X(a0), Y(a1) instructions is 1368 XXXX YYYY (in hex)
// we only want to update XXXX, so we update at an offset of 2, and increment the pointer by 6, as each instruction is 6 bytes
#define COPY_CASE                                           \
    case MAX_CACHE_INSTRUCTIONS-__COUNTER__:                \
        __asm volatile(                                     \
            "move.w (%0)+, 2(%1)\t\n                        \
            addq.l #6, %1"                                  \
            : "+a" (scale_routine_coefficients), "+a" (ptr) \
        )

#define SIXTEEN_COPY_CASES \
    COPY_CASE; COPY_CASE; COPY_CASE; COPY_CASE; COPY_CASE; COPY_CASE; COPY_CASE; COPY_CASE; \
    COPY_CASE; COPY_CASE; COPY_CASE; COPY_CASE; COPY_CASE; COPY_CASE; COPY_CASE; COPY_CASE

#define TWO_HUNDRED_FIFTY_SIX_COPY_CASES \
    SIXTEEN_COPY_CASES; SIXTEEN_COPY_CASES; SIXTEEN_COPY_CASES; SIXTEEN_COPY_CASES; SIXTEEN_COPY_CASES; SIXTEEN_COPY_CASES; SIXTEEN_COPY_CASES; SIXTEEN_COPY_CASES; \
    SIXTEEN_COPY_CASES; SIXTEEN_COPY_CASES; SIXTEEN_COPY_CASES; SIXTEEN_COPY_CASES; SIXTEEN_COPY_CASES; SIXTEEN_COPY_CASES; SIXTEEN_COPY_CASES; SIXTEEN_COPY_CASES
    
void set_up_scale_routine(s16 unclipped_dy) {
    static s16 last_unclipped_dy = -1;
    if(last_unclipped_dy == unclipped_dy) {  return; }
    last_unclipped_dy = unclipped_dy;
    // this routine looks up the table of X coefficients/offsets for the scaled height of the sprite we are about to draw 
    // (unclipped_dy means the number of unclipped pixels this sprite is scaled to)
    // and copies them into sprite_draw_cache.  the loop is fully unrolled.
    const u16* scale_routine_coefficients = sprite_scale_coefficients_pointer_lut[unclipped_dy];
    u16 instructions_to_update = unclipped_dy;
    u16* ptr = (u16*)sprite_draw_cache;

    // if we previously called this function, we left an RTS at the end of the required number of pixels
    // here we reset it to a move opcode, in case our new copy reaches that point
    // neglecting this would cause pixel spans to be cut off early by returning early, in some cases
    static s32 prev_rts_slot = -1;
    if(prev_rts_slot != -1) {
        ptr[prev_rts_slot] = SPRITE_MOVE_OPCODE; 
    }
    // update YYYY offsets in the sprite_draw_cache table, not touching the opcodes
    switch(instructions_to_update) {
        TWO_HUNDRED_FIFTY_SIX_COPY_CASES;
    }
    // store the slot at which we are writing a RTS opcode to terminate the array, so that it can be patched later.
    prev_rts_slot = ptr-((u16*)sprite_draw_cache);
    *ptr++ = RTS_OPCODE;
}

void call_sprite_cache_for_run(u16* jump_tgt, const u8* cur_texel_ptr, u8* cur_column_ptr) {
    register const u8* a0 asm ("%a0") = cur_texel_ptr;   // this lets me make sure a0 and a1 are initialized correctly
    register const u8* a1 asm ("%a1") = cur_column_ptr;

    __asm volatile(
        "jsr (%0)\t\n\
        "
        : : "a" (jump_tgt), "a" (a0), "a" (a1) // instructions are 6 bytes long, so multiply patch index by 6
    );
}

void call_sprite_cache_for_wide_run(u16* jump_tgt, u8* cur_texel_ptr, u8* cur_column_ptr) {
    register const a0 asm ("%a0") = (u32)cur_texel_ptr;   // this lets me make sure a0 and a1 are initialized correctly
    register const a1 asm ("%a1") = (u32)cur_column_ptr;
            
    __asm volatile(
        "jsr (%0)\t\n\
        addq.l #1, %2\t\n\
        jsr (%0)\t\n\
        "
        : : "a" (jump_tgt), "a" (a0), "a" (a1) // instructions are 6 bytes long, so multiply patch index by 6
    );
}


void draw_col(s16 ytop, s16 min_drawable_y, s16 max_drawable_y, 
              s16 unclipped_dy, 
              column* col,
              u32 texels_per_scaled_vpixel, u32 y_per_texels_fix, 
              u8* col_ptr, u32* scaled_run_lengths_lut,
              const u8 draw_wide
               ) {
    //u8 top_draw_y = ytop; //clamp(ytop, min_drawable_y, max_drawable_y);
    //u8 bot_draw_y = ybot; //clamp(ybot, min_drawable_y, max_drawable_y);
    
    s32 cur_y_fix = ytop<<16;
    
    const u16* span_ptr = col->spans;

    const u8* base_texel_ptr = col->texels;
    const u8* cur_texel_ptr = base_texel_ptr;
    u8 num_spans = col->num_spans;


    for(int i = 0; i < num_spans; i++) {
        u16 skip = *span_ptr++;
        u16 len = *span_ptr++;

        u32 scaled_skip = scaled_run_lengths_lut[skip];
        u32 num_scaled_pixels_fix = scaled_run_lengths_lut[len];
        u16 num_scaled_pixels = num_scaled_pixels_fix>>16;
        
        cur_y_fix += scaled_skip; //y_per_texels_fix * skip;

        s16 span_draw_top = (cur_y_fix>>16);

        
        u8 clipped_bot = 0;
        s16 bot = (span_draw_top+num_scaled_pixels);


        s16 clip_bot = 0;
        s16 clip_top = 0;
        if(bot > max_drawable_y) {
            clip_bot = (bot-max_drawable_y);
            if(clip_bot >= num_scaled_pixels) {
                return;
            }
            num_scaled_pixels -= clip_bot;
            clipped_bot = 1;
        }
        //u16 jump_index = 0;
        volatile u16* jump_tgt = ((u16*)sprite_draw_cache);//+(jump_index*3);
        if(span_draw_top < min_drawable_y) {
            clip_top = min_drawable_y - span_draw_top;
            if(clip_top >= num_scaled_pixels) {
                goto continue_loop;
            }
            num_scaled_pixels -= clip_top;
            //jump_index += clip_top;
            jump_tgt += (clip_top*3);
        }

        u8* cur_col_ptr = &col_ptr[span_draw_top<<1];

        jump_tgt[num_scaled_pixels*3] = RTS_OPCODE;

        if(draw_wide) {
            call_sprite_cache_for_wide_run(jump_tgt, cur_texel_ptr, cur_col_ptr);
        } else {
            call_sprite_cache_for_run(jump_tgt, cur_texel_ptr, cur_col_ptr);
        }

        jump_tgt[num_scaled_pixels*3] = SPRITE_MOVE_OPCODE;

        //KLog_U2("restoring: ", prev_val, " to slot: ", num_pix_times_3);
        //jump_tgt[num_pix_times_3] = prev_val;


        if(clipped_bot) {
            return;
        }

    continue_loop:
        cur_y_fix += num_scaled_pixels_fix;
        cur_texel_ptr += len;       

    }

}



obj_type drawn_to_center_cols;
object_link sprite_on_center_col;

void reset_sprite_hit_info() {
    sprite_on_center_col = NULL_OBJ_LINK;
    drawn_to_center_cols = OBJECT;
}

/* for drawing moving objects */
void draw_rle_sprite(s16 x1, s16 x2, s16 ytop, s16 ybot,
                 u16 window_min, u16 window_max,
                 clip_buf* clipping_buffer,
                 const rle_sprite* obj, 
                 object_link obj_link, obj_type type) {

    s16 unclipped_dy = ybot-ytop;
    if(unclipped_dy > MAX_CACHE_INSTRUCTIONS) { return; }
    if(unclipped_dy > 512) { return; }

    if(unclipped_dy == 0) { return ;}
    s16 unclipped_dx = x2-x1;
    //s16 dcol = unclipped_dx>>2;
    if(unclipped_dx == 0) { return ;}


    s16 beginx = max(x1, window_min);

    s16 skip_x = beginx - x1;


    s16 endx = min(window_max, x2);
    //u32 u_per_x = (1<<16)/dx;
    u32 u_fix = 0;

    if(endx < beginx) { return; }

    set_up_scale_routine(unclipped_dy);
    // optimize with lookup table
    u32 y_per_texels_fix = (unclipped_dy<<16)/ 64; // 16.16 fixed point
    
    

    u32 texels_per_scaled_pixel = (64<<16)/unclipped_dy;
    // each column only covers half an h-pixel

    u32* scaled_run_lengths_lut = &scaled_sprite_run_lengths[unclipped_dy<<6];

    u16 num_cols_pre_scaled = (obj->num_columns-1)<<8>>1;
    u16 cols_per_scaled_hpixel_16 = divu_32_by_16(num_cols_pre_scaled, unclipped_dx); // 8.8
    u32 cols_per_scaled_hpixel = cols_per_scaled_hpixel_16 << 8; // 16.16


    u32 texels_per_scaled_vpixel = scaled_sprite_texel_per_pixel_lut[unclipped_dy];


    if(skip_x > 0) {
        // double this here because we have to skip 2 rle columns per screen-pixel
        u32 skip_16_8 = mulu_16_by_16(skip_x, cols_per_scaled_hpixel_16); // skip_x * (cols_per_scaled_hpixel_16);
        //skip_16_8 <= 1;
        u32 skip_16_16 = skip_16_8 << 8;

        u_fix += skip_16_16; // skip_x * (cols_per_scaled_hpixel<<1); //(skip_x * u_per_x);
        u_fix += skip_16_16;
    }


    s16 x = beginx;
    u8* yclip_ptr = &(clipping_buffer->y_clip_buffer[x<<1]);

    u16* offset_ptr = (&buf_column_offset_table[x]);


    u16 col_dx = (endx-beginx);
    u8 cur_x = beginx;

    u8 hit_center = 0;

 

    while(col_dx--) {
        u8 min_drawable_y = *yclip_ptr++;
        u8 max_drawable_y = *yclip_ptr++;

        u8 top_draw_y = max(ytop, min_drawable_y);
        u8 bot_draw_y = min(ybot, max_drawable_y);

        u8* col_ptr = GET_COLUMN_PTR(offset_ptr, bmp_buffer_write);

        // TODO: information about whether this column was filled in front of the object 
        // should be loaded from clip buffer
        u8 col_drawn = is_column_drawn(min_drawable_y, max_drawable_y);

        if(col_drawn) {
            u_fix += cols_per_scaled_hpixel;
            u_fix += cols_per_scaled_hpixel;
        } else {
            u_fix += cols_per_scaled_hpixel;
            u16 mid_u_int = u_fix>>16;
            u_fix += cols_per_scaled_hpixel;
            u16 end_u_int = u_fix>>16;
            
            
            if(ytop >= max_drawable_y || ybot <= min_drawable_y) {
                continue;
            }

            u8 within_low = 0;
            u8 within_high = 0;
            if(cur_x >= within_low && cur_x <= within_high) {
                hit_center = 1;
            }
            cur_x++;

            column* col = &obj->columns[mid_u_int];
            
            if(mid_u_int == end_u_int) {
                draw_col(
                    ytop,
                    min_drawable_y, max_drawable_y, 
                    unclipped_dy, col,
                    texels_per_scaled_vpixel, y_per_texels_fix,
                    col_ptr, scaled_run_lengths_lut, 1
                );
            } else {
                draw_col(
                    ytop,
                    min_drawable_y, max_drawable_y, 
                    unclipped_dy, col,
                    texels_per_scaled_vpixel, y_per_texels_fix,
                    col_ptr, scaled_run_lengths_lut, 0
                );
                col = &obj->columns[end_u_int];
                draw_col(
                    ytop,
                    min_drawable_y, max_drawable_y, 
                    unclipped_dy, col,
                    texels_per_scaled_vpixel, y_per_texels_fix,
                    col_ptr+1, scaled_run_lengths_lut, 0
                );
            }

        }
    }

    if(hit_center) {
        drawn_to_center_cols = type;
        sprite_on_center_col = obj_link;
    }
    flip();

    return; 
}

/* for drawing moving objects */
void draw_masked(s16 x1, s16 x2, s16 ytop, s16 ybot,
                 u16 window_min, u16 window_max,
                 clip_buf* clipping_buffer,
                 u8 wall_col) {

    // 4 subpixel bits here
    ybot>>=4;
    ytop>>=4;
    s16 unclipped_dy = ybot-ytop;
    if(unclipped_dy == 0) { return ;}
    s16 dx = x2-x1;
    if(dx == 0) { return ;}


    s16 beginx = max(x1, window_min);

    s16 skip_x = beginx - x1;


    s16 endx = min(window_max, x2);
    u32 u_per_x = (1<<16)/dx;
    u32 u_fix = 0;

    if(skip_x > 0) {
        u_fix += (skip_x * u_per_x);
    }


    s16 x = beginx;
    u8* yclip_ptr = &(clipping_buffer->y_clip_buffer[x<<1]);

    u16* offset_ptr = (&buf_column_offset_table[x]);

    //u16* tex = raw_key_mid;
    const u16* tex = raw_key_32_32_mid;
    u32 y_per_texels_fix = (unclipped_dy<<16)/ 64; // 16.16 fixed point
    
    //s32 dv_per_y_f16 = (64<<16)/unclipped_dy;


    for(;x < endx; x++) {    
        u8 min_drawable_y = *yclip_ptr++;
        u8 max_drawable_y = *yclip_ptr++;
        //if(min_drawable_y >= max_drawable_y) { continue; }

        u8 top_draw_y = min_drawable_y; 
        u8 bot_draw_y = max_drawable_y; 

        //u8* col_ptr = *offset_ptr++;
        u8* col_ptr = GET_COLUMN_PTR(offset_ptr, bmp_buffer_write);
        // TODO: information about whether this column was filled in front of the object 
        // should be loaded from clip buffer
        u8 col_drawn = is_column_drawn(min_drawable_y, max_drawable_y);
        if(!col_drawn) {
            u8 top_draw_y = clamp(ytop, min_drawable_y, max_drawable_y);
            u8 bot_draw_y = clamp(ybot, min_drawable_y, max_drawable_y);
            
            u32 clipped_y_pix_top, clipped_y_pix_bot;
            u8 skipped_texels;

            clipped_y_pix_top = clipped_y_pix_bot = 0;

            //clipped_y_pix_bot = 0;

            //u8 skipped_bot_texels = 33;
            //(y_per_texels_fix * skipped_bot_texels) >> 16;

            // get texture column 
            u32 tex_col = ((u_fix<<TEX_WIDTH_SHIFT)>>16); //TEX_WIDTH_SHIFT)>>16);
            u32 tex_idx = tex_col<<TEX_HEIGHT_SHIFT; //TEX_HEIGHT_SHIFT;
            const u16* tex_column = &tex[tex_idx+1];

            // this sprite only has 28 columns of non-transparent pixels :)
            
            if(tex_col > 27) {
                break;
            } else if (tex_col < 4) {
                skipped_texels = 8;
                clipped_y_pix_top = ((y_per_texels_fix << 3)>>16)+1;
            } else if (tex_col >= 24) {
                skipped_texels = 28;
                clipped_y_pix_top = ((y_per_texels_fix * skipped_texels)>>16)+1; 
            }

            //draw_bottom_clipped_texture_with_tex_chunk(
            //    chk, 
            //    ytop, top_draw_y, //+clipped_y_pix_top, 
            //    ybot, bot_draw_y, 
            //    col_ptr, tex_column);
            //draw_bottom_clipped_texture_vertical_line(top_y_int, top_draw_y+clipped_y_pix_top, bot_y_int, bot_draw_y, col_ptr, tex_column);
            draw_bottom_clipped_texture_vertical_line(ytop, top_draw_y+clipped_y_pix_top, ybot, bot_draw_y, col_ptr, tex_column);

        }
        u_fix += u_per_x;

    }
    
    flip();

    return; 
}

/* for drawing forcefields */
void draw_forcefield(s16 x1, s16 x2,
                     u16 window_min, u16 window_max,
                     clip_buf* clipping_buffer,
                     u8 wall_col) {


    // 4 subpixel bits here
    s16 beginx = max(x1, window_min);
    s16 endx = min(window_max, x2);

    s16 x = beginx;

    u8* yclip_ptr = &(clipping_buffer->y_clip_buffer[x<<1]);

    u16* offset_ptr = (&buf_column_offset_table[x]);

    for(;x < endx; x++) {
        u8 min_drawable_y = *yclip_ptr++;
        u8 max_drawable_y = *yclip_ptr++;
        //if(min_drawable_y >= max_drawable_y) { continue; }
        u8 top_draw_y = min_drawable_y; 
        u8 bot_draw_y = max_drawable_y; 

        //u8* col_ptr = *offset_ptr++;
        u8* col_ptr = GET_COLUMN_PTR(offset_ptr, bmp_buffer_write);
        if(top_draw_y < bot_draw_y) {
            draw_native_vertical_transparent_line_unrolled(top_draw_y, bot_draw_y, wall_col, col_ptr, x&1);
            
        }
    }

    
    flip();

    return; 
}

const s16 floor_light_positions[512*4] = {
-1, -1, -1, -1, 62, 62, 63, 66, 68, 68, 69, 71, 70, 70, 71, 73, 71, 71, 72, 74, 71, 72, 72, 74, 72, 72, 73, 75, 72, 72, 73, 75, 72, 72, 73, 75, 72, 72, 73, 75, 72, 73, 73, 75, 72, 73, 74, 76, 73, 73, 74, 76, 73, 73, 74, 77, 73, 73, 75, 77, 73, 74, 75, 78, 73, 74, 75, 78, 74, 74, 75, 79, 74, 74, 76, 79, 74, 74, 76, 80, 74, 75, 76, 80, 74, 75, 77, 81, 74, 75, 77, 81, 75, 75, 77, 82, 75, 75, 77, 82, 75, 76, 78, 83, 75, 76, 78, 83, 75, 76, 78, 83, 76, 76, 79, 84, 76, 76, 79, 84, 76, 77, 79, 85, 76, 77, 79, 85, 76, 77, 80, 86, 76, 77, 80, 86, 77, 77, 80, 87, 77, 78, 81, 87, 77, 78, 81, 88, 77, 78, 81, 88, 77, 78, 81, 89, 78, 79, 82, 89, 78, 79, 82, 90, 78, 79, 82, 90, 78, 79, 83, 91, 78, 79, 83, 91, 78, 80, 83, 92, 79, 80, 83, 92, 79, 80, 84, 93, 79, 80, 84, 93, 79, 80, 84, 94, 79, 81, 85, 94, 80, 81, 85, 95, 80, 81, 85, 95, 80, 81, 85, 95, 80, 81, 86, 96, 80, 82, 86, 96, 80, 82, 86, 97, 81, 82, 87, 97, 81, 82, 87, 98, 81, 82, 87, 98, 81, 83, 87, 99, 81, 83, 88, 99, 81, 83, 88, 100, 82, 83, 88, 100, 82, 83, 89, 101, 82, 84, 89, 101, 82, 84, 89, 102, 82, 84, 90, 102, 83, 84, 90, 103, 83, 84, 90, 103, 83, 85, 90, 104, 83, 85, 91, 104, 83, 85, 91, 105, 83, 85, 91, 105, 84, 86, 92, 106, 84, 86, 92, 106, 84, 86, 92, 107, 84, 86, 92, 107, 84, 86, 93, 107, 85, 87, 93, 108, 85, 87, 93, 108, 85, 87, 94, 109, 85, 87, 94, 109, 85, 87, 94, 110, 85, 88, 94, 110, 86, 88, 95, 111, 86, 88, 95, 111, 86, 88, 95, 112, 86, 88, 96, 112, 86, 89, 96, 113, 87, 89, 96, 113, 87, 89, 96, 114, 87, 89, 97, 114, 87, 89, 97, 115, 87, 90, 97, 115, 87, 90, 98, 116, 88, 90, 98, 116, 88, 90, 98, 117, 88, 90, 98, 117, 88, 91, 99, 118, 88, 91, 99, 118, 89, 91, 99, 119, 89, 91, 100, 119, 89, 91, 100, 119, 89, 92, 100, 120, 89, 92, 100, 120, 89, 92, 101, 121, 90, 92, 101, 121, 90, 93, 101, 122, 90, 93, 102, 122, 90, 93, 102, 123, 90, 93, 102, 123, 90, 93, 102, 124, 91, 94, 103, 124, 91, 94, 103, 125, 91, 94, 103, 125, 91, 94, 104, 126, 91, 94, 104, 126, 92, 95, 104, 127, 92, 95, 104, 127, 92, 95, 105, 128, 92, 95, 105, 128, 92, 95, 105, 129, 92, 96, 106, 129, 93, 96, 106, 130, 93, 96, 106, 130, 93, 96, 107, 131, 93, 96, 107, 131, 93, 97, 107, 131, 94, 97, 107, 132, 94, 97, 108, 132, 94, 97, 108, 133, 94, 97, 108, 133, 94, 98, 109, 134, 94, 98, 109, 134, 95, 98, 109, 135, 95, 98, 109, 135, 95, 98, 110, 136, 95, 99, 110, 136, 95, 99, 110, 137, 96, 99, 111, 137, 96, 99, 111, 138, 96, 100, 111, 138, 96, 100, 111, 139, 96, 100, 112, 139, 96, 100, 112, 140, 97, 100, 112, 140, 97, 101, 113, 141, 97, 101, 113, 141, 97, 101, 113, 142, 97, 101, 113, 142, 98, 101, 114, 143, 98, 102, 114, 143, 98, 102, 114, 143, 98, 102, 115, 144, 98, 102, 115, 144, 98, 102, 115, 144, 99, 103, 115, 144, 99, 103, 116, 144, 99, 103, 116, 144, 99, 103, 116, 144, 99, 103, 117, 144, 99, 104, 117, 144, 100, 104, 117, 144, 100, 104, 117, 144, 100, 104, 118, 144, 100, 104, 118, 144, 100, 105, 118, 144, 101, 105, 119, 144, 101, 105, 119, 144, 101, 105, 119, 144, 101, 105, 119, 144, 101, 106, 120, 144, 101, 106, 120, 144, 102, 106, 120, 144, 102, 106, 121, 144, 102, 107, 121, 144, 102, 107, 121, 144, 102, 107, 121, 144, 103, 107, 122, 144, 103, 107, 122, 144, 103, 108, 
122, 144, 103, 108, 123, 144, 103, 108, 123, 144, 103, 108, 123, 144, 104, 108, 123, 144, 104, 109, 124, 144, 104, 109, 124, 144, 104, 109, 124, 144, 104, 109, 125, 144, 105, 109, 125, 144, 105, 110, 125, 144, 105, 110, 126, 144, 105, 110, 126, 144, 105, 110, 126, 144, 105, 110, 126, 144, 106, 111, 127, 144, 106, 111, 127, 144, 106, 111, 127, 144, 106, 111, 128, 144, 106, 111, 128, 144, 107, 112, 128, 144, 107, 112, 128, 144, 107, 112, 129, 144, 107, 112, 129, 144, 107, 112, 129, 144, 107, 113, 130, 144, 108, 113, 130, 144, 108, 113, 130, 144, 108, 113, 130, 144, 108, 113, 131, 144, 108, 114, 131, 144, 108, 114, 131, 144, 109, 114, 132, 144, 109, 114, 132, 144, 109, 115, 132, 144, 109, 115, 132, 144, 109, 115, 133, 144, 110, 115, 133, 144, 110, 115, 133, 144, 110, 116, 134, 144, 110, 116, 134, 144, 110, 116, 134, 144, 110, 116, 134, 144, 111, 116, 135, 144, 111, 117, 135, 144, 111, 117, 135, 144, 111, 117, 136, 144, 111, 117, 136, 144, 112, 117, 136, 144, 112, 118, 136, 144, 112, 118, 137, 144, 112, 118, 137, 144, 112, 118, 137, 144, 112, 118, 138, 144, 113, 119, 138, 144, 113, 119, 138, 144, 113, 119, 138, 144, 113, 119, 139, 144, 113, 119, 139, 144, 114, 120, 139, 144, 114, 120, 140, 144, 114, 120, 140, 144, 114, 120, 140, 144, 114, 120, 140, 144, 114, 121, 141, 144, 115, 121, 141, 144, 115, 121, 141, 144, 115, 121, 142, 144, 115, 122, 142, 144, 115, 122, 142, 144, 116, 122, 143, 144, 116, 122, 143, 144, 
116, 122, 143, 144, 116, 123, 143, 144, 116, 123, 144, 144, 116, 123, 144, 144, 117, 123, 144, 144, 117, 123, 144, 144, 117, 124, 144, 144, 117, 124, 144, 144, 117, 124, 144, 144, 117, 124, 144, 144, 118, 124, 144, 144, 118, 125, 144, 144, 118, 125, 144, 144, 118, 125, 144, 144, 118, 125, 144, 144, 119, 125, 144, 144, 119, 126, 144, 144, 119, 126, 144, 144, 119, 126, 144, 144, 119, 126, 144, 144, 119, 126, 144, 144, 120, 127, 144, 144, 120, 127, 144, 144, 120, 127, 144, 144, 120, 127, 144, 144, 120, 127, 144, 144, 121, 128, 144, 144, 121, 128, 144, 144, 121, 128, 144, 144, 121, 128, 144, 144, 121, 129, 144, 144, 121, 129, 144, 144, 122, 129, 144, 144, 122, 129, 144, 144, 122, 129, 144, 144, 122, 130, 144, 144, 122, 130, 144, 144, 123, 130, 144, 144, 123, 130, 144, 144, 123, 130, 144, 144, 123, 131, 144, 144, 123, 131, 144, 144, 123, 131, 144, 144, 124, 131, 144, 144, 124, 131, 144, 144, 124, 132, 144, 144, 124, 132, 144, 144, 124, 132, 144, 144, 125, 132, 144, 144, 125, 132, 144, 144, 125, 133, 144, 144, 125, 133, 144, 144, 125, 133, 144, 144, 125, 133, 144, 144, 126, 133, 144, 144, 126, 134, 144, 144, 126, 134, 144, 144, 126, 134, 144, 144, 126, 134, 144, 144, 126, 134, 144, 144, 127, 135, 144, 144, 127, 135, 144, 144, 127, 135, 144, 144, 127, 135, 144, 144, 127, 136, 144, 144, 128, 136, 144, 144, 128, 136, 144, 144, 128, 136, 144, 144, 128, 136, 144, 144, 128, 137, 144, 144, 128, 137, 144, 144, 129, 137, 
144, 144, 129, 137, 144, 144, 129, 137, 144, 144, 129, 138, 144, 144, 129, 138, 144, 144, 130, 138, 144, 144, 130, 138, 144, 144, 130, 138, 144, 144, 130, 139, 144, 144, 130, 139, 144, 144, 130, 139, 144, 144, 131, 139, 144, 144, 131, 139, 144, 144, 131, 140, 144, 144, 131, 140, 144, 144, 131, 140, 144, 144, 132, 140, 144, 144, 132, 140, 144, 144, 132, 141, 144, 144, 132, 141, 144, 144, 132, 141, 144, 144, 132, 141, 144, 144, 133, 141, 144, 144, 133, 142, 144, 144, 133, 142, 144, 144, 133, 142, 144, 144, 133, 142, 144, 144, 134, 143, 144, 144, 134, 143, 144, 144, 134, 143, 144, 144, 134, 143, 144, 144, 134, 143, 144, 144, 134, 144, 144, 144, 135, 144, 144, 144, 135, 144, 144, 144, 135, 144, 144, 144, 135, 144, 144, 144, 135, 144, 144, 144, 135, 144, 144, 144, 136, 144, 144, 144, 136, 144, 144, 144, 136, 144, 144, 144, 136, 144, 144, 144, 136, 144, 144, 144, 137, 144, 144, 144, 137, 144, 144, 144, 137, 144, 144, 144, 137, 144, 144, 144, 137, 144, 144, 144, 137, 144, 144, 144, 138, 144, 144, 144, 138, 144, 144, 144, 138, 144, 144, 144, 138, 144, 144, 144, 138, 144, 144, 144, 139, 144, 144, 144, 139, 144, 144, 144, 139, 144, 144, 144, 139, 144, 144, 144, 139, 144, 144, 144, 139, 144, 144, 144, 140, 144, 144, 144, 140, 144, 144, 144, 140, 144, 144, 144, 140, 144, 144, 144, 140, 144, 144, 144, 141, 144, 144, 144, 141, 144, 144, 144, 141, 144, 144, 144, 141, 144, 144, 144, 141, 144, 144, 144, 141, 144, 144, 144, 
142, 144, 144, 144, 142, 144, 144, 144, 142, 144, 144, 144, 142, 144, 144, 144, 142, 144, 144, 144, 143, 144, 144, 144, 143, 144, 144, 144, 143, 144, 144, 144, 143, 144, 144, 144, 143, 144, 144, 144, 143, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 
144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144
};

/*
const s16 floor_light_positions[512*4] = {
    -1, -1, -1, -1, 
    62, 63, 66, 68, 
    68, 69, 71, 73, 
    70, 71, 73, 75, 
    71, 72, 74, 76, 
    72, 72, 74, 76, 
    72, 73, 75, 76, 
    72, 73, 75, 77, 72, 73, 75, 77, 72, 73, 75, 77, 73, 73, 75, 77, 73, 74, 76, 78, 73, 74, 76, 78, 73, 74, 77, 79, 73, 75, 77, 80, 74, 75, 78, 80, 74, 75, 78, 81, 74, 75, 79, 82, 74, 76, 79, 82, 74, 76, 80, 83, 75, 76, 80, 84, 75, 77, 81, 84, 75, 77, 81, 85, 75, 77, 82, 86, 75, 77, 82, 86, 76, 78, 83, 87, 76, 78, 83, 88, 76, 78, 83, 88, 76, 79, 84, 89, 76, 79, 84, 89, 77, 79, 85, 90, 77, 79, 85, 91, 77, 80, 86, 91, 77, 80, 86, 92, 77, 80, 87, 93, 78, 81, 87, 93, 78, 81, 88, 94, 78, 81, 88, 95, 78, 81, 89, 95, 79, 82, 89, 96, 79, 82, 90, 97, 79, 82, 90, 97, 79, 83, 91, 98, 79, 83, 91, 99, 80, 83, 92, 99, 80, 83, 92, 100, 80, 84, 93, 101, 80, 84, 93, 101, 80, 84, 94, 102, 81, 85, 94, 103, 81, 85, 95, 103, 81, 85, 95, 104, 81, 85, 95, 105, 81, 86, 96, 105, 82, 86, 96, 106, 82, 86, 97, 107, 82, 87, 97, 107, 82, 87, 98, 108, 82, 87, 98, 108, 83, 87, 99, 109, 83, 88, 99, 110, 83, 88, 100, 110, 83, 88, 100, 111, 83, 89, 101, 112, 84, 89, 101, 112, 84, 89, 102, 113, 84, 90, 102, 114, 84, 90, 103, 114, 84, 90, 103, 115, 85, 90, 104, 116, 85, 91, 104, 116, 85, 91, 105, 117, 85, 91, 105, 118, 86, 92, 106, 118, 86, 92, 106, 119, 86, 92, 107, 120, 86, 92, 107, 120, 86, 93, 107, 121, 87, 93, 108, 122, 87, 93, 108, 122, 87, 94, 109, 123, 87, 94, 109, 124, 87, 94, 110, 124, 88, 94, 110, 125, 88, 95, 111, 125, 88, 95, 111, 126, 88, 95, 112, 127, 88, 96, 112, 127, 89, 96, 113, 128, 89, 96, 113, 129, 89, 96, 114, 129, 89, 97, 114, 130, 89, 97, 115, 131, 90, 97, 115, 131, 90, 98, 116, 132, 90, 98, 116, 133, 90, 98, 117, 133, 90, 98, 117, 134, 91, 99, 118, 135, 91, 99, 118, 135, 91, 99, 119, 136, 91, 100, 119, 137, 91, 100, 119, 137, 92, 100, 120, 138, 92, 100, 120, 139, 92, 101, 121, 139, 92, 101, 121, 140, 93, 101, 122, 141, 93, 102, 122, 141, 93, 102, 123, 142, 93, 102, 123, 143, 93, 102, 124, 143, 94, 103, 124, 144, 94, 103, 125, 144, 94, 103, 125, 144, 94, 104, 126, 144, 94, 104, 126, 144, 95, 104, 127, 144, 95, 104, 127, 144, 95, 105, 128, 144, 95, 105, 128, 144, 95, 105, 129, 144, 96, 106, 129, 144, 96, 106, 130, 144, 96, 106, 130, 144, 96, 107, 131, 144, 96, 107, 131, 144, 97, 107, 131, 144, 97, 107, 132, 144, 97, 108, 132, 144, 97, 108, 133, 144, 97, 108, 133, 144, 98, 109, 134, 144, 98, 109, 134, 144, 98, 109, 135, 144, 98, 109, 135, 144, 98, 110, 136, 144, 99, 110, 136, 144, 99, 110, 137, 144, 99, 111, 137, 144, 99, 111, 138, 144, 100, 111, 138, 144, 100, 111, 139, 144, 100, 112, 139, 144, 100, 112, 140, 144, 100, 112, 140, 144, 101, 113, 141, 144, 101, 113, 141, 144, 101, 113, 142, 144, 101, 113, 142, 144, 101, 114, 143, 144, 102, 114, 143, 144, 102, 114, 143, 144, 102, 115, 144, 144, 102, 115, 144, 144, 102, 115, 144, 144, 103, 115, 144, 144, 103, 116, 144, 144, 103, 116, 144, 144, 103, 116, 144, 144, 103, 117, 144, 144, 104, 117, 144, 144, 104, 117, 144, 144, 104, 117, 144, 144, 104, 118, 144, 144, 104, 118, 144, 144, 105, 118, 144, 144, 105, 119, 144, 144, 105, 119, 144, 144, 105, 119, 144, 144, 105, 119, 144, 144, 106, 120, 144, 144, 106, 120, 144, 144, 106, 120, 144, 144, 106, 121, 144, 144, 107, 121, 144, 144, 107, 121, 144, 144, 107, 121, 144, 144, 107, 122, 144, 144, 107, 122, 144, 144, 108, 122, 144, 144, 108, 123, 144, 144, 108, 123, 144, 144, 108, 123, 144, 144, 108, 123, 144, 144, 109, 124, 144, 144, 109, 124, 144, 144, 109, 124, 144, 144, 109, 125, 144, 144, 109, 125, 144, 144, 110, 125, 144, 144, 110, 126, 144, 144, 110, 126, 144, 144, 110, 126, 144, 144, 110, 126, 144, 144, 111, 127, 144, 144, 111, 127, 144, 144, 111, 127, 144, 144, 111, 128, 144, 144, 111, 128, 144, 144, 112, 128, 144, 144, 112, 128, 144, 144, 112, 129, 144, 144, 112, 129, 144, 144, 112, 129, 144, 144, 113, 130, 144, 144, 113, 130, 144, 144, 113, 130, 144, 144, 113, 130, 144, 144, 113, 131, 144, 144, 114, 131, 144, 144, 114, 131, 144, 144, 114, 132, 144, 144, 114, 132, 144, 144, 115, 132, 144, 144, 115, 132, 144, 144, 115, 133, 144, 144, 115, 133, 144, 144, 115, 133, 144, 144, 116, 134, 144, 144, 116, 134, 144, 144, 116, 134, 144, 144, 116, 134, 144, 144, 116, 135, 144, 144, 117, 135, 144, 144, 117, 135, 144, 144, 117, 136, 144, 144, 117, 136, 144, 144, 117, 136, 144, 144, 118, 136, 144, 144, 118, 137, 144, 144, 118, 137, 144, 144, 118, 137, 144, 144, 118, 138, 144, 144, 119, 138, 144, 144, 119, 138, 144, 144, 119, 138, 144, 144, 119, 139, 144, 144, 119, 139, 144, 144, 120, 139, 144, 144, 120, 140, 144, 144, 120, 140, 144, 144, 120, 140, 144, 144, 120, 140, 144, 144, 121, 141, 144, 144, 121, 141, 144, 144, 121, 141, 144, 144, 121, 142, 144, 144, 122, 142, 144, 144, 122, 142, 144, 144, 122, 143, 144, 144, 122, 143, 144, 144, 122, 143, 144, 144, 123, 143, 144, 144, 123, 144, 144, 144, 123, 144, 144, 144, 123, 144, 144, 144, 123, 144, 144, 144, 124, 144, 144, 144, 124, 144, 144, 144, 124, 144, 144, 144, 124, 144, 144, 144, 124, 144, 144, 144, 125, 144, 144, 144, 125, 144, 144, 144, 125, 144, 144, 144, 125, 144, 144, 144, 125, 144, 144, 144, 126, 144, 144, 144, 126, 144, 144, 144, 126, 144, 144, 144, 126, 144, 144, 144, 126, 144, 144, 144, 127, 144, 144, 144, 127, 144, 144, 144, 127, 144, 144, 144, 127, 144, 144, 144, 127, 144, 144, 144, 128, 144, 144, 144, 128, 144, 144, 144, 128, 144, 144, 144, 128, 144, 144, 144, 129, 144, 144, 144, 129, 144, 144, 144, 129, 144, 144, 144, 129, 144, 144, 144, 129, 144, 144, 144, 130, 144, 144, 144, 130, 144, 144, 144, 130, 144, 144, 144, 130, 144, 144, 144, 130, 144, 144, 144, 131, 144, 144, 144, 131, 144, 144, 144, 131, 144, 144, 144, 131, 144, 144, 144, 131, 144, 144, 144, 132, 144, 144, 144, 132, 144, 144, 144, 132, 144, 144, 144, 132, 144, 144, 144, 132, 144, 144, 144, 133, 144, 144, 144, 133, 144, 144, 144, 133, 144, 144, 144, 133, 144, 144, 144, 133, 144, 144, 144, 134, 144, 144, 144, 134, 144, 144, 144, 134, 144, 144, 144, 134, 144, 144, 144, 134, 144, 144, 144, 135, 144, 144, 144, 135, 144, 144, 144, 135, 144, 144, 144, 135, 144, 144, 144, 136, 144, 144, 144, 136, 144, 144, 144, 136, 144, 144, 144, 136, 144, 144, 144, 136, 144, 144, 144, 137, 144, 144, 144, 137, 144, 144, 144, 137, 144, 144, 144, 137, 144, 144, 144, 137, 144, 144, 144, 138, 144, 144, 144, 138, 144, 144, 144, 138, 144, 144, 144, 138, 144, 144, 144, 138, 144, 144, 144, 139, 144, 144, 144, 139, 144, 144, 144, 139, 144, 144, 144, 139, 144, 144, 144, 139, 144, 144, 144, 140, 144, 144, 144, 140, 144, 144, 144, 140, 144, 144, 144, 140, 144, 144, 144, 140, 144, 144, 144, 141, 144, 144, 144, 141, 144, 144, 144, 141, 144, 144, 144, 141, 144, 144, 144, 141, 144, 144, 144, 142, 144, 144, 144, 142, 144, 144, 144, 142, 144, 144, 144, 142, 144, 144, 144, 143, 144, 144, 144, 143, 144, 144, 144, 143, 144, 144, 144, 143, 144, 144, 144, 143, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144};
*/

const s16 ceil_light_positions[512*4] = {
    -1, -1, -1, -1, 75, 78, 78, 79, 70, 72, 73, 73, 68, 70, 71, 71, 67, 69, 70, 70, 67, 69, 69, 70, 66, 68, 69, 69, 66, 68, 69, 69, 66, 68, 69, 69, 66, 68, 69, 69, 66, 68, 68, 69, 65, 67, 68, 69, 65, 67, 68, 68, 64, 67, 68, 68, 64, 66, 68, 68, 63, 66, 67, 68, 63, 66, 67, 68, 62, 66, 67, 67, 62, 65, 67, 67, 61, 65, 67, 67, 61, 65, 66, 67, 60, 64, 66, 67, 60, 64, 66, 67, 59, 64, 66, 66, 59, 64, 66, 66, 58, 63, 65, 66, 58, 63, 65, 66, 58, 63, 65, 66, 57, 62, 65, 65, 57, 62, 65, 65, 56, 62, 64, 65, 56, 62, 64, 65, 55, 61, 64, 65, 55, 61, 64, 65, 54, 61, 64, 64, 54, 60, 63, 64, 53, 60, 63, 64, 53, 60, 63, 64, 52, 60, 63, 64, 52, 59, 62, 63, 51, 59, 62, 63, 51, 59, 62, 63, 50, 58, 62, 63, 50, 58, 62, 63, 49, 58, 61, 63, 49, 58, 61, 62, 48, 57, 61, 62, 48, 57, 61, 62, 47, 57, 61, 62, 47, 56, 60, 62, 46, 56, 60, 62, 46, 56, 60, 61, 46, 56, 60, 61, 45, 55, 60, 61, 45, 55, 59, 61, 44, 55, 59, 61, 44, 54, 59, 60, 43, 54, 59, 60, 43, 54, 59, 60, 42, 54, 58, 60, 42, 53, 58, 60, 41, 53, 58, 60, 41, 53, 58, 59, 40, 52, 58, 59, 40, 52, 57, 59, 39, 52, 57, 59, 39, 51, 57, 59, 38, 51, 57, 58, 38, 51, 57, 58, 37, 51, 56, 58, 37, 50, 56, 58, 36, 50, 56, 58, 36, 50, 56, 58, 35, 49, 55, 57, 35, 49, 55, 57, 35, 49, 55, 57, 34, 49, 55, 57, 34, 48, 55, 57, 33, 48, 54, 56, 33, 48, 54, 56, 32, 47, 54, 56, 32, 47, 54, 56, 31, 47, 54, 56, 31, 47, 53, 56, 30, 46, 53, 55, 30, 46, 53, 55, 29, 46, 53, 55, 29, 45, 53, 55, 28, 45, 52, 55, 28, 45, 52, 54, 27, 45, 52, 54, 27, 44, 52, 54, 26, 44, 52, 54, 26, 44, 51, 54, 25, 43, 51, 54, 25, 43, 51, 53, 24, 43, 51, 53, 24, 43, 51, 53, 23, 42, 50, 53, 23, 42, 50, 53, 22, 42, 50, 52, 22, 41, 50, 52, 22, 41, 50, 52, 21, 41, 49, 52, 21, 41, 49, 52, 20, 40, 49, 52, 20, 40, 49, 51, 19, 40, 48, 51, 19, 39, 48, 51, 18, 39, 48, 51, 18, 39, 48, 51, 17, 39, 48, 51, 17, 38, 47, 50, 16, 38, 47, 50, 16, 38, 47, 50, 15, 37, 47, 50, 15, 37, 47, 50, 14, 37, 46, 49, 14, 37, 46, 49, 13, 36, 46, 49, 13, 36, 46, 49, 12, 36, 46, 49, 12, 35, 45, 49, 11, 35, 45, 48, 11, 35, 45, 48, 10, 35, 45, 48, 10, 34, 45, 48, 10, 34, 44, 48, 9, 34, 44, 47, 9, 33, 44, 47, 8, 33, 44, 47, 8, 33, 44, 47, 7, 32, 43, 47, 7, 32, 43, 47, 6, 32, 43, 46, 6, 32, 43, 46, 5, 31, 43, 46, 5, 31, 42, 46, 4, 31, 42, 46, 4, 30, 42, 45, 3, 30, 42, 45, 3, 30, 41, 45, 2, 30, 41, 45, 2, 29, 41, 45, 1, 29, 41, 45, 1, 29, 41, 44, 0, 28, 40, 44, 0, 28, 40, 44, 0, 28, 40, 44, 0, 28, 40, 44, -1, 27, 40, 43, -1, 27, 39, 43, -1, 27, 39, 43, -2, 26, 39, 43, -2, 26, 39, 43, -3, 26, 39, 43, -3, 26, 38, 42, -4, 25, 38, 42, -4, 25, 38, 42, -5, 25, 38, 42, -5, 24, 38, 42, -6, 24, 37, 42, -6, 24, 37, 41, -7, 24, 37, 41, -7, 23, 37, 41, -8, 23, 37, 41, -8, 23, 36, 41, -9, 22, 36, 40, -9, 22, 36, 40, -10, 22, 36, 40, -10, 22, 36, 40, -11, 21, 35, 40, -11, 21, 35, 40, -12, 21, 35, 39, -12, 20, 35, 39, -13, 20, 35, 39, -13, 20, 34, 39, -13, 20, 34, 39, -14, 19, 34, 38, -14, 19, 34, 38, -15, 19, 33, 38, -15, 18, 33, 38, -16, 18, 33, 38, -16, 18, 33, 38, -17, 18, 33, 37, -17, 17, 32, 37, -18, 17, 32, 37, -18, 17, 32, 37, -19, 16, 32, 37, -19, 16, 32, 36, -20, 16, 31, 36, -20, 15, 31, 36, -21, 15, 31, 36, -21, 15, 31, 36, -22, 15, 31, 36, -22, 14, 30, 35, -23, 14, 30, 35, -23, 14, 30, 35, -24, 13, 30, 35, -24, 13, 30, 35, -25, 13, 29, 34, -25, 13, 29, 34, -25, 12, 29, 34, -26, 12, 29, 34, -26, 12, 29, 34, -27, 11, 28, 34, -27, 11, 28, 33, -28, 11, 28, 33, -28, 11, 28, 33, -29, 10, 28, 33, -29, 10, 27, 33, -30, 10, 27, 33, -30, 9, 27, 32, -31, 9, 27, 32, -31, 9, 26, 32, -32, 9, 26, 32, -32, 8, 26, 32, -33, 8, 26, 31, -33, 8, 26, 31, -34, 7, 25, 31, -34, 7, 25, 31, -35, 7, 25, 31, -35, 7, 25, 31, -36, 6, 25, 30, -36, 6, 24, 30, -37, 6, 24, 30, -37, 5, 24, 30, -37, 5, 24, 30, -38, 5, 24, 29, -38, 5, 23, 29, -39, 4, 23, 29, 
-39, 4, 23, 29, -40, 4, 23, 29, -40, 3, 23, 29, -41, 3, 22, 28, -41, 3, 22, 28, -42, 3, 22, 28, -42, 2, 22, 28, -43, 2, 22, 28, -43, 2, 21, 27, -44, 1, 21, 27, -44, 1, 21, 27, -45, 1, 21, 27, -45, 1, 21, 27, -46, 0, 20, 27, -46, 0, 20, 26, -47, 0, 20, 26, -47, 0, 20, 26, -48, 0, 19, 26, -48, 0, 19, 26, -49, -1, 19, 25, -49, -1, 19, 25, -49, -1, 19, 25, -50, -1, 18, 25, -50, -2, 18, 25, -51, -2, 18, 25, -51, -2, 18, 24, -52, -3, 18, 24, -52, -3, 17, 24, -53, -3, 17, 24, -53, -3, 17, 24, -54, -4, 17, 24, -54, -4, 17, 23, -55, -4, 16, 23, -55, -5, 16, 23, -56, -5, 16, 
23, -56, -5, 16, 23, -57, -5, 16, 22, -57, -6, 15, 22, -58, -6, 15, 22, -58, -6, 15, 22, -59, -7, 15, 22, -59, -7, 15, 22, -60, -7, 14, 21, -60, -7, 14, 21, -61, -8, 14, 21, -61, -8, 14, 21, -61, -8, 14, 21, -62, -9, 13, 20, -62, -9, 13, 20, -63, -9, 13, 20, -63, -9, 13, 20, -64, -10, 
12, 20, -64, -10, 12, 20, -65, -10, 12, 19, -65, -11, 12, 19, -66, -11, 12, 19, -66, -11, 11, 19, -67, -11, 11, 19, -67, -12, 11, 18, -68, -12, 11, 18, -68, -12, 11, 18, -69, -13, 10, 18, -69, -13, 10, 18, -70, -13, 10, 18, -70, -13, 10, 17, -71, -14, 10, 17, -71, -14, 9, 17, -72, -14, 9, 17, -72, -15, 9, 17, -73, -15, 9, 17, -73, -15, 9, 16, -73, -15, 8, 16, -74, -16, 8, 16, -74, -16, 8, 16, -75, -16, 8, 16, -75, -17, 8, 15, -76, -17, 7, 15, -76, -17, 7, 15, -77, -17, 7, 15, -77, -18, 7, 15, -78, -18, 7, 15, -78, -18, 6, 14, -79, -19, 6, 14, -79, -19, 6, 14, -80, -19, 6, 14, -80, -20, 5, 14, -81, -20, 5, 13, -81, -20, 5, 13, -82, -20, 5, 13, -82, -21, 5, 13, -83, -21, 4, 13, -83, -21, 4, 13, -84, -22, 4, 12, -84, -22, 4, 12, -85, -22, 4, 12, -85, -22, 3, 12, -85, -23, 3, 12, -86, -23, 3, 11, -86, -23, 3, 11, -87, -24, 3, 11, -87, -24, 2, 11, -88, -24, 2, 11, -88, -24, 2, 11, -89, -25, 2, 10, -89, -25, 2, 10, -90, -25, 1, 10, -90, -26, 1, 10, -91, -26, 1, 10, -91, -26, 1, 9, -92, -26, 1, 9, -92, -27, 0, 9, -93, -27, 0, 9, -93, -27, 0, 9, -94, -28, 0, 9, -94, -28, 0, 8, -95, -28, 0, 8, -95, -28, 0, 8, -96, -29, 0, 8, -96, -29, 
0, 8, -97, -29, 0, 8, -97, -30, -1, 7, -97, -30, -1, 7, -98, -30, -1, 7, -98, -30, -1, 7, -99, -31, -2, 7, -99, -31, -2, 6, -100, -31, -2, 6, -100, -32, -2, 6, -101, -32, -2, 6, -101, -32, -3, 6, -102, -32, -3, 6, -102, -33, -3, 5, -103, -33, -3, 5, -103, -33, -3, 5, -104, -34, -4, 5, 
-104, -34, -4, 5, -105, -34, -4, 4, -105, -34, -4, 4, -106, -35, -4, 4, -106, -35, -5, 4, -107, -35, -5, 4, -107, -36, -5, 4, -108, -36, -5, 3, -108, -36, -5, 3, -109, -37, -6, 3, -109, -37, -6, 3, -109, -37, -6, 3, -110, -37, -6, 2, -110, -38, -6, 2, -111, -38, -7, 2, -111, -38, -7, 2, -112, -39, -7, 2, -112, -39, -7, 2, -113, -39, -7, 1, -113, -39, -8, 1, -114, -40, -8, 1, -114, -40, -8, 1, -115, -40, -8, 1, -115, -41, -9, 0, -116, -41, -9, 0, -116, -41, -9, 0, -117, -41, -9, 0, -117, -42, -9, 0, -118, -42, -10, 0, -118, -42, -10, 0, -119, -43, -10, 0, -119, -43, 
-10, 0, -120, -43, -10, 0, -120, -43, -11, 0, -121, -44, -11, -1, -121, -44, -11, -1, -121, -44, -11, -1, -122, -45, -11, -1, -122, -45, -12, -1, -123, -45, -12, -1, -123, -45, -12, -2, -124, -46, -12, -2, -124, -46, -12, -2, -125, -46, -13, -2, -125, -47, -13, -2, -126, -47, -13, -2, 
-126, -47, -13, -3, -127, -47, -13, -3, -127, -48, -14, -3, -128, -48, -14, -3, -128, -48, -14, -3, -129, -49, -14, -4, -129, -49, -14, -4, -130, -49, -15, -4, -130, -49, -15, -4, -131, -50, -15, -4, -131, -50, -15, -4, -132, -50, -16, -5, -132, -51, -16, -5, -133, -51, -16, -5, -133, 
-51, -16, -5, -133, -51, -16, -5, -134, -52, -17, -6, -134, -52, -17, -6, -135, -52, -17, -6, -135, -53, -17, -6, -136, -53, -17, -6, -136, -53, -18, -6, -137, -53, -18, -7, -137, -54, -18, -7, -138, -54, -18, -7, -138, -54, -18, -7, -139, -55, -19, -7, -139, -55, -19, -8, -140, -55, -19, -8, -140, -56, -19, -8, -141, -56, -19, -8, -141, -56, -20, -8, -142, -56, -20, -8, -142, -57, -20, -9, -143, -57, -20, -9, -143, -57, -20, -9, -144, -58, -21, -9, -144, -58, -21, -9, -145, -58, -21, -10, -145, -58, -21, -10, -145, -59, -21, -10, -146, -59, -22, -10, -146, -59, -22, -10, -147, -60, -22, -10, -147, -60, -22, -11, -148, -60, -23, -11, -148, -60, -23, -11, -149, -61, -23, -11, -149, -61, -23, -11, -150, -61, -23, -11, -150, -62, -24, -12, -151, -62, -24, -12, -151, -62, -24, -12, -152, -62, -24, -12, -152, -63, -24, -12, -153, -63, -25, -13, -153, 
-63, -25, -13, -154, -64, -25, -13, -154, -64, -25, -13, -155, -64, -25, -13, -155, -64, -26, -13, -156, -65, -26, -14, -156, -65, -26, -14, -157, -65, -26, -14, -157, -66, -26, -14, -157, -66, -27, -14, -158, -66, -27, -15, -158, -66, -27, -15, -159, -67, -27, -15, -159, -67, -27, -15, -160, -67, -28, -15, -160, -68, -28, -15, -161, -68, -28, -16, -161, -68, -28, -16, -162, -68, -28, -16, -162, -69, -29, -16, -163, -69, -29, -16, -163, -69, -29, -17, -164, -70, -29, -17, -164, -70, -30, -17, -165, -70, -30, -17, -165, -70, -30, -17, -166, -71, -30, -17, -166, -71, 
-30, -18, -167, -71, -31, -18, -167, -72, -31, -18, -168, -72, -31, -18, -168, -72, -31, -18, -169, -73, -31, -19, -169, -73, -32, -19, -169, -73, -32, -19, -170, -73, -32, -19, -170, -74, -32, -19, -171, -74, -32, -19, -171, -74, -33, -20, -172, -75, -33, -20, -172, -75, -33, -20, -173, -75, -33, -20, -173, -75, -33, -20, -174, -76, -34, -20
};


void cache_floor_light_params(s16 rel_floor_height, u8 floor_col, s8 light_level, light_params* params) {
    u32 light_color, mid_color, dark_color;

    //if(rel_floor_height == cached_rel_floor_height &&
    //   floor_col == cached_floor_col &&
    //   light_level == cached_floor_light_level) {
    //    return;      
    //}
    //cached_rel_floor_height = rel_floor_height;
    //cached_floor_col = floor_col;
    //cached_floor_light_level = light_level;

    
    // shift, lookup, lookup
    light_color = get_light_color(floor_col, light_level);
    
    // shift, lookup, lookup
    //mid_color = get_mid_dark_color(floor_col, light_level);

    // shift, lookup, lookup
    dark_color = get_dark_color(floor_col, light_level);
    params->light_color = light_color;
    //params->mid_color = mid_color;
    params->dark_color = dark_color;
    

    params->needs_lighting = 1;
    
    int table_idx = (-rel_floor_height)*4;

    // lookup

    //s16 fade_bot = floor_light_positions[table_idx++];
    table_idx++; // skip fade


    s16 dark_bot = floor_light_positions[table_idx++];

    table_idx++; // mid_dark

    //s16 mid_top = mid_dark_bot;
    // lookup
    //s16 mid_bot = floor_light_positions[table_idx++];

    params->dark_y = dark_bot;
    //params->mid_y = mid_bot;
    //params->fade_y = fade_bot;
}


void cache_ceil_light_params(s16 rel_ceil_height, u8 ceil_col, s8 light_level, light_params* params) {
    u32 light_color, mid_color, dark_color;

    //if(rel_ceil_height == cached_rel_ceil_height &&
    //   ceil_col == cached_ceil_col &&
    //   light_level == cached_ceil_light_level) {
    //    return;      
    //}
    //cached_rel_ceil_height = rel_ceil_height;
    //cached_ceil_col = ceil_col;
    //cached_ceil_light_level = light_level;


    light_color = get_light_color(ceil_col, light_level);
    params->light_color = light_color;

    
    //mid_color = get_mid_dark_color(ceil_col, light_level);
    dark_color = get_dark_color(ceil_col, light_level);
    //if(light_color == mid_color && mid_color == dark_color) {
    //    params->needs_lighting = 0;
    //    return;
    //}
    //params->mid_color = mid_color;
    params->dark_color = dark_color;
    params->needs_lighting = 1;


    int table_idx = rel_ceil_height*4;


    //s16 mid_top = ceil_light_positions[table_idx++];
    table_idx++; // mid
    table_idx++; // mid-dark
    s16 dark_top = ceil_light_positions[table_idx++];

    //s16 fade_top = ceil_light_positions[table_idx++];
    // skip fade

    /*
    table_idx++; // light
    s16 mid_top = ceil_light_positions[table_idx++];
    table_idx++; // mid-dark
    s16 dark_top = ceil_light_positions[table_idx++];
    */

    params->dark_y = dark_top;
    //params->mid_y = mid_top;
    //params->fade_y = fade_top;
}

// draw just light
// draw light and med
// draw med and dark
// draw light, med, dark

void draw_lit_floor_light_only(s16 floor_top_y, s16 floor_bot_y, u8* col_ptr, light_params* params) {
    draw_native_vertical_line_unrolled(floor_top_y, floor_bot_y, params->light_color, col_ptr);
}

void draw_lit_floor(s16 floor_top_y, s16 floor_bot_y, u8* col_ptr, light_params* params) {
    
    //return draw_lit_floor_light_only(floor_top_y, floor_bot_y, col_ptr, params);
    #ifndef FLATS_DIST_LIGHTING
    s16 floor_mid = (floor_bot_y+floor_top_y)>>1;
    s16 dark_y = params->dark_y;
    s16 mid_y = params->mid_y;
    if(floor_mid >= dark_y) {
        return draw_native_vertical_line_unrolled(floor_top_y, floor_bot_y, params->dark_color, col_ptr);
    } else if (floor_mid >= mid_y) {
        return draw_native_vertical_line_unrolled(floor_top_y, floor_bot_y, params->mid_color, col_ptr);
    } else {
        return draw_native_vertical_line_unrolled(floor_top_y, floor_bot_y, params->light_color, col_ptr);
    }
    #endif

    /*
    s16 dark_y = params->dark_y;
    s16 mid_y = params->mid_y;

    s16 y0 = floor_top_y;
    s16 y1 = floor_bot_y;
    if(y0 < dark_y) {
        if(y1 > mid_y) {
            draw_native_vertical_line_unrolled(y0, dark_y, params->dark_color, col_ptr);
            draw_native_vertical_line_unrolled(dark_y, mid_y, params->mid_color, col_ptr);
            draw_native_vertical_line_unrolled(mid_y, y1, params->light_color, col_ptr);
        } else if (y1 > dark_y) {
            draw_native_double_vertical_line_unrolled(y0, dark_y, y1, params->dark_color, params->mid_color, col_ptr);
            //draw_native_vertical_line_unrolled(y0, dark_y, params->dark_color, col_ptr);
            //draw_native_vertical_line_unrolled(dark_y, y1, params->mid_color, col_ptr);
        } else {
            draw_native_vertical_line_unrolled(y0, y1, params->dark_color, col_ptr);
        }
    } else if (y0 < mid_y) {
        if(y1 > mid_y) {
            //draw_native_vertical_line_unrolled(y0, mid_y, params->mid_color, col_ptr);
            //draw_native_vertical_line_unrolled(mid_y, y1, params->light_color, col_ptr);
            draw_native_double_vertical_line_unrolled(y0, mid_y, y1, params->mid_color, params->light_color, col_ptr);
        } else {
            draw_native_vertical_line_unrolled(y0, y1, params->mid_color, col_ptr);
        }
    } else {
        draw_native_vertical_line_unrolled(y0, y1, params->light_color, col_ptr);
    }
    */
    
    //int draw_fade_top = floor_top_y;
    //int draw_fade_bot = min(floor_bot_y, params->fade_y);
    //if(draw_fade_top < draw_fade_bot) {
    //    draw_native_vertical_line_unrolled(draw_fade_top, draw_fade_bot, 0, col_ptr);
    //}
    //int draw_dark_top = max(floor_top_y, params->fade_y);

    int draw_dark_top = floor_top_y;
    int draw_dark_bot = min(floor_bot_y, params->dark_y);

    if(draw_dark_top < draw_dark_bot) {
        draw_native_vertical_line_unrolled(draw_dark_top, draw_dark_bot, params->dark_color, col_ptr);
    }

    //int draw_mid_top = max(floor_top_y, params->dark_y);
    //int draw_mid_bot = min(floor_bot_y, params->mid_y);
    //if(draw_mid_top < draw_mid_bot) {
    //    draw_native_vertical_line_unrolled(draw_mid_top, draw_mid_bot, params->mid_color, col_ptr);
    //}

    int draw_light_top = max(floor_top_y, params->dark_y);
    int draw_light_bot = floor_bot_y;
    if(draw_light_top < draw_light_bot) {
        draw_native_vertical_line_unrolled(draw_light_top, draw_light_bot, params->light_color, col_ptr);
    }

}

void draw_lit_ceil_light_only(s16 ceil_top_y, s16 ceil_bot_y, u8* col_ptr, light_params* params) {
    return draw_native_vertical_line_unrolled(ceil_top_y, ceil_bot_y, params->light_color, col_ptr);
}

void draw_lit_ceiling(s16 ceil_top_y, s16 ceil_bot_y, u8* col_ptr, light_params* params) {
    //return draw_lit_ceil_light_only(ceil_top_y, ceil_bot_y, col_ptr, params);
    #ifndef FLATS_DIST_LIGHTING
    s16 mid = ((ceil_bot_y-ceil_top_y)<<1)+ceil_top_y;
    if(mid < params->mid_y) {
        return draw_native_vertical_line_unrolled(ceil_top_y, ceil_bot_y, params->light_color, col_ptr);
    } else {

    }
    #endif

    int draw_light_top = ceil_top_y;
    int draw_light_bot = min(ceil_bot_y, params->dark_y); //params->mid_y);

    if(draw_light_top < draw_light_bot) {
        draw_native_vertical_line_unrolled(draw_light_top, draw_light_bot, params->light_color, col_ptr);
    }

    //int draw_mid_top = max(ceil_top_y, params->mid_y);
    //int draw_mid_bot = min(ceil_bot_y, params->dark_y);
    //if(draw_mid_top < draw_mid_bot) {
    //    draw_native_vertical_line_unrolled(draw_mid_top, draw_mid_bot, params->mid_color, col_ptr);
    //}

    int draw_dark_top = max(ceil_top_y, params->dark_y);
    int draw_dark_bot = ceil_bot_y;
    if (draw_dark_top < draw_dark_bot) {
        draw_native_vertical_line_unrolled(draw_dark_top, draw_dark_bot, params->dark_color, col_ptr);
    }

    //int draw_dark_bot = min(ceil_bot_y, params->fade_y);
    //int draw_fade_top = max(ceil_top_y, params->fade_y);
    //int draw_fade_bot = ceil_bot_y;
    //if (draw_fade_top < draw_fade_bot) {
    //    draw_native_vertical_line_unrolled(draw_fade_top, draw_fade_bot, 0, col_ptr);
    //}



}

typedef void (*draw_lit_plane_fp)(s16 top_y, s16 bot_y, u8* col_ptr, light_params* params);



//u32 tex_col_buffer[RENDER_WIDTH];

//u16 tex_col_buffer[RENDER_WIDTH];

u16 tex_col_buffer[RENDER_WIDTH*2];

void calculate_tex_coords_for_wall(
    s16 beginx, s16 endx, s16 skip_x, s16 dx,
    u16 z1_12_4,     u16 z2_12_4,
    u16 inv_z1,      u16 inv_z2,
    texmap_params* tmap_info
) {

    u16 repetitions = tmap_info->repetitions;


    u32 left_u_16 = (tmap_info->left_u * repetitions);
    u32 right_u_16 = (tmap_info->right_u * repetitions);


    persp_params persp = calc_perspective(z1_12_4, z2_12_4, inv_z1, inv_z2, left_u_16, right_u_16, dx);
    
    
    u32 one_over_z_26 = persp.one_over_z_26;
    s32 d_one_over_z_26 = persp.d_one_over_z_dx_26;

    u32 u_over_z_23 = persp.u_over_z_23;
    s32 d_u_over_z_23 = persp.d_u_over_z_dx_23;

    if(skip_x > 0) {
        one_over_z_26 += (skip_x * d_one_over_z_26);
        u_over_z_23 += skip_x * d_u_over_z_23;
    }

    u16 one_over_z_16 = one_over_z_26>>10;
    s16 d_one_over_z_16 = d_one_over_z_26>>10;
    //u16 u_over_z_16 = u_over_z_23>>7;
    //s16 d_u_over_z_16 = d_u_over_z_23>>7;

    u16 cnt = endx-beginx;
    u16* tex_col_ptr = tex_col_buffer;
 
    
    u8 rem_cnt = cnt & 0b1;
    while(rem_cnt--) { //cnt & 1) {        
        u16 u_7= divu_32_by_16(u_over_z_23, one_over_z_16); // 9.7 fixed point

        
        u16 u_11 = u_7 <<4;
        u16 u_masked_11 = 0b11111000000 & u_11;
        __asm volatile(
            "move.w %2, (%0)+\t\n\
             move.w %1, (%0)+"
            : "+a" (tex_col_ptr)
            : "d" (u_masked_11), "d" (one_over_z_16)
        );
        //u16 u_7; u32 u_tmp = u_over_z_23;// u_over_z_16<<7;           
        
        //__asm volatile(                             
        //    "divu.w %3, %1\t\n\
        //    move.w %1, %0\t\n\
        //    lsl.w #4, %0\t\n\
        //    and.w #1984, %0\t\n\
        //    move.w %3, (%2)+\t\n\
        //    move.w %0, (%2)+"                       
        //    : "=d" (u_7), "+d" (u_tmp), "+a" (tex_col_ptr) 
        //    : "d" (one_over_z_16));                 
        one_over_z_16 += d_one_over_z_16; 
        u_over_z_23 += d_u_over_z_23;  
        //u_over_z_16 += d_u_over_z_16;     
    }
    cnt>>=1;

    //u16 u_7_start = divu_32_by_16(u_over_z_16<<7, one_over_z_16);
    u16 u_7_start = divu_32_by_16(u_over_z_23, one_over_z_16);

    //s16 d_one_over_z_16_times_2 = d_one_over_z_16 << 1;
    //s16 d_u_over_z_16_times_2 = d_u_over_z_16 << 1;
    
    s16 d_one_over_z_16_times_2 = d_one_over_z_16 << 1;
    //s16 d_u_over_z_16_times_2 = d_u_over_z_16 << 1;
    s32 d_u_over_z_23_times_2 = d_u_over_z_23 << 1;
    //u16 tmp;

    while(cnt--) {
        u16 one_over_z_16_span_end = one_over_z_16 + d_one_over_z_16_times_2;
        //u16 u_over_z_16_span_end = u_over_z_16 + d_u_over_z_16_times_2;
        u32 u_over_z_23_span_end = u_over_z_23 + d_u_over_z_23_times_2;
        
        u16 u_7_end = divu_32_by_16(u_over_z_23_span_end, one_over_z_16_span_end); // 9.7 fixed point
        
        u16 u_per_col = u_7_end;
        u_per_col -= u_7_start;
        u_per_col >>= 1;

        
        u16 u_7 = u_7_start;
        
        u16 u_11 = u_7 <<4;
        u16 u_masked_11 = 0b11111000000 & u_11;
        __asm volatile(
            "move.w %2, (%0)+\t\n\
             move.w %1, (%0)+"
            : "+a" (tex_col_ptr)
            : "d" (u_masked_11), "d" (one_over_z_16)
        );
        u_7 += u_per_col;

        u_11 = u_7<<4;
        u_masked_11 = 0b11111000000 & u_11;

        one_over_z_16 += d_one_over_z_16;

        __asm volatile(
            "move.w %2, (%0)+\t\n\
             move.w %1, (%0)+"
            : "+a" (tex_col_ptr)
            : "d" (u_masked_11), "d" (one_over_z_16)
        );

        u_over_z_23 = u_over_z_23_span_end;
        one_over_z_16 = one_over_z_16_span_end;
        u_7_start = u_7_end;
    }
    
}

void calculate_tex_coords_for_wall_old(
    s16 beginx, s16 endx, s16 skip_x, s16 dx,
    u16 z1_12_4,     u16 z2_12_4,
    u16 inv_z1,      u16 inv_z2,
    texmap_params* tmap_info) {
    
    u16 repetitions = tmap_info->repetitions;
    u32 left_u_16 = tmap_info->left_u * repetitions;
    u32 right_u_16 = tmap_info->right_u * repetitions;
    persp_params persp = calc_perspective(z1_12_4, z2_12_4, inv_z1, inv_z2, left_u_16, right_u_16, dx);
    

    u32 one_over_z_26 = persp.one_over_z_26;
    s32 d_one_over_z_dx_26 = persp.d_one_over_z_dx_26;
    u32 u_over_z_23 = persp.u_over_z_23;
    u32 d_u_over_z_dx_23 = persp.d_u_over_z_dx_23;

    //u32 du = right_u_16 - left_u_16;

    //u32 du_dx = du/dx;

    //u32 cur_u = left_u_16;
    if(skip_x > 0) {
        // always use perspective
        one_over_z_26 += (skip_x * d_one_over_z_dx_26);
        u_over_z_23 += skip_x * d_u_over_z_dx_23;

        // TEMP HACK FOR NO PERSPECTIVE
        //cur_u += (skip_x * du_dx);
    }

    u16 one_over_z_16 = one_over_z_26>>10;
    //u16 u_over_z_16 = u_over_z_23>>7;
    s16 d_one_over_z_dx_16 = d_one_over_z_dx_26>>10;
    //u16 d_u_over_z_dx_16 = d_u_over_z_dx_23>>7;

    



    //if(skip_x > 0) {
    //    // always use perspective
    //    one_over_z_26 += (skip_x * d_one_over_z_dx_26);
    //    u_over_z_23 += skip_x * d_u_over_z_dx_23;
    //}


    //const u8 tex_width_shift = MID_MIP_WIDTH_SHIFT;
    //const u8 tex_width = MID_MIP_WIDTH;
    

    
    u16 cnt = endx-beginx;
    u16* tex_col_ptr = tex_col_buffer;


    //while(cnt--) {
    //    //u16 u_16 = cur_u>>16;
    //    u16 u_masked = cur_u & 0b1111100000000000;
    //    *tex_col_ptr++ = u_masked>>5; //(cur_u>>16)<<4;
    //    cur_u += du_dx;
    //}
    //return;

    

    #define TEXMAP_ITER do { \
        u32 z = (1<<26)/one_over_z_26;\
        u32 u_23 = u_over_z_23 * z;\
        u32 u_scaled_by_width = 0; \
        __asm volatile(                         \
            "move.l %1, %2\t\n\
            swap %2\t\n\
            and.w #124, %2\t\n\
            lsl.w #4, %2\t\n\
            move.w %2, (%0)+"\
            : "+a" (tex_col_ptr)\
            : "d" (u_23), "d" (u_scaled_by_width)\
        );\
        one_over_z_26 += d_one_over_z_dx_26;\
        u_over_z_23 += d_u_over_z_dx_23;\
    } while(0)

    //divu_32_by_16((1<<16),one_over_z_16);
    // = u_over_z_16 * z;
    // u32 tmp = 1<<16; 


    #define TEXMAP_16_ITER do { \
        u32 tmp;\
    __asm volatile(\
        "moveq #1, %3\t\n\
        swap %3\t\n\
        divu.w %1, %3\t\n\
        muls.w %2, %3\t\n\
        and.w #63488, %3\t\n\
        lsr.w #5, %3\t\n\
        move.w %3, (%0)+\t\n\
        add.w %4, %1\t\n\
        add.w %5, %2"\
        : "+a" (tex_col_ptr), "+d" (one_over_z_16), "+d" (u_over_z_16)\
        : "d" (tmp), "d" (d_one_over_z_dx_16), "d" (d_u_over_z_dx_16)\
    );\
    } while(0)

    // u_7 = u_over_z_23/one_over_z_16;
    // shift left by four to get u_11
    // mask so we keep only the top 5 fractional bits out of 11


    #define TEXMAP_16_32_ITER do {                  \
        u16 u_7; u32 u_tmp = u_over_z_23;           \
        __asm volatile(                             \
            "divu.w %3, %1\t\n                      \
            move.w %1, %0\t\n                       \
            lsl.w #4, %0\t\n                        \
            and.w #1984, %0\t\n                     \
            move.w %0, (%2)+"                       \
            : "=d" (u_7), "+d" (u_tmp), "+a" (tex_col_ptr) \
            : "d" (one_over_z_16));                 \
        one_over_z_16 += d_one_over_z_dx_16;        \
        u_over_z_23 += d_u_over_z_dx_23;            \
    } while(0)

    #define AFFINE_TEXMAP_16_32_ITER do {                  \
        u16 u_7; u32 u_tmp = u_over_z_23;           \
        __asm volatile(                             \
            "lsl.w #4, %0\t\n                       \
            and.w #1984, %0\t\n                     \
            move.w %0, (%1)+"                       \
            : "=d" (u_7), "+a" (tex_col_ptr)        \
            : );                                    \
        u_7 += u_per_col;                           \
    } while(0)

    //one_over_z_16 += d_one_over_z_dx_16;
    //u_over_z_16 += d_u_over_z_dx_16;

    #define AFFINE_TEXMAP_ITER do { \
        u32 u_scaled_by_width = 0;      \
        __asm volatile(                         \
            "move.l %1, %2\t\n\
            swap %2\t\n\
            and.w #124, %2\t\n\
            lsl.w #4, %2\t\n\
            move.w %2, (%0)+"\
            : "+a" (tex_col_ptr)\
            : "d" (u_23), "d" (u_scaled_by_width)\
        );\
        u_23 += u_per_col;\
    } while(0)


    #define AFFINE_TEXMAP_16_ITER do { \
        u16 u_scaled_by_width = 0;      \
        __asm volatile(                         \
            "move.w %1, %2\t\n\
             and.w #63488, %2\t\n\
             lsr.w #5, %2\t\n\
             move.w %2, (%0)+"\
            : "+a" (tex_col_ptr)\
            : "d" (u_start), "d" (u_scaled_by_width)\
        );\
        u_start += u_per_col;\
    } while(0)


    
    if(cnt & 1) {
        TEXMAP_16_32_ITER;
    }
    cnt>>=1;
    
    u32 tmp = u_over_z_23;
    u16 u_7_start;// = u_over_z_23/one_over_z_16;
    __asm volatile(
        "divu.w %2, %1\t\n\
        move.w %1, %0"
        : "=d" (u_7_start), "+d" (tmp)
        : "d" (one_over_z_16)
    );
    s16 d_one_over_z_dx_16_times_2 = d_one_over_z_dx_16 + d_one_over_z_dx_16; //<<1;
    u32 d_u_over_z_dx_23_times_2 = d_u_over_z_dx_23 + d_u_over_z_dx_23; //<<1;

    while(cnt--) {
        u16 one_over_z_16_span_end = one_over_z_16 + d_one_over_z_dx_16_times_2;
        u32 u_over_z_23_span_end = u_over_z_23 + d_u_over_z_dx_23_times_2;

        
        tmp = u_over_z_23_span_end;
        u16 u_7_end; // 9.7 fixed point
        __asm volatile(
            "divu.w %2, %1\t\n\
            move.w %1, %0\t\n\
            "
            : "+d" (u_7_end), "+d" (tmp)
            : "d" (one_over_z_16_span_end)
        );

        //u16 u_7_end = u_over_z_23_span_end / one_over_z_16_span_end;
        
        u16 u_per_col = u_7_end;
        u_per_col -= u_7_start;
        u_per_col >>= 1;
        //u_per_col <<= 4;

        //u16 u_per_col = ((u_7_end - u_7_start)>>1)<<4;
        
        u16 u_7 = u_7_start;
        
        u16 u_11 = u_7 <<4;
        u16 u_masked_11 = 0b11111000000 & u_11;
        //*tex_col_ptr++ = u_masked_11;
        __asm volatile(
            "move.w %1, (%0)+"
            : "+a" (tex_col_ptr)
            : "d" (u_masked_11)
        );
        u_7 += u_per_col;

        u_11 = u_7<<4;
        u_masked_11 = 0b11111000000 & u_11;

        //*tex_col_ptr++ = u_masked_11;
        __asm volatile(
            "move.w %1, (%0)+"
            : "+a" (tex_col_ptr)
            : "d" (u_masked_11)
        );
        

        //u16 u_11 = u_7_start<<4;
        //__asm volatile(
        //    : "d" "+a" (tex_col_ptr)
        //)
        //AFFINE_TEXMAP_16_32_ITER;
        //AFFINE_TEXMAP_16_32_ITER;

        u_over_z_23 = u_over_z_23_span_end;
        one_over_z_16 = one_over_z_16_span_end;
        u_7_start = u_7_end;
        //u16 u_16 = u_start;
    }
    
    // u_7 = u_over_z_23/one_over_z_16;
    // shift left by four to get u_11
    // mask so we keep only the top 5 fractional bits out of 11
    

    return;
    


    /*
   
    //u16 z_start = z_start = (1<<16)/one_over_z_16;
    u16 z_start = z_recip_table_16[one_over_z_16];
    u16 u_start = u_over_z_16 * z_start;

    s16 d_one_over_z_dx_16_times_2 = d_one_over_z_dx_16<<1;
    s16 d_u_over_z_dx_16_times_2 = d_u_over_z_dx_16<<1;

    if(cnt & 1) {
        // 4 divisions and 4 multiplies
        // could be 2 divisions and 2 multiplies
        
        // for small texture strips this can help performance a bit probably
        //u16 one_over_z_16_span_end = one_over_z_16 + d_one_over_z_dx_16_times_2;
        //u16 u_over_z_16_span_end = u_over_z_16 + d_u_over_z_dx_16_times_2;
        u16 one_over_z_16_span_end = one_over_z_16;
        u16 u_over_z_16_span_end = u_over_z_16;
        __asm volatile(
            "add.w %2, %0\t\n\
             add.w %3, %1"
            : "+d" (one_over_z_16_span_end), "+d" (u_over_z_16_span_end)
            : "d" (d_one_over_z_dx_16_times_2), "d" (d_u_over_z_dx_16_times_2));

        //u16 z_end = (1<<16)/one_over_z_16_span_end;
        u16 z_end = z_recip_table_16[one_over_z_16_span_end];
        
        u16 u_end = u_over_z_16_span_end * z_end;
        
        u16 u_per_col = u_end;
        __asm volatile(  
            "sub.w %1, %0\t\n\
            asr.w #1, %0"
            : "+d" (u_per_col)
            : "d" (u_start)
        );
        //u16 u_per_col = (u_end - u_start)>>1;
        //u16 u_16 = u_start;

        AFFINE_TEXMAP_16_ITER;
        AFFINE_TEXMAP_16_ITER;
        u_over_z_16 = u_over_z_16_span_end;
        one_over_z_16 = one_over_z_16_span_end;
        z_start = z_end;
        u_start = u_end;
    }
    
    cnt >>= 1;

    s16 d_one_over_z_dx_16_times_4 = d_one_over_z_dx_16_times_2<<1;
    s16 d_u_over_z_dx_16_times_4 = d_u_over_z_dx_16_times_2<<1;

    while(cnt--) {
        // 4 divisions and 4 multiplies
        // could be 2 divisions and 2 multiplies
        
        // for small texture strips this can help performance a bit probably
        //u16 one_over_z_16_span_end = one_over_z_16 + d_one_over_z_dx_16_times_4;
        //u16 u_over_z_16_span_end = u_over_z_16 + d_u_over_z_dx_16_times_4;
        u16 one_over_z_16_span_end = one_over_z_16;
        u16 u_over_z_16_span_end = u_over_z_16;
        __asm volatile(
            "add.w %2, %0\t\n\
             add.w %3, %1"
            : "+d" (one_over_z_16_span_end), "+d" (u_over_z_16_span_end)
            : "d" (d_one_over_z_dx_16_times_4), "d" (d_u_over_z_dx_16_times_4));

        //u16 z_end = (1<<16)/one_over_z_16_span_end;
        u16 z_end = z_recip_table_16[one_over_z_16_span_end];
        
        u16 u_end = u_over_z_16_span_end * z_end;

        u16 u_per_col = u_end;
        //u16 u_per_col = (u_end - u_start)>>2;
        __asm volatile(  
            "sub.w %1, %0\t\n\
            asr.w #2, %0"
            : "+d" (u_per_col)
            : "d" (u_start)
        );
        
        //u16 u_16 = u_start;

        AFFINE_TEXMAP_16_ITER;
        AFFINE_TEXMAP_16_ITER;
        AFFINE_TEXMAP_16_ITER;
        AFFINE_TEXMAP_16_ITER;
        u_over_z_16 = u_over_z_16_span_end;
        one_over_z_16 = one_over_z_16_span_end;
        z_start = z_end;
        u_start = u_end;
    }
    
    return;
    */

    /*
    if(cnt & 1) {
        TEXMAP_ITER;
    }
    cnt >>= 1;

    u32 z_start = z_start = (1<<26)/one_over_z_26;
    u32 u_start = u_over_z_23 * z_start;

    s32 d_one_over_z_dx_26_times_2 = d_one_over_z_dx_26<<1;
    s32 d_u_over_z_dx_23_times_2 = d_u_over_z_dx_23<<1;
    
    if(cnt & 1) {
        // 4 divisions and 4 multiplies
        // could be 2 divisions and 2 multiplies
        
        // for small texture strips this can help performance a bit probably
        u32 one_over_z_26_span_end = one_over_z_26 + d_one_over_z_dx_26_times_2;
        u32 u_over_z_23_span_end = u_over_z_23 + d_u_over_z_dx_23_times_2;
        u32 z_end = (1<<26)/one_over_z_26_span_end;
        
        u32 u_end = u_over_z_23_span_end * z_end;

        u32 u_per_col = (u_end - u_start)>>1;
        u32 u_23 = u_start;

        AFFINE_TEXMAP_ITER;
        AFFINE_TEXMAP_ITER;
        u_over_z_23 = u_over_z_23_span_end;
        one_over_z_26 = one_over_z_26_span_end;
        z_start = z_end;
        u_start = u_end;
    }
    cnt >>= 1;

    s32 d_one_over_z_dx_26_times_4 = d_one_over_z_dx_26_times_2<<1;
    s32 d_u_over_z_dx_23_times_4 = d_u_over_z_dx_23_times_2<<1;
    if(cnt & 1) {
        // 4 divisions and 4 multiplies
        // could be 2 divisions and 2 multiplies
        
        // for small texture strips this can help performance a bit probably
        u32 one_over_z_26_span_end = one_over_z_26 + d_one_over_z_dx_26_times_4;
        u32 u_over_z_23_span_end = u_over_z_23 + d_u_over_z_dx_23_times_4;
        u32 z_end = (1<<26)/one_over_z_26_span_end;
        
        u32 u_end = u_over_z_23_span_end * z_end;

        u32 u_per_col = (u_end - u_start)>>2;
        u32 u_23 = u_start;

        AFFINE_TEXMAP_ITER;
        AFFINE_TEXMAP_ITER;
        AFFINE_TEXMAP_ITER;
        AFFINE_TEXMAP_ITER;
        u_over_z_23 = u_over_z_23_span_end;
        one_over_z_26 = one_over_z_26_span_end;
        z_start = z_end;
        u_start = u_end;
    }
    cnt >>= 1;

    s32 d_one_over_z_dx_26_times_8 = d_one_over_z_dx_26_times_4<<1;
    s32 d_u_over_z_dx_23_times_8 = d_u_over_z_dx_23_times_4<<1;

    while(cnt--) {
        // 400 cycles per division, 200 cycles per multiply
        // 600 cycles per column

        // 1/8th div per column, 1/8 mult per column
        // 50 + 50, 100 cycles per column
        // removes 8 divisions and 8 multiplies, adds 1 divisions and 2 multiplies 
        // 2 divs, 2 mults setup

        // maybe 38,400 cycles down to 6400

        u32 one_over_z_26_span_end = one_over_z_26 + d_one_over_z_dx_26_times_8;
        u32 u_over_z_23_span_end = u_over_z_23 + d_u_over_z_dx_23_times_8;
        //u32 z_start = (1<<26)/one_over_z_26;
        u32 z_end = (1<<26)/one_over_z_26_span_end;
        
        //u32 u_start = u_over_z_23 * z_start;
        u32 u_end = u_over_z_23_span_end * z_end;

        u32 u_per_col = (u_end - u_start)>>3;
        u32 u_23 = u_start;

        AFFINE_TEXMAP_ITER;  // 0 divs, 0 mults
        AFFINE_TEXMAP_ITER;  // 0 divs, 0 mults
        AFFINE_TEXMAP_ITER;  // 0 divs, 0 mults
        AFFINE_TEXMAP_ITER;  // 0 divs, 0 mults
        AFFINE_TEXMAP_ITER;  // 0 divs, 0 mults
        AFFINE_TEXMAP_ITER;  // 0 divs, 0 mults
        AFFINE_TEXMAP_ITER;  // 0 divs, 0 mults
        AFFINE_TEXMAP_ITER;  // 0 divs, 0 mults

        u_over_z_23 = u_over_z_23_span_end;
        one_over_z_26 = one_over_z_26_span_end;
        z_start = z_end;
        u_start = u_end;
    }
    */
}

typedef enum {
    //FADE, 
    DARK=0, 
    MID=1, 
    LIGHT=2, 
} calc_light_level;

u8 num_light_levels;
u8 light_levels[6];

// TODO: add a check first for <= FIX_0_16_INV_FADE_DIST?
// requires modifying the draw loops below to handle another light level case
// for solid walls, drawing 0 is sufficient, and for textured walls, 
// using a different column drawing loop which draws constant 0 pixels should be sufficient

//    if ((inv_z) <= FIX_0_16_INV_FADE_DIST) {        
//        light_var = FADE;     

#define CHECK_DIST_FADE(inv_z, light_var) do {      \
    if ((inv_z) <= FIX_0_16_INV_FADE_DIST) {        \
        light_var = FADE;                           \
    } else if ((inv_z) <= FIX_0_16_INV_DARK_DIST) { \
        light_var = DARK;                           \
    } else if ((inv_z) <= FIX_0_16_INV_MID_DIST) {  \
        light_var = MID;                            \
    } else {                                        \
        light_var = LIGHT;                          \
    }                                               \
} while(0);

#define CHECK_DIST_WITH_MID(inv_z, light_var) do {  \
    if ((inv_z) <= FIX_0_16_INV_DARK_DIST) {        \
        light_var = DARK;                           \
    } else if ((inv_z) <= FIX_0_16_INV_MID_DIST) { \
        light_var = MID;                           \
    } else {                                        \
        light_var = LIGHT;                          \
    }                                               \
} while(0);

#define CHECK_DIST(inv_z, light_var) do {           \
    if ((inv_z) <= FIX_0_16_INV_DARK_DIST) {    \
        light_var = DARK;                           \
    } else {                                        \
        light_var = LIGHT;                          \
    }                                               \
} while(0);

void calculate_light_levels_for_wall(u32 clipped_dx, s16 inv_z1, s16 inv_z2, s16 fix_inv_dz_per_dx, s8 light_level) {
    num_light_levels = 0;
    u8* out_ptr = light_levels;
    s32 cnt = clipped_dx;
    s16 cur_inv_z = inv_z1;
    u8 cur_light;
    CHECK_DIST(cur_inv_z, cur_light);
     




    // TODO: for distance fading, we can't use this shortcut
    // we have to calculate per-column distance for ALL rendered walls
    // this here will cause far off walls with light level -2 or +2 to re-appear suddenly
    
    //if(0) { 
    if (light_level == -2 || light_level == 2) { 
        // z stays the same
        //output_light_span(cur_light, cnt);
        *out_ptr++ = (light_level == -2) ? DARK : LIGHT; //cur_light; //MID;
        *out_ptr++ = cnt; //cur_light;
        //*out_ptr++ = cnt;
        num_light_levels = 1;
        return;
    }
   

    #ifndef WALLS_DIST_LIGHTING
        // just average lighting
        //s32 avg_z = (inv_z1+inv_z2)>>1;
        //CHECK_DIST(avg_z, cur_light);
        
        *out_ptr++ = LIGHT; //cur_light;
        *out_ptr++ = cnt;
        num_light_levels = 1;
        return;
    #endif

    if(fix_inv_dz_per_dx == 0) {
        // z stays the same
        //output_light_span(cur_light, cnt);
        *out_ptr++ = cur_light;
        *out_ptr++ = cnt; //cur_light;
        //*out_ptr++ = cnt;
        num_light_levels = 1;

        return;
    }

    s16 end_inv_z = inv_z2; // + (cnt * fix_inv_dz_per_dx);
    u8 end_light;
    CHECK_DIST(end_inv_z, end_light);


    u8 cur_span = 0;


   #define OUTPUT_SPAN(nxt) do {\
    __asm volatile(     \
        "move.b %1, (%0)+\t\n\
        move.b %2, (%0)+"\
        : "+a" (out_ptr)\
        : "d" (cur_light), "d" (cur_span)\
    );                                  \
    cur_light = nxt;  \
    cur_span = 1;           \
   } while(0) \



    while(cnt--) {
        u8 next_light; 
        
        CHECK_DIST(cur_inv_z, next_light);

        if(cur_light == next_light) {
            cur_span++;
        } else {
            __asm volatile(
                "move.b %1, (%0)+\t\n\
                move.b %3, (%0)+\t\n\
                move.b %4, %1\t\n\
                moveq #1, %2"
                : "+a" (out_ptr), "+d" (cur_light), "=d" (cur_span)
                : "d" (cur_span), "d" (next_light)
            );

            // we only have two light levels, so we can just quit now
            cur_span += cnt;
            break;
        }
        
        cur_inv_z += fix_inv_dz_per_dx;
    }
    
   
    __asm volatile(
        "move.b %1, (%0)+\t\n\
        move.b %2, (%0)+"
        : "+a" (out_ptr)
        : "d" (cur_light), "d" (cur_span)
    );
    //*out_ptr++ = cur_light;
    //*out_ptr++ = cur_span;

    num_light_levels = (out_ptr-light_levels)>>1;
    
}


void draw_upper_step(s16 x1, s16 x1_ytop, s16 nx1_ytop, s16 x2, s16 x2_ytop, s16 nx2_ytop, 
                     u16 inv_z1, u16 inv_z2,
                     u16 window_min, u16 window_max, u8 upper_color, s8 light_level, light_params* params) {


    u16 far_inv_z = min(inv_z1, inv_z2);
    draw_lit_plane_fp ceil_func = &draw_lit_ceil_light_only;
    #ifdef FLATS_LIGHTING
    if(far_inv_z <= FIX_0_16_INV_MID_DIST) {
        ceil_func = &draw_lit_ceiling;
    }
    #endif

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

    s16 inv_dz = inv_z2 - inv_z1;
    s16 fix_inv_dz_per_dx = inv_dz / dx;

    s32 cur_fix_inv_z = inv_z1;

    s16 skip_x = beginx - x1;

    if(skip_x > 0) {
        top_y_fix += (skip_x * top_dy_per_dx);
        ntop_y_fix += (skip_x * ntop_dy_per_dx);
        cur_fix_inv_z += (skip_x * fix_inv_dz_per_dx);
    }


    s16 endx = min(window_max, x2);

    s16 x = beginx;
    u16 cnt = endx-x;
    u8* yclip_ptr = &(yclip[x<<1]);
    //u8* drawn_buf_ptr = &(drawn_buf[x]);
    
    calculate_light_levels_for_wall(cnt, cur_fix_inv_z, inv_z2, fix_inv_dz_per_dx, light_level);

    u32 dark_color = get_dark_color(upper_color, light_level);
    //u32 mid_color = get_mid_dark_color(upper_color, light_level);
    u32 light_color = get_light_color(upper_color, light_level);

    u16* offset_ptr = (&buf_column_offset_table[x]);

    u8* light_ptr = light_levels;
    for(int i = 0; i < num_light_levels; i++) {
        u8 light_level = *light_ptr++;
        u8 light_cnt = *light_ptr++;
        u32 color;
        switch(light_level) {
            //case FADE:
            //    color = 0;
            //    break;
            case DARK:
                color = dark_color;
                break;
            case MID:
                //color = mid_color;
                //break;
            case LIGHT:
                color = light_color;
                break;
        }
        while(light_cnt--) {
            top_y_int = top_y_fix >> 16;
            ntop_y_int = ntop_y_fix >> 16;
            u8 min_drawable_y = *yclip_ptr;
            u8 max_drawable_y = *(yclip_ptr+1);


            //u8* col_ptr = *offset_ptr++;  
            u8* col_ptr = GET_COLUMN_PTR(offset_ptr, bmp_buffer_write);
            u8 col_drawn = is_column_drawn(min_drawable_y, max_drawable_y);

            if(!col_drawn) {
                u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);
                u8 bot_draw_y = clamp(ntop_y_int, min_drawable_y, max_drawable_y);
                //u8 clip_top = max(top_draw_y, bot_draw_y);
                if(min_drawable_y < top_draw_y) {
                    ceil_func(min_drawable_y, top_draw_y, col_ptr, params);
                    *yclip_ptr = top_draw_y;
                }
                if(top_draw_y < bot_draw_y) {
                    draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, color, col_ptr);
                    *yclip_ptr = bot_draw_y;
                }
            }
            yclip_ptr += 2;

            top_y_fix += top_dy_per_dx;
            ntop_y_fix += ntop_dy_per_dx;
        }

    }
    
    flip();

    return; 
}


void draw_top_pegged_textured_upper_step(s16 x1, s16 x1_ytop, s16 nx1_ytop, s16 x2, s16 x2_ytop, s16 nx2_ytop,
                                         u16 z1_12_4, u16 z2_12_4,
                                         u16 inv_z1, u16 inv_z2,
                                         u16 window_min, u16 window_max, s8 light_level,
                                         texmap_params* tmap_info, 
                                         light_params* params, 
                                         s16 x1_pegged_top, s16 x2_pegged_top) {
          

    u16 far_inv_z = min(inv_z1, inv_z2);
    draw_lit_plane_fp ceil_func = &draw_lit_ceil_light_only;
    #ifdef FLATS_LIGHTING
    if(far_inv_z <= FIX_0_16_INV_MID_DIST) {
        ceil_func = &draw_lit_ceiling;
    }
    #endif

    // 4 subpixel bits here
    s16 top_dy_fix = x2_ytop - x1_ytop;
    s16 ntop_dy_fix = nx2_ytop - nx1_ytop;

    s16 pegged_top_dy_fix = x2_pegged_top - x1_pegged_top;

    s16 dx = x2-x1;

    fix32 top_dy_per_dx = (top_dy_fix<<12) / dx;
    fix32 ntop_dy_per_dx = (ntop_dy_fix<<12) / dx;
    fix32 pegged_top_dy_per_dx = (pegged_top_dy_fix<<12) / dx;

    fix32 top_y_fix = x1_ytop<<12; 
    fix32 ntop_y_fix = nx1_ytop<<12;
    fix32 pegged_top_y_fix = x1_pegged_top<<12;

    s16 top_y_int;
    s16 ntop_y_int;

    s16 beginx = max(x1, window_min);
    s16 endx = min(window_max, x2);

    s16 inv_dz = inv_z2 - inv_z1;
    s32 fix_inv_dz_per_dx = (inv_dz) / dx;

    s32 cur_fix_inv_z = inv_z1;

    s16 skip_x = beginx - x1;

    if(skip_x > 0) {
        top_y_fix += (skip_x * top_dy_per_dx);
        ntop_y_fix += (skip_x * ntop_dy_per_dx);
        pegged_top_y_fix += (skip_x * pegged_top_dy_per_dx);
        cur_fix_inv_z += (skip_x * fix_inv_dz_per_dx);
    }

    calculate_tex_coords_for_wall(beginx, endx, skip_x, dx,
        z1_12_4, z2_12_4, inv_z1, inv_z2, tmap_info);

    lit_texture* lit_tex = tmap_info->tex;
    const u16* dark_tex = lit_tex->dark;
    //u16* mid_tex = lit_tex->mid;
    const u16* light_tex = lit_tex->light;


    s16 x = beginx;
    u16 cnt = endx-x;
    u8* yclip_ptr = &(yclip[x<<1]);
    
    u16* offset_ptr = (&buf_column_offset_table[x]);

    u16* tex_col_ptr = tex_col_buffer;

    while(cnt--) {
        top_y_int = top_y_fix >> 16;
        ntop_y_int = ntop_y_fix >> 16;
        u8 min_drawable_y = *yclip_ptr;
        u8 max_drawable_y = *(yclip_ptr+1);


        //u8* col_ptr = *offset_ptr++;
        u8* col_ptr = GET_COLUMN_PTR(offset_ptr, bmp_buffer_write);

        u8 col_drawn = is_column_drawn(min_drawable_y, max_drawable_y);
        u16 one_over_z_16 = *tex_col_ptr++;
        u16 tex_idx = *tex_col_ptr++;
        if(!col_drawn) {
            u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);
            u8 bot_draw_y = clamp(ntop_y_int, min_drawable_y, max_drawable_y);
            if(top_draw_y < bot_draw_y) {
                s16 pegged_top_y_int = pegged_top_y_fix>>16;
                const u16* tex_column;
                if (light_level == -2 || one_over_z_16 <= FIX_0_16_INV_DARK_DIST) {
                    tex_column = &dark_tex[tex_idx];
                } else {
                    tex_column = &light_tex[tex_idx];
                }
                if(bot_draw_y == ntop_y_int) {
                    draw_texture_vertical_line(pegged_top_y_int, top_draw_y, ntop_y_int, col_ptr, tex_column);
                } else {
                    draw_bottom_clipped_texture_vertical_line(pegged_top_y_int, top_draw_y, ntop_y_int, bot_draw_y, col_ptr, tex_column);
                }

                *yclip_ptr++ = bot_draw_y;
                yclip_ptr++;
            } else {
                yclip_ptr += 2;
            }
            if(min_drawable_y < top_draw_y) {
                ceil_func(min_drawable_y, top_draw_y, col_ptr, params);
            }
        } else {
            yclip_ptr+=2;
        }

        top_y_fix += top_dy_per_dx;
        ntop_y_fix += ntop_dy_per_dx;
        pegged_top_y_fix += pegged_top_dy_per_dx;
    }
    
    flip();

    return; 
}



void draw_bottom_pegged_textured_lower_step(
    s16 x1, s16 x1_ybot, s16 nx1_ybot, s16 x2, s16 x2_ybot, s16 nx2_ybot, 
    u16 z1_12_4,     u16 z2_12_4,
    u16 inv_z1, u16 inv_z2,
    u16 window_min, u16 window_max, s8 light_level,
    texmap_params* tmap_info, light_params* params,
    s16 x1_pegged_bot, s16 x2_pegged_bot) {
    
    u16 far_inv_z = min(inv_z1, inv_z2);
    draw_lit_plane_fp floor_func = &draw_lit_floor_light_only;
    #ifdef FLATS_LIGHTING
    if(far_inv_z <= FIX_0_16_INV_MID_DIST) {
        floor_func = &draw_lit_floor;
    }
    #endif

    s16 bot_dy_fix = x2_ybot - x1_ybot;
    s16 nbot_dy_fix = nx2_ybot - nx1_ybot;

    s16 pegged_bot_dy_fix = x2_pegged_bot - x1_pegged_bot;

    s16 dx = x2-x1;

    fix32 bot_dy_per_dx = (bot_dy_fix<<12) / dx;
    fix32 nbot_dy_per_dx = (nbot_dy_fix<<12) / dx;
    fix32 pegged_bot_dy_per_dx = (pegged_bot_dy_fix<<12) / dx;

    fix32 bot_y_fix = x1_ybot<<12; 
    fix32 nbot_y_fix = nx1_ybot<<12;
    fix32 pegged_bot_y_fix = x1_pegged_bot<<12;

    s16 bot_y_int;
    s16 nbot_y_int;

    s16 beginx = max(x1, window_min);
    s16 endx = min(window_max, x2);

    s16 inv_dz = inv_z2 - inv_z1;
    s16 fix_inv_dz_per_dx = (inv_dz) / dx; //(inv_dz << 16) / dx;

    s16 cur_fix_inv_z = inv_z1; //inv_z1<<16;
    
   

    s16 skip_x = beginx - x1;
    if(skip_x > 0) {
        bot_y_fix += (skip_x * bot_dy_per_dx);
        nbot_y_fix += (skip_x * nbot_dy_per_dx);
        pegged_bot_y_fix += (skip_x * pegged_bot_dy_per_dx);
        cur_fix_inv_z += (skip_x * fix_inv_dz_per_dx);
    }

    calculate_tex_coords_for_wall(beginx, endx, skip_x, dx,
        z1_12_4, z2_12_4, inv_z1, inv_z2, tmap_info);


    lit_texture* lit_tex = tmap_info->tex;
    const u16* dark_tex = lit_tex->dark;
    //u16* mid_tex = lit_tex->mid;
    const u16* light_tex = lit_tex->light;

    s16 x = beginx;
    u16 cnt = endx-x;
    u8* yclip_ptr = &(yclip[x<<1]);

    u16* offset_ptr = (&buf_column_offset_table[x]);

    u16* tex_col_ptr = tex_col_buffer;

    while(cnt--) {
        bot_y_int = bot_y_fix >> 16;
        nbot_y_int = nbot_y_fix >> 16;
        u8 min_drawable_y = *yclip_ptr;
        u8 max_drawable_y = *(yclip_ptr+1);


        //u8* col_ptr = *offset_ptr++;
        u8* col_ptr = GET_COLUMN_PTR(offset_ptr, bmp_buffer_write);
        u8 col_drawn = is_column_drawn(min_drawable_y, max_drawable_y);
        u16 one_over_z_16 = *tex_col_ptr++;
        u16 tex_idx = *tex_col_ptr++;
        if(!col_drawn) {
            u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);
            u8 top_draw_y = clamp(nbot_y_int, min_drawable_y, max_drawable_y);
            if(top_draw_y < bot_draw_y) {
                s16 pegged_bot_y_int = pegged_bot_y_fix>>16;
                
                const u16* tex_column;
                if (light_level == -2 || one_over_z_16 <= FIX_0_16_INV_DARK_DIST) {
                    tex_column = &dark_tex[tex_idx];
                } else {
                    tex_column = &light_tex[tex_idx];
                }
                
                if(pegged_bot_y_int == bot_draw_y) {
                    draw_texture_vertical_line(nbot_y_int, top_draw_y, bot_draw_y, col_ptr, tex_column);
                } else {
                    draw_bottom_clipped_texture_vertical_line(nbot_y_int, top_draw_y, pegged_bot_y_int, bot_draw_y, col_ptr, tex_column);
                }

                yclip_ptr++;
                *yclip_ptr++ = top_draw_y;
            } else {
                yclip_ptr += 2;
            }
            if(max_drawable_y > bot_draw_y) {
                floor_func(bot_draw_y, max_drawable_y, col_ptr, params);
            }
        } else {
            yclip_ptr += 2;
        }

        bot_y_fix += bot_dy_per_dx;
        nbot_y_fix += nbot_dy_per_dx;
        pegged_bot_y_fix += pegged_bot_dy_per_dx;
    }
    
    flip();

    return; 
}


void draw_lower_step(s16 x1, s16 x1_ybot, s16 nx1_ybot, s16 x2, s16 x2_ybot, s16 nx2_ybot, 
                     u16 inv_z1, u16 inv_z2,
                     u16 window_min, u16 window_max, u8 lower_color, s8 light_level, light_params* params) {
    
    u16 far_inv_z = min(inv_z1, inv_z2);
    draw_lit_plane_fp floor_func = &draw_lit_floor_light_only;
    
    #ifdef FLATS_LIGHTING
    if(far_inv_z <= FIX_0_16_INV_MID_DIST) {
        floor_func = &draw_lit_floor;
    }
    #endif

    s16 bot_dy_fix = x2_ybot - x1_ybot;
    s16 nbot_dy_fix = nx2_ybot - nx1_ybot;

    s16 dx = x2-x1;

    //fix32 bot_dy_per_dx = (bot_dy_fix<<12) / dx;
    //fix32 nbot_dy_per_dx = (nbot_dy_fix<<12) / dx;
    //u16 dx_recip_16 = z_recip_table_16[dx];  
    //fix32 bot_dy_per_dx = muls_16_by_16(bot_dy_fix, dx_recip_16)>>4;
    //fix32 nbot_dy_per_dx = muls_16_by_16(nbot_dy_fix, dx_recip_16)>>4;
    fix16 bot_dy_per_dx_8 = divs_32_by_16((bot_dy_fix<<4), dx);
    fix16 nbot_dy_per_dx_8 = divs_32_by_16(nbot_dy_fix<<4, dx);
    fix32 bot_dy_per_dx = bot_dy_per_dx_8<<8;
    fix32 nbot_dy_per_dx = nbot_dy_per_dx_8<<8; 

    fix32 bot_y_fix = x1_ybot<<12; 
    fix32 nbot_y_fix = nx1_ybot<<12;
    s16 bot_y_int;
    s16 nbot_y_int;

    s16 beginx = max(x1, window_min);

    s16 inv_dz = inv_z2 - inv_z1;
    s16 fix_inv_dz_per_dx = (inv_dz) / dx; //(inv_dz << 16) / dx;

    s16 cur_fix_inv_z = inv_z1; //inv_z1<<16;


    s16 skip_x = beginx - x1;
    if(skip_x > 0) {
        // TODO causes inaccuracy issues
        //bot_y_fix += muls_16_by_16(skip_x, bot_dy_per_dx_8)<<8;
        //nbot_y_fix += muls_16_by_16(skip_x, nbot_dy_per_dx_8)<<8;
        bot_y_fix += (skip_x * bot_dy_per_dx);
        nbot_y_fix += (skip_x * nbot_dy_per_dx);
        if(light_level > -2 && light_level < 2) {
            cur_fix_inv_z += (skip_x * fix_inv_dz_per_dx);
        }
    }
    

    s16 endx = min(window_max, x2);

    s16 x = beginx;
    u8* yclip_ptr = &(yclip[x<<1]);
    //u8* drawn_buf_ptr = &(drawn_buf[x]);

    u32 dark_color = get_dark_color(lower_color, light_level);
    //u32 mid_color = get_mid_dark_color(lower_color, light_level);
    u32 light_color = get_light_color(lower_color, light_level);


    u16 cnt = endx-x;
    calculate_light_levels_for_wall(cnt, cur_fix_inv_z, inv_z2, fix_inv_dz_per_dx, light_level);

    u16* offset_ptr = (&buf_column_offset_table[x]);

    u8* light_ptr = light_levels;
        for(int i = 0; i < num_light_levels; i++) {
        u8 light_level = *light_ptr++;
        u8 light_cnt = *light_ptr++;
        u32 color;
        
        switch(light_level) {
            //case FADE:
            //    color = 0;
            //    break;
            case DARK:
                color = dark_color;
                break;
            case MID:
            //    color = mid_color;
            //    break;
            case LIGHT:
                color = light_color;
                break;
        }
        while(light_cnt--) {
            bot_y_int = bot_y_fix >> 16;
            nbot_y_int = nbot_y_fix >> 16;
            u8 min_drawable_y = *yclip_ptr;
            u8 max_drawable_y = *(yclip_ptr+1);


            //u8* col_ptr = *offset_ptr++;
            u8* col_ptr = GET_COLUMN_PTR(offset_ptr, bmp_buffer_write);
            u8 col_drawn = is_column_drawn(min_drawable_y, max_drawable_y);
            if(!col_drawn) {
                u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);
                u8 top_draw_y = clamp(nbot_y_int, min_drawable_y, max_drawable_y);
                if(max_drawable_y > bot_draw_y) {
                    floor_func(bot_draw_y, max_drawable_y, col_ptr, params);
                    *(yclip_ptr+1) = bot_draw_y;
                }
                if(top_draw_y < bot_draw_y) {
                    draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, color, col_ptr);

                    //draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, lower_color, col_ptr);
                    
                }
                *(yclip_ptr+1) = min(top_draw_y, bot_draw_y);
            }
            yclip_ptr += 2;


            bot_y_fix += bot_dy_per_dx;
            nbot_y_fix += nbot_dy_per_dx;
        }
    }
    
    flip();

    return; 
}



void draw_ceiling_update_clip(s16 x1, s16 x1_ytop, s16 x2, s16 x2_ytop,
                              u16 far_inv_z,
                              u16 window_min, u16 window_max, light_params* params) {
      
    draw_lit_plane_fp ceil_func = &draw_lit_ceil_light_only;
    #ifdef FLATS_LIGHTING
    if(far_inv_z <= FIX_0_16_INV_MID_DIST) {
        ceil_func = &draw_lit_ceiling;
    }
    #endif


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

    int cnt = endx-beginx;

    u16* offset_ptr = (&buf_column_offset_table[x]);

    while(cnt--) {
        top_y_int = top_y_fix >> 16;
        u8 min_drawable_y = *yclip_ptr;
        u8 max_drawable_y = *(yclip_ptr+1);
        u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);

        //u8* col_ptr = *offset_ptr++;
        u8* col_ptr = GET_COLUMN_PTR(offset_ptr, bmp_buffer_write);

        u8 col_drawn = is_column_drawn(min_drawable_y, max_drawable_y);

        if(min_drawable_y < top_draw_y && !col_drawn) {
            ceil_func(min_drawable_y, top_draw_y, col_ptr, params);
            *yclip_ptr = top_draw_y;
        } else {
            //yclip_ptr += 2;
        }
        //*yclip_ptr++ = top_draw_y;
        //yclip_ptr++;
        yclip_ptr += 2;

        top_y_fix += top_dy_per_dx;
    }
    
    flip();

    return; 
}
                            

void draw_floor_update_clip(s16 x1, s16 x1_ybot, s16 x2, s16 x2_ybot, 
                            u16 far_inv_z,
                            u16 window_min, u16 window_max, light_params* params) {             


    draw_lit_plane_fp floor_func = &draw_lit_floor_light_only;
    
    #ifdef FLATS_LIGHTING
    if(far_inv_z <= FIX_0_16_INV_MID_DIST) {
        floor_func = &draw_lit_floor;
    }
    #endif
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

    u16* offset_ptr = (&buf_column_offset_table[x]);

    for(;x < endx; x++) {
        bot_y_int = bot_y_fix >> 16;
        u8 min_drawable_y = *yclip_ptr;
        u8 max_drawable_y = *(yclip_ptr+1);
        //if(min_drawable_y >= max_drawable_y) { continue; }
        
        u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);

        //u8* col_ptr = *offset_ptr++;
        u8* col_ptr = GET_COLUMN_PTR(offset_ptr, bmp_buffer_write);
        u8 col_drawn = is_column_drawn(min_drawable_y, max_drawable_y);
        

        if(max_drawable_y > bot_draw_y && !col_drawn) {
            floor_func(bot_draw_y, max_drawable_y, col_ptr, params);
            *(yclip_ptr+1) = bot_draw_y;

        }
        yclip_ptr += 2;

        bot_y_fix += bot_dy_per_dx;
    }
    
    flip();

    return; 
}



void draw_solid_color_wall(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 inv_z1, u16 inv_z2,
              u16 window_min, u16 window_max,  s8 light_level, 
              u8 wall_color,
              light_params* floor_params, light_params* ceil_params) {

    
    u16 far_inv_z = min(inv_z1, inv_z2);
    const u8 light_floor_ceil = (far_inv_z <= FIX_0_16_INV_MID_DIST);
    draw_lit_plane_fp floor_func = &draw_lit_floor_light_only;
    draw_lit_plane_fp ceil_func = &draw_lit_ceil_light_only;
    #ifdef FLATS_LIGHTING
    if(light_floor_ceil) {
        floor_func = &draw_lit_floor;
        ceil_func = &draw_lit_ceiling;
    }
    #endif


    // 4 subpixel bits here
    s16 top_dy_fix, bot_dy_fix;
    fix32 top_y_fix, bot_y_fix;
    top_dy_fix = x2_ytop - x1_ytop;
    bot_dy_fix = x2_ybot - x1_ybot;
    top_y_fix = x1_ytop<<12; 
    bot_y_fix = x1_ybot<<12;
    s16 dx = x2-x1;

    //z_recip_table_16
    //u16 dx_recip_16 = z_recip_table_16[dx]; 
    
    //fix32 top_dy_per_dx = (top_dy_fix<<12) / dx;
    //fix32 bot_dy_per_dx = (bot_dy_fix<<12) / dx; 
    fix16 top_dy_per_dx_8 = divs_32_by_16((top_dy_fix<<4), dx);
    fix16 bot_dy_per_dx_8 = divs_32_by_16(bot_dy_fix<<4, dx);
    fix32 top_dy_per_dx = top_dy_per_dx_8<<8;
    fix32 bot_dy_per_dx = bot_dy_per_dx_8<<8; 
    //fix32 top_dy_per_dx = muls_16_by_16(top_dy_fix, dx_recip_16);
    //fix32 bot_dy_per_dx = muls_16_by_16(bot_dy_fix, dx_recip_16);

    
    s16 top_y_int;
    s16 bot_y_int;


    s16 inv_dz = inv_z2 - inv_z1;
    
    s16 fix_inv_dz_per_dx = divs_32_by_16(inv_dz, dx);// inv_dz / dx;

    s32 cur_fix_inv_z = inv_z1;

    s16 beginx = max(x1, window_min);
    s16 endx = min(window_max, x2);
    s16 skip_x = beginx - x1;

    

    if(skip_x > 0) {
        //top_y_fix += muls_16_by_16(skip_x, top_dy_per_dx_8)<<8;
        //bot_y_fix += muls_16_by_16(skip_x, bot_dy_per_dx_8)<<8;
        top_y_fix += (skip_x * top_dy_per_dx);
        bot_y_fix += (skip_x * bot_dy_per_dx);
        if(light_level > -2 && light_level < 2) {
            //cur_fix_inv_z += (skip_x * fix_inv_dz_per_dx);
            cur_fix_inv_z += muls_16_by_16(skip_x, fix_inv_dz_per_dx);
        }
    }
    
    s16 x = beginx;
    u16 cnt = endx-x;

    u16* offset_ptr = (&buf_column_offset_table[x]);
    
    u8* yclip_ptr = &(yclip[(x<<1)]);

    calculate_light_levels_for_wall(cnt, cur_fix_inv_z, inv_z2, fix_inv_dz_per_dx, light_level);
    
    u32 light_color = get_light_color(wall_color, light_level);
    //u32 mid_color = get_mid_dark_color(wall_color, light_level);
    u32 dark_color = get_dark_color(wall_color, light_level);


    u8* light_ptr = light_levels;
    for(int i = 0; i < num_light_levels; i++) {
        u8 light_level = *light_ptr++;
        u8 light_cnt = *light_ptr++;
        u32 color;
        
        switch(light_level) {
            //case FADE:
            //    color = 0;
            //    break;
            case DARK:
                color = dark_color;
                break;
            case MID:
            //    color = mid_color;
            //    break;
            case LIGHT:
                color = light_color;
                break;
        }
        while(light_cnt--) {
            top_y_int = top_y_fix >> 16;
            bot_y_int = bot_y_fix >> 16;

            //if(min_drawable_y >= max_drawable_y) { continue; }

            
            //u8* col_ptr = *offset_ptr++;
            u8* col_ptr = GET_COLUMN_PTR(offset_ptr, bmp_buffer_write);
            u8 min_drawable_y = *yclip_ptr++;
            u8 max_drawable_y = *yclip_ptr++;

            u8 col_drawn = is_column_drawn(min_drawable_y, max_drawable_y);
            if(!col_drawn) {
                u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);
                u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);
                if(min_drawable_y < top_draw_y) {
                    ceil_func(min_drawable_y, top_draw_y, col_ptr, ceil_params);
                }
                if(top_draw_y < bot_draw_y) {
                    draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, color, col_ptr);
                }
                if(bot_draw_y < max_drawable_y) {
                    floor_func(bot_draw_y, max_drawable_y, col_ptr, floor_params);
                }
                set_column_drawn(yclip_ptr);
            }
            top_y_fix += top_dy_per_dx;
            bot_y_fix += bot_dy_per_dx;
        }
    }
    
    flip();

    return; 
}

inline u16* get_tex_column(const u16* light_tex, const u16* dark_tex, const u16 tex_idx, const u16 one_over_z_16, const s8 light_level) {
    #ifndef WALLS_DIST_LIGHTING
        return &light_tex[tex_idx];
    #else 
        if (light_level == -2 || one_over_z_16 <= FIX_0_16_INV_DARK_DIST) {
            return &dark_tex[tex_idx];
        } else {
            return &light_tex[tex_idx];
        }
    #endif
}

void draw_wall(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 z1_12_4,     u16 z2_12_4,
              u16 inv_z1, u16 inv_z2,
              u16 window_min, u16 window_max, s8 light_level, 
              texmap_params *tmap_info, 
              light_params* floor_params, light_params* ceil_params) {



    // 4 subpixel bits here
    s16 top_dy_fix, bot_dy_fix;
    fix32 top_y_fix, bot_y_fix;
    top_dy_fix = x2_ytop - x1_ytop;
    bot_dy_fix = x2_ybot - x1_ybot;
    top_y_fix = x1_ytop<<12; 
    bot_y_fix = x1_ybot<<12;
    u16 dx = x2-x1;

    fix32 top_dy_per_dx = (top_dy_fix<<12) / dx; // 16.16 / 16 -> 16.16 
    fix32 bot_dy_per_dx = (bot_dy_fix<<12) / dx;
    
    //fix16 top_dy_per_dx_8 = divs_32_by_16((top_dy_fix), dx);
    //fix16 bot_dy_per_dx_8 = divs_32_by_16(bot_dy_fix, dx);
    //fix32 top_dy_per_dx = top_dy_per_dx_8<<8;
    //fix32 bot_dy_per_dx = bot_dy_per_dx_8<<8; 
    
    s16 top_y_int;
    s16 bot_y_int;


    s16 inv_dz = inv_z2 - inv_z1;
    
    //s32 fix_inv_dz_per_dx = inv_dz / dx;
    
    //s16 fix_inv_dz_per_dx = inv_dz/dx;
    //s16 fix_inv_dz_per_dx = divs_32_by_16(inv_dz, dx);// inv_dz / dx;


    //s32 cur_fix_inv_z = inv_z1;

    s16 beginx = max(x1, window_min);
    s16 endx = min(window_max, x2);
    s16 skip_x = beginx - x1;

    if(beginx >= endx) { return; }

    if(skip_x > 0) {
        //top_y_fix += muls_16_by_16(skip_x, top_dy_per_dx_8)<<8;
        //bot_y_fix += muls_16_by_16(skip_x, bot_dy_per_dx_8)<<8;
        top_y_fix += (skip_x * top_dy_per_dx);
        bot_y_fix += (skip_x * bot_dy_per_dx);
        //cur_fix_inv_z += (skip_x * fix_inv_dz_per_dx);
        
        //if(light_level > -2 && light_level < 2) {
        //    cur_fix_inv_z += muls_16_by_16(skip_x, fix_inv_dz_per_dx);
        //}
    }

    calculate_tex_coords_for_wall(beginx, endx, skip_x, dx,
        z1_12_4, z2_12_4, inv_z1, inv_z2, tmap_info);
        

    lit_texture* lit_tex = tmap_info->tex;
    const u16* dark_tex = lit_tex->dark;
    //u16* mid_tex = lit_tex->mid;
    const u16* light_tex = lit_tex->light;

    s16 x = beginx;
    u16 cnt = endx-x;

    u16* offset_ptr = (&buf_column_offset_table[x]);

    u8* yclip_ptr = &(yclip[(x<<1)]);

    //u32* tex_col_ptr = tex_col_buffer;
    u16* tex_col_ptr = tex_col_buffer;

    
    //u16 far_inv_z = min(inv_z1, inv_z2);
    //draw_lit_plane_fp floor_func = &draw_lit_floor_light_only;
    //draw_lit_plane_fp ceil_func = &draw_lit_ceil_light_only;
    #ifdef FLATS_LIGHTING
    const u8 light_floor_ceil = ((inv_z1 <= FIX_0_16_INV_MID_DIST) || (inv_z2 <= FIX_0_16_INV_MID_DIST));
    #else 
    const u8 light_floor_ceil = 0;
    #endif

    //u8* light_ptr = light_levels;
        
    const u16* base_tex = light_tex;
    if(light_floor_ceil) {
        while(cnt--) {
            top_y_int = top_y_fix >> 16;
            bot_y_int = bot_y_fix >> 16;
            
            //u8* col_ptr = *offset_ptr++;
            u8* col_ptr = GET_COLUMN_PTR(offset_ptr, bmp_buffer_write);
            u8 min_drawable_y = *yclip_ptr++;
            u8 max_drawable_y = *yclip_ptr++;
            u8 col_drawn = is_column_drawn(min_drawable_y, max_drawable_y);

            top_y_fix += top_dy_per_dx;
            bot_y_fix += bot_dy_per_dx;
            
            u16 one_over_z_16 = *tex_col_ptr++;
            u16 tex_idx = *tex_col_ptr++;

            if(!col_drawn) {
                if(min_drawable_y >= max_drawable_y) {
                    __builtin_unreachable();
                }
                set_column_drawn(yclip_ptr);
                s16 ctop = min_drawable_y;
                s16 cbot = top_y_int;
                if(ctop < cbot) {
                    if(max_drawable_y < ctop) {
                        // bottom fully clips ceiling
                        continue;
                    } else {
                        // bottom might partially clip ceiling
                        cbot = (cbot <= max_drawable_y ? cbot : max_drawable_y);
                        draw_lit_ceiling(ctop, cbot, col_ptr, ceil_params);
                        //ceil_func(ctop, cbot, col_ptr, ceil_params);
                    }
                }

                s16 ftop = bot_y_int;
                s16 fbot = max_drawable_y;
                if(ftop < fbot) {
                    if(min_drawable_y >= fbot) {
                        // top fully clips floor
                        continue;
                    } else {
                        // top partially clips floor
                        ftop = (ftop >= min_drawable_y ? ftop : min_drawable_y);
                        draw_lit_floor(ftop, fbot, col_ptr, floor_params);
                        //floor_func(ftop, fbot, col_ptr, floor_params);
                    }
                }


                s16 wtop = max(top_y_int, min_drawable_y);
                s16 wbot = min(bot_y_int, max_drawable_y);
                //wtop = (wtop >= min_drawable_y ? wtop : min_drawable_y);
                //wbot = (wbot <= max_drawable_y ? wbot : max_drawable_y);

                if(wtop < wbot) {
                    if(max_drawable_y <= wtop) {
                        // fully clips wall
                    } else if (min_drawable_y >= wbot) {
                        // fully clips wall
                    //} else if (fade) {
                    //    //draw_native_vertical_line_unrolled(wtop, wbot, 0, col_ptr);
                    } else {
                        const u16* tex_column = get_tex_column(light_tex, dark_tex, tex_idx, light_level, one_over_z_16);

                        if(bot_y_int == wbot) {
                            draw_texture_vertical_line(top_y_int, wtop, bot_y_int, col_ptr, tex_column);
                        } else {
                            draw_bottom_clipped_texture_vertical_line(top_y_int, wtop, bot_y_int, wbot, col_ptr, tex_column);
                        }   
                    }
                }
            }
        }
    } else {
        
        while(cnt--) {
            top_y_int = top_y_fix >> 16;
            bot_y_int = bot_y_fix >> 16;


            //u8* col_ptr = *offset_ptr++;
            u8* col_ptr = GET_COLUMN_PTR(offset_ptr, bmp_buffer_write);

            u8 min_drawable_y = *yclip_ptr++;
            u8 max_drawable_y = *yclip_ptr++;
            u8 col_drawn = is_column_drawn(min_drawable_y, max_drawable_y);

            top_y_fix += top_dy_per_dx;
            bot_y_fix += bot_dy_per_dx;

            u16 one_over_z_16 = *tex_col_ptr++;
            u16 tex_idx = *tex_col_ptr++;

            if(!col_drawn) {
                set_column_drawn(yclip_ptr);
                s16 ctop = min_drawable_y;
                s16 cbot = top_y_int;
                if(ctop < cbot) {
                    if(max_drawable_y < ctop) {
                        // bottom fully clips ceiling
                        continue;
                    } else {
                        // bottom might partially clip ceiling
                        cbot = (cbot <= max_drawable_y ? cbot : max_drawable_y);
                        draw_lit_ceil_light_only(ctop, cbot, col_ptr, ceil_params);
                    }
                }

                s16 ftop = bot_y_int;
                s16 fbot = max_drawable_y;
                if(ftop < fbot) {
                    if(min_drawable_y >= fbot) {
                        // top fully clips floor
                        continue;
                    } else {
                        // top partially clips floor
                        ftop = (ftop >= min_drawable_y ? ftop : min_drawable_y);
                        draw_lit_floor_light_only(ftop, fbot, col_ptr, floor_params);

                    }
                }


                s16 wtop = top_y_int;
                s16 wbot = bot_y_int;
                wtop = (wtop >= min_drawable_y ? wtop : min_drawable_y);
                wbot = (wbot <= max_drawable_y ? wbot : max_drawable_y);

                if(wtop < wbot) {
                    if(max_drawable_y <= wtop) {
                        // fully clips wall
                    } else if (min_drawable_y >= wbot) {
                        // fully clips wall
                    //} else if (fade) { 
                    //    //draw_native_vertical_line_unrolled(wtop, wbot, 0, col_ptr);
                    } else {      
                        const u16* tex_column = get_tex_column(light_tex, dark_tex, tex_idx, one_over_z_16, light_level);
 
                        if(bot_y_int == wbot) {
                            draw_texture_vertical_line(top_y_int, wtop, bot_y_int, col_ptr, tex_column);
                        } else {
                            draw_bottom_clipped_texture_vertical_line(top_y_int, wtop, bot_y_int, wbot, col_ptr, tex_column);
                        }   
                    }
                }
            }

        }
    }    

    flip();

    return; 
}


void draw_top_pegged_wall(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 z1_12_4,     u16 z2_12_4,
              u16 inv_z1, u16 inv_z2,
              u16 window_min, u16 window_max, s8 light_level,
              texmap_params *tmap_info, 
              light_params* floor_params, light_params* ceil_params,
              s16 x1_pegged_top, s16 x2_pegged_top) {


    u16 far_inv_z = min(inv_z1, inv_z2);
 
    draw_lit_plane_fp floor_func = &draw_lit_floor_light_only;
    draw_lit_plane_fp ceil_func = &draw_lit_ceil_light_only;
    #ifdef FLATS_LIGHTING
    if(far_inv_z <= FIX_0_16_INV_MID_DIST) {
        floor_func = &draw_lit_floor;
    }
    if(far_inv_z <= FIX_0_16_INV_MID_DIST) {
        ceil_func = &draw_lit_ceiling;
    }
    #endif

    // 4 subpixel bits here
    s16 top_dy_fix, bot_dy_fix;
    fix32 top_y_fix, bot_y_fix;
    top_dy_fix = x2_ytop - x1_ytop;
    bot_dy_fix = x2_ybot - x1_ybot;

    s16 pegged_top_dy_fix = x2_pegged_top - x1_pegged_top;

    top_y_fix = x1_ytop<<12; 
    bot_y_fix = x1_ybot<<12;
    fix32 pegged_top_y_fix = x1_pegged_top<<12;
    s16 dx = x2-x1;

    fix32 top_dy_per_dx = (top_dy_fix<<12) / dx; // 16.16 / 16 -> 16.16 
    fix32 bot_dy_per_dx = (bot_dy_fix<<12) / dx;
    fix32 pegged_top_dy_per_dx = (pegged_top_dy_fix<<12) / dx;
    
    s16 top_y_int;
    s16 bot_y_int;


    s16 inv_dz = inv_z2 - inv_z1;
    
    s32 fix_inv_dz_per_dx = (inv_dz) / dx;

    s32 cur_fix_inv_z = inv_z1;
    

    s16 beginx = max(x1, window_min);
    s16 endx = min(window_max, x2);
    s16 skip_x = beginx - x1;
    

    if(skip_x > 0) {
        top_y_fix += (skip_x * top_dy_per_dx);
        bot_y_fix += (skip_x * bot_dy_per_dx);
        pegged_top_y_fix += (skip_x * pegged_top_dy_per_dx);
        cur_fix_inv_z += (skip_x * fix_inv_dz_per_dx);
    }

    calculate_tex_coords_for_wall(beginx, endx, skip_x, dx,
        z1_12_4, z2_12_4, inv_z1, inv_z2, tmap_info);


    lit_texture* lit_tex = tmap_info->tex;
    const u16* dark_tex = lit_tex->dark;
    //u16* mid_tex = lit_tex->mid;
    const u16* light_tex = lit_tex->light;

    s16 x = beginx;
    u16 cnt = endx-x;

    u16* offset_ptr = (&buf_column_offset_table[x]);

    u8* yclip_ptr = &(yclip[(x<<1)]);

    u16* tex_col_ptr = tex_col_buffer;

    while(cnt--) {
        top_y_int = top_y_fix >> 16;
        bot_y_int = bot_y_fix >> 16;

        u8 min_drawable_y = *yclip_ptr++;
        u8 max_drawable_y = *yclip_ptr++;


        //u8* col_ptr = *offset_ptr++;
        u8* col_ptr = GET_COLUMN_PTR(offset_ptr, bmp_buffer_write);
        u8 col_drawn = is_column_drawn(min_drawable_y, max_drawable_y);
            
        u16 one_over_z_16 = *tex_col_ptr++;
        u16 tex_idx = *tex_col_ptr++;

        if(!col_drawn) {
            u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);
            u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);
            if(min_drawable_y < top_draw_y) {
                ceil_func(min_drawable_y, top_draw_y, col_ptr, ceil_params);
            }
            if(top_draw_y < bot_draw_y) {
                s16 pegged_top_y_int = pegged_top_y_fix>>16;

                const u16* tex_column;
                if (light_level == -2 || one_over_z_16 <= FIX_0_16_INV_DARK_DIST) {
                    tex_column = &dark_tex[tex_idx];
                } else {
                    tex_column = &light_tex[tex_idx];
                }
                if(bot_y_int == bot_draw_y) {
                    draw_texture_vertical_line(pegged_top_y_int, top_draw_y, bot_y_int, col_ptr, tex_column);
                } else {
                    draw_bottom_clipped_texture_vertical_line(pegged_top_y_int, top_draw_y, bot_y_int, bot_draw_y, col_ptr, tex_column);
                }
            }
            if(bot_draw_y < max_drawable_y) {
                floor_func(bot_draw_y, max_drawable_y, col_ptr, floor_params);
            }
            set_column_drawn(yclip_ptr);
        }

        top_y_fix += top_dy_per_dx;
        bot_y_fix += bot_dy_per_dx;
        pegged_top_y_fix += pegged_top_dy_per_dx;
    }
    

    flip();

    return; 
}


void draw_bot_pegged_wall(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 z1_12_4,     u16 z2_12_4,
              u16 inv_z1, u16 inv_z2,
              u16 window_min, u16 window_max, s8 light_level,
              texmap_params *tmap_info,  
              light_params* floor_params, light_params* ceil_params,
              s16 x1_pegged_bot, s16 x2_pegged_bot) {

    u16 far_inv_z = min(inv_z1, inv_z2);
 
    draw_lit_plane_fp floor_func = &draw_lit_floor_light_only;
    draw_lit_plane_fp ceil_func = &draw_lit_ceil_light_only;
    #ifdef FLATS_LIGHTING
    if(far_inv_z <= FIX_0_16_INV_MID_DIST) {
        floor_func = &draw_lit_floor;
    }
    if(far_inv_z <= FIX_0_16_INV_MID_DIST) {
        ceil_func = &draw_lit_ceiling;
    }
    #endif

    // 4 subpixel bits here
    s16 top_dy_fix, bot_dy_fix;
    fix32 top_y_fix, bot_y_fix;
    top_dy_fix = x2_ytop - x1_ytop;
    bot_dy_fix = x2_ybot - x1_ybot;

    s16 pegged_bot_dy_fix = x2_pegged_bot - x1_pegged_bot;

    top_y_fix = x1_ytop<<12; 
    bot_y_fix = x1_ybot<<12;
    fix32 pegged_bot_y_fix = x1_pegged_bot<<12;
    s16 dx = x2-x1;

    fix32 top_dy_per_dx = (top_dy_fix<<12) / dx; // 16.16 / 16 -> 16.16 
    fix32 bot_dy_per_dx = (bot_dy_fix<<12) / dx;
    fix32 pegged_bot_dy_per_dx = (pegged_bot_dy_fix<<12) / dx;
    
    s16 top_y_int;
    s16 bot_y_int;


    
    //s16 dz_12_4 = z2_12_4-z1_12_4;
    s16 inv_dz = inv_z2 - inv_z1;
    
    s32 fix_inv_dz_per_dx = (inv_dz) / dx;

    s32 cur_fix_inv_z = inv_z1;


    s16 beginx = max(x1, window_min);
    s16 endx = min(window_max, x2);
    s16 skip_x = beginx - x1;
    

    if(skip_x > 0) {
        top_y_fix += (skip_x * top_dy_per_dx);
        bot_y_fix += (skip_x * bot_dy_per_dx);
        pegged_bot_y_fix += (skip_x * pegged_bot_dy_per_dx);
        cur_fix_inv_z += (skip_x * fix_inv_dz_per_dx);
    }

    calculate_tex_coords_for_wall(beginx, endx, skip_x, dx,
        z1_12_4, z2_12_4, inv_z1, inv_z2, tmap_info);


    lit_texture* lit_tex = tmap_info->tex;
    const u16* dark_tex = lit_tex->dark;
    //u16* mid_tex = lit_tex->mid;
    const u16* light_tex = lit_tex->light;

    s16 x = beginx;
    u16 cnt = endx-x;

    u16* offset_ptr = (&buf_column_offset_table[x]);

    u8* yclip_ptr = &(yclip[(x<<1)]);

    u16* tex_col_ptr = tex_col_buffer;

    while(cnt--) {
        top_y_int = top_y_fix >> 16;
        bot_y_int = bot_y_fix >> 16;

        u8 min_drawable_y = *yclip_ptr++;
        u8 max_drawable_y = *yclip_ptr++;
        
        u8 col_drawn = is_column_drawn(min_drawable_y, max_drawable_y);


        
        //u8* col_ptr = *offset_ptr++;
        u8* col_ptr = GET_COLUMN_PTR(offset_ptr, bmp_buffer_write);
        u16 one_over_z_16 = *tex_col_ptr++;
        u16 tex_idx = *tex_col_ptr++;
        if(!col_drawn) {
            u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);
            u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);
            if(min_drawable_y < top_draw_y) {
                ceil_func(min_drawable_y, top_draw_y, col_ptr, ceil_params);
            }
            if(top_draw_y < bot_draw_y) {
                s16 pegged_bot_y_int = pegged_bot_y_fix>>16;

                const u16* tex_column;
                if (light_level == -2 || one_over_z_16 <= FIX_0_16_INV_DARK_DIST) {
                    tex_column = &dark_tex[tex_idx];
                } else {
                    tex_column = &light_tex[tex_idx];
                }
                if(pegged_bot_y_int == bot_draw_y) {
                    draw_texture_vertical_line(top_y_int, top_draw_y, pegged_bot_y_int, col_ptr, tex_column);
                } else {
                    draw_bottom_clipped_texture_vertical_line(top_y_int, top_draw_y, pegged_bot_y_int, bot_draw_y, col_ptr, tex_column);
                }
            }
            if(bot_draw_y < max_drawable_y) {
                floor_func(bot_draw_y, max_drawable_y, col_ptr, floor_params);
            }
            set_column_drawn(yclip_ptr);
        }
        
        top_y_fix += top_dy_per_dx;
        bot_y_fix += bot_dy_per_dx;
        pegged_bot_y_fix += pegged_bot_dy_per_dx;
    }

    

    flip();

    return; 
}