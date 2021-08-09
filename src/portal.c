#include <genesis.h>
#include <kdebug.h>
#include "clip_buf.h"
#include "colors.h"
#include "common.h"
#include "debug.h"
#include "div_lut.h"
#include "ball.h"
#include "draw.h"
#include "game.h"
#include "level.h"
#include "math3D.h"
#include "portal_map.h"
#include "portal_maps.h"
#include "sector.h"
#include "slope.h"
#include "vertex.h"


#define PORTAL_QUEUE_SIZE 32

//static Vect2D_s32* cached_verts;
static u32* sector_visited_cache; // i don't know if this helps much, but we might be able to use it



void init_portal_renderer() {
    //vert_transform_cache = MEM_alloc(sizeof(u32) * cur_portal_map->num_verts);
    sector_visited_cache = MEM_alloc(sizeof(u32) * cur_portal_map->num_sectors);   
}

void clear_portal_cache() {
    //memset(sector_visited_cache, 0, sizeof(u32) * cur_portal_map->num_sectors);
}

void cleanup_portal_renderer() {
    MEM_free(sector_visited_cache);
}



// #define DEBUG_PORTAL_CLIP


#define MAX_DEPTH 32
int nwalls;

int post_project_backfacing_walls;
int walls_frustum_culled;
int pre_transform_backfacing_walls;
int portals_frustum_culled;
// u8* src_pvs
void visit_graph(u16 src_sector, u16 sector, u16 x1, u16 x2, u32 cur_frame, uint8_t depth) {
    if(depth >= MAX_DEPTH) {
        return;
    }

    portal_map* map = (portal_map*)cur_portal_map;



    // if this sector has been visited 32 times, or is already being currently rendered, skip it
    if(sector_visited_cache[sector] == cur_frame) { //} & 0b101) { //0x21) {
            return; // Odd = still rendering, 0x20 = give up
    }

    sector_visited_cache[sector] = cur_frame;
        


    u16 window_min = x1; //cur_item.x1;
    u16 window_max = x2; //cur_item.x2;

    s16 wall_offset = sector_wall_offset(sector, map);
    s16 portal_offset = sector_portal_offset(sector, map);
    s16 num_walls = sector_num_walls(sector, map);

    s16 floor_height = sector_floor_height(sector, map); 
    s16 ceil_height = sector_ceil_height(sector, map);

    u8 floor_color = sector_floor_color(sector, map);
    u8 ceil_color = sector_ceil_color(sector, map);

    //Vect2D_f32 sector_center = sector_centers[sector];
    //u32 avg_dist = (
    //    abs(fix32ToInt(sector_center.x - cur_player_pos.x))+
    //    abs(fix32ToInt(sector_center.y - cur_player_pos.y)));

    s8 light_level = get_sector_light_level(sector);

    u16 init_v1_idx = map->walls[wall_offset];
    vertex init_v1 = map->vertexes[init_v1_idx];
    Vect2D_s16 prev_transformed_vert = transform_map_vert_16(init_v1.x, init_v1.y);
    
    s16 last_frontfacing_wall = -1;
    
    u16 prev_v2_idx = init_v1_idx;

    s16 intPx = fix32ToInt(cur_player_pos.x);
    s16 intPy = fix32ToInt(cur_player_pos.y);

    //int render_red_ball = (sector == 10);
    object* objects_in_sect = objects_in_sector(sector);
    int needs_object_clip_buffer = (objects_in_sect != NULL);
    //int needs_object_clip_buffer = render_red_ball;
    
    clip_buf* obj_clip_buf;
    if(needs_object_clip_buffer) {
        //KLog_U1("allocating object clip buffer in sector: ", sector);
        obj_clip_buf = alloc_clip_buffer();
        if(obj_clip_buf == NULL) {
            die("no more clip bufs");
        }
        copy_2d_buffer(window_min, window_max, obj_clip_buf);
    }

    for(s16 i = 0; i < num_walls; i++) {
        // this is plus 1 because we store the number of real walls, 
        // but duplicate the first at the end of the wall list per sector, so we don't need modulo math
        s16 wall_idx = wall_offset+i+1;

        u16 portal_idx = portal_offset+i;
        s16 portal_sector = map->portals[portal_idx];
        int is_portal = portal_sector != -1;

     
        u16 v2_idx = map->walls[wall_idx];

        u16 old_prev_v2_idx = prev_v2_idx;
        prev_v2_idx = v2_idx;
        //if(is_portal) {
        //    u8 byte_idx = (portal_sector>>3);
        //    u8 bit_idx = (portal_sector&0b111);
        //    u8 in_pvs = src_pvs[byte_idx] & (1 << bit_idx);
        //    if(!in_pvs) {
        //        continue;
        //    }
        //}
        

        vertex v1 = map->vertexes[old_prev_v2_idx];
        vertex v2 = map->vertexes[v2_idx];



        u32 v1_dist = (
            abs(v1.x - playerXInt)+
            abs(v1.y - playerYInt));
        u32 v2_dist = (
            abs(v2.x - playerXInt)+
            abs(v2.y - playerYInt));
        u32 avg_dist = (v1_dist + v2_dist)>>1;



        volatile Vect2D_s16 trans_v1;

        if(last_frontfacing_wall == i-1) {
            trans_v1 = prev_transformed_vert;
        } else {
            trans_v1 = transform_map_vert_16(v1.x, v1.y);
        }        

        prev_v2_idx = v2_idx;
        
        
        normal_quadrant normal_dir = map->wall_norm_quadrants[portal_idx];

        u8 backfacing = 0;
        
        switch(normal_dir) {
            case QUADRANT_0:
                backfacing = (intPx < v2.x && intPy < v1.y);
                break;
            case QUADRANT_1:
                backfacing = (intPx > v1.x && intPy < v2.y);
                break;
            case QUADRANT_2:
                backfacing = (intPx > v2.x && intPy > v1.y);
                break;
            case QUADRANT_3:
                backfacing = (intPx < v1.x && intPy > v2.y);
                break;

            case FACING_UP:
                backfacing = (intPy < v1.y);
                break;
            case FACING_DOWN:
                backfacing = (intPy > v1.y);
                break;
            case FACING_LEFT:
                backfacing = (intPx > v1.x);
                break;
            case FACING_RIGHT:
                backfacing = (intPx < v1.x);
                break;
        }
        
        if(backfacing) {
            #ifdef DEBUG_PORTAL_CLIP
            pre_transform_backfacing_walls++;
            if(is_portal) {
                KLog_S1("pre-transform portal cull: ", portal_idx);
            }
            #endif
            continue;
        } else {
        }
        
        
                

        last_frontfacing_wall = i;
        volatile Vect2D_s16 trans_v2 = transform_map_vert_16(v2.x, v2.y);
        prev_transformed_vert = trans_v2;
        

        clip_result clipped = clip_map_vertex_16(&trans_v1, &trans_v2);  
        //u16 z1 = trans_v1.y;
        //u16 z2 = trans_v2.y;
        u16 z_recip_v1 = z_recip_table[trans_v1.y];
        u16 z_recip_v2 = z_recip_table[trans_v2.y];  
        s16 neighbor_floor_height;
        s16 neighbor_ceil_height;

        if(clipped == OFFSCREEN) { 
            if(sector == src_sector && is_portal) {
                #ifdef DEBUG_PORTAL_CLIP
                if(is_portal) {
                    portals_frustum_culled++;
                    KLog_S1("portal fully near clipped: ", portal_idx);
                    KLog_S2("z1: ", trans_v1.y, " z2: ", trans_v2.y);
                }
                #endif
                if(trans_v1.y == 0) { trans_v1.y = 1; z_recip_v1 = 65535; }
                if(trans_v2.y == 0) { trans_v2.y = 1; z_recip_v2 = 65535; }
                if(trans_v1.y < 0 && trans_v2.y < 0) {  continue; }
                s16 x1 = project_and_adjust_x(trans_v1.x, z_recip_v1);
                s16 x2 = project_and_adjust_x(trans_v2.x, z_recip_v2);

                if(x1 > window_max) { 
                    if(trans_v1.y < 0 && trans_v2.y > 0) {
                        x1 = window_min;
                    } else {
                    #ifdef DEBUG_PORTAL_CLIP
                        walls_frustum_culled++; 
                        KLog_S1("portal right frustum culled after near clip?: ", portal_idx);
                        KLog_S2("left: ", x1, " right: ", x2);
                    #endif
                        continue;
                    }
                }
                if(x2 <= window_min) {
                    if(trans_v2.y < 0 && trans_v1.y > 0) {
                        //goto submit_anyway;
                        x2 = window_max;
                    } else {
                    #ifdef DEBUG_PORTAL_CLIP
                    walls_frustum_culled++; 
                    KLog_S1("portal left frustum culled after near clip?: ", portal_idx);
                    #endif
                    continue;
                    }
                }
                if(x1 >= x2) { 
                    #ifdef DEBUG_PORTAL_CLIP
                    post_project_backfacing_walls++; 
                    KLog_S1("portal backface culled after near clip?: ", portal_idx);
                    #endif
                    continue; 
                }
                neighbor_floor_height = sector_floor_height(portal_sector, cur_portal_map);
                neighbor_ceil_height = sector_ceil_height(portal_sector, cur_portal_map);
                if (neighbor_ceil_height > floor_height && neighbor_floor_height < ceil_height) {
                    visit_graph(src_sector, portal_sector, window_min, window_max, cur_frame, depth+1);
                    //add_new_sector_to_queue(window_min, window_max, portal_sector);
                }
                
            }
            
            continue;
        }
        s16 trans_v1_z = trans_v1.y;
        s16 trans_v2_z = trans_v2.y;

        s16 max_z = max(trans_v1_z, trans_v2_z);
        s16 x1 = project_and_adjust_x(trans_v1.x, z_recip_v1);
        s16 x2 = project_and_adjust_x(trans_v2.x, z_recip_v2);
        s16 beginx = x1;
        if(beginx <= window_min) {
            beginx = window_min;
        }

        s16 endx = x2; 
        if(endx >= window_max) {
            endx = window_max;
        }
        

        if (x1 > window_max) {
                #ifdef DEBUG_PORTAL_CLIP
                walls_frustum_culled++; 
                if(is_portal) {
                    KLog_S1("portal right frustum culled: ", portal_idx);
                }
                #endif
                if(is_portal && clipped == LEFT_CLIPPED && x2 >= window_min) {
                    #ifdef DEBUG_PORTAL_CLIP
                        KLog_S1("portal right frustum culled but still enqueued: ", portal_idx);
                    #endif
                    visit_graph(src_sector, portal_sector, window_min, endx, cur_frame, depth+1);
                }
                continue;
            } else if (x2 <= window_min) { 
                #ifdef DEBUG_PORTAL_CLIP
                    walls_frustum_culled++; 
                    if(is_portal) {
                        KLog_S1("portal left frustum culled: ", portal_idx);
                    }
                #endif
                if(is_portal && clipped == RIGHT_CLIPPED && x1 < window_max) {
                    #ifdef DEBUG_PORTAL_CLIP
                        KLog_S2("portal left frustum culled but still enqueued: ", portal_idx, " to sector: ", portal_sector);
                    #endif
                    visit_graph(src_sector, portal_sector, beginx, window_max, cur_frame, depth+1);
                }

                continue; 
            } else {
                #ifdef DEBUG_PORTAL_CLIP
                if(is_portal) {
                    KLog_S2("portal not frustum culled: ", portal_idx, " to sector: ", portal_sector);
                }
                #endif
            }
            
        if(x1 >= x2) {  
            #ifdef DEBUG_PORTAL_CLIP
            post_project_backfacing_walls++;
            if(is_portal) {
                KLog_S1("portal backface culled: ", portal_idx);
                KLog_S2("coords x1: ", x1, "x2: ", x2);
            }
            #endif
            continue; 
        } else {
            #ifdef DEBUG_PORTAL_CLIP
            if(is_portal) {
                KLog_S1("portal not backface culled: ", portal_idx);
            }
            #endif
        }

        slope_type floor_slope = cur_portal_map->wall_floor_slope_types[portal_idx];
        slope_type ceil_slope = cur_portal_map->wall_ceil_slope_types[portal_idx];
        int recur = 0;

        s16 x1_ytop;
        s16 x1_ybot;
        s16 x2_ytop;
        s16 x2_ybot;
        
        u8 last_wall = (i == num_walls-1);
        u8 first_wall = (i == 0);

        if(is_portal) {
            neighbor_floor_height = sector_floor_height(portal_sector, cur_portal_map);
            neighbor_ceil_height = sector_ceil_height(portal_sector, cur_portal_map);
            x1_ytop = project_and_adjust_y_fix(ceil_height, z_recip_v1);
            x1_ybot = project_and_adjust_y_fix(floor_height, z_recip_v1);
            x2_ytop = project_and_adjust_y_fix(ceil_height, z_recip_v2);
            x2_ybot = project_and_adjust_y_fix(floor_height, z_recip_v2);

            if (neighbor_ceil_height > floor_height && neighbor_floor_height < ceil_height) {
                recur = 1;
            }
        } else {
            
            // we can have slopes on floor and ceiling, sloping in different ways :^)
            s16 x2_ybot_floor_height = floor_height;
            s16 x1_ybot_floor_height = floor_height;


            if (floor_slope == SLOPE_TRANSITION_RIGHT) {
                s16 floor_slope_portal_sector = map->floor_slope_portals[portal_offset+i];
                //s16 right_portal_sector;
                //if(last_wall) {
                //    right_portal_sector = map->portals[portal_offset+0];
                //} else {
                //    right_portal_sector = map->portals[portal_offset+i+1];
                //}
                neighbor_floor_height = sector_floor_height(floor_slope_portal_sector, cur_portal_map);
                x2_ybot_floor_height = neighbor_floor_height;
            } else if (floor_slope == SLOPE_TRANSITION_LEFT) {
                s16 floor_slope_portal_sector = map->floor_slope_portals[portal_offset+i];
                //s16 left_portal_sector;
                //if(first_wall) {
                //    left_portal_sector = map->portals[portal_offset+num_walls-1];
                //} else {
                //    left_portal_sector = map->portals[portal_offset+i-1];
                //}
                neighbor_floor_height = sector_floor_height(floor_slope_portal_sector, cur_portal_map);
                x1_ybot_floor_height = neighbor_floor_height;
            }
            x2_ybot = project_and_adjust_y_fix(x2_ybot_floor_height, z_recip_v2);
            x1_ybot = project_and_adjust_y_fix(x1_ybot_floor_height, z_recip_v1);


            s16 x2_ytop_floor_height = ceil_height;
            s16 x1_ytop_floor_height = ceil_height;
            

            if(ceil_slope == SLOPE_TRANSITION_RIGHT) {
                s16 ceil_slope_portal_sector = map->ceil_slope_portals[portal_offset+i];
                //s16 right_portal_sector;
                //if(last_wall) {
                //    right_portal_sector = map->portals[portal_offset+0];
                //} else {
                //    right_portal_sector = map->portals[portal_offset+i+1];
                //}
                neighbor_ceil_height = sector_ceil_height(ceil_slope_portal_sector, cur_portal_map);
                x2_ytop_floor_height = neighbor_ceil_height, z_recip_v2;
            } else if (ceil_slope == SLOPE_TRANSITION_LEFT) {
                s16 ceil_slope_portal_sector = map->ceil_slope_portals[portal_offset+i];
                //s16 left_portal_sector;
                //if(first_wall) {
                //    left_portal_sector = map->portals[portal_offset+num_walls-1];
                //} else {
                //    left_portal_sector = map->portals[portal_offset+i-1];
                //}
                neighbor_ceil_height = sector_ceil_height(ceil_slope_portal_sector, cur_portal_map);
                x1_ytop_floor_height = neighbor_ceil_height;
            }
            x2_ytop = project_and_adjust_y_fix(x2_ytop_floor_height, z_recip_v2);
            x1_ytop = project_and_adjust_y_fix(x1_ytop_floor_height, z_recip_v1);
            

        }

     
        // draw floor and ceiling
        // check if floor and ceiling are on-screen


        clip_buf* clipping_buffer = NULL;

        s16 rel_floor_height = (floor_height>>4) - (cur_player_pos.z>>(FIX32_FRAC_BITS));
        s16 rel_ceil_height = (ceil_height>>4) - (cur_player_pos.z>>(FIX32_FRAC_BITS));
        cache_ceil_light_params(rel_ceil_height, ceil_color, light_level);
        cache_floor_light_params(rel_floor_height, floor_color, light_level);

        int render_forcefield = (sector == 0 && is_portal && portal_sector == 2) || (sector == 2 && is_portal && portal_sector == 0); 
        render_forcefield = (sector == 6 && is_portal && portal_sector == 7) || (sector == 7 && is_portal && portal_sector == 6);
        int render_glass = (sector == 0 && is_portal && portal_sector == 1) || (sector == 1 && is_portal && portal_sector == 0);


        
        if (render_forcefield || render_glass)  {
            
            //clipping_buffer = &clip_buffers[0]; 
            clipping_buffer = alloc_clip_buffer();
            //KLog_U1("allocating wall clip buffer in sector: ", sector);
            if(clipping_buffer == NULL) {
                die("Out of clipping buffers!");
            }
        }
        

        if (is_portal) {


            #ifdef DEBUG_PORTAL_CLIP
            KLog_S2("portal in sector: ", sector, " drawn with wall idx: ", i);
            #endif
            // draw step down from ceiling
            if(neighbor_ceil_height < ceil_height && neighbor_ceil_height > floor_height) {
                s16 nx1_ytop = project_and_adjust_y_fix(neighbor_ceil_height, z_recip_v1);
                s16 nx2_ytop = project_and_adjust_y_fix(neighbor_ceil_height, z_recip_v2);
                if(ceil_slope == SLOPE_PORTAL) {

                    draw_ceiling_update_clip(x1, nx1_ytop, x2, nx2_ytop, max_z, window_min, window_max, ceil_color);
                } else {
                    u8 upper_color = map->wall_colors[portal_idx].upper_col;
                    upper_color = calculate_color(upper_color, avg_dist, light_level);
                    // draw step from ceiling
                    draw_upper_step(x1, x1_ytop, nx1_ytop, x2, x2_ytop, nx2_ytop, 
                                    z_recip_v1, z_recip_v2,
                                    window_min, window_max, upper_color, ceil_color, light_level);
                }
            } else {
                draw_ceiling_update_clip(x1, x1_ytop, x2, x2_ytop, max_z, window_min, window_max, ceil_color);
            }

            // not sure if this logic is correct
            // if the neighbor's floor is higher than our ceiling we should still draw the lower step, right?
            if(neighbor_floor_height > floor_height && neighbor_floor_height < ceil_height) {
                s16 nx1_ybot = project_and_adjust_y_fix(neighbor_floor_height, z_recip_v1);
                s16 nx2_ybot = project_and_adjust_y_fix(neighbor_floor_height, z_recip_v2);
                if(floor_slope == SLOPE_PORTAL) {
                    draw_floor_update_clip(x1, nx1_ybot, x2, nx2_ybot, 
                                           max_z,
                                           window_min, window_max, floor_color);

                } else {
                    u8 lower_color = map->wall_colors[portal_idx].lower_col;
                    lower_color = calculate_color(lower_color, avg_dist, light_level);

                    // draw step from floor
                    draw_lower_step(x1, x1_ybot, nx1_ybot, x2, x2_ybot, nx2_ybot, 
                                    z_recip_v1, z_recip_v2,
                                    window_min, window_max, lower_color, floor_color, light_level);
                }
            } else {
                draw_floor_update_clip(x1, x1_ybot, x2, x2_ybot, 
                                       max_z,
                                       window_min, window_max, floor_color);
            }
            if(render_forcefield || render_glass) {
                copy_2d_buffer(window_min, window_max, clipping_buffer);
            }
            
            if(recur) {
                visit_graph(src_sector, portal_sector,
                            ((clipped == LEFT_CLIPPED) ? window_min : beginx),
                            ((clipped == RIGHT_CLIPPED) ? window_max : endx),
                            cur_frame, depth+1);
            }

        } else { 
            #ifdef DEBUG_PORTAL_CLIP
            KLog_S2("wall in sector: ", sector, " drawn with wall idx: ", i);
            #endif


            u8 wall_color = map->wall_colors[portal_idx].mid_col;
            //wall_color = calculate_color(wall_color, avg_dist, light_level);
            draw_wall(x1, x1_ytop, x1_ybot, x2, x2_ytop, x2_ybot, 
                      z_recip_v1, z_recip_v2,
                      window_min, window_max, ceil_color, wall_color, floor_color, light_level);
        }
        //nwalls++;


        // after this point, draw some sprites :^)
        
        if (render_forcefield) {

            uint16_t col = (RED_IDX<<4)|LIGHT_GREEN_IDX;
            draw_forcefield(x1, x2, window_min, window_max, clipping_buffer, col);

            free_clip_buffer(clipping_buffer);

        } else if (render_glass) {
            draw_transparent_wall(x1, x1_ytop, x1_ybot, x2, x2_ytop, x2_ybot, window_min, window_max, clipping_buffer, (LIGHT_RED_IDX<<4)|LIGHT_GREEN_IDX);
            free_clip_buffer(clipping_buffer);
        }
        

        
    }

    
    /*
    if(objects_in_sect != NULL) {
        object* cur_obj = objects_in_sect;
        while(cur_obj != NULL) {
            object_pos pos = cur_obj->pos;
            s16 flr_height = sector_floor_height(sector, cur_portal_map);
            Vect2D_s16 trans_pos = transform_map_vert_16(fix32ToInt(pos.x), fix32ToInt(pos.y));
            s16 z_recip = z_recip_table[trans_pos.y];

            if(trans_pos.y > 0) {

                s16 left_x = project_and_adjust_x(trans_pos.x-6, trans_pos.y, z_recip);
                s16 right_x = project_and_adjust_x(trans_pos.x+6, trans_pos.y, z_recip);
                
                object_template type = object_types[cur_obj->object_type];

                s16 top_y = project_and_adjust_y_fix(pos.z+type.from_floor_draw_offset+type.height, trans_pos.y, z_recip);
                s16 bot_y = project_and_adjust_y_fix(pos.z+type.from_floor_draw_offset, trans_pos.y, z_recip);

                draw_masked(left_x,top_y, bot_y,
                        right_x, top_y, bot_y,
                        window_min, window_max,
                        obj_clip_buf,
                        type.sprite_col);
            }
            break;
            //cur_obj = cur_obj->next;
        }
        free_clip_buffer(obj_clip_buf);
    }
    */


}


void portal_rend(u16 src_sector, u32 cur_frame) {
    #ifdef DEBUG_PORTAL_CLIP
    pre_transform_backfacing_walls = 0;
    walls_frustum_culled = 0;
    portals_frustum_culled = 0;
    post_project_backfacing_walls = 0;
    #endif
    visit_graph(src_sector, src_sector, 0, RENDER_WIDTH-1, cur_frame, 0);
    #ifdef DEBUG_PORTAL_CLIP
    KLog_S1("walls pre-transform backface culled ", pre_transform_backfacing_walls);
    KLog_S1("portals/walls frustum culled ", walls_frustum_culled);
    KLog_S1("portals frustum culled ", portals_frustum_culled);
    KLog_S1("walls post-project backface culled ", post_project_backfacing_walls);
    KLog("-------- frame-end --------");
    #endif
}
