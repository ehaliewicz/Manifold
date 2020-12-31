#include <genesis.h>
#include "common.h"
#include "draw.h"
#include "level.h"
#include "math3D.h"
#include "portal_map.h"
#include "portal_small_map.h"
#include "vertex.h"


#define PORTAL_QUEUE_SIZE 32

//static Vect2D_s32* cached_verts;
static u32* sector_visited_cache; // i don't know if this helps much, but we might be able to use it
static u8* sector_render_queue;

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

void init_portal_renderer() {
    //vert_transform_cache = MEM_alloc(sizeof(u32) * cur_portal_map->num_verts);
    queue_read = 0;
    queue_write = 0;

    sector_visited_cache = MEM_alloc(sizeof(u32) * cur_portal_map->num_sectors);   
}

void clear_portal_cache() {
    memset(sector_visited_cache, 0, sizeof(u32) * cur_portal_map->num_sectors);
}

void portal_rend(u16 src_sector, u32 cur_frame) {
    static const portal_map* map = &portal_level_1;


    render_sector_item queue_item = { .x1 = 0, .x2 = W-1, .sector = src_sector };

    queue_push(queue_item);

    int visited = 0;

    while(!queue_empty()) {
        visited++;
        render_sector_item cur_item = queue_pop();

        s16 sector = cur_item.sector;

        // if this sector has been visited 32 times, or is already being currently rendered, skip it
        if(sector_visited_cache[sector] >= 1) { //} & 0b101) { //0x21) {
             continue; // Odd = still rendering, 0x20 = give up
        }

        sector_visited_cache[sector]++;

        u16 window_min = cur_item.x1;
        u16 window_max = cur_item.x2;

        s16 wall_offset = sector_wall_offset(sector, map);
        s16 num_walls = sector_num_walls(sector, map);

        s16 floor_height = sector_floor_height(sector, map); 
        s16 ceil_height = sector_ceil_height(sector, map);
        
        
        u16 init_v1_idx = map->walls[wall_offset];
        vertex init_v1 = map->vertexes[init_v1_idx];
        Vect2D_s32 prev_transformed_vert = transform_map_vert(init_v1.x, init_v1.y);

        for(int i = 0; i < num_walls; i++) {
            int last_wall = (i == num_walls-1);
            u16 v2_idx = map->walls[last_wall ? wall_offset : wall_offset+i+1];
            vertex v2 = map->vertexes[v2_idx];

            s16 portal_sector = map->portals[i+wall_offset];
            int is_portal = portal_sector != -1;

            volatile Vect2D_s32 trans_v1 = prev_transformed_vert;
            volatile Vect2D_s32 trans_v2 = transform_map_vert(v2.x, v2.y);
            prev_transformed_vert = trans_v2;
            if(is_portal && !queue_full()) {
                vis_query_result vis = is_visible(trans_v1, trans_v2, window_min, window_max);

                if(vis.visible) {
                    queue_item.x1 = vis.x1;
                    queue_item.x2 = vis.x2;
                    queue_item.sector = portal_sector;
                    queue_push(queue_item);
                }
            } else {
                draw_wall_from_verts(trans_v1, trans_v2, ceil_height, floor_height, window_min, window_max);
            }
        }
        sector_visited_cache[sector]++;
    }

    char buf[32];
    sprintf(buf, "sectors visited: %i", visited);
    BMP_drawText(buf, 1, 3);


}