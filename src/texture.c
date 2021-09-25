#include <genesis.h>
#include "cart_ram.h"
#include "colors.h"
#include "config.h"
#include "div_lut.h"
#include "math3d.h"
#include "utils.h"
#include "texture.h"
#include "textures.h"
#include "tex_tables_lookup.h"


// 64 pixels tall
// 64 pixels wide
// which means 64 rasterized columns

void calc_textures_for_light_level(s8 light_level, lit_texture* lit_tex, u16** dark_tex, u16** mid_tex, u16** light_tex) {
    
    switch(light_level) {
      case -2:
        *light_tex = lit_tex->dark;
        *mid_tex = lit_tex->dark;
        *dark_tex = lit_tex->dark;
        break;
      case -1:
        *light_tex = lit_tex->mid;
        *mid_tex = lit_tex->dark;
        *dark_tex = lit_tex->dark;
        break;
      case 0:
        *light_tex = lit_tex->light;
        *mid_tex = lit_tex->mid;
        *dark_tex = lit_tex->dark;
        break;
      case 1:
        *dark_tex = lit_tex->mid;
        *mid_tex = lit_tex->light;
        *light_tex = lit_tex->light;
        break;
      case 2:
        *dark_tex = lit_tex->light;
        *mid_tex = lit_tex->light;
        *light_tex = lit_tex->light;
        break;
    }
}


persp_params calc_perspective(u16 z1_12_4, u16 z2_12_4, u32 left_u_16, u32 right_u_16, s16 dx) {
    persp_params ret;
    u32 one_over_z_26 = z_12_4_to_one_over_z_26[z1_12_4>>4];
    u32 one_over_z_end_26 = z_12_4_to_one_over_z_26[z2_12_4>>4];


    s32 d_one_over_z_26 = (one_over_z_end_26 - one_over_z_26);

    u32 u_over_z_23 = (left_u_16<<(TRANS_Z_FRAC_BITS+7))/z1_12_4;  
    u32 u_over_z_end_23 = (right_u_16<<(TRANS_Z_FRAC_BITS+7))/z2_12_4;


    s32 d_u_over_z_23 = (u_over_z_end_23 - u_over_z_23);


    ret.one_over_z_26 = one_over_z_26;

    ret.d_one_over_z_dx_26 = (d_one_over_z_26/dx);
    ret.u_over_z_23 = u_over_z_23;
    ret.d_u_over_z_dx_23 = d_u_over_z_23/dx;
    return ret;
}

void draw_texture_vertical_line(s16 unclipped_y0, s16 y0, s16 unclipped_y1, s16 y1, u8* col_ptr, u16* tex_column) {
    //return ;


    col_ptr = col_ptr + (y0<<1);
    //u32* tex_column = &(texture[tex_column*TEX_HEIGHT]);
    u16* word_col_ptr = (u16*)col_ptr;

    s32 dy = y1-y0;
    s16 unclipped_dy = unclipped_y1 - unclipped_y0;
    if(unclipped_dy > 512) { return; }


    // using ROM jump tables
    void* base_call_loc = jump_table_lut[unclipped_dy];

    int clip_top = y0-unclipped_y0;
    int clip_bot = unclipped_y1-y1;


    if(clip_bot != 0) {
        u32 du_dy = (TEX_HEIGHT<<16)/ unclipped_dy;
        u32 clip_bot_du_fix = du_dy * clip_bot;
        u16 clip_bot_du = clip_bot_du_fix>>16;
        tex_column -= (clip_bot_du);
    }   
    
    register const a0 asm ("%a0") = ((u32)tex_column); // - (clip_bot<<1 * du_dy);
    register const  a1 asm ("%a1") = (u32)col_ptr;

    register const  a2 asm ("%a2") = base_call_loc + (clip_top<<2) + (clip_bot << 2);

    
    __asm volatile(
        "jsr (%0)"
        :
        : "a" (a2), "a" (a0), "a" (a1)
    );


    return; 
    
}
    


// column 0            column 1            column 2                column 3
// blue blue blue blue blue blue blue blue green green green green green green green green