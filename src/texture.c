#include <genesis.h>
#include "colors.h"
#include "config.h"
#include "div_lut.h"
#include "math3d.h"
#include "texture.h"
#include "textures.h"
#include "tex_tables_lookup.h"
#include "utils.h"


// 64 pixels tall
// 64 pixels wide
// which means 64 rasterized columns

#define Z_PERSP_MIN (100<<4)

persp_params calc_perspective(u16 z1_12_4, u16 z2_12_4, u16 inv_z1, u16 inv_z2, u32 left_u_16, u32 right_u_16, u16 dx) {
    persp_params ret;
    //if (abs(z2_12_4 - z1_12_4) < Z_PERSP_MIN) {
    //    u32 du_16 = right_u_16-left_u_16;
    //    u32 du_dx_16 = du_16 / dx;
    //    ret.left_u_16 = left_u_16;
    //    ret.du_dx_16 = du_dx_16;
    //    ret.needs_perspective = 0;
    //    return ret;
    //}
    u32 one_over_z_26 = z_12_4_to_one_over_z_26[z1_12_4>>4];
    u32 one_over_z_end_26 = z_12_4_to_one_over_z_26[z2_12_4>>4];


    s32 d_one_over_z_26 = (one_over_z_end_26 - one_over_z_26);

    //u32 u_over_z_23 = (left_u_16<<(TRANS_Z_FRAC_BITS+7))/z1_12_4;  
    //u32 u_over_z_end_23 = (right_u_16<<(TRANS_Z_FRAC_BITS+7))/z2_12_4;

    //u16 inv_z1_0_16 = 65536/(z1_12_4>>4);
    //u16 inv_z2_0_16 = 65536/(z2_12_4>>4);
    //u16 inv_z1_0_16 = z_recip_table_16[z1_12_4>>4];
    //u16 inv_z2_0_16 = z_recip_table_16[z2_12_4>>4];
    u32 u_over_z_23 = ((left_u_16)*(inv_z1))>>9; // 11+12
    u32 u_over_z_end_23 = ((right_u_16)*(inv_z2))>>9;

    


    //u32 u_over_z_end = (right_u_16<<8)/z2_12_4;
    //u32 u_over_z_23 = u_over_z<<19;
    //u32 u_over_z_end_23 = u_over_z_end<<19;

    s32 d_u_over_z_23 = (u_over_z_end_23 - u_over_z_23);


    ret.one_over_z_26 = one_over_z_26;


    ret.d_one_over_z_dx_26 = (d_one_over_z_26/dx);

    ret.u_over_z_23 = u_over_z_23;
    ret.d_u_over_z_dx_23 = d_u_over_z_23/dx;
    return ret;
}

typedef u8* (*top_clip_wall_fill_func)(u8* col_ptr, u16* tex_column, u16 clip_top);
typedef u8* (*bot_clip_wall_fill_func)(u8* col_ptr, u16* tex_column, u16 clip_top, u16 clip_bot);


void draw_texture_vertical_line(s16 unclipped_y0, u16 y0, s16 unclipped_y1, u8* col_ptr, const u16* tex_column) {
    u16 unclipped_dy = unclipped_y1;
    __asm volatile(             
        "sub.w %1, %0"          
        : "+d" (unclipped_dy)              
        : "d" (unclipped_y0)               
    );

    if(unclipped_dy > 512) { return; }
    __asm volatile(
        "add.l %1, %0\t\n\
        add.l %1, %0"
        : "+a" (col_ptr)
        : "d" (y0)
    );

    u16 clip_top = y0;//y0-unclipped_y0;
    __asm volatile(
        "sub.w %1, %0"
        : "+d" (clip_top)
        : "d" (unclipped_y0)
    );
    u16 f;// = unclipped_dy;

    register const u16* a0 asm ("%a0") = tex_column;
    register const u8* a1 asm ("%a1") = col_ptr;

    register const u32 d0 asm ("%d0") = clip_top;

    __asm volatile(
        "lsl.l #2, %6\t\n\
        move.l 0(%5, %6.w), %1\t\n\
        jsr (%1)"
        : "+a" (col_ptr)
        : "a" (f), "a" (a0), "a" (a1), "d" (d0), "a" (jump_table_top_clip_lut), "d" (unclipped_dy)
    );

}



void draw_bottom_clipped_texture_vertical_line(s16 unclipped_y0, u16 y0, s16 unclipped_y1, u16 y1, u8* col_ptr, const u16* tex_column) {

    u16 unclipped_dy = unclipped_y1;
    __asm volatile(             
        "sub.w %1, %0"          
        : "+d" (unclipped_dy)              
        : "d" (unclipped_y0)               
    );      

    if(unclipped_dy > 512) { return; }
    s16 clipped_dy = y1-y0;
    if(clipped_dy <= 0) { return; }
    __asm volatile(
        "add.l %1, %0\t\n\
        add.l %1, %0"
        : "+a" (col_ptr)
        : "d" (y0)
    );

    u16 clip_top = y0;
    u16 clip_bot = unclipped_y1;
    __asm volatile(
        "sub.w %2, %0\t\n\
         sub.w %3, %1\t\n\
         "
        : "+d" (clip_top), "+d" (clip_bot)
        : "d" (unclipped_y0), "d" (y1)
    );
    u16 f;
    register const u32 d0 asm ("%d0") = clip_top;
    register const u32 d1 asm ("%d1") = clip_bot;
    register const u16* a0 asm ("%a0") = tex_column;
    register const u8* a1 asm ("%a1") = col_ptr;


    __asm volatile(
        "lsl.l #2, %7\t\n\
         move.l 0(%6, %7.w), %1\t\n\
         jsr (%1)\t\n\
    "
        : "+a" (col_ptr)
        : "a" (f), "a" (a0), "a" (a1), "d" (d0), "d" (d1), "a" (jump_table_bot_clip_lut), "d" (unclipped_dy)
    );
  
}
    
