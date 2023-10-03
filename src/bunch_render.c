#include <genesis.h>

#include "bunch_render.h"
#include "div_lut.h"
#include "game.h"
#include "level.h"
#include "math3d.h"
#include "portal_map.h"
#include "textures.h"

#define MAX_SORT_BUNCHES 32

// if we have 32 bunches, each bunch can only have 2 walls each
#define MAX_AVG_WALLS_PER_BUNCH 2 

typedef struct {
    u16 sector;
    u16 start_entry;
    u16 num_entries;
    s16 min_x; s16 max_x;
    s16 min_z; s16 max_z;  
} sort_bunch;

typedef struct {
    s16 x1; s16 x2;
    s16 z1; s16 z2;
    s16 wall_idx;
} bunch_wall_entry;

sort_bunch sort_bunches[MAX_SORT_BUNCHES]; // 320 bytes
bunch_wall_entry bunch_wall_entries[MAX_SORT_BUNCHES * MAX_AVG_WALLS_PER_BUNCH]; // up to 64 walls, 512 bytes


u16 cur_bunch_entry_idx;
u16 cur_sort_bunch_idx;

#define dot_scale2_s16(a,b,c,d,res) do {             \
    s16 tmp1 = (b); s16 tmp2 = (d);                  \
    __asm volatile(                                  \
        "muls.w %3, %1\t\n                           \
        muls.w %4, %2\t\n                            \
        add.l %1,%2\t\n                              \
        asr.l #2,%2\t\n                              \
        move.l %2,%0"                                \
    : "=d" (res),  "+d" (tmp1),  "+d" (tmp2)         \
    : "d" (a), "d" (c)                               \
    );                                               \
} while(0);

//s32 dot_scale2_s16(s16 a, s16 b, s16 c, s16 d) {
//    return (muls_16_by_16(a,b)+muls_16_by_16(c,d))>>2;
//}

int first_wall_is_behind_second(bunch_wall_entry* bunch1_wall, bunch_wall_entry* bunch2_wall) {
    //s16* b1ptr = (s16*)bunch1_wall;
    //s16 w1x1; FETCH_INC_WORD(w1x1, b1ptr); //bunch1_wall->x1;
    //s16 w1x2; FETCH_INC_WORD(w1x2, b1ptr); // = *b1ptr++;//bunch1_wall->x2;
    //s16 w1y1; FETCH_INC_WORD(w1y1, b1ptr); // = *b1ptr++;//bunch1_wall->z1; // 12.4
    //s16 w1y2; FETCH_INC_WORD(w1y2, b1ptr); // = *b1ptr++;//bunch2_wall->z2; // 12.4

    //s16* b2ptr = (s16*)bunch2_wall;
    //s16 w2x1; FETCH_INC_WORD(w2x1, b2ptr);// = bunch2_wall->x1;
    //s16 w2x2; FETCH_INC_WORD(w2x2, b2ptr);// = bunch2_wall->x2;
    //s16 w2y1; FETCH_INC_WORD(w2y1, b2ptr);// = bunch2_wall->z1; // 12.4
    //s16 w2y2; FETCH_INC_WORD(w2y2, b2ptr);// = bunch2_wall->z2; // 12.4
    s16 w1x1 = bunch1_wall->x1;
    s16 w1x2 = bunch1_wall->x2;
    s16 w1y1 = bunch1_wall->z1; // 12.4
    s16 w1y2 = bunch2_wall->z2; // 12.4

    s16* b2ptr = (s16*)bunch2_wall;
    s16 w2x1 = bunch2_wall->x1;
    s16 w2x2 = bunch2_wall->x2;
    s16 w2y1 = bunch2_wall->z1; // 12.4
    s16 w2y2 = bunch2_wall->z2; // 12.4
    
    s16 dx = w1x2-w1x1;
    s16 dy = w1y2-w1y1; // 12.4

    // PART 1 of wall test

    //dot product between wall 1 vector and [wall1 point 1 -> wall2 point 1] vector


    s32 t1; dot_scale2_s16(w2x1-w1x1, dy, -dx, w2y1-w1y1, t1);
    //dot product between wall 1 vector and [wall1 point 1 -> wall2 point 2] vector
    s32 t2; dot_scale2_s16(w2x2-w1x1, dy, -dx, w2y2-w1y1, t2);


    if (t1 == 0) {
        t1 = t2;
        if (t1 == 0) {
            return 0;
        }
    }

    if (t2 == 2) {
        t2 = t1;
    }

   
    s16 cur_cam_x = cur_player_pos.x >> FIX32_FRAC_BITS; // 16.0
    s16 cur_cam_y = cur_player_pos.y >> (FIX32_FRAC_BITS-TRANS_Z_FRAC_BITS); // 12.4
    if ((t1 ^ t2) == 0) {
        //KLog("wall test couldn't early out");
        //same sign
        //now determine which is in front relative to the player position
        s32 t2; dot_scale2_s16(cur_cam_x-w1x1, dy, -dx, cur_cam_y-w1y1, t2);

        //if t2^t1 >= 0 then both dot products have a different sign, so wall1 is in front of wall2
        //return ((t2^t1) >= 0)
        if ((t2^t1) >= 0) {
            return 0;
        } else {
            return 1;
        }
    }


    
    // PART 2 of wall test
   
    // do everything again, but with wall 2's plane
    dx = w2x2-w2x1;
    dy = w2y2-w2y1; // 12.4

    //t1 = 
    dot_scale2_s16(w1x1-w2x1, dy, -dx, w1y1-w2y1, t1); // wall1 point1 vs line 2
    //t2 = 
    dot_scale2_s16(w1x2-w2x1, dy, -dx, w1y2-w2y1, t2); // wall1 point2 vs line 2

    if (t1 == 0) {
        t1 = t2;
        if (t1 == 0) {
            return 0; // unoccluded
        }
    }

    if (t2 == 0) {
        t2 = t1;
    }

    if ((t2^t1) >= 0) {
        //KLog("wall test couldn't early out");
        //t2 = 
        dot_scale2_s16(cur_cam_x-w2x1, dy, -dx, cur_cam_y-w2y1, t2);

        if ((t2^t1) >= 0) {
            // wall 2 is in front of wall 1, therefore wall 1 needs to move right :)
            return 1;
        }
        else {
            return 0;
        }
    }

    //assert False, "Found degenerate pair of walls {}, {}".format(bunch1_wall.index, bunch2_wall.index)
    die("Degenerate map!");
    return -2;

}

/*
def wall_occlusion_test(bunch1_wall, bunch2_wall):

    w1x1 = bunch1_wall.x1
    w1x2 = bunch1_wall.x2
    w1y1 = bunch1_wall.y1
    w1y2 = bunch2_wall.y2

    w2x1 = bunch2_wall.x1
    w2x2 = bunch2_wall.x2
    w2y1 = bunch2_wall.y1
    w2y2 = bunch2_wall.y2

    w1dx = w1x2-w1x1
    w1dy = w1y2-w1y1

    # PART 1 of wall test
   
    # dot product between wall 1 vector and [wall1 point 1 -> wall2 point 1] vector
    t1 = dot_scale2(w2x1-w1x1, dy, -dx, w2y1-w1y1)

    # dot product between wall 1 vector and [wall1 point 1 -> wall2 point 2] vector
    t2 = dot_scale2(w2x2-w1x1, dy, -dx, w2y2-w1y1)

    if t1 == 0:
        t1 = t2
        if t1 == 0:
            return BUNCH_NO_OVERLAP

    if t2 == 2:
        t2 = t1

   
    cur_cam_x = cur_player_pos.x >> SCALE_FACTOR
    cur_cam_y = cur_player_pos.y >> SCALE_FACTOR
    if t1 ^ t2 == 0:
        # same sign
        # now determine which is in front relative to the player position
        t2 = dot_scale2(cur_cam_x-w1x1, dy, -dx, cur_cam_y-w1y1)

        # if t2^t1 >= 0 then both dot products have a different sign, so wall1 is in front of wall2
        #return ((t2^t1) >= 0)

        if ((t2^t1) >= 0):
            return BUNCH_UNOCCLUDED
        else:
            return BUNCH_OCCLUDED #

   
    # PART 2 of wall test
   
    # do everything again, but with wall 2's plane
    dx = w2x2-w2x1
    dy = w2y2-w2y1

    t1 = dot_scale2(w1x1-w2x1, dy, -dx, w1y1-w2y1) # wall1 point1 vs line 2
    t2 = dot_scale2(w1x2-w2x1, dy, -dx, w1y2-w2y1) # wall1 point2 vs line 2

    if t1 == 0:
        t1 = t2
        if t1 == 0:
            return BUNCH_UNOCCLUDED

    if t2 == 0:
        t2 = t1

    if t2^t1 >= 0:
        t2 = dot_scale2(cur_cam_x-w2x1, dy, -dx, cur_cam_y, w2y1)

        if (((t2^t1) >= 0):
            # wall 2 is in front of wall1
            return BUNCH_OCCLUDED
        else:
            return BUNCH_UNOCCLUDED
           

    assert False, "Found degenerate pair of walls {}, {}".format(bunch1_wall.index, bunch2_wall.index)
   
    return BUNCH_DEGENERATE
*/

int bunch_needs_to_move_right(sort_bunch* b1, sort_bunch* b2) {
    if(b1->max_x < b2->min_x) {
        //KLog("bunch x early out");
        return 0;
    }
    if(b1->min_x >= b2->max_x) {
        //KLog("bunch x early out");
        return 0;
    }
    if(b1->max_z <= b2->min_z) {
        //KLog("bunch z early out");
        // early out, baby :)
        return 0;
    }
    if(b1->min_z >= b2->max_z) {
        //KLog("bunch z early out");
        // early out, baby :)
        return 1; // move right, this is potentially occluded
    }
    //KLog("bunch comparison couldn't early out");
    if(b1->min_x >= b2->max_x) {

        // get first wall in bunch 2 that overlaps with bunch 1
        u16 first_wall_idx;
        for(first_wall_idx = b2->start_entry;
            bunch_wall_entries[first_wall_idx].x2 < b1->min_x;
            first_wall_idx++);
        return first_wall_is_behind_second(&bunch_wall_entries[b1->start_entry], &bunch_wall_entries[first_wall_idx]);
    } else {

        // get first wall in first bunch that overlaps with bunch 2
        u16 first_wall_idx;
        for(first_wall_idx = b1->start_entry;
            bunch_wall_entries[first_wall_idx].x2 < b2->min_x;
            first_wall_idx++);
        return first_wall_is_behind_second(&bunch_wall_entries[first_wall_idx], &bunch_wall_entries[b2->start_entry]);
    }

}



void prepare_bunches(u16 src_sector, u32 cur_frame) {

    //KLog("PREPARING BUNCHES");
    cur_sort_bunch_idx = 0;
    cur_bunch_entry_idx = 0;

    pvs_bunch_group* group = &cur_portal_map->pvs_bunch_groups[src_sector];

    u16 bunch_offset = group->bunch_offset;
    u16 num_bunches = group->num_bunches;
    
    s16* sort_bunches_ptr = (s16*)sort_bunches;
    //Vect2D_s16 transformed_vert_buffer[32];

    for(u16 idx = 0; idx < num_bunches; idx++) {
        //KLog_U2("- starting work on sector ", src_sector, " static bunch: ", idx);
        pvs_bunch_entry* entry = &cur_portal_map->pvs_bunch_entries[idx+bunch_offset];
        u16 wall_offset = entry->wall_offset;
        u8 num_walls = entry->num_walls;
        //KLog_U1("- num walls in bunch?: ", num_walls);
        u16 sector_idx = entry->sector_num;

        s16 min_x = MAX_S16; s16 max_x = MIN_S16;
        s16 min_z = MAX_S16; s16 max_z = MIN_S16;

        // start a new bunch
        
        sort_bunches[cur_sort_bunch_idx].start_entry = cur_bunch_entry_idx;
        sort_bunches[cur_sort_bunch_idx].sector = sector_idx;

        //WRITE_WORD_INC(sector_idx, sort_bunches_ptr);
        //WRITE_BYTE_INC(cur_bunch_entry_idx, sort_bunches_ptr);

        //WRITE_WORD_INC(sector_idx, sort_bunches_ptr);         // write sector field
        //WRITE_WORD_INC(cur_bunch_entry_idx,  sort_bunches_ptr); // write start entry field



        u8 visible_walls = 0;
        
        //Vect2D_s16 transformed_vert_buffer[32];
        //Vect2D_s16* transformed_vert_buffer_ptr = transformed_vert_buffer;
        //load_transform_and_duplicate_verts(
        //    num_walls, 
        //    &cur_portal_map->walls[wall_offset], 
        //    cur_portal_map->vertexes, 
        //    transformed_vert_buffer_ptr
        //);

        for(u16 wall_idx = 0; wall_idx < num_walls; wall_idx++) {
            u16 v1_idx = cur_portal_map->walls[wall_idx+wall_offset];
            u16 v2_idx = cur_portal_map->walls[wall_idx+wall_offset+1];
            vertex v1 = cur_portal_map->vertexes[v1_idx];
            vertex v2 = cur_portal_map->vertexes[v2_idx];
            Vect2D_s16 trans_v1 = transform_map_vert_16(v1.x, v1.y);
            Vect2D_s16 trans_v2 = transform_map_vert_16(v2.x, v2.y);  
            
            //Vect2D_s16 trans_v1 = *transformed_vert_buffer_ptr++;
            //Vect2D_s16 trans_v2 = *transformed_vert_buffer_ptr++;

            s16 rx1 = trans_v1.x; s16 rx2 = trans_v2.x;
            s16 rz1_12_4 = trans_v1.y; s16 rz2_12_4 = trans_v2.y;


            bunch_wall_entries[cur_bunch_entry_idx].wall_idx = wall_offset+wall_idx;

            // check if fully near clipped
            if(rz1_12_4 < NEAR_Z_FIX && rz2_12_4 < NEAR_Z_FIX) {   
                //KLog_U1_("- - wall ", wall_idx, " fully near clipped");    
                if(visible_walls) {
                    sort_bunches[cur_sort_bunch_idx].num_entries = visible_walls;
                    sort_bunches[cur_sort_bunch_idx].min_x = min_x;
                    sort_bunches[cur_sort_bunch_idx].max_x = max_x;
                    sort_bunches[cur_sort_bunch_idx].min_z = min_z;
                    sort_bunches[cur_sort_bunch_idx++].max_z = max_z;
                    sort_bunches[cur_sort_bunch_idx].sector = sector_idx;
                    sort_bunches[cur_sort_bunch_idx].start_entry = cur_bunch_entry_idx;
                    //WRITE_WORD_INC(visible_walls, sort_bunches_ptr);
                    //WRITE_WORD_INC(min_x, sort_bunches_ptr);
                    //WRITE_WORD_INC(max_x, sort_bunches_ptr);
                    //WRITE_WORD_INC(min_z, sort_bunches_ptr);
                    //WRITE_WORD_INC(max_z, sort_bunches_ptr);
                    //WRITE_WORD_INC(sector_idx, sort_bunches_ptr);
                    //WRITE_WORD_INC(cur_bunch_entry_idx, sort_bunches_ptr);
                    //cur_sort_bunch_idx++;
                    visible_walls = 0;
                }
                continue;
            }

            // check against left side of frustum
            const u8 left_vert_outside_left_frustum = (-rx1 > (rz1_12_4>>4));
            const u8 right_vert_outside_left_frustum = (-rx2 > (rz2_12_4>>4));
            if(left_vert_outside_left_frustum && right_vert_outside_left_frustum) {
                //KLog_U1_("- - wall ", wall_idx, " left frustum clipped");       
                if(visible_walls) {
                    sort_bunches[cur_sort_bunch_idx].num_entries = visible_walls;
                    sort_bunches[cur_sort_bunch_idx].min_x = min_x;
                    sort_bunches[cur_sort_bunch_idx].max_x = max_x;
                    sort_bunches[cur_sort_bunch_idx].min_z = min_z;
                    sort_bunches[cur_sort_bunch_idx++].max_z = max_z;
                    sort_bunches[cur_sort_bunch_idx].start_entry = cur_bunch_entry_idx;
                    sort_bunches[cur_sort_bunch_idx].sector = sector_idx;
                    //WRITE_WORD_INC(visible_walls, sort_bunches_ptr);
                    //WRITE_WORD_INC(min_x, sort_bunches_ptr);
                    //WRITE_WORD_INC(max_x, sort_bunches_ptr);
                    //WRITE_WORD_INC(min_z, sort_bunches_ptr);
                    //WRITE_WORD_INC(max_z, sort_bunches_ptr);
                    //WRITE_WORD_INC(sector_idx, sort_bunches_ptr);
                    //WRITE_WORD_INC(cur_bunch_entry_idx, sort_bunches_ptr);
                    //cur_sort_bunch_idx++;
                    visible_walls = 0;
                }
                continue;
            }

            // check against right side of frustum
            const u8 left_vert_outside_right_frustum = (rx1 > (rz1_12_4>>4));
            const u8 right_vert_outside_right_frustum = (rx2 > (rz2_12_4>>4));
            if(left_vert_outside_right_frustum && right_vert_outside_right_frustum) {  
                //KLog_U1_("- - wall ", wall_idx, " right frustum clipped");      
                if(visible_walls) {
                    sort_bunches[cur_sort_bunch_idx].num_entries = visible_walls;
                    sort_bunches[cur_sort_bunch_idx].min_x = min_x;
                    sort_bunches[cur_sort_bunch_idx].max_x = max_x;
                    sort_bunches[cur_sort_bunch_idx].min_z = min_z;
                    sort_bunches[cur_sort_bunch_idx++].max_z = max_z;
                    sort_bunches[cur_sort_bunch_idx].start_entry = cur_bunch_entry_idx;
                    sort_bunches[cur_sort_bunch_idx].sector = sector_idx;
                    //WRITE_WORD_INC(visible_walls, sort_bunches_ptr);
                    //WRITE_WORD_INC(min_x, sort_bunches_ptr);
                    //WRITE_WORD_INC(max_x, sort_bunches_ptr);
                    //WRITE_WORD_INC(min_z, sort_bunches_ptr);
                    //WRITE_WORD_INC(max_z, sort_bunches_ptr);
                    //WRITE_WORD_INC(sector_idx, sort_bunches_ptr);
                    //WRITE_WORD_INC(cur_bunch_entry_idx, sort_bunches_ptr);
                    //cur_sort_bunch_idx++;
                    visible_walls = 0;
                }
                continue;
            }

            s16 x1; s16 x2;
            if(rz1_12_4 < NEAR_Z_FIX) {
                //s16 dz_12_4 = rz2_12_4 - rz1_12_4;
                //s16 dx = rx2-rx1;
                //s32 dx_over_dz_6 = (dx<<(6+TRANS_Z_FRAC_BITS))/ dz_12_4; // << 6
                //s16 z_adjust_12_4 = NEAR_Z_FIX-rz1_12_4;
                //s32 x_adjust_10 = dx_over_dz_6 * z_adjust_12_4;
                //s16 x_adjust_int = x_adjust_10>>(6+TRANS_Z_FRAC_BITS);
                //rx1 += x_adjust_int;    // modify x coord
                //KLog_S1("left clipped x: ", rx1);
                //rz1_12_4 = NEAR_Z_FIX;
                if(visible_walls) {
                    sort_bunches[cur_sort_bunch_idx].num_entries = visible_walls;
                    sort_bunches[cur_sort_bunch_idx].min_x = min_x;
                    sort_bunches[cur_sort_bunch_idx].max_x = max_x;
                    sort_bunches[cur_sort_bunch_idx].min_z = min_z;
                    sort_bunches[cur_sort_bunch_idx++].max_z = max_z;
                    sort_bunches[cur_sort_bunch_idx].start_entry = cur_bunch_entry_idx;
                    sort_bunches[cur_sort_bunch_idx].sector = sector_idx;
                    //WRITE_WORD_INC(visible_walls, sort_bunches_ptr);
                    //WRITE_WORD_INC(min_x, sort_bunches_ptr);
                    //WRITE_WORD_INC(max_x, sort_bunches_ptr);
                    //WRITE_WORD_INC(min_z, sort_bunches_ptr);
                    //WRITE_WORD_INC(max_z, sort_bunches_ptr);
                    //WRITE_WORD_INC(sector_idx, sort_bunches_ptr);
                    //WRITE_WORD_INC(cur_bunch_entry_idx, sort_bunches_ptr);
                    //cur_sort_bunch_idx++;
                    visible_walls = 0;
                }
                continue;

            } else if(rz2_12_4 < NEAR_Z_FIX) {
                // clip right side, need to adjust textures here
                //rz2_12_4 = NEAR_Z_FIX;
                //u16 z_recip_v1 = z_recip_table_16[rz1_12_4>>TRANS_Z_FRAC_BITS];
                //x1 = project_and_adjust_x(rx1, z_recip_v1);
                //x2 = RENDER_WIDTH;     
                //KLog_U1_("- - wall ", wall_idx, " right near clipped second vertex");  

                if(visible_walls) {
                    //KLog_U2_("- bunch: ", cur_sort_bunch_idx, " completed with ", visible_walls, " walls");
                    sort_bunches[cur_sort_bunch_idx].num_entries = visible_walls;
                    sort_bunches[cur_sort_bunch_idx].min_x = min_x;
                    sort_bunches[cur_sort_bunch_idx].max_x = max_x;
                    sort_bunches[cur_sort_bunch_idx].min_z = min_z;
                    sort_bunches[cur_sort_bunch_idx++].max_z = max_z;
                    sort_bunches[cur_sort_bunch_idx].start_entry = cur_bunch_entry_idx;
                    sort_bunches[cur_sort_bunch_idx].sector = sector_idx;
                    //WRITE_WORD_INC(visible_walls, sort_bunches_ptr);
                    //WRITE_WORD_INC(min_x, sort_bunches_ptr);
                    //WRITE_WORD_INC(max_x, sort_bunches_ptr);
                    //WRITE_WORD_INC(min_z, sort_bunches_ptr);
                    //WRITE_WORD_INC(max_z, sort_bunches_ptr);
                    //WRITE_WORD_INC(sector_idx, sort_bunches_ptr);
                    //WRITE_WORD_INC(cur_bunch_entry_idx, sort_bunches_ptr);
                    //cur_sort_bunch_idx++;
                    visible_walls = 0;
                }
                continue; // skip partially clipped walls 

            } else {
                u16 z_recip_v1 = z_recip_table_16[rz1_12_4>>TRANS_Z_FRAC_BITS];
                u16 z_recip_v2 = z_recip_table_16[rz2_12_4>>TRANS_Z_FRAC_BITS];
                x1 = project_and_adjust_x(trans_v1.x, z_recip_v1);
                x2 = project_and_adjust_x(trans_v2.x, z_recip_v2);
            }
            
            min_x = x1 < min_x ? x1 : min_x;
            max_x = x2 > max_x ? x2 : max_x;
            if(rz1_12_4 > rz2_12_4) {
                max_z = rz1_12_4 > max_z ? rz1_12_4 : max_z;
                min_z = rz2_12_4 < min_z ? rz2_12_4 : min_z;
            } else {
                max_z = rz2_12_4 > max_z ? rz2_12_4 : max_z;
                min_z = rz1_12_4 < min_z ? rz1_12_4 : min_z;
            }
            //KLog_U1_("- - wall ", wall_idx, " not clipped, adding to wall entries.");
            bunch_wall_entries[cur_bunch_entry_idx].x1 = x1;
            bunch_wall_entries[cur_bunch_entry_idx].x2 = x2;
            bunch_wall_entries[cur_bunch_entry_idx].z1 = rz1_12_4;
            bunch_wall_entries[cur_bunch_entry_idx].z2 = rz2_12_4;
            bunch_wall_entries[cur_bunch_entry_idx++].wall_idx = wall_idx+wall_offset;
            visible_walls++;
        }
        
        if(visible_walls) {
            //KLog_U2_("- bunch: ", cur_sort_bunch_idx, " completed with ", visible_walls, " walls");
     
            sort_bunches[cur_sort_bunch_idx].num_entries = visible_walls;
            sort_bunches[cur_sort_bunch_idx].min_x = min_x;
            sort_bunches[cur_sort_bunch_idx].max_x = max_x;
            sort_bunches[cur_sort_bunch_idx].min_z = min_z;
            sort_bunches[cur_sort_bunch_idx++].max_z = max_z;
            //WRITE_WORD_INC(visible_walls, sort_bunches_ptr);
            //WRITE_WORD_INC(min_x, sort_bunches_ptr);
            //WRITE_WORD_INC(max_x, sort_bunches_ptr);
            //WRITE_WORD_INC(min_z, sort_bunches_ptr);
            //WRITE_WORD_INC(max_z, sort_bunches_ptr);
            //cur_sort_bunch_idx++;
        }
    }
    //KLog("BUNCHES PREPARED");

    
    //KLog("SORTING PROJECTED BUNCHES");
    //{
    //    // insertion sort :^)
    //    int i, key, j;
    //    for (i = 1; i < n; i++) {
    //    key = arr[i];
    //    j = i - 1; 
    //    // Move elements of arr[0..i-1],
    //    // that are greater than key,
    //    // to one position ahead of their
    //    // current position
    //    while (j >= 0 && arr[j] > key) {
    //        arr[j + 1] = arr[j];
    //        j = j - 1;
    //    }
    //    arr[j + 1] = key;
    //}
    //}
    // shell sort


    for (int gap = cur_sort_bunch_idx/2; gap > 0; gap /= 2) {
        for (int i = gap; i < cur_sort_bunch_idx; i += 1) {
           sort_bunch temp = sort_bunches[i];  
            int j;            
            for (j = i; j >= gap && bunch_needs_to_move_right(&sort_bunches[j - gap], &temp); j -= gap) {
                //KLog("moving bunch ");
                sort_bunches[j] = sort_bunches[j - gap];
            }              
            //  put temp (the original a[i]) in its correct location
            sort_bunches[j] = temp;
        }   
    }
    //KLog("BUNCHES SORTED");
    

}



void draw_bunches(u16 src_sector, u32 cur_frame) {

    pvs_bunch_group* group = &cur_portal_map->pvs_bunch_groups[src_sector];

    u16 bunch_offset = group->bunch_offset;
    u16 num_bunches = group->num_bunches;

    
    KLog_U2("bunch offset: ", bunch_offset, " sorted and split bunches: ", num_bunches);

    s16 window_min = 0;
    s16 window_max = RENDER_WIDTH;
    s16 cur_player_z_int = cur_player_pos.x>>FIX32_FRAC_BITS;

    Vect2D_s16 transformed_vert_buffer[32];

    for(int i = 0; i < num_bunches; i++) {
        pvs_bunch_entry* entry = &cur_portal_map->pvs_bunch_entries[bunch_offset+i];

        u16 bunch_wall_offset = entry->wall_offset;
        u8 num_walls = entry->num_walls;
        KLog_U2("bunch wall offset: ", entry->wall_offset, " num walls: ", num_walls);    


        //u8 bunch_start = sort_bunches[i].start_entry;
        //u8 num_walls = sort_bunches[i].num_entries;
        
        //KLog_U1_("- Drawing bunch of ", num_walls, " walls.");
        //u16 sector_idx = sort_bunches[i].sector;
        u16 sector_idx = entry->sector_num;


        u16 sect_grp = sector_group(sector_idx, cur_portal_map);

        s8 light_level = get_sector_group_light_level(sect_grp);
        u8 floor_col = get_sector_group_floor_color(sect_grp);
        u8 ceil_col = get_sector_group_ceil_color(sect_grp);
        s16 floor_height = get_sector_group_floor_height(sect_grp);
        s16 ceil_height = get_sector_group_ceil_height(sect_grp);

        s16 rel_floor_height = (floor_height>>4) - cur_player_z_int;
        s16 rel_ceil_height = (ceil_height>>4) - cur_player_z_int;

        light_params floor_params;
        light_params ceil_params;
        cache_ceil_light_params(rel_ceil_height, ceil_col, light_level, &ceil_params);
        cache_floor_light_params(rel_floor_height, floor_col, light_level, &floor_params);
        load_transform_and_duplicate_verts(num_walls, &cur_portal_map->walls[bunch_wall_offset], cur_portal_map->vertexes, transformed_vert_buffer);
        Vect2D_s16* transformed_vert_buffer_ptr = transformed_vert_buffer;

        for(int j = 0; j < num_walls; j++) {
            //bunch_wall_entry* w = &bunch_wall_entries[bunch_start+j];
            u16 wall_index = bunch_wall_offset+j;
            u16 portal_idx = wall_index-sector_idx;
            //u16 wall_index = w->wall_idx;
            //u16 portal_index = wall_index - sector_idx;
            s16 adj_sector_idx = cur_portal_map->portals[portal_idx];
            u8 is_portal = (adj_sector_idx != -1);
            
            KLog_U2("wall indexes: ", wall_index, " ", wall_index+1);
            KLog_U1("portal index: ", portal_idx);
            vertex v1 = cur_portal_map->vertexes[wall_index];
            vertex v2 = cur_portal_map->vertexes[wall_index+1];
            KLog_S2("v1.x: ", v1.x, " v1.y: ", v1.y);
            KLog_S2("v2.x: ", v2.x, " v2.y: ", v2.y);

            Vect2D_s16 trans_v1 = transform_map_vert_16(v1.x, v1.y);
            Vect2D_s16 trans_v2 = transform_map_vert_16(v2.x, v2.y);
            
            //Vect2D_s16 trans_v1 = *transformed_vert_buffer_ptr++;
            //Vect2D_s16 trans_v2 = *transformed_vert_buffer_ptr++;
            s16 x1 = trans_v1.x; s16 x2 = trans_v2.x;
            s16 trans_v1_z_fix = trans_v1.y; s16 trans_v2_z_fix = trans_v2.y;
            //s16 x1 = w->x1; s16 x2 = w->x2;
            //s16 z1 = w->z1; s16 z2 = w->z2;
            if(x1 >= window_max) { continue; }
            if(x2 < window_min) { continue; }
            s16 z_recip_v1 = z_recip_table_16[trans_v1_z_fix>>4];
            s16 z_recip_v2 = z_recip_table_16[trans_v2_z_fix>>4];
            s16 x1_ytop = project_and_adjust_y_fix(ceil_height, z_recip_v1);
            s16 x1_ybot = project_and_adjust_y_fix(floor_height, z_recip_v1);
            s16 x2_ytop = project_and_adjust_y_fix(ceil_height, z_recip_v2);
            s16 x2_ybot = project_and_adjust_y_fix(floor_height, z_recip_v2);


            u8 tex_idx = cur_portal_map->wall_colors[(portal_idx<<WALL_COLOR_NUM_PARAMS_SHIFT)+WALL_TEXTURE_IDX];
            u8 is_solid_color = 1; //cur_portal_map->wall_colors[(portal_idx<<WALL_COLOR_NUM_PARAMS_SHIFT)+WALL_SOLID_COLOR_IDX];
            
            texmap_params tmap_info;
            if(is_solid_color) { 
                tmap_info.needs_texture = 0;
            } else {
                s32 base_left_u_16 = 0;
                s32 base_right_u_16 = (1)<<16;
                s32 fix_du_16 = (base_right_u_16-base_left_u_16);
                s16 dz_12_4 = trans_v2_z_fix - trans_v1_z_fix;
                s32 du_over_dz_16 = 0;

                if(dz_12_4 != 0) {
                    du_over_dz_16 = divs_32_by_16(fix_du_16<<TRANS_Z_FRAC_BITS, dz_12_4);     
                }    
                tmap_info.repetitions = cur_portal_map->wall_tex_repetitions[portal_idx];
                tmap_info.needs_texture = 1;
                tmap_info.tex = get_texture(tex_idx, light_level);
                tmap_info.du_over_dz = du_over_dz_16;
                tmap_info.left_u = base_left_u_16;
                tmap_info.right_u = base_right_u_16;
            }

            if(is_portal) {
                /*
                s16 x1_pegged, x2_pegged;
                //KLog("skipping portal");            
                u16 neighbor_sect_group = sector_group(adj_sector_idx, cur_portal_map);
                s16* neighbor_sect_group_param_pointer = get_sector_group_pointer(neighbor_sect_group);

                s16 neighbor_floor_height = neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_FLOOR_HEIGHT_IDX]; //get_sector_group_floor_height(neighbor_sect_group);
                s16 neighbor_ceil_height = neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_CEIL_HEIGHT_IDX]; //get_sector_group_ceil_height(neighbor_sect_group);


                if(neighbor_floor_height <= floor_height) {
                    //draw_floor_update_clip(x1, x1_ybot, x2, x2_ybot,
                    //                    min(z_recip_v1, z_recip_v2),
                    //                    0, RENDER_WIDTH, &floor_params);
                }

                u8 neighbor_sector_group_type = GET_SECTOR_GROUP_TYPE(cur_portal_map->sector_group_types[neighbor_sect_group]);
                if(neighbor_ceil_height < ceil_height) {

                    s16 nx1_ytop = project_and_adjust_y_fix(neighbor_ceil_height, z_recip_v1);
                    s16 nx2_ytop = project_and_adjust_y_fix(neighbor_ceil_height, z_recip_v2);

                    if(neighbor_sector_group_type == DOOR) {

                        s16 orig_door_height = neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_ORIG_HEIGHT_IDX]; //get_sector_group_orig_height(neighbor_sect_group);
                        s16 orig_height_diff = orig_door_height - neighbor_floor_height;
                        x1_pegged = project_and_adjust_y_fix(neighbor_ceil_height+orig_height_diff, z_recip_v1);
                        x2_pegged = project_and_adjust_y_fix(neighbor_ceil_height+orig_height_diff, z_recip_v2);
                    }

                    u8 upper_color = cur_portal_map->wall_colors[(portal_idx<<WALL_COLOR_NUM_PARAMS_SHIFT)+WALL_HIGH_COLOR_IDX];
                    // draw step from ceiling
                    //draw_upper_step(x1, x1_ytop, nx1_ytop, x2, x2_ytop, nx2_ytop,
                    //                z_recip_v1, z_recip_v2,
                    //                window_min, window_max, upper_color, light_level, &ceil_params);

                    if(neighbor_sector_group_type == DOOR && !is_solid_color) {
                        //continue;
                        draw_top_pegged_textured_upper_step(x1, x1_ytop, nx1_ytop, x2, x2_ytop, nx2_ytop,
                                                            trans_v1_z_fix, trans_v2_z_fix,
                                                            z_recip_v1, z_recip_v2,
                                                            0, RENDER_WIDTH, light_level,
                                                            &tmap_info,
                                                            &ceil_params, x1_pegged, x2_pegged);
                    } else {
                        draw_upper_step(x1, x1_ytop, nx1_ytop, x2, x2_ytop, nx2_ytop,
                                        z_recip_v1, z_recip_v2,
                                        0, RENDER_WIDTH, upper_color, light_level, &ceil_params);

                    }
                } else {
                    //u16 neighbor_ceil_color = neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_CEIL_COLOR_IDX]; //get_sector_group_ceil_color(neighbor_sect_group);
                    //u16 neighbor_light_level = neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_LIGHT_IDX];
                    //if(neighbor_ceil_color != ceil_color || neighbor_ceil_height != ceil_height || !recur || neighbor_light_level != light_level) {
                        draw_ceiling_update_clip(x1, x1_ytop, x2, x2_ytop,
                                                min(z_recip_v1, z_recip_v2),
                                                0, RENDER_WIDTH, &ceil_params);
                    //}
                }

                if(neighbor_floor_height > floor_height) { //} && neighbor_floor_height <= ceil_height) {
                    s16 nx1_ybot = project_and_adjust_y_fix(neighbor_floor_height, z_recip_v1);
                    s16 nx2_ybot = project_and_adjust_y_fix(neighbor_floor_height, z_recip_v2);

                    if(neighbor_sector_group_type == LIFT) {
                        u16 neighbor_state = neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_STATE_IDX];
                        if(neighbor_state == GOING_DOWN || neighbor_state == GOING_UP || neighbor_state == OPEN) {
                            s16 diff = neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_SCRATCH_ONE_IDX] - 
                                    floor_height; //neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_SCRATCH_TWO_IDX];
                            x1_pegged = project_and_adjust_y_fix(neighbor_floor_height-diff, z_recip_v1);
                            x2_pegged = project_and_adjust_y_fix(neighbor_floor_height-diff, z_recip_v2);

                        } else { // if (neighbor_state == CLOSED) {
                            x1_pegged = x1_ybot;
                            x2_pegged = x2_ybot;
                        }
                    }

                    u8 lower_color = cur_portal_map->wall_colors[(portal_idx<<WALL_COLOR_NUM_PARAMS_SHIFT)+WALL_LOW_COLOR_IDX];

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
                    
                    // this is redundant
                    //    draw_floor_update_clip(x1, x1_ybot, x2, x2_ybot,
                    //                        min(z_recip_v1, z_recip_v2),
                    //                        window_min, window_max, &floor_params);
                    
                    //}
                }
                
                */
            } else {
                //KLog("- - Drawing wall ");
                //texmap_params tmap_info;
                //tmap_info.du_over_dz = 0;
                //tmap_info.needs_texture = 0;

                if(is_solid_color) {
                    draw_solid_color_wall(
                        x1, x1_ytop, x1_ybot, x2, x2_ytop, x2_ybot,
                        z_recip_v1, z_recip_v2, window_min, window_max,
                        light_level, tex_idx, &floor_params, &ceil_params);
                } else {   

                    draw_wall(
                        x1, x1_ytop, x1_ybot, x2, x2_ytop, x2_ybot,
                        trans_v1_z_fix, trans_v2_z_fix, z_recip_v1, z_recip_v2, window_min, window_max, 
                        light_level, &tmap_info, &floor_params, &ceil_params
                    );
                }
            }

            /*
            // performance optimization only
            // re-enable when debugged
            if(x1 <= 0 && x2 > window_min) {
                window_min = x2;
                if(window_min >= RENDER_WIDTH) {
                    return;
                }
            }
            if(x2 >= window_max && x1 < window_max) {
                window_max = x1;
                if(window_max <= 0) {
                    return;
                }
            }
            */
        }
        //KLog("- Done drawing bunch.");
    }
    //KLog("Done drawing bunches.");
}

/*
void draw_bunches() {

    //KLog_U1("sorted and split bunches: ", cur_sort_bunch_idx);

    s16 window_min = 0;
    s16 window_max = RENDER_WIDTH;
    s16 cur_player_z_int = cur_player_pos.x>>FIX32_FRAC_BITS;


    for(int i = 0; i < cur_sort_bunch_idx; i++) {
        u8 bunch_start = sort_bunches[i].start_entry;
        u8 num_walls = sort_bunches[i].num_entries;
        //KLog_U1_("- Drawing bunch of ", num_walls, " walls.");
        u16 sector_idx = sort_bunches[i].sector;
        u16 sect_grp = sector_group(sector_idx, cur_portal_map);

        s8 light_level = get_sector_group_light_level(sect_grp);
        u8 floor_col = get_sector_group_floor_color(sect_grp);
        u8 ceil_col = get_sector_group_ceil_color(sect_grp);
        s16 floor_height = get_sector_group_floor_height(sect_grp);
        s16 ceil_height = get_sector_group_ceil_height(sect_grp);

        s16 rel_floor_height = (floor_height>>4) - cur_player_z_int;
        s16 rel_ceil_height = (ceil_height>>4) - cur_player_z_int;

        light_params floor_params;
        light_params ceil_params;
        cache_ceil_light_params(rel_ceil_height, ceil_col, light_level, &ceil_params);
        cache_floor_light_params(rel_floor_height, floor_col, light_level, &floor_params);

        for(int j = 0; j < num_walls; j++) {
            bunch_wall_entry* w = &bunch_wall_entries[bunch_start+j];
            u16 wall_index = w->wall_idx;
            u16 portal_index = wall_index - sector_idx;
            s16 adj_sector_idx = cur_portal_map->portals[portal_index];
            u8 is_portal = (adj_sector_idx != -1);

            s16 x1 = w->x1; s16 x2 = w->x2;
            s16 z1 = w->z1; s16 z2 = w->z2;
            if(x1 >= window_max) { continue; }
            if(x2 < window_min) { continue; }
            s16 z_recip_v1 = z_recip_table_16[z1>>4];
            s16 z_recip_v2 = z_recip_table_16[z2>>4];
            s16 x1_ytop = project_and_adjust_y_fix(ceil_height, z_recip_v1);
            s16 x1_ybot = project_and_adjust_y_fix(floor_height, z_recip_v1);
            s16 x2_ytop = project_and_adjust_y_fix(ceil_height, z_recip_v2);
            s16 x2_ybot = project_and_adjust_y_fix(floor_height, z_recip_v2);


            u8 tex_idx = cur_portal_map->wall_colors[(portal_index<<WALL_COLOR_NUM_PARAMS_SHIFT)+WALL_TEXTURE_IDX];
            u8 is_solid_color = cur_portal_map->wall_colors[(portal_index<<WALL_COLOR_NUM_PARAMS_SHIFT)+WALL_SOLID_COLOR_IDX];
            
            texmap_params tmap_info;
            if(is_solid_color) { 
                tmap_info.needs_texture = 0;
            } else {
                s32 base_left_u_16 = 0;
                s32 base_right_u_16 = (1)<<16;
                s32 fix_du_16 = (base_right_u_16-base_left_u_16);
                s16 dz_12_4 = z2 - z1;
                s32 du_over_dz_16 = 0;

                if(dz_12_4 != 0) {
                    du_over_dz_16 = divs_32_by_16(fix_du_16<<TRANS_Z_FRAC_BITS, dz_12_4);     
                }    
                tmap_info.repetitions = cur_portal_map->wall_tex_repetitions[portal_index];
                tmap_info.needs_texture = 1;
                tmap_info.tex = get_texture(tex_idx, light_level);
                tmap_info.du_over_dz = du_over_dz_16;
                tmap_info.left_u = base_left_u_16;
                tmap_info.right_u = base_right_u_16;
            }

            if(is_portal) {
                // do block comment start here
                s16 x1_pegged, x2_pegged;
                //KLog("skipping portal");            
                u16 neighbor_sect_group = sector_group(adj_sector_idx, cur_portal_map);
                s16* neighbor_sect_group_param_pointer = get_sector_group_pointer(neighbor_sect_group);

                s16 neighbor_floor_height = neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_FLOOR_HEIGHT_IDX]; //get_sector_group_floor_height(neighbor_sect_group);
                s16 neighbor_ceil_height = neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_CEIL_HEIGHT_IDX]; //get_sector_group_ceil_height(neighbor_sect_group);


                if(neighbor_floor_height <= floor_height) {
                    //draw_floor_update_clip(x1, x1_ybot, x2, x2_ybot,
                    //                    min(z_recip_v1, z_recip_v2),
                    //                    0, RENDER_WIDTH, &floor_params);
                }

                u8 neighbor_sector_group_type = GET_SECTOR_GROUP_TYPE(cur_portal_map->sector_group_types[neighbor_sect_group]);
                if(neighbor_ceil_height < ceil_height) {

                    s16 nx1_ytop = project_and_adjust_y_fix(neighbor_ceil_height, z_recip_v1);
                    s16 nx2_ytop = project_and_adjust_y_fix(neighbor_ceil_height, z_recip_v2);

                    if(neighbor_sector_group_type == DOOR) {

                        s16 orig_door_height = neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_ORIG_HEIGHT_IDX]; //get_sector_group_orig_height(neighbor_sect_group);
                        s16 orig_height_diff = orig_door_height - neighbor_floor_height;
                        x1_pegged = project_and_adjust_y_fix(neighbor_ceil_height+orig_height_diff, z_recip_v1);
                        x2_pegged = project_and_adjust_y_fix(neighbor_ceil_height+orig_height_diff, z_recip_v2);
                    }

                    u8 upper_color = cur_portal_map->wall_colors[(portal_index<<WALL_COLOR_NUM_PARAMS_SHIFT)+WALL_HIGH_COLOR_IDX];
                    // draw step from ceiling
                    //draw_upper_step(x1, x1_ytop, nx1_ytop, x2, x2_ytop, nx2_ytop,
                    //                z_recip_v1, z_recip_v2,
                    //                window_min, window_max, upper_color, light_level, &ceil_params);

                    if(neighbor_sector_group_type == DOOR && !is_solid_color) {
                        //continue;
                        draw_top_pegged_textured_upper_step(x1, x1_ytop, nx1_ytop, x2, x2_ytop, nx2_ytop,
                                                            z1, z2,
                                                            z_recip_v1, z_recip_v2,
                                                            0, RENDER_WIDTH, light_level,
                                                            &tmap_info,
                                                            &ceil_params, x1_pegged, x2_pegged);
                    } else {
                        draw_upper_step(x1, x1_ytop, nx1_ytop, x2, x2_ytop, nx2_ytop,
                                        z_recip_v1, z_recip_v2,
                                        0, RENDER_WIDTH, upper_color, light_level, &ceil_params);

                    }
                } else {
                    //u16 neighbor_ceil_color = neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_CEIL_COLOR_IDX]; //get_sector_group_ceil_color(neighbor_sect_group);
                    //u16 neighbor_light_level = neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_LIGHT_IDX];
                    //if(neighbor_ceil_color != ceil_color || neighbor_ceil_height != ceil_height || !recur || neighbor_light_level != light_level) {
                        draw_ceiling_update_clip(x1, x1_ytop, x2, x2_ytop,
                                                min(z_recip_v1, z_recip_v2),
                                                0, RENDER_WIDTH, &ceil_params);
                    //}
                }

                if(neighbor_floor_height > floor_height) { //} && neighbor_floor_height <= ceil_height) {
                    s16 nx1_ybot = project_and_adjust_y_fix(neighbor_floor_height, z_recip_v1);
                    s16 nx2_ybot = project_and_adjust_y_fix(neighbor_floor_height, z_recip_v2);

                    if(neighbor_sector_group_type == LIFT) {
                        u16 neighbor_state = neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_STATE_IDX];
                        if(neighbor_state == GOING_DOWN || neighbor_state == GOING_UP || neighbor_state == OPEN) {
                            s16 diff = neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_SCRATCH_ONE_IDX] - 
                                    floor_height; //neighbor_sect_group_param_pointer[SECTOR_GROUP_PARAM_SCRATCH_TWO_IDX];
                            x1_pegged = project_and_adjust_y_fix(neighbor_floor_height-diff, z_recip_v1);
                            x2_pegged = project_and_adjust_y_fix(neighbor_floor_height-diff, z_recip_v2);

                        } else { // if (neighbor_state == CLOSED) {
                            x1_pegged = x1_ybot;
                            x2_pegged = x2_ybot;
                        }
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
                    
                    // this is redundant
                    //    draw_floor_update_clip(x1, x1_ybot, x2, x2_ybot,
                    //                        min(z_recip_v1, z_recip_v2),
                    //                        window_min, window_max, &floor_params);
                    
                    //}
                }
                // do block comment end here

            } else {
                //KLog("- - Drawing wall ");
                //texmap_params tmap_info;
                //tmap_info.du_over_dz = 0;
                //tmap_info.needs_texture = 0;

                if(is_solid_color) {
                    draw_solid_color_wall(
                        x1, x1_ytop, x1_ybot, x2, x2_ytop, x2_ybot,
                        z_recip_v1, z_recip_v2, window_min, window_max,
                        light_level, tex_idx, &floor_params, &ceil_params);
                } else {   

                    draw_wall(
                        x1, x1_ytop, x1_ybot, x2, x2_ytop, x2_ybot,
                        z1, z2, z_recip_v1, z_recip_v2, window_min, window_max, 
                        light_level, &tmap_info, &floor_params, &ceil_params
                    );
                }
            }

            if(x1 <= 0 && x2 > window_min) {
                window_min = x2;
                if(window_min >= RENDER_WIDTH) {
                    return;
                }
            }
            if(x2 >= window_max && x1 < window_max) {
                window_max = x1;
                if(window_max <= 0) {
                    return;
                }
            }
        }
        //KLog("- Done drawing bunch.");
    }
    //KLog("Done drawing bunches.");
}
*/