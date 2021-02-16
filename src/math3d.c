#include <genesis.h>
#include "math3d.h"
#include "game.h"

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


/*
s16 project_and_adjust_x(Vect2D_f32 trans_map_vert) {
    fix32 rx = trans_map_vert.x;
    fix32 rz = trans_map_vert.y;
    
    
    fix32 const2Rx = (CONST2 * rx);

    s16 const2RxDivZ = const2Rx / rz;
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

    return (BMP_HEIGHT-1)-yproj;
}*/


/*
s16 project_and_adjust_y_fix(Vect2D_f32 trans_map_vert, s16 y) {    
    fix32 rz = trans_map_vert.y>>FIX32_FRAC_BITS; // 22.10

    fix32 playPosZFrac4 = cur_player_pos.z>>(10-4); //
    // y = 10.8
    fix32 const4Ry = (CONST4 * ((y<<4) - playPosZFrac4)); // 28.4
    fix32 const4RyDivZ = const4Ry / rz;  // 28.4

    fix32 yproj = (CONST3<<4) + (const4RyDivZ);

    return ((BMP_HEIGHT-1)<<4)-yproj;
}
*/

/*
s16 project_and_adjust_y_fix(Vect2D_f32 trans_map_vert, s16 y) {    
    s16 int_rz = trans_map_vert.y>>FIX32_FRAC_BITS; // integer

    s16 playPosZFrac4 = cur_player_pos.z>>(10-4); // 12.4
    s16 yMinusPosZ = (y<<4) - playPosZFrac4;      // 12.4
    s32 const4Ry = CONST4 * yMinusPosZ;

    s16 const4RyDivZ = const4Ry / int_rz;

    s16 yproj = (CONST3<<4) + const4RyDivZ;
    return ((BMP_HEIGHT-1)<<4)-yproj;
}
*/

//const s32 near_z = 160 << FIX32_FRAC_BITS;
const s32 near_z_32 = 20 << FIX32_FRAC_BITS;
const s16 near_z_16 = 20;


clip_result clip_map_vertex_16(Vect2D_s16* trans_v1, Vect2D_s16* trans_v2) {
    // TODO: figure out how much precision we need here (16-bit instead of 32?)
    // TODO: adjust texture coordinates here as well

    s16 rx1 = trans_v1->x;
    s16 rz1 = trans_v1->y;
    s16 rx2 = trans_v2->x;
    s16 rz2 = trans_v2->y;

    if(rz1 <= near_z_16 && rz2 <= near_z_16) {
        return OFFSCREEN;
    }
    if(rz1 > near_z_16 && rz2 > near_z_16) {
        return UNCLIPPED;
    }

    //return OFFSCREEN;

    
    s16 dz = rz2-rz1;
    s16 dx = rx2-rx1;

    // SLOW
    s32 fix_dx = dx<<6;
    //s16 dx_over_dz = fix_dx/dz; // 12.6 fixed point
    s16 dx_over_dz = fix_dx;

    __asm volatile(
        "divs.w %1, %0"
        :  "+d" (dx_over_dz), "+d" (dz) // output
        : // input
    );
    
    
    //s16 dx_over_dz = fix32Div(dx, dz); // change in x per change in z


    if(rz1 < near_z_16) {
        // left clipped
        s16 z_adjust = near_z_16-rz1;
        s32 x_adjust = dx_over_dz * z_adjust;
        s16 x_adjust_16 = x_adjust>>6;
        
        rx1 += x_adjust_16;    // modify x coord
        rz1 = near_z_16;
        trans_v1->x = rx1;
        trans_v1->y = rz1;
        return LEFT_CLIPPED;
    } else {
        // right clipped
        s16 z_adjust = near_z_16 - rz2;
        s32 x_adjust = dx_over_dz * z_adjust;
        s16 x_adjust_16 = x_adjust>>6;
        rx2 += x_adjust_16;
        rz2 = near_z_16;
        trans_v2->x = rx2;
        trans_v2->y = rz2;
        return RIGHT_CLIPPED;
    }
    
}



clip_result clip_map_vertex(Vect2D_f32* trans_v1, Vect2D_f32* trans_v2) {
    // TODO: figure out how much precision we need here (16-bit instead of 32?)
    // TODO: adjust texture coordinates here as well

    fix32 rx1 = trans_v1->x;
    fix32 rz1 = trans_v1->y;
    fix32 rx2 = trans_v2->x;
    fix32 rz2 = trans_v2->y;

    if(rz1 <= near_z_32 && rz2 <= near_z_32) {
        return OFFSCREEN;
    }
    if(rz1 > near_z_32 && rz2 > near_z_32) {
        return UNCLIPPED;
    }    

    fix32 dz = rz2-rz1;
    fix32 dx = rx2-rx1;

    // SLOW
    s16 dx_over_dz = fix32Div(dx, dz); // change in x per change in z


    if(rz1 < near_z_32) {
        // left clipped
        fix32 z_adjust = near_z_32 - rz1;  // how much we need to adjust z by
        fix32 x_adjust = fix32Mul(z_adjust, dx_over_dz); // multiply z_adjust by dx_over_dx -> x_adjust
        rx1 += x_adjust;    // modify x coord
        rz1 = near_z_32;
        trans_v1->x = rx1;
        trans_v1->y = rz1;
        return LEFT_CLIPPED;
    } else {
        // right clipped
        fix32 z_adjust = near_z_32 - rz2;
        fix32 x_adjust = fix32Mul(z_adjust, dx_over_dz);
        rx2 += x_adjust;
        rz2 = near_z_32;
        trans_v2->x = rx2;
        trans_v2->y = rz2;
        return RIGHT_CLIPPED;
    }
}
