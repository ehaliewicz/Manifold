#include <genesis.h>
#include <kdebug.h>
#include "clip_buf.h"
#include "colors.h"
#include "config.h"
#include "div_lut.h"
#include "draw.h"
#include "game.h"
#include "level.h"
#include "math3D.h"
#include "obj_sprite.h"
#include "portal_map.h"
#include "sector_group.h"
#include "textures.h"
#include "utils.h"
#include "vertex.h"


#define PORTAL_QUEUE_SIZE 32

//static Vect2D_s32* cached_verts;
static u32* sector_visited_cache; // i don't know if this helps much, but we might be able to use it

// num_walls, sector_idx

// left_x, right_x, left_z_fix, right_z_fix, left_u, right_u, portal_sector_idx, EMPTY



void init_portal_renderer() {
    //vert_transform_cache = malloc(sizeof(u32) * cur_portal_map->num_verts, "vertex transform cache");
    sector_visited_cache = malloc(sizeof(u32) * cur_portal_map->num_sectors, "sector visited cache");
}


void clear_portal_cache() {
    memset(sector_visited_cache, 0, sizeof(u32) * cur_portal_map->num_sectors);
}

void cleanup_portal_renderer() {
    free(sector_visited_cache, "sector visited cache");
}


// #define DEBUG_PORTAL_CLIP


#define MAX_DEPTH 32
int nwalls;

int post_project_backfacing_walls;
int walls_frustum_culled;
int pre_transform_backfacing_walls;
int portals_frustum_culled;

void draw_sprite_obj(
    s16 left_x, s16 right_x, 
    s16 top_y, s16 bot_y, 
    u16 window_min, u16 window_max,
    clip_buf* obj_clip_buf);

int sectors_scanned = 0;
void visit_graph(u16 src_sector, u16 sector, u16 x1, u16 x2, u32 cur_frame, uint8_t depth) {
    if(depth >= MAX_DEPTH ) {
        return;
    }
    portal_map* map = cur_portal_map;

    // if this sector has been visited 32 times, or is already being currently rendered, skip it
    
    if(sector_visited_cache[sector] & 0b1) { //} & 0b101) { //0x21) {
            return; // Odd = still rendering, 0x20 = give up
    }
    // if we've visited this sector 7 times in one frame (lol)
    if(sector_visited_cache[sector] >= 14) {
        return;
    }

    sector_visited_cache[sector]++; // = cur_frame;

    u16 window_min = x1; //cur_item.x1;
    u16 window_max = x2; //cur_item.x2;

    s16 wall_offset = sector_wall_offset(sector, map);
    s16 portal_offset = sector_portal_offset(sector, map);
    s16 num_walls = sector_num_walls(sector, map);
    u16 sect_group = sector_group(sector, map);




    s8 light_level = get_sector_group_light_level(sect_group);

    u16 init_v1_idx = map->walls[wall_offset];
    vertex init_v1 = map->vertexes[init_v1_idx];
    Vect2D_s16 prev_transformed_vert = transform_map_vert_16(init_v1.x, init_v1.y);

    s16 last_frontfacing_wall = -1;

    u16 prev_v2_idx = init_v1_idx;

    s16 intPx = fix32ToInt(cur_player_pos.x);
    s16 intPy = fix32ToInt(cur_player_pos.y);

    //int render_red_ball = (sector == 10);
    object* objects_in_sect = NULL; // objects_in_sector(sector);
    //KLog_U1("objects in sect?: ", objects_in_sect);
    int needs_object_clip_buffer = (objects_in_sect != NULL); //render_red_ball && (objects_in_sect != NULL);
    //int needs_object_clip_buffer = ;

    clip_buf* obj_clip_buf;
    if(needs_object_clip_buffer) {
        obj_clip_buf = alloc_clip_buffer();
        //KLog_U2("allocating object clip buffer in sector: ", sector, " id: ", obj_clip_buf->id);
        if(obj_clip_buf == NULL) {
            die("no more clip bufs");
        }
        copy_2d_buffer(window_min, window_max, obj_clip_buf);
    }


    s16 floor_height = get_sector_group_floor_height(sect_group);
    s16 ceil_height = get_sector_group_ceil_height(sect_group);
    s16 rel_floor_height = (floor_height>>4) - (cur_player_pos.z>>(FIX32_FRAC_BITS));
    s16 rel_ceil_height = (ceil_height>>4) - (cur_player_pos.z>>(FIX32_FRAC_BITS));

    u8 floor_color = get_sector_group_floor_color(sect_group);
    u8 ceil_color = get_sector_group_ceil_color(sect_group);

    u8 cur_sect_group_type = cur_portal_map->sector_group_types[sect_group];


    light_params ceil_params, floor_params;
    cache_ceil_light_params(rel_ceil_height, ceil_color, light_level, &ceil_params);
    cache_floor_light_params(rel_floor_height, floor_color, light_level, &floor_params);

    for(s16 i = 0; i < num_walls; i++) {


        //const int debug = (sector == 0) && (i == 0);
        // this is plus 1 because we store the number of real walls,
        // but duplicate the first at the end of the wall list per sector, so we don't need modulo math
        s16 wall_idx = wall_offset+i+1;

        u16 portal_idx = portal_offset+i;

        s16 portal_sector = map->portals[portal_idx];
        int is_portal = portal_sector != -1;


        u16 v2_idx = map->walls[wall_idx];

        u16 old_prev_v2_idx = prev_v2_idx;
        prev_v2_idx = v2_idx;


        vertex v1 = map->vertexes[old_prev_v2_idx];
        vertex v2 = map->vertexes[v2_idx];

        volatile Vect2D_s16 trans_v1;

        if(last_frontfacing_wall == i-1) {
            trans_v1 = prev_transformed_vert;
        } else {
            if(0) { //if(debug) {
                KLog("VERT 1");
            }
            trans_v1 = transform_map_vert_16(v1.x, v1.y);
        }

        prev_v2_idx = v2_idx;

        
        normal_quadrant normal_dir = map->wall_norm_quadrants[portal_idx];

        u8 backfacing = 0;
        /*
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
            //inc_counter(COARSE_BACKFACE_CULL_COUNTER);
            continue;
        } else {
        }
        */




        last_frontfacing_wall = i;
        if(0) { //if(debug) {
            KLog("VERT 2");
        }
        volatile Vect2D_s16 trans_v2 = transform_map_vert_16(v2.x, v2.y);
        prev_transformed_vert = trans_v2;


        u8 tex_idx = map->wall_colors[(portal_idx<<WALL_COLOR_NUM_PARAMS_SHIFT)+WALL_TEXTURE_IDX];
        u8 is_solid_color = map->wall_colors[(portal_idx<<WALL_COLOR_NUM_PARAMS_SHIFT)+WALL_SOLID_COLOR_IDX];
        //KLog_U1("solid color: ", is_solid_color);

        
        texmap_params tmap_info = {
            .repetitions = wall_tex_repetitions[portal_idx],
            .needs_texture = !is_solid_color
        };
        u8 wall_col = is_solid_color; //tex_idx;
        if(!is_solid_color) {
            lit_texture *tex = get_texture(tex_idx, light_level);
            tmap_info.tex = tex;
        }
        clip_result clipped = clip_map_vertex_16(&trans_v1, &trans_v2, &tmap_info);


        u16 z_recip_v1 = z_recip_table_16[trans_v1.y>>TRANS_Z_FRAC_BITS];
        u16 z_recip_v2 = z_recip_table_16[trans_v2.y>>TRANS_Z_FRAC_BITS];
        s16 neighbor_floor_height, neighbor_ceil_height;

        if(clipped == OFFSCREEN) {
            
            //inc_counter(NEAR_Z_CULL_COUNTER);
            if(sector == src_sector && is_portal) {
                #ifdef DEBUG_PORTAL_CLIP
                if(is_portal) {
                    portals_frustum_culled++;
                    KLog_S1("portal fully near clipped: ", portal_idx);
                    KLog_S2("z1: ", trans_v1.y, " z2: ", trans_v2.y);
                }
                #endif
                if(trans_v1.y < NEAR_Z_FIX) { trans_v1.y = (1<<2)<<TRANS_Z_FRAC_BITS; z_recip_v1 = 65535; }
                if(trans_v2.y < NEAR_Z_FIX) { trans_v2.y = (1<<2)<<TRANS_Z_FRAC_BITS; z_recip_v2 = 65535; }
                //if(trans_v1.y < NEAR_Z_FIX) { trans_v1.y =  NEAR_Z_FIX; z_recip_v1 = 65535; }
                //if(trans_v2.y < NEAR_Z_FIX) { trans_v2.y = NEAR_Z_FIX; z_recip_v2 = 65535; }
                if(trans_v1.y < 0 && trans_v2.y < 0) {  continue; }
                s16 x1 = (clipped & LEFT_FRUSTUM_CLIPPED) ? 0 : project_and_adjust_x(trans_v1.x, z_recip_v1);
                s16 x2 = (clipped & RIGHT_FRUSTUM_CLIPPED) ? RENDER_WIDTH : project_and_adjust_x(trans_v2.x, z_recip_v2);

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
                u16 portal_sect_group = sector_group(portal_sector, map);
                neighbor_floor_height = get_sector_group_floor_height(portal_sect_group);
                neighbor_ceil_height = get_sector_group_ceil_height(portal_sect_group);
                if (neighbor_ceil_height > floor_height && neighbor_floor_height < ceil_height) {
                    visit_graph(src_sector, portal_sector, window_min, window_max, cur_frame, depth+1);
                    //add_new_sector_to_queue(window_min, window_max, portal_sector);
                }

            }

            continue;
        }

        //KLog("potentially on-screen");
        s16 trans_v1_z_fix = trans_v1.y;
        //s32 trans_v1_z_int = trans_v1_z_fix>>TRANS_Z_FRAC_BITS;
        s16 trans_v2_z_fix = trans_v2.y;
        //s32 trans_v2_z_int = trans_v2_z_fix>>TRANS_Z_FRAC_BITS;

        //s16 max_z_int = max(trans_v1_z_int, trans_v2_z_int);

        s16 x1 = clipped & LEFT_FRUSTUM_CLIPPED ? 0 : project_and_adjust_x(trans_v1.x, z_recip_v1);
        s16 x2 = clipped & RIGHT_FRUSTUM_CLIPPED ? RENDER_WIDTH : project_and_adjust_x(trans_v2.x, z_recip_v2);
        //KLog_S2("px1: ", x1, "px2: ", x2);

        s16 beginx = x1;
        if(beginx <= window_min) {
            beginx = window_min;
        }

        s16 endx = x2;
        if(endx >= window_max) {
            endx = window_max;
        }


        if (x1 > window_max) {
            if(is_portal && clipped & LEFT_Z_CLIPPED && x2 >= window_min) {
                visit_graph(src_sector, portal_sector, window_min, endx, cur_frame, depth+1);
            }
            continue;
        } else if (x2 <= window_min) {
            if(is_portal && clipped & RIGHT_Z_CLIPPED && x1 < window_max) {
                visit_graph(src_sector, portal_sector, beginx, window_max, cur_frame, depth+1);
            }

            continue;
        } else {
        }

        if(x1 >= x2) {
            //inc_counter(POST_PROJ_BACKFACE_CULL_COUNTER);
            continue;
        } else {
        }

        int recur = 0;

        s16 x1_ytop, x1_ybot;
        s16 x2_ytop, x2_ybot;

        s16 x1_pegged, x2_pegged;

        //u16 neighbor_sect_group; 
        s16 *neighbor_sect_group_param_pointer;

        if(is_portal) {
            u16 neighbor_sect_group = sector_group(portal_sector, map);
            neighbor_sect_group_param_pointer = get_sector_group_pointer(neighbor_sect_group);

            //u8 neighbor_sector_group_type = cur_portal_map->sector_group_types[neighbor_sect_group];
            //s16 neighbor_ceil_color = get_sector_group_floor_color(neighbor_sect_group);
            //s16 neighbor_floor_color = get_sector_group_floor_color(neighbor_sect_group);

            neighbor_floor_height = neighbor_sect_group_param_pointer[SECTOR_PARAM_FLOOR_HEIGHT_IDX]; //get_sector_group_floor_height(neighbor_sect_group);
            neighbor_ceil_height = neighbor_sect_group_param_pointer[SECTOR_PARAM_CEIL_HEIGHT_IDX]; //get_sector_group_ceil_height(neighbor_sect_group);



            x1_ybot = project_and_adjust_y_fix(floor_height, z_recip_v1);
            x2_ybot = project_and_adjust_y_fix(floor_height, z_recip_v2);

            x1_ytop = project_and_adjust_y_fix(ceil_height, z_recip_v1);
            x2_ytop = project_and_adjust_y_fix(ceil_height, z_recip_v2);

            // this test is iffy here
            // can cause issues with floors/ceilings leaking e.g. medusa effect
            if (neighbor_ceil_height > floor_height && neighbor_floor_height < ceil_height) {
                recur = 1;
            }
        } else {

            s16 x2_ybot_floor_height = floor_height;
            s16 x1_ybot_floor_height = floor_height;

            x2_ybot = project_and_adjust_y_fix(x2_ybot_floor_height, z_recip_v2);
            x1_ybot = project_and_adjust_y_fix(x1_ybot_floor_height, z_recip_v1);


            s16 x2_ytop_floor_height = ceil_height;
            s16 x1_ytop_floor_height = ceil_height;


            x2_ytop = project_and_adjust_y_fix(x2_ytop_floor_height, z_recip_v2);
            x1_ytop = project_and_adjust_y_fix(x1_ytop_floor_height, z_recip_v1);

            if(cur_sect_group_type == DOOR || cur_sect_group_type == LIFT) {
                // top peg all walls in a door/crusher type sector
                s16 orig_door_height = get_sector_group_orig_height(sect_group);
                //s16 orig_height_diff = orig_door_height - neighbor_floor_height;
                x1_pegged = project_and_adjust_y_fix(orig_door_height, z_recip_v1);
                x2_pegged = project_and_adjust_y_fix(orig_door_height, z_recip_v2);
            }

        }


        // draw floor and ceiling
        // check if floor and ceiling are on-screen
        //clip_buf* clipping_buffer = NULL;


        //int render_forcefield = (sector == 0 && is_portal && portal_sector == 2) || (sector == 2 && is_portal && portal_sector == 0);
        //int render_forcefield = (sector == 6 && is_portal && portal_sector == 7) || (sector == 7 && is_portal && portal_sector == 6);
        //int render_glass = 0; //(sector == 0 && is_portal && portal_sector == 1) || (sector == 1 && is_portal && portal_sector == 0);

        //if (render_forcefield)  {
        //    clipping_buffer = alloc_clip_buffer();
        //    if(clipping_buffer == NULL) {
        //        die("Out of clipping buffers!");
        //    }
        //}


        if (is_portal) {

            #ifdef DEBUG_PORTAL_CLIP
            KLog_S2("portal in sector: ", sector, " drawn with wall idx: ", i);
            #endif
            // draw step down from ceiling
            
            u16 nsect_group = sector_group(portal_sector, map);
            u8 neighbor_sector_group_type = cur_portal_map->sector_group_types[nsect_group];
            if(neighbor_ceil_height < ceil_height) {

                s16 nx1_ytop = project_and_adjust_y_fix(neighbor_ceil_height, z_recip_v1);
                s16 nx2_ytop = project_and_adjust_y_fix(neighbor_ceil_height, z_recip_v2);

                if(neighbor_sector_group_type == DOOR) {

                    s16 orig_door_height = neighbor_sect_group_param_pointer[SECTOR_PARAM_ORIG_HEIGHT_IDX]; //get_sector_group_orig_height(neighbor_sect_group);
                    s16 orig_height_diff = orig_door_height - neighbor_floor_height;
                    x1_pegged = project_and_adjust_y_fix(neighbor_ceil_height+orig_height_diff, z_recip_v1);
                    x2_pegged = project_and_adjust_y_fix(neighbor_ceil_height+orig_height_diff, z_recip_v2);
                }

                u8 upper_color = map->wall_colors[(portal_idx<<WALL_COLOR_NUM_PARAMS_SHIFT)+WALL_HIGH_COLOR_IDX];
                // draw step from ceiling
                //draw_upper_step(x1, x1_ytop, nx1_ytop, x2, x2_ytop, nx2_ytop,
                //                z_recip_v1, z_recip_v2,
                //                window_min, window_max, upper_color, light_level, &ceil_params);

                if(neighbor_sector_group_type == DOOR && !is_solid_color) {
                    //KLog_S2("nx1_ytop: ", nx1_ytop, "nx2_ytop: ", nx2_ytop);
                    //KLog_S2("x1 door top: ", x1_door_top, "x2 door top: ", x2_door_top);
                    draw_top_pegged_textured_upper_step(x1, x1_ytop, nx1_ytop, x2, x2_ytop, nx2_ytop,
                                                        trans_v1_z_fix, trans_v2_z_fix,
                                                        z_recip_v1, z_recip_v2,
                                                        window_min, window_max, light_level,
                                                        &tmap_info,
                                                        &ceil_params, x1_pegged, x2_pegged);
                } else {
                    draw_upper_step(x1, x1_ytop, nx1_ytop, x2, x2_ytop, nx2_ytop,
                                    z_recip_v1, z_recip_v2,
                                    window_min, window_max, upper_color, light_level, &ceil_params);

                }
            } else {
                u16 neighbor_ceil_color = neighbor_sect_group_param_pointer[SECTOR_PARAM_CEIL_COLOR_IDX]; //get_sector_group_ceil_color(neighbor_sect_group);
                if(neighbor_ceil_color != ceil_color || neighbor_ceil_height != ceil_height || !recur) {
                    draw_ceiling_update_clip(x1, x1_ytop, x2, x2_ytop,
                                            min(z_recip_v1, z_recip_v2),
                                            window_min, window_max, &ceil_params);
                }
            }

            // not sure if this logic is correct
            // if the neighbor's floor is higher than our ceiling we should still draw the lower step, right?
            if(neighbor_floor_height > floor_height) { //} && neighbor_floor_height <= ceil_height) {
                s16 nx1_ybot = project_and_adjust_y_fix(neighbor_floor_height, z_recip_v1);
                s16 nx2_ybot = project_and_adjust_y_fix(neighbor_floor_height, z_recip_v2);

                if(neighbor_sector_group_type == LIFT) {

                    s16 orig_lift_height = neighbor_sect_group_param_pointer[SECTOR_PARAM_ORIG_HEIGHT_IDX]; //get_sector_group_orig_height(neighbor_sect_group);
                    s16 orig_height_diff = neighbor_ceil_height - orig_lift_height;
                    x1_pegged = project_and_adjust_y_fix(neighbor_floor_height-orig_height_diff, z_recip_v1);
                    x2_pegged = project_and_adjust_y_fix(neighbor_floor_height-orig_height_diff, z_recip_v2);
                }

                u8 lower_color = map->wall_colors[(portal_idx<<WALL_COLOR_NUM_PARAMS_SHIFT)+WALL_LOW_COLOR_IDX];

                if(neighbor_sector_group_type == LIFT && !is_solid_color) {

                    draw_bottom_pegged_textured_lower_step(x1, x1_ybot, nx1_ybot, x2, x2_ybot, nx2_ybot,
                                                        trans_v1_z_fix, trans_v2_z_fix,
                                                        z_recip_v1, z_recip_v2,
                                                        window_min, window_max, light_level,
                                                        &tmap_info,
                                                        &floor_params, x1_pegged, x2_pegged);
                } else {
                    // draw step from floor
                    draw_lower_step(x1, x1_ybot, nx1_ybot, x2, x2_ybot, nx2_ybot,
                                    z_recip_v1, z_recip_v2,
                                    window_min, window_max, lower_color, light_level, &floor_params);
                }
            } else {            
                u16 neighbor_floor_color = neighbor_sect_group_param_pointer[SECTOR_PARAM_FLOOR_COLOR_IDX]; //get_sector_group_floor_color(neighbor_sect_group);
                if(neighbor_floor_color != floor_color || neighbor_floor_height != floor_height || !recur) {
                    draw_floor_update_clip(x1, x1_ybot, x2, x2_ybot,
                                        min(z_recip_v1, z_recip_v2),
                                        window_min, window_max, &floor_params);
                }
            }

            //if(render_forcefield) {
            //    copy_2d_buffer(window_min, window_max, clipping_buffer);
            //}

            if(recur) {
                visit_graph(src_sector, portal_sector,
                            ((clipped & LEFT_Z_CLIPPED) ? window_min : beginx),
                            ((clipped & RIGHT_Z_CLIPPED) ? window_max : endx),
                            cur_frame, depth+1);

            }

        } else {
            #ifdef DEBUG_PORTAL_CLIP
            KLog_S2("wall in sector: ", sector, " drawn with wall idx: ", i);
            #endif
            if(is_solid_color) {

               draw_solid_color_wall(
                   x1, x1_ytop, x1_ybot, x2, x2_ytop, x2_ybot,
                    z_recip_v1, z_recip_v2, 
                    window_min, window_max,
                    light_level, wall_col,
                    &floor_params, &ceil_params);
            } else {
                if(cur_sect_group_type == DOOR) {
                    draw_top_pegged_wall(x1, x1_ytop, x1_ybot, x2, x2_ytop, x2_ybot,
                                        trans_v1_z_fix, trans_v2_z_fix,
                                        z_recip_v1, z_recip_v2,
                                        window_min, window_max, light_level,
                                        &tmap_info,
                                        &floor_params, &ceil_params,
                                        x1_pegged, x2_pegged);
                } else if (cur_sect_group_type == LIFT) {
                    draw_bot_pegged_wall(x1, x1_ytop, x1_ybot, x2, x2_ytop, x2_ybot,
                                        trans_v1_z_fix, trans_v2_z_fix,
                                        z_recip_v1, z_recip_v2,
                                        window_min, window_max, light_level,
                                        &tmap_info,
                                        &floor_params, &ceil_params,
                                        x1_pegged, x2_pegged);
                } else {
                    draw_wall(x1, x1_ytop, x1_ybot, x2, x2_ytop, x2_ybot,
                                trans_v1_z_fix, trans_v2_z_fix,
                                z_recip_v1, z_recip_v2,
                                window_min, window_max, light_level,
                                &tmap_info,
                                &floor_params, &ceil_params);
                }
            }
        }
        //nwalls++;
        // after this point, draw some sprites :^)
                
        //if (render_forcefield) {
        //    uint16_t col = (RED_IDX<<4)|LIGHT_GREEN_IDX;
        //    draw_forcefield(x1, x2, window_min, window_max, clipping_buffer, col);
        //    free_clip_buffer(clipping_buffer);
        //}
    }


    
    if(objects_in_sect != NULL) {
        object* cur_obj = objects_in_sect;

        while(cur_obj != NULL) {
            object_pos pos = cur_obj->pos;
            //s16 flr_height = get_sector_group_floor_height(sector);

            Vect2D_s16 trans_pos = transform_map_vert_16(fix32ToInt(pos.x), fix32ToInt(pos.y));
            
            u16 z_recip = z_recip_table_16[trans_pos.y>>TRANS_Z_FRAC_BITS];
            
            if(trans_pos.y > 0) {
                
                //s16 left_x = project_and_adjust_x(trans_pos.x-6, z_recip);
                //s16 right_x = project_and_adjust_x(trans_pos.x+6, z_recip);
                object_template type = object_types[cur_obj->object_type];

                s16 left_x = project_and_adjust_x(trans_pos.x-(type.width>>1), z_recip);
                s16 right_x = project_and_adjust_x(trans_pos.x+(type.width>>1), z_recip);

                s16 top_y = project_and_adjust_y_fix(pos.z+type.from_floor_draw_offset+type.height, z_recip);
                s16 bot_y = project_and_adjust_y_fix(pos.z+type.from_floor_draw_offset, z_recip);

                //while(1) { 
                //KLog_S1("trans_pos.y: ", trans_pos.y);
                //KLog_U1("z_recip: ", z_recip);
                //KLog_S1("left_x: ", left_x);
                //KLog_S1("right_x: ", right_x);
                //KLog_S1("top_y: ", top_y);
                //KLog_S1("bot_y: ", bot_y);
                //}

                

                //draw_masked(left_x, right_x, top_y, bot_y,
                //        window_min, window_max,
                //        obj_clip_buf,
                //        type.sprite_col);
                draw_sprite_obj(left_x, right_x, top_y>>4, bot_y>>4,
                                window_min, window_max,
                                obj_clip_buf);

            }
            break;
            //cur_obj = cur_obj->next;
        }
        //KLog_U2("freeing clip buf in sector: ", sector, " id: ", obj_clip_buf->id);
        

        free_clip_buffer(obj_clip_buf);
    }
    sector_visited_cache[sector]++;
}

const u16 col_9_spans[2] = {34,5};
const u8 col_9_texels[5] = {0x44,0x44,0x44,0x44,0x22};
const u16 col_10_spans[2] = {32,8};
const u8 col_10_texels[8] = {0x44,0x44,0x44,0x44,0x22,0x11,0x22,0x77};
const u16 col_11_spans[2] = {31,10};
const u8 col_11_texels[10] = {0x44,0x44,0x44,0x44,0x22,0x33,0x33,0x11,0x22,0xaa};     
const u16 col_12_spans[2] = {30,11};
const u8 col_12_texels[11] = {0x44,0x55,0x55,0x55,0x44,0x11,0x11,0x11,0x11,0x22,0xaa};
const u16 col_13_spans[2] = {30,11};
const u8 col_13_texels[11] = {0x55,0xaa,0xaa,0x55,0x44,0x33,0x11,0x11,0x33,0x44,0x77};
const u16 col_14_spans[2] = {29,12};
const u8 col_14_texels[12] = {0x22,0xaa,0xaa,0x55,0x55,0x44,0x11,0x11,0x11,0x11,0x44,0x22};
const u16 col_15_spans[2] = {28,13};
const u8 col_15_texels[13] = {0x44,0x22,0x55,0x55,0x44,0x44,0x22,0x11,0x33,0x33,0x11,0x44,0x22};
const u16 col_16_spans[2] = {27,13};
const u8 col_16_texels[13] = {0x44,0x44,0x22,0x44,0x44,0x44,0x22,0x22,0x22,0x11,0x11,0x44,0x22};
const u16 col_17_spans[4] = {21,5,1,15};
const u8 col_17_texels[20] = {0x44,0x44,0x22,0x22,0x77,0x44,0x44,0x22,0x22,0x22,0x22,0x44,0xaa,0x77,0x44,0x22,0x22,0x22,0x22,0x22};
const u16 col_18_spans[4] = {20,14,8,1};
const u8 col_18_texels[15] = {0x44,0x44,0x44,0x44,0x22,0x44,0x77,0x44,0x22,0xaa,0x22,0x44,0x44,0x22,0x55};
const u16 col_19_spans[4] = {8,22,12,1};
const u8 col_19_texels[23] = {0x55,0x88,0x88,0x55,0x88,0x55,0x55,0x55,0x55,0x55,0x44,0x44,0xaa,0x55,0x44,0x44,0x44,0x22,0x44,0x22,0x77,0x22,0x55};
const u16 col_20_spans[4] = {7,22,13,1};
const u8 col_20_texels[23] = {0x55,0xaa,0xaa,0xaa,0x88,0xaa,0x88,0x55,0xaa,0x55,0x55,0x55,0x44,0x55,0x55,0x44,0x44,0x44,0x22,0x22,0x22,0x44,0xaa};
const u16 col_21_spans[2] = {6,36};
const u8 col_21_texels[36] = {0x55,0xaa,0xff,0xff,0xaa,0xaa,0x88,0x55,0x88,0x55,0x88,0xaa,0x55,0x44,0x55,0x44,0x44,0x99,0xbb,0xbb,0x99,0x44,0x44,0x22,0x22,0x22,0x22,0x22,0x22,0x77,0x77,0x55,0x55,0xaa,0xaa,0xaa};
const u16 col_22_spans[2] = {6,24};
const u8 col_22_texels[24] = {0xaa,0xff,0xff,0xff,0xaa,0xaa,0x88,0xaa,0x55,0x88,0x55,0x55,0x44,0x44,0x44,0x44,0x99,0xbb,0xbb,0xbb,0xbb,0x99,0x44,0x44};
const u16 col_23_spans[4] = {6,28,9,3};
const u8 col_23_texels[31] = {0xaa,0xff,0xff,0xaa,0xaa,0x88,0x88,0x88,0x88,0x55,0x88,0x55,0x44,0x44,0x44,0x99,0xbb,0xdd,0xbb,0xbb,0xbb,0x99,0x99,0x44,0x22,0x44,0x44,0x22,0xcc,0xcc,0x11};
const u16 col_24_spans[8] = {6,29,2,5,1,3,4,5};
const u8 col_24_texels[42] = {0x55,0xaa,0xaa,0xaa,0x88,0xaa,0x88,0x88,0x88,0xaa,0x88,0x55,0x44,0x11,0x99,0x99,0xdd,0x99,0x44,0xff,0xbb,0xbb,0x99,0x44,0x22,0xaa,0x55,0x44,0x22,0x22,0x22,0x22,0x22,0x22,0xcc,0x11,0x11,0x44,0x77,0x77,0x77,0x44};
const u16 col_25_spans[4] = {7,39,2,9};
const u8 col_25_texels[48] = {0x55,0xaa,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x55,0x44,0x11,0x11,0xbb,0x99,0xff,0x44,0x99,0xff,0xbb,0xbb,0x99,0x44,0x22,0x22,0x22,0x22,0x44,0x22,0x44,0x22,0x44,0x22,0x11,0x22,0x22,0x11,0x11,0xcc,0x44,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x44};      
const u16 col_26_spans[6] = {7,1,1,37,1,12};
const u8 col_26_texels[50] = {0x66,0x55,0xbb,0xbb,0xbb,0xbb,0xbb,0x99,0x55,0x11,0x11,0x99,0xbb,0x99,0xff,0x44,0xbb,0xbb,0xbb,0xbb,0x99,0x44,0x44,0x44,0x44,0x44,0x22,0x22,0x55,0x22,0x44,0x22,0x11,0x22,0x22,0x11,0xcc,0xcc,0x44,0x77,0x77,0xaa,0xaa,0xaa,0xaa,0x77,0x77,0x77,0x77,0x44};
const u16 col_27_spans[4] = {6,1,2,51};
const u8 col_27_texels[52] = {0x66,0xbb,0xdd,0xdd,0xdd,0x99,0xbb,0xbb,0x44,0x99,0x44,0xbb,0xbb,0x44,0x44,0x44,0x44,0x44,0xff,0x99,0x44,0x44,0x55,0x44,0x44,0x55,0xff,0xaa,0xaa,0x55,0x44,0x77,0x11,0x22,0x22,0xcc,0xcc,0x11,0x44,0x77,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0x77,0x77,0x44};
const u16 col_28_spans[2] = {3,57};
const u8 col_28_texels[57] = {0x66,0x66,0x66,0x66,0x66,0x99,0xdd,0xff,0xff,0xdd,0x99,0xbb,0x55,0x44,0x55,0x99,0x77,0x99,0xbb,0xdd,0x44,0xbb,0x99,0x99,0x44,0x44,0x55,0xaa,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x77,0x11,0x11,0x77,0xcc,0x11,0x11,0x77,0xaa,0xaa,0xaa,0xff,0xff,0xff,0xff,0xff,0xaa,0xaa,0xaa,0x77,0x77};
const u16 col_29_spans[2] = {2,60};
const u8 col_29_texels[60] = {0x66,0xbb,0xcc,0xbb,0x66,0x66,0x66,0x66,0x66,0x66,0xdd,0xff,0xff,0x44,0x44,0x44,0xbb,0x77,0x77,0x99,0xbb,0x44,0x99,0x99,0x22,0x22,0x22,0xaa,0x44,0x22,0x44,0x22,0x44,0x22,0x44,0x22,0xaa,0x11,0x11,0x11,0xaa,0x11,0x11,0xcc,0x77,0xaa,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xaa,0xaa,0xaa,0xaa,0x77,0x77};
const u16 col_30_spans[2] = {3,59};
const u8 col_30_texels[59] = {0x66,0xbb,0xcc,0xcc,0xcc,0xbb,0x66,0x66,0x66,0xdd,0xbb,0xbb,0x44,0x44,0x44,0xbb,0x77,0x77,0x99,0xbb,0x44,0x99,0x99,0x22,0x22,0x22,0xff,0x44,0x22,0x55,0x22,0xaa,0x22,0x55,0x22,0xaa,0x11,0x11,0x11,0xaa,0x11,0xcc,0xcc,0x77,0xaa,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xaa,0xaa,0xaa,0xaa,0x77,0x77};
const u16 col_31_spans[2] = {3,57};
const u8 col_31_texels[57] = {0x66,0x66,0x66,0x66,0x66,0x99,0xdd,0xff,0xff,0xdd,0x99,0xbb,0x55,0x44,0x55,0x99,0x77,0x99,0xbb,0xdd,0x44,0xbb,0x99,0x99,0x44,0x44,0x55,0x55,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x77,0x11,0x11,0x77,0xcc,0xcc,0x11,0x77,0xaa,0xaa,0xaa,0xff,0xff,0xff,0xff,0xff,0xaa,0xaa,0xaa,0x77,0x77};
const u16 col_32_spans[6] = {4,1,1,1,2,51};
const u8 col_32_texels[53] = {0x66,0x66,0xbb,0xdd,0xdd,0xdd,0x99,0xbb,0xbb,0x44,0x99,0x44,0xbb,0xbb,0xdd,0xff,0x44,0xff,0xbb,0xbb,0x99,0x44,0x44,0x55,0x44,0x44,0x55,0xff,0xaa,0xaa,0x55,0x44,0x77,0x11,0x22,0x22,0xcc,0x11,0x11,0x44,0x77,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0x77,0x77,0x44};
const u16 col_33_spans[6] = {3,1,6,36,1,12};
const u8 col_33_texels[49] = {0x66,0xbb,0xbb,0xbb,0xbb,0xbb,0x99,0x44,0x11,0x11,0x99,0xbb,0x99,0xff,0x99,0x44,0xff,0xbb,0xbb,0x99,0x44,0x44,0x44,0x44,0x44,0x22,0x22,0x55,0x22,0x44,0x22,0x11,0x22,0x22,0x11,0x11,0xcc,0x44,0x77,0x77,0xaa,0xaa,0xaa,0xaa,0x77,0x77,0x77,0x77,0x44}; 
const u16 col_34_spans[4] = {12,34,2,9};
const u8 col_34_texels[43] = {0x44,0xaa,0x55,0x44,0x22,0x22,0x11,0x11,0xbb,0x99,0xff,0xff,0x99,0x44,0x99,0xff,0x99,0x44,0x22,0x22,0x22,0x22,0x44,0x22,0x44,0x22,0x44,0x22,0x11,0x22,0x22,0x11,0xcc,0xcc,0x44,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x44};
const u16 col_35_spans[8] = {12,23,2,5,1,3,4,5};
const u8 col_35_texels[36] = {0x44,0x55,0xaa,0x55,0x44,0x22,0x22,0x11,0x99,0x99,0xdd,0xff,0xdd,0x99,0x99,0xff,0x99,0x44,0x22,0xaa,0x55,0x44,0x22,0x22,0x22,0x22,0x22,0x22,0xcc,0xcc,0x11,0x44,0x77,0x77,0x77,0x44};
const u16 col_36_spans[4] = {13,21,9,3};
const u8 col_36_texels[24] = {0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x99,0xbb,0xdd,0xbb,0xbb,0xbb,0x99,0x99,0x44,0x22,0x44,0x44,0x22,0xcc,0x11,0x11};
const u16 col_37_spans[2] = {19,11};
const u8 col_37_texels[11] = {0x44,0x44,0x44,0x99,0xbb,0xbb,0xbb,0xbb,0x99,0x44,0x44};
const u16 col_38_spans[2] = {19,10};
const u8 col_38_texels[10] = {0x44,0x55,0x44,0x44,0x99,0xbb,0xbb,0x99,0x44,0x44};
const u16 col_39_spans[2] = {17,12};
const u8 col_39_texels[12] = {0x55,0x44,0x44,0x55,0x55,0x44,0x44,0x44,0x22,0x22,0x77,0x22};
const u16 col_40_spans[2] = {15,16};
const u8 col_40_texels[16] = {0x55,0xaa,0xaa,0x55,0x44,0xaa,0x55,0x44,0x44,0x44,0x22,0x44,0x22,0xaa,0x44,0x22};
const u16 col_41_spans[4] = {20,13,7,2};
const u8 col_41_texels[15] = {0x44,0x44,0x44,0x44,0x22,0x44,0x77,0x44,0x22,0x77,0x22,0x22,0x22,0x22,0x22};
const u16 col_42_spans[6] = {19,7,1,7,5,4};
const u8 col_42_texels[18] = {0x55,0x44,0x44,0x44,0x22,0x22,0x77,0x44,0x44,0x22,0x22,0x44,0x22,0x22,0x22,0x55,0x77,0x22};
const u16 col_43_spans[6] = {19,2,6,8,3,6};
const u8 col_43_texels[16] = {0xaa,0x55,0x44,0x44,0x22,0x44,0x44,0x77,0x22,0x22,0x22,0x44,0x44,0x55,0x44,0x22};
const u16 col_44_spans[6] = {18,2,8,8,1,8};
const u8 col_44_texels[18] = {0xaa,0x55,0x44,0x22,0x55,0x44,0x77,0xaa,0x22,0x22,0x22,0x44,0x44,0x44,0x22,0x22,0x44,0x22};
const u16 col_45_spans[6] = {18,1,10,12,2,2};
const u8 col_45_texels[15] = {0x55,0x55,0x55,0x55,0x44,0x77,0xaa,0x22,0x22,0x22,0xaa,0x44,0x22,0x22,0x44};
const u16 col_46_spans[4] = {29,11,4,1};
const u8 col_46_texels[12] = {0x55,0xaa,0x55,0x55,0x44,0x77,0x22,0x22,0xaa,0x44,0x22,0x22};
const u16 col_47_spans[2] = {29,11};
const u8 col_47_texels[11] = {0x55,0xff,0x55,0x55,0x55,0x22,0x22,0x22,0xff,0x44,0x22};
const u16 col_48_spans[4] = {29,10,6,1};
const u8 col_48_texels[11] = {0x55,0xaa,0xff,0x55,0x22,0x55,0x44,0x22,0xaa,0x22,0x44};
const u16 col_49_spans[4] = {28,11,6,1};
const u8 col_49_texels[12] = {0x88,0x55,0x55,0xaa,0x22,0xff,0xaa,0x77,0x44,0x55,0x22,0x44};
const u16 col_50_spans[4] = {27,12,5,2};
const u8 col_50_texels[14] = {0x88,0xaa,0x55,0x55,0x55,0x22,0xff,0xaa,0xaa,0x77,0x22,0x22,0x44,0x22};
const u16 col_51_spans[4] = {26,14,2,3};
const u8 col_51_texels[17] = {0x88,0xaa,0x55,0x88,0x55,0x44,0x22,0xaa,0x55,0xaa,0x77,0x55,0x44,0x22,0x22,0x44,0x44};
const u16 col_52_spans[4] = {25,6,1,13};
const u8 col_52_texels[19] = {0x88,0xff,0xaa,0x88,0x55,0x88,0x22,0x44,0x22,0x55,0xaa,0x55,0x55,0x44,0x44,0x44,0x55,0x55,0x44};
const u16 col_53_spans[4] = {24,6,3,12};
const u8 col_53_texels[18] = {0x88,0xff,0xaa,0x88,0x55,0x88,0x22,0x22,0x55,0xaa,0xff,0x55,0x55,0x55,0x55,0xaa,0x55,0x22};
const u16 col_54_spans[4] = {24,5,5,10};
const u8 col_54_texels[15] = {0xaa,0xaa,0x88,0x88,0x88,0x22,0x44,0x55,0xaa,0xff,0xff,0xaa,0xaa,0x55,0x44};
const u16 col_55_spans[4] = {24,4,7,9};
const u8 col_55_texels[13] = {0x88,0xaa,0x88,0x88,0x22,0x44,0x55,0xaa,0xaa,0xaa,0x55,0x44,0x22};
const u16 col_56_spans[4] = {25,2,9,7};
const u8 col_56_texels[9] = {0x88,0x88,0x44,0x44,0x44,0x44,0x44,0x44,0x44};
const u16 col_57_spans[2] = {37,5};
const u8 col_57_texels[5] = {0x22,0x44,0x44,0x44,0x22};
const rle_object test_obj = {
.num_columns = 64,
.columns = {
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 1, .spans = col_9_spans, .texels = col_9_texels},
{.num_spans = 1, .spans = col_10_spans, .texels = col_10_texels},
{.num_spans = 1, .spans = col_11_spans, .texels = col_11_texels},
{.num_spans = 1, .spans = col_12_spans, .texels = col_12_texels},
{.num_spans = 1, .spans = col_13_spans, .texels = col_13_texels},
{.num_spans = 1, .spans = col_14_spans, .texels = col_14_texels},
{.num_spans = 1, .spans = col_15_spans, .texels = col_15_texels},
{.num_spans = 1, .spans = col_16_spans, .texels = col_16_texels},
{.num_spans = 2, .spans = col_17_spans, .texels = col_17_texels},
{.num_spans = 2, .spans = col_18_spans, .texels = col_18_texels},
{.num_spans = 2, .spans = col_19_spans, .texels = col_19_texels},
{.num_spans = 2, .spans = col_20_spans, .texels = col_20_texels},
{.num_spans = 1, .spans = col_21_spans, .texels = col_21_texels},
{.num_spans = 1, .spans = col_22_spans, .texels = col_22_texels},
{.num_spans = 2, .spans = col_23_spans, .texels = col_23_texels},
{.num_spans = 4, .spans = col_24_spans, .texels = col_24_texels},
{.num_spans = 2, .spans = col_25_spans, .texels = col_25_texels},
{.num_spans = 3, .spans = col_26_spans, .texels = col_26_texels},
{.num_spans = 2, .spans = col_27_spans, .texels = col_27_texels},
{.num_spans = 1, .spans = col_28_spans, .texels = col_28_texels},
{.num_spans = 1, .spans = col_29_spans, .texels = col_29_texels},
{.num_spans = 1, .spans = col_30_spans, .texels = col_30_texels},
{.num_spans = 1, .spans = col_31_spans, .texels = col_31_texels},
{.num_spans = 3, .spans = col_32_spans, .texels = col_32_texels},
{.num_spans = 3, .spans = col_33_spans, .texels = col_33_texels},
{.num_spans = 2, .spans = col_34_spans, .texels = col_34_texels},
{.num_spans = 4, .spans = col_35_spans, .texels = col_35_texels},
{.num_spans = 2, .spans = col_36_spans, .texels = col_36_texels},
{.num_spans = 1, .spans = col_37_spans, .texels = col_37_texels},
{.num_spans = 1, .spans = col_38_spans, .texels = col_38_texels},
{.num_spans = 1, .spans = col_39_spans, .texels = col_39_texels},
{.num_spans = 1, .spans = col_40_spans, .texels = col_40_texels},
{.num_spans = 2, .spans = col_41_spans, .texels = col_41_texels},
{.num_spans = 3, .spans = col_42_spans, .texels = col_42_texels},
{.num_spans = 3, .spans = col_43_spans, .texels = col_43_texels},
{.num_spans = 3, .spans = col_44_spans, .texels = col_44_texels},
{.num_spans = 3, .spans = col_45_spans, .texels = col_45_texels},
{.num_spans = 2, .spans = col_46_spans, .texels = col_46_texels},
{.num_spans = 1, .spans = col_47_spans, .texels = col_47_texels},
{.num_spans = 2, .spans = col_48_spans, .texels = col_48_texels},
{.num_spans = 2, .spans = col_49_spans, .texels = col_49_texels},
{.num_spans = 2, .spans = col_50_spans, .texels = col_50_texels},
{.num_spans = 2, .spans = col_51_spans, .texels = col_51_texels},
{.num_spans = 2, .spans = col_52_spans, .texels = col_52_texels},
{.num_spans = 2, .spans = col_53_spans, .texels = col_53_texels},
{.num_spans = 2, .spans = col_54_spans, .texels = col_54_texels},
{.num_spans = 2, .spans = col_55_spans, .texels = col_55_texels},
{.num_spans = 2, .spans = col_56_spans, .texels = col_56_texels},
{.num_spans = 1, .spans = col_57_spans, .texels = col_57_texels},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL}
}
};


void draw_sprite_obj(
    s16 left_x, s16 right_x, 
    s16 top_y, s16 bot_y, 
    u16 window_min, u16 window_max,
    clip_buf* obj_clip_buf) {
    
    draw_rle_sprite(left_x, right_x, top_y, bot_y, 
                    window_min, window_max, 
                    obj_clip_buf, &test_obj);
}

/*
int has_clip_buffer_remaining() {
    return (free_clip_buf_stack != NULL);
}

clip_buf* pop_free_clip_buf() {
    clip_buf* ret = free_clip_buf_stack;
    free_clip_buf_stack = free_clip_buf_stack->next;
    return ret;
}

void push_used_clip_buf(clip_buf* buf) {
    clip_buf* old_head = used_clip_buf_stack;
    buf->next = old_head;
    used_clip_buf_stack = buf;
}

void copy_clipping_state_to_buffer() {
    // copy 2 bytes for each column, 128 bytes?
    // copy z buffer, one byte for each column, so 3 bytes

    // z-buffer 1x64 -> 16 longword copies

    // top-y-buffer 1x64 -> 16 longword copies

    // bot-y-buffer 1x64 -> 16 longword copies

    // WAIT

    // all we need it to store clipped columns


}
*/

void pvs_scan(u16 src_sector, s16 window_min, s16 window_max, u32 cur_frame) {
    portal_map* map = cur_portal_map;
    u16 raw_pvs_offset = map->pvs[src_sector<<PVS_SHIFT];
    u16 num_sect_pvs_groups = map->pvs[(src_sector<<PVS_SHIFT)+1];


    //u16 k = 0;
    for(int i = 0; i < num_sect_pvs_groups; i++) {

        u16 sector = map->raw_pvs[raw_pvs_offset++];
        u16 sect_group = sector_group(sector, map);
        
        /*
        if(enemies_in_sect(sector) && !rendered_wall_in_sector(sector, cur_frame)) {
            set_rendered_wall_in_sector(sector, cur_frame);
            if(has_clip_buffer_remaining()) {
                clip_buf* buf = pop_free_clip_buf();
                copy_clipping_state_to_buffer(buf);
                push_used_clip_buf(buf);
            }
        }
        */
        

        s8 light_level = get_sector_group_light_level(sect_group);
        s16 floor_height = get_sector_group_floor_height(sect_group);
        s16 ceil_height = get_sector_group_ceil_height(sect_group);
        s16 floor_col = get_sector_group_floor_color(sect_group);
        s16 ceil_col = get_sector_group_ceil_color(sect_group);
        s16 rel_floor_height = (floor_height>>4) - (cur_player_pos.z>>(FIX32_FRAC_BITS));
        s16 rel_ceil_height = (ceil_height>>4) - (cur_player_pos.z>>(FIX32_FRAC_BITS));
        light_params ceil_params, floor_params;
        cache_ceil_light_params(rel_ceil_height, ceil_col, light_level, &ceil_params);
        cache_floor_light_params(rel_floor_height, floor_col, light_level, &floor_params);

        u16 num_walls = map->raw_pvs[raw_pvs_offset++];

        for(int j = 0; j < num_walls; j++) {
            //KLog_U1("wall ", k++);
            u16 wall_idx = map->raw_pvs[raw_pvs_offset++];
            u16 portal_idx = wall_idx - sector;
            s16 portal_sector = map->portals[portal_idx];

            u8 is_portal = (portal_sector != -1);

            u16 v1_idx = map->walls[wall_idx];
            u16 v2_idx = map->walls[wall_idx+1];

            vertex v1 = map->vertexes[v1_idx];
            vertex v2 = map->vertexes[v2_idx];

            Vect2D_s16 trans_v1 = transform_map_vert_16(v1.x, v1.y);
            Vect2D_s16 trans_v2 = transform_map_vert_16(v2.x, v2.y);

            u8 tex_idx = map->wall_colors[(portal_idx<<WALL_COLOR_NUM_PARAMS_SHIFT)+WALL_TEXTURE_IDX];
            u8 is_solid_color = map->wall_colors[(portal_idx<<WALL_COLOR_NUM_PARAMS_SHIFT)+WALL_SOLID_COLOR_IDX];
            u8 wall_col = is_solid_color;
            texmap_params tmap_info = {
                .repetitions = wall_tex_repetitions[portal_idx], 
                .needs_texture = !is_solid_color
            };

            if(!is_solid_color) {

                lit_texture *tex = get_texture(tex_idx, light_level);
                tmap_info.tex = tex;
            }
                     
            clip_result clipped = clip_map_vertex_16(&trans_v1, &trans_v2, &tmap_info);

            if(clipped == OFFSCREEN) {
                continue;
            }

            s16 trans_v1_z_fix = trans_v1.y;
            s16 trans_v2_z_fix = trans_v2.y;
            u16 z_recip_v1 = z_recip_table_16[trans_v1.y>>TRANS_Z_FRAC_BITS];
            u16 z_recip_v2 = z_recip_table_16[trans_v2.y>>TRANS_Z_FRAC_BITS];

            s16 x1 = (clipped & LEFT_FRUSTUM_CLIPPED) ? 0 : project_and_adjust_x(trans_v1.x, z_recip_v1);
            s16 x2 = (clipped & RIGHT_FRUSTUM_CLIPPED) ? RENDER_WIDTH : project_and_adjust_x(trans_v2.x, z_recip_v2);
            //KLog_S2("tx1: ", trans_v1.x, "tx2: ", trans_v2.x);
            //KLog_S2("tz1: ", trans_v1_z_fix>>4, "tz2: ", trans_v2_z_fix>>4);
            //KLog_S2("px1: ", x1, "px2: ", x2);

            if(x1 >= x2) {
                //inc_counter(POST_PROJ_BACKFACE_CULL_COUNTER);
                continue;
            }

            //if(x1 < 0) {
            //    die("frustum clipping failed!!");
            //}
            
            if(x1 > window_max) {
                continue;
            } else if (x2 <= window_min) {
                continue;
            }


            s16 x1_ytop, x1_ybot;
            s16 x2_ytop, x2_ybot;

            x1_ybot = project_and_adjust_y_fix(floor_height, z_recip_v1);
            x2_ybot = project_and_adjust_y_fix(floor_height, z_recip_v2);


            x1_ytop = project_and_adjust_y_fix(ceil_height, z_recip_v1);
            x2_ytop = project_and_adjust_y_fix(ceil_height, z_recip_v2);


 
 

            if(is_portal) {
                u16 neighbor_sector_group = sector_group(portal_sector, map);
                u8 neighbor_sector_group_type = cur_portal_map->sector_group_types[neighbor_sector_group];
                //s16 neighbor_ceil_color = get_sector_group_floor_color(neighbor_sector_group);
                //s16 neighbor_floor_color = get_sector_group_floor_color(neighbor_sector_group);

                s16 neighbor_floor_height = get_sector_group_floor_height(neighbor_sector_group);
                s16 neighbor_ceil_height = get_sector_group_ceil_height(neighbor_sector_group);


                // draw step down from ceiling

                if(neighbor_ceil_height < ceil_height) {

                    s16 nx1_ytop = project_and_adjust_y_fix(neighbor_ceil_height, z_recip_v1);
                    s16 nx2_ytop = project_and_adjust_y_fix(neighbor_ceil_height, z_recip_v2);

                    u8 upper_color = map->wall_colors[(portal_idx<<WALL_COLOR_NUM_PARAMS_SHIFT)+WALL_HIGH_COLOR_IDX];
                    if(neighbor_sector_group_type == DOOR) {

                        s16 orig_door_height = get_sector_group_orig_height(neighbor_sector_group);
                        s16 orig_height_diff = orig_door_height - neighbor_floor_height;
                        s16 x1_pegged = project_and_adjust_y_fix(neighbor_ceil_height+orig_height_diff, z_recip_v1);
                        s16 x2_pegged = project_and_adjust_y_fix(neighbor_ceil_height+orig_height_diff, z_recip_v2);

                        draw_top_pegged_textured_upper_step(x1, x1_ytop, nx1_ytop, x2, x2_ytop, nx2_ytop,
                                                            trans_v1_z_fix, trans_v2_z_fix,
                                                            z_recip_v1, z_recip_v2,
                                                            window_min, window_max, light_level,
                                                            &tmap_info,
                                                            &ceil_params, x1_pegged, x2_pegged);
                    } else {
                        draw_upper_step(x1, x1_ytop, nx1_ytop, x2, x2_ytop, nx2_ytop,
                                        z_recip_v1, z_recip_v2,
                                        window_min, window_max, upper_color, light_level, &ceil_params);

                    }
                } else {
                    draw_ceiling_update_clip(x1, x1_ytop, x2, x2_ytop,
                                            min(z_recip_v1, z_recip_v2),
                                            window_min, window_max, &ceil_params);
                }

                // not sure if this logic is correct
                // if the neighbor's floor is higher than our ceiling we should still draw the lower step, right?
                if(neighbor_floor_height > floor_height) { //} && neighbor_floor_height <= ceil_height) {
                    s16 nx1_ybot = project_and_adjust_y_fix(neighbor_floor_height, z_recip_v1);
                    s16 nx2_ybot = project_and_adjust_y_fix(neighbor_floor_height, z_recip_v2);

                    u8 lower_color = map->wall_colors[(portal_idx<<WALL_COLOR_NUM_PARAMS_SHIFT)+WALL_LOW_COLOR_IDX];
                    if(neighbor_sector_group_type == LIFT) {

                        s16 orig_lift_height = get_sector_group_orig_height(neighbor_sector_group);
                        s16 orig_height_diff = neighbor_ceil_height - orig_lift_height;
                        s16 x1_pegged = project_and_adjust_y_fix(neighbor_floor_height-orig_height_diff, z_recip_v1);
                        s16 x2_pegged = project_and_adjust_y_fix(neighbor_floor_height-orig_height_diff, z_recip_v2);

                        draw_bottom_pegged_textured_lower_step(x1, x1_ybot, nx1_ybot, x2, x2_ybot, nx2_ybot,
                                                            trans_v1_z_fix, trans_v2_z_fix,
                                                            z_recip_v1, z_recip_v2,
                                                            window_min, window_max, light_level,
                                                            &tmap_info,
                                                            &floor_params, x1_pegged, x2_pegged);
                    } else {
                        // draw step from floor
                        draw_lower_step(x1, x1_ybot, nx1_ybot, x2, x2_ybot, nx2_ybot,
                                        z_recip_v1, z_recip_v2,
                                        window_min, window_max, lower_color, light_level, &floor_params);
                    }
                } else {
                    draw_floor_update_clip(x1, x1_ybot, x2, x2_ybot,
                                        min(z_recip_v1, z_recip_v2),
                                        window_min, window_max, &floor_params);
                }


            } else {

                if(is_solid_color) {
                    draw_solid_color_wall(
                        x1, x1_ytop, x1_ybot,
                        x2, x2_ytop, x2_ybot,
                        z_recip_v1, z_recip_v2,
                        window_min, window_max, light_level, 
                        wall_col, &floor_params, &ceil_params);
                } else {
                    draw_wall(
                        x1, x1_ytop, x1_ybot,
                        x2, x2_ytop, x2_ybot,
                        trans_v1_z_fix, trans_v2_z_fix, z_recip_v1, z_recip_v2,
                        window_min, window_max, light_level, 
                        &tmap_info, &floor_params, &ceil_params);

                }

                // update window based on drawn opaque walls for easy out
                if(x1 <= window_min && x2 > window_min) {
                    window_min = x2; //max(0, x2);
                    if(window_min == RENDER_WIDTH) {
                        return;
                    }
                }
                if(x2 >= window_max && x1 < window_max) {
                    window_max = x1; // min(x1, RENDER_WIDTH);
                    if(window_max == 0) {
                        return;
                    }
                }
                if(window_min >= window_max) {
                    return;
                }

            }
        }
    }
}



void portal_rend(u16 src_sector, u32 cur_frame) {
    #ifdef DEBUG_PORTAL_CLIP
    pre_transform_backfacing_walls = 0;
    walls_frustum_culled = 0;
    portals_frustum_culled = 0;
    post_project_backfacing_walls = 0;
    #endif

    #ifdef H32_MODE
        if(cur_portal_map->has_pvs) {
        pvs_scan(src_sector, 0, RENDER_WIDTH-7-1, cur_frame);
        } else {
            visit_graph(src_sector, src_sector, 0, RENDER_WIDTH-7-1, cur_frame, 0);
        }
        //visit_graph(src_sector, src_sector, 7, RENDER_WIDTH-7-1, cur_frame, 0);
    #else
    sectors_scanned = 0;
    //clear_light_cache();

    if(cur_portal_map->has_pvs) {
        pvs_scan(src_sector, 0, RENDER_WIDTH, cur_frame);
    } else {
        visit_graph(src_sector, src_sector, 0, RENDER_WIDTH, cur_frame, 0);
    }
    //KLog_U1("sectors recursively scanned: ", sectors_scanned);
    #endif
    //sectors_scanned = 0;
    //portal_scan(src_sector, 0, RENDER_WIDTH, cur_frame);
    return;
    #ifdef DEBUG_PORTAL_CLIP
    KLog_S1("walls pre-transform backface culled ", pre_transform_backfacing_walls);
    KLog_S1("portals/walls frustum culled ", walls_frustum_culled);
    KLog_S1("portals frustum culled ", portals_frustum_culled);
    KLog_S1("walls post-project backface culled ", post_project_backfacing_walls);
    KLog("-------- frame-end --------");
    #endif

}
