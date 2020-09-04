#ifndef NODE_H
#define NODE_H

#include <genesis.h>

typedef struct {
    s16 split_x, split_y;
    s16 split_dx, split_dy;
    s16 right_box_top, right_box_bottom, right_box_left, right_box_right;
    s16 left_box_top, left_box_bottom, left_box_left, left_box_right;
    u16 right_child, left_child;
} node;

#endif