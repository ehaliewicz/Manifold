#include <genesis.h>
#include "div_lut.h"
#include "math3d.h"
#include "game.h"
#include "texture.h"
#include "utils.h"

Vect2D_f32 transform_map_vert(s16 x, s16 y) {
    fix32 tlx = intToFix32(x) - cur_player_pos.x;
    fix32 tly = intToFix32(y) - cur_player_pos.y;
    
    fix32 rx = fix32Mul((tlx),(angleSin32)) - fix32Mul((tly),(angleCos32));
    fix32 ry = fix32Mul((tlx),(angleCos32)) + fix32Mul((tly),(angleSin32));

    return (Vect2D_f32){.x = rx, .y = ry};
}

Vect2D_s16 transform_map_vert_16(s16 x, s16 y) {
    
    s16 tlx = x - playerXInt; // 16-bit integer
    s16 tly = y - playerYInt; // 16-bit integer

    s32 rx = (tlx * angleSin16) - (tly * angleCos16); // 22.10 * 16 -> 22.10
    s32 ry = (tlx * angleCos16) + (tly * angleSin16);
    s16 res_x = rx>>(FIX16_FRAC_BITS);
    s16 res_y = ry>>(FIX16_FRAC_BITS);
    return (Vect2D_s16){.x = res_x, .y = res_y};
}



s16 project_and_adjust_x(s16 x, s16 z_recip) {
    //fix32 rx = trans_map_vert.x;
    //fix32 rz = trans_map_vert.y;
    
    
    s16 const2Rx = x; //<<5; //(32 * x);
    __asm volatile(
            "lsl.w #5, %0"
            : "+d" (const2Rx)
    );
    //s16 const2RxDivZ = const2Rx / z;
    //s16 z_recip = z_recip_table[z];
    s32 const2RXMulZRecip = const2Rx; // * x_project_z_recip_table[z];
    __asm volatile(
            "muls.w %1, %0"
            :  "+d" (const2RXMulZRecip), "+d" (z_recip) // output
            : // input
    );


    s16 transX = 32 + (const2RXMulZRecip>>16);
    return transX;
}


/*
s16 project_and_adjust_x(s16 x, s16 z) {
    //fix32 rx = trans_map_vert.x;
    //fix32 rz = trans_map_vert.y;
    
    
    s32 const2Rx = (32 * x);

    s16 const2RxDivZ = const2Rx / z;
    s16 transX = CONST1 + const2RxDivZ;
    return transX;
}
*/

/*
s16 project_and_adjust_y(Vect2D_f32 trans_map_vert, s16 y) {    
    fix32 rz = trans_map_vert.y;

    s16 const4Ry = (CONST4 * (y - fix32ToInt(cur_player_pos.z)));
    s16 const4RyDivZ = ((const4Ry)<<FIX32_FRAC_BITS) / rz;

    s16 yproj = CONST3 + const4RyDivZ;

    return (H-1)-yproj;
}
*/

/*
s16 project_and_adjust_y_fix_c(Vect2D_f32 trans_map_vert, s16 y) {    
    fix32 rz = trans_map_vert.y>>FIX32_FRAC_BITS; // 22.10

    fix32 playPosZFrac4 = cur_player_pos.z>>(10-4); //
    // y = 10.8
    fix32 const4Ry = (CONST4 * ((y<<4) - playPosZFrac4)); // 28.4
    fix32 const4RyDivZ = const4Ry / rz;  // 28.4

    fix32 yproj = (CONST3<<4) + (const4RyDivZ);

    return ((H-1)<<4)-yproj;
}
*/

s16 project_and_adjust_y_fix(s16 y, s16 z_recip) {    
    s16 yMinusPosZ = y - playerZ12Frac4;      // 12.4
    s32 const4Ry = yMinusPosZ; //CONST4 * yMinusPosZ;

    //s32 const4RyMulRecipZ = const4Ry * z_recip_table[z];
    //s16 z_recip = z_recip_table[z];

    
    s32 const4RyMulRecipZ = const4Ry * z_recip * CONST4;
    /*
    s32 const4RyMulRecipZ = const4Ry; // * x_project_z_recip_table[z];
    __asm volatile(
            "muls.w %1, %0"
            :  "+d" (const4RyMulRecipZ), "+d" (z_recip) // output
            : // input
    );
    const4RyMulRecipZ = (const4RyMulRecipZ<<3) + (const4RyMulRecipZ<<6); // <<= 6;
    */
    s16 yproj = (CONST3<<4) + (const4RyMulRecipZ>>16);
    return ((SCREEN_HEIGHT-1)<<4)-yproj;

    //fix32 rx = trans_map_vert.x;
    //fix32 rz = trans_map_vert.y;
}

/*
s16 project_and_adjust_y_fix_c(s16 y, s16 z) {    
    s16 yMinusPosZ = y - playerZ12Frac4;      // 12.4
    s32 const4Ry = CONST4 * yMinusPosZ;

    s16 const4RyDivZ = const4Ry / z;

    s16 yproj = (CONST3<<4) + const4RyDivZ;
    return ((SCREEN_HEIGHT-1)<<4)-yproj;
}
*/


//const s32 near_z = 160 << FIX32_FRAC_BITS;

#define CLIP_DIVISION

clip_result clip_map_vertex_16(Vect2D_s16* trans_v1, Vect2D_s16* trans_v2, texmap_info* tmap, u32 wall_len) {
    // TODO: adjust texture coordinates here as well

    s16 rx1 = trans_v1->x;
    s16 rz1 = trans_v1->y;
    s16 rx2 = trans_v2->x;
    s16 rz2 = trans_v2->y;

    s16 dz = rz2 - rz1;

    if(rz1 <= NEAR_Z_16 && rz2 <= NEAR_Z_16) {
        tmap->needs_perspective = 0;
    
        return OFFSCREEN;
    }

    tmap->needs_perspective = (dz != 0);
    #define TEX_REPEAT_DIST 64
    u16 repetitions = max(1, wall_len / TEX_REPEAT_DIST);
    //KLog_U1("repetitions: ", repetitions);

    s32 base_left_u = 0;
    s32 base_right_u = (1)<<16;

    s32 fix_du = (base_right_u-base_left_u);
    //KLog_S1("fix_du: ", fix_du);
    //KLog_S1("du: ", fix_du>>8);
    //KLog_S1("dz: ", dz);
    s32 du_over_dz; // = fix_du; // 16.16
    if(dz != 0) {
        du_over_dz = fix_du / dz;
        //__asm volatile(
        //    "divs.w %1, %0"
        //    : "+d" (du_over_dz) 
        //    : "d" (dz)
        //);
        tmap->du_over_dz = du_over_dz;
        //KLog_S1("du over dz: ", du_over_dz);
        tmap->needs_perspective = 1;
    } else {
        tmap->needs_perspective = 0;
        tmap->left_u = base_left_u;
        tmap->right_u = base_right_u;
    }

    if(rz1 > NEAR_Z_16 && rz2 > NEAR_Z_16) {
        //u32 fix_du = 32<<6;

        //__asm volatile(
        //    "divs.w %1, %0"
        //    :  "+d" (du_over_dx)  // output
        //    :  "d" (dx) // input
        //);
        tmap->left_u = base_left_u;
        tmap->right_u = base_right_u;

        return UNCLIPPED;
    }
    
    s16 dx = rx2-rx1;
    


    // SLOW
    #ifdef CLIP_DIVISION
    s32 fix_dx = dx<<6;
    s16 dx_over_dz = fix_dx;

    //divs.w %3, %1
    __asm volatile(
        "divs.w %1, %0"
        :  "+d" (dx_over_dz)  // output
        :  "d" (dz) // input
    );
   
    
   #else
   s16 inv_dz = (dz < 0) ? -(z_recip_table[-dz]>>10) : (z_recip_table[dz]>>10);
   s16 dx_over_dz = dx * inv_dz; //dx_over_dz_32 >> 10;
   #endif 
    
    //s16 dx_over_dz = fix32Div(dx, dz); // change in x per change in z


    if(rz1 <= NEAR_Z_16) { 
        //if(dz == 0) { die("wtf"); }
        // left clipped
        s16 z_adjust = NEAR_Z_16-rz1;
        s32 x_adjust = dx_over_dz * z_adjust;
        s16 x_adjust_16 = x_adjust>>6;

        s32 u_adjust = du_over_dz * z_adjust; // 12.4 fixed point
        
        
        rx1 += x_adjust_16;    // modify x coord
        rz1 = NEAR_Z_16;
        trans_v1->x = rx1;
        trans_v1->y = rz1;
        
        //KLog_S1("z adjust: ", z_adjust);
        //KLog_S1("u adjust: ", u_adjust>>16);
        tmap->left_u = (base_left_u + u_adjust);
        tmap->right_u = base_right_u; //1<<16;

        return LEFT_CLIPPED;
    } else {
        // right clipped
        s16 z_adjust = NEAR_Z_16 - rz2; 
        s32 x_adjust = dx_over_dz * z_adjust; // dx_over_dz is negative
        s16 x_adjust_16 = x_adjust>>6;
        rx2 += x_adjust_16;
        rz2 = NEAR_Z_16;
        trans_v2->x = rx2;
        trans_v2->y = rz2;

        s32 u_adjust = (du_over_dz) * z_adjust;
        tmap->left_u = base_left_u;
        tmap->right_u = ((base_right_u)+(u_adjust));
        return RIGHT_CLIPPED;
    }
    
}


/*
clip_result clip_map_vertex(Vect2D_f32* trans_v1, Vect2D_f32* trans_v2) {
    // TODO: figure out how much precision we need here (16-bit instead of 32?)
    // TODO: adjust texture coordinates here as well

    fix32 rx1 = trans_v1->x;
    fix32 rz1 = trans_v1->y;
    fix32 rx2 = trans_v2->x;
    fix32 rz2 = trans_v2->y;

    if(rz1 <= NEAR_Z_32 && rz2 <= NEAR_Z_32) {
        return OFFSCREEN;
    }
    if(rz1 > NEAR_Z_32 && rz2 > NEAR_Z_32) {
        return UNCLIPPED;
    }    

    fix32 dz = rz2-rz1;
    fix32 dx = rx2-rx1;

    // SLOW
    s16 dx_over_dz = fix32Div(dx, dz); // change in x per change in z


    if(rz1 < NEAR_Z_32) {
        // left clipped
        fix32 z_adjust = NEAR_Z_32 - rz1;  // how much we need to adjust z by
        fix32 x_adjust = fix32Mul(z_adjust, dx_over_dz); // multiply z_adjust by dx_over_dx -> x_adjust
        rx1 += x_adjust;    // modify x coord
        rz1 = NEAR_Z_32;
        trans_v1->x = rx1;
        trans_v1->y = rz1;
        return LEFT_CLIPPED;
    } else {
        // right clipped
        fix32 z_adjust = NEAR_Z_32 - rz2;
        fix32 x_adjust = fix32Mul(z_adjust, dx_over_dz);
        rx2 += x_adjust;
        rz2 = NEAR_Z_32;
        trans_v2->x = rx2;
        trans_v2->y = rz2;
        return RIGHT_CLIPPED;
    }
}
*/