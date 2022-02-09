#include <genesis.h>
#include "counter.h"
#include "div_lut.h"
#include "math3d.h"
#include "game.h"
#include "texture.h"
#include "utils.h"

/*
Vect2D_f32 transform_map_vert(s16 x, s16 y) {
    fix32 tlx = intToFix32(x) - cur_player_pos.x;
    fix32 tly = intToFix32(y) - cur_player_pos.y;
    
    fix32 rx = fix32Mul((tlx),(angleSin32)) - fix32Mul((tly),(angleCos32));
    fix32 ry = fix32Mul((tlx),(angleCos32)) + fix32Mul((tly),(angleSin32));

    return (Vect2D_f32){.x = rx, .y = ry};
}
*/

/*
Vect2D_s16 transform_map_vert_16(s16 x, s16 y) {
    
    s16 tlx = x - playerXInt; // 16-bit integer
    s16 tly = y - playerYInt; // 16-bit integer

    s32 rx = (tlx * angleSin16) - (tly * angleCos16); // 22.10 * 16 -> 22.10
    s32 ry = (tlx * angleCos16) + (tly * angleSin16);
    s16 res_x = rx>>(FIX16_FRAC_BITS);
    s16 res_y = ry>>(FIX16_FRAC_BITS);
    return (Vect2D_s16){.x = res_x, .y = res_y};
}
*/

Vect2D_s16 transform_map_vert_16(s16 x, s16 y) {
    s16 tlx = x - playerXInt; // 16-bit integer
    s16 tly = y - playerYInt; // 16-bit integer

    s32 rx = (tlx * angleSin16) - (tly * angleCos16); // 22.10 * 16 -> 22.10
    s32 ry = (tlx * angleCos16) + (tly * angleSin16);
    s16 res_x = rx>>(FIX16_FRAC_BITS);
    s16 res_y = ry>>(FIX16_FRAC_BITS-TRANS_Z_FRAC_BITS); // 12.4

    /*
    if(0) { //if(debug) {
        KLog_S1("playerXInt: ", playerXInt);
        KLog_S1("playerYInt: ", playerYInt);
        KLog_S1("vert x: ", x);
        KLog_S1("vert y: ", y);
        KLog_S1("playerAngle: ", cur_player_pos.ang);
        KLog_S1("angleSin16: ", angleSin16);
        KLog_S1("angleCos16: ", angleCos16);
        KLog_S1("tlx: ", tlx);
        KLog_S1("tly: ", tly);
        KLog_S1("rx: ", rx);
        KLog_S1("ry: ", ry);
        KLog_S1("res_x int: ", res_x);
        KLog_S1("res_y_fix_4: ", res_y);
    }
    */

    return (Vect2D_s16){.x = res_x, .y = res_y};
}


s8 point_sign_int_vert(fix32 x, fix32 y, s16 v1_x, s16 v1_y, s16 v2_x, s16 v2_y) {
    fix32 left = (v2_x-v1_x)*(y-intToFix32(v1_y));
    fix32 right = (v2_y-v1_y)*(x-intToFix32(v1_x));
    fix32 res = left - right;
    return (res > 0 ? 1 : (res < 0 ? -1 : 0));
}


s8 sign(s16 ax, s16 ay, s16 bx, s16 by, s16 cx, s16 cy) {
    s32 p1 = bx-ax;
    s32 val = ((bx - ax)*(cy - ay) - (by - ay)*(cx - ax));
    //KLog_S2("ax: ", ax, "ay: ", ay);
    //KLog_S2("bx: ", bx, "by: ", by);
    //KLog_S2("cx: ", cx, "cy: ", cy);
    //KLog_S1("val: ", val);
    //return val;
    
    return (val > 0 ? 1 : (val < 0 ? -1 : 0));
}


s16 sign_left_frustum(s16 cx, s16 cy) {
    s16 val = (-cy - cx);
    return val; //(val > 0 ? 1 : (val < 0 ? -1 : 0));
}

s16 sign_right_frustum(s16 cx, s16 cy) {
    s16 val = (cy- cx); 
    return val; //(val > 0 ? 1 : (val < 0 ? -1 : 0));
}

u8 point_within_frustum(s16 x, s16 y) {
    u8 within = ((sign(0,0, -1,1, x, y) <= 0) && (sign(0,0,1,1,x,y) >= 0));

    return within;
}

u8 within_frustum(s16 x1, s16 y1, s16 x2, s16 y2) {
    //s8 p1_left_sign = sign_left_frustum(x1, y1) <= 0;
    //if(p1_left_sign) { return 1; }
    //s8 p1_right_sign = sign_right_frustum(x1,y1) >= 0;
    //if(p1_right_sign) { return 1; }
    //s8 p2_left_sign = sign_left_frustum(x2,y2) <= 0;
    //if(p2_left_sign) { return 1; }
    //s8 p2_right_sign = sign_right_frustum(x2,y2) >= 0;
    //if(p2_right_sign) { return 1; }
    
    s16 tmp; u8 ret=0;
    __asm volatile(
        "move.w %2, %5\t\n\
         sub.w %1, %5\t\n\
         bge.b second_half_%=\t\n\
         move.w %4, %5\t\n\
         sub.w %3, %5\t\n\
         bge.b second_half_%=\t\n\
         bra.b exit_%=\t\n\
        second_half_%=:\t\n\
         neg.w %2\t\n\
         sub.w %1, %2\t\n\
         ble.b within_%=\t\n\
         neg.w %4\t\n\
         sub.w %3, %4\t\n\
         ble.b within_%=\t\n\
         bra.b exit_%=\t\n\
        within_%=:\t\n\
         moveq #1, %0\t\n\
        exit_%=:"
        : "+d" (ret)
        : "d" (x1), "d" (y1), "d" (x2), "d" (y2), "d" (tmp));
        
    inc_counter(PRE_PROJ_FRUSTUM_CULL_COUNTER, ret);
    return ret;
}


s16 project_and_adjust_x(s16 x, s16 z_recip) {

    
    //s16 const2Rx = x; //<<5; //(32 * x);

    
    //s16 transX; // = 32;
    /*
    __asm volatile(
        "asl.w #5, %0\t\n\
        muls.w %1, %0\t\n\
        swap %0\t\n\
        add.w #32, %0"
        : "+d" (x)
        : "d" (z_recip)
    );
    return x;
    */

    __asm volatile(
            "lsl.w #5, %0"
            : "+d" (x)
    );
    s16 const2RXMulZRecip = x;

    __asm volatile(
            "muls.w %1, %0\t\n\
             swap %0"
            :  "+d" (const2RXMulZRecip), "+d" (z_recip) // output
            : // input
    );

    

    s16 transX = 32 + const2RXMulZRecip; //(const2RXMulZRecip>>16);
    
    return transX;
    //return transX;
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

clip_result clip_map_vertex_16(Vect2D_s16* trans_v1, Vect2D_s16* trans_v2, texmap_params* tmap, u32 wall_len) {
    // TODO: adjust texture coordinates here as well
    

    
    s16 rx1 = trans_v1->x;
    s16 rz1_12_4 = trans_v1->y; // 12.4
    s16 rx2 = trans_v2->x;
    s16 rz2_12_4 = trans_v2->y; // 12.4

    //s16 rz1 = rz1_fix >> TRANS_Z_FRAC_BITS;
    //s16 rz2 = rz2_fix >> TRANS_Z_FRAC_BITS;

    if(rz1_12_4 < NEAR_Z_FIX && rz2_12_4 < NEAR_Z_FIX) {    
        return OFFSCREEN;
    }

    s16 dz_12_4 = rz2_12_4 - rz1_12_4;

    #define TEX_REPEAT_DIST 64
    u16 repetitions = max(1, wall_len / TEX_REPEAT_DIST);
    tmap->repetitions = repetitions;
    //KLog_U1("repetitions: ", repetitions);

    s32 base_left_u_16 = 0;
    s32 base_right_u_16 = (repetitions)<<16;
    s32 fix_du_16 = (base_right_u_16-base_left_u_16);

    s32 du_over_dz_16; // = fix_du; // 16.16

    if((dz_12_4>>4) != 0) {
        du_over_dz_16 = (fix_du_16<<TRANS_Z_FRAC_BITS) / dz_12_4; // fix 16
        if(0) { //if(debug) {
            KLog_S1("du_over_dz_20: ", du_over_dz_16);
        }
        //__asm volatile(
        //    "divs.w %1, %0"
        //    : "+d" (du_over_dz) 
        //    : "d" (dz)
        //);
        tmap->du_over_dz = du_over_dz_16;

    } else {
        if(0) { //if(debug) {
            KLog("NO PERSPECTIVE NEEDED");
        }
        tmap->left_u = base_left_u_16;
        tmap->right_u = base_right_u_16;
    }

    if(rz1_12_4 >= NEAR_Z_FIX && rz2_12_4 >= NEAR_Z_FIX) {
        if(0) { //if(debug) {
            KLog("NOT CLIPPED");
        }
        //u32 fix_du = 32<<6;

        //__asm volatile(
        //    "divs.w %1, %0"
        //    :  "+d" (du_over_dx)  // output
        //    :  "d" (dx) // input
        //);
        tmap->left_u = base_left_u_16;
        tmap->right_u = base_right_u_16;

        return UNCLIPPED;
    }
    
    s16 dx = rx2-rx1;
    //KLog_S1("dx: ", dx);


    // SLOW
    #ifdef CLIP_DIVISION
    //s32 fix_dx = dx<<6; // .6
    s32 dx_over_dz_6 = (dx<<(6+TRANS_Z_FRAC_BITS))/ dz_12_4; // << 6
    if(0) { //if(debug) {
        KLog_S1("dx over dz: ", dx_over_dz_6);
    }

    //s16 dx_over_dz_6 = fix_dx<<TRANS_Z_FRAC_BITS;

    //divs.w %3, %1
    // dx_over_dz = fix 6
    //__asm volatile(
    //    "divs.w %1, %0"
    //    :  "+d" (dx_over_dz_6)  // output
    //    :  "d" (dz_12_4) // input
    //);
   
    
   #else
   s16 inv_dz = (dz < 0) ? -(z_recip_table[-dz]>>10) : (z_recip_table[dz]>>10);
   s16 dx_over_dz = dx * inv_dz; //dx_over_dz_32 >> 10;
   #endif 
    

    if(rz1_12_4 < NEAR_Z_FIX) { 
        if(0) { //if(debug) {
            KLog("LEFT CLIPPED");
        }
        //if(dz == 0) { die("wtf"); }
        // left clipped
        s16 z_adjust_12_4 = NEAR_Z_FIX-rz1_12_4;
        s32 x_adjust_10 = dx_over_dz_6 * z_adjust_12_4;
        s16 x_adjust_int = x_adjust_10>>(6+TRANS_Z_FRAC_BITS);

        if(0) { //if(debug) {
            KLog_S1("z_adjust_10_6: ", z_adjust_12_4);
            KLog_S1("x_adjust_12: ", x_adjust_10);
            KLog_S1("x_adjust_int: ", x_adjust_int);
        }

        s32 u_adjust_20 = (du_over_dz_16 * z_adjust_12_4); // 12.20 // 12.4 fixed point

        if(0) { //if(debug) {
            //KLog_S1("u_adjust_22: ", u_adjust_20);
        }
        
        rx1 += x_adjust_int;    // modify x coord
        rz1_12_4 = NEAR_Z_FIX;
        trans_v1->x = rx1;
        trans_v1->y = rz1_12_4;
        
        //KLog_S1("z adjust: ", z_adjust);
        //KLog_S1("u adjust: ", u_adjust);
        //KLog_S1("u result: ", base_left_u + u_adjust);
        tmap->left_u = base_left_u_16 + (u_adjust_20>>4);
        if(0) { //if(debug) {
            KLog_S1("clipped left u: ", tmap->left_u);
        }
        tmap->right_u = base_right_u_16;

        return LEFT_CLIPPED;
    } else {
        if(0) { //if(debug) {
            KLog("RIGHT CLIPPED");
        }
        // right clipped
        s16 z_adjust_12_4 = NEAR_Z_FIX - rz2_12_4; 
        s32 x_adjust_10 = dx_over_dz_6 * z_adjust_12_4; // dx_over_dz is negative
        s16 x_adjust_int = x_adjust_10>>(6+TRANS_Z_FRAC_BITS);
        rx2 += x_adjust_int;
        rz1_12_4 = NEAR_Z_FIX;
        trans_v2->x = rx2;
        trans_v2->y = rz1_12_4;

        s32 u_adjust_20 = du_over_dz_16 * z_adjust_12_4;
        tmap->left_u = base_left_u_16;
        tmap->right_u = base_right_u_16 + (u_adjust_20>>4);
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