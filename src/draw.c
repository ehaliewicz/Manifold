#include <genesis.h>
#include <kdebug.h>
#include "colors.h"
#include "clip_buf.h"
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

u8 cache_valid = 0;
void clear_2d_buffers() {
    for(int i = 0; i < 64; i++) { //SCREEN_WIDTH; i++) {
        yclip[i*2] = 0;
        yclip[i*2+1] = SCREEN_HEIGHT-1; // 8; idk why this is messed up 
    }
    cache_valid = 0;
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




void draw_vertical_line(s16 y0, s16 y1, u8 col, u8* col_ptr) {
    u8* cur_ptr = col_ptr + y0*128;
    u8* end_ptr = col_ptr + y1*128;
    //u8 cnt = 1+y1-y0;
    while(cur_ptr <= end_ptr) {
        *cur_ptr = col;
        cur_ptr += 128;
    }
}

/*
void run_texture_test() {
    BMP_vertical_clear();
    static u16 last_input = 0;

    //static s16 tex_col = 0;
    static s16 top_clip = 0;
    static s16 bot_clip = 0;
    static u16 x0 = 0; //RENDER_WIDTH/2;
    //static u16 x1 = 32;
    static u16 y0 = 0; //(SCREEN_HEIGHT/2)-(32/2);
    static u16 y1 = 128; //(SCREEN_HEIGHT/2)+(32/2);
    //u16 tex_idx = tex_col * TEX_HEIGHT;
    //u16* tex_column = &wall_light[tex_idx];

    u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x0]) : (&buf_1_column_offset_table[x0]);


    u16 input = JOY_readJoypad(JOY_1); 
    u16 new_input = (last_input ^ input) & input;
    u8 pushed_up = (input & BUTTON_UP);
    u8 pushed_down = (input & BUTTON_DOWN);
    u8 pushed_left = (input & BUTTON_LEFT);
    u8 pushed_right = (input & BUTTON_RIGHT);

    s16 clipped_y0 = y0+top_clip;
    s16 clipped_y1 = y1-bot_clip;
    s16 dy = clipped_y1-clipped_y0;
    if(input & BUTTON_A) {
        if(pushed_up && top_clip != 0) {
            top_clip--;
        } else if (pushed_down && clipped_y0 < clipped_y1) {
            top_clip++;
        }
    } else if(input & BUTTON_B)  {
        if(pushed_up && clipped_y1 > clipped_y0) {
            bot_clip++;
        } else if (pushed_down && bot_clip != 0) {
            bot_clip--;
        }
    
    } else {
        if(pushed_up && y0 != 0) {
            y0--;
            y1--; 
        } else if (pushed_down && y1 != SCREEN_HEIGHT-1) {
            y0++;
            y1++;
        }
    }
    last_input = input;
    for(int i = 0; i < 32; i++) {
        u8* col_ptr = *offset_ptr++;
        u16 tex_idx = i * TEX_HEIGHT;
        u16* tex_column = &wall_light[tex_idx];
        draw_texture_vertical_line(y0, y0+top_clip, y1, y1-bot_clip, col_ptr, tex_column);
    }
    showFPS(1);
    request_flip();
}
*/


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

const u32 long_color_table[256] = {
 0x00000000, 0x01010101, 0x02020202, 0x03030303,
0x04040404, 0x05050505, 0x06060606, 0x07070707,
0x08080808, 0x09090909, 0x0a0a0a0a, 0x0b0b0b0b,
0x0c0c0c0c, 0x0d0d0d0d, 0x0e0e0e0e, 0x0f0f0f0f,
0x10101010, 0x11111111, 0x12121212, 0x13131313,
0x14141414, 0x15151515, 0x16161616, 0x17171717,
0x18181818, 0x19191919, 0x1a1a1a1a, 0x1b1b1b1b,
0x1c1c1c1c, 0x1d1d1d1d, 0x1e1e1e1e, 0x1f1f1f1f,
0x20202020, 0x21212121, 0x22222222, 0x23232323,
0x24242424, 0x25252525, 0x26262626, 0x27272727,
0x28282828, 0x29292929, 0x2a2a2a2a, 0x2b2b2b2b,
0x2c2c2c2c, 0x2d2d2d2d, 0x2e2e2e2e, 0x2f2f2f2f,
0x30303030, 0x31313131, 0x32323232, 0x33333333,
0x34343434, 0x35353535, 0x36363636, 0x37373737,
0x38383838, 0x39393939, 0x3a3a3a3a, 0x3b3b3b3b,
0x3c3c3c3c, 0x3d3d3d3d, 0x3e3e3e3e, 0x3f3f3f3f,
0x40404040, 0x41414141, 0x42424242, 0x43434343,
0x44444444, 0x45454545, 0x46464646, 0x47474747,
0x48484848, 0x49494949, 0x4a4a4a4a, 0x4b4b4b4b,
0x4c4c4c4c, 0x4d4d4d4d, 0x4e4e4e4e, 0x4f4f4f4f,
0x50505050, 0x51515151, 0x52525252, 0x53535353,
0x54545454, 0x55555555, 0x56565656, 0x57575757,
0x58585858, 0x59595959, 0x5a5a5a5a, 0x5b5b5b5b,
0x5c5c5c5c, 0x5d5d5d5d, 0x5e5e5e5e, 0x5f5f5f5f,
0x60606060, 0x61616161, 0x62626262, 0x63636363,
0x64646464, 0x65656565, 0x66666666, 0x67676767,
0x68686868, 0x69696969, 0x6a6a6a6a, 0x6b6b6b6b,
0x6c6c6c6c, 0x6d6d6d6d, 0x6e6e6e6e, 0x6f6f6f6f,
0x70707070, 0x71717171, 0x72727272, 0x73737373,
0x74747474, 0x75757575, 0x76767676, 0x77777777,
0x78787878, 0x79797979, 0x7a7a7a7a, 0x7b7b7b7b,
0x7c7c7c7c, 0x7d7d7d7d, 0x7e7e7e7e, 0x7f7f7f7f,
0x80808080, 0x81818181, 0x82828282, 0x83838383,
0x84848484, 0x85858585, 0x86868686, 0x87878787,
0x88888888, 0x89898989, 0x8a8a8a8a, 0x8b8b8b8b,
0x8c8c8c8c, 0x8d8d8d8d, 0x8e8e8e8e, 0x8f8f8f8f,
0x90909090, 0x91919191, 0x92929292, 0x93939393,
0x94949494, 0x95959595, 0x96969696, 0x97979797,
0x98989898, 0x99999999, 0x9a9a9a9a, 0x9b9b9b9b,
0x9c9c9c9c, 0x9d9d9d9d, 0x9e9e9e9e, 0x9f9f9f9f,
0xa0a0a0a0, 0xa1a1a1a1, 0xa2a2a2a2, 0xa3a3a3a3,
0xa4a4a4a4, 0xa5a5a5a5, 0xa6a6a6a6, 0xa7a7a7a7,
0xa8a8a8a8, 0xa9a9a9a9, 0xaaaaaaaa, 0xabababab,
0xacacacac, 0xadadadad, 0xaeaeaeae, 0xafafafaf,
0xb0b0b0b0, 0xb1b1b1b1, 0xb2b2b2b2, 0xb3b3b3b3,
0xb4b4b4b4, 0xb5b5b5b5, 0xb6b6b6b6, 0xb7b7b7b7,
0xb8b8b8b8, 0xb9b9b9b9, 0xbabababa, 0xbbbbbbbb,
0xbcbcbcbc, 0xbdbdbdbd, 0xbebebebe, 0xbfbfbfbf,
0xc0c0c0c0, 0xc1c1c1c1, 0xc2c2c2c2, 0xc3c3c3c3,
0xc4c4c4c4, 0xc5c5c5c5, 0xc6c6c6c6, 0xc7c7c7c7,
0xc8c8c8c8, 0xc9c9c9c9, 0xcacacaca, 0xcbcbcbcb,
0xcccccccc, 0xcdcdcdcd, 0xcececece, 0xcfcfcfcf,
0xd0d0d0d0, 0xd1d1d1d1, 0xd2d2d2d2, 0xd3d3d3d3,
0xd4d4d4d4, 0xd5d5d5d5, 0xd6d6d6d6, 0xd7d7d7d7,
0xd8d8d8d8, 0xd9d9d9d9, 0xdadadada, 0xdbdbdbdb,
0xdcdcdcdc, 0xdddddddd, 0xdededede, 0xdfdfdfdf,
0xe0e0e0e0, 0xe1e1e1e1, 0xe2e2e2e2, 0xe3e3e3e3,
0xe4e4e4e4, 0xe5e5e5e5, 0xe6e6e6e6, 0xe7e7e7e7,
0xe8e8e8e8, 0xe9e9e9e9, 0xeaeaeaea, 0xebebebeb,
0xecececec, 0xedededed, 0xeeeeeeee, 0xefefefef,
0xf0f0f0f0, 0xf1f1f1f1, 0xf2f2f2f2, 0xf3f3f3f3,
0xf4f4f4f4, 0xf5f5f5f5, 0xf6f6f6f6, 0xf7f7f7f7,
0xf8f8f8f8, 0xf9f9f9f9, 0xfafafafa, 0xfbfbfbfb,
0xfcfcfcfc, 0xfdfdfdfd, 0xfefefefe, 0xffffffff,
};


void draw_native_vertical_line_unrolled(s16 y0, s16 y1, u32 full_col,  u8* col_ptr) {


    col_ptr = col_ptr + (y0<<1);
    //u32 full_col = long_color_table[col];
    u16 word_col = full_col & 0xFFFF; 
    u16* word_col_ptr = (u16*)col_ptr;

    //u16 word_col = (col<<8) | col;    

    u16 dy = (y1-y0);
    if(dy & 0b1) {
        __asm volatile(
            "move.w %1, (%0)+"
            : "+a" (word_col_ptr)
            : "d" (word_col)
        );
        //*word_col_ptr++ = word_col;
        dy--;
    }

    u32* lw_col_ptr = (u32*)word_col_ptr;

    __asm volatile(
        "sub.w #158, %0\t\n\
         neg.w %0\t\n\
         jmp movel_draw_table_%=(%%pc, %0.W)\t\n\
movel_draw_table_%=:\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+\t\n\
      move.l %1, (%2)+"
      :
      : "d" (dy), "d" (full_col), "a" (lw_col_ptr)
    );

    
    return;


    if(dy & 0b1) {
        __asm volatile(
            "move.l %1, (%0)+"
            : "+a" (lw_col_ptr)
            : "d" (full_col)
        );
        //*lw_col_ptr++ = full_col;
    }

    dy>>=1;
    if(dy & 0b1) {
        __asm volatile(
            "move.l %1, (%0)+\t\n\
            move.l %1, (%0)+"
            : "+a" (lw_col_ptr)
            : "d" (full_col)
        );
        //*lw_col_ptr++ = full_col;
        //*lw_col_ptr++ = full_col;
    }

    dy>>=1;
    
    while(dy--) {
        __asm volatile(
            "move.l %1, (%0)+\t\n\
            move.l %1, (%0)+\t\n\
            move.l %1, (%0)+\t\n\
            move.l %1, (%0)+"
            : "+a" (lw_col_ptr)
            : "d" (full_col)
            );

    }
    
    

    return;
}

// this is fast and efficient when each column is mostly empty space
// used for partially transparent textured walls
void draw_rle_monochrome_col(uint16_t *rle_column, uint8_t *col_ptr, uint16_t y0_clipped, uint16_t  y1_clipped, uint32_t full_col, s32 y0_fix, s32 y1_fix) {

  // scale is 16.16 scale factor, basically, it is 1/z in fixed point

  if(y1_fix <= y0_fix) { return; }
  if(y1_clipped <= y0_clipped) { return; }




  u16 num_runs; // = *rle_column++;
  __asm volatile(
        "move.w (%0)+, %1"
        : "+a" (rle_column), "=d" (num_runs)
  );
  if(num_runs == 0) { return; }

  //u8 byte_col = (col<<4) | col;
  u16 word_col = full_col & 0xFFFF;
  //u16 word_col = (byte_col<<8) | byte_col;
  u32 pix_fix = y1_fix - y0_fix;

  u32 cur_y_fix = y0_fix; 

  u32 pixels_per_texel = pix_fix >> 6;  // 16.16 / 16.0 = 16.16 //
  u16 pix_per_tex_16 = (pixels_per_texel >> 7); // 7.9
  s32 y_top_fix = y0_clipped << 16;
  s32 y_bot_fix = y1_clipped << 16;


  int i = num_runs;
  while(i--> 0) {
    u16 texels_skipped_fix_7; // = *rle_column++;
    __asm volatile(
        "move.w (%0)+, %1"
        : "+a" (rle_column), "=d" (texels_skipped_fix_7)
    );

    if(texels_skipped_fix_7) {
        u32 pixels_skipped_fix = pix_per_tex_16;
           __asm volatile(
            "mulu.w %1, %0"
            :  "+d" (pixels_skipped_fix)  // output
            : "d" (texels_skipped_fix_7) // input
        );
    
        cur_y_fix += pixels_skipped_fix;
    }

    if(cur_y_fix >= y_bot_fix) { return; }

    u16 texels_length_fix_7; // = *rle_column++;
    __asm volatile(
        "move.w (%0)+, %1"
        : "+a" (rle_column), "=d" (texels_length_fix_7)
    );


    u32 num_pixels_fix = pix_per_tex_16;
    __asm volatile(
        "mulu.w %1, %0"
        :  "+d" (num_pixels_fix)  // output
        : "d" (texels_length_fix_7) // input
    );    
    //s32 num_pixels_fix = texels_length * pixels_per_texel;
    s32 end_y_fix = (cur_y_fix + num_pixels_fix);

    if(end_y_fix >= y_top_fix) {
        
        s16 span_top_y = cur_y_fix>>16; //>>5;
        span_top_y = max(y0_clipped, min(span_top_y, y1_clipped));
        s16 span_bot_y = end_y_fix>>16; //>>5;

        span_bot_y = max(y0_clipped, min(span_bot_y, y1_clipped));
        s16 dy = span_bot_y - span_top_y;
        u16* col_span_ptr = &col_ptr[span_top_y<<1];

        

        if(dy & 0b1) {
            __asm volatile(
                "move.w %1, (%0)+"
                : "+a" (col_span_ptr)
                : "d" (word_col)
            );
        }
        dy >>= 1;
        if(dy) {
            u32* lw_col_ptr = (u32*)col_span_ptr;
            //u32 full_col = (word_col << 16) | word_col;
            while(dy--) {
                //*col_span_ptr++ = word_col;
                __asm volatile(
                    "move.l %1, (%0)+"
                    : "+a" (lw_col_ptr)
                    : "d" (full_col)
                );
            }
        }
    }

    cur_y_fix = end_y_fix;
    if(cur_y_fix >= y_bot_fix) { return; }

  }
}

#define FLAT_COLOR

void flip() {  
  //u8* dst_buf = (bmp_buffer_write == bmp_buffer_0) ? bmp_buffer_1 : bmp_buffer_0;
  //request_flip();
  //memcpy(bmp_buffer_read, bmp_buffer_write, 256*144/2);
  //waitMs(250);
  //request_flip();
        //request_flip();
        //waitMs(500);
        //BMP_clear();
    /*if(JOY_readJoypad(JOY_1) & BUTTON_A) {
        request_flip();
        waitMs(500);
        //BMP_clear();
    }*/
}


const uint16_t rle_glass[102] = {
//const uint8_t rle_glass[102] = {
    0, // num runs in column 0
    1, // num runs in column 1
    44<<7, // skip 44 texels
    1<<7, // draw 1 texels
    1, // num runs in column 2
    41<<7, // skip 41 texels
    3<<7, // draw 3 texels
    2, // num runs in column 3
    22<<7, // skip 22 texels
    2<<7, // draw 2 texels
    14<<7, // skip 14 texels
    3<<7, // draw 3 texels
    2, // num runs in column 4
    20<<7, // skip 20 texels
    3<<7, // draw 3 texels
    13<<7, // skip 13 texels
    3<<7, // draw 3 texels
    2, // num runs in column 5
    18<<7, // skip 18 texels
    3<<7, // draw 3 texels
    14<<7, // skip 14 texels
    1<<7, // draw 1 texels
    1, // num runs in column 6
    16<<7, // skip 16 texels
    3<<7, // draw 3 texels
    1, // num runs in column 7
    15<<7, // skip 15 texels
    2<<7, // draw 2 texels
    1, // num runs in column 8
    13<<7, // skip 13 texels
    3<<7, // draw 3 texels
    1, // num runs in column 9
    11<<7, // skip 11 texels
    3<<7, // draw 3 texels
    1, // num runs in column 10
    9<<7, // skip 9 texels
    3<<7, // draw 3 texels
    2, // num runs in column 11
    7<<7, // skip 7 texels
    3<<7, // draw 3 texels
    40<<7, // skip 40 texels
    1<<7, // draw 1 texels
    2, // num runs in column 12
    7<<7, // skip 7 texels
    1<<7, // draw 1 texels
    40<<7, // skip 40 texels
    2<<7, // draw 2 texels
    1, // num runs in column 13
    47<<7, // skip 47 texels
    3<<7, // draw 3 texels
    1, // num runs in column 14
    45<<7, // skip 45 texels
    3<<7, // draw 3 texels
    1, // num runs in column 15
    43<<7, // skip 43 texels
    3<<7, // draw 3 texels
    1, // num runs in column 16
    41<<7, // skip 41 texels
    3<<7, // draw 3 texels
    1, // num runs in column 17
    39<<7, // skip 39 texels
    3<<7, // draw 3 texels
    1, // num runs in column 18
    37<<7, // skip 37 texels
    3<<7, // draw 3 texels
    1, // num runs in column 19
    35<<7, // skip 35 texels
    3<<7, // draw 3 texels
    1, // num runs in column 20
    34<<7, // skip 34 texels
    1<<7, // draw 1 texels
    1, // num runs in column 21
    18<<7, // skip 18 texels
    1<<7, // draw 1 texels
    1, // num runs in column 22
    16<<7, // skip 16 texels
    2<<7, // draw 2 texels
    1, // num runs in column 23
    14<<7, // skip 14 texels
    3<<7, // draw 3 texels
    2, // num runs in column 24
    12<<7, // skip 12 texels
    3<<7, // draw 3 texels
    23, // skip 23 texels
    2<<7, // draw 2 texels
    2, // num runs in column 25
    10<<7, // skip 10 texels
    3<<7, // draw 3 texels
    23<<7, // skip 23 texels
    2<<7, // draw 2 texels
    2, // num runs in column 26
    9<<7, // skip 9 texels
    2<<7, // draw 2 texels
    23<<7, // skip 23 texels
    2<<7, // draw 2 texels
    1, // num runs in column 27
    34<<7, // skip 34 texels
    1<<7, // draw 1 texels
    0, // num runs in column 28
    0, // num runs in column 29
    0, // num runs in column 30
    0, // num runs in column 31
};

const uint16_t rle_glass_column_indexes[32] = {
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



/* for drawing partially transparent walls */
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
    u32 full_col = long_color_table[(LIGHT_STEEL_IDX<<4)|LIGHT_STEEL_IDX];

    for(;x < endx; x++) {
        u8 min_drawable_y = *yclip_ptr++;
        u8 max_drawable_y = *yclip_ptr++;
        //if(min_drawable_y >= max_drawable_y) { continue; }
        u8 top_draw_y = min_drawable_y; 
        u8 bot_draw_y = max_drawable_y; 

        u8* col_ptr = *offset_ptr++;
        if(top_draw_y < bot_draw_y) {
            //top_y_int = top_y_fix >> 16;
            //bot_y_int = bot_y_fix >> 16;
            tex_col_int = tex_col_fix>>16;

            
            uint16_t rle_glass_index = rle_glass_column_indexes[tex_col_int];
            tex_col_fix += tex_per_pix_fix;
            draw_rle_monochrome_col(&rle_glass[rle_glass_index], col_ptr, 
                                    min_drawable_y, max_drawable_y, 
                                    full_col, 
                                    top_y_fix, bot_y_fix);
                                    //top_y_int, bot_y_int);
            

        }
        //if(bot_draw_y < max_drawable_y) {
        //    draw_native_vertical_line_unrolled(bot_draw_y, max_drawable_y, floor_col, col_ptr);
        //}

        top_y_fix += top_dy_per_dx;
        bot_y_fix += bot_dy_per_dx;
    }
    
    flip();

    return; 
}

/* for drawing moving objects */
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

    u32 full_col = long_color_table[wall_col];

    s16 x = beginx;
    u8* yclip_ptr = &(clipping_buffer->clip_buffer[x<<1]);

    u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    
    for(;x < endx; x++) {
        u8 min_drawable_y = *yclip_ptr++;
        u8 max_drawable_y = *yclip_ptr++;
        //if(min_drawable_y >= max_drawable_y) { continue; }

        u8 top_draw_y = min_drawable_y; 
        u8 bot_draw_y = max_drawable_y; 

        u8* col_ptr = *offset_ptr++;
        if(top_draw_y < bot_draw_y) {
            top_y_int = top_y_fix >> 16;
            bot_y_int = bot_y_fix >> 16;
            u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);
            u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);

            if(top_draw_y < bot_draw_y) {
                draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, full_col, col_ptr);
            }


        }

        top_y_fix += top_dy_per_dx;
        bot_y_fix += bot_dy_per_dx;
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

    u8* yclip_ptr = &(clipping_buffer->clip_buffer[x<<1]);

    u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    
    u8 tex_col_int;

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

    
    flip();

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


void cache_floor_light_params(s16 rel_floor_height, u8 floor_col, s8 light_level, u32 avg_dist, u8 sloped, light_params* params) {
    u32 light_color, mid_color, dark_color;

    light_color = long_color_table[get_light_color(floor_col, light_level)];
    params->light_color = light_color;

    if(sloped) {
        //params->needs_lighting = 0;
        //return;
    }

    mid_color = long_color_table[get_mid_dark_color(floor_col, light_level)];
    dark_color = long_color_table[get_dark_color(floor_col, light_level)];
    if(light_color == mid_color && mid_color == dark_color) {
        params->needs_lighting = 0;
        return;
    }

    params->mid_color = mid_color;
    params->dark_color = dark_color;
    params->needs_lighting = 1;
    
    int table_idx = (-rel_floor_height)*4;

    //s16 dark_top = 0;
    s16 dark_bot = floor_light_positions[table_idx++];

    params->dark_y = dark_bot;

    table_idx++;

    //s16 mid_top = mid_dark_bot;
    s16 mid_bot = floor_light_positions[table_idx++];

    params->mid_y = mid_bot;
}


void cache_ceil_light_params(s16 rel_ceil_height, u8 ceil_col, s8 light_level, u32 avg_dist, u8 sloped, light_params* params) {
    u32 light_color, mid_color, dark_color;

    light_color = long_color_table[get_light_color(ceil_col, light_level)];
    params->light_color = light_color;
    if(sloped) {
      //params->needs_lighting = 0;
      //return;
    }
    
    mid_color = long_color_table[get_mid_dark_color(ceil_col, light_level)];
    dark_color = long_color_table[get_dark_color(ceil_col, light_level)];
    if(light_color == mid_color && mid_color == dark_color) {
        params->needs_lighting = 0;
        return;
    }
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

void draw_lit_floor_light_only(s16 floor_top_y, s16 floor_bot_y, u8* col_ptr, light_params* params) {
  draw_native_vertical_line_unrolled(floor_top_y, floor_bot_y, params->light_color, col_ptr);
}

void draw_lit_floor(s16 floor_top_y, s16 floor_bot_y, u8* col_ptr, light_params* params) {

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
}

void draw_lit_ceil_light_only(s16 ceil_top_y, s16 ceil_bot_y, u8* col_ptr, light_params* params) {
    draw_native_vertical_line_unrolled(ceil_top_y, ceil_bot_y, params->light_color, col_ptr);
}

void draw_lit_ceiling(s16 ceil_top_y, s16 ceil_bot_y, u8* col_ptr, light_params* params) {
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
}

typedef void (*draw_lit_plane_fp)(s16 top_y, s16 bot_y, u8* col_ptr, light_params* params);





void draw_upper_step(s16 x1, s16 x1_ytop, s16 nx1_ytop, s16 x2, s16 x2_ytop, s16 nx2_ytop, 
                     u16 inv_z1, u16 inv_z2,
                     u16 window_min, u16 window_max, u8 upper_color, s8 light_level, light_params* params) {

    u16 far_inv_z = min(inv_z1, inv_z2);
    //int needs_ceil_lighting = ceil_needs_lighting &&  (far_inv_z <= FIX_0_16_INV_MID_DIST); //(far_z >= MID_DIST);

    draw_lit_plane_fp ceil_func = &draw_lit_ceil_light_only;
    if(params->needs_lighting && far_inv_z <= FIX_0_16_INV_MID_DIST) {
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
    s32 fix_inv_dz_per_dx = (inv_dz << 16) / dx;

    s32 cur_fix_inv_z = inv_z1<<16;

    s16 skip_x = beginx - x1;

    if(skip_x > 0) {
        top_y_fix += (skip_x * top_dy_per_dx);
        ntop_y_fix += (skip_x * ntop_dy_per_dx);
        cur_fix_inv_z += (skip_x * fix_inv_dz_per_dx);
    }


    s16 endx = min(window_max, x2);

    s16 x = beginx;
    u8* yclip_ptr = &(yclip[x<<1]);
    

    u32 dark_full_col = long_color_table[get_dark_color(upper_color, light_level)];
    u32 light_full_col = long_color_table[get_light_color(upper_color, light_level)];
    u32 mid_full_col = long_color_table[get_mid_dark_color(upper_color, light_level)];

    u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    for(;x < endx; x++) {
        top_y_int = top_y_fix >> 16;
        ntop_y_int = ntop_y_fix >> 16;
        u8 min_drawable_y = *yclip_ptr;
        u8 max_drawable_y = *(yclip_ptr+1);
        //if(min_drawable_y >= max_drawable_y) { continue; }
        u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);
        u8 bot_draw_y = clamp(ntop_y_int, min_drawable_y, max_drawable_y);


        u8* col_ptr = *offset_ptr++;
        if(top_draw_y < bot_draw_y) {
            s16 cur_inv_z = cur_fix_inv_z>>16;

            if (cur_inv_z <= FIX_0_16_INV_DARK_DIST) { //(cur_z >= (DARK_DIST<<16))  {
                //draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, wall_col, col_ptr);
                draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, dark_full_col, col_ptr);
            } else if (cur_inv_z <= FIX_0_16_INV_MID_DIST) { //(cur_z >= (MID_DIST<<16)) {
                draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, mid_full_col, col_ptr);
            } else {
                draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, light_full_col, col_ptr);
            }

            //draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, upper_color, col_ptr);
            *yclip_ptr++ = bot_draw_y;
            yclip_ptr++;
        } else {
            yclip_ptr += 2;
        }
        if(min_drawable_y < top_draw_y) {
            ceil_func(min_drawable_y, top_draw_y, col_ptr, params);
        }

        top_y_fix += top_dy_per_dx;
        ntop_y_fix += ntop_dy_per_dx;
        cur_fix_inv_z += fix_inv_dz_per_dx;
    }
    
    flip();

    return; 
}

void draw_lower_step(s16 x1, s16 x1_ybot, s16 nx1_ybot, s16 x2, s16 x2_ybot, s16 nx2_ybot, 
                     u16 inv_z1, u16 inv_z2,
                     u16 window_min, u16 window_max, u8 lower_color, s8 light_level, light_params* params) {

    u16 far_inv_z = min(inv_z1, inv_z2);

    draw_lit_plane_fp floor_func = &draw_lit_floor_light_only;
    if(params->needs_lighting && far_inv_z <= FIX_0_16_INV_MID_DIST) {
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
    s16 fix_inv_dz_per_dx = inv_dz / dx; //(inv_dz << 16) / dx;

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

    u32 dark_full_col = long_color_table[get_dark_color(lower_color, light_level)];
    u32 light_full_col = long_color_table[get_light_color(lower_color, light_level)];
    u32 mid_full_col = long_color_table[get_mid_dark_color(lower_color, light_level)];


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
            //s16 cur_inv_z = cur_fix_inv_z>>16;
            s16 cur_inv_z = cur_fix_inv_z;

            if (cur_inv_z <= FIX_0_16_INV_DARK_DIST) { //(cur_z >= (DARK_DIST<<16))  {
                //draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, wall_col, col_ptr);
                draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, dark_full_col, col_ptr);
            } else if (cur_inv_z <= FIX_0_16_INV_MID_DIST) { //(cur_z >= (MID_DIST<<16)) {
                draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, mid_full_col, col_ptr);
            } else {
                draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, light_full_col, col_ptr);
            }
            //draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, lower_color, col_ptr);
            yclip_ptr++;
            *yclip_ptr++ = top_draw_y;
        } else {
            yclip_ptr += 2;
        }
        if(max_drawable_y > bot_draw_y) {
            floor_func(bot_draw_y, max_drawable_y, col_ptr, params);
        }

        bot_y_fix += bot_dy_per_dx;
        nbot_y_fix += nbot_dy_per_dx;
        cur_fix_inv_z += fix_inv_dz_per_dx;

    }
    
    flip();

    return; 
}

void draw_ceiling_update_clip(s16 x1, s16 x1_ytop, s16 x2, s16 x2_ytop,
                              s16 max_z,
                              u16 window_min, u16 window_max, light_params* params) {
        
    draw_lit_plane_fp ceil_func = &draw_lit_ceil_light_only;
    if(params->needs_lighting && (max_z >= MID_DIST)) {
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

    u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    for(;x < endx; x++) { 
        top_y_int = top_y_fix >> 16;
        u8 min_drawable_y = *yclip_ptr;
        u8 max_drawable_y = *(yclip_ptr+1);
        //if(min_drawable_y >= max_drawable_y) { continue; }
        u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);

        u8* col_ptr = *offset_ptr++;

        if(min_drawable_y < top_draw_y) {
            ceil_func(min_drawable_y, top_draw_y, col_ptr, params);
            *yclip_ptr++ = top_draw_y;
            yclip_ptr++;
        } else {
            yclip_ptr += 2;
        }

        top_y_fix += top_dy_per_dx;
    }
    
    flip();

    return; 
}
                            

void draw_floor_update_clip(s16 x1, s16 x1_ybot, s16 x2, s16 x2_ybot, 
                            s16 max_z,
                            u16 window_min, u16 window_max, light_params* params) {             
    draw_lit_plane_fp floor_func = &draw_lit_floor_light_only;
    if(params->needs_lighting && (max_z >= MID_DIST)) {
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

    u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);
    for(;x < endx; x++) {
        bot_y_int = bot_y_fix >> 16;
        u8 min_drawable_y = *yclip_ptr;
        u8 max_drawable_y = *(yclip_ptr+1);
        //if(min_drawable_y >= max_drawable_y) { continue; }
        u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);

        u8* col_ptr = *offset_ptr++;
        if(max_drawable_y > bot_draw_y) {
            floor_func(bot_draw_y, max_drawable_y, col_ptr, params);

            yclip_ptr++;
            *yclip_ptr++ = bot_draw_y;
        } else {
            yclip_ptr += 2;
        }

        bot_y_fix += bot_dy_per_dx;
    }
    
    flip();

    return; 
}

/*
void rasterize_line(s16 x1, s16 y1, s16 x2, s16 y2, 
                    u16 window_min, u16 window_max, 
                    s16* buf) {
    // 4 subpixel bits here

    s16 dy_fix = y2 - y1;
    s16 dx = x2-x1;

    fix32 dy_per_dx = (dy_fix<<12) / dx;

    fix32 y_fix = y1<<12; 
    s16 y_int;

    s16 beginx = x1;
    if(window_min > beginx) {
      s16 skip_x = window_min - beginx;
      beginx = window_min;
      y_fix += (skip_x * dy_per_dx);
    }

    s16 endx = min(window_max, x2);

    
    u16 cnt = endx-beginx;

    s16* buf_ptr = buf+beginx;
    
    
    //  __asm volatile(
    //      "rasterize_line_lp_%=:\t\n\
    //      swap %1\t\n\
    //      move.w %1, (%3)+\t\n\
    //      swap %1\t\n\
    //      add.l %2, %1\t\n\
    //      dbra %0, rasterize_line_lp_%="
    //      : 
    //      : "d" (cnt), "d" (y_fix), "d" (dy_per_dx), "a" (buf_ptr)
    //  ); 
    
    
    
    while(cnt--) {
        y_int = y_fix >> 16;
        *buf_ptr++ = y_int;

        y_fix += dy_per_dx;
    }
    

}

s16 top_buf[RENDER_WIDTH];
s16 bot_buf[RENDER_WIDTH];
u32 full_light_buf[SCREEN_WIDTH];


void light_increasing_z_line(s16 x1, s16 x2, u16 window_min, u16 window_max, s16 inv_z1, s16 inv_z2, u8 wall_col, s8 light_level) {

    u32 dark_full_col = long_color_table[get_dark_color(wall_col, light_level)];
    u32 light_full_col = long_color_table[get_light_color(wall_col, light_level)];
    u32 mid_full_col = long_color_table[get_mid_dark_color(wall_col, light_level)];

    s16 inv_dz = inv_z2 - inv_z1;
    s16 dx = x2-x1;
    s32 fix_inv_dz_per_dx = (inv_dz << 16) / dx;
    s32 cur_fix_inv_z = inv_z1<<16;

    s16 beginx = x1;
    if(window_min > beginx) {
      s16 skip_x = window_min - beginx;
      beginx = window_min;
      cur_fix_inv_z += (skip_x * fix_inv_dz_per_dx);
    }

    s16 endx = min(window_max, x2);
    u16 cnt = endx-beginx;

    u32* buf_ptr = full_light_buf+beginx;
    while(cnt--) {
    //for(;x < endx; x++) {
      s16 cur_inv_z = cur_fix_inv_z >> 16;

      if (cur_inv_z <= FIX_0_16_INV_DARK_DIST) {
        *buf_ptr++ = dark_full_col;
        goto fill_remaining_with_dark;
      } else if (cur_inv_z <= FIX_0_16_INV_MID_DIST) {
        *buf_ptr++ = mid_full_col;
      } else {
        *buf_ptr++ = light_full_col;
      }
        
      cur_fix_inv_z += fix_inv_dz_per_dx;
    }
    return;
    
    fill_remaining_with_dark:
    while(cnt--) {
      *buf_ptr++ = dark_full_col;
    }
}


void light_decreasing_z_line(s16 x1, s16 x2, u16 window_min, u16 window_max, s16 inv_z1, s16 inv_z2, u8 wall_col, s8 light_level) {

    u32 dark_full_col = long_color_table[get_dark_color(wall_col, light_level)];
    u32 light_full_col = long_color_table[get_light_color(wall_col, light_level)];
    u32 mid_full_col = long_color_table[get_mid_dark_color(wall_col, light_level)];

    s16 inv_dz = inv_z2 - inv_z1;
    s16 dx = x2-x1;
    s32 fix_inv_dz_per_dx = (inv_dz << 16) / dx;

    s32 cur_fix_inv_z = inv_z1<<16;

    s16 beginx = x1;
    if(window_min > beginx) {
      s16 skip_x = window_min - beginx;
      beginx = window_min;
      cur_fix_inv_z += (skip_x * fix_inv_dz_per_dx);
    }

    s16 endx = min(window_max, x2);
    u16 cnt = endx-beginx;

    u32* buf_ptr = full_light_buf+beginx;
    while(cnt--) {
      s16 cur_inv_z = cur_fix_inv_z >> 16;

      if (cur_inv_z <= FIX_0_16_INV_DARK_DIST) {
        *buf_ptr++ = dark_full_col;
      } else if (cur_inv_z <= FIX_0_16_INV_MID_DIST) {
        *buf_ptr++ = mid_full_col;
      } else {
        *buf_ptr++ = light_full_col;
        goto fill_remaining_with_light;
      }
        
      cur_fix_inv_z += fix_inv_dz_per_dx;
    }
    return;
    
    fill_remaining_with_light:
    while(cnt--) {
      *buf_ptr++ = light_full_col;
    }
}

void clip_and_draw_lit_wall(
  s16 x1, s16 x2, u16 window_min, u16 window_max, 
  s16* top_buf, s16* bot_buf, 
  draw_lit_plane_fp ceil_func, draw_lit_plane_fp floor_func) {

    s16 beginx = max(window_min, x1);
    s16 endx = min(window_max, x2);
    u16 cnt = endx-beginx;

    u32* light_buf_ptr = full_light_buf+beginx;
    s16* top_ptr = top_buf+beginx;
    s16* bot_ptr = bot_buf+beginx;
    u8* yclip_ptr = &(yclip[beginx<<1]);    
    u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[beginx]) : (&buf_1_column_offset_table[beginx]);

    while(cnt--) {
      s16 top_int = *top_ptr++;
      s16 bot_int = *bot_ptr++;
      u32 wall_full_col =  *light_buf_ptr++;
      //u32 wall_full_col = long_color_table[wall_col];
      u8 min_drawable_y = *yclip_ptr++;
      u8 max_drawable_y = *yclip_ptr++;
      u8* col_ptr = *offset_ptr++;
      if(min_drawable_y < max_drawable_y) {
        u8 top_draw_y = clamp(top_int, min_drawable_y, max_drawable_y);
        u8 bot_draw_y = clamp(bot_int, min_drawable_y, max_drawable_y);


        if(min_drawable_y < top_draw_y) {
            ceil_func(min_drawable_y, top_draw_y, col_ptr);
        }

        if(top_draw_y < bot_draw_y) {
            draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, wall_full_col, col_ptr);
        }

        if(bot_draw_y < max_drawable_y) {
            floor_func(bot_draw_y, max_drawable_y, col_ptr);
        }
      }
    }
}
  
void clip_and_draw_unlit_wall(s16 x1, s16 x2, 
                              u16 window_min, u16 window_max, 
                              s16* top_buf, s16* bot_buf, 
                              draw_lit_plane_fp ceil_func, u8 wall_col, draw_lit_plane_fp floor_func) {
    s16 beginx = max(window_min, x1);
    s16 endx = min(window_max, x2);
    u16 cnt = endx-beginx;

    s16* top_ptr = top_buf+beginx;
    s16* bot_ptr = bot_buf+beginx;
    u8* yclip_ptr = &(yclip[beginx<<1]);    
    u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[beginx]) : (&buf_1_column_offset_table[beginx]);
    u32 wall_full_col = long_color_table[wall_col];
    while(cnt--) {
      s16 top_int = *top_ptr++;
      s16 bot_int = *bot_ptr++;
      u8 min_drawable_y = *yclip_ptr++;
      u8 max_drawable_y = *yclip_ptr++;
      u8* col_ptr = *offset_ptr++;
      if(min_drawable_y < max_drawable_y) {
        u8 top_draw_y = clamp(top_int, min_drawable_y, max_drawable_y);
        u8 bot_draw_y = clamp(bot_int, min_drawable_y, max_drawable_y);
        if(min_drawable_y < top_draw_y) {
            ceil_func(min_drawable_y, top_draw_y, col_ptr);
        }
        if(top_draw_y < bot_draw_y) {
            draw_native_vertical_line_unrolled(top_draw_y, bot_draw_y, wall_full_col, col_ptr);
        }

        if(bot_draw_y < max_drawable_y) {
            floor_func(bot_draw_y, max_drawable_y, col_ptr);
        }
        *(yclip_ptr-2) = top_draw_y;
        *(yclip_ptr-1) = bot_draw_y;
      }
    }
}
*/

//#define NEW_DRAW_WALL

#ifdef NEW_DRAW_WALL



void draw_wall(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 inv_z1, u16 inv_z2,
              u16 window_min, u16 window_max,
              u8 ceil_col, u8 wall_col, u8 floor_col, s8 light_level) {

  u16 far_inv_z = min(inv_z1, inv_z2);
  draw_lit_plane_fp floor_func = &draw_lit_floor_light_only;
  if(floor_needs_lighting && far_inv_z <= FIX_0_16_INV_MID_DIST) {
      floor_func = &draw_lit_floor;
  }
  draw_lit_plane_fp ceil_func = &draw_lit_ceil_light_only;
  if(ceil_needs_lighting && far_inv_z <= FIX_0_16_INV_MID_DIST) {
      ceil_func = &draw_lit_ceiling;
  }

  rasterize_line(x1, x1_ytop, x2, x2_ytop, window_min, window_max, top_buf);
  rasterize_line(x1, x1_ybot, x2, x2_ybot, window_min, window_max, bot_buf);

  //u8 wall_lighting_needed = 1;
  //if(inv_z1 > FIX_0_16_INV_MID_DARK_DIST && inv_z2 > FIX_0_16_INV_MID_DARK_DIST) {
  //  wall_lighting_needed = 0;
  //} else if (inv_z1 < FIX_0_16_INV_DARK_DIST && inv_z2 < FIX_0_16_INV_DARK_DIST) {
  //  wall_lighting_needed = 0;
  //}
  if(inv_z1 == inv_z2) { //} || wall_lighting_needed == 0) {
    if (inv_z1 <= FIX_0_16_INV_DARK_DIST) {
      wall_col = get_dark_color(wall_col, light_level);
    } else if (inv_z1 <= FIX_0_16_INV_MID_DIST) {
      wall_col = get_mid_dark_color(wall_col, light_level);
    } else {
      wall_col = get_light_color(wall_col, light_level);
    }
    clip_and_draw_unlit_wall(x1, x2, window_min, window_max, top_buf, bot_buf, ceil_func, wall_col, floor_func);
  } else if(inv_z1 > inv_z2) {
    light_increasing_z_line(x1, x2, window_min, window_max, inv_z1, inv_z2, wall_col, light_level); 
    clip_and_draw_lit_wall(x1, x2, window_min, window_max, top_buf, bot_buf, ceil_func, floor_func);
  } else if (inv_z1 < inv_z2) {
    light_increasing_z_line(x1, x2, window_min, window_max, inv_z1, inv_z2, wall_col, light_level); 
    clip_and_draw_lit_wall(x1, x2, window_min, window_max, top_buf, bot_buf, ceil_func, floor_func);
  }
}

#else 
/*
typedef int SItype __attribute__ ((mode (SI)));
typedef unsigned int USItype __attribute__ ((mode (SI)));
typedef float DFtype __attribute__ ((mode (DF)));

DFtype
__floatunsidf (USItype u)
{
  SItype s = (SItype) u;
  DFtype r = (DFtype) s;
  if (s < 0)
    r += (DFtype)2.0 * (DFtype) ((USItype) 1
				 << (sizeof (USItype) * __CHAR_BIT__ - 1));
  return r;
}
*/

typedef int SItype __attribute__ ((mode (SI)));
typedef unsigned int USItype __attribute__ ((mode (SI)));
typedef float DFtype __attribute__ ((mode (DF)));

/*
DFtype floatunsidf (USItype u) {
  SItype s = (SItype) u;
  DFtype r = (DFtype) s;
  if (s < 0)
    r += (DFtype)2.0 * (DFtype) ((USItype) 1
				 << (sizeof (USItype) * __CHAR_BIT__ - 1));
  return r;
}
*/
typedef float SFtype __attribute__ ((mode (SF)));

SFtype
floatunsisf (USItype u)
{
  SItype s = (SItype) u;
  if (s < 0)
    {
      /* As in expand_float, compute (u & 1) | (u >> 1) to ensure
	 correct rounding if a nonzero bit is shifted out.  */
      return (SFtype) 2.0 * (SFtype) (SItype) ((u & 1) | (u >> 1));
    }
  else
    return (SFtype) s;
}

void draw_wall(s16 x1, s16 x1_ytop, s16 x1_ybot,
              s16 x2, s16 x2_ytop, s16 x2_ybot,
              u16 z1_12_4,     u16 z2_12_4,
              u16 inv_z1, u16 inv_z2,
              u16 window_min, u16 window_max,
              s8 light_level, texmap_info *tmap_info, 
              light_params* floor_params, light_params* ceil_params) {

    

    u16 far_inv_z = min(inv_z1, inv_z2);
 
    draw_lit_plane_fp floor_func = &draw_lit_floor_light_only;
    if(floor_params->needs_lighting && far_inv_z <= FIX_0_16_INV_MID_DIST) {
        floor_func = &draw_lit_floor;
    }
    draw_lit_plane_fp ceil_func = &draw_lit_ceil_light_only;
    if(ceil_params->needs_lighting && far_inv_z <= FIX_0_16_INV_MID_DIST) {
        ceil_func = &draw_lit_ceiling;
    }


    const u8 needs_perspective = tmap_info->needs_perspective;

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


    
    //s16 dz_12_4 = z2_12_4-z1_12_4;
    s16 inv_dz = inv_z2 - inv_z1;
    
    s32 fix_inv_dz_per_dx = (inv_dz << 16) / dx;

    s32 cur_fix_inv_z = inv_z1<<16;

    //u32 left_u_20 = tmap_info->left_u;
    //u32 right_u_20 = tmap_info->right_u;

    float z1_float = z1_12_4/16.0;
    float z2_float = z2_12_4/16.0;

    u32 left_u_16 = tmap_info->left_u;
    u32 right_u_16 = tmap_info->right_u;

    u32 one_over_z_26, one_over_z_end_26; // 16.16
    s32 d_one_over_z_dx_26;

    u32 u_over_z_23, u_over_z_end_23;
    u32 d_u_over_z_dx_23;


    u32 u_fix_16;
    
    
    //u32 du_over_dx_16 = (((right_u_20-left_u_20)>>4)/dx); // 16.16
    u32 du_over_dx_16 = (right_u_16-left_u_16)/dx; // 16.16
    
    if(needs_perspective) {
        
        one_over_z_26 = (1<<(16+TRANS_Z_FRAC_BITS+10))/(z1_12_4);
        one_over_z_end_26 = (1<<(16+TRANS_Z_FRAC_BITS+10))/(z2_12_4);


        s32 d_one_over_z_26 = (one_over_z_end_26 - one_over_z_26);
        d_one_over_z_dx_26 = (d_one_over_z_26/dx);
        

        u_over_z_23 = (left_u_16<<(TRANS_Z_FRAC_BITS+7))/z1_12_4;      // 12.20
        u_over_z_end_23 = (right_u_16<<(TRANS_Z_FRAC_BITS+7))/z2_12_4; // 12.20
        //u_over_z_23 = left_u_16 * (one_over_z_26>>19);
        //u_over_z_end_23 = right_u_16 * (one_over_z_end_26>>19);

        //float d_u_over_z_float = u_over_z_end - u_over_z;
        s32 d_u_over_z_23 = (u_over_z_end_23 - u_over_z_23);

        //d_u_over_z_dx = d_u_over_z_float / dx;
        d_u_over_z_dx_23 = d_u_over_z_23/dx;
        if(0) {
            /*
            KLog_S1("dx: ", dx);
            KLog_S1("z1: ", z1_12_4);
            KLog_S1("z1_int: ", z1_12_4>>TRANS_Z_FRAC_BITS);
            KLog_S1("one over z left: ", one_over_z_16);
            KLog_S1("z2: ", z2_12_4);
            KLog_S1("z2_int: ", z2_12_4>>TRANS_Z_FRAC_BITS);
            KLog_S1("one over z right: ", one_over_z_end_16);
            KLog_S1("d one over z: ", d_one_over_z_16);
            KLog_S1("d one over z per dx: ", d_one_over_z_dx_16);
            */
            //KLog_S1("u_left_20: ", left_u_20);
            //KLog_S1("u over z: ", u_over_z_20);
            //KLog_S1("u_right_20: ", right_u_20);
            //KLog_S1("u over z right: ", u_over_z_end_20);
            //KLog_S1("d_u_over_z: ", d_u_over_z_20);
            //KLog_S1("d_u_over_z_per_dx: ", d_u_over_z_dx_20);
        }
        
    } else {
        u_fix_16 = left_u_16;
    }

    
    lit_texture lit_tex;
    u8 tex_width_shift;
    u8 tex_width;

    const u32 scaled_du_over_dx_fix = du_over_dx_16 << TEX_WIDTH_SHIFT;
    const u32 test_du_over_dx_fix = scaled_du_over_dx_fix>>16;
    
    texture_set tex = sci_fi_wall_texture;
    //if(test_du_over_dx_fix >= 2) {
    //    lit_tex = tex.mip_far;
    //    tex_width_shift = FAR_MIP_WIDTH_SHIFT;
    //    tex_width = FAR_MIP_WIDTH;
    //} else if (test_du_over_dx_fix >= 1) {
         lit_tex = tex.mip_mid;
        tex_width_shift = MID_MIP_WIDTH_SHIFT;
        tex_width = MID_MIP_WIDTH;
    //} else {
    //    lit_tex = tex.mip_near;
    //    tex_width_shift = NEAR_MIP_WIDTH_SHIFT;
    //    tex_width = NEAR_MIP_WIDTH;
    //}


    s16 beginx = max(x1, window_min);
    s16 endx = min(window_max, x2);
    s16 skip_x = beginx - x1;
    

    if(skip_x > 0) {
        top_y_fix += (skip_x * top_dy_per_dx);
        bot_y_fix += (skip_x * bot_dy_per_dx);
        cur_fix_inv_z += (skip_x * fix_inv_dz_per_dx);
        if(needs_perspective) {
            one_over_z_26 += (skip_x * d_one_over_z_dx_26);
            u_over_z_23 += skip_x * d_u_over_z_dx_23;
            if(0) {
                //KLog_S1("!!!!! skipping n pixels", skip_x);
                //KLog_S1("1/z skip amount: ", skip_x*d_one_over_z_dx_16);
                //KLog_S1("u/z skip amount: ", skip_x * d_u_over_z_dx_20);
                //KLog_S1("result one over z: ", one_over_z_16);
                //KLog_S1("result u over z: ", u_over_z_20);
            }
        } else {
            u_fix_16 += skip_x * du_over_dx_16;
        }
    }

    u16* dark_tex;
    u16* mid_tex;
    u16* light_tex;


    switch(light_level) {
      case -2:
        light_tex = lit_tex.dark;
        mid_tex = lit_tex.dark;
        dark_tex = lit_tex.dark;
        break;
      case -1:
        light_tex = lit_tex.mid;
        mid_tex = lit_tex.dark;
        dark_tex = lit_tex.dark;
        break;
      case 0:
        light_tex = lit_tex.light;
        mid_tex = lit_tex.mid;
        dark_tex = lit_tex.dark;
        break;
      case 1:
        dark_tex = lit_tex.mid;
        mid_tex = lit_tex.light;
        light_tex = lit_tex.light;
        break;
      case 2:
        dark_tex = lit_tex.light;
        mid_tex = lit_tex.light;
        light_tex = lit_tex.light;
        break;
    }

    
    s16 x = beginx;
    u16 cnt = endx-x;

    u8** offset_ptr = (bmp_buffer_write == bmp_buffer_0) ? (&buf_0_column_offset_table[x]) : (&buf_1_column_offset_table[x]);

    u8* yclip_ptr = &(yclip[(x<<1)]);
    //u16 repetitions = tmap_info->repetitions;

    while(cnt--) {
        top_y_int = top_y_fix >> 16; // 16;
        bot_y_int = bot_y_fix >> 16; // 16;

        u8 min_drawable_y = *yclip_ptr++;
        u8 max_drawable_y = *yclip_ptr++;
        //if(min_drawable_y >= max_drawable_y) { continue; }
        u8 top_draw_y = clamp(top_y_int, min_drawable_y, max_drawable_y);
        u8 bot_draw_y = clamp(bot_y_int, min_drawable_y, max_drawable_y);

        
        u8* col_ptr = *offset_ptr++;
        if(min_drawable_y < top_draw_y) {
            ceil_func(min_drawable_y, top_draw_y, col_ptr, ceil_params);
        }
        if(top_draw_y < bot_draw_y) {
            s16 cur_inv_z = cur_fix_inv_z >> 16;

            u32 tex_col;
            if(needs_perspective) {

                u32 z = (1<<26)/one_over_z_26;

                u32 u_23 = u_over_z_23 * z;

                u32 u_scaled_by_width = (u_23 >> (23-tex_width_shift));

                tex_col = u_scaled_by_width & (tex_width-1);
                

                if(0) {
                    //KLog_S1("--------- x: ", x++);
                    
                    //KLog_S1("cur one over z: ", one_over_z_16);
                    //KLog_S1("cur z: ", z);
                    //KLog_S1("cur u_over_z: ", u_over_z_16);
                    //KLog_S1("cur u fix: ", u_fix);
                    //KLog_S1("tex_col_repeat: ", tex_col_repeat);
                }

            } else {
                //tex_col = ((u_fix<<tex_width_shift)>>16) & (tex_width-1);
                tex_col = ((u_fix_16<<tex_width_shift)>>16) & (tex_width-1);

            }
            
            u32 tex_idx = tex_col*TEX_HEIGHT;
        

            u16* tex_column;

            if (cur_inv_z <= FIX_0_16_INV_DARK_DIST) {
                //tex_column = &dark_texture[tex_col_offset];
                tex_column = &dark_tex[tex_idx];
            } else if (cur_inv_z <= FIX_0_16_INV_MID_DIST) {
                //tex_column = &mid_texture[tex_col_offset];
                tex_column = &mid_tex[tex_idx];
            } else {
                //tex_column = &light_texture[tex_col_offset];
                tex_column = &light_tex[tex_idx];
            }

            draw_texture_vertical_line(top_y_int, top_draw_y, bot_y_int, bot_draw_y, col_ptr, tex_column);

        }
        if(bot_draw_y < max_drawable_y) {
            floor_func(bot_draw_y, max_drawable_y, col_ptr, floor_params);
        }

        top_y_fix += top_dy_per_dx;
        bot_y_fix += bot_dy_per_dx;
        cur_fix_inv_z += fix_inv_dz_per_dx;
        if(needs_perspective) {
            one_over_z_26 += d_one_over_z_dx_26;
            u_over_z_23 += d_u_over_z_dx_23;
        } else {
            u_fix_16 += du_over_dx_16;
        }

    }

    flip();

    return; 
}
#endif