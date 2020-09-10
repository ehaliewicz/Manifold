#include <genesis.h>
#include "game_mode.h"
#include "graphics_res.h"
#include "joy_helper.h"
#include "level.h"
#include "music.h"
#include "music_res.h"
#include "menu_helper.h"


fix32 cur_player_x, cur_player_y;
fix32 prescaled_player_x, prescaled_player_y;
fix16 playerXFrac4, playerYFrac4;
fix32 cur_player_angle;

fix32 angleCos32, angleSin32;
fix16 angleCos16, angleSin16;
fix16 angleSinFrac12, angleCosFrac12;

const fix32 angle_90_degrees = 256;
const fix32 angle_58_degrees = 164; // 58 degrees from player viewpoint to top left of map


const u16 gamePalette[16] = {
    RGB24_TO_VDPCOLOR(0),
    RGB24_TO_VDPCOLOR(0xFF0000),
    RGB24_TO_VDPCOLOR(0xFFFF00),
    RGB24_TO_VDPCOLOR(0xAF0000),
    RGB24_TO_VDPCOLOR(0xAFAF00),
    RGB24_TO_VDPCOLOR(0),
    RGB24_TO_VDPCOLOR(0),
    RGB24_TO_VDPCOLOR(0),
    RGB24_TO_VDPCOLOR(0),
    RGB24_TO_VDPCOLOR(0),
    RGB24_TO_VDPCOLOR(0),
    RGB24_TO_VDPCOLOR(0),
    RGB24_TO_VDPCOLOR(0),
    RGB24_TO_VDPCOLOR(0),
    RGB24_TO_VDPCOLOR(0),
    RGB24_TO_VDPCOLOR(0xFFFFFF),
};


u8* processed_linedef_bitmap;


void init_processed_linedef_bitmap() {
    int num_linedefs = cur_level->num_linedefs;
    processed_linedef_bitmap = MEM_alloc(num_linedefs/8+1);
}




u8 is_linedef_processed(u16 linedef_idx) {
    u16 byte_idx = linedef_idx>>3;
    u8 bit_idx = linedef_idx & 0b111;
    u8 byte = processed_linedef_bitmap[byte_idx];
    if(byte == 0) {
        return 0;
    }
    u8 bit_mask = (1<<bit_idx);
    return byte & bit_mask;
    
}

void set_linedef_processed(u16 linedef_idx) {
    u16 byte_idx = linedef_idx>>3;
    u8 bit_idx = linedef_idx & 0b111;
    processed_linedef_bitmap[byte_idx] |= (1<<bit_idx);
}

void clear_linedef_processed_bitmap() {
    int num_linedefs = cur_level->num_linedefs;
    memset(processed_linedef_bitmap, 0, num_linedefs/8+1);
}

#define MAX_TRANSFORM_CACHE 4


Vect2D_s16* vertex_transform_cache = NULL;
u8* vertex_transform_cache_map = NULL;
int num_cached_vertexes = 0;

void init_vertex_transform_cache() {
    vertex_transform_cache = MEM_alloc(sizeof(Vect2D_s16) * (MAX_TRANSFORM_CACHE+1));
    vertex_transform_cache_map = MEM_alloc(cur_level->num_vertexes);
    num_cached_vertexes = 0;
}


inline int is_vertex_cached(u16 vertex_idx) {
    return vertex_transform_cache_map[vertex_idx];
}

void clear_vertex_transform_cache() {
    memset(vertex_transform_cache_map, 0, cur_level->num_vertexes);
    num_cached_vertexes = 1;
}

void cache_transformed_vertex(int vertex_idx, Vect2D_s16 v) {
    int cached = is_vertex_cached(vertex_idx);
    if((!cached) && num_cached_vertexes < MAX_TRANSFORM_CACHE) {
        int assigned_idx = num_cached_vertexes++;
        vertex_transform_cache_map[vertex_idx] = assigned_idx;
        vertex_transform_cache[assigned_idx] = v;
    }
}

inline Vect2D_s16 get_transformed_vertex(int vertex_idx) {
    return vertex_transform_cache[vertex_transform_cache_map[vertex_idx]];
}


typedef enum {
    INSIDE = 0b00000,
    LEFT   = 0b00010,
    RIGHT  = 0b00100,
    BOTTOM = 0b01000,
    TOP    = 0b10000
} outcode;

typedef enum {
    OFFSCREEN=0,
    ONSCREEN=1,
} clip_result;

const s16 int_max_x = BMP_WIDTH-1;
const s16 int_max_y = BMP_HEIGHT-1;
const s16 subpixel_max_x = (BMP_WIDTH-1)<<4;
const s16 subpixel_max_y = (BMP_HEIGHT-1)<<4;

s16 round_subpix(s16 subpix_coord) {
    s16 shift_three = subpix_coord>>3;

    //s16 subpix_bits = subpix_coord & 0b1100;

    //s16 round = (subpix_coord & 0b1100) ? 1 : 0;
    return (shift_three>>1) + (shift_three&0b1);

}

s16 div_numer, div_denom;
s16 fast_div_native();

s16 fast_div(s16 numer, s16 denom) {
    if(numer < 0) {
        if(denom < 0) {
            div_numer = -numer;
            div_denom = -denom;
            return fast_div_native();
        } else {
            div_numer = -numer;
            div_denom = denom;
            return -fast_div_native();
        }
    } else if (denom < 0) {
        div_numer = numer;
        div_denom = -denom;
        return -fast_div_native();
    } else {
        div_numer = numer;
        div_denom = denom;
        return fast_div_native();
    }

}


//const int zoom = 4;

//#define ZOOM 4
//#define ZOOM_SHIFT 2
#define ZOOM 4
#define ZOOM_SHIFT 2



static int pause_game = 0;
static int quit_game = 0;

extern s16 trans_vert_x;
extern s16 trans_vert_y;
s16 asm_x_shifted;
s16 asm_y_shifted;
s16 asm_tlx;
s16 asm_tly; 
Vect2D_s16 transform_vert_native(s16 x, s16 y);
  
Vect2D_s16 transform_res;

Vect2D_s16 inner_transform_vert(s16 x, s16 y) {
    //Vect2D_s16 res;
    return transform_vert_native(x, y);
    //return res;

    //Vect2D_s16 res = {.x = trans_vert_x, .y = trans_vert_y};
    //return res;
    s16 x_shifted = ((x<<4)>>ZOOM_SHIFT);
    s16 y_shifted = ((y<<4)>>ZOOM_SHIFT);
    s16 tlx = x_shifted - playerXFrac4;
    s16 tly = y_shifted - playerYFrac4; // scaling factor of 64 (2^6)
    
    fix32 rx = ((tlx)*(angleCosFrac12)) - ((tly)*(angleSinFrac12)); // 12.4 * 1.12 = 13.16? result I know it's 16 bits of fractional precision
    fix32 ry = ((tlx)*(angleSinFrac12)) + ((tly)*(angleCosFrac12));


    s16 trx = ((rx>>16) + (BMP_WIDTH>>1));
    //s16 try = (int_max_y)-((ry>>16) + (BMP_HEIGHT>>1));
    s16 try = ((ry>>16) + (BMP_HEIGHT>>1))-(159);
    Vect2D_s16 vert = {.x = trx, .y = -try};

    
    return vert;
}


Vect2D_s16 transform_vert(u16 v_idx) {
    if(is_vertex_cached(v_idx)) {
        return get_transformed_vertex(v_idx);
    }
    vertex v = cur_level->vertexes[v_idx];
    s16 x = v.x;
    s16 y = v.y;   
    return inner_transform_vert(x, y);    
}               

typedef enum {
    SINGLE_LINEDEF = 0,
    POLY_LINEDEF = 1
} linedef_segment_type;

void draw_blockmap_cell(Line lin, u16* cell_ptr) {
    u16 num_linedefs = *cell_ptr++;

    for(u16 i = 0; i < num_linedefs; i++) {
        u16 linedef_byte_idx = *cell_ptr++;    
        u8 byte = processed_linedef_bitmap[linedef_byte_idx];
        u16 bit_mask_and_is_portal = *cell_ptr++;
        u8 bit_mask = bit_mask_and_is_portal>>8;
        if(byte == 0 || ((byte & bit_mask) == 0)) {
            processed_linedef_bitmap[linedef_byte_idx] |= bit_mask;
        } else {
        //if(byte == 0xFF || byte & bit_mask) {
            // skip rest of linedef
            // skip v1_index, v1x, v1y
            cell_ptr += 3;
            // skip v2_index, v2x, v2y
            cell_ptr += 3;
            //linedefs_skipped++;
            continue;
        }

        u8 is_portal = bit_mask_and_is_portal&0xFF; 
        u16 v1 = *cell_ptr++;
        Vect2D_s16 tv1;

        //u8 vertex1_cached = vertex_transform_cache_map[v1];
        if(0) { // is_vertex_cached(v1)) {
            cell_ptr += 2; // skip over inline vertex x and y
            //verts_reused++;

            //tv1 = vertex_transform_cache[vertex1_cached];
        } else {
            u16 v1x = *cell_ptr++;
            u16 v1y = *cell_ptr++;
            tv1 = inner_transform_vert(v1x, v1y);
        }
        u16 v2 = *cell_ptr++;
        Vect2D_s16 tv2;
        //u8 vertex2_cached = vertex_transform_cache_map[v2];
        if(0) { //is_vertex_cached(v2)) {
            //verts_reused++;
            cell_ptr += 2; // skip over inline vertex x and y
            //tv2 = vertex_transform_cache[vertex2_cached]; //get_transformed_vertex(v2);
        } else {
            u16 v2x = *cell_ptr++;
            u16 v2y = *cell_ptr++;
            tv2 = inner_transform_vert(v2x, v2y);
        }

        //Vect2D_s16 tv1 = transform_vert(line.v1);
        //Vect2D_s16 tv2 = transform_vert(line.v2);


        u8 col = is_portal ? 0x11 : 0x22;

        lin.pt1.x = tv1.x; 
        lin.pt1.y = tv1.y;
        lin.pt2.x = tv2.x;
        lin.pt2.y = tv2.y;
        lin.col = col;


        if(BMP_clipLine(&lin)) {
            //lines_onscreen++;
            //verts_cached+=2;
            //verts_drawn+=2;
            //cache_transformed_vertex(v1, tv1);
            //cache_transformed_vertex(v2, tv2);

            BMP_drawLine(&lin);
            
        }
        
    }

}

void draw_automap() {
    clear_vertex_transform_cache();
    clear_linedef_processed_bitmap();
    BMP_waitWhileFlipRequestPending();
    BMP_clear();

    int lines_onscreen = 0;
    int verts_reused = 0;
    int linedefs_skipped = 0;
    int verts_drawn = 0;
    int verts_cached = 0;

    // inverse project coordinates

    // inverse project window corners and figure out the min/max x/y blockmap coordinates to render
    
    //const blockmap* blkmap = cur_level->blkmap;
    const render_blockmap* render_blkmap = cur_level->render_blkmap;
    int blockmap_min_x = render_blkmap->x_origin;
    int blockmap_min_y = render_blkmap->y_origin;
    //int blockmap_min_x = big_blkmap->x_origin;
    //int blockmap_min_y = big_blkmap->y_origin;

    //int blockmap_max_x = blockmap_min_x + (blkmap->num_columns*BLOCKMAP_CELL_SIZE);
    //int blockmap_max_y = blockmap_min_y + (blkmap->num_rows*BLOCKMAP_CELL_SIZE);
    

    /* calculate world-space coordinates for all four corners of the map screen 


    the angle from the center of the map to the top and left corners is 58 degrees

    calculate the x and y components of those vectors, scale by half the diagonal length of the screen, as well as the world->screen zoom factor

    mirror the top left vector to get the bottom right vector, and the top right vector to get the bottom left vector

    from this point, we can easily find the min and max x and y blockmap coordinates to iterate over.

    */
    
    int world_half_screen_diag_length = 152 << ZOOM_SHIFT;

    fix32 top_left_angle = (cur_player_angle-angle_58_degrees);
    fix32 top_left_angle_sin = sinFix32(top_left_angle);
    fix32 top_left_angle_cos = cosFix32(top_left_angle);

    fix32 top_left_x_off = top_left_angle_sin * world_half_screen_diag_length;
    fix32 top_left_y_off = top_left_angle_cos * world_half_screen_diag_length;

    fix32 world_screen_top_left_x = cur_player_x + top_left_x_off;
    fix32 world_screen_top_left_y = cur_player_y + top_left_y_off;

    fix32 world_screen_bottom_right_x = cur_player_x - top_left_x_off;
    fix32 world_screen_bottom_right_y = cur_player_y - top_left_y_off;


    fix32 top_right_angle = (cur_player_angle+angle_58_degrees);
    fix32 top_right_angle_sin = sinFix32(top_right_angle);
    fix32 top_right_angle_cos = cosFix32(top_right_angle);

    fix32 top_right_x_off = top_right_angle_sin * world_half_screen_diag_length;
    fix32 top_right_y_off = top_right_angle_cos * world_half_screen_diag_length;

    fix32 world_screen_top_right_x = cur_player_x + top_right_x_off;
    fix32 world_screen_top_right_y = cur_player_y + top_right_y_off;

    fix32 world_screen_bottom_left_x = cur_player_x - top_right_x_off;
    fix32 world_screen_bottom_left_y = cur_player_y - top_right_y_off;

    

    fix32 min_world_x = min(min(world_screen_top_left_x, world_screen_top_right_x),
                            min(world_screen_bottom_left_x, world_screen_bottom_right_x));

    fix32 max_world_x = max(max(world_screen_top_left_x, world_screen_top_right_x),
                            max(world_screen_bottom_left_x, world_screen_bottom_right_x));


    fix32 min_world_y = min(min(world_screen_top_left_y, world_screen_top_right_y),
                            min(world_screen_bottom_left_y, world_screen_bottom_right_y));

    fix32 max_world_y = max(max(world_screen_top_left_y, world_screen_top_right_y),
                            max(world_screen_bottom_left_y, world_screen_bottom_right_y));



    int min_blockmap_x = fix32ToRoundedInt(min_world_x-intToFix32(blockmap_min_x))/BLOCKMAP_CELL_SIZE;
    int max_blockmap_x = fix32ToRoundedInt(max_world_x-intToFix32(blockmap_min_x))/BLOCKMAP_CELL_SIZE;
    int min_blockmap_y = fix32ToRoundedInt(min_world_y-intToFix32(blockmap_min_y))/BLOCKMAP_CELL_SIZE;
    int max_blockmap_y = fix32ToRoundedInt(max_world_y-intToFix32(blockmap_min_y))/BLOCKMAP_CELL_SIZE;


    Line lin;
    int y_start = max(0, min_blockmap_y);
    int y_end = min(max_blockmap_y, render_blkmap->num_rows-1);
    int x_start = max(0, min_blockmap_x);
    int x_end = min(max_blockmap_x, render_blkmap->num_columns-1);

    int num_cols = render_blkmap->num_columns;
    int blockmap_y_off = y_start * num_cols;
    for(int y = y_start; y <= y_end; y++) {
        for(int x = x_start; x <= x_end; x++) {


            int blockmap_offset_idx = blockmap_y_off+x;
            
            int blockmap_table_idx = render_blkmap->offsets_plus_table[blockmap_offset_idx];
            if(blockmap_table_idx == 0) {
                continue;
            }

            const u16* cell_ptr =  &(render_blkmap->offsets_plus_table[blockmap_table_idx]);
            draw_blockmap_cell(lin, cell_ptr);
            /*
            while(1) {
                u16 linedef_idx = *linedef_list_ptr++;
                if(linedef_idx == 0xFFFF) { break; }

   

                if(is_linedef_processed(linedef_idx)) { linedefs_skipped++; continue; }
                set_linedef_processed(linedef_idx);
                
                linedef line = cur_level->linedefs[linedef_idx];
                int is_portal = line.left_sidedef != 0xFFFF && line.right_sidedef != 0xFFFF;

                Vect2D_s16 tv1 = transform_vert(line.v1);
                Vect2D_s16 tv2 = transform_vert(line.v2);


                u8 col = is_portal ? 0x11 : 0x22;

                lin.pt1.x = tv1.x; 
                lin.pt1.y = tv1.y;
                lin.pt2.x = tv2.x;
                lin.pt2.y = tv2.y;
                lin.col = col;


                if(BMP_clipLine(&lin)) {
                    lines_onscreen++;
                    cache_transformed_vertex(line.v1, tv1);
                    cache_transformed_vertex(line.v2, tv2);

                    BMP_drawLine(&lin);
                    
                }
            
            }
            */
        }
        
        blockmap_y_off += num_cols;

    }


    lin.col = 0xFF;
   // draw border
    memset(bmp_buffer_write, 0xFF, 128);
    memset(bmp_buffer_write+((BMP_HEIGHT-1)*128), 0xFF, 128);
    lin.pt1.x = 0; lin.pt1.y = 0;
    lin.pt2.x = 0; lin.pt2.y = (BMP_HEIGHT-1);
    //draw_subpixel_line(&lin);
    BMP_drawLine(&lin);
    lin.pt1.x = (BMP_WIDTH-1); lin.pt1.y = 0;
    lin.pt2.x = (BMP_WIDTH-1); lin.pt2.y = (BMP_HEIGHT-1);
    BMP_drawLine(&lin);
    //draw_subpixel_line(&lin);
    
    
    Vect2D_s16 pts[3] = {
        {.x = BMP_WIDTH/2, .y = (BMP_HEIGHT/2)-2},
        {.x = (BMP_WIDTH/2)+2, .y = (BMP_HEIGHT/2)+2},
        {.x = (BMP_WIDTH/2)-2, .y = (BMP_HEIGHT/2)+2},
    };
    
    BMP_drawPolygon(pts, 3, 0x11);


    BMP_showFPS(1);
    
    //char buf[32];
    //sprintf(buf, "lines drawn %i.", lines_onscreen);
    //BMP_drawText(buf, 0, 6);
    //sprintf(buf, "vertexes cached %i.", num_cached_vertexes-1);
    //BMP_drawText(buf, 0, 7);
    //sprintf(buf, "vertexes reused %i.", verts_reused);
    //BMP_drawText(buf, 0, 8);
    //sprintf(buf, "vertexes drawn %i.", verts_drawn);
    //BMP_drawText(buf, 0, 9);
    //sprintf(buf, "linedefs skipped %i.", linedefs_skipped);
    //BMP_drawText(buf, 0, 10);
    

    //int y_off = 4;
    //char buf[32];
    //u32 fps = SYS_getFPS();

    //sprintf(buf, "-%i-", fps);
    //VDP_drawTextBG(BG_A, buf, 0, y_off++);


    BMP_flip(1);

    //u8* bmp = BMP_getWritePointer(0, 0);
    
    //DMA_doDma(DMA_VRAM, bmp, BMP_FB0TILE, (BMP_FB0ENDTILEINDEX-BMP_FB0TILE)/2, 4);
    //DMA_doDma(DMA_VRAM, bmp, BMP_FB0TILE+(2*32), (BMP_FB0ENDTILEINDEX-BMP_FB0TILE)/2, 4);
}

const fix32 move_speed = 15;

void handle_input() {
    int strafe = joy_button_pressed(BUTTON_C);

    if(joy_button_pressed(BUTTON_DOWN)) {
        cur_player_y -= angleCos32*move_speed;
        cur_player_x -= angleSin32*move_speed;
        
    } else if (joy_button_pressed(BUTTON_UP)) {
        cur_player_y += angleCos32*move_speed;
        cur_player_x += angleSin32*move_speed;
    }
    

    if (joy_button_pressed(BUTTON_LEFT)) {
        if(strafe) {
            fix32 leftAngle = (cur_player_angle-angle_90_degrees);
            fix32 leftAngleSin = sinFix32(leftAngle);
            fix32 leftAngleCos = cosFix32(leftAngle);
            cur_player_y += leftAngleCos*move_speed;
            cur_player_x += leftAngleSin*move_speed;
        } else {
            cur_player_angle -= 8;
        }
    } else if (joy_button_pressed(BUTTON_RIGHT)) {
        if(strafe) {
            fix32 leftAngle = (cur_player_angle+angle_90_degrees);
            fix32 leftAngleSin = sinFix32(leftAngle);
            fix32 leftAngleCos = cosFix32(leftAngle);
            cur_player_y += leftAngleCos*move_speed;
            cur_player_x += leftAngleSin*move_speed;
        } else {
            cur_player_angle += 8;
        }
    }

    if(joy_button_held(BUTTON_START)) {
        pause_game = 1;
    } else {
        pause_game = 0;
    }

}


void return_to_menu() {
    quit_game = 1;
    pause_game = 0;
}

const menu game_menu = {
    .header_text = "Pause",
    .num_items = 2,
    .items = {
        {.text = "Restart level", .submenu = NULL, .select = NULL, .render = NULL, .selectable = 1},
        {.text = "Return to menu", .submenu = NULL, .select = &return_to_menu, .render = NULL, .selectable = 1},
    }
};

void init_pause_menu() {

}

game_mode run_pause_menu() {
    return PAUSE_MENU;
}

void cleanup_pause_menu() {
    
}

void init_game() {
    quit_game = 0;
    if(pause_game) {
        pause_game = 0;
    } else {
        cur_player_x = intToFix32(cur_level->things[0].x);
        cur_player_y = intToFix32(cur_level->things[0].y);
        cur_player_angle = 0;
    }

    VDP_setPalette(PAL1, gamePalette);

    clear_menu();

    BMP_init(0, BG_B, PAL1, 0);
    BMP_clear();
    BMP_setBufferCopy(0);
    BMP_flip(0);
    init_processed_linedef_bitmap(); 
    init_vertex_transform_cache();
    XGM_stopPlay();
    if(music_on) {
        XGM_startPlay(xgm_e1m4);
    }
}


game_mode run_game() {

    angleCos32 = cosFix32(cur_player_angle);
    angleSin32 = sinFix32(cur_player_angle); 
    angleCos16 = cosFix16(cur_player_angle);
    angleSin16 = sinFix16(cur_player_angle); 
    angleSinFrac12 = angleSin16<<6;
    angleCosFrac12 = angleCos16<<6;

    prescaled_player_x = ((cur_player_x>>ZOOM_SHIFT)>>(FIX32_FRAC_BITS-6)); // scaling factor is now 64 (2^6) instead of 1024 (2^10)
    prescaled_player_y = ((cur_player_y>>ZOOM_SHIFT)>>(FIX32_FRAC_BITS-6));
    playerXFrac4 = ((cur_player_x>>ZOOM_SHIFT)>>(FIX32_FRAC_BITS-4)); // scaling factor is now 16 (2^4) instead of 1024 (2^10)
    playerYFrac4 = ((cur_player_y>>ZOOM_SHIFT)>>(FIX32_FRAC_BITS-4));
    handle_input();

    //JOY_waitPress
    /*
    if(joy_button_newly_pressed(BUTTON_START)) { ////pause_game) {
        while(1) {}
        return PAUSE_MENU;
    } else if (quit_game) {
        return MAIN_MENU;
    }
    */
   
    draw_automap();

    return SAME_MODE;
}

void cleanup_game() {
    BMP_end();
    MEM_free(processed_linedef_bitmap);
    //MEM_free(vertex_transform_cache_map);
    //MEM_free(vertex_transform_cache);
    MEM_pack();
}