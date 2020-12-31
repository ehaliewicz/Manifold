#include <genesis.h>
#include "game.h"
#include "level.h"
#include "sector.h"
#include "span_buf.h"
#include "math3d.h"

u32 *visited_ssector_frames;

void init_visited_sector_graph() {
    int num_ssectors = cur_level->num_ssectors;
    visited_ssector_frames = MEM_alloc(sizeof(u32) * num_ssectors);
    memsetU32(visited_ssector_frames, 0xFFFFFFFF, num_ssectors);
}

void cleanup_visited_sector_graph() {
    MEM_free(visited_ssector_frames);
}

#define MAX_VISIT_SSECTORS 64

void traverse_and_render_ssector_graph(int ply_ssector) {
    int ssector_queue[MAX_VISIT_SSECTORS];
    int wptr = 0;
    int rptr = 0;
    ssector_queue[wptr++] = ply_ssector;

    while(rptr != wptr) {
        // as long as we keep adding new sectors, continue this loop
        if(wptr >= MAX_VISIT_SSECTORS) {
            break;
        }
        
        int cur_ssector = ssector_queue[rptr++];


        ssector cur_subsector = cur_level->ssectors[cur_ssector];
        sector cur_sector = cur_level->sectors[cur_ssector]; // WRONG!

        seg* start_seg_ptr = &(cur_level->segs[cur_subsector.first_seg]);
        for(int i = 0; i < cur_subsector.num_segs; i++) {
            seg cur_seg = *start_seg_ptr++;
            linedef l = cur_level->linedefs[cur_seg.linedef];
            int is_portal = l.left_sidedef != 0xFFFF && l.right_sidedef != 0xFFFF;
            int adj_ssector;
            if(is_portal) {
                adj_ssector = seg_adjacent_subsector(cur_seg);
            }

            if(is_backfacing(cur_seg, cur_player_pos.ang)) {
                continue;
            }

            if(is_portal && !is_in_pvs(ply_ssector, adj_ssector)) {
                // if not in pvs, that means this span is not visible from the current location
                continue;
            }

            vertex v1 = cur_level->vertexes[cur_seg.begin_vert];
            vertex v2 = cur_level->vertexes[cur_seg.end_vert];
            Vect2D_s32 tv1 = transform_map_vert(v1.x, v1.y);
            Vect2D_s32 tv2 = transform_map_vert(v2.x, v2.y);

            clip_result clp = clip_map_vertex(&tv1, &tv2);

            if(clp == OFFSCREEN) {
                continue;
            }


            transformed_vert proj_v1 = project_and_adjust_3d(tv1, cur_sector.floor_height, cur_sector.ceil_height);
            transformed_vert proj_v2 = project_and_adjust_3d(tv2, cur_sector.floor_height, cur_sector.ceil_height);


            if(is_portal) {
                int num_draw_spans = insert_portal_span(proj_v1, proj_v2);
                if(num_draw_spans > 0) {
                    ssector_queue[wptr++] = adj_ssector;
                }
            } else {
                
                int num_draw_spans = insert_solid_color_wall_span(proj_v1, proj_v2, 0xFF);
                if(clp == LEFT_CLIPPED) {
                    // draw transparent span from right edge of span, to right edge of screen
                    //transformed_vert proj_v3 = {.x = proj_v2.x+1, .yceil = 0, .yfloor = BMP_HEIGHT-1};
                    //transformed_vert proj_v4 = {.x = BMP_WIDTH, .yceil = 0, .yfloor = BMP_HEIGHT-1};
                    //insert_solid_color_wall_span(proj_v3, proj_v4, transparent_col);

                } else if (clp == RIGHT_CLIPPED) {
                    // draw transparent span from left of screen up to left edge of span
                    //transformed_vert proj_v3 = {.x = 0, .yceil = 0, .yfloor = BMP_HEIGHT-1};
                    //transformed_vert proj_v4 = {.x = proj_v1.x-1, .yceil = 0, .yfloor = BMP_HEIGHT-1};
                    //insert_solid_color_wall_span(proj_v3, proj_v4, transparent_col);
                }
            }

        }

    }
}