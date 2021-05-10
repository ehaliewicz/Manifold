#include <genesis.h>
#include "common.h"

volatile u32 vram_copy_dst;
volatile u8* vram_copy_src;

vu8 vint_flipping;
vu8 vint_flip_requested;

volatile u32 in_use_vram_copy_dst;
volatile u8* in_use_vram_copy_src;

#define FULL_BYTES (SCREEN_WIDTH*SCREEN_HEIGHT)
#define FULL_WORDS (FULL_BYTES/2)
#define HALF_WORDS (FULL_WORDS/2)
#define HALF_BYTES (FULL_BYTES/2)
#define QUARTER_WORDS (HALF_WORDS/2)
#define QUARTER_BYTES (HALF_BYTES/2)


u16 getDMAWriteOffset(u16 x, u16 y) {
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

void reset_dma_framebuffer() {
    vint_flip_requested = 0;
    vint_flipping = 0;
}

void after_flip_vscroll_adjustment() {
    u16 vscr;
    if(vram_copy_src == bmp_buffer_1) {
        vscr = (BMP_PLANHEIGHT * 8) / 2;
    } else {
        vscr = 0;
    }
    VDP_setVerticalScroll(BG_A, vscr);
}

void copy_quarter_words(u8* src, u32 dst) {
    DMA_doDma(DMA_VRAM,
        src,
        dst,
        QUARTER_WORDS, 4);
}

void do_vint_flip() {
    
    if(vint_flipping == 1) {

        // not finished
        // complete second half here

        // second half for framebuffer 0
        // first half for framebuffer 1

        
        if(in_use_vram_copy_src == bmp_buffer_0) {
            // draw second quarter of framebuffer to third quarter of VRAM framebuffer

            copy_quarter_words(
                (u8*)in_use_vram_copy_src+QUARTER_BYTES, 
                in_use_vram_copy_dst+HALF_BYTES);
            copy_quarter_words(
                (u8*)in_use_vram_copy_src+QUARTER_BYTES + HALF_BYTES,
                in_use_vram_copy_dst+2+HALF_BYTES);
            //copy_quarter_words(
            //    (u8*)in_use_vram_copy_src+QUARTER_BYTES,
            //    in_use_vram_copy_dst+2+HALF_BYTES);
        } else {
            copy_quarter_words( 
                (u8*)in_use_vram_copy_src, 
                in_use_vram_copy_dst);
            //copy_quarter_words(
            //    (u8*)in_use_vram_copy_src,
            //    in_use_vram_copy_dst+2);
            copy_quarter_words(
                (u8*)in_use_vram_copy_src+HALF_BYTES,
                in_use_vram_copy_dst+2);
        }
        

        after_flip_vscroll_adjustment();

        vint_flipping = 0;
    } else if(vint_flip_requested) {
        vint_flipping = 1;
        vint_flip_requested = 0;
        in_use_vram_copy_dst = vram_copy_dst;
        in_use_vram_copy_src = vram_copy_src;


        // first half for framebuffer 1
        // second half for framebuffer 0
        if(in_use_vram_copy_src == bmp_buffer_0) {
            copy_quarter_words(
                (u8*)in_use_vram_copy_src, 
                in_use_vram_copy_dst);
            //copy_quarter_words(
            //    (u8*)in_use_vram_copy_src,
            //    in_use_vram_copy_dst+2);
            copy_quarter_words(
                (u8*)in_use_vram_copy_src+HALF_BYTES,
                in_use_vram_copy_dst+2);
        } else { 
            copy_quarter_words(
                (u8*)in_use_vram_copy_src+QUARTER_BYTES, 
                in_use_vram_copy_dst+HALF_BYTES);
            //copy_quarter_words(
            //    (u8*)in_use_vram_copy_src+QUARTER_BYTES, 
            //    in_use_vram_copy_dst+2+HALF_BYTES);
            copy_quarter_words(
                (u8*)in_use_vram_copy_src+QUARTER_BYTES+HALF_BYTES,
                in_use_vram_copy_dst+2+HALF_BYTES);
        }
    }


}



#define FB0MIDDLEINDEX (BMP_BASETILEINDEX + (BMP_CELLWIDTH/2 * BMP_CELLHEIGHT))
#define FB0MIDDLE (FB0MIDDLEINDEX*32)

void request_dma_flip() {
    while(vint_flip_requested || vint_flipping) {
        // vblank is behind one request
        // wait until it has started, and then we can safely flip to the next framebuffer
        //return;
    }


    if(bmp_buffer_write == bmp_buffer_0) {
        vram_copy_src = bmp_buffer_0;
        vram_copy_dst = BMP_FB0TILE;
        bmp_buffer_write = bmp_buffer_1;
        bmp_buffer_read = bmp_buffer_0;
    } else {
        vram_copy_src = bmp_buffer_1;
        vram_copy_dst = FB0MIDDLE; //BMP_FB1TILE;
        bmp_buffer_write = bmp_buffer_0;
        bmp_buffer_read = bmp_buffer_0;
    }
    vint_flip_requested = 1;
}
