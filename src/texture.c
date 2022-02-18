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

#define Z_PERSP_MIN (100<<4)

persp_params calc_perspective(u16 z1_12_4, u16 z2_12_4, u32 left_u_16, u32 right_u_16, u16 dx) {
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

    u32 u_over_z_23 = (left_u_16<<(TRANS_Z_FRAC_BITS+7))/z1_12_4;  
    u32 u_over_z_end_23 = (right_u_16<<(TRANS_Z_FRAC_BITS+7))/z2_12_4;


    s32 d_u_over_z_23 = (u_over_z_end_23 - u_over_z_23);


    ret.one_over_z_26 = one_over_z_26;

    ret.d_one_over_z_dx_26 = (d_one_over_z_26/dx);
    ret.u_over_z_23 = u_over_z_23;
    ret.d_u_over_z_dx_23 = d_u_over_z_23/dx;
    return ret;
}


u16 unclipped_dy_fix10_recip_table[512] = {
    0xF00F,
    (TEX_HEIGHT<<10)/1, (TEX_HEIGHT<<10)/2, (TEX_HEIGHT<<10)/3, (TEX_HEIGHT<<10)/4, (TEX_HEIGHT<<10)/5, 
(TEX_HEIGHT<<10)/6, (TEX_HEIGHT<<10)/7, (TEX_HEIGHT<<10)/8, (TEX_HEIGHT<<10)/9, (TEX_HEIGHT<<10)/10, 
(TEX_HEIGHT<<10)/11, (TEX_HEIGHT<<10)/12, (TEX_HEIGHT<<10)/13, (TEX_HEIGHT<<10)/14, (TEX_HEIGHT<<10)/15, 
(TEX_HEIGHT<<10)/16, (TEX_HEIGHT<<10)/17, (TEX_HEIGHT<<10)/18, (TEX_HEIGHT<<10)/19, (TEX_HEIGHT<<10)/20, 
(TEX_HEIGHT<<10)/21, (TEX_HEIGHT<<10)/22, (TEX_HEIGHT<<10)/23, (TEX_HEIGHT<<10)/24, (TEX_HEIGHT<<10)/25, 
(TEX_HEIGHT<<10)/26, (TEX_HEIGHT<<10)/27, (TEX_HEIGHT<<10)/28, (TEX_HEIGHT<<10)/29, (TEX_HEIGHT<<10)/30, 
(TEX_HEIGHT<<10)/31, (TEX_HEIGHT<<10)/32, (TEX_HEIGHT<<10)/33, (TEX_HEIGHT<<10)/34, (TEX_HEIGHT<<10)/35, 
(TEX_HEIGHT<<10)/36, (TEX_HEIGHT<<10)/37, (TEX_HEIGHT<<10)/38, (TEX_HEIGHT<<10)/39, (TEX_HEIGHT<<10)/40, 
(TEX_HEIGHT<<10)/41, (TEX_HEIGHT<<10)/42, (TEX_HEIGHT<<10)/43, (TEX_HEIGHT<<10)/44, (TEX_HEIGHT<<10)/45, 
(TEX_HEIGHT<<10)/46, (TEX_HEIGHT<<10)/47, (TEX_HEIGHT<<10)/48, (TEX_HEIGHT<<10)/49, (TEX_HEIGHT<<10)/50, 
(TEX_HEIGHT<<10)/51, (TEX_HEIGHT<<10)/52, (TEX_HEIGHT<<10)/53, (TEX_HEIGHT<<10)/54, (TEX_HEIGHT<<10)/55, 
(TEX_HEIGHT<<10)/56, (TEX_HEIGHT<<10)/57, (TEX_HEIGHT<<10)/58, (TEX_HEIGHT<<10)/59, (TEX_HEIGHT<<10)/60, 
(TEX_HEIGHT<<10)/61, (TEX_HEIGHT<<10)/62, (TEX_HEIGHT<<10)/63, (TEX_HEIGHT<<10)/64, (TEX_HEIGHT<<10)/65, 
(TEX_HEIGHT<<10)/66, (TEX_HEIGHT<<10)/67, (TEX_HEIGHT<<10)/68, (TEX_HEIGHT<<10)/69, (TEX_HEIGHT<<10)/70, 
(TEX_HEIGHT<<10)/71, (TEX_HEIGHT<<10)/72, (TEX_HEIGHT<<10)/73, (TEX_HEIGHT<<10)/74, (TEX_HEIGHT<<10)/75, 
(TEX_HEIGHT<<10)/76, (TEX_HEIGHT<<10)/77, (TEX_HEIGHT<<10)/78, (TEX_HEIGHT<<10)/79, (TEX_HEIGHT<<10)/80, 
(TEX_HEIGHT<<10)/81, (TEX_HEIGHT<<10)/82, (TEX_HEIGHT<<10)/83, (TEX_HEIGHT<<10)/84, (TEX_HEIGHT<<10)/85, 
(TEX_HEIGHT<<10)/86, (TEX_HEIGHT<<10)/87, (TEX_HEIGHT<<10)/88, (TEX_HEIGHT<<10)/89, (TEX_HEIGHT<<10)/90, 
(TEX_HEIGHT<<10)/91, (TEX_HEIGHT<<10)/92, (TEX_HEIGHT<<10)/93, (TEX_HEIGHT<<10)/94, (TEX_HEIGHT<<10)/95, 
(TEX_HEIGHT<<10)/96, (TEX_HEIGHT<<10)/97, (TEX_HEIGHT<<10)/98, (TEX_HEIGHT<<10)/99, (TEX_HEIGHT<<10)/100, 
(TEX_HEIGHT<<10)/101, (TEX_HEIGHT<<10)/102, (TEX_HEIGHT<<10)/103, (TEX_HEIGHT<<10)/104, (TEX_HEIGHT<<10)/105, 
(TEX_HEIGHT<<10)/106, (TEX_HEIGHT<<10)/107, (TEX_HEIGHT<<10)/108, (TEX_HEIGHT<<10)/109, (TEX_HEIGHT<<10)/110, 
(TEX_HEIGHT<<10)/111, (TEX_HEIGHT<<10)/112, (TEX_HEIGHT<<10)/113, (TEX_HEIGHT<<10)/114, (TEX_HEIGHT<<10)/115, 
(TEX_HEIGHT<<10)/116, (TEX_HEIGHT<<10)/117, (TEX_HEIGHT<<10)/118, (TEX_HEIGHT<<10)/119, (TEX_HEIGHT<<10)/120, 
(TEX_HEIGHT<<10)/121, (TEX_HEIGHT<<10)/122, (TEX_HEIGHT<<10)/123, (TEX_HEIGHT<<10)/124, (TEX_HEIGHT<<10)/125, 
(TEX_HEIGHT<<10)/126, (TEX_HEIGHT<<10)/127, (TEX_HEIGHT<<10)/128, (TEX_HEIGHT<<10)/129, (TEX_HEIGHT<<10)/130, 
(TEX_HEIGHT<<10)/131, (TEX_HEIGHT<<10)/132, (TEX_HEIGHT<<10)/133, (TEX_HEIGHT<<10)/134, (TEX_HEIGHT<<10)/135, 
(TEX_HEIGHT<<10)/136, (TEX_HEIGHT<<10)/137, (TEX_HEIGHT<<10)/138, (TEX_HEIGHT<<10)/139, (TEX_HEIGHT<<10)/140, 
(TEX_HEIGHT<<10)/141, (TEX_HEIGHT<<10)/142, (TEX_HEIGHT<<10)/143, (TEX_HEIGHT<<10)/144, (TEX_HEIGHT<<10)/145, 
(TEX_HEIGHT<<10)/146, (TEX_HEIGHT<<10)/147, (TEX_HEIGHT<<10)/148, (TEX_HEIGHT<<10)/149, (TEX_HEIGHT<<10)/150, 
(TEX_HEIGHT<<10)/151, (TEX_HEIGHT<<10)/152, (TEX_HEIGHT<<10)/153, (TEX_HEIGHT<<10)/154, (TEX_HEIGHT<<10)/155, 
(TEX_HEIGHT<<10)/156, (TEX_HEIGHT<<10)/157, (TEX_HEIGHT<<10)/158, (TEX_HEIGHT<<10)/159, (TEX_HEIGHT<<10)/160, 
(TEX_HEIGHT<<10)/161, (TEX_HEIGHT<<10)/162, (TEX_HEIGHT<<10)/163, (TEX_HEIGHT<<10)/164, (TEX_HEIGHT<<10)/165, 
(TEX_HEIGHT<<10)/166, (TEX_HEIGHT<<10)/167, (TEX_HEIGHT<<10)/168, (TEX_HEIGHT<<10)/169, (TEX_HEIGHT<<10)/170, 
(TEX_HEIGHT<<10)/171, (TEX_HEIGHT<<10)/172, (TEX_HEIGHT<<10)/173, (TEX_HEIGHT<<10)/174, (TEX_HEIGHT<<10)/175, 
(TEX_HEIGHT<<10)/176, (TEX_HEIGHT<<10)/177, (TEX_HEIGHT<<10)/178, (TEX_HEIGHT<<10)/179, (TEX_HEIGHT<<10)/180, 
(TEX_HEIGHT<<10)/181, (TEX_HEIGHT<<10)/182, (TEX_HEIGHT<<10)/183, (TEX_HEIGHT<<10)/184, (TEX_HEIGHT<<10)/185, 
(TEX_HEIGHT<<10)/186, (TEX_HEIGHT<<10)/187, (TEX_HEIGHT<<10)/188, (TEX_HEIGHT<<10)/189, (TEX_HEIGHT<<10)/190, 
(TEX_HEIGHT<<10)/191, (TEX_HEIGHT<<10)/192, (TEX_HEIGHT<<10)/193, (TEX_HEIGHT<<10)/194, (TEX_HEIGHT<<10)/195, 
(TEX_HEIGHT<<10)/196, (TEX_HEIGHT<<10)/197, (TEX_HEIGHT<<10)/198, (TEX_HEIGHT<<10)/199, (TEX_HEIGHT<<10)/200, 
(TEX_HEIGHT<<10)/201, (TEX_HEIGHT<<10)/202, (TEX_HEIGHT<<10)/203, (TEX_HEIGHT<<10)/204, (TEX_HEIGHT<<10)/205, 
(TEX_HEIGHT<<10)/206, (TEX_HEIGHT<<10)/207, (TEX_HEIGHT<<10)/208, (TEX_HEIGHT<<10)/209, (TEX_HEIGHT<<10)/210, 
(TEX_HEIGHT<<10)/211, (TEX_HEIGHT<<10)/212, (TEX_HEIGHT<<10)/213, (TEX_HEIGHT<<10)/214, (TEX_HEIGHT<<10)/215, 
(TEX_HEIGHT<<10)/216, (TEX_HEIGHT<<10)/217, (TEX_HEIGHT<<10)/218, (TEX_HEIGHT<<10)/219, (TEX_HEIGHT<<10)/220, 
(TEX_HEIGHT<<10)/221, (TEX_HEIGHT<<10)/222, (TEX_HEIGHT<<10)/223, (TEX_HEIGHT<<10)/224, (TEX_HEIGHT<<10)/225, 
(TEX_HEIGHT<<10)/226, (TEX_HEIGHT<<10)/227, (TEX_HEIGHT<<10)/228, (TEX_HEIGHT<<10)/229, (TEX_HEIGHT<<10)/230, 
(TEX_HEIGHT<<10)/231, (TEX_HEIGHT<<10)/232, (TEX_HEIGHT<<10)/233, (TEX_HEIGHT<<10)/234, (TEX_HEIGHT<<10)/235, 
(TEX_HEIGHT<<10)/236, (TEX_HEIGHT<<10)/237, (TEX_HEIGHT<<10)/238, (TEX_HEIGHT<<10)/239, (TEX_HEIGHT<<10)/240, 
(TEX_HEIGHT<<10)/241, (TEX_HEIGHT<<10)/242, (TEX_HEIGHT<<10)/243, (TEX_HEIGHT<<10)/244, (TEX_HEIGHT<<10)/245, 
(TEX_HEIGHT<<10)/246, (TEX_HEIGHT<<10)/247, (TEX_HEIGHT<<10)/248, (TEX_HEIGHT<<10)/249, (TEX_HEIGHT<<10)/250, 
(TEX_HEIGHT<<10)/251, (TEX_HEIGHT<<10)/252, (TEX_HEIGHT<<10)/253, (TEX_HEIGHT<<10)/254, (TEX_HEIGHT<<10)/255, 
(TEX_HEIGHT<<10)/256, (TEX_HEIGHT<<10)/257, (TEX_HEIGHT<<10)/258, (TEX_HEIGHT<<10)/259, (TEX_HEIGHT<<10)/260, 
(TEX_HEIGHT<<10)/261, (TEX_HEIGHT<<10)/262, (TEX_HEIGHT<<10)/263, (TEX_HEIGHT<<10)/264, (TEX_HEIGHT<<10)/265, 
(TEX_HEIGHT<<10)/266, (TEX_HEIGHT<<10)/267, (TEX_HEIGHT<<10)/268, (TEX_HEIGHT<<10)/269, (TEX_HEIGHT<<10)/270, 
(TEX_HEIGHT<<10)/271, (TEX_HEIGHT<<10)/272, (TEX_HEIGHT<<10)/273, (TEX_HEIGHT<<10)/274, (TEX_HEIGHT<<10)/275, 
(TEX_HEIGHT<<10)/276, (TEX_HEIGHT<<10)/277, (TEX_HEIGHT<<10)/278, (TEX_HEIGHT<<10)/279, (TEX_HEIGHT<<10)/280, 
(TEX_HEIGHT<<10)/281, (TEX_HEIGHT<<10)/282, (TEX_HEIGHT<<10)/283, (TEX_HEIGHT<<10)/284, (TEX_HEIGHT<<10)/285, 
(TEX_HEIGHT<<10)/286, (TEX_HEIGHT<<10)/287, (TEX_HEIGHT<<10)/288, (TEX_HEIGHT<<10)/289, (TEX_HEIGHT<<10)/290, 
(TEX_HEIGHT<<10)/291, (TEX_HEIGHT<<10)/292, (TEX_HEIGHT<<10)/293, (TEX_HEIGHT<<10)/294, (TEX_HEIGHT<<10)/295, 
(TEX_HEIGHT<<10)/296, (TEX_HEIGHT<<10)/297, (TEX_HEIGHT<<10)/298, (TEX_HEIGHT<<10)/299, (TEX_HEIGHT<<10)/300, 
(TEX_HEIGHT<<10)/301, (TEX_HEIGHT<<10)/302, (TEX_HEIGHT<<10)/303, (TEX_HEIGHT<<10)/304, (TEX_HEIGHT<<10)/305, 
(TEX_HEIGHT<<10)/306, (TEX_HEIGHT<<10)/307, (TEX_HEIGHT<<10)/308, (TEX_HEIGHT<<10)/309, (TEX_HEIGHT<<10)/310, 
(TEX_HEIGHT<<10)/311, (TEX_HEIGHT<<10)/312, (TEX_HEIGHT<<10)/313, (TEX_HEIGHT<<10)/314, (TEX_HEIGHT<<10)/315, 
(TEX_HEIGHT<<10)/316, (TEX_HEIGHT<<10)/317, (TEX_HEIGHT<<10)/318, (TEX_HEIGHT<<10)/319, (TEX_HEIGHT<<10)/320, 
(TEX_HEIGHT<<10)/321, (TEX_HEIGHT<<10)/322, (TEX_HEIGHT<<10)/323, (TEX_HEIGHT<<10)/324, (TEX_HEIGHT<<10)/325, 
(TEX_HEIGHT<<10)/326, (TEX_HEIGHT<<10)/327, (TEX_HEIGHT<<10)/328, (TEX_HEIGHT<<10)/329, (TEX_HEIGHT<<10)/330, 
(TEX_HEIGHT<<10)/331, (TEX_HEIGHT<<10)/332, (TEX_HEIGHT<<10)/333, (TEX_HEIGHT<<10)/334, (TEX_HEIGHT<<10)/335, 
(TEX_HEIGHT<<10)/336, (TEX_HEIGHT<<10)/337, (TEX_HEIGHT<<10)/338, (TEX_HEIGHT<<10)/339, (TEX_HEIGHT<<10)/340, 
(TEX_HEIGHT<<10)/341, (TEX_HEIGHT<<10)/342, (TEX_HEIGHT<<10)/343, (TEX_HEIGHT<<10)/344, (TEX_HEIGHT<<10)/345, 
(TEX_HEIGHT<<10)/346, (TEX_HEIGHT<<10)/347, (TEX_HEIGHT<<10)/348, (TEX_HEIGHT<<10)/349, (TEX_HEIGHT<<10)/350, 
(TEX_HEIGHT<<10)/351, (TEX_HEIGHT<<10)/352, (TEX_HEIGHT<<10)/353, (TEX_HEIGHT<<10)/354, (TEX_HEIGHT<<10)/355, 
(TEX_HEIGHT<<10)/356, (TEX_HEIGHT<<10)/357, (TEX_HEIGHT<<10)/358, (TEX_HEIGHT<<10)/359, (TEX_HEIGHT<<10)/360, 
(TEX_HEIGHT<<10)/361, (TEX_HEIGHT<<10)/362, (TEX_HEIGHT<<10)/363, (TEX_HEIGHT<<10)/364, (TEX_HEIGHT<<10)/365, 
(TEX_HEIGHT<<10)/366, (TEX_HEIGHT<<10)/367, (TEX_HEIGHT<<10)/368, (TEX_HEIGHT<<10)/369, (TEX_HEIGHT<<10)/370, 
(TEX_HEIGHT<<10)/371, (TEX_HEIGHT<<10)/372, (TEX_HEIGHT<<10)/373, (TEX_HEIGHT<<10)/374, (TEX_HEIGHT<<10)/375, 
(TEX_HEIGHT<<10)/376, (TEX_HEIGHT<<10)/377, (TEX_HEIGHT<<10)/378, (TEX_HEIGHT<<10)/379, (TEX_HEIGHT<<10)/380, 
(TEX_HEIGHT<<10)/381, (TEX_HEIGHT<<10)/382, (TEX_HEIGHT<<10)/383, (TEX_HEIGHT<<10)/384, (TEX_HEIGHT<<10)/385, 
(TEX_HEIGHT<<10)/386, (TEX_HEIGHT<<10)/387, (TEX_HEIGHT<<10)/388, (TEX_HEIGHT<<10)/389, (TEX_HEIGHT<<10)/390, 
(TEX_HEIGHT<<10)/391, (TEX_HEIGHT<<10)/392, (TEX_HEIGHT<<10)/393, (TEX_HEIGHT<<10)/394, (TEX_HEIGHT<<10)/395, 
(TEX_HEIGHT<<10)/396, (TEX_HEIGHT<<10)/397, (TEX_HEIGHT<<10)/398, (TEX_HEIGHT<<10)/399, (TEX_HEIGHT<<10)/400, 
(TEX_HEIGHT<<10)/401, (TEX_HEIGHT<<10)/402, (TEX_HEIGHT<<10)/403, (TEX_HEIGHT<<10)/404, (TEX_HEIGHT<<10)/405, 
(TEX_HEIGHT<<10)/406, (TEX_HEIGHT<<10)/407, (TEX_HEIGHT<<10)/408, (TEX_HEIGHT<<10)/409, (TEX_HEIGHT<<10)/410, 
(TEX_HEIGHT<<10)/411, (TEX_HEIGHT<<10)/412, (TEX_HEIGHT<<10)/413, (TEX_HEIGHT<<10)/414, (TEX_HEIGHT<<10)/415, 
(TEX_HEIGHT<<10)/416, (TEX_HEIGHT<<10)/417, (TEX_HEIGHT<<10)/418, (TEX_HEIGHT<<10)/419, (TEX_HEIGHT<<10)/420, 
(TEX_HEIGHT<<10)/421, (TEX_HEIGHT<<10)/422, (TEX_HEIGHT<<10)/423, (TEX_HEIGHT<<10)/424, (TEX_HEIGHT<<10)/425, 
(TEX_HEIGHT<<10)/426, (TEX_HEIGHT<<10)/427, (TEX_HEIGHT<<10)/428, (TEX_HEIGHT<<10)/429, (TEX_HEIGHT<<10)/430, 
(TEX_HEIGHT<<10)/431, (TEX_HEIGHT<<10)/432, (TEX_HEIGHT<<10)/433, (TEX_HEIGHT<<10)/434, (TEX_HEIGHT<<10)/435, 
(TEX_HEIGHT<<10)/436, (TEX_HEIGHT<<10)/437, (TEX_HEIGHT<<10)/438, (TEX_HEIGHT<<10)/439, (TEX_HEIGHT<<10)/440, 
(TEX_HEIGHT<<10)/441, (TEX_HEIGHT<<10)/442, (TEX_HEIGHT<<10)/443, (TEX_HEIGHT<<10)/444, (TEX_HEIGHT<<10)/445, 
(TEX_HEIGHT<<10)/446, (TEX_HEIGHT<<10)/447, (TEX_HEIGHT<<10)/448, (TEX_HEIGHT<<10)/449, (TEX_HEIGHT<<10)/450, 
(TEX_HEIGHT<<10)/451, (TEX_HEIGHT<<10)/452, (TEX_HEIGHT<<10)/453, (TEX_HEIGHT<<10)/454, (TEX_HEIGHT<<10)/455, 
(TEX_HEIGHT<<10)/456, (TEX_HEIGHT<<10)/457, (TEX_HEIGHT<<10)/458, (TEX_HEIGHT<<10)/459, (TEX_HEIGHT<<10)/460, 
(TEX_HEIGHT<<10)/461, (TEX_HEIGHT<<10)/462, (TEX_HEIGHT<<10)/463, (TEX_HEIGHT<<10)/464, (TEX_HEIGHT<<10)/465, 
(TEX_HEIGHT<<10)/466, (TEX_HEIGHT<<10)/467, (TEX_HEIGHT<<10)/468, (TEX_HEIGHT<<10)/469, (TEX_HEIGHT<<10)/470, 
(TEX_HEIGHT<<10)/471, (TEX_HEIGHT<<10)/472, (TEX_HEIGHT<<10)/473, (TEX_HEIGHT<<10)/474, (TEX_HEIGHT<<10)/475, 
(TEX_HEIGHT<<10)/476, (TEX_HEIGHT<<10)/477, (TEX_HEIGHT<<10)/478, (TEX_HEIGHT<<10)/479, (TEX_HEIGHT<<10)/480, 
(TEX_HEIGHT<<10)/481, (TEX_HEIGHT<<10)/482, (TEX_HEIGHT<<10)/483, (TEX_HEIGHT<<10)/484, (TEX_HEIGHT<<10)/485, 
(TEX_HEIGHT<<10)/486, (TEX_HEIGHT<<10)/487, (TEX_HEIGHT<<10)/488, (TEX_HEIGHT<<10)/489, (TEX_HEIGHT<<10)/490, 
(TEX_HEIGHT<<10)/491, (TEX_HEIGHT<<10)/492, (TEX_HEIGHT<<10)/493, (TEX_HEIGHT<<10)/494, (TEX_HEIGHT<<10)/495, 
(TEX_HEIGHT<<10)/496, (TEX_HEIGHT<<10)/497, (TEX_HEIGHT<<10)/498, (TEX_HEIGHT<<10)/499, (TEX_HEIGHT<<10)/500, 
(TEX_HEIGHT<<10)/501, (TEX_HEIGHT<<10)/502, (TEX_HEIGHT<<10)/503, (TEX_HEIGHT<<10)/504, (TEX_HEIGHT<<10)/505, 
(TEX_HEIGHT<<10)/506, (TEX_HEIGHT<<10)/507, (TEX_HEIGHT<<10)/508, (TEX_HEIGHT<<10)/509, (TEX_HEIGHT<<10)/510, 
(TEX_HEIGHT<<10)/511,
};

u8* draw_texture_vertical_line(s16 unclipped_y0, u16 y0, s16 unclipped_y1, u8* col_ptr, u16* tex_column) {
    //return ;

    //u16 unclipped_dy = unclipped_y1 - unclipped_y0;
    u16 unclipped_dy = unclipped_y1;
    __asm volatile(             
        "sub.w %1, %0"          
        : "+d" (unclipped_dy)              
        : "d" (unclipped_y0)               
    );      
    if(unclipped_dy > 512) { return col_ptr; }

    u16 y0_x2 = y0+y0;
    col_ptr = col_ptr + y0_x2;

    // using ROM jump tables
    void* base_call_loc = jump_table_lut[unclipped_dy];

    u8 clip_top = y0-unclipped_y0;

    
    register const a0 asm ("%a0") = ((u32)tex_column); // - (clip_bot<<1 * du_dy);
    register const  a1 asm ("%a1") = (u32)col_ptr;

    register const  a2 asm ("%a2") = base_call_loc + (clip_top<<2);

    
    __asm volatile(
        "jsr (%0)"
        :
        : "a" (a2), "a" (a0), "a" (a1)
    );


    return col_ptr;
    
}


u8* draw_bottom_clipped_texture_vertical_line(s16 unclipped_y0, u16 y0, s16 unclipped_y1, u16 y1, u8* col_ptr, u16* tex_column) {
    //return ;

    //u16 unclipped_dy = unclipped_y1 - unclipped_y0;    
    u16 unclipped_dy = unclipped_y1;
    __asm volatile(             
        "sub.w %1, %0"          
        : "+d" (unclipped_dy)              
        : "d" (unclipped_y0)               
    );      
    if(unclipped_dy > 512) { return col_ptr; }

    u16 y0_x2 = y0+y0;
    col_ptr = col_ptr + y0_x2;


    // using ROM jump tables
    void* base_call_loc = jump_table_lut[unclipped_dy];

    u8 clip_top = y0-unclipped_y0;
    u8 clip_bot = unclipped_y1-y1;


    //u32 du_dy = unclipped_dy__recip_table[unclipped_dy-1]; // 16.16
    //u16 du_dy_16 = du_dy>>6; // 6.10
    //u32 clip_bot_du_fix = du_dy * clip_bot;
    //u32 clip_bot_du_fix = du_dy_16 * clip_bot;

    u16 du_dy_fix10 = unclipped_dy_fix10_recip_table[unclipped_dy];
    //u32 clip_bot_du_fix = mulu_16_by_16(du_dy_fix10, clip_bot);
    u32 clip_bot_du_fix = du_dy_fix10;
    __asm volatile(
        "mulu.w %1, %0"
        : "+d" (clip_bot_du_fix) // output
        : "d" (clip_bot)
    );

    //u16 clip_bot_du = clip_bot_du_fix>>16;
    u16 clip_bot_du = clip_bot_du_fix >> 10;

    tex_column -= (clip_bot_du);
    
    register const a0 asm ("%a0") = ((u32)tex_column); // - (clip_bot<<1 * du_dy);
    register const  a1 asm ("%a1") = (u32)col_ptr;

    register const  a2 asm ("%a2") = base_call_loc + (clip_top<<2) + (clip_bot << 2);

    
    __asm volatile(
        "jsr (%0)"
        :
        : "a" (a2), "a" (a0), "a" (a1)
    );


    return col_ptr;
    
}
    


// column 0            column 1            column 2                column 3
// blue blue blue blue blue blue blue blue green green green green green green green green