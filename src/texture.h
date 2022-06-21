#ifndef TEXTURE_H
#define TEXTURE_H

#include <genesis.h>

//#define TEX_HEIGHT 128
//#define TEX_HEIGHT_SHIFT 7

//#define TEX_WIDTH 64
//#define TEX_WIDTH_SHIFT 6
#define TEX_WIDTH 32
#define TEX_WIDTH_SHIFT 5

#define TEX_HEIGHT 64
#define TEX_HEIGHT_SHIFT 6
//#define TEX_WIDTH 32
//#define TEX_WIDTH_SHIFT 5

extern u16* dark_texture;
extern u16* mid_texture;
extern u16* light_texture;
void tick_texture();


typedef struct {
    const uint16_t* const dark;
    const uint16_t* const mid;
    const uint16_t* const light;
} lit_texture;

typedef struct {
    u32 left_u;    // 16.16
    s16 left_z;
    u32 right_u;   // 16.16
    s16 right_z;
    s32 du_over_dz;
    u16 repetitions;
    lit_texture* tex;
} texmap_params;

typedef struct {
    u32 one_over_z_26;
    s32 d_one_over_z_dx_26;

    u32 u_over_z_23;
    u32 d_u_over_z_dx_23;
} persp_params;

persp_params calc_perspective(u16 z1_12_4, u16 z2_12_4, u32 left_u_16, u32 right_u_16, u16 dx);

#define FAR_MIP_WIDTH 16
#define FAR_MIP_WIDTH_SHIFT 4
#define FAR_MIP_HEIGHT 64

#define MID_MIP_WIDTH 32
#define MID_MIP_WIDTH_SHIFT 5
#define MID_MIP_HEIGHT 64

#define NEAR_MIP_WIDTH 64
#define NEAR_MIP_WIDTH_SHIFT 6
#define NEAR_MIP_HEIGHT 64

// three mipmap levels


//typedef struct {
//    lit_texture mip_far;
//    lit_texture mip_mid;
//    lit_texture mip_near;
//} texture_set;


u8* draw_texture_vertical_line(s16 unclipped_y0, u16 y0, s16 unclipped_y1, u8* col_ptr, const u16* tex_column);

u8* draw_bottom_clipped_texture_vertical_line(s16 unclipped_y0, u16 y0, s16 unclipped_y1, u16 y1, u8* col_ptr, const u16* tex_column);

#endif