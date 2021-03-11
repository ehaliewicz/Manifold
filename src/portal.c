#include <genesis.h>
#include <kdebug.h>
#include "colors.h"
#include "common.h"
#include "draw.h"
#include "game.h"
#include "level.h"
#include "math3D.h"
#include "portal_map.h"
#include "portal_maps.h"
#include "sector.h"
#include "vertex.h"


#define PORTAL_QUEUE_SIZE 32

//static Vect2D_s32* cached_verts;
static u32* sector_visited_cache; // i don't know if this helps much, but we might be able to use it

typedef struct {
    s16 x1, x2;
    s16 sector;
} render_sector_item;

static render_sector_item portal_queue[PORTAL_QUEUE_SIZE];
static u8 queue_read;
static u8 queue_write;

static u8 mask(u8 idx) { return idx & (PORTAL_QUEUE_SIZE-1); }

static void queue_push(render_sector_item i) { 
    portal_queue[mask(queue_write++)] = i;
}

static render_sector_item queue_pop() {
    return portal_queue[mask(queue_read++)];
}

static int queue_size()     { return queue_write - queue_read; }
static int queue_empty()    { return queue_size() == 0 ; }
static int queue_full()     { return queue_size() == PORTAL_QUEUE_SIZE; }

u8* min_max_visible_wall_idx_cache;

void init_portal_renderer() {
    //vert_transform_cache = MEM_alloc(sizeof(u32) * cur_portal_map->num_verts);
    queue_read = 0;
    queue_write = 0;

    sector_visited_cache = MEM_alloc(sizeof(u32) * cur_portal_map->num_sectors);   
    min_max_visible_wall_idx_cache = MEM_alloc(sizeof(u8) * cur_portal_map->num_sectors*2);
    for(int sector = 0; sector < cur_portal_map->num_sectors; sector++) {
        int idx = sector<<1;
        min_max_visible_wall_idx_cache[idx] = 0;
        min_max_visible_wall_idx_cache[idx+1] = sector_num_walls(sector, (portal_map*)cur_portal_map);
    }
}

void clear_portal_cache() {
    //memset(sector_visited_cache, 0, sizeof(u32) * cur_portal_map->num_sectors);
}

void cleanup_portal_renderer() {
    MEM_free(sector_visited_cache);
    MEM_free(min_max_visible_wall_idx_cache);
}


typedef enum {
    PORTAL_TOP_VISIBLE,
    PORTAL_BOT_VISIBLE,
    PORTAL_TOP_AND_BOT_VISIBLE,
    PORTAL_NO_STEP_VISIBLE,
    WALL_VISIBLE,
} draw_types;

render_sector_item queue_item;

//#define DEBUG_PORTAL_CLIP

void add_new_sector_to_queue(s16 new_x1, s16 new_x2, s16 sect) {
    //if(sector_visited_cache[sect] == cur_frame) { //} & 0b101) { //0x21) {
    //        return;
    //        //continue; // Odd = still rendering, 0x20 = give up
    //}

    queue_item.x1 = new_x1;
    queue_item.x2 = new_x2;

    queue_item.sector = sect;

    #ifdef DEBUG_PORTAL_CLIP
    KLog_S3("Portal to sector ", sect, " enqueued with frustum l:", new_x1, " r: ", new_x2);
    #endif
    queue_push(queue_item);
}


void portal_rend(u16 src_sector, u32 cur_frame) {
    
    #ifdef DEBUG_PORTAL_CLIP
        KLog_S3("player x: ", cur_player_pos.x, " y: ", cur_player_pos.y, " ang: ", cur_player_pos.ang);
        KLog_S1("cur sect: ", cur_player_pos.cur_sector);
    #endif

    portal_map* map = (portal_map*)cur_portal_map;

    //render_sector_item queue_item = { .x1 = 0, .x2 = SCREEN_WIDTH-1, .sector = src_sector };
    render_sector_item queue_item = { .x1 = 0, .x2 = 64-1, .sector = src_sector };

    queue_push(queue_item);

    int visited = 0;
    int pre_transform_backfacing_walls = 0;
    int post_project_backfacing_walls = 0;
    int walls_frustum_culled = 0;

    #ifdef DEBUG_PORTAL_CLIP
        KLog_U1("start frame: ", cur_frame);
    #endif
    //KLog_U1("start frame: ", cur_frame);
    
    int visited_sects[32];
    while(!queue_empty()) {
        render_sector_item cur_item = queue_pop();

        s16 sector = cur_item.sector;

        #ifdef DEBUG_PORTAL_CLIP
            KLog_U1("processing sector: ", sector);
        #endif
        visited++;

        // if this sector has been visited 32 times, or is already being currently rendered, skip it
        if(sector_visited_cache[sector] == cur_frame) { //} & 0b101) { //0x21) {
             continue; // Odd = still rendering, 0x20 = give up
        }

        sector_visited_cache[sector] = cur_frame;
        


        u16 window_min = cur_item.x1;
        u16 window_max = cur_item.x2;

        s16 wall_offset = sector_wall_offset(sector, map);
        s16 portal_offset = sector_portal_offset(sector, map);
        s16 num_walls = sector_num_walls(sector, map);

        s16 floor_height = sector_floor_height(sector, map); 
        s16 ceil_height = sector_ceil_height(sector, map);

        u8 floor_color = sector_floor_color(sector, map);
        u8 ceil_color = sector_ceil_color(sector, map);

        Vect2D_f32 sector_center = sector_centers[sector];
        u32 avg_dist = (
            abs(fix32ToInt(sector_center.x - cur_player_pos.x))+
            abs(fix32ToInt(sector_center.y - cur_player_pos.y)));

        s8 light_level = get_sector_light_level(sector);


        floor_color = calculate_color(floor_color, avg_dist, light_level);
        ceil_color = calculate_color(ceil_color, avg_dist, light_level);
        
        u16 init_v1_idx = map->walls[wall_offset];
        vertex init_v1 = map->vertexes[init_v1_idx];
        Vect2D_s16 prev_transformed_vert = transform_map_vert_16(init_v1.x, init_v1.y);
        
        s16 last_frontfacing_wall = -1;
        
        u16 prev_v2_idx = init_v1_idx;

        s16 intPx = fix32ToInt(cur_player_pos.x);
        s16 intPy = fix32ToInt(cur_player_pos.y);
        for(s16 i = 0; i < num_walls; i++) {
            s16 wall_idx = wall_offset+i+1;

            u16 portal_idx = portal_offset+i;
            s16 portal_sector = map->portals[portal_idx];
            int is_portal = portal_sector != -1;

	        u16 v2_idx = map->walls[wall_idx];

            vertex v1 = map->vertexes[prev_v2_idx];
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
            
            u8 wall_color = map->wall_colors[portal_idx].mid_col;
            
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
                //pre_transform_backfacing_walls++;
                #ifdef DEBUG_PORTAL_CLIP
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
            s16 neighbor_floor_height;
            s16 neighbor_ceil_height;
            if(clipped == OFFSCREEN) {   
                if(sector == src_sector && is_portal) {
                    #ifdef DEBUG_PORTAL_CLIP
                    if(is_portal) {
                        KLog_S1("portal fully near clipped: ", portal_idx);
                        KLog_S2("z1: ", trans_v1.y, " z2: ", trans_v2.y);
                    }
                    #endif
                    if(trans_v1.y == 0) { trans_v1.y = 1; }
                    if(trans_v2.y == 0) { trans_v2.y = 1; }
                    if(trans_v1.y < 0 && trans_v2.y < 0) { continue; }
                    s16 x1 = project_and_adjust_x(trans_v1.x, trans_v1.y);
                    s16 x2 = project_and_adjust_x(trans_v2.x, trans_v2.y);

                    if(x1 > window_max) { 
                        if(trans_v1.y < 0 && trans_v2.y > 0) {
                            x1 = window_min;
                        } else {
                        #ifdef DEBUG_PORTAL_CLIP
                            KLog_S1("portal right frustum culled after near clip?: ", portal_idx);
                            KLog_S2("left: ", x1, " right: ", x2);
                        #endif
                        //walls_frustum_culled++; 
                        continue;
                        }
                    }
                    if(x2 <= window_min) {
                        if(trans_v2.y < 0 && trans_v1.y > 0) {
                            //goto submit_anyway;
                            x2 = window_max;
                        } else {
                        #ifdef DEBUG_PORTAL_CLIP
                        KLog_S1("portal left frustum culled after near clip?: ", portal_idx);
                        #endif
                            //walls_frustum_culled++; 
                            continue;
                        }
                    }
                    if(x1 >= x2) { 
                        #ifdef DEBUG_PORTAL_CLIP
                        KLog_S1("portal backface culled after near clip?: ", portal_idx);
                        #endif
                        //post_project_backfacing_walls++; 
                        continue; 
                    }
                    neighbor_floor_height = sector_floor_height(portal_sector, cur_portal_map);
                    neighbor_ceil_height = sector_ceil_height(portal_sector, cur_portal_map);
                    if (neighbor_ceil_height > floor_height && neighbor_floor_height < ceil_height && !queue_full()) {
                        add_new_sector_to_queue(window_min, window_max, portal_sector);
                    }
                    
                }
                
                continue;
            }
            s16 trans_v1_z = trans_v1.y;
            s16 trans_v2_z = trans_v2.y;
            s16 x1 = project_and_adjust_x(trans_v1.x, trans_v1_z);
            s16 x2 = project_and_adjust_x(trans_v2.x, trans_v2_z);
            s16 beginx = max(x1, window_min);
            s16 endx = min(x2, window_max);
            
            if (x1 > window_max) {
                #ifdef DEBUG_PORTAL_CLIP
                if(is_portal) {
                    KLog_S1("portal right frustum culled: ", portal_idx);
                }
                #endif
                if(is_portal && clipped == LEFT_CLIPPED && x2 >= window_min) {
                    #ifdef DEBUG_PORTAL_CLIP
                        KLog_S1("portal right frustum culled but still enqueued: ", portal_idx);
                    #endif
                    add_new_sector_to_queue(window_min, endx, portal_sector);
                }
                continue;
                //walls_frustum_culled++; 
            } else if (x2 <= window_min) { 
                #ifdef DEBUG_PORTAL_CLIP
                    if(is_portal) {
                        KLog_S1("portal left frustum culled: ", portal_idx);
                    }
                #endif
                if(is_portal && clipped == RIGHT_CLIPPED && x1 < window_max) {
                    #ifdef DEBUG_PORTAL_CLIP
                        KLog_S2("portal left frustum culled but still enqueued: ", portal_idx, " to sector: ", portal_sector);
                    #endif
                    add_new_sector_to_queue(beginx, window_max, portal_sector);
                }

                //walls_frustum_culled++; 
                continue; 
            } else {
                #ifdef DEBUG_PORTAL_CLIP
                if(is_portal) {
                    KLog_S2("portal not frustum culled: ", portal_idx, " to sector: ", portal_sector);
                }
                #endif
            }
            
            if(x1 >= x2) { 
                post_project_backfacing_walls++; 
                #ifdef DEBUG_PORTAL_CLIP
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


            if(is_portal) {
                neighbor_floor_height = sector_floor_height(portal_sector, cur_portal_map);
                neighbor_ceil_height = sector_ceil_height(portal_sector, cur_portal_map);
                if (neighbor_ceil_height > floor_height && neighbor_floor_height < ceil_height && !queue_full()) {

                    add_new_sector_to_queue(((clipped == LEFT_CLIPPED) ? window_min : beginx), 
                                            ((clipped == RIGHT_CLIPPED) ? window_max : endx), 
                                            portal_sector);
                }
                
            }


            s16 x1_ytop = project_and_adjust_y_fix(ceil_height, trans_v1_z);
            s16 x1_ybot = project_and_adjust_y_fix(floor_height, trans_v1_z);
            s16 x2_ytop = project_and_adjust_y_fix(ceil_height, trans_v2_z);
            s16 x2_ybot = project_and_adjust_y_fix(floor_height, trans_v2_z);
            

            // draw floor and ceiling
            // check if floor and ceiling are on-screen

            wall_color = calculate_color(wall_color, avg_dist, light_level);

            if (is_portal) {
                #ifdef DEBUG_PORTAL_CLIP
                KLog_S2("portal in sector: ", sector, " drawn with wall idx: ", i);
                #endif
                // draw step down from ceiling
                if(neighbor_ceil_height < ceil_height && neighbor_ceil_height > floor_height) {
                    u8 upper_color = map->wall_colors[portal_idx].upper_col;
                    upper_color = calculate_color(upper_color, avg_dist, light_level);
                    // draw step from ceiling
                    
                    s16 nx1_ytop = project_and_adjust_y_fix(neighbor_ceil_height, trans_v1_z);
                    s16 nx2_ytop = project_and_adjust_y_fix(neighbor_ceil_height, trans_v2_z);

                    draw_upper_step(x1, x1_ytop, nx1_ytop, x2, x2_ytop, nx2_ytop, window_min, window_max, upper_color, ceil_color);
                } else {
                    draw_ceiling_update_clip(x1, x1_ytop, x2, x2_ytop, window_min, window_max, ceil_color);
                }


                if(neighbor_floor_height > floor_height && neighbor_floor_height < ceil_height) {
                    u8 lower_color = map->wall_colors[portal_idx].lower_col;
                    lower_color = calculate_color(lower_color, avg_dist, light_level);

                    // draw step from floor
                    s16 nx1_ybot = project_and_adjust_y_fix(neighbor_floor_height, trans_v1_z);
                    s16 nx2_ybot = project_and_adjust_y_fix(neighbor_floor_height, trans_v2_z);

                    draw_lower_step(x1, x1_ybot, nx1_ybot, x2, x2_ybot, nx2_ybot, window_min, window_max, lower_color, floor_color);
                } else {
                    draw_floor_update_clip(x1, x1_ybot, x2, x2_ybot, window_min, window_max, floor_color);
                }

            } else { 
                #ifdef DEBUG_PORTAL_CLIP
                KLog_S2("wall in sector: ", sector, " drawn with wall idx: ", i);
                #endif
                draw_wall(x1, x1_ytop, x1_ybot, x2, x2_ytop, x2_ybot, window_min, window_max, ceil_color, wall_color, floor_color);
            }
        }
    }
    #ifdef DEBUG_PORTAL_CLIP
    KLog_U1("VISITED SECTORS: ", visited);
    #endif
}

/*
void portal_render_temporal_locality(u16 src_sector, u32 cur_frame) {
    portal_map* map = cur_portal_map;

    //render_sector_item queue_item = { .x1 = 0, .x2 = SCREEN_WIDTH-1, .sector = src_sector };
    render_sector_item queue_item = { .x1 = 0, .x2 = 64-1, .sector = src_sector };

    queue_push(queue_item);

    int visited = 0;
    int pre_transform_backfacing_walls = 0;
    int post_project_backfacing_walls = 0;
    int walls_frustum_culled = 0;

    while(!queue_empty()) {
        visited++;
        render_sector_item cur_item = queue_pop();

        s16 sector = cur_item.sector;

        // if this sector has been visited 32 times, or is already being currently rendered, skip it
        if(sector_visited_cache[sector] == cur_frame) { //} & 0b101) { //0x21) {
             continue; // Odd = still rendering, 0x20 = give up
        }

        sector_visited_cache[sector] = cur_frame;
        

        u16 window_min = cur_item.x1;
        u16 window_max = cur_item.x2;

        s16 wall_offset = sector_wall_offset(sector, map);
        s16 portal_offset = sector_portal_offset(sector, map);
        s16 num_walls = sector_num_walls(sector, map);

        s16 floor_height = sector_floor_height(sector, map); 
        s16 ceil_height = sector_ceil_height(sector, map);

        u8 floor_color = sector_floor_color(sector, map);
        u8 ceil_color = sector_ceil_color(sector, map);
        
        
        u16 init_v1_idx = map->walls[wall_offset];
        vertex init_v1 = map->vertexes[init_v1_idx];
        Vect2D_s16 prev_transformed_vert = transform_map_vert_16(init_v1.x, init_v1.y);
        
        s16 last_frontfacing_wall = -1;
        
        u16 prev_v2_idx = init_v1_idx;

        s16 intPx = fix32ToInt(cur_player_pos.x);
        s16 intPy = fix32ToInt(cur_player_pos.y);

        for(s16 i = 0; i < num_walls; i++) {
	        u16 v2_idx = map->walls[wall_offset+i+1];

            u16 wall_idx = wall_offset+i;

            vertex v1 = map->vertexes[prev_v2_idx];
	        vertex v2 = map->vertexes[v2_idx];

            u16 portal_idx = portal_offset+i;
            s16 portal_sector = map->portals[portal_idx];
            int is_portal = portal_sector != -1;
            
            //u8 wall_col = map->wall_colors[portal_idx];

            volatile Vect2D_s16 trans_v1;

            if(last_frontfacing_wall == i-1) {
                trans_v1 = prev_transformed_vert;
            } else {
                trans_v1 = transform_map_vert_16(v1.x, v1.y);
            }        

            prev_v2_idx = v2_idx;
            
            u8 wall_color = map->wall_colors[portal_idx].mid_col;
            
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
                //pre_transform_backfacing_walls++;
                continue;
            }
            
            

            last_frontfacing_wall = i;
            volatile Vect2D_s16 trans_v2 = transform_map_vert_16(v2.x, v2.y);
            prev_transformed_vert = trans_v2;
            
            clip_result clipped = clip_map_vertex_16(&trans_v1, &trans_v2);    
            s16 neighbor_floor_height;
            s16 neighbor_ceil_height;
            if(clipped == OFFSCREEN) {    
                
                if(is_portal && sector == src_sector) {
                    if(trans_v1.y == 0) { trans_v1.y = 1; }
                    if(trans_v2.y == 0) { trans_v2.y = 1; }
                    s16 x1 = project_and_adjust_x(trans_v1.x, trans_v1.y);
                    s16 x2 = project_and_adjust_x(trans_v2.x, trans_v2.y);

                    //f(x1 > window_max) { walls_frustum_culled++; continue; }
                    //if(x2 <= window_min) { walls_frustum_culled++; continue; }
                    //if(x1 >= x2) { post_project_backfacing_walls++; continue; }
                    neighbor_floor_height = sector_floor_height(portal_sector, cur_portal_map);
                    neighbor_ceil_height = sector_ceil_height(portal_sector, cur_portal_map);
                    if (neighbor_ceil_height > floor_height && neighbor_floor_height < ceil_height && !queue_full()) {

                        queue_item.x1 = window_min;
                        queue_item.x2 = window_max;

                        queue_item.sector = portal_sector;
                        queue_push(queue_item);
                    }
                    
                }
                
                continue;
            }

            s16 trans_v1_z = trans_v1.y;
            s16 trans_v2_z = trans_v2.y;
            s16 x1 = project_and_adjust_x(trans_v1.x, trans_v1_z);
            s16 x2 = project_and_adjust_x(trans_v2.x, trans_v2_z);
            s16 beginx = max(x1, window_min);
            s16 endx = min(x2, window_max);

            if(x1 > window_max) { walls_frustum_culled++; continue; }
            if(x2 <= window_min) { walls_frustum_culled++; continue; }
            if(x1 >= x2) { post_project_backfacing_walls++; continue; }


            if(is_portal) {
                
                neighbor_floor_height = sector_floor_height(portal_sector, cur_portal_map);
                neighbor_ceil_height = sector_ceil_height(portal_sector, cur_portal_map);
                if (neighbor_ceil_height > floor_height && neighbor_floor_height < ceil_height && !queue_full()) {

                    queue_item.x1 = (clipped == LEFT_CLIPPED) ? window_min : beginx;
                    queue_item.x2 = (clipped == RIGHT_CLIPPED) ? window_max : endx;

                    queue_item.sector = portal_sector;
                    queue_push(queue_item);
                }
                
            }


            s16 x1_ytop = project_and_adjust_y_fix(ceil_height, trans_v1_z);
            s16 x1_ybot = project_and_adjust_y_fix(floor_height, trans_v1_z);
            s16 x2_ytop = project_and_adjust_y_fix(ceil_height, trans_v2_z);
            s16 x2_ybot = project_and_adjust_y_fix(floor_height, trans_v2_z);
            

            // draw floor and ceiling
            // check if floor and ceiling are on-screen

            if (is_portal) {
                // draw step down from ceiling
                if(neighbor_ceil_height < ceil_height && neighbor_ceil_height > floor_height) {
                    u8 upper_color = map->wall_colors[portal_idx].upper_col;
                    // draw step from ceiling
                    
                    s16 nx1_ytop = project_and_adjust_y_fix(neighbor_ceil_height, trans_v1_z);
                    s16 nx2_ytop = project_and_adjust_y_fix(neighbor_ceil_height, trans_v2_z);

                    draw_upper_step(x1, x1_ytop, nx1_ytop, x2, x2_ytop, nx2_ytop, window_min, window_max, upper_color, ceil_color);
                } else {
                    draw_ceiling_update_clip(x1, x1_ytop, x2, x2_ytop, window_min, window_max, ceil_color);
                }


                if(neighbor_floor_height > floor_height && neighbor_floor_height < ceil_height) {
                    u8 lower_color = map->wall_colors[portal_idx].lower_col;
                    // draw step from floor
                    //s16 nx1_ybot = project_and_adjust_y_fix(trans_v1, neighbor_floor_height);
                    //s16 nx2_ybot = project_and_adjust_y_fix(trans_v2, neighbor_floor_height);
                    s16 nx1_ybot = project_and_adjust_y_fix(neighbor_floor_height, trans_v1_z);
                    s16 nx2_ybot = project_and_adjust_y_fix(neighbor_floor_height, trans_v2_z);

                    draw_lower_step(x1, x1_ybot, nx1_ybot, x2, x2_ybot, nx2_ybot, window_min, window_max, lower_color, floor_color);
                } else {
                    draw_floor_update_clip(x1, x1_ybot, x2, x2_ybot, window_min, window_max, floor_color, );
                }

            } else { 
                draw_wall(x1, x1_ytop, x1_ybot, x2, x2_ytop, x2_ybot, window_min, window_max, ceil_color, wall_color, floor_color, dither_wall, dither_floor);
            }
            
        }
    }
}
*/