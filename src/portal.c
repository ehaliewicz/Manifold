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
#include "object.h"
#include "portal_map.h"
#include "sector_group.h"
#include "textures.h"
#include "utils.h"
#include "vertex.h"


#define PORTAL_QUEUE_SIZE 32

//static Vect2D_s32* cached_verts;
u8* sector_rendered_cache; // i don't know if this helps much, but we might be able to use it

// num_walls, sector_idx

// left_x, right_x, left_z_fix, right_z_fix, left_u, right_u, portal_sector_idx, EMPTY



void init_portal_renderer() {
    //vert_transform_cache = malloc(sizeof(u32) * cur_portal_map->num_verts, "vertex transform cache");
    sector_rendered_cache = malloc(sizeof(u8) * cur_portal_map->num_sectors, "sector visited cache");
}


void clear_portal_cache() {
    memset(sector_rendered_cache, 0, sizeof(u8) * cur_portal_map->num_sectors);
}

void cleanup_portal_renderer() {
    free(sector_rendered_cache, "sector visited cache");
}


// #define DEBUG_PORTAL_CLIP


#define MAX_DEPTH 32
int nwalls;

int post_project_backfacing_walls;
int walls_frustum_culled;
int pre_transform_backfacing_walls;
int portals_frustum_culled;

int sectors_scanned = 0;

int needs_to_move_right(z_buf_obj j1, z_buf_obj j2) {
    u16 j1_z_recip = j1.z_recip;
    u16 j2_z_recip = j2.z_recip;
    if(j1_z_recip > j2_z_recip) {
        return 1;
    } else if (j1_z_recip == j2_z_recip) {
        s16 j1_height = j1.height;
        s16 j2_height = j1.height;
        if(j1_height > j2_height) {
            return 1;
        }
    }
    return 0;
}


void load_transform_and_duplicate_verts(u16 num_vertexes, u16* indexes, vertex* map_vertexes, Vect2D_s16* out) {
    /* 
        this function will load and transform all vertexes in a sector
        but in backwards order
        this is so that by duplicated them per wall,

        e.g. 
        vertexes 0,1,2,3
        into
        wall vertexes: 0,1, 1,2, 2,3
    
        we don't overwrite any values we need later

        other than that, it's just simple vertex transformations with the transform function manually inlined.
    */

    s16* vert_component_pointer = (s16*)map_vertexes;

    u16* last_index_ptr = indexes + num_vertexes;

    s16 num_walls = (num_vertexes-1);
    s16* last_out_ptr = out + (num_walls*2);

    u16 index = *--last_index_ptr;
    u16 component_index = index+index;
    s16 cur_x = vert_component_pointer[component_index++];
    s16 cur_y = vert_component_pointer[component_index];

    s16 tlx = cur_x - playerXInt;
    s16 tly = cur_y - playerYInt;
    s32 rx = (tlx * angleSin16) - (tly * angleCos16); // 22.10 * 16 -> 22.10
    s32 ry = (tlx * angleCos16) + (tly * angleSin16);
    s16 cur_res_x = rx>>(FIX16_FRAC_BITS);
    s16 cur_res_y = ry>>(FIX16_FRAC_BITS-TRANS_Z_FRAC_BITS); // 12.4


    s16 prev_res_x = cur_res_x;
    s16 prev_res_y = cur_res_y;

    while(num_walls--) {
    //for(int i = 0; i < num_walls; i++) {

        index = *--last_index_ptr;
        component_index = index + index;
        s16* ptr = vert_component_pointer+component_index;


        cur_x = *ptr++;
        cur_y = *ptr++;
        cur_x = vert_component_pointer[component_index++];     
        cur_y = vert_component_pointer[component_index];       

        tlx = cur_x - playerXInt;                              
        tly = cur_y - playerYInt;                              
        rx = (tlx * angleSin16) - (tly * angleCos16);          
        ry = (tlx * angleCos16) + (tly * angleSin16);          
        cur_res_x = rx >> (FIX16_FRAC_BITS);                   
        cur_res_y = ry >> (FIX16_FRAC_BITS-TRANS_Z_FRAC_BITS); 

        __asm volatile(
            "move.w %2, -(%0)\t\n\
            move.w %1, -(%0)\t\n\
            move.w %4, -(%0)\t\n\
            move.w %3, -(%0)\t\n\
            "
            : "+a" (last_out_ptr)
            : "d" (prev_res_x), "d" (prev_res_y), "d" (cur_res_x), "d" (cur_res_y)
        );

        prev_res_x = cur_res_x;
        prev_res_y = cur_res_y;

    }
}

void load_and_transform_pvs_walls(u16 num_walls, u16* pvs_wall_indexes, 
                                  u16* map_walls, vertex* map_vertexes, 
                                  Vect2D_s16* out) {

    s16* vert_component_pointer = (s16*)map_vertexes;

    for(int i = 0; i < num_walls; i++) {
        
        s16 wall_idx = *pvs_wall_indexes++;
        u16 v1_idx = map_walls[wall_idx++];
        u16 v2_idx = map_walls[wall_idx];
        v1_idx += v1_idx;
        v2_idx += v2_idx;
        
        s16 v1x = vert_component_pointer[v1_idx++];
        s16 v1y = vert_component_pointer[v1_idx];

        s16 tlx = v1x - playerXInt; // 16-bit integer
        s16 tly = v1y - playerYInt; // 16-bit integer

        s32 rx = (tlx * angleSin16) - (tly * angleCos16); // 22.10 * 16 -> 22.10
        s32 ry = (tlx * angleCos16) + (tly * angleSin16);
        s16 res_x = rx>>(FIX16_FRAC_BITS);
        s16 res_y = ry>>(FIX16_FRAC_BITS-TRANS_Z_FRAC_BITS); // 12.4

        __asm volatile(
            "move.w %1, (%0)+\t\n\
             move.w %2, (%0)+\t\n\
            "
            : "+a" (out)
            : "d" (res_x), "d" (res_y)
        );

        s16 v2x = vert_component_pointer[v2_idx++];
        s16 v2y = vert_component_pointer[v2_idx];
        tlx = v2x - playerXInt;
        tly = v2y - playerYInt;

        rx = (tlx * angleSin16) - (tly * angleCos16); // 22.10 * 16 -> 22.10
        ry = (tlx * angleCos16) + (tly * angleSin16);
        res_x = rx>>(FIX16_FRAC_BITS);
        res_y = ry>>(FIX16_FRAC_BITS-TRANS_Z_FRAC_BITS); // 12.4

        __asm volatile(
            "move.w %1, (%0)+\t\n\
             move.w %2, (%0)+\t\n\
            "
            : "+a" (out)
            : "d" (res_x), "d" (res_y)
        );
    }
}



//u8 is_portal[62];
//s32 u_buffer[62];
//u8 tex_idx_buffer[62];
//s16 proj_y_buffer[62];
//s16 proj_x_buffer[62];


void visit_graph(u16 src_sector, u16 sector, u16 x1, u16 x2, u32 cur_frame, uint8_t depth) {
    #ifdef DEBUG_PORTAL_CLIP
    KLog_U1("in sector: ", sector);
    #endif 
    if(depth >= MAX_DEPTH ) {    
    #ifdef DEBUG_PORTAL_CLIP
        KLog_U1("bailing out due to depth from sector : ", sector);
    #endif 
        return;
    }
    portal_map* map = cur_portal_map;

    // if this sector has been visited 32 times, or is already being currently rendered, skip it

    if(sector_rendered_cache[sector] & 0b1) { //} & 0b101) { //0x21) {
            return; // Odd = still rendering, 0x20 = give up
    }
    // if we've visited this sector 7 times in one frame (lol)
    if(sector_rendered_cache[sector] >= 14) {
        return;
    }
    // don't revisit the start sector
    // i dont think we'll ever need to do this
    if(sector == src_sector && depth != 0) { return; }

    sector_rendered_cache[sector]++; // = cur_frame;

    u16 window_min = x1; //cur_item.x1;
    u16 window_max = x2; //cur_item.x2;

    s16 wall_offset = sector_wall_offset(sector, map);
    s16 portal_offset = sector_portal_offset(sector, map);
    s16 num_walls = sector_num_walls(sector, map);
    u16 sect_group = sector_group(sector, map);




    s8 light_level = get_sector_group_light_level(sect_group);


    Vect2D_s16 transformed_vert_buffer[62];

    Vect2D_s16* transformed_vert_buffer_ptr = transformed_vert_buffer;
    load_transform_and_duplicate_verts(num_walls+1, &map->walls[wall_offset], map->vertexes, transformed_vert_buffer);
    //Vect2D_s16 prev_transformed_vert = *transformed_vert_buffer_ptr++;

    //duplicate_and_clip_verts(num_walls);


    object_link objects_in_sect = objects_in_sector(sector);
    decoration_link decorations_in_sect = decorations_in_sector(sector);
    int needs_object_clip_buffer = (objects_in_sect != NULL_OBJ_LINK) || (decorations_in_sect != NULL_DEC_LINK);
    

    clip_buf* obj_clip_buf = NULL;
    if(needs_object_clip_buffer) {
        // this breaks bottom clipped rendering somehow LOL
        //KLog_U1("allocating clip buffer in sector: ", sector);
        obj_clip_buf = alloc_clip_buffer();
        //KLog_U2("allocating object clip buffer in sector: ", sector, " id: ", obj_clip_buf->id);
        if(obj_clip_buf != NULL) {
            copy_2d_buffer(window_min, window_max, obj_clip_buf);
        }
    }


    s16 floor_height = get_sector_group_floor_height(sect_group);
    s16 ceil_height = get_sector_group_ceil_height(sect_group);
    s16 rel_floor_height = (floor_height>>4) - (cur_player_pos.z>>(FIX32_FRAC_BITS));
    s16 rel_ceil_height = (ceil_height>>4) - (cur_player_pos.z>>(FIX32_FRAC_BITS));

    u8 floor_color = get_sector_group_floor_color(sect_group);
    u8 ceil_color = get_sector_group_ceil_color(sect_group);

    u8 cur_sect_group_type = GET_SECTOR_GROUP_TYPE(cur_portal_map->sector_group_types[sect_group]);


    light_params ceil_params, floor_params;
    cache_ceil_light_params(rel_ceil_height, ceil_color, light_level, &ceil_params);
    cache_floor_light_params(rel_floor_height, floor_color, light_level, &floor_params);




    for(s16 i = 0; i < num_walls; i++) {

        //const int debug = (sector == 0) && (i == 0);
        // this is plus 1 because we store the number of real walls,
        // but duplicate the first at the end of the wall list per sector, so we don't need modulo math
        //s16 wall_idx = wall_offset+i+1;

        u16 portal_idx = portal_offset+i;

        s16 portal_sector = map->portals[portal_idx];
        int is_portal = portal_sector != -1;



        Vect2D_s16 trans_v1 = *transformed_vert_buffer_ptr++;
        Vect2D_s16 trans_v2 = *transformed_vert_buffer_ptr++;
        #ifdef DEBUG_PORTAL_CLIP
        KLog_S3("wall idx: ", i, " x1: ", trans_v1.x, " x2: ", trans_v2.x);
        #endif 
        
        //Vect2D_s16 trans_v1 = prev_transformed_vert;
        //prev_transformed_vert = trans_v2;



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
        clip_result clipped = clip_map_vertex_16((Vect2D_s16*)&trans_v1, (Vect2D_s16*)&trans_v2, &tmap_info);
        #ifdef DEBUG_PORTAL_CLIP
        KLog_S3("post clip, wall idx: ", i, " x1: ", trans_v1.x, " x2: ", trans_v2.x);
        #endif 


        u16 z_recip_v1 = z_recip_table_16[trans_v1.y>>TRANS_Z_FRAC_BITS];
        u16 z_recip_v2 = z_recip_table_16[trans_v2.y>>TRANS_Z_FRAC_BITS];
        s16 neighbor_floor_height, neighbor_ceil_height;

        if(clipped == OFFSCREEN) {
        #ifdef DEBUG_PORTAL_CLIP
            KLog_U2("in sector: ", sector, " wall idx offscreen: ", i);
        #endif 
            //inc_counter(NEAR_Z_CULL_COUNTER);
            if(sector == src_sector && is_portal) {
                #ifdef DEBUG_PORTAL_CLIP
                if(is_portal) {
                    portals_frustum_culled++;
                    KLog_S2("portal fully clipped: ", portal_idx, " to sect ", portal_sector);
                    KLog_S2("z1: ", trans_v1.y, " z2: ", trans_v2.y);
                }
                #endif
                
                if(trans_v1.y < 0 && trans_v2.y < 0) {  
                #ifdef DEBUG_PORTAL_CLIP
                    KLog("both verts are near clipped skipping this");
                #endif
                    continue;
                }
                if(trans_v1.y < NEAR_Z_FIX) { 
                #ifdef DEBUG_PORTAL_CLIP
                    KLog("left vert is near z clipped, setting it to near z plane");
                #endif
                    trans_v1.y = (1<<2)<<TRANS_Z_FRAC_BITS; z_recip_v1 = 65535;
                }
                if(trans_v2.y < NEAR_Z_FIX) { 
                #ifdef DEBUG_PORTAL_CLIP
                    KLog("right vert is near z clipped, setting it to near z plane");
                #endif
                    trans_v2.y = (1<<2)<<TRANS_Z_FRAC_BITS; z_recip_v2 = 65535;
                 }
                //if(trans_v1.y < NEAR_Z_FIX) { trans_v1.y =  NEAR_Z_FIX; z_recip_v1 = 65535; }
                //if(trans_v2.y < NEAR_Z_FIX) { trans_v2.y = NEAR_Z_FIX; z_recip_v2 = 65535; }
                
                //s16 x1 = (clipped & LEFT_FRUSTUM_CLIPPED) ? 0 : project_and_adjust_x(trans_v1.x, z_recip_v1);
                //s16 x2 = (clipped & RIGHT_FRUSTUM_CLIPPED) ? RENDER_WIDTH : project_and_adjust_x(trans_v2.x, z_recip_v2);
                s16 x1 = project_and_adjust_x(trans_v1.x, z_recip_v1);
                s16 x2 = project_and_adjust_x(trans_v2.x, z_recip_v2);


                if(x1 > window_max) {
                    if(trans_v1.y < 0 && trans_v2.y > 0) {
                        x1 = window_min;
                    } else {
                    #ifdef DEBUG_PORTAL_CLIP
                        walls_frustum_culled++;
                        KLog_S2("portal right frustum culled after near clip?: ", portal_idx, " to sect ", portal_sector);
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
                    KLog_S2("portal left frustum culled after near clip?: ", portal_idx, " to sect ", portal_sector);
                    #endif
                    continue;
                    }
                }
                if(x1 >= x2) { 
                    #ifdef DEBUG_PORTAL_CLIP
                    post_project_backfacing_walls++;
                    KLog_S2("portal backface culled after near clip?: ", portal_idx, " to sect ", portal_sector);
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
        #ifdef DEBUG_PORTAL_CLIP
        KLog_U3("clipped: ", clipped, "left frustum clipped: ", clipped & LEFT_FRUSTUM_CLIPPED, " right frustum clipped: ", clipped & RIGHT_FRUSTUM_CLIPPED);
        #endif
        s16 x1 = clipped & LEFT_FRUSTUM_CLIPPED ? 0 : project_and_adjust_x(trans_v1.x, z_recip_v1);
        s16 x2 = clipped & RIGHT_FRUSTUM_CLIPPED ? RENDER_WIDTH : project_and_adjust_x(trans_v2.x, z_recip_v2);
        #ifdef DEBUG_PORTAL_CLIP
        KLog_S2("px1: ", x1, "px2: ", x2);
        #endif

        s16 beginx = x1;
        if(beginx <= window_min) {
            beginx = window_min;
        }

        s16 endx = x2;
        if(endx >= window_max) {
            endx = window_max;
        }

        if(x1 >= x2) {
            #ifdef DEBUG_PORTAL_CLIP
            if(is_portal) {
                KLog_S2("portal backface culled without being clipped: ", portal_idx, " to sect ", portal_sector);
            } else {
                KLog_S1("wall backface culled without being clipped: ", portal_idx);
            }
            #endif
            //inc_counter(POST_PROJ_BACKFACE_CULL_COUNTER);
            continue;
        }


        if (x1 > window_max) {
            if(is_portal && clipped & LEFT_Z_CLIPPED && x2 >= window_min) {
                visit_graph(src_sector, portal_sector, window_min, endx, cur_frame, depth+1);
            } else {
                #ifdef DEBUG_PORTAL_CLIP 
                if(is_portal) {
                    KLog_S2("x1 is off right hand side for portal idx : ", i, " to sect ", portal_sector);
                } else {
                    KLog_S1("x1 is off right hand side for wall idx : ", i);
                }
                #endif 
            }
            continue;
        } else if (x2 <= window_min) {
            if(is_portal && clipped & RIGHT_Z_CLIPPED && x1 < window_max) {
                visit_graph(src_sector, portal_sector, beginx, window_max, cur_frame, depth+1);
            } else {
                
                #ifdef DEBUG_PORTAL_CLIP 
                if(is_portal) {
                    KLog_S2("x2 is off left hand side for portal idx : ", i, " to sect ", portal_sector);
                } else {
                    KLog_S1("x2 is off left hand side for portal idx : ", i);
                }
                #endif 
            }

            continue;
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

            neighbor_floor_height = neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_FLOOR_HEIGHT_IDX]; //get_sector_group_floor_height(neighbor_sect_group);
            neighbor_ceil_height = neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_CEIL_HEIGHT_IDX]; //get_sector_group_ceil_height(neighbor_sect_group);

            x1_ybot = project_and_adjust_y_fix(floor_height, z_recip_v1);
            x2_ybot = project_and_adjust_y_fix(floor_height, z_recip_v2);

            x1_ytop = project_and_adjust_y_fix(ceil_height, z_recip_v1);
            x2_ytop = project_and_adjust_y_fix(ceil_height, z_recip_v2);

            // this test is iffy here
            // can cause issues with floors/ceilings leaking e.g. medusa effect
            if (neighbor_ceil_height > floor_height && neighbor_floor_height < ceil_height) {
                recur = 1;
            } else {
                #ifdef DEBUG_PORTAL_CLIP 
                KLog_S2("neighbor ceiling height test not passed for portal : ", portal_idx, " to sect ", portal_sector);
                #endif 
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



        if (is_portal) {

            #ifdef DEBUG_PORTAL_CLIP
            KLog_S2("portal in sector: ", sector, " drawn with wall idx: ", i);
            #endif
                
            // draw floor first if necessary, so that lower steps cannot block it from drawing
            // if they go below our current floor
            if(neighbor_floor_height <= floor_height) {
                draw_floor_update_clip(x1, x1_ybot, x2, x2_ybot,
                                    min(z_recip_v1, z_recip_v2),
                                    window_min, window_max, &floor_params);
            }

            u16 nsect_group = sector_group(portal_sector, map);
            u8 neighbor_sector_group_type = GET_SECTOR_GROUP_TYPE(cur_portal_map->sector_group_types[nsect_group]);
            if(neighbor_ceil_height < ceil_height) {

                s16 nx1_ytop = project_and_adjust_y_fix(neighbor_ceil_height, z_recip_v1);
                s16 nx2_ytop = project_and_adjust_y_fix(neighbor_ceil_height, z_recip_v2);

                if(neighbor_sector_group_type == DOOR) {

                    s16 orig_door_height = neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_ORIG_HEIGHT_IDX]; //get_sector_group_orig_height(neighbor_sect_group);
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
                    //continue;
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
                u16 neighbor_ceil_color = neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_CEIL_COLOR_IDX]; //get_sector_group_ceil_color(neighbor_sect_group);
                u16 neighbor_light_level = neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_LIGHT_IDX];
                //if(neighbor_ceil_color != ceil_color || neighbor_ceil_height != ceil_height || !recur || neighbor_light_level != light_level) {
                    draw_ceiling_update_clip(x1, x1_ytop, x2, x2_ytop,
                                            min(z_recip_v1, z_recip_v2),
                                            window_min, window_max, &ceil_params);
                //}
            }

            // not sure if this logic is correct
            // if the neighbor's floor is higher than our ceiling we should still draw the lower step, right?
            if(neighbor_floor_height > floor_height) { //} && neighbor_floor_height <= ceil_height) {
                s16 nx1_ybot = project_and_adjust_y_fix(neighbor_floor_height, z_recip_v1);
                s16 nx2_ybot = project_and_adjust_y_fix(neighbor_floor_height, z_recip_v2);

                if(neighbor_sector_group_type == LIFT) {

                    s16 orig_lift_height = neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_ORIG_HEIGHT_IDX]; //get_sector_group_orig_height(neighbor_sect_group);
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
                //u16 neighbor_floor_color = neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_FLOOR_COLOR_IDX]; //get_sector_group_floor_color(neighbor_sect_group);
                //u16 neighbor_light_level = neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_LIGHT_IDX];
                //if(neighbor_floor_color != floor_color || neighbor_floor_height != floor_height || !recur || neighbor_light_level != light_level) {
                    draw_floor_update_clip(x1, x1_ybot, x2, x2_ybot,
                                        min(z_recip_v1, z_recip_v2),
                                        window_min, window_max, &floor_params);
                //}
            }


            if(recur) {
            #ifdef DEBUG_PORTAL_CLIP
            KLog_S2("recursing through portal in sector: ", sector, " with wall idx: ", i);
            #endif
                visit_graph(src_sector, portal_sector,
                            ((clipped & LEFT_Z_CLIPPED) ? window_min : beginx),
                            ((clipped & RIGHT_Z_CLIPPED) ? window_max : endx),
                            cur_frame, depth+1);

            } else {

            #ifdef DEBUG_PORTAL_CLIP
            KLog_S2("not recursing through portal in sector: ", sector, " with wall idx: ", i);
            #endif
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

    

    
    if(obj_clip_buf != NULL) { //} objects_in_sect != NULL_OBJ_LINK || decorations_in_sect != NULL_DEC_LINK) {
        u8 buf_idx = 0;
        object_link cur_obj = objects_in_sect;
        //KLog_U1("start draw objs in sector", sector);
        while(cur_obj != NULL_OBJ_LINK && buf_idx < OBJ_SORT_BUF_SZ) {
            //KLog("drawing obj in sector");
            f32 obj_x = OBJ_LINK_DEREF(cur_obj).x;
            f32 obj_y = OBJ_LINK_DEREF(cur_obj).y;
            s16 obj_z = OBJ_LINK_DEREF(cur_obj).z;
            //Vect2D_s16 trans_pos = transform_map_vert_16(obj_x, obj_y);

            Vect2D_s16 trans_pos = transform_map_vert_16(fix32ToInt(obj_x), fix32ToInt(obj_y));
            if(trans_pos.y > 0) {
                u16 z_recip = z_recip_table_16[trans_pos.y>>TRANS_Z_FRAC_BITS];
                //z_buf[buf_idx] = &obj_buf[buf_idx];
                u8 obj_type = OBJ_LINK_DEREF(cur_obj).object_type;
                volatile object_template* type = &object_types[obj_type];
                u16 floor_draw_offset = type->from_floor_draw_offset;
                u16 height = type->height;

                s16 ytop = project_and_adjust_y_fix(obj_z+floor_draw_offset+height, z_recip);
                s16 ybot = project_and_adjust_y_fix(obj_z+floor_draw_offset, z_recip);

                obj_sort_buf[buf_idx].obj_type = obj_type;
                obj_sort_buf[buf_idx].x = trans_pos.x;
                obj_sort_buf[buf_idx].ytop = ytop;
                obj_sort_buf[buf_idx].ybot = ybot;
                obj_sort_buf[buf_idx].obj_link = cur_obj;
                //obj_sort_buf[buf_idx].obj_type = OBJECT;

                z_sort_buf[buf_idx].buf_idx = buf_idx;
                z_sort_buf[buf_idx].height = ybot-ytop;
                z_sort_buf[buf_idx].z_recip = z_recip;
                buf_idx++;

            }
            object_link nxt = OBJ_LINK_DEREF(cur_obj).next;
            
            //if(cur_dec == nxt) {
            //    die("infinite loop!");
            //}

            cur_obj = nxt;
        }

        decoration_link cur_dec = decorations_in_sect;
        while(cur_dec != NULL_DEC_LINK && buf_idx < OBJ_SORT_BUF_SZ) {
            s16 obj_x = DEC_LINK_DEREF(cur_dec).x;
            s16 obj_y = DEC_LINK_DEREF(cur_dec).y;
            s16 obj_z = DEC_LINK_DEREF(cur_dec).z;

            Vect2D_s16 trans_pos = transform_map_vert_16(obj_x, obj_y);
            if(trans_pos.y > 0) {
                u16 z_recip = z_recip_table_16[trans_pos.y>>TRANS_Z_FRAC_BITS];
                //z_buf[buf_idx] = &obj_buf[buf_idx];
                u8 obj_type = DEC_LINK_DEREF(cur_dec).object_type;
                volatile object_template* type = &object_types[obj_type];
                u16 floor_draw_offset = type->from_floor_draw_offset;
                u16 height = type->height;

                s16 ytop = project_and_adjust_y_fix(obj_z+floor_draw_offset+height, z_recip);
                s16 ybot = project_and_adjust_y_fix(obj_z+floor_draw_offset, z_recip);

                obj_sort_buf[buf_idx].obj_type = obj_type;
                obj_sort_buf[buf_idx].x = trans_pos.x;
                obj_sort_buf[buf_idx].ytop = ytop;
                obj_sort_buf[buf_idx].ybot = ybot;
                obj_sort_buf[buf_idx].obj_link = cur_dec;
                //obj_sort_buf[buf_idx].obj_type = DECORATION;
                //KLog_U1("decoration slot", obj_type);

                z_sort_buf[buf_idx].buf_idx = buf_idx;
                z_sort_buf[buf_idx].height = ybot-ytop;
                z_sort_buf[buf_idx].z_recip = z_recip;
                buf_idx++;

            }
            decoration_link nxt = DEC_LINK_DEREF(cur_dec).next;
            
            //if(cur_dec == nxt) {
            //    die("infinite loop!");
            //}

            cur_dec = nxt;

        }

        for (int gap = buf_idx/2; gap > 0; gap /= 2) {
            for (int i = gap; i < buf_idx; i += 1) {
            z_buf_obj temp = z_sort_buf[i];
  
            int j;            
            for (j = i; j >= gap && needs_to_move_right(z_sort_buf[j - gap], temp); j -= gap) {
                z_sort_buf[j] = z_sort_buf[j - gap];
            }              
            //  put temp (the original a[i]) in its correct location
            z_sort_buf[j] = temp;
            }   
        }
        for(int i = 0; i < buf_idx; i++) {
            u8 obj_buf_idx = z_sort_buf[i].buf_idx;
            u16 z_recip = z_sort_buf[i].z_recip;
            u8 cur_obj_type = obj_sort_buf[obj_buf_idx].obj_type;
            s16 x = obj_sort_buf[obj_buf_idx].x;
            s16 top_y = obj_sort_buf[obj_buf_idx].ytop;
            s16 bot_y = obj_sort_buf[obj_buf_idx].ybot;

            //u16 z_recip = obj_buf[i].z_recip;
            volatile object_template* type = &object_types[cur_obj_type];
            u16 width = type->width;
            u16 half_width = width>>2; // width needs to be pre-halved because we have double pixels, so it's actually a quarter here :)

            s16 left_x = project_and_adjust_x(x-half_width, z_recip);
            s16 right_x = project_and_adjust_x(x+half_width, z_recip);
            //u16 floor_draw_offset = type->from_floor_draw_offset;
            //s16 top_y = project_and_adjust_y_fix(pos.z+floor_draw_offset+height, z_recip);
            //s16 bot_y = project_and_adjust_y_fix(pos.z+floor_draw_offset, z_recip);

            //KLog_U1("obj sprite pointer: ", type->sprite);
            u8 obj_link = obj_sort_buf[obj_buf_idx].obj_link;
            draw_rle_sprite(left_x, right_x, top_y>>4, bot_y>>4, 
                            window_min, window_max, 
                            obj_clip_buf, type->sprite,
                            obj_link, type->init_state == IDLE_STATE ? DECORATION : OBJECT, sector);
        }
        
        free_clip_buffer(obj_clip_buf);
    }
    
    sector_rendered_cache[sector]++;
}


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

        Vect2D_s16 transformed_vertex_buffer[62];
        load_and_transform_pvs_walls(num_walls, 
                                     &map->raw_pvs[raw_pvs_offset],
                                     map->walls, map->vertexes, 
                                     transformed_vertex_buffer);


        Vect2D_s16* transformed_vertex_ptr = transformed_vertex_buffer;
        for(int j = 0; j < num_walls; j++) {
            //KLog_U1("wall ", k++);
            u16 wall_idx = map->raw_pvs[raw_pvs_offset++];
            u16 portal_idx = wall_idx - sector;
            s16 portal_sector = map->portals[portal_idx];

            u8 is_portal = (portal_sector != -1);

            //u16 v1_idx = map->walls[wall_idx];
            //u16 v2_idx = map->walls[wall_idx+1];

            //vertex v1 = map->vertexes[v1_idx];
            //vertex v2 = map->vertexes[v2_idx];

            //Vect2D_s16 trans_v1 = transform_map_vert_16(v1.x, v1.y);
            //Vect2D_s16 trans_v2 = transform_map_vert_16(v2.x, v2.y);

            Vect2D_s16 trans_v1 = *transformed_vertex_ptr++;
            Vect2D_s16 trans_v2 = *transformed_vertex_ptr++;
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
                u8 neighbor_sector_group_type = GET_SECTOR_GROUP_TYPE(cur_portal_map->sector_group_types[neighbor_sector_group]);
                //s16 neighbor_ceil_color = get_sector_group_floor_color(neighbor_sector_group);
                //s16 neighbor_floor_color = get_sector_group_floor_color(neighbor_sector_group);

                s16 neighbor_floor_height = get_sector_group_floor_height(neighbor_sector_group);
                s16 neighbor_ceil_height = get_sector_group_ceil_height(neighbor_sector_group);


                // draw floor first if necessary, so that lower steps cannot block it from drawing
                // if they go below our current floor
                if(neighbor_floor_height <= floor_height) {
                    draw_floor_update_clip(x1, x1_ybot, x2, x2_ybot,
                                        min(z_recip_v1, z_recip_v2),
                                        window_min, window_max, &floor_params);
                }

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
                    //draw_floor_update_clip(x1, x1_ybot, x2, x2_ybot,
                    //                    min(z_recip_v1, z_recip_v2),
                    //                    window_min, window_max, &floor_params);
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
        pvs_scan(src_sector, 0, RENDER_WIDTH-1, cur_frame);
        } else {
            visit_graph(src_sector, src_sector, 2, RENDER_WIDTH-2, cur_frame, 0);
        }
        //visit_graph(src_sector, src_sector, 7, RENDER_WIDTH-7-1, cur_frame, 0);
    #else
    sectors_scanned = 0;
    //clear_light_cache();

    if(cur_portal_map->has_pvs) {
        pvs_scan(src_sector, 0, RENDER_WIDTH, cur_frame);
    } else {
        //KLog_U1("pvs offsets: ", cur_portal_map->pvs_offsets);
        //if(cur_portal_map->pvs_offsets == NULL) {
            //KLog("not using pvs");

            visit_graph(src_sector, src_sector, 0, RENDER_WIDTH, cur_frame, 0);


        //} else {
        //    u16 pvs_offset = cur_portal_map->pvs_offsets[src_sector];
        //    s16* pvs = &cur_portal_map->sector_list_offsets[pvs_offset];
        //    //KLog_U1("pvs base ptr: ", pvs);
        //    //KLog_U1("sector 0 in pvs: ", pvs);
        //    visit_graph_with_pvs(pvs, src_sector, src_sector, 0, RENDER_WIDTH, cur_frame, 0);
        //}
    }
    //KLog_U1("sectors recursively scanned: ", sectors_scanned);
    #endif
    //sectors_scanned = 0;
    //portal_scan(src_sector, 0, RENDER_WIDTH, cur_frame);
    #ifdef DEBUG_PORTAL_CLIP
    KLog("Rendered.");
    #endif
    return;
    #ifdef DEBUG_PORTAL_CLIP
    KLog_S1("walls pre-transform backface culled ", pre_transform_backfacing_walls);
    KLog_S1("portals/walls frustum culled ", walls_frustum_culled);
    KLog_S1("portals frustum culled ", portals_frustum_culled);
    KLog_S1("walls post-project backface culled ", post_project_backfacing_walls);
    KLog("-------- frame-end --------");
    #endif

}
