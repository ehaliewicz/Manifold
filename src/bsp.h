#ifndef BSP_H
#define BSP_H

#include <genesis.h>

//void traverse_bsp_nodes_front_to_back(s32 x, s32 y);
void draw_bsp_node(fix32 x, fix32 y, int root_node_idx);

ssector* find_subsector_for_position(fix32 x, fix32 y, int root_node_idx);

#endif