#include <genesis.h>
#include "cart_ram.h"
#include "colors.h"
#include "config.h"
#include "utils.h"
#include "texture.h"
#include "textures.h"
#include "tex_tables_lookup.h"


// 64 pixels tall
// 64 pixels wide
// which means 64 rasterized columns

u16* mid_texture = (u16*)wall_mid; //hello_tex_0_mid;
u16* light_texture = (u16*)wall_light; //hello_tex_0_light;
u16* dark_texture = (u16*)wall_dark; //hello_tex_0_dark;
int ticks = 50;


void tick_texture() {
    return;
    ticks--;
    /*
    if(ticks == 0) {
        if(mid_texture == hello_tex_0_mid) {
            mid_texture = hello_tex_1_mid;
            light_texture = hello_tex_1_light;
            dark_texture = hello_tex_1_dark;
            ticks = 15;
        } else {
            mid_texture = hello_tex_0_mid;
            light_texture = hello_tex_0_light;
            dark_texture = hello_tex_0_dark;
            ticks = 35;
        }
    }
    */
}
// goes up to maybe 1024
static s16 max_dy = 0;

//void draw_unclipped_vertical_line(s16 y0, s16 y1, s16 unclipped_y1, s16 y1, u8* col_ptr, u8 tex_col_idx) {}

u32 dv_per_dy_tab[1024] = {
8192, // index should never be 0
8192, 4096, 2730, 2048, 1638, 
1365, 1170, 1024, 910, 819, 
744, 682, 630, 585, 546, 
512, 481, 455, 431, 409, 
390, 372, 356, 341, 327, 
315, 303, 292, 282, 273, 
264, 256, 248, 240, 234, 
227, 221, 215, 210, 204, 
199, 195, 190, 186, 182, 
178, 174, 170, 167, 163, 
160, 157, 154, 151, 148, 
146, 143, 141, 138, 136, 
134, 132, 130, 128, 126, 
124, 122, 120, 118, 117, 
115, 113, 112, 110, 109, 
107, 106, 105, 103, 102, 
101, 99, 98, 97, 96, 
95, 94, 93, 92, 91, 
90, 89, 88, 87, 86, 
85, 84, 83, 82, 81, 
81, 80, 79, 78, 78, 
77, 76, 75, 75, 74, 
73, 73, 72, 71, 71, 
70, 70, 69, 68, 68, 
67, 67, 66, 66, 65, 
65, 64, 64, 63, 63, 
62, 62, 61, 61, 60, 
60, 59, 59, 58, 58, 
58, 57, 57, 56, 56, 
56, 55, 55, 54, 54, 
54, 53, 53, 53, 52, 
52, 52, 51, 51, 51, 
50, 50, 50, 49, 49, 
49, 49, 48, 48, 48, 
47, 47, 47, 47, 46, 
46, 46, 46, 45, 45, 
45, 45, 44, 44, 44, 
44, 43, 43, 43, 43, 
42, 42, 42, 42, 42, 
41, 41, 41, 41, 40, 
40, 40, 40, 40, 39, 
39, 39, 39, 39, 39, 
38, 38, 38, 38, 38, 
37, 37, 37, 37, 37, 
37, 36, 36, 36, 36, 
36, 36, 35, 35, 35, 
35, 35, 35, 35, 34, 
34, 34, 34, 34, 34, 
33, 33, 33, 33, 33, 
33, 33, 33, 32, 32, 
32, 32, 32, 32, 32, 
32, 31, 31, 31, 31, 
31, 31, 31, 31, 30, 
30, 30, 30, 30, 30, 
30, 30, 30, 29, 29, 
29, 29, 29, 29, 29, 
29, 29, 28, 28, 28, 
28, 28, 28, 28, 28, 
28, 28, 27, 27, 27, 
27, 27, 27, 27, 27, 
27, 27, 27, 26, 26, 
26, 26, 26, 26, 26, 
26, 26, 26, 26, 26, 
25, 25, 25, 25, 25, 
25, 25, 25, 25, 25, 
25, 25, 24, 24, 24, 
24, 24, 24, 24, 24, 
24, 24, 24, 24, 24, 
24, 23, 23, 23, 23, 
23, 23, 23, 23, 23, 
23, 23, 23, 23, 23, 
23, 22, 22, 22, 22, 
22, 22, 22, 22, 22, 
22, 22, 22, 22, 22, 
22, 22, 21, 21, 21, 
21, 21, 21, 21, 21, 
21, 21, 21, 21, 21, 
21, 21, 21, 21, 21, 
20, 20, 20, 20, 20, 
20, 20, 20, 20, 20, 
20, 20, 20, 20, 20, 
20, 20, 20, 20, 19, 
19, 19, 19, 19, 19, 
19, 19, 19, 19, 19, 
19, 19, 19, 19, 19, 
19, 19, 19, 19, 19, 
19, 18, 18, 18, 18, 
18, 18, 18, 18, 18, 
18, 18, 18, 18, 18, 
18, 18, 18, 18, 18, 
18, 18, 18, 18, 18, 
17, 17, 17, 17, 17, 
17, 17, 17, 17, 17, 
17, 17, 17, 17, 17, 
17, 17, 17, 17, 17, 
17, 17, 17, 17, 17, 
17, 16, 16, 16, 16, 
16, 16, 16, 16, 16, 
16, 16, 16, 16, 16, 
16, 16, 16, 16, 16, 
16, 16, 16, 16, 16, 
16, 16, 16, 16, 16, 
16, 16, 15, 15, 15, 
15, 15, 15, 15, 15, 
15, 15, 15, 15, 15, 
15, 15, 15, 15, 15, 
15, 15, 15, 15, 15, 
15, 15, 15, 15, 15, 
15, 15, 15, 15, 15, 
15, 14, 14, 14, 14, 
14, 14, 14, 14, 14, 
14, 14, 14, 14, 14, 
14, 14, 14, 14, 14, 
14, 14, 14, 14, 14, 
14, 14, 14, 14, 14, 
14, 14, 14, 14, 14, 
14, 14, 14, 14, 14, 
13, 13, 13, 13, 13, 
13, 13, 13, 13, 13, 
13, 13, 13, 13, 13, 
13, 13, 13, 13, 13, 
13, 13, 13, 13, 13, 
13, 13, 13, 13, 13, 
13, 13, 13, 13, 13, 
13, 13, 13, 13, 13, 
13, 13, 13, 13, 13, 
12, 12, 12, 12, 12, 
12, 12, 12, 12, 12, 
12, 12, 12, 12, 12, 
12, 12, 12, 12, 12, 
12, 12, 12, 12, 12, 
12, 12, 12, 12, 12, 
12, 12, 12, 12, 12, 
12, 12, 12, 12, 12, 
12, 12, 12, 12, 12, 
12, 12, 12, 12, 12, 
12, 12, 11, 11, 11, 
11, 11, 11, 11, 11, 
11, 11, 11, 11, 11, 
11, 11, 11, 11, 11, 
11, 11, 11, 11, 11, 
11, 11, 11, 11, 11, 
11, 11, 11, 11, 11, 
11, 11, 11, 11, 11, 
11, 11, 11, 11, 11, 
11, 11, 11, 11, 11, 
11, 11, 11, 11, 11, 
11, 11, 11, 11, 11, 
11, 11, 11, 11, 10, 
10, 10, 10, 10, 10, 
10, 10, 10, 10, 10, 
10, 10, 10, 10, 10, 
10, 10, 10, 10, 10, 
10, 10, 10, 10, 10, 
10, 10, 10, 10, 10, 
10, 10, 10, 10, 10, 
10, 10, 10, 10, 10, 
10, 10, 10, 10, 10, 
10, 10, 10, 10, 10, 
10, 10, 10, 10, 10, 
10, 10, 10, 10, 10, 
10, 10, 10, 10, 10, 
10, 10, 10, 10, 10, 
10, 10, 10, 10, 9, 
9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 
8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 
8, 8, 8, 

};

void draw_texline_asm(u16 unclipped_dy, u16 clip_top, u16 dy, u16* word_col_ptr, u16* tex_column) {
    //u32 dv_per_dy = (32<<8) / unclipped_dy; 
    u32 dv_per_dy = dv_per_dy_tab[unclipped_dy];


    u32 v_fix = clip_top; 

    __asm volatile(
        "mulu.w %1, %0\t\n"
        : "+d" (v_fix)
        : "d" (dv_per_dy)
    );

    if(dy & 0b1) {
        //*word_col_ptr++ = tex_column[v_fix>>8];
        //v_fix += dv_per_dy;
    }

    dy>>=1;
    if(!dy) {
        return;
    }

    u32* lw_col_ptr = (u32*)word_col_ptr;
    u32* lw_tex_column = (u32*)tex_column;
    dv_per_dy<<=1;
    v_fix <<= 1;
    u8 v_low = (v_fix&0xFF);
    u8 v_high = (v_fix&0xFF00)>>8;
    u8 dvdy_low = (dv_per_dy&0xFF);
    u8 dvdy_high = (dv_per_dy&0xFF00)>>8;
    u16 scratch = 0;
    dy--;
    

    // 8.5 cycles per pixel without branch
    // 9.75 with branch
    /*
    __asm volatile(
        "loop%=:\t\n\
         move.w %1, %4\t\n\
         add.w %4, %4\t\n\
         move.w 0(%6, %4.w), (%7)+\t\n\
         add.b %2, %0\t\n\
         addx.b %3, %1\t\n\
         move.w %1, %4\t\n\
         add.w %4, %4\t\n\
         move.w 0(%6, %4.w), (%7)+\t\n\
         add.b %2, %0\t\n\
         addx.b %3, %1\t\n\
         dbra %5, loop%="
        : // output
        : "d" (v_low), "d" (v_high), "d" (dvdy_low), "d" (dvdy_high), "d" (scratch), "d" (dy),
          "a" (tex_column), "a" (word_col_ptr) // input
    );
    */
   // 4+4+14+8+8+4+4+4+4
    
    // 6.75 cycles per pixel without branch
    // with branch it's 8 cycles per pixel
    __asm volatile(
        "loop%=:\t\n\
         move.w %1, %4\t\n\
         add.w %4, %4\t\n\
         add.w %4, %4\t\n\
         move.l 0(%6, %4.w), (%7)+\t\n\
         add.b %2, %0\t\n\
         addx.b %3, %1\t\n\
         dbra %5, loop%="
        : // output
        : "d" (v_low), "d" (v_high), "d" (dvdy_low), "d" (dvdy_high), "d" (scratch), "d" (dy),
          "a" (tex_column), "a" (word_col_ptr) // input
    );
}

s16 texline_jmp_offset_table[80] = {
948,
936,924,912,900,888,
876,864,852,840,828,
816,804,792,780,768,
756,744,732,720,708,
696,684,672,660,648,
636,624,612,600,588,
576,564,552,540,528,
516,504,492,480,468,
456,444,432,420,408,
396,384,372,360,348,
336,324,312,300,288,
276,264,252,240,228,
216,204,192,180,168,
156,144,132,120,108,
96,84,72,60,48,
36,24,12,0
};

void draw_texline_asm_unrolled(u16 unclipped_dy, u16 clip_top, u16 dy, u16* word_col_ptr, u16* tex_column) {
    //s32 dv_per_dy = (TEX_HEIGHT<<8) / dy; 
    u32 dv_per_dy = dv_per_dy_tab[unclipped_dy];


    s32 v_fix = clip_top; 

    __asm volatile(
        "mulu.w %1, %0\t\n"
        : "+d" (v_fix)
        : "d" (dv_per_dy)
    );

    if(dy & 0b1) {
        *word_col_ptr++ = tex_column[v_fix>>8];
        v_fix += dv_per_dy;
    }

    if(!dy) {
        return;
    }

    u32* lw_col_ptr = (u32*)word_col_ptr;
    u32* lw_tex_column = (u32*)tex_column;
    dv_per_dy <<=1;
    v_fix <<= 1;
    dy>>=1;

    u8 v_low = (v_fix&0xFF);
    u8 v_high = (v_fix&0xFF00)>>8;
    u8 dvdy_low = (dv_per_dy&0xFF);
    u8 dvdy_high = (dv_per_dy&0xFF00)>>8;
    u16 scratch;
    
    u16 skip_bytes = texline_jmp_offset_table[dy]; //79*12 - dy*12;

    /*

        move.b %2, %5    ; 4
        lsl.w #2, %5     ; 10, 8 for word
        move.l 0(%6, %5.w), (%7)+ ; 26, 18 for word
        add.b %3, %1     ; 4
        addx.b %4, %2    ; 4

        48, 6 cycles per pixel
        

    */
    __asm volatile(
        "jmp addx_jump_tbl_%=(%%pc, %0.W)\t\n\
        addx_jump_tbl_%=:\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2\t\n\
         move.b %2, %5\t\n\
         lsl.w #2, %5\t\n\
         move.l 0(%6, %5.w), (%7)+\t\n\
         add.b %3, %1\t\n\
         addx.b %4, %2"
        : // output
        : "d" (skip_bytes), "d" (v_low), "d" (v_high), "d" (dvdy_low), "d" (dvdy_high), "d" (scratch),
          "a" (tex_column), "a" (word_col_ptr) // input
    );  
}




void draw_texture_vertical_line(s16 unclipped_y0, s16 y0, s16 unclipped_y1, s16 y1, u8* col_ptr, u16* tex_column) {
    //return ;


    col_ptr = col_ptr + (y0<<1);
    //u32* tex_column = &(texture[tex_column*TEX_HEIGHT]);
    u16* word_col_ptr = (u16*)col_ptr;

    s32 dy = y1-y0;
    s16 unclipped_dy = unclipped_y1 - unclipped_y0;
    if(unclipped_dy > 512) { return; }


    // using RAM jump tables
    void* base_call_loc = jump_table_lut[unclipped_dy];

    int clip_top = y0-unclipped_y0;
    int clip_bot = unclipped_y1-y1;


    #ifndef RAM_TEXTURE
    if(clip_bot != 0) {
        u32 du_dy = (TEX_HEIGHT<<16)/ unclipped_dy;
        u32 clip_bot_du_fix = du_dy * clip_bot;
        u16 clip_bot_du = clip_bot_du_fix>>16;
        tex_column -= (clip_bot_du);
    }   
    #endif
    
    register const a0 asm ("%a0") = ((u32)tex_column); // - (clip_bot<<1 * du_dy);
    register const  a1 asm ("%a1") = col_ptr;
    #ifdef RAM_TEXTURE
    register const  a2 asm ("%a2") = base_call_loc + (clip_top<<2); // + (clip_top<<2);
    #else 
    register const  a2 asm ("%a2") = base_call_loc + (clip_top<<2) + (clip_bot << 2);
    #endif 

    #ifdef RAM_TEXTURE
    if(clip_bot != 0) {
        //die("wtf");
        unlock_ram();
        volatile u32 volatile* patch_arr = (volatile u32 volatile*)base_call_loc;
        u32 prev_val = patch_arr[unclipped_dy-clip_bot];
        patch_arr[unclipped_dy-clip_bot] = 0x4E750000;
        if(patch_arr[unclipped_dy-clip_bot] != 0x4E750000) {
            die("what in the fuck");
        }
        u32 scratch = 0;
        //u32 patch_loc = (u32)base_call_loc + (unclipped_dy<<2) - (clip_bot<<2); // + (unclipped_dy<<2) - (clip_bot<<3);
        //volatile u16* wpatch_loc = (volatile u16*)base_call_loc; //(volatile u16*)patch_loc;
        //u16 prev_val = *wpatch_loc;
        //*wpatch_loc = 0x4E75;
        //if(*wpatch_loc != 0x4E75) { die("wtf"); }

        __asm volatile("jsr (%0)"
            :
            : "a" (a2), "a" (a0), "a" (a1), "a" (clip_bot), "d" (scratch)
        );
        //patch_arr[unclipped_dy-clip_bot] = prev_val;
        //*wpatch_loc = prev_val;
        lock_ram();
    } else {
        __asm volatile(
            "jsr (%0)"
            :
            : "a" (a2), "a" (a0), "a" (a1)
        );
    }
    #else 
        __asm volatile(
            "jsr (%0)"
            :
            : "a" (a2), "a" (a0), "a" (a1)
        );
    #endif 


    return; 
    
    //draw_texline_asm_unrolled(unclipped_dy, y0-unclipped_y0, dy, word_col_ptr, tex_column);
    //draw_texline_asm(unclipped_dy, y0-unclipped_y0, dy, word_col_ptr, tex_column);
    //return;

    /*
    s32 numer = TEX_HEIGHT<<8;
    
    //u16 dv_per_dy = div_32_by_16(TEX_HEIGHT<<8, unclipped_dy);
    __asm volatile(
        "divs.w %1, %0"
        : "+d" (numer) // output
        : "d" (unclipped_dy)
    );
    s32 dv_per_dy = numer & 0xFFFF;
    //(TEX_HEIGHT << 8) / unclipped_dy;
    */

    u32 dv_per_dy = dv_per_dy_tab[unclipped_dy];
    //u32 dv_per_dy = (32<<8) / unclipped_dy;


    //max_dy = max(unclipped_dy, max_dy);
    //KLog_S1("max dy: ", max_dy);

    //s16 v_fix = dv_per_dy * (y0 - unclipped_y0);
    u32 v_fix = y0-unclipped_y0; //dv_per_dy;

    __asm volatile(
        "mulu.w %1, %0\t\n"
        : "+d" (v_fix)
        : "d" (dv_per_dy)
    );

    if(dy & 0b1) {
        *word_col_ptr++ = tex_column[v_fix>>8];
        v_fix += dv_per_dy;
        //dy;
    }

    u32* lw_col_ptr = (u32*)word_col_ptr;
    u32* lw_tex_column = (u32*)tex_column;
    dy>>=1;

    v_fix <<= 9;
    dv_per_dy <<= 9;

    s16 v_fix_int;

    
    //dv_per_dy <<= 1;

    /*
    __asm volatile(
        "swap %0\t\n\
         swap %1\t\n\
         addx.l %1, %0"
    : "+d" (v_fix), "+d" (dv_per_dy)
    );
    */
    while(dy--) {
    //for(int y = 0; y < dy; y++) {
        
        
        v_fix_int = v_fix>>16;
        //u16 col = tex_column[v_fix_int];
        //*word_col_ptr++ = col;
        //*word_col_ptr++ = col;
        *word_col_ptr++ = tex_column[v_fix_int];
        v_fix += dv_per_dy;
        v_fix_int = v_fix>>16;
        *word_col_ptr++ = tex_column[v_fix_int];
        v_fix += dv_per_dy;
        
        
        /*
        u32 scratch;
        __asm volatile(
            "move.l %0, %2\t\n\
            add.w %2, %2\t\n\
            move.w (%4,%2.w), %2\t\n\
            move.w %2, (%1)+\t\n\
            move.w %2, (%1)+\t\n\
            addx.l %3, %0"
        : "+d" (v_fix), "+a" (word_col_ptr)
        : "d" (scratch), "d" (dv_per_dy), "a" (tex_column)
        );
        */

        // 10.75 cycles per pixel with branch
        // 9.5 cycles per pixel without
        /*
        u32 scratch;
        __asm volatile(
            "move.l %0, %2\t\n\
            swap %2\t\n\
            add.w %2, %2\t\n\
            move.w (%4,%2.w), (%1)+\t\n\
            add.l %3, %0\t\n\
            move.l %0, %2\t\n\
            swap %2\t\n\
            add.w %2, %2\t\n\
            move.w (%4,%2.w), (%1)+\t\n\
            add.l %3, %0"
        : "+d" (v_fix), "+a" (word_col_ptr)
        : "d" (scratch), "d" (dv_per_dy), "a" (tex_column)
        );
        */
   
    }

    return;
}
    


// column 0            column 1            column 2                column 3
// blue blue blue blue blue blue blue blue green green green green green green green green