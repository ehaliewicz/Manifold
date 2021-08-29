#include <genesis.h>
#include "colors.h"
#include "utils.h"
#include "texture.h"


// 64 pixels tall
// 64 pixels wide
// which means 64 rasterized columns

//BB_IDX (BLUE_IDX << 4 | )


// 2x1 byte
#define STEEL_PIX \
    ((LIGHT_STEEL_IDX << 4) | LIGHT_STEEL_IDX)
#define GREEN_PIX \
    ((GREEN_IDX << 4) | DARK_GREEN_IDX)
#define BROWN_PIX \
    ((BROWN_IDX << 4) | BROWN_IDX)
#define BLUE_PIX \
    ((BLUE_IDX << 4) | BLUE_IDX)

#define LIGHT_GREEN_PIX \
    ((LIGHT_GREEN_IDX << 4) | LIGHT_GREEN_IDX)
#define LIGHT_BROWN_PIX \
    ((LIGHT_BROWN_IDX << 4) | LIGHT_BROWN_IDX)

#define DARK_GREEN_PIX \
    ((DARK_GREEN_IDX << 4) | DARK_GREEN_IDX)
#define DARK_BROWN_PIX \
    ((DARK_BROWN_IDX << 4) | DARK_BROWN_IDX)

// 4x2 longword
#define BLUE_LONGWORD \
    ((BROWN_PIX << 24) | (BROWN_PIX << 16) | (BROWN_PIX << 8) | (BROWN_PIX))

#define GREEN_LONGWORD \
    ((GREEN_PIX << 24) | (GREEN_PIX << 16) | (GREEN_PIX << 8) | (GREEN_PIX))

#define LIGHT_BLUE_LONGWORD \
    ((LIGHT_BROWN_PIX << 24) | (LIGHT_BROWN_PIX << 16) | (LIGHT_BROWN_PIX << 8) | (LIGHT_BROWN_PIX))

#define LIGHT_GREEN_LONGWORD \
    ((LIGHT_GREEN_PIX << 24) | (LIGHT_GREEN_PIX << 16) | (LIGHT_GREEN_PIX << 8) | (LIGHT_GREEN_PIX))

#define DARK_BLUE_LONGWORD \
    ((DARK_BROWN_PIX << 24) | (DARK_BROWN_PIX << 16) | (DARK_BROWN_PIX << 8) | (DARK_BROWN_PIX))

#define DARK_GREEN_LONGWORD \
    ((DARK_GREEN_PIX << 24) | (DARK_GREEN_PIX << 16) | (DARK_GREEN_PIX << 8) | (DARK_GREEN_PIX))

// 4x4 squares
#define BLUE_SQUARE \
    BLUE_LONGWORD, BLUE_LONGWORD
#define GREEN_SQUARE \
    GREEN_LONGWORD, GREEN_LONGWORD 
#define LIGHT_BLUE_SQUARE \
    LIGHT_BLUE_LONGWORD, LIGHT_BLUE_LONGWORD
#define LIGHT_GREEN_SQUARE \
    LIGHT_GREEN_LONGWORD, LIGHT_GREEN_LONGWORD 
#define DARK_BLUE_SQUARE \
    DARK_BLUE_LONGWORD, DARK_BLUE_LONGWORD
#define DARK_GREEN_SQUARE \
    DARK_GREEN_LONGWORD, DARK_GREEN_LONGWORD 

// 4x8 squares
#define BLUE_SQUARE2 \
    BLUE_SQUARE, BLUE_SQUARE
#define GREEN_SQUARE2 \
    GREEN_SQUARE, GREEN_SQUARE
#define LIGHT_BLUE_SQUARE2 \
    LIGHT_BLUE_SQUARE, LIGHT_BLUE_SQUARE
#define LIGHT_GREEN_SQUARE2 \
    LIGHT_GREEN_SQUARE, LIGHT_GREEN_SQUARE
#define DARK_BLUE_SQUARE2 \
    DARK_BLUE_SQUARE, DARK_BLUE_SQUARE
#define DARK_GREEN_SQUARE2 \
    DARK_GREEN_SQUARE, DARK_GREEN_SQUARE


// 4x64
#define BLUE_START_COLUMN \
    BLUE_SQUARE2, GREEN_SQUARE2, BLUE_SQUARE2, GREEN_SQUARE2, BLUE_SQUARE2, GREEN_SQUARE2, BLUE_SQUARE2, GREEN_SQUARE2
#define GREEN_START_COLUMN \
    GREEN_SQUARE2, BLUE_SQUARE2, GREEN_SQUARE2, BLUE_SQUARE2, GREEN_SQUARE2, BLUE_SQUARE2, GREEN_SQUARE2, BLUE_SQUARE2
#define LIGHT_BLUE_START_COLUMN \
    LIGHT_BLUE_SQUARE2, LIGHT_GREEN_SQUARE2, LIGHT_BLUE_SQUARE2, LIGHT_GREEN_SQUARE2, LIGHT_BLUE_SQUARE2, LIGHT_GREEN_SQUARE2, LIGHT_BLUE_SQUARE2, LIGHT_GREEN_SQUARE2
#define LIGHT_GREEN_START_COLUMN \
    LIGHT_GREEN_SQUARE2, LIGHT_BLUE_SQUARE2, LIGHT_GREEN_SQUARE2, LIGHT_BLUE_SQUARE2, LIGHT_GREEN_SQUARE2, LIGHT_BLUE_SQUARE2, LIGHT_GREEN_SQUARE2, LIGHT_BLUE_SQUARE2
#define DARK_BLUE_START_COLUMN \
    DARK_BLUE_SQUARE2, DARK_GREEN_SQUARE2, DARK_BLUE_SQUARE2, DARK_GREEN_SQUARE2, DARK_BLUE_SQUARE2, DARK_GREEN_SQUARE2, DARK_BLUE_SQUARE2, DARK_GREEN_SQUARE2
#define DARK_GREEN_START_COLUMN \
    DARK_GREEN_SQUARE2, DARK_BLUE_SQUARE2, DARK_GREEN_SQUARE2, DARK_BLUE_SQUARE2, DARK_GREEN_SQUARE2, DARK_BLUE_SQUARE2, DARK_GREEN_SQUARE2, DARK_BLUE_SQUARE2

// 8x64
#define BLUE_START_SQUARE_COLUMN \
    BLUE_START_COLUMN, BLUE_START_COLUMN
#define GREEN_START_SQUARE_COLUMN \
    GREEN_START_COLUMN, GREEN_START_COLUMN

#define LIGHT_BLUE_START_SQUARE_COLUMN \
    LIGHT_BLUE_START_COLUMN, LIGHT_BLUE_START_COLUMN
#define LIGHT_GREEN_START_SQUARE_COLUMN \
    LIGHT_GREEN_START_COLUMN, LIGHT_GREEN_START_COLUMN

#define DARK_BLUE_START_SQUARE_COLUMN \
    DARK_BLUE_START_COLUMN, DARK_BLUE_START_COLUMN
#define DARK_GREEN_START_SQUARE_COLUMN \
    DARK_GREEN_START_COLUMN, DARK_GREEN_START_COLUMN







// each column is 32 pixels high
// 32 longword columns, each 4 pixels wide
const u32 dark_texture[32*32] = {
    DARK_BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    DARK_GREEN_START_SQUARE_COLUMN ,
    DARK_BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    DARK_GREEN_START_SQUARE_COLUMN ,
    DARK_BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    DARK_GREEN_START_SQUARE_COLUMN ,
    DARK_BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    DARK_GREEN_START_SQUARE_COLUMN ,
    DARK_BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    DARK_GREEN_START_SQUARE_COLUMN ,
    DARK_BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    DARK_GREEN_START_SQUARE_COLUMN ,
    DARK_BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    DARK_GREEN_START_SQUARE_COLUMN ,
    DARK_BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    DARK_GREEN_START_SQUARE_COLUMN ,
};

const u32 mid_texture[32*32] = {
    BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    GREEN_START_SQUARE_COLUMN ,
    BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    GREEN_START_SQUARE_COLUMN ,
    BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    GREEN_START_SQUARE_COLUMN ,
    BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    GREEN_START_SQUARE_COLUMN ,
    BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    GREEN_START_SQUARE_COLUMN ,
    BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    GREEN_START_SQUARE_COLUMN ,
    BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    GREEN_START_SQUARE_COLUMN ,
    BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    GREEN_START_SQUARE_COLUMN ,
};

const u32 light_texture[32*32] = {
    LIGHT_BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    LIGHT_GREEN_START_SQUARE_COLUMN ,
    LIGHT_BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    LIGHT_GREEN_START_SQUARE_COLUMN ,
    LIGHT_BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    LIGHT_GREEN_START_SQUARE_COLUMN ,
    LIGHT_BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    LIGHT_GREEN_START_SQUARE_COLUMN ,
    LIGHT_BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    LIGHT_GREEN_START_SQUARE_COLUMN ,
    LIGHT_BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    LIGHT_GREEN_START_SQUARE_COLUMN ,
    LIGHT_BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    LIGHT_GREEN_START_SQUARE_COLUMN ,
    LIGHT_BLUE_START_SQUARE_COLUMN , // two 64-pixel tall columns 
    LIGHT_GREEN_START_SQUARE_COLUMN ,
};


// goes up to maybe 1024
static s16 max_dy = 0;

//void draw_unclipped_vertical_line(s16 y0, s16 y1, s16 unclipped_y1, s16 y1, u8* col_ptr, u8 tex_col_idx) {}

s32 dv_per_dy_tab[1024] = {
    8192, 4096, 2731, 2048, 1638, 1365, 1170, 1024, 910, 819, 
745, 683, 630, 585, 546, 512, 482, 455, 431, 410, 
390, 372, 356, 341, 328, 315, 303, 293, 282, 273, 
264, 256, 248, 241, 234, 228, 221, 216, 210, 205, 
200, 195, 191, 186, 182, 178, 174, 171, 167, 164, 
161, 158, 155, 152, 149, 146, 144, 141, 139, 137, 
134, 132, 130, 128, 126, 124, 122, 120, 119, 117, 
115, 114, 112, 111, 109, 108, 106, 105, 104, 102, 
101, 100, 99, 98, 96, 95, 94, 93, 92, 91, 
90, 89, 88, 87, 86, 85, 84, 84, 83, 82, 
81, 80, 80, 79, 78, 77, 77, 76, 75, 74, 
74, 73, 72, 72, 71, 71, 70, 69, 69, 68, 
68, 67, 67, 66, 66, 65, 65, 64, 64, 63, 
63, 62, 62, 61, 61, 60, 60, 59, 59, 59, 
58, 58, 57, 57, 56, 56, 56, 55, 55, 55, 
54, 54, 54, 53, 53, 53, 52, 52, 52, 51, 
51, 51, 50, 50, 50, 49, 49, 49, 48, 48, 
48, 48, 47, 47, 47, 47, 46, 46, 46, 46, 
45, 45, 45, 45, 44, 44, 44, 44, 43, 43, 
43, 43, 42, 42, 42, 42, 42, 41, 41, 41, 
41, 41, 40, 40, 40, 40, 40, 39, 39, 39, 
39, 39, 38, 38, 38, 38, 38, 38, 37, 37, 
37, 37, 37, 37, 36, 36, 36, 36, 36, 36, 
35, 35, 35, 35, 35, 35, 35, 34, 34, 34, 
34, 34, 34, 34, 33, 33, 33, 33, 33, 33, 
33, 33, 32, 32, 32, 32, 32, 32, 32, 32, 
31, 31, 31, 31, 31, 31, 31, 31, 30, 30, 
30, 30, 30, 30, 30, 30, 30, 29, 29, 29, 
29, 29, 29, 29, 29, 29, 29, 28, 28, 28, 
28, 28, 28, 28, 28, 28, 28, 27, 27, 27, 
27, 27, 27, 27, 27, 27, 27, 27, 27, 26, 
26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 
26, 25, 25, 25, 25, 25, 25, 25, 25, 25, 
25, 25, 25, 25, 24, 24, 24, 24, 24, 24, 
24, 24, 24, 24, 24, 24, 24, 24, 23, 23, 
23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 
23, 23, 23, 23, 22, 22, 22, 22, 22, 22, 
22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 
22, 21, 21, 21, 21, 21, 21, 21, 21, 21, 
21, 21, 21, 21, 21, 21, 21, 21, 21, 20, 
20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 
20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 
19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 
19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 
19, 19, 18, 18, 18, 18, 18, 18, 18, 18, 
18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 
18, 18, 18, 18, 18, 18, 18, 18, 17, 17, 
17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 
17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 
17, 17, 17, 17, 17, 17, 16, 16, 16, 16, 
16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 
16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 
16, 16, 16, 16, 16, 16, 16, 16, 15, 15, 
15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 
15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 
15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 
15, 15, 15, 15, 14, 14, 14, 14, 14, 14, 
14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 
14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 
14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 
14, 14, 14, 14, 14, 14, 13, 13, 13, 13, 
13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 
13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 
13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 
13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 
13, 13, 13, 13, 13, 12, 12, 12, 12, 12, 
12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 
12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 
12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 
12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 
12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 
12, 12, 11, 11, 11, 11, 11, 11, 11, 11, 
11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 
11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 
11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 
11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 
11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 
11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 
10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 
10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 
10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 
10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 
10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 
10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 
10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 
10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 
10, 10, 9, 9, 9, 9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 
9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 
9, 9, 9, 8, 8, 8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
8, 8, 8
};

void draw_texture_vertical_line(s16 unclipped_y0, s16 y0, s16 unclipped_y1, s16 y1, u8* col_ptr, u16* tex_column) {


    col_ptr = col_ptr + (y0<<1);
    //u32* tex_column = &(texture[tex_column*TEX_HEIGHT]);
    u16* word_col_ptr = (u16*)col_ptr;

    s16 dy = y1-y0;
    s16 unclipped_dy = unclipped_y1 - unclipped_y0;

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

    s32 dv_per_dy = dv_per_dy_tab[unclipped_dy];


    //max_dy = max(unclipped_dy, max_dy);
    //KLog_S1("max dy: ", max_dy);

    //s16 v_fix = dv_per_dy * (y0 - unclipped_y0);
    s32 v_fix = y0-unclipped_y0; //dv_per_dy;

    __asm volatile(
        "muls.w %1, %0\t\n"
        : "+d" (v_fix)
        : "d" (dv_per_dy)
    );

    if(dy & 0b1) {
        *word_col_ptr++ = tex_column[v_fix>>8];
        v_fix += dv_per_dy;
        //dy;
    }

    u32* lw_col_ptr = (u32*)word_col_ptr;
    //v_fix >>= 1;
    u32* lw_tex_column = (u32*)tex_column;
    //s16 double_dv_per_dy = dv_per_dy << 1;
    dy>>=1;
    //v_fix>>=1;
    v_fix <<= 9;
    dv_per_dy <<= 9;

    s16 v_fix_int;

    
    dv_per_dy <<= 1;
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
        
        /*
        v_fix_int = v_fix>>16;
        u16 col = tex_column[v_fix_int];
        *word_col_ptr++ = col;
        *word_col_ptr++ = col;
        //*word_col_ptr++ = tex_column[v_fix_int];
        v_fix += dv_per_dy;
        //v_fix_int = v_fix>>16;
        //*word_col_ptr++ = tex_column[v_fix_int];
        v_fix += dv_per_dy;
        */
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
        u32 scratch;
        __asm volatile(
            "move.l %0, %2\t\n\
            swap %2\t\n\
            add.w %2, %2\t\n\
            move.l (%4,%2.w), (%1)+\t\n\
            add.l %3, %0"
        : "+d" (v_fix), "+a" (word_col_ptr)
        : "d" (scratch), "d" (dv_per_dy), "a" (tex_column)
        );
        
   
    }

    return;
}
    


// column 0            column 1            column 2                column 3
// blue blue blue blue blue blue blue blue green green green green green green green green