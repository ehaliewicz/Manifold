#include <genesis.h>

#include "draw_queues.h"
#include "level.h"


#include "math3d.h"

#include "span_buf.h"

// returns 1 if on-screen
int face_on_screen(Vect2D_s32 trans_v1, Vect2D_s32 trans_v2) {
    clip_result clip_res = clip_map_vertex(&trans_v1, &trans_v2);
    if(clip_res == OFFSCREEN) {
        return 0;
    }

    s16 screen_x1 = project_and_adjust_2d(trans_v1);
    s16 screen_x2 = project_and_adjust_2d(trans_v2);
    Line l; 
    l.pt1.x = screen_x1;
    l.pt1.y = 0; 
    l.pt2.x = screen_x2;
    l.pt2.y = 0;
    return BMP_clipLine(&l);
}

int bbox_on_screen(s16 left_x, s16 right_x, s16 top_y, s16 bottom_y) {
    Vect2D_s32 tl = transform_map_vert(left_x,  top_y);
    Vect2D_s32 tr = transform_map_vert(right_x, top_y);
    if(face_on_screen(tl, tr)) { return 1; }
    Vect2D_s32 bl = transform_map_vert(left_x,  bottom_y);
    if(face_on_screen(bl, tl)) { return 1; }
    Vect2D_s32 br = transform_map_vert(right_x, bottom_y);
    if(face_on_screen(bl, br)) { return 1; }
    return face_on_screen(tr, br);
}

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

/*
void traverse_bsp_nodes_front_to_back(s32 x, s32 y) {
    node* root_node = &(cur_level->nodes[cur_level->num_nodes-1]);

    s16 left_x = root_node->left_box_left;
    s16 right_x = root_node->left_box_right;
    s16 top_y = root_node->left_box_top;
    s16 bottom_y = root_node->left_box_bottom;
    draw_bbox(left_x, right_x, top_y, bottom_y);
}
*/


int get_node_real_idx(u16 idx) {
    return (idx & (~(1<<15)));
}

int is_ssector_idx(u16 idx) {
    return idx & (1<<15);
}

int right_bbox_is_onscreen(const node* nd) {
    return bbox_on_screen(nd->right_box_left, nd->right_box_right, nd->right_box_top, nd->right_box_bottom);
}

int left_bbox_is_onscreen(const node* nd) {
    return bbox_on_screen(nd->left_box_left, nd->left_box_right, nd->left_box_top, nd->left_box_bottom);
}


int is_on_left(const node* nd, fix32 x, fix32 y) {
    s16 dx = fix32ToInt(x) - nd->split_x;
    s16 dy = fix32ToInt(y) - nd->split_y;

    return (((dx * nd->split_dy) - (dy * nd->split_dx)) <= 0);
}

ssector* find_subsector_for_position(fix32 x, fix32 y, int root_node_idx) {
    const node* nodes = cur_level->nodes;
    const ssector* subsectors = cur_level->ssectors;
      int recur(int node_idx) {
        if(is_ssector_idx(node_idx)) {
            return &subsectors[get_node_real_idx(node_idx)];
        } else {
            const node* nd = &(nodes[get_node_real_idx(node_idx)]);
            int on_left = is_on_left(nd, x, y);

            return recur(on_left ? nd->left_child : nd->right_child);
        }
    }
    
    recur(root_node_idx);
}


void draw_bsp_node(fix32 x, fix32 y, int root_node_idx) {
    const node* nodes = cur_level->nodes;

    int recur(int node_idx) {
        if(is_ssector_idx(node_idx)) {
            return enqueue_draw_ssector(get_node_real_idx(node_idx));
        } else {
            const node* nd = &(nodes[get_node_real_idx(node_idx)]);
            int on_left = is_on_left(nd, x, y);

            const uint16_t first_node_idx = (on_left ? nd->left_child : nd->right_child);
            const uint16_t second_node_idx = (on_left ? nd->right_child : nd->left_child);
            const u8 visiting_second_node = (on_left ? right_bbox_is_onscreen(nd) : left_bbox_is_onscreen(nd));

            u8 done = recur(first_node_idx);
            if(!done && visiting_second_node) {
                return recur(second_node_idx);
            }
            return done;
        }
    }
    
    recur(root_node_idx);
}