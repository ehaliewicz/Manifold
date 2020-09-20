#include <genesis.h>
#include "level.h"


#include "math3d.h"

#include "span_buf.h"

void draw_face(Vect2D_s32 trans_v1, Vect2D_s32 trans_v2) {
    clip_result clip_res = clip_map_vertex(&trans_v1, &trans_v2);
    if(clip_res == OFFSCREEN) {
        return;
    }

    u8 col = (clip_res == UNCLIPPED) ? 0xFF : 0x11;

    transformed_vert screen_v1 = project_and_adjust_3d(trans_v1, -100, 100);
    transformed_vert screen_v2 = project_and_adjust_3d(trans_v2, -100, 100);

    Line l;
    l.col = col;
    l.pt1 = (Vect2D_s16){.x = screen_v1.x, .y = screen_v1.yceil};
    l.pt2 = (Vect2D_s16){.x = screen_v2.x, .y = screen_v2.yceil};
    if(BMP_clipLine(&l)) { BMP_drawLine(&l); }
    l.pt1 = (Vect2D_s16){.x = screen_v2.x, .y = screen_v2.yceil};
    l.pt2 = (Vect2D_s16){.x = screen_v2.x, .y = screen_v2.yfloor};
    if(BMP_clipLine(&l)) { BMP_drawLine(&l); }
    l.pt1 = (Vect2D_s16){.x = screen_v2.x, .y = screen_v2.yfloor};
    l.pt2 = (Vect2D_s16){.x = screen_v1.x, .y = screen_v1.yfloor};
    if(BMP_clipLine(&l)) { BMP_drawLine(&l); }
    l.pt1 = (Vect2D_s16){.x = screen_v1.x, .y = screen_v1.yfloor};
    l.pt2 = (Vect2D_s16){.x = screen_v1.x, .y = screen_v1.yceil};
    if(BMP_clipLine(&l)) { BMP_drawLine(&l); }

}

void draw_bbox(s16 left_x, s16 right_x, s16 top_y, s16 bottom_y) {
    
    // TODO: factor out shared calculations
    Vect2D_s32 tl = transform_map_vert(left_x,  top_y);
    Vect2D_s32 tr = transform_map_vert(right_x, top_y);
    Vect2D_s32 bl = transform_map_vert(left_x,  bottom_y);
    Vect2D_s32 br = transform_map_vert(right_x, bottom_y);

    draw_face(tl, tr);
    draw_face(tr, br);
    draw_face(br, bl);
    draw_face(bl, tl);

}

void traverse_bsp_nodes_front_to_back(s32 x, s32 y) {
    node* root_node = &(cur_level->nodes[cur_level->num_nodes-1]);

    s16 left_x = root_node->left_box_left;
    s16 right_x = root_node->left_box_right;
    s16 top_y = root_node->left_box_top;
    s16 bottom_y = root_node->left_box_bottom;
    draw_bbox(left_x, right_x, top_y, bottom_y);
    

    
}