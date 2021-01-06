#include "draw.h"
#include "game.h"
#include "level.h"
#include "math3d.h"
#include "portal.h"


//u32 column_buffer[128] = { 0 };

void bresenham_line(u8 x0, u8 x1, u8 y0, u8 y1, u16 win_min, u16 win_max, u8 col) {
    //col = col<<4 |

    
    int dx =  abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs (y1 - y0), sy = y0 < y1 ? 1 : -1; 
    int err = dx + dy, e2;  // error value e_xy 
    
    while(x0 < win_min) {
        if (x0 == x1 && y0 == y1) { break; }
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; } // e_xy+e_x > 0 
        if (e2 <= dx) { err += dx; y0 += sy; } // e_xy+e_y < 0 
    }

    for (;;){ 
        //if(x0 >= win_min) {
        //if(column_buffer[x0] != cur_frame) {
            BMP_setPixel(x0<<1, y0, col);
            //BMP_setPixel((x0<<1)+1, y0, col);
            //column_buffer[x0] = cur_frame;
        //}
        //}
        if(x0 >= win_max) { break; }
        if (x0 == x1 && y0 == y1) { break; }
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx;  } // e_xy+e_x > 0 
        if (e2 <= dx) { err += dx; y0 += sy; } // e_xy+e_y < 0 
        
    }
    
}

/*
typedef struct {
    u8 has_two_ranges:1;
    u8 rng_0_min; u8 rng_0_max;
    u8 rng_1_min; u8 rng_1_max;
} backfacing_entry;

backfacing_entry wall_backfacing_table[32];

int is_backfacing(u16 wall_id, fix32 cur_player_angle) {
    u8 ply_ang = quantize_angle(cur_player_angle);
    backfacing_entry wall_entry = wall_backfacing_table[wall_id];
    if(ply_ang >= wall_entry.rng_0_min && ply_ang <= wall_entry.rng_0_max) {
        return 0;
    }

    if(wall_entry.has_two_ranges && ply_ang >= wall_entry.rng_1_min && ply_ang <= wall_entry.rng_1_max) {
        return 0;
    }
    return 1;
}
*/

// TODO: optimize backface culling!
// 
vis_query_result is_visible(volatile Vect2D_s32 trans_v1, volatile Vect2D_s32 trans_v2, u16 window_min, u16 window_max) {
    vis_query_result res; 
    res.visible = 0;

    // with optimized backface culling
    //if(is_backfacing(wall.backface_angle)

    //if(trans_v1.x <= trans_v2.x) { return res;}

    clip_result clipped = clip_map_vertex(&trans_v1, &trans_v2);    
    
    if(clipped == OFFSCREEN) {
        return res;
    }

    s16 x2 = project_and_adjust_2d(trans_v1);
    if(x2 <= window_min) { return res; }
    s16 x1 = project_and_adjust_2d(trans_v2);
    if(x1 >= window_max) { return res; }

    if(x1 >= x2) {
        return res;
    }
    res.visible = 1;
    res.x1 = max(x1, window_min);
    res.x2 = min(x2, window_max);
    return res;

}



int draw_wall_from_verts(volatile Vect2D_s32 trans_v1, volatile Vect2D_s32 trans_v2, s16 ceil_height, s16 floor_height, u16 window_min, u16 window_max) {
    

    //if(trans_v1.x <= trans_v2.x) { return; }

    // clip against near z plane if necessary
    clip_result clipped = clip_map_vertex(&trans_v1, &trans_v2);

    if(clipped == OFFSCREEN) {
        return 0;
    }


    // project map vertex with height attributes
    transformed_vert screen_v2 = project_and_adjust_3d(trans_v1, floor_height, ceil_height);
    transformed_vert screen_v1 = project_and_adjust_3d(trans_v2, floor_height, ceil_height);

    u8 col = clipped != UNCLIPPED ? 0x11 : 0x22;


    //if(screen_v1.x >= screen_v2.x) {
    //    return 0;
    //}
    
    Line l3d_ceil = {.pt1 = {.x = screen_v1.x, .y = screen_v1.yceil}, 
                     .pt2 = {.x = screen_v2.x, .y = screen_v2.yceil}, .col = col};
    Line l3d_floor = {.pt1 = {.x = screen_v1.x, .y = screen_v1.yfloor}, 
                      .pt2 = {.x = screen_v2.x, .y = screen_v2.yfloor}, .col = col};

    Line l3d_left = {.pt1 = {.x = screen_v1.x, .y = screen_v1.yceil}, .pt2 = {.x = screen_v1.x, .y = screen_v1.yfloor}, .col = col};
    Line l3d_right = {.pt1 = {.x = screen_v2.x, .y = screen_v2.yceil}, .pt2 = {.x = screen_v2.x, .y = screen_v2.yfloor}, .col = col};
        
    /*
    if(render_mode == GAME_SOLID) {    
        Vect2D_s16 pts[4] = {
            {screen_v1.x, screen_v1.yceil},
            {screen_v2.x, screen_v2.yceil},
            {screen_v2.x, screen_v2.yfloor},
            {screen_v1.x, screen_v1.yfloor}
        };

        BMP_drawPolygon(pts, 4, col);

    } else {
    */
    if(BMP_clipLine(&l3d_ceil)) {
        bresenham_line(l3d_ceil.pt1.x, l3d_ceil.pt2.x, l3d_ceil.pt1.y, l3d_ceil.pt2.y, window_min, window_max, col);
    }
    if(BMP_clipLine(&l3d_floor)) {
        bresenham_line(l3d_floor.pt1.x, l3d_floor.pt2.x, l3d_floor.pt1.y, l3d_floor.pt2.y, window_min, window_max, col);
    }
    
    if(BMP_clipLine(&l3d_left)) {
        bresenham_line(l3d_left.pt1.x, l3d_left.pt2.x, l3d_left.pt1.y, l3d_left.pt2.y, window_min, window_max, col);
    }
    if(BMP_clipLine(&l3d_right)) {
        bresenham_line(l3d_right.pt1.x, l3d_right.pt2.x, l3d_right.pt1.y, l3d_right.pt2.y, window_min, window_max, col);
    }
        
    //}
    
    return 1;


}
