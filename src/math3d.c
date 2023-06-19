#include <genesis.h>
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



// returns -1 if on right side, -1 if on left side

s8 point_sign_int_vert(f32 x, f32 y, s16 v1_x, s16 v1_y, s16 v2_x, s16 v2_y) {
    //s32 left = muls_16_by_16(
    //    (v2_x-v1_x),
    //    (y-v1_y));
    //s32 right = muls_16_by_16(
    //    (v2_y-v1_y),
    //    (x-v1_x));
    //s32 res = left-right;
    //return (res > 0 ? 1 : (res < 0 ? -1 : 0));

    
    //__asm volatile(
    //    "sub.w v1_x, v2_x"
    //    : "+d" (ret) // output
    //    : // input
    //    );

    s16 ndx = v2_x-v1_x;
    if(ndx == 0) {
        s16 ndy = v2_y-v1_y;
        f32 fv1_x = intToFix32(v1_x);
        if(x <= fv1_x) {
            return ndy > 0;
        }
        return ndy < 0;
    }

    s16 ndy = v2_y-v1_y;
    if(ndy == 0) {
        f32 fv1_y = intToFix32(v1_y);
        if(y <= fv1_y) {
            return ndx < 0;
        }
        return ndx > 0;
    }

    f32 dx = (x - intToFix32(v1_x));
    f32 dy = (y - intToFix32(v1_y));
    f32 fndx = intToFix32(ndx);
    f32 fndy = intToFix32(ndy);
    if ( (fndy ^ fndx ^ dx ^ dy)&0x80000000 ) {
        if  ( (ndy ^ dx) & 0x80000000 ) {
            // (left is negative)
            return 1;
        }
        return 0;
    }
    

    f32 left = ndy*dx;
    f32 right = dy*ndx;

    if(right < left) {
        return 0;
    }

    return 1; // back side

    /*
        WORKING CODE

    s16 dx = v2_x-v1_x;
    if(dx == 0) {

        s16 dy = v2_y-v1_y;
        // if right is > 0, it's larger than left
        // if it's less than 0, it's less than left
        // if it's equal to 0, it's equal to left
        if(dy == 0) {
            return 0;
        }
        fix32 right = dy*(x-intToFix32(v1_x));
        return (right > 0 ? -1 : (right < 0 ? 1 : 0));
    }

    s16 dy = v2_y-v1_y;
    if(dy == 0) {
        // if left > 0, it's larger than right
        // if left < 0, it's smaller than left
        // if left == 0, it's equal to left
        fix32 left = dx*(y-intToFix32(v1_y));
        return (left > 0 ? 1 : (left < 0 ? -1 : 0));
    }
    fix32 left = dx*(y-intToFix32(v1_y));
    fix32 right = dy*(x-intToFix32(v1_x));
    fix32 res = left - right;
    // if left > right return 1
    // if left < right return -1
    // else return 0
    return (res > 0 ? 1 : (res < 0 ? -1 : 0));
    */
}


s8 sign(s16 ax, s16 ay, s16 bx, s16 by, s16 cx, s16 cy) {
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
    return (val > 0 ? 1 : (val < 0 ? -1 : 0));
}

s16 sign_right_frustum(s16 cx, s16 cy) {
    s16 val = (cy- cx); 
    return (val > 0 ? 1 : (val < 0 ? -1 : 0));
}

u8 within_frustum(s16 x1, s16 y1, s16 x2, s16 y2) {
    //KLog_S4("x1: ", x1, " y1: ", y1, " x2: ", x2, " y2: ", y2);
    u8 ret = 0;
    //if((y1-x1 >= 0) || (y2-x2 >= 0)) {
    //    ret = (-y1-x1 <= 0) || (-y2-x2 <= 0);
    //}
    //return ret;
    u8 p1_out_on_left = -x1 > y1;
    u8 p2_out_on_left = -x2 > y2;
    u8 p1_out_on_right = x1 > y1;
    u8 p2_out_on_right = x2 > y2;
    if(p1_out_on_left && p2_out_on_left) {
        return 0;
    }
    if(p1_out_on_right && p2_out_on_right) {
        return 0;
    }
    return 1;
    //ret = ((!p1_out_on_left) || (!p2_out_on_left)) && ((!p1_out_on_right) || (!p2_out_on_right));

    
    //s8 p1_left_sign = sign_left_frustum(x1, y1) <= 0;
    //if(p1_left_sign) { ret = 1; }
    //s8 p1_right_sign = sign_right_frustum(x1,y1) >= 0;
    //if(p1_right_sign) { ret = 1; }
    //s8 p2_left_sign = sign_left_frustum(x2,y2) <= 0;
    //if(p2_left_sign) { ret =  1; }
    //s8 p2_right_sign = sign_right_frustum(x2,y2) >= 0;
    //if(p2_right_sign) { ret =  1; }
    
    
    //  first check against right frustum (cy-cx) >= 0 for both points
    //  if either are valid, go to second check
    //  if neither are valid, exit with failure
    //  second, check against left frustum (-cy-cx) <= 0
    
    /*
    __asm volatile(
        "cmp.w %1, %2\t\n\
         bge.b second_half_%=\t\n\
         cmp.w %3, %4\t\n\
         blt.b exit_%=\t\n\
        second_half_%=:\t\n\
         cmp.w %2, %1\t\n\
         ble.b within_%=\t\n\
         cmp.w %4, %3\t\n\
         ble.b within_%=\t\n\
         bra.b exit_%=\t\n\
        within_%=:\t\n\
         moveq #1, %0\t\n\
        exit_%=:"
        : "+d" (ret)
        : "d" (x1), "d" (y1), "d" (x2), "d" (y2), "d" (tmp));
    */

    if(!ret) {
        //inc_counter(PRE_PROJ_FRUSTUM_CULL_COUNTER);
    }
    //KLog_U1("within frustum: ", ret);
    return ret;
    
}


s16 project_and_adjust_x(s16 x, s16 z_recip) {
    
    //z_recip >>= 1; // x is 16.0, z_recip is now 0.15
    //x <<= 4; // x is 16.4
    //s32 mult = z_recip * x; // 13.19
    //mult<<=2;
    //return (mult>>16) + 32;
    
   
    // trying putting the shift after the mul and swap
    __asm volatile(
             "lsl.w #5, %0\t\n\
             muls.w %1, %0\t\n\
             swap %0\t\n\
             add.w #32, %0\t\n\
            "
            : "+d" (x)
            : "d" (z_recip)
    );
    return x;
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
    s16 yMinusPosZ = y - playerZCam12Frac4;      // 12.4
    s32 const4Ry = yMinusPosZ; //CONST4 * yMinusPosZ;

    //s32 const4RyMulRecipZ = const4Ry * z_recip_table[z];
    //s16 z_recip = z_recip_table[z];

    s32 const4RyMulRecipZ = const4Ry * z_recip;
    s32 highConst4RyMulRecipZ;

    // shifts are 28 cycles

    // vs load 3 and shift with register
    // thats 4 cycles to load

    //u8 shift = 3;
    __asm volatile(
        "move.l %0, %1\t\n\
        lsl.l #3, %1\t\n\
        add.l %1, %0\t\n\
        lsl.l #3, %0\t\n\
        swap %0\t\n\
        sub.w #1136, %0\t\n\
        neg.w %0\t\n\
        "
        : "+d" (const4RyMulRecipZ), "=&d" (highConst4RyMulRecipZ)
        : //"d" (shift)
    );

    return const4RyMulRecipZ;

    //s32 const4RyMulRecipZ = (const4Ry * z_recip);// * CONST4);
    //const4RyMulRecipZ += (const4RyMulRecipZ<<3);
    //const4RyMulRecipZ <<= 3;

    /*
    s32 const4RyMulRecipZ = const4Ry; // * x_project_z_recip_table[z];
    __asm volatile(
            "muls.w %1, %0"
            :  "+d" (const4RyMulRecipZ), "+d" (z_recip) // output
            : // input
    );
    const4RyMulRecipZ = (const4RyMulRecipZ<<3) + (const4RyMulRecipZ<<6); // <<= 6;
    */

    //s16 highConst4RyMulRecipZ = const4RyMulRecipZ>>16;



    //s16 yproj = (CONST3<<4) + highConst4RyMulRecipZ;//(const4RyMulRecipZ>>16);
    //return -(yproj-((SCREEN_HEIGHT-1)<<4));
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

s16 line_slope(s16 x1, s16 y1_4, s16 x2, s16 y2_4) {
    if((x1-x2) == 0) {
        return 0;
    }
    return (y1_4-y2_4)/(x1-x2);
}


#define TEX_REPEAT_DIST (64)


unsigned int int_sqrt (unsigned int n) {

    unsigned int result = 0;
    unsigned int odd    = 1;
    unsigned int oddsum = 1;

    while (oddsum <= n) {

        result = result + 1;
        odd = odd + 2;
        oddsum = oddsum + odd;

    }

    return result;

}

u16 get_texture_repetitions(s16 v1x, s16 v1y, s16 v2x, s16 v2y) {
    //u16 repetitions = max(1, wall_len / TEX_REPEAT_DIST);
    //u32 len = getApproximatedDistance(v2x-v1x, v2y-v1y);
    u32 sqx = (v2x-v1x)*(v2x-v1x); 
    u32 sqy = (v2y-v1y)*(v2y-v1y);

    u32 sum_sqs = sqx+sqy;
    u32 dist = int_sqrt(sum_sqs);

    u16 repetitions = max(1, dist / TEX_REPEAT_DIST);
    return repetitions;
}

clip_result frustum_cull_vertex_16(Vect2D_s16* __restrict__ trans_v1, Vect2D_s16* __restrict__ trans_v2) {
    
    s16 rx1 = trans_v1->x;
    s16 rz1_12_4 = trans_v1->y; // 12.4
    s16 rx2 = trans_v2->x;
    s16 rz2_12_4 = trans_v2->y; // 12.4

    const u8 left_vert_outside_left_frustum = (-rx1 > (rz1_12_4>>4));
    const u8 right_vert_outside_left_frustum = (-rx2 > (rz2_12_4>>4));
    const u8 left_vert_outside_right_frustum = (rx1 > (rz1_12_4>>4));
    const u8 right_vert_outside_right_frustum = (rx2 > (rz2_12_4>>4));
    if(left_vert_outside_left_frustum && right_vert_outside_left_frustum) {
        return OFFSCREEN;
    }
    if(left_vert_outside_right_frustum && right_vert_outside_right_frustum) {
        return OFFSCREEN;
    }
    return UNCLIPPED;
}

clip_result clip_map_vertex_16(Vect2D_s16* __restrict__ trans_v1, Vect2D_s16* __restrict__ trans_v2, texmap_params* __restrict__ tmap) {
    // TODO: adjust texture coordinates here as well
    

    
    s16 rx1 = trans_v1->x;
    s16 rz1_12_4 = trans_v1->y; // 12.4
    s16 rx2 = trans_v2->x;
    s16 rz2_12_4 = trans_v2->y; // 12.4


    clip_result clip_status = UNCLIPPED;

    if(rz1_12_4 < NEAR_Z_FIX && rz2_12_4 < NEAR_Z_FIX) {  
        return OFFSCREEN;  
    }

    
    const u8 left_vert_outside_left_frustum = (-rx1 > (rz1_12_4>>4));
    const u8 right_vert_outside_left_frustum = (-rx2 > (rz2_12_4>>4));
    const u8 left_vert_outside_right_frustum = (rx1 > (rz1_12_4>>4));
    const u8 right_vert_outside_right_frustum = (rx2 > (rz2_12_4>>4));
    if(left_vert_outside_left_frustum && right_vert_outside_left_frustum) {
        return OFFSCREEN;
    }
    if(left_vert_outside_right_frustum && right_vert_outside_right_frustum) {
        return OFFSCREEN;
    }

    s16 dz_12_4 = rz2_12_4 - rz1_12_4;

    s32 base_left_u_16 = 0;
    s32 base_right_u_16 = (1)<<16;
    s32 fix_du_16 = (base_right_u_16-base_left_u_16);

    s32 du_over_dz_16;
    if(1) { //tmap->needs_texture) {
        if(dz_12_4 != 0) {
            du_over_dz_16 = divs_32_by_16(fix_du_16<<TRANS_Z_FRAC_BITS, dz_12_4);      
            tmap->du_over_dz = du_over_dz_16;  
        } else {
            tmap->left_u = base_left_u_16;
            tmap->right_u = base_right_u_16;
        }
    }

    // this frustum code seems to cause some issues with portal clipping
    // with pvs renderer, doesn't seem to be an issue?
    if(0) { //left_vert_outside_left_frustum) {


        //s16 frustum_left_x = -16384;
        //s16 frustum_left_y = 16384;

        s16 intersection_z_4;
        s16 intersection_x;


        if(rx1 == rx2) {
            intersection_x = rx1;
            intersection_z_4 = -rx1<<4;
            
            // TODO: for solid color walls we can skip all this stuff
            // but it breaks for some reason
            if(tmap->needs_texture) {
                s16 z_adjust_4 = intersection_z_4 - rz1_12_4;
                
                s32 u_adjust_20 = du_over_dz_16 * z_adjust_4;
                base_left_u_16 += (u_adjust_20>>4);
            }

        } else {
            
            //s16 slope_a_4 = (rz2_12_4-rz1_12_4)/(rx2-rx1);            
            s16 dx = rx2-rx1;
            s32 tmp = rz2_12_4-rz1_12_4; 
            s16 slope_a_4;
            __asm volatile(
                "divs.w %2, %0\t\n\
                move.w %0, %1"

                : "+d" (tmp), "=d" (slope_a_4)
                : "d" (dx)
            );

            if(slope_a_4 == 0) {
                intersection_x = -rz1_12_4>>4;
                intersection_z_4 = rz1_12_4;

                if(tmap->needs_texture) {
                    s32 du_over_dx_16 = (fix_du_16) / (rx2-rx1);
                    s16 x_adjust = intersection_x - rx1;
                    s32 u_adjust_16 = x_adjust * du_over_dx_16;
                    base_left_u_16 += u_adjust_16;
                }
            } else {
 
                const s16 slope_b_4 = -1<<4;
                s16 dslope = slope_a_4-slope_b_4;
                s32 tmp = slope_a_4; //*rx1;
                __asm volatile(
                    "muls.w %1, %0"
                    : "+d" (tmp)
                    : "d" (rx1)
                );
                tmp -= rz1_12_4;
                __asm volatile(
                    "divs.w %2, %0\t\n\
                    move.w %0, %1"
                    : "+d" (tmp), "=d" (intersection_x)
                    : "d" (dslope)
                );

                intersection_z_4 = -intersection_x<<4; 
  

                //    kinda working!
                if(1) { //tmap->needs_texture) {
                    s16 z_adjust_4 = intersection_z_4 - rz1_12_4;
                    s16 x_adjust = intersection_x - rx1;

                
                    if((z_adjust_4>>4) > x_adjust) {
                        s32 u_adjust_20 = du_over_dz_16 * z_adjust_4;
                        base_left_u_16 += (u_adjust_20>>4);

                    } else {
                        
                        s32 du_over_dx_16 = (fix_du_16) / (rx2-rx1);

                        s32 u_adjust_16 = x_adjust * du_over_dx_16;
                        base_left_u_16 += u_adjust_16;
                    }
                }

            }
        }

        rz1_12_4 = intersection_z_4;
        trans_v1->y = rz1_12_4;
        rx1 = intersection_x;
        trans_v1->x = rx1;
        clip_status |= LEFT_FRUSTUM_CLIPPED;
        
    }

    if(0) { //right_vert_outside_right_frustum) {
        s16 intersection_z_4;
        s16 intersection_x;
        if(rx1 == rx2) {
            intersection_x = rx2;
            intersection_z_4 = rx2<<4;

            if(tmap->needs_texture) {
                s16 z_adjust_4 = intersection_z_4 - rz2_12_4;
                s32 u_adjust_20 = du_over_dz_16 * z_adjust_4;
                base_right_u_16 += (u_adjust_20>>4);
            }
        } else {
            s16 dx = rx2-rx1;
            s32 tmp = rz2_12_4-rz1_12_4; 
            s16 slope_a_4;
            __asm volatile(
                "divs.w %2, %0\t\n\
                move.w %0, %1"

                : "+d" (tmp), "=d" (slope_a_4)
                : "d" (dx)
            );

            if(slope_a_4 == 0) {
                intersection_x = rz2_12_4>>4;
                intersection_z_4 = rz2_12_4;

                if(tmap->needs_texture) {
                    s32 du_over_dx_16 = (fix_du_16) / (rx2-rx1);
                    s16 x_adjust = intersection_x - rx2;
                    s32 u_adjust_16 = x_adjust * du_over_dx_16;
                    base_right_u_16 += u_adjust_16;
                }
            } else {
                const s16 slope_b_4 = 1<<4;
                s16 dslope = slope_a_4-slope_b_4;
                s32 tmp = slope_a_4; // * rx2
                __asm volatile(
                    "muls.w %1, %0"
                    : "+d" (tmp)
                    : "d" (rx2)
                );
                tmp -= rz2_12_4;
                __asm volatile(
                    "divs.w %2, %0\t\n\
                    move.w %0, %1"
                    : "+d" (tmp), "=d" (intersection_x)
                    : "d" (dslope)
                );

                intersection_z_4 = intersection_x<<4;

                if(tmap->needs_texture) {
                    s16 z_adjust_4 = intersection_z_4 - rz2_12_4;
                    s16 x_adjust = intersection_x - rx2;

                    //    kinda working!
                    if((z_adjust_4>>4) > x_adjust) {
                        s32 u_adjust_20 = du_over_dz_16 * z_adjust_4;
                        base_right_u_16 += (u_adjust_20>>4);

                    } else {
                        
                        s32 du_over_dx_16 = (fix_du_16) / (rx2-rx1);
                        s32 u_adjust_16 = x_adjust * du_over_dx_16;
                        base_right_u_16 += u_adjust_16;
                    }
                }

            }
        }
        rz2_12_4 = intersection_z_4;
        trans_v2->y = rz2_12_4;
        rx2 = intersection_x;
        trans_v2->x = rx2;
        clip_status |= RIGHT_FRUSTUM_CLIPPED;
    }

    if(rz1_12_4 >= NEAR_Z_FIX && rz2_12_4 >= NEAR_Z_FIX) {
        if(tmap->needs_texture) {
            tmap->left_u = base_left_u_16;
            tmap->right_u = base_right_u_16;
        }
        clip_status = UNCLIPPED;
        return clip_status;
    }
    
    s16 dx = rx2-rx1;


    // SLOW
    #ifdef CLIP_DIVISION
    s32 dx_over_dz_6 = (dx<<(6+TRANS_Z_FRAC_BITS))/ dz_12_4; // << 6
   #else
    s16 inv_dz = (dz < 0) ? -(z_recip_table[-dz]>>10) : (z_recip_table[dz]>>10);
    s16 dx_over_dz = dx * inv_dz; //dx_over_dz_32 >> 10;
   #endif 




    // left near clip
    if(rz1_12_4 < NEAR_Z_FIX) { 
        s16 z_adjust_12_4 = NEAR_Z_FIX-rz1_12_4;
        s32 x_adjust_10 = dx_over_dz_6 * z_adjust_12_4;
        s16 x_adjust_int = x_adjust_10>>(6+TRANS_Z_FRAC_BITS);

        rx1 += x_adjust_int;    // modify x coord
        rz1_12_4 = NEAR_Z_FIX;
        trans_v1->x = rx1;
        trans_v1->y = rz1_12_4;
   
        if(tmap->needs_texture) {
            s32 u_adjust_20 = (du_over_dz_16 * z_adjust_12_4); // 12.20 // 12.4 fixed point
            tmap->left_u = base_left_u_16 + (u_adjust_20>>4);
            tmap->right_u = base_right_u_16;
        }
        clip_status |= LEFT_Z_CLIPPED;
        clip_status &= (~LEFT_FRUSTUM_CLIPPED);
    } else {

        // right clipped
        s16 z_adjust_12_4 = NEAR_Z_FIX - rz2_12_4; 
        s32 x_adjust_10 = dx_over_dz_6 * z_adjust_12_4; // dx_over_dz is negative
        s16 x_adjust_int = x_adjust_10>>(6+TRANS_Z_FRAC_BITS);
        rx2 += x_adjust_int;
        rz1_12_4 = NEAR_Z_FIX;
        trans_v2->x = rx2;
        trans_v2->y = rz1_12_4;

        if(tmap->needs_texture) {
            s32 u_adjust_20 = du_over_dz_16 * z_adjust_12_4;
            tmap->left_u = base_left_u_16;
            tmap->right_u = base_right_u_16 + (u_adjust_20>>4);
        }
        clip_status |= RIGHT_Z_CLIPPED;
        clip_status &= (~RIGHT_FRUSTUM_CLIPPED);
    }
    return clip_status;
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