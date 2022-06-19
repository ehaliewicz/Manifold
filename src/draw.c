#include <genesis.h>
#include <kdebug.h>
#include "my_bmp.h"
#include "clip_buf.h"
#include "colors.h"
#include "counter.h"
#include "div_lut.h"
#include "draw.h"
#include "game.h"
#include "level.h"
#include "math3d.h"
#include "portal.h"
#include "texture.h"
#include "textures.h"
#include "utils.h"


u8* yclip;
u8* drawn_buf;
u8** bmp_ptr_buf;


u8** buf_0_column_offset_table;
u8** buf_1_column_offset_table;

void init_column_offset_table() {
  // offset from last column

  for(int x = 0; x < SCREEN_WIDTH/2; x++) {  
    u16 cur_off = bmp_get_dma_write_offset(x<<1, 0);
    buf_0_column_offset_table[x] = bmp_buffer_0 + cur_off;
    buf_1_column_offset_table[x] = bmp_buffer_1 + cur_off;
  }
}

s16* raster_buf;//[RENDER_WIDTH*2];

void init_2d_buffers() {
    yclip = MEM_alloc(RENDER_WIDTH*2); //256); //SCREEN_WIDTH*2*sizeof(u8));
    bmp_ptr_buf = MEM_alloc(sizeof(u8*) * SCREEN_WIDTH/2);
    
    drawn_buf = MEM_alloc(RENDER_WIDTH);

    raster_buf = MEM_alloc(RENDER_WIDTH*2*2);

    buf_0_column_offset_table = MEM_alloc(sizeof(u8*) * SCREEN_WIDTH/2);
    buf_1_column_offset_table = MEM_alloc(sizeof(u8*) * SCREEN_WIDTH/2);
    init_column_offset_table();
}


void clear_2d_buffers() {
    u8* yclip_ptr = yclip;
    u8* drawn_buf_ptr = drawn_buf;

    u32 u32val = (SCREEN_HEIGHT << 16) | SCREEN_HEIGHT;

    // 128 bytes need to be moved
    
    if(bmp_buffer_write == bmp_buffer_0) {
        memcpy(bmp_ptr_buf, buf_0_column_offset_table, RENDER_WIDTH*sizeof(u8*));
    } else {
        memcpy(bmp_ptr_buf, buf_1_column_offset_table, RENDER_WIDTH*sizeof(u8*));
    }
    
    //u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    
    
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
        move.l %1, (%0)+\t\n\
         "
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

void copy_2d_buffer(u16 left, u16 right, clip_buf* dest) {
    int bytes_to_copy = ((right+1)-left)*2;
    memcpy(&(dest->y_clip_buffer[left<<1]), &yclip[left<<1], bytes_to_copy); //bytes_to_copy); //128);
}

void release_2d_buffers() {
    MEM_free(yclip);
    MEM_free(drawn_buf);
    MEM_free(buf_1_column_offset_table);
    MEM_free(buf_1_column_offset_table);
    MEM_free(raster_buf);
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

void vline_native_movel(s16 dy, u32 col, u8* col_ptr);



u8* draw_native_double_vertical_line_unrolled(s16 y0, s16 y1, s16 y2, u32 full_col1, u32 full_col2, u8* col_ptr) {
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

u8* draw_native_vertical_line_unrolled(s16 y0, s16 y1, u32 full_col,  u8* col_ptr) {


    col_ptr = col_ptr + (y0<<1);

    u16 word_col = full_col;

    u16 dy = (y1-y0);
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

    
    return col_ptr;
}


#define FLAT_COLOR

int debug_draw_cleared = 0;

int last_pressed_a = 0;
void flip() {  
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



/* for drawing moving objects */
void draw_masked(s16 x1, s16 x1_ytop, s16 x1_ybot,
                            s16 x2, s16 x2_ytop, s16 x2_ybot,
                            u16 window_min, u16 window_max,
                            clip_buf* clipping_buffer,
                            u8 wall_col) {

    return;
    // 4 subpixel bits here
    s16 top_dy_fix = x2_ytop - x1_ytop;
    s16 bot_dy_fix = x2_ybot - x1_ybot;

    s16 dx = x2-x1;

    //fix32 top_dy_per_dx = (top_dy_fix<<12) / dx; // (dy_fix<<4) / dx; // 22.10
    //fix32 bot_dy_per_dx = (bot_dy_fix<<12) / dx;

    fix32 top_y_fix = x1_ytop<<12; 
    fix32 bot_y_fix = x1_ybot<<12;
    s16 top_y_int = top_y_fix >> 16;
    s16 bot_y_int = bot_y_fix >> 16;
    //KLog_S1("top_y_int: ", top_y_int);
    //KLog_S1("bot_y_int: ", bot_y_int);
    //u16 height = bot_y_int-top_y_int;

    s16 beginx = max(x1, window_min);

    s16 skip_x = beginx - x1;



    s16 endx = min(window_max, x2);
    u32 u_per_x = (1<<16)/dx;
    u32 u_fix = 0;

    if(skip_x > 0) {
        //top_y_fix += (skip_x * top_dy_per_dx);
        //bot_y_fix += (skip_x * bot_dy_per_dx);
        //tex_col_fix +=  (skip_x * tex_per_pix_fix);'
        u_fix += (skip_x * u_per_x);
    }

    //while(1) { }
    //u32 full_col = long_color_table[wall_col];

    s16 x = beginx;
    u8* yclip_ptr = &(clipping_buffer->y_clip_buffer[x<<1]);

    //u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    u8** offset_ptr = bmp_ptr_buf[x];



    //u16* tex = raw_key_mid;
    const u16* tex = raw_key_32_32_mid;
    u32 y_per_texels_fix = ((bot_y_int - top_y_int)<<16)/ 64;
    //u32 texels_per_y_fix = (64<<16) / (bot_y_int-top_y_int);

    for(;x < endx; x++) {
        u8 min_drawable_y = *yclip_ptr++;
        u8 max_drawable_y = *yclip_ptr++;
        //if(min_drawable_y >= max_drawable_y) { continue; }

        u8 top_draw_y = min_drawable_y; 
        u8 bot_draw_y = max_drawable_y; 

        u8* col_ptr = *offset_ptr++;
        u8 col_drawn = (col_ptr == NULL);
        if(!col_drawn) {
            if(top_draw_y < bot_draw_y) {
                //top_y_int = top_y_fix >> 16;
                //bot_y_int = bot_y_fix >> 16;
                u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);
                u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);
                
                u32 clipped_y_pix_top, clipped_y_pix_bot;
                u8 skipped_texels;
                u8 skipped_bot_texels = 33;
                clipped_y_pix_bot = (y_per_texels_fix * skipped_bot_texels) >> 16;

                // if there are visible pixels in this column
                if(top_draw_y < bot_draw_y) {

                    // get texture column 
                    u32 tex_col = ((u_fix<<TEX_WIDTH_SHIFT)>>16);
                    u16 tex_idx = tex_col<<TEX_HEIGHT_SHIFT;
                    u16* tex_column = &tex[tex_idx];

                    // this sprite only has 28 columns of non-transparent pixels :)
                    if(tex_col >= 27) { 
                        break;
                    } else if (tex_col >= 24) {
                        // last few columns skip 29 texels
                        skipped_texels = 29;

                        clipped_y_pix_top = (y_per_texels_fix * skipped_texels)>>16;
                    } else if (tex_col < 2) {
                        // first few columns skip 9 texels
                        skipped_texels = 9;
                        clipped_y_pix_top = (y_per_texels_fix * skipped_texels)>>16;
                    } else {
                        skipped_texels = 0;
                        clipped_y_pix_top = 0;
                    }
                    //KLog_S1("top y int: ", top_y_int);
                    //KLog_U1("top_draw_y: ", top_draw_y);
                    //KLog_U1("top_draw_y+clipped_y_pix: ", top_draw_y+clipped_y_pix);
                    //KLog_S1("bot_y_int: ", bot_y_int);
                    //KLog_U1("bot_draw_y: ", bot_draw_y);
                    draw_bottom_clipped_texture_vertical_line(top_y_int, top_draw_y+clipped_y_pix_top, bot_y_int, bot_draw_y-clipped_y_pix_bot, col_ptr, tex_column);

                }

                u_fix += u_per_x;
            }
        }

        //top_y_fix += top_dy_per_dx;
        //bot_y_fix += bot_dy_per_dx;
    }
    
    //flip();

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

    //u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    u8** offset_ptr = bmp_ptr_buf;

    for(;x < endx; x++) {
        u8 min_drawable_y = *yclip_ptr++;
        u8 max_drawable_y = *yclip_ptr++;
        //if(min_drawable_y >= max_drawable_y) { continue; }
        u8 top_draw_y = min_drawable_y; 
        u8 bot_draw_y = max_drawable_y; 

        u8* col_ptr = *offset_ptr++;
        if(top_draw_y < bot_draw_y) {
            draw_native_vertical_transparent_line_unrolled(top_draw_y, bot_draw_y, wall_col, col_ptr, x&1);
            
        }
    }

    
    //flip();

    return; 
}



const s16 floor_light_positions[512*4] = {
    -1, -1, -1, -1, 
    62, 63, 66, 68, 
    68, 69, 71, 73, 
    70, 71, 73, 75, 
    71, 72, 74, 76, 
    72, 72, 74, 76, 
    72, 73, 75, 76, 
    72, 73, 75, 77, 72, 73, 75, 77, 72, 73, 75, 77, 73, 73, 75, 77, 73, 74, 76, 78, 73, 74, 76, 78, 73, 74, 77, 79, 73, 75, 77, 80, 74, 75, 78, 80, 74, 75, 78, 81, 74, 75, 79, 82, 74, 76, 79, 82, 74, 76, 80, 83, 75, 76, 80, 84, 75, 77, 81, 84, 75, 77, 81, 85, 75, 77, 82, 86, 75, 77, 82, 86, 76, 78, 83, 87, 76, 78, 83, 88, 76, 78, 83, 88, 76, 79, 84, 89, 76, 79, 84, 89, 77, 79, 85, 90, 77, 79, 85, 91, 77, 80, 86, 91, 77, 80, 86, 92, 77, 80, 87, 93, 78, 81, 87, 93, 78, 81, 88, 94, 78, 81, 88, 95, 78, 81, 89, 95, 79, 82, 89, 96, 79, 82, 90, 97, 79, 82, 90, 97, 79, 83, 91, 98, 79, 83, 91, 99, 80, 83, 92, 99, 80, 83, 92, 100, 80, 84, 93, 101, 80, 84, 93, 101, 80, 84, 94, 102, 81, 85, 94, 103, 81, 85, 95, 103, 81, 85, 95, 104, 81, 85, 95, 105, 81, 86, 96, 105, 82, 86, 96, 106, 82, 86, 97, 107, 82, 87, 97, 107, 82, 87, 98, 108, 82, 87, 98, 108, 83, 87, 99, 109, 83, 88, 99, 110, 83, 88, 100, 110, 83, 88, 100, 111, 83, 89, 101, 112, 84, 89, 101, 112, 84, 89, 102, 113, 84, 90, 102, 114, 84, 90, 103, 114, 84, 90, 103, 115, 85, 90, 104, 116, 85, 91, 104, 116, 85, 91, 105, 117, 85, 91, 105, 118, 86, 92, 106, 118, 86, 92, 106, 119, 86, 92, 107, 120, 86, 92, 107, 120, 86, 93, 107, 121, 87, 93, 108, 122, 87, 93, 108, 122, 87, 94, 109, 123, 87, 94, 109, 124, 87, 94, 110, 124, 88, 94, 110, 125, 88, 95, 111, 125, 88, 95, 111, 126, 88, 95, 112, 127, 88, 96, 112, 127, 89, 96, 113, 128, 89, 96, 113, 129, 89, 96, 114, 129, 89, 97, 114, 130, 89, 97, 115, 131, 90, 97, 115, 131, 90, 98, 116, 132, 90, 98, 116, 133, 90, 98, 117, 133, 90, 98, 117, 134, 91, 99, 118, 135, 91, 99, 118, 135, 91, 99, 119, 136, 91, 100, 119, 137, 91, 100, 119, 137, 92, 100, 120, 138, 92, 100, 120, 139, 92, 101, 121, 139, 92, 101, 121, 140, 93, 101, 122, 141, 93, 102, 122, 141, 93, 102, 123, 142, 93, 102, 123, 143, 93, 102, 124, 143, 94, 103, 124, 144, 94, 103, 125, 144, 94, 103, 125, 144, 94, 104, 126, 144, 94, 104, 126, 144, 95, 104, 127, 144, 95, 104, 127, 144, 95, 105, 128, 144, 95, 105, 128, 144, 95, 105, 129, 144, 96, 106, 129, 144, 96, 106, 130, 144, 96, 106, 130, 144, 96, 107, 131, 144, 96, 107, 131, 144, 97, 107, 131, 144, 97, 107, 132, 144, 97, 108, 132, 144, 97, 108, 133, 144, 97, 108, 133, 144, 98, 109, 134, 144, 98, 109, 134, 144, 98, 109, 135, 144, 98, 109, 135, 144, 98, 110, 136, 144, 99, 110, 136, 144, 99, 110, 137, 144, 99, 111, 137, 144, 99, 111, 138, 144, 100, 111, 138, 144, 100, 111, 139, 144, 100, 112, 139, 144, 100, 112, 140, 144, 100, 112, 140, 144, 101, 113, 141, 144, 101, 113, 141, 144, 101, 113, 142, 144, 101, 113, 142, 144, 101, 114, 143, 144, 102, 114, 143, 144, 102, 114, 143, 144, 102, 115, 144, 144, 102, 115, 144, 144, 102, 115, 144, 144, 103, 115, 144, 144, 103, 116, 144, 144, 103, 116, 144, 144, 103, 116, 144, 144, 103, 117, 144, 144, 104, 117, 144, 144, 104, 117, 144, 144, 104, 117, 144, 144, 104, 118, 144, 144, 104, 118, 144, 144, 105, 118, 144, 144, 105, 119, 144, 144, 105, 119, 144, 144, 105, 119, 144, 144, 105, 119, 144, 144, 106, 120, 144, 144, 106, 120, 144, 144, 106, 120, 144, 144, 106, 121, 144, 144, 107, 121, 144, 144, 107, 121, 144, 144, 107, 121, 144, 144, 107, 122, 144, 144, 107, 122, 144, 144, 108, 122, 144, 144, 108, 123, 144, 144, 108, 123, 144, 144, 108, 123, 144, 144, 108, 123, 144, 144, 109, 124, 144, 144, 109, 124, 144, 144, 109, 124, 144, 144, 109, 125, 144, 144, 109, 125, 144, 144, 110, 125, 144, 144, 110, 126, 144, 144, 110, 126, 144, 144, 110, 126, 144, 144, 110, 126, 144, 144, 111, 127, 144, 144, 111, 127, 144, 144, 111, 127, 144, 144, 111, 128, 144, 144, 111, 128, 144, 144, 112, 128, 144, 144, 112, 128, 144, 144, 112, 129, 144, 144, 112, 129, 144, 144, 112, 129, 144, 144, 113, 130, 144, 144, 113, 130, 144, 144, 113, 130, 144, 144, 113, 130, 144, 144, 113, 131, 144, 144, 114, 131, 144, 144, 114, 131, 144, 144, 114, 132, 144, 144, 114, 132, 144, 144, 115, 132, 144, 144, 115, 132, 144, 144, 115, 133, 144, 144, 115, 133, 144, 144, 115, 133, 144, 144, 116, 134, 144, 144, 116, 134, 144, 144, 116, 134, 144, 144, 116, 134, 144, 144, 116, 135, 144, 144, 117, 135, 144, 144, 117, 135, 144, 144, 117, 136, 144, 144, 117, 136, 144, 144, 117, 136, 144, 144, 118, 136, 144, 144, 118, 137, 144, 144, 118, 137, 144, 144, 118, 137, 144, 144, 118, 138, 144, 144, 119, 138, 144, 144, 119, 138, 144, 144, 119, 138, 144, 144, 119, 139, 144, 144, 119, 139, 144, 144, 120, 139, 144, 144, 120, 140, 144, 144, 120, 140, 144, 144, 120, 140, 144, 144, 120, 140, 144, 144, 121, 141, 144, 144, 121, 141, 144, 144, 121, 141, 144, 144, 121, 142, 144, 144, 122, 142, 144, 144, 122, 142, 144, 144, 122, 143, 144, 144, 122, 143, 144, 144, 122, 143, 144, 144, 123, 143, 144, 144, 123, 144, 144, 144, 123, 144, 144, 144, 123, 144, 144, 144, 123, 144, 144, 144, 124, 144, 144, 144, 124, 144, 144, 144, 124, 144, 144, 144, 124, 144, 144, 144, 124, 144, 144, 144, 125, 144, 144, 144, 125, 144, 144, 144, 125, 144, 144, 144, 125, 144, 144, 144, 125, 144, 144, 144, 126, 144, 144, 144, 126, 144, 144, 144, 126, 144, 144, 144, 126, 144, 144, 144, 126, 144, 144, 144, 127, 144, 144, 144, 127, 144, 144, 144, 127, 144, 144, 144, 127, 144, 144, 144, 127, 144, 144, 144, 128, 144, 144, 144, 128, 144, 144, 144, 128, 144, 144, 144, 128, 144, 144, 144, 129, 144, 144, 144, 129, 144, 144, 144, 129, 144, 144, 144, 129, 144, 144, 144, 129, 144, 144, 144, 130, 144, 144, 144, 130, 144, 144, 144, 130, 144, 144, 144, 130, 144, 144, 144, 130, 144, 144, 144, 131, 144, 144, 144, 131, 144, 144, 144, 131, 144, 144, 144, 131, 144, 144, 144, 131, 144, 144, 144, 132, 144, 144, 144, 132, 144, 144, 144, 132, 144, 144, 144, 132, 144, 144, 144, 132, 144, 144, 144, 133, 144, 144, 144, 133, 144, 144, 144, 133, 144, 144, 144, 133, 144, 144, 144, 133, 144, 144, 144, 134, 144, 144, 144, 134, 144, 144, 144, 134, 144, 144, 144, 134, 144, 144, 144, 134, 144, 144, 144, 135, 144, 144, 144, 135, 144, 144, 144, 135, 144, 144, 144, 135, 144, 144, 144, 136, 144, 144, 144, 136, 144, 144, 144, 136, 144, 144, 144, 136, 144, 144, 144, 136, 144, 144, 144, 137, 144, 144, 144, 137, 144, 144, 144, 137, 144, 144, 144, 137, 144, 144, 144, 137, 144, 144, 144, 138, 144, 144, 144, 138, 144, 144, 144, 138, 144, 144, 144, 138, 144, 144, 144, 138, 144, 144, 144, 139, 144, 144, 144, 139, 144, 144, 144, 139, 144, 144, 144, 139, 144, 144, 144, 139, 144, 144, 144, 140, 144, 144, 144, 140, 144, 144, 144, 140, 144, 144, 144, 140, 144, 144, 144, 140, 144, 144, 144, 141, 144, 144, 144, 141, 144, 144, 144, 141, 144, 144, 144, 141, 144, 144, 144, 141, 144, 144, 144, 142, 144, 144, 144, 142, 144, 144, 144, 142, 144, 144, 144, 142, 144, 144, 144, 143, 144, 144, 144, 143, 144, 144, 144, 143, 144, 144, 144, 143, 144, 144, 144, 143, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144};

const s16 ceil_light_positions[512*4] = {
    -1, -1, -1, -1, 73, 75, 78, 78, 68, 70, 72, 73, 66, 68, 70, 71, 65, 67, 69, 70, 65, 67, 69, 69, 65, 66, 68, 69, 64, 66, 68, 69, 64, 66, 68, 69, 64, 66, 68, 69, 64, 66, 68, 68, 63, 65, 67, 68, 63, 65, 67, 68, 62, 64, 67, 68, 61, 64, 66, 68, 61, 63, 66, 67, 60, 63, 66, 67, 59, 62, 66, 67, 59, 62, 65, 67, 58, 61, 65, 67, 57, 61, 65, 66, 57, 60, 64, 66, 56, 60, 64, 66, 55, 59, 64, 66, 55, 59, 64, 66, 54, 58, 63, 65, 53, 58, 63, 65, 53, 58, 63, 65, 52, 57, 62, 65, 52, 57, 62, 65, 51, 56, 62, 64, 50, 56, 62, 64, 50, 55, 61, 64, 49, 55, 61, 64, 48, 54, 61, 64, 48, 54, 60, 63, 47, 53, 60, 63, 46, 53, 60, 63, 46, 52, 60, 63, 45, 52, 59, 62, 44, 51, 59, 62, 44, 51, 59, 62, 43, 50, 58, 62, 42, 50, 58, 62, 42, 49, 58, 61, 41, 49, 58, 61, 40, 48, 57, 61, 40, 48, 57, 61, 39, 47, 57, 61, 38, 47, 56, 60, 38, 46, 56, 60, 37, 46, 56, 60, 36, 46, 56, 60, 36, 45, 55, 60, 35, 45, 55, 59, 35, 44, 55, 59, 34, 44, 54, 59, 33, 43, 54, 59, 33, 43, 54, 59, 32, 42, 54, 58, 31, 42, 53, 58, 31, 41, 53, 58, 30, 41, 53, 58, 29, 40, 52, 58, 29, 40, 52, 57, 28, 39, 52, 57, 27, 39, 51, 57, 27, 38, 51, 57, 26, 38, 51, 57, 25, 37, 51, 56, 25, 37, 50, 56, 24, 36, 50, 56, 23, 36, 50, 56, 23, 35, 49, 55, 22, 35, 49, 55, 21, 35, 49, 55, 21, 34, 49, 55, 20, 34, 48, 55, 19, 33, 48, 54, 19, 33, 48, 54, 18, 32, 47, 54, 17, 32, 47, 54, 17, 31, 47, 54, 16, 31, 47, 53, 16, 30, 46, 53, 15, 30, 46, 53, 14, 29, 46, 53, 14, 29, 45, 53, 13, 28, 45, 52, 12, 28, 45, 52, 12, 27, 45, 52, 11, 27, 44, 52, 10, 26, 44, 52, 10, 26, 44, 51, 9, 25, 43, 51, 8, 25, 43, 51, 8, 24, 43, 51, 7, 24, 43, 51, 6, 23, 42, 50, 6, 23, 42, 50, 5, 22, 42, 50, 4, 22, 41, 50, 4, 22, 41, 50, 3, 21, 41, 49, 2, 21, 41, 49, 2, 20, 40, 49, 1, 20, 40, 49, 0, 19, 40, 48, 0, 19, 39, 48, 0, 18, 39, 48, 0, 18, 39, 48, -1, 17, 39, 48, -2, 17, 38, 47, -2, 16, 38, 47, -3, 16, 38, 47, -4, 15, 37, 47, -4, 15, 37, 47, -5, 14, 37, 46, -6, 14, 37, 46, -6, 13, 36, 46, -7, 13, 36, 46, -8, 12, 36, 46, -8, 12, 35, 45, -9, 11, 35, 45, -10, 11, 35, 45, -10, 10, 35, 45, -11, 10, 34, 45, -12, 10, 34, 44, -12, 9, 34, 44, -13, 9, 33, 44, -14, 8, 33, 44, -14, 8, 33, 44, -15, 7, 32, 43, -16, 7, 32, 43, -16, 6, 32, 43, -17, 6, 32, 43, -18, 5, 31, 43, -18, 5, 31, 42, -19, 4, 31, 42, -19, 4, 30, 42, -20, 3, 30, 42, -21, 3, 30, 41, -21, 2, 30, 41, -22, 2, 29, 41, -23, 1, 29, 41, -23, 1, 29, 41, -24, 0, 28, 40, -25, 0, 28, 40, -25, 0, 28, 40, -26, 0, 28, 40, -27, -1, 27, 40, -27, -1, 27, 39, -28, -1, 27, 39, -29, -2, 26, 39, -29, -2, 26, 39, -30, -3, 26, 39, -31, -3, 26, 38, -31, -4, 25, 38, -32, -4, 25, 38, -33, -5, 25, 38, -33, -5, 24, 38, -34, -6, 24, 37, -35, -6, 24, 37, -35, -7, 24, 37, -36, -7, 23, 37, -36, -8, 23, 37, -37, -8, 23, 36, -38, -9, 22, 36, -38, -9, 22, 36, -39, -10, 22, 36, -40, -10, 22, 36, -40, -11, 21, 35, -41, -11, 21, 35, -42, -12, 21, 35, -42, -12, 20, 35, -43, -13, 20, 35, -44, -13, 20, 34, -44, -13, 20, 34, -45, -14, 19, 34, -46, -14, 19, 34, -46, -15, 19, 33, -47, -15, 18, 33, -48, -16, 18, 33, -48, -16, 18, 33, -49, -17, 18, 33, -50, -17, 17, 32, -50, -18, 17, 32, -51, -18, 17, 32, -52, -19, 16, 32, -52, -19, 16, 32, -53, -20, 16, 31, -54, -20, 15, 31, -54, -21, 15, 31, -55, -21, 15, 31, -55, -22, 15, 31, -56, -22, 14, 30, -57, -23, 14, 30, -57, -23, 14, 30, -58, -24, 13, 30, -59, -24, 13, 30, -59, -25, 13, 29, -60, -25, 13, 29, -61, -25, 12, 29, -61, -26, 12, 29, -62, -26, 12, 29, -63, -27, 11, 28, -63, -27, 11, 28, -64, -28, 11, 28, -65, -28, 11, 28, -65, -29, 10, 28, -66, -29, 10, 27, -67, -30, 10, 27, -67, -30, 9, 27, -68, -31, 9, 27, -69, -31, 9, 26, -69, -32, 9, 26, -70, -32, 8, 26, -71, -33, 8, 26, -71, -33, 8, 26, -72, -34, 7, 25, -72, -34, 7, 25, -73, -35, 7, 25, -74, -35, 7, 25, -74, -36, 6, 25, -75, -36, 6, 24, -76, -37, 6, 24, -76, -37, 5, 24, -77, -37, 5, 24, -78, -38, 5, 24, -78, -38, 5, 23, -79, -39, 4, 23, -80, -39, 4, 23, -80, -40, 4, 23, -81, -40, 3, 23, -82, -41, 3, 22, -82, -41, 3, 22, -83, -42, 3, 22, -84, -42, 2, 22, -84, -43, 2, 22, -85, -43, 2, 21, -86, -44, 1, 21, -86, -44, 1, 21, -87, -45, 1, 21, -88, -45, 1, 21, -88, -46, 0, 20, -89, -46, 0, 20, -90, -47, 0, 20, -90, -47, 0, 20, -91, -48, 0, 19, -91, -48, 0, 19, -92, -49, -1, 19, -93, -49, -1, 19, -93, -49, -1, 19, -94, -50, -1, 18, -95, -50, -2, 18, -95, -51, -2, 18, -96, -51, -2, 18, -97, -52, -3, 18, -97, -52, -3, 17, -98, -53, -3, 17, -99, -53, -3, 17, -99, -54, -4, 17, -100, -54, -4, 17, -101, -55, -4, 16, -101, -55, -5, 16, -102, -56, -5, 16, -103, -56, -5, 16, -103, -57, -5, 16, -104, -57, -6, 15, -105, -58, -6, 15, -105, -58, -6, 15, -106, -59, -7, 15, -107, -59, -7, 15, -107, -60, -7, 14, -108, -60, -7, 14, -108, -61, -8, 14, -109, -61, -8, 14, -110, -61, -8, 14, -110, -62, -9, 13, -111, -62, -9, 13, -112, -63, -9, 13, -112, -63, -9, 13, -113, -64, -10, 12, -114, -64, -10, 12, -114, -65, -10, 12, -115, -65, -11, 12, -116, -66, -11, 12, -116, -66, -11, 11, -117, -67, -11, 11, -118, -67, -12, 11, -118, -68, -12, 11, -119, -68, -12, 11, -120, -69, -13, 10, -120, -69, -13, 10, -121, -70, -13, 10, -122, -70, -13, 10, -122, -71, -14, 10, -123, -71, -14, 9, -124, -72, -14, 9, -124, -72, -15, 9, -125, -73, -15, 9, -126, -73, -15, 9, -126, -73, -15, 8, -127, -74, -16, 8, -127, -74, -16, 8, -128, -75, -16, 8, -129, -75, -17, 8, -129, -76, -17, 7, -130, -76, -17, 7, -131, -77, -17, 7, -131, -77, -18, 7, -132, -78, -18, 7, -133, -78, -18, 6, -133, -79, -19, 6, -134, -79, -19, 6, -135, -80, -19, 6, -135, -80, -20, 5, -136, -81, -20, 5, -137, -81, -20, 5, -137, -82, -20, 5, -138, -82, -21, 5, -139, -83, -21, 4, -139, -83, -21, 4, -140, -84, -22, 4, -141, -84, -22, 4, -141, -85, -22, 4, -142, -85, -22, 3, -143, -85, -23, 3, -143, -86, -23, 3, -144, -86, -23, 3, -145, -87, -24, 3, -145, -87, -24, 2, -146, -88, -24, 2, -146, -88, -24, 2, -147, -89, -25, 2, -148, -89, -25, 2, -148, -90, -25, 1, -149, -90, -26, 1, -150, -91, -26, 1, -150, -91, -26, 1, -151, -92, -26, 1, -152, -92, -27, 0, -152, -93, -27, 0, -153, -93, -27, 0, -154, -94, -28, 0, -154, -94, -28, 0, -155, -95, -28, 0, -156, -95, -28, 0, -156, -96, -29, 0, -157, -96, -29, 0, -158, -97, -29, 0, -158, -97, -30, -1, -159, -97, -30, -1, -160, -98, -30, -1, -160, -98, -30, -1, -161, -99, -31, -2, -162, -99, -31, -2, -162, -100, -31, -2, -163, -100, -32, -2, -163, -101, -32, -2, -164, -101, -32, -3, -165, -102, -32, -3, -165, -102, -33, -3, -166, -103, -33, -3, -167, -103, -33, -3, -167, -104, -34, -4, -168, -104, -34, -4, -169, -105, -34, -4, -169, -105, -34, -4, -170, -106, -35, -4, -171, -106, -35, -5, -171, -107, -35, -5, -172, -107, -36, -5, -173, -108, -36, -5, -173, -108, -36, -5, -174, -109, -37, -6, -175, -109, -37, -6, -175, -109, -37, -6, -176, -110, -37, -6, -177, -110, -38, -6, -177, -111, -38, -7, -178, -111, -38, -7, -179, -112, -39, -7, -179, -112, -39, -7, -180, -113, -39, -7, -181, -113, -39, -8, -181, -114, -40, -8, -182, -114, -40, -8, -182, -115, -40, -8, -183, -115, -41, -9, -184, -116, -41, -9, -184, -116, -41, -9, -185, -117, -41, -9, -186, -117, -42, -9, -186, -118, -42, -10, -187, -118, -42, -10, -188, -119, -43, -10, -188, -119, -43, -10, -189, -120, -43, -10, -190, -120, -43, -11, -190, -121, -44, -11, -191, -121, -44, -11, -192, -121, -44, -11, -192, -122, -45, -11, -193, -122, -45, -12, -194, -123, -45, -12, -194, -123, -45, -12, -195, -124, -46, -12, -196, -124, -46, -12, -196, -125, -46, -13, -197, -125, -47, -13, -198, -126, -47, -13, -198, -126, -47, -13, -199, -127, -47, -13, -199, -127, -48, -14, -200, -128, -48, -14, -201, -128, -48, -14, -201, -129, -49, -14, -202, -129, -49, -14, -203, -130, -49, -15, -203, -130, -49, -15, -204, -131, -50, -15, -205, -131, -50, -15, -205, -132, -50, -16, -206, -132, -51, -16, -207, -133, -51, -16, -207, -133, -51, -16, -208, -133, -51, -16, -209, -134, -52, -17, -209, -134, -52, -17, -210, -135, -52, -17, -211, -135, -53, -17, -211, -136, -53, -17, -212, -136, -53, -18, -213, -137, -53, -18, -213, -137, -54, -18, -214, -138, -54, -18, -215, -138, -54, -18, -215, -139, -55, -19, -216, -139, -55, -19, -216, -140, -55, -19, -217, -140, -56, -19, -218, -141, -56, -19, -218, -141, -56, -20, -219, -142, -56, -20, -220, -142, -57, -20, -220, -143, -57, -20, -221, -143, -57, -20, -222, -144, -58, -21, -222, -144, -58, -21, -223, -145, -58, -21, -224, -145, -58, -21, -224, -145, -59, -21, -225, -146, -59, -22, -226, -146, -59, -22, -226, -147, -60, -22, -227, -147, -60, -22, -228, -148, -60, -23, -228, -148, -60, -23, -229, -149, -61, -23, -230, -149, -61, -23, -230, -150, -61, -23, -231, -150, -62, -24, -232, -151, -62, -24, -232, -151, -62, -24, -233, -152, -62, -24, -234, -152, -63, -24, -234, -153, -63, -25, -235, -153, -63, -25, -235, -154, -64, -25, -236, -154, -64, -25, -237, -155, -64, -25, -237, -155, -64, -26, -238, -156, -65, -26, -239, -156, -65, -26, -239, -157, -65, -26, -240, -157, -66, -26, -241, -157, -66, -27, -241, -158, -66, -27, -242, -158, -66, -27, -243, -159, -67, -27, -243, -159, -67, -27, -244, -160, -67, -28, -245, -160, -68, -28, -245, -161, -68, -28, -246, -161, -68, -28, -247, -162, -68, -28, -247, -162, -69, -29, -248, -163, -69, -29, -249, -163, -69, -29, -249, -164, -70, -29, -250, -164, -70, -30, -251, -165, -70, -30, -251, -165, -70, -30, -252, -166, -71, -30, -253, -166, -71, -30, -253, -167, -71, -31, -254, -167, -72, -31, -254, -168, -72, -31, -255, -168, -72, -31, -256, -169, -73, -31, -256, -169, -73, -32, -257, -169, -73, -32, -258, -170, -73, -32, -258, -170, -74, -32, -259, -171, -74, -32, -260, -171, -74, -33, -260, -172, -75, -33, -261, -172, -75, -33, -262, -173, -75, -33, -262, -173, -75, -33, -263, -174, -76, -34};

//u16 cached_floor_col = MAX_U16;
//u16 cached_floor_light_level = MAX_U16;
//u32 cached_rel_floor_height = MAX_U32;
//u16 cached_ceil_col = MAX_U16;
//u16 cached_ceil_light_level = MAX_U16;
//u32 cached_rel_ceil_height = MAX_U32;


void clear_light_cache() {
//    cached_floor_col = MAX_U16;
//    cached_floor_light_level = MAX_U16;
//    cached_rel_floor_height = MAX_U32;
//    cached_ceil_col = MAX_U16;
//    cached_ceil_light_level = MAX_U16;
//    cached_rel_ceil_height = MAX_U32;
}

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
    //light_color = get_light_color(floor_col, light_level);
    // shift, lookup, lookup
    //mid_color = get_mid_dark_color(floor_col, light_level);
    // shift, lookup, lookup
    //dark_color = get_dark_color(floor_col, light_level);
    
    u32* col_ptr = get_color_ptr(floor_col, light_level);



    //params->light_color = light_color;
    //params->mid_color = mid_color;
    //params->dark_color = dark_color;
    params->dark_color = *col_ptr++;
    params->mid_color = *col_ptr++;
    params->light_color = *col_ptr++;

    params->needs_lighting = 1;
    
    int table_idx = (-rel_floor_height)*4;

    //s16 dark_top = 0;
    // lookup
    s16 dark_bot = floor_light_positions[table_idx++];

    table_idx++;

    //s16 mid_top = mid_dark_bot;
    // lookup
    s16 mid_bot = floor_light_positions[table_idx++];

    params->dark_y = dark_bot;
    params->mid_y = mid_bot;
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

    
    mid_color = get_mid_dark_color(ceil_col, light_level);
    dark_color = get_dark_color(ceil_col, light_level);
    //if(light_color == mid_color && mid_color == dark_color) {
    //    params->needs_lighting = 0;
    //    return;
    //}
    params->mid_color = mid_color;
    params->dark_color = dark_color;
    params->needs_lighting = 1;


    int table_idx = rel_ceil_height*4;

    table_idx++;
    s16 mid_top = ceil_light_positions[table_idx++];
    table_idx++;
    s16 dark_top = ceil_light_positions[table_idx++];

    params->dark_y = dark_top;
    params->mid_y = mid_top;
}

// draw just light
// draw light and med
// draw med and dark
// draw light, med, dark

u8* draw_lit_floor_light_only(s16 floor_top_y, s16 floor_bot_y, u8* col_ptr, light_params* params) {
  return draw_native_vertical_line_unrolled(floor_top_y, floor_bot_y, params->light_color, col_ptr);
}

u8* draw_lit_floor(s16 floor_top_y, s16 floor_bot_y, u8* col_ptr, light_params* params) {
    #ifndef FLATS_DIST_LIGHTING
    return draw_native_vertical_line_unrolled(floor_top_y, floor_bot_y, params->light_color, col_ptr);
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
    
    int draw_dark_top = floor_top_y;
    int draw_dark_bot = min(floor_bot_y, params->dark_y);

    if(draw_dark_top < draw_dark_bot) {
        draw_native_vertical_line_unrolled(draw_dark_top, draw_dark_bot, params->dark_color, col_ptr);
    }

    int draw_mid_top = max(floor_top_y, params->dark_y);
    int draw_mid_bot = min(floor_bot_y, params->mid_y);
    if(draw_mid_top < draw_mid_bot) {
        draw_native_vertical_line_unrolled(draw_mid_top, draw_mid_bot, params->mid_color, col_ptr);
    }

    int draw_light_top = max(floor_top_y, params->mid_y);
    int draw_light_bot = floor_bot_y;
    if(draw_light_top < draw_light_bot) {
        draw_native_vertical_line_unrolled(draw_light_top, draw_light_bot, params->light_color, col_ptr);
    }

    return col_ptr;
}

u8* draw_lit_ceil_light_only(s16 ceil_top_y, s16 ceil_bot_y, u8* col_ptr, light_params* params) {
    return draw_native_vertical_line_unrolled(ceil_top_y, ceil_bot_y, params->light_color, col_ptr);
}

u8*  draw_lit_ceiling(s16 ceil_top_y, s16 ceil_bot_y, u8* col_ptr, light_params* params) {
    #ifndef FLATS_DIST_LIGHTING
    s16 mid = ((ceil_bot_y-ceil_top_y)<<1)+ceil_top_y;
    if(mid < params->mid_y) {
        return draw_native_vertical_line_unrolled(ceil_top_y, ceil_bot_y, params->light_color, col_ptr);
    } else {

    }
    #endif

    int draw_light_top = ceil_top_y;
    int draw_light_bot = min(ceil_bot_y, params->mid_y);

    if(draw_light_top < draw_light_bot) {
        draw_native_vertical_line_unrolled(draw_light_top, draw_light_bot, params->light_color, col_ptr);
    }

    int draw_mid_top = max(ceil_top_y, params->mid_y);
    int draw_mid_bot = min(ceil_bot_y, params->dark_y);
    if(draw_mid_top < draw_mid_bot) {
        draw_native_vertical_line_unrolled(draw_mid_top, draw_mid_bot, params->mid_color, col_ptr);
    }

    int draw_dark_top = max(ceil_top_y, params->dark_y);
    int draw_dark_bot = ceil_bot_y;
    if (draw_dark_top < draw_dark_bot) {
        draw_native_vertical_line_unrolled(draw_dark_top, draw_dark_bot, params->dark_color, col_ptr);
    }

    return col_ptr;
}

typedef u8* (*draw_lit_plane_fp)(s16 top_y, s16 bot_y, u8* col_ptr, light_params* params);



//u32 tex_col_buffer[RENDER_WIDTH];
u16 tex_col_buffer[RENDER_WIDTH];

void calculate_tex_coords_for_wall(
    s16 beginx, s16 endx, s16 skip_x, s16 dx,
    u16 z1_12_4,     u16 z2_12_4,
    texmap_params* tmap_info) {
    u32 left_u_16 = tmap_info->left_u * tmap_info->repetitions;
    u32 right_u_16 = tmap_info->right_u * tmap_info->repetitions;
    persp_params persp = calc_perspective(z1_12_4, z2_12_4, left_u_16, right_u_16, dx);
    

    u32 one_over_z_26 = persp.one_over_z_26;
    s32 d_one_over_z_dx_26 = persp.d_one_over_z_dx_26;
    u32 u_over_z_23 = persp.u_over_z_23;
    u32 d_u_over_z_dx_23 = persp.d_u_over_z_dx_23;

    u32 du = right_u_16 - left_u_16;
    u32 du_dx = du/dx;

    u32 cur_u = left_u_16;
    if(skip_x > 0) {
        // always use perspective
        one_over_z_26 += (skip_x * d_one_over_z_dx_26);
        u_over_z_23 += skip_x * d_u_over_z_dx_23;

        // TEMP HACK FOR NO PERSPECTIVE
        cur_u += (skip_x * du_dx);
    }

    u16 one_over_z_16 = one_over_z_26>>10;
    u16 u_over_z_16 = u_over_z_23>>7;
    s16 d_one_over_z_dx_16 = d_one_over_z_dx_26>>10;
    u16 d_u_over_z_dx_16 = d_u_over_z_dx_23>>7;

    



    //if(skip_x > 0) {
    //    // always use perspective
    //    one_over_z_26 += (skip_x * d_one_over_z_dx_26);
    //    u_over_z_23 += skip_x * d_u_over_z_dx_23;
    //}


    const u8 tex_width_shift = MID_MIP_WIDTH_SHIFT;
    const u8 tex_width = MID_MIP_WIDTH;
    

    
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


    
    //while(cnt--) {
    if(cnt & 1) {
        TEXMAP_16_32_ITER;
    }
    //return;
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
        u16 u_7_end;
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
    DARK, MID, LIGHT
} calc_light_level;

u8 num_light_levels;
u8 light_levels[6];

void calculate_light_levels_for_wall(u32 clipped_dx, s32 inv_z1, s32 inv_z2, s32 fix_inv_dz_per_dx, s8 light_level) {
    num_light_levels = 0;
    u8* out_ptr = light_levels;
    u8 cnt = clipped_dx;

    s16 cur_inv_z = inv_z1;
    u8 cur_light;
    if (cur_inv_z <= FIX_0_16_INV_DARK_DIST) {
        cur_light = DARK;
    } else if (cur_inv_z <= FIX_0_16_INV_MID_DIST) {
        cur_light = MID;
    } else {
        cur_light = LIGHT;
    }


    if(light_level == -2 || light_level == 2) { // 0) {
        // z stays the same
        //output_light_span(cur_light, cnt);
        *out_ptr++ = cur_light; //MID;
        *out_ptr++ = cnt; //cur_light;
        //*out_ptr++ = cnt;
        num_light_levels = 1;
        return;
    }
   

    #ifndef WALLS_DIST_LIGHTING
        *out_ptr++ = cur_light;
        *out_ptr++ = cnt; //cur_light;
        //*out_ptr++ = cnt;
        num_light_levels = 1;
        return 0;
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
    if (end_inv_z <= FIX_0_16_INV_DARK_DIST) {
        end_light = DARK;
    } else if (end_inv_z <= FIX_0_16_INV_MID_DIST) {
        end_light = MID;
    } else {
        end_light = LIGHT;
    }



    u8 cur_span = 0;

    /*

    while(cnt--) {
        u8 next_light; 
        if (cur_inv_z <= FIX_0_16_INV_DARK_DIST) {
            next_light = DARK;
        } else if (cur_inv_z <= FIX_0_16_INV_MID_DIST) {
            next_light = MID;
        } else {
            next_light = LIGHT;
        }

        if(cur_light == next_light) {
            cur_span++;
        } else {
            __asm volatile(
                "move.b %1, (%0)+\t\n\
                move.b %2, (%0)+"
                : "+a" (out_ptr)
                : "d" (cur_light), "d" (cur_span)
            );
            //*out_ptr++ = cur_light;
            //*out_ptr++ = cur_span;
            cur_light = next_light;
            cur_span = 1;


            if(end_light == cur_light) {
                cur_span += cnt;
                break;
            }
        }
        cur_inv_z += fix_inv_dz_per_dx;
        
    }
    */
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


    u8 next_light; 

    while(cnt--) {
        u8 next_light; 
        if (cur_inv_z <= FIX_0_16_INV_DARK_DIST) {
            next_light = DARK;
        } else if (cur_inv_z <= FIX_0_16_INV_MID_DIST) {
            next_light = MID;
        } else {
            next_light = LIGHT;
        }

        if(cur_light == next_light) {
            cur_span++;
        } else {
            __asm volatile(
                "move.b %1, (%0)+\t\n\
                move.b %2, (%0)+"
                : "+a" (out_ptr)
                : "d" (cur_light), "d" (cur_span)
            );
            //*out_ptr++ = cur_light;
            //*out_ptr++ = cur_span;
            cur_light = next_light;
            cur_span = 1;


            if(end_light == cur_light) {
                cur_span += cnt;
                break;
            }
        }
        cur_inv_z += fix_inv_dz_per_dx;
    }

   
finished:
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
    if(far_inv_z <= FIX_0_16_INV_MID_DIST) {
        ceil_func = &draw_lit_ceiling;
    }

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
    s32 fix_inv_dz_per_dx = inv_dz / dx;

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
    u32 mid_color = get_mid_dark_color(upper_color, light_level);
    u32 light_color = get_light_color(upper_color, light_level);

    //u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    u8** offset_ptr = &bmp_ptr_buf[x];

    u8* light_ptr = light_levels;
    for(int i = 0; i < num_light_levels; i++) {
        u8 light_level = *light_ptr++;
        u8 light_cnt = *light_ptr++;
        u32 color;
        switch(light_level) {
            case DARK:
                color = dark_color;
                break;
            case MID:
                color = mid_color;
                break;
            case LIGHT:
                color = light_color;
                break;
        }
        while(light_cnt--) {
            top_y_int = top_y_fix >> 16;
            ntop_y_int = ntop_y_fix >> 16;
            u8 min_drawable_y = *yclip_ptr;
            u8 max_drawable_y = *(yclip_ptr+1);
            //if(min_drawable_y >= max_drawable_y) { continue; }
            u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);
            u8 bot_draw_y = clamp(ntop_y_int, min_drawable_y, max_drawable_y);


            u8* col_ptr = *offset_ptr++;
            u8 col_drawn = (col_ptr == NULL);

            //u8 col_drawn = *drawn_buf_ptr++;
            if(!col_drawn) {
                u8 clip_top = max(top_draw_y, bot_draw_y);
                if(min_drawable_y < top_draw_y) {
                    u8* ret_ptr = ceil_func(min_drawable_y, top_draw_y, col_ptr, params);
                    *yclip_ptr = top_draw_y;
                    //*offset_ptr = ret_ptr;
                }
                if(top_draw_y < bot_draw_y) {
                    u8* ret_ptr = draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, color, col_ptr);
                    *yclip_ptr = bot_draw_y;
                    //*offset_ptr = ret_ptr;
                }
            }
            yclip_ptr += 2;
            //offset_ptr++;

            top_y_fix += top_dy_per_dx;
            ntop_y_fix += ntop_dy_per_dx;
        }

    }
    
    //flip();

    return; 
}

void draw_top_pegged_textured_upper_step(s16 x1, s16 x1_ytop, s16 nx1_ytop, s16 x2, s16 x2_ytop, s16 nx2_ytop,
                                         u16 z1_12_4, u16 z2_12_4,
                                         u16 inv_z1, u16 inv_z2,
                                         u16 window_min, u16 window_max,
                                         texmap_params* tmap_info, 
                                         light_params* params, 
                                         s16 x1_pegged_top, s16 x2_pegged_top) {
              
    u16 far_inv_z = min(inv_z1, inv_z2);
    draw_lit_plane_fp ceil_func = &draw_lit_ceil_light_only;
    if(far_inv_z <= FIX_0_16_INV_MID_DIST) {
        ceil_func = &draw_lit_ceiling;
    }

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
        z1_12_4, z2_12_4, tmap_info);

    lit_texture* lit_tex = tmap_info->tex;
    u16* dark_tex = lit_tex->dark;
    u16* mid_tex = lit_tex->mid;
    u16* light_tex = lit_tex->light;


    s16 x = beginx;
    u16 cnt = endx-x;
    u8* yclip_ptr = &(yclip[x<<1]);
    //u8* drawn_buf_ptr = &(drawn_buf[x]);
    
    u8** offset_ptr = &bmp_ptr_buf[x];
    //(bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    u16* tex_col_ptr = tex_col_buffer;

    while(cnt--) {
        top_y_int = top_y_fix >> 16;
        ntop_y_int = ntop_y_fix >> 16;
        u8 min_drawable_y = *yclip_ptr;
        u8 max_drawable_y = *(yclip_ptr+1);
        //if(min_drawable_y >= max_drawable_y) { continue; }
        u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);
        u8 bot_draw_y = clamp(ntop_y_int, min_drawable_y, max_drawable_y);

        //u8 col_drawn = *drawn_buf_ptr++;

        u8* col_ptr = *offset_ptr++;
        u8 col_drawn = (col_ptr == NULL);

        u16 tex_idx = *tex_col_ptr++;
        if(!col_drawn) {
            if(top_draw_y < bot_draw_y) {
                s16 cur_inv_z = cur_fix_inv_z;
                s16 pegged_top_y_int = pegged_top_y_fix>>16;


                u16* tex_column;
                if (cur_inv_z <= FIX_0_16_INV_DARK_DIST) {
                    tex_column = &dark_tex[tex_idx];
                } else if (cur_inv_z <= FIX_0_16_INV_MID_DIST) {
                    tex_column = &mid_tex[tex_idx];
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
        cur_fix_inv_z += fix_inv_dz_per_dx;
        pegged_top_y_fix += pegged_top_dy_per_dx;
    }
    
    //flip();

    return; 
}

void draw_bottom_pegged_textured_lower_step(
    s16 x1, s16 x1_ybot, s16 nx1_ybot, s16 x2, s16 x2_ybot, s16 nx2_ybot, 
    u16 z1_12_4,     u16 z2_12_4,
    u16 inv_z1, u16 inv_z2,
    u16 window_min, u16 window_max,
    texmap_params* tmap_info, light_params* params,
    s16 x1_pegged_bot, s16 x2_pegged_bot) {
    
    u16 far_inv_z = min(inv_z1, inv_z2);
    draw_lit_plane_fp floor_func = &draw_lit_floor_light_only;
    if(far_inv_z <= FIX_0_16_INV_MID_DIST) {
        floor_func = &draw_lit_floor;
    }

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
        z1_12_4, z2_12_4, tmap_info);


    lit_texture* lit_tex = tmap_info->tex;
    u16* dark_tex = lit_tex->dark;
    u16* mid_tex = lit_tex->mid;
    u16* light_tex = lit_tex->light;

    s16 x = beginx;
    u16 cnt = endx-x;
    u8* yclip_ptr = &(yclip[x<<1]);
    //u8* drawn_buf_ptr = &(drawn_buf[x]);

    u8** offset_ptr = &bmp_ptr_buf[x];
    //u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    u16* tex_col_ptr = tex_col_buffer;

    while(cnt--) {
        bot_y_int = bot_y_fix >> 16;
        nbot_y_int = nbot_y_fix >> 16;
        u8 min_drawable_y = *yclip_ptr;
        u8 max_drawable_y = *(yclip_ptr+1);

        u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);
        u8 top_draw_y = clamp(nbot_y_int, min_drawable_y, max_drawable_y);

        u8* col_ptr = *offset_ptr++;
        //u8 col_drawn = *drawn_buf_ptr++;
        u8 col_drawn = (col_ptr == NULL);
        u16 tex_idx = *tex_col_ptr++;
        if(!col_drawn) {
            if(top_draw_y < bot_draw_y) {
                s16 cur_inv_z = cur_fix_inv_z;
                s16 pegged_bot_y_int = pegged_bot_y_fix>>16;
                
                u16* tex_column;
                if (cur_inv_z <= FIX_0_16_INV_DARK_DIST) {
                    tex_column = &dark_tex[tex_idx];
                } else if (cur_inv_z <= FIX_0_16_INV_MID_DIST) {
                    tex_column = &mid_tex[tex_idx];
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
        cur_fix_inv_z += fix_inv_dz_per_dx;
        pegged_bot_y_fix += pegged_bot_dy_per_dx;
    }
    
    //flip();

    return; 
}

void draw_lower_step(s16 x1, s16 x1_ybot, s16 nx1_ybot, s16 x2, s16 x2_ybot, s16 nx2_ybot, 
                     u16 inv_z1, u16 inv_z2,
                     u16 window_min, u16 window_max, u8 lower_color, s8 light_level, light_params* params) {
    
    u16 far_inv_z = min(inv_z1, inv_z2);
    draw_lit_plane_fp floor_func = &draw_lit_floor_light_only;
    if(far_inv_z <= FIX_0_16_INV_MID_DIST) {
        floor_func = &draw_lit_floor;
    }

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

    s16 inv_dz = inv_z2 - inv_z1;
    s16 fix_inv_dz_per_dx = (inv_dz) / dx; //(inv_dz << 16) / dx;

    s16 cur_fix_inv_z = inv_z1; //inv_z1<<16;


    s16 skip_x = beginx - x1;
    if(skip_x > 0) {
        bot_y_fix += (skip_x * bot_dy_per_dx);
        nbot_y_fix += (skip_x * nbot_dy_per_dx);
        cur_fix_inv_z += (skip_x * fix_inv_dz_per_dx);
    }
    

    s16 endx = min(window_max, x2);

    s16 x = beginx;
    u8* yclip_ptr = &(yclip[x<<1]);
    u8* drawn_buf_ptr = &(drawn_buf[x]);

    u32 dark_color = get_dark_color(lower_color, light_level);
    u32 mid_color = get_mid_dark_color(lower_color, light_level);
    u32 light_color = get_light_color(lower_color, light_level);


    u16 cnt = endx-x;
    calculate_light_levels_for_wall(cnt, cur_fix_inv_z, inv_z2, fix_inv_dz_per_dx, light_level);

    //u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    u8** offset_ptr = &bmp_ptr_buf[x];

    u8* light_ptr = light_levels;
        for(int i = 0; i < num_light_levels; i++) {
        u8 light_level = *light_ptr++;
        u8 light_cnt = *light_ptr++;
        u32 color;
        
        switch(light_level) {
            case DARK:
                color = dark_color;
                break;
            case MID:
                color = mid_color;
                break;
            case LIGHT:
                color = light_color;
                break;
        }
        while(light_cnt--) {
            bot_y_int = bot_y_fix >> 16;
            nbot_y_int = nbot_y_fix >> 16;
            u8 min_drawable_y = *yclip_ptr;
            u8 max_drawable_y = *(yclip_ptr+1);

            u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);
            u8 top_draw_y = clamp(nbot_y_int, min_drawable_y, max_drawable_y);

            u8* col_ptr = *offset_ptr++;
            u8 col_drawn = (col_ptr == NULL);
            //u8 col_drawn = *drawn_buf_ptr++;
            if(!col_drawn) {
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
    
    //flip();

    return; 
}



void draw_ceiling_update_clip(s16 x1, s16 x1_ytop, s16 x2, s16 x2_ytop,
                              u16 far_inv_z,
                              u16 window_min, u16 window_max, light_params* params) {
        
    draw_lit_plane_fp ceil_func = &draw_lit_ceil_light_only;
    if(far_inv_z <= FIX_0_16_INV_MID_DIST) {
        ceil_func = &draw_lit_ceiling;
    }


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
    //u8* drawn_buf_ptr = &(drawn_buf[x]);

    int cnt = endx-beginx;

    u8** offset_ptr = &bmp_ptr_buf[x];
    while(cnt--) {
        top_y_int = top_y_fix >> 16;
        u8 min_drawable_y = *yclip_ptr;
        u8 max_drawable_y = *(yclip_ptr+1);
        u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);

        u8* col_ptr = *offset_ptr++;
        u8 col_drawn = (col_ptr == NULL);

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
    
    //flip();

    return; 
}
                            

void draw_floor_update_clip(s16 x1, s16 x1_ybot, s16 x2, s16 x2_ybot, 
                            u16 far_inv_z,
                            u16 window_min, u16 window_max, light_params* params) {             


    draw_lit_plane_fp floor_func = &draw_lit_floor_light_only;
    if(far_inv_z <= FIX_0_16_INV_MID_DIST) {
        floor_func = &draw_lit_floor;
    }
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
    //u8* drawn_buf_ptr = &(drawn_buf[x]);

    //u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    u8** offset_ptr = &bmp_ptr_buf[x];

    for(;x < endx; x++) {
        bot_y_int = bot_y_fix >> 16;
        u8 min_drawable_y = *yclip_ptr;
        u8 max_drawable_y = *(yclip_ptr+1);
        //if(min_drawable_y >= max_drawable_y) { continue; }
        
        u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);

        u8* col_ptr = *offset_ptr++;
        //u8 col_drawn = *drawn_buf_ptr++;
        u8 col_drawn = (col_ptr == NULL);
        

        if(max_drawable_y > bot_draw_y && !col_drawn) {
            floor_func(bot_draw_y, max_drawable_y, col_ptr, params);
            *(yclip_ptr+1) = bot_draw_y;

        }
        yclip_ptr += 2;

        bot_y_fix += bot_dy_per_dx;
    }
    
    //flip();

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
    if(light_floor_ceil) {
        floor_func = &draw_lit_floor;
        ceil_func = &draw_lit_ceiling;
    }


    // 4 subpixel bits here
    s16 top_dy_fix, bot_dy_fix;
    fix32 top_y_fix, bot_y_fix;
    top_dy_fix = x2_ytop - x1_ytop;
    bot_dy_fix = x2_ybot - x1_ybot;
    top_y_fix = x1_ytop<<12; 
    bot_y_fix = x1_ybot<<12;
    s16 dx = x2-x1;

    fix32 top_dy_per_dx = (top_dy_fix<<12) / dx; // 16.16 / 16 -> 16.16 
    fix32 bot_dy_per_dx = (bot_dy_fix<<12) / dx;
    
    s16 top_y_int;
    s16 bot_y_int;


    s16 inv_dz = inv_z2 - inv_z1;
    
    s32 fix_inv_dz_per_dx = inv_dz / dx;

    s32 cur_fix_inv_z = inv_z1;

    s16 beginx = max(x1, window_min);
    s16 endx = min(window_max, x2);
    s16 skip_x = beginx - x1;

    

    if(skip_x > 0) {
        top_y_fix += (skip_x * top_dy_per_dx);
        bot_y_fix += (skip_x * bot_dy_per_dx);
        cur_fix_inv_z += (skip_x * fix_inv_dz_per_dx);
    }
    
    s16 x = beginx;
    u16 cnt = endx-x;

    //u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    u8** offset_ptr = &bmp_ptr_buf[x];
    u8* yclip_ptr = &(yclip[(x<<1)]);
    //u8* drawn_buf_ptr = &(drawn_buf[x]);

    calculate_light_levels_for_wall(cnt, cur_fix_inv_z, inv_z2, fix_inv_dz_per_dx, light_level);
    
    u32 light_color = get_light_color(wall_color, light_level);
    u32 mid_color = get_mid_dark_color(wall_color, light_level);
    u32 dark_color = get_dark_color(wall_color, light_level);


    u8* light_ptr = light_levels;
    for(int i = 0; i < num_light_levels; i++) {
        u8 light_level = *light_ptr++;
        u8 light_cnt = *light_ptr++;
        u32 color;
        
        switch(light_level) {
            case DARK:
                color = dark_color;
                break;
            case MID:
                color = mid_color;
                break;
            case LIGHT:
                color = light_color;
                break;
        }
        while(light_cnt--) {
            top_y_int = top_y_fix >> 16;
            bot_y_int = bot_y_fix >> 16;

            u8 min_drawable_y = *yclip_ptr++;
            u8 max_drawable_y = *yclip_ptr++;
            //if(min_drawable_y >= max_drawable_y) { continue; }
            u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);
            u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);

            
            u8* col_ptr = *offset_ptr;
            //u8 col_drawn = *drawn_buf_ptr;
            u8 col_drawn = (col_ptr == NULL);
            if(!col_drawn) {
                if(min_drawable_y < top_draw_y) {
                    ceil_func(min_drawable_y, top_draw_y, col_ptr, ceil_params);
                }
                if(top_draw_y < bot_draw_y) {
                    draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, color, col_ptr);
                }
                if(bot_draw_y < max_drawable_y) {
                    floor_func(bot_draw_y, max_drawable_y, col_ptr, floor_params);
                }
                *offset_ptr++ = NULL;
            } else {
                offset_ptr++;
            }

            top_y_fix += top_dy_per_dx;
            bot_y_fix += bot_dy_per_dx;
        }
    }
    
    //flip();

    return; 
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
    
    s16 top_y_int;
    s16 bot_y_int;


    s16 inv_dz = inv_z2 - inv_z1;
    
    s32 fix_inv_dz_per_dx = inv_dz / dx;

    s32 cur_fix_inv_z = inv_z1;

    s16 beginx = max(x1, window_min);
    s16 endx = min(window_max, x2);
    s16 skip_x = beginx - x1;

    

    if(skip_x > 0) {
        top_y_fix += (skip_x * top_dy_per_dx);
        bot_y_fix += (skip_x * bot_dy_per_dx);
        cur_fix_inv_z += (skip_x * fix_inv_dz_per_dx);
    }

    calculate_tex_coords_for_wall(beginx, endx, skip_x, dx,
        z1_12_4, z2_12_4, tmap_info);
        

    lit_texture* lit_tex = tmap_info->tex;
    u16* dark_tex = lit_tex->dark;
    u16* mid_tex = lit_tex->mid;
    u16* light_tex = lit_tex->light;

    s16 x = beginx;
    u16 cnt = endx-x;

    //u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    u8** offset_ptr = &bmp_ptr_buf[x];
    u8* yclip_ptr = &(yclip[(x<<1)]);
    //u8* drawn_buf_ptr = &(drawn_buf[x]);

    //u32* tex_col_ptr = tex_col_buffer;
    u16* tex_col_ptr = tex_col_buffer;

    calculate_light_levels_for_wall(cnt, cur_fix_inv_z, inv_z2, fix_inv_dz_per_dx, light_level);
    
    //u16 far_inv_z = min(inv_z1, inv_z2);
    draw_lit_plane_fp floor_func = &draw_lit_floor_light_only;
    draw_lit_plane_fp ceil_func = &draw_lit_ceil_light_only;

    const u8 light_floor_ceil = ((inv_z1 <= FIX_0_16_INV_MID_DIST) || (inv_z2 <= FIX_0_16_INV_MID_DIST));


    u8* light_ptr = light_levels;
    if(light_floor_ceil) {
        for(u16 i = 0; i < num_light_levels; i++) {
            u8 light_level = *light_ptr++;
            u8 light_cnt = *light_ptr++;
            u16* base_tex;
            switch(light_level) {
                case DARK:
                    base_tex = dark_tex;
                    break;
                case MID:
                    base_tex = mid_tex;
                    break;
                case LIGHT:
                    base_tex = light_tex;
                    break;
            }
            while(light_cnt--) {
                top_y_int = top_y_fix >> 16;
                bot_y_int = bot_y_fix >> 16;


                u8 min_drawable_y = *yclip_ptr++;
                u8 max_drawable_y = *yclip_ptr++;
                
                u8* col_ptr = *offset_ptr;
                u8 col_drawn = (col_ptr == NULL);
                *offset_ptr++ = NULL;

                u16 tex_idx = *tex_col_ptr++;
                top_y_fix += top_dy_per_dx;
                bot_y_fix += bot_dy_per_dx;


                
                if(!col_drawn) {
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


                    s16 wtop = top_y_int;
                    s16 wbot = bot_y_int;
                    wtop = (wtop >= min_drawable_y ? wtop : min_drawable_y);
                    wbot = (wbot <= max_drawable_y ? wbot : max_drawable_y);

                    if(wtop < wbot) {
                        if(max_drawable_y <= wtop) {
                            // fully clips wall
                        } else if (min_drawable_y >= wbot) {
                           // fully clips wall
                        } else {                
                            u16* tex_column = &base_tex[tex_idx];
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
    } else {
        for(u16 i = 0; i < num_light_levels; i++) {
            u8 light_level = *light_ptr++;
            u8 light_cnt = *light_ptr++;
            u16* base_tex;
            switch(light_level) {
                case DARK:
                    base_tex = dark_tex;
                    break;
                case MID:
                    base_tex = mid_tex;
                    break;
                case LIGHT:
                    base_tex = light_tex;
                    break;
            }
            while(light_cnt--) {
                top_y_int = top_y_fix >> 16;
                bot_y_int = bot_y_fix >> 16;

                u8 min_drawable_y = *yclip_ptr++;
                u8 max_drawable_y = *yclip_ptr++;

                u8* col_ptr = *offset_ptr;
                u8 col_drawn = (col_ptr == NULL);
                *offset_ptr++ = NULL;

                u16 tex_idx = *tex_col_ptr++;
                top_y_fix += top_dy_per_dx;
                bot_y_fix += bot_dy_per_dx;

                if(!col_drawn) {

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
                        } else {                
                            u16* tex_column = &base_tex[tex_idx];
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
    }    

    //flip();

    return; 
}


void draw_top_pegged_wall(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 z1_12_4,     u16 z2_12_4,
              u16 inv_z1, u16 inv_z2,
              u16 window_min, u16 window_max,
              texmap_params *tmap_info, 
              light_params* floor_params, light_params* ceil_params,
              s16 x1_pegged_top, s16 x2_pegged_top) {


    u16 far_inv_z = min(inv_z1, inv_z2);
 
    draw_lit_plane_fp floor_func = &draw_lit_floor_light_only;
    if(far_inv_z <= FIX_0_16_INV_MID_DIST) {
        floor_func = &draw_lit_floor;
    }
    draw_lit_plane_fp ceil_func = &draw_lit_ceil_light_only;
    if(far_inv_z <= FIX_0_16_INV_MID_DIST) {
        ceil_func = &draw_lit_ceiling;
    }

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
        z1_12_4, z2_12_4, tmap_info);


    lit_texture* lit_tex = tmap_info->tex;
    u16* dark_tex = lit_tex->dark;
    u16* mid_tex = lit_tex->mid;
    u16* light_tex = lit_tex->light;

    s16 x = beginx;
    u16 cnt = endx-x;

    u8** offset_ptr = &bmp_ptr_buf[x];
    //u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);

    u8* yclip_ptr = &(yclip[(x<<1)]);
    //u8* drawn_buf_ptr = &(drawn_buf[x]);

    u16* tex_col_ptr = tex_col_buffer;

    while(cnt--) {
        top_y_int = top_y_fix >> 16;
        bot_y_int = bot_y_fix >> 16;

        u8 min_drawable_y = *yclip_ptr++;
        u8 max_drawable_y = *yclip_ptr++;
        //if(min_drawable_y >= max_drawable_y) { continue; }
        u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);
        u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);

        u8* col_ptr = *offset_ptr;
        u8 col_drawn = col_ptr == NULL; //*drawn_buf_ptr;
        u16 tex_idx = *tex_col_ptr++;
        if(!col_drawn) {
            if(min_drawable_y < top_draw_y) {
                ceil_func(min_drawable_y, top_draw_y, col_ptr, ceil_params);
            }
            if(top_draw_y < bot_draw_y) {
                s16 cur_inv_z = cur_fix_inv_z;
                s16 pegged_top_y_int = pegged_top_y_fix>>16;

                u16* tex_column;
                if (cur_inv_z <= FIX_0_16_INV_DARK_DIST) {
                    tex_column = &dark_tex[tex_idx];
                } else if (cur_inv_z <= FIX_0_16_INV_MID_DIST) {
                    tex_column = &mid_tex[tex_idx];
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
            *offset_ptr++ = NULL;
            //*drawn_buf_ptr++ = 1;
        } else {
            offset_ptr++;
            //drawn_buf_ptr++;
        }

        top_y_fix += top_dy_per_dx;
        bot_y_fix += bot_dy_per_dx;
        pegged_top_y_fix += pegged_top_dy_per_dx;
        cur_fix_inv_z += fix_inv_dz_per_dx;
    }
    

    //flip();

    return; 
}


void draw_bot_pegged_wall(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 z1_12_4,     u16 z2_12_4,
              u16 inv_z1, u16 inv_z2,
              u16 window_min, u16 window_max,
            texmap_params *tmap_info, 
              light_params* floor_params, light_params* ceil_params,
              s16 x1_pegged_bot, s16 x2_pegged_bot) {


    u16 far_inv_z = min(inv_z1, inv_z2);
 
    draw_lit_plane_fp floor_func = &draw_lit_floor_light_only;
    if(far_inv_z <= FIX_0_16_INV_MID_DIST) {
        floor_func = &draw_lit_floor;
    }
    draw_lit_plane_fp ceil_func = &draw_lit_ceil_light_only;
    if(far_inv_z <= FIX_0_16_INV_MID_DIST) {
        ceil_func = &draw_lit_ceiling;
    }

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
        z1_12_4, z2_12_4, tmap_info);


    lit_texture* lit_tex = tmap_info->tex;
    u16* dark_tex = lit_tex->dark;
    u16* mid_tex = lit_tex->mid;
    u16* light_tex = lit_tex->light;

    s16 x = beginx;
    u16 cnt = endx-x;

    u8** offset_ptr = &bmp_ptr_buf[x];
    //u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    u8* yclip_ptr = &(yclip[(x<<1)]);
    //u8* drawn_buf_ptr = &(drawn_buf[x]);

    u16* tex_col_ptr = tex_col_buffer;

    while(cnt--) {
        top_y_int = top_y_fix >> 16;
        bot_y_int = bot_y_fix >> 16;

        u8 min_drawable_y = *yclip_ptr++;
        u8 max_drawable_y = *yclip_ptr++;
        //if(min_drawable_y >= max_drawable_y) { continue; }
        u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);
        u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);

        
        u8* col_ptr = *offset_ptr;
        u8 col_drawn = (col_ptr == NULL);
        //u8 col_drawn = *drawn_buf_ptr;
        u16 tex_idx = *tex_col_ptr++;
        if(!col_drawn) {
            if(min_drawable_y < top_draw_y) {
                ceil_func(min_drawable_y, top_draw_y, col_ptr, ceil_params);
            }
            if(top_draw_y < bot_draw_y) {
                s16 cur_inv_z = cur_fix_inv_z;
                s16 pegged_bot_y_int = pegged_bot_y_fix>>16;
            

                u16* tex_column;
                if (cur_inv_z <= FIX_0_16_INV_DARK_DIST) {
                    tex_column = &dark_tex[tex_idx];
                } else if (cur_inv_z <= FIX_0_16_INV_MID_DIST) {
                    tex_column = &mid_tex[tex_idx];
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
            *offset_ptr++ = NULL;
            //*drawn_buf_ptr++ = 1;
        } else {
            offset_ptr++;
            //drawn_buf_ptr++;
        }
        
        top_y_fix += top_dy_per_dx;
        bot_y_fix += bot_dy_per_dx;
        pegged_bot_y_fix += pegged_bot_dy_per_dx;
        cur_fix_inv_z += fix_inv_dz_per_dx;
    }

    

    //flip();

    return; 
}