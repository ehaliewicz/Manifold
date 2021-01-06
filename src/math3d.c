#include <genesis.h>
#include "math3d.h"
#include "game.h"

//Vect2D_s32
Vect2D_s32 transform_map_vert(s16 x, s16 y) {
    
    //s16 x_shifted = ((x<<4)/ZOOM); //>>ZOOM_SHIFT);
    //s16 y_shifted = ((y<<4)/ZOOM); //ZOOM_SHIFT);
    //s16 x_shifted = x;
    //s16 y_shifted = y;
    
    //s16 tlx = x_shifted - playerXFrac4;
    //s16 tly = y_shifted - playerYFrac4; // scaling factor of 64 (2^6)
    
    //fix32 rx = ((tlx)*(angleCosFrac12)) - ((tly)*(angleSinFrac12)); // 12.4 * 1.12 = 13.16? result I know it's 16 bits of fractional precision
    //fix32 ry = ((tlx)*(angleSinFrac12)) + ((tly)*(angleCosFrac12));

    //return transform_vert_native(x, y);
    
    //s16 tlx = fix32ToInt(intToFix32(x) - cur_player_pos.x);
    //s16 tly = fix32ToInt(intToFix32(y) - cur_player_pos.y);

    s32 tlx = intToFix32(x) - cur_player_pos.x;
    s32 tly = intToFix32(y) - cur_player_pos.y;

    // 10 fractional bits

    //s32 rx = ((tlx)*(angleCos32)) - ((tly)*(angleSin32)); // 22 * 22.10 = 22.10
    //s32 ry = ((tlx)*(angleSin32)) + ((tly)*(angleCos32));
    
    s32 rx = fix32Mul((tlx),(angleSin32)) - fix32Mul((tly),(angleCos32));
    s32 ry = fix32Mul((tlx),(angleCos32)) + fix32Mul((tly),(angleSin32));

    return (Vect2D_s32){.x = rx, .y = ry};
}




s16 project_and_adjust_2d(Vect2D_s32 trans_map_vert) {
    s32 rx = trans_map_vert.x;
    s32 rz = trans_map_vert.y;

    s32 const2Rx = (CONST2 * rx);
    s16 const2RxDivZ = const2Rx / rz;
    s16 transX = CONST1 + const2RxDivZ;
    return transX;
}


//const s32 near_z = 160 << FIX32_FRAC_BITS;
const s32 near_z = 40 << FIX32_FRAC_BITS;

int is_behind_near_plane(Vect2D_s32 transformed_map_vert) {
    return transformed_map_vert.y < near_z;
}


clip_result clip_map_vertex(Vect2D_s32* trans_v1, Vect2D_s32* trans_v2) {
    // TODO: figure out how much precision we need here (16-bit instead of 32?)
    // TODO: adjust texture coordinates here as well

    s32 rx1 = trans_v1->x;
    s32 rz1 = trans_v1->y;
    s32 rx2 = trans_v2->x;
    s32 rz2 = trans_v2->y;

    if(rz1 <= near_z && rz2 <= near_z) {
        return OFFSCREEN;
    }
    if(rz1 > near_z && rz2 > near_z) {
        return UNCLIPPED;
    }    

    s32 dz = rz2-rz1;
    s32 dx = rx2-rx1;

    // SLOW
    s16 dx_over_dz = fix32Div(dx, dz); // change in x per change in z


    if(rz1 < near_z) {
        // left clipped
        s32 z_adjust = near_z - rz1;  // how much we need to adjust z by
        s32 x_adjust = fix32Mul(z_adjust, dx_over_dz); // multiply z_adjust by dx_over_dx -> x_adjust
        rx1 += x_adjust;    // modify x coord
        rz1 = near_z;
        trans_v1->x = rx1;
        trans_v1->y = rz1;
        return LEFT_CLIPPED;
    } else {
        // right clipped
        s32 z_adjust = near_z - rz2;
        s32 x_adjust = fix32Mul(z_adjust, dx_over_dz);
        rx2 += x_adjust;
        rz2 = near_z;
        trans_v2->x = rx2;
        trans_v2->y = rz2;
        return RIGHT_CLIPPED;
    }
}

transformed_vert project_and_adjust_3d(Vect2D_s32 trans_map_vert, s16 floor, s16 ceil) {
    s32 rx = trans_map_vert.x;
    s32 rz = trans_map_vert.y;

    s32 const2Rx = (CONST2 * rx); // 20.10 precision, might overflow
    s16 cosnt2RxDivZ = const2Rx / rz; // 20 bits of precision here
    s16 transX = CONST1 + cosnt2RxDivZ;

    // needs to adjust by camera height here
    s16 const4RyCeil = (CONST4 * (ceil - fix32ToInt(cur_player_pos.z)));
    s16 const4RyCeilDivZ = ((const4RyCeil)<<FIX32_FRAC_BITS) / rz;

    s16 yceil = CONST3 + const4RyCeilDivZ;

    s16 const4RyFloor = (CONST4 * (floor - fix32ToInt(cur_player_pos.z)));
    s16 const4RyFloorDivZ = ((const4RyFloor)<<FIX32_FRAC_BITS) / rz;

    s16 yfloor = CONST3 + const4RyFloorDivZ;

    transformed_vert ret = {.x = transX, .yceil = (BMP_HEIGHT-1)-yceil, .yfloor = (BMP_HEIGHT-1)-yfloor};
    return ret;
}