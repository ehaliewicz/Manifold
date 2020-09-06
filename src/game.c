#include <genesis.h>
#include "game_mode.h"
#include "Graphics_res.h"
#include "joy_helper.h"
#include "level.h"
#include "music.h"
#include "music_res.h"


fix32 cur_player_x, cur_player_y;
fix32 cur_player_angle;

fix32 angleCos32, angleSin32;
fix16 angleCos16, angleSin16;

u8* drawn_linedef_bitmap;

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



int is_linedef_drawn(u16 linedef_idx) {
    u16 byte_idx = linedef_idx/8;
    u8 bit_idx = linedef_idx - byte_idx*8;
    return drawn_linedef_bitmap[byte_idx] & (1<<bit_idx);
}

void set_linedef_drawn(u16 linedef_idx) {
    u16 byte_idx = linedef_idx/8;
    u8 bit_idx = linedef_idx - byte_idx*8;
    drawn_linedef_bitmap[byte_idx] |= (1<<bit_idx);
}

typedef struct {
    s16 x1, y1; // 4 bits of subpixel resolution
    s16 x2, y2;
    u8 col;
} line;

typedef enum {
    INSIDE = 0b00000,
    LEFT   = 0b00010,
    RIGHT  = 0b00100,
    BOTTOM = 0b01000,
    TOP    = 0b10000
} outcode;

typedef enum {
    ONSCREEN,
    OFFSCREEN
} clip_result;

const s16 int_max_x = BMP_WIDTH-1;
const s16 int_max_y = BMP_HEIGHT-1;
const s16 subpixel_max_x = (BMP_WIDTH-1)<<4;
const s16 subpixel_max_y = (BMP_HEIGHT-1)<<4;

u8 compute_outcode(s16 x, s16 y, s16 max_x, s16 max_y) {
    u8 code = INSIDE;
    if(x < 0) {
        code |= LEFT;
    } else if (x > max_x) {
        code |= RIGHT;
    }
    if(y < 0) {
        code |= TOP;
    } else if (y > max_y) {
        code |= BOTTOM;
    }
    return code;
}

clip_result internal_clip_line(line* l, s16 max_x, s16 max_y) {

    s16 x1 = l->x1; s16 x2 = l->x2;
    s16 y1 = l->y1; s16 y2 = l->y2;

    u8 outcode0 = compute_outcode(x1, y1, max_x, max_y);
    u8 outcode1 = compute_outcode(x2, y2, max_x, max_y);

    clip_result res = OFFSCREEN;
    while(1) {
        if(!(outcode0 | outcode1)) {
            // trivial accept, both points are inside window
            res = ONSCREEN;
            break;
        } else if (outcode0 & outcode1) {
            res = OFFSCREEN;
            break;
        }

        // At least one endpoint is outside the clip rectangle; pick it.
        outcode code_out = outcode1 > outcode0 ? outcode1 : outcode0;

        s32 new_x, new_y;  
        if (code_out & TOP) {     
            new_x = x1 + (x2-x1) * (0 - y1) / (y2-y1);
            new_y = 0;
        } else if (code_out & BOTTOM) { 
            new_x = x1 + (x2-x1) * (max_y - y1) / (y2-y1);
            new_y = max_y;
        } else if (code_out & RIGHT) {  
            new_y = y1 + (y2-y1) * (max_x-x1) / (x2-x1);
            new_x = max_x;
        } else if (code_out & LEFT) {  
            new_y = y1 + (y2-y1) * (0-x1) / (x2-x1);
            new_x = 0;
        }

        if (code_out == outcode0) {
            x1 = new_x;
            y1 = new_y;
            outcode0 = compute_outcode(x1, y1, max_x, max_y);
        } else {
            x2 = new_x;
            y2 = new_y;
            outcode1 = compute_outcode(x2, y2, max_x, max_y);
        }

    }

    if(res == ONSCREEN) {
        l->x1 = x1;
        l->y1 = y1;
        l->x2 = x2;
        l->y2 = y2;
    }
    return res;
    
}

clip_result clip_line(line* l) {
    return internal_clip_line(l, int_max_x, int_max_y);
}

clip_result subpixel_clip_line(line* l) {
    return internal_clip_line(l, subpixel_max_x, subpixel_max_y);
}


void init_game() {
    VDP_setPalette(PAL1, gamePalette);

    BMP_init(0, BG_A, PAL1, 0);
    cur_player_x = intToFix32(cur_level->things[0].x);
    cur_player_y = intToFix32(cur_level->things[0].y);
    cur_player_angle = 0;
    int num_linedefs = cur_level->num_linedefs;

    drawn_linedef_bitmap = MEM_alloc(num_linedefs/8+1);
    XGM_stopPlay();
    if(music_on) {
        XGM_startPlay(xgm_e1m4);
    }
}

s16 round_subpix(s16 subpix_coord) {
    s16 round = (subpix_coord & 0b1100) ? 1 : 0;
    return (subpix_coord>>4) + round;
}

void draw_automap() {
    memset(drawn_linedef_bitmap, 0, cur_level->num_linedefs/8+1);
    BMP_waitWhileFlipRequestPending();
    BMP_clear();


    // inverse project coordinates
    const int zoom = 4;
    const int width = BMP_WIDTH;
    const int height = BMP_HEIGHT;

    // TODO: inverse project window corners and figure out the min/max x/y blockmap coordinates to render
    blockmap* blkmap = cur_level->blkmap;
    int blockmap_min_x = blkmap->x_origin;
    int blockmap_min_y = blkmap->y_origin;
    int blockmap_max_x = blockmap_min_x + (blkmap->num_columns*BLOCKMAP_CELL_SIZE);
    int blockmap_max_y = blockmap_min_y + (blkmap->num_rows*BLOCKMAP_CELL_SIZE);
    

    int int_player_x = fix32ToInt(cur_player_x);
    int int_player_y = fix32ToInt(cur_player_y);

    int screen_left = int_player_x - (width*zoom);
    int screen_right = int_player_x + (width*zoom);
    int screen_top = int_player_y - (height*zoom);
    int screen_bot = int_player_y + (height*zoom);

    //int left_blockmap_x = (screen_left-blockmap_min_x)/BLOCKMAP_CELL_SIZE;
    //int right_blockmap_x = ((screen_right-blockmap_min_x)/BLOCKMAP_CELL_SIZE)+1;
    //int up_blockmap_y = (screen_top-blockmap_min_y)/BLOCKMAP_CELL_SIZE;
    //int down_blockmap_y = ((screen_bot-blockmap_min_y)/BLOCKMAP_CELL_SIZE)+1;

    
    int blockmap_x = ((int_player_x-blockmap_min_x)/BLOCKMAP_CELL_SIZE);
    int blockmap_y = ((int_player_y-blockmap_min_y)/BLOCKMAP_CELL_SIZE);
    

    line lin;
    Line l;
    for(int y = blockmap_y-3; y <= blockmap_y+3; y++) {
        if(y < 0) { continue; }
        if(y >= blkmap->num_rows) { break; }
        for(int x = blockmap_x-3; x <= blockmap_x+3; x++) {
            if(x < 0) { continue; }
            if(x >= blkmap->num_columns) { break; }
            int blockmap_offset_idx = (y * blkmap->num_columns) + x;
            int blockmap_table_idx = blkmap->offsets_plus_table[blockmap_offset_idx];
            int blockmap_next_table_idx = blkmap->offsets_plus_table[blockmap_offset_idx+1];
            u16* linedef_list_ptr = &(blkmap->offsets_plus_table[blockmap_table_idx]);
            int num_linedefs = blockmap_next_table_idx-blockmap_table_idx;

            for(int i = 0; i < num_linedefs; i++) {
                u16 linedef_idx = *linedef_list_ptr++;
                if(linedef_idx == 0xFFFF) { break; }

                if(is_linedef_drawn(linedef_idx)) { continue; }
                set_linedef_drawn(linedef_idx);
                
                linedef line = cur_level->linedefs[linedef_idx];
                int is_portal = line.left_sidedef != 0xFFFF && line.right_sidedef != 0xFFFF;


                vertex v1 = cur_level->vertexes[line.v1];
                vertex v2 = cur_level->vertexes[line.v2];

                fix32 tl1x = (intToFix32(v1.x) - cur_player_x);
                fix32 tl1y = (intToFix32(v1.y) - cur_player_y);
                fix32 tl2x = (intToFix32(v2.x) - cur_player_x);
                fix32 tl2y = (intToFix32(v2.y) - cur_player_y);

                fix32 r1x = fix32Mul(tl1x, angleCos32) - fix32Mul(tl1y, angleSin32);
                fix32 r1y = fix32Mul(tl1x, angleSin32) + fix32Mul(tl1y, angleCos32);
                fix32 r2x = fix32Mul(tl2x, angleCos32) - fix32Mul(tl2y, angleSin32);
                fix32 r2y = fix32Mul(tl2x, angleSin32) + fix32Mul(tl2y, angleCos32);
                
                fix32 tr1x_subpix = (r1x/zoom + intToFix32(BMP_WIDTH/2));
                fix32 tr1y_subpix = (r1y/zoom + intToFix32(BMP_HEIGHT/2));
                fix32 tr2x_subpix = (r2x/zoom + intToFix32(BMP_WIDTH/2));
                fix32 tr2y_subpix = (r2y/zoom + intToFix32(BMP_HEIGHT/2));
                
                
                lin.x1 = (tr1x_subpix) >> (FIX32_FRAC_BITS-4);
                lin.y1 = (FIX32(int_max_y)-tr1y_subpix) >> (FIX32_FRAC_BITS-4);
                lin.x2 = (tr2x_subpix) >> (FIX32_FRAC_BITS-4);
                lin.y2 = (FIX32(int_max_y)-tr2y_subpix) >> (FIX32_FRAC_BITS-4);


                u8 col = is_portal ? 0x11 : 0x22;
                u8 bmp_clip_col = is_portal ? 0x33 : 0x44;

                clip_result clip_res = subpixel_clip_line(&lin);


                if(clip_res == ONSCREEN) {
                    Line l = {.pt1 = {.x = round_subpix(lin.x1), .y = round_subpix(lin.y1)}, .pt2 = {.x = round_subpix(lin.x2), .y = round_subpix(lin.y2)}, .col = col};
                    BMP_drawLine(&l);
                    
                }

                
            }

        }
    }


    l.col = 0xFF;
   // draw border
    memset(bmp_buffer_write, 0xFF, 128);
    memset(bmp_buffer_write+((BMP_HEIGHT-1)*128), 0xFF, 128);
    l.pt1.x = 0; l.pt1.y = 0;
    l.pt2.x = 0; l.pt2.y = BMP_HEIGHT-1;
    BMP_drawLine(&l);
    l.pt1.x = BMP_WIDTH-1; l.pt1.y = 0;
    l.pt2.x = BMP_WIDTH-1; l.pt2.y = BMP_HEIGHT-1;
    BMP_drawLine(&l);
    
    


    Vect2D_s16 pts[3] = {
        {.x = BMP_WIDTH/2, .y = (BMP_HEIGHT/2)-2},
        {.x = (BMP_WIDTH/2)+2, .y = (BMP_HEIGHT/2)+2},
        {.x = (BMP_WIDTH/2)-2, .y = (BMP_HEIGHT/2)+2},
    };
    
    BMP_drawPolygon(pts, 3, 0x11);

    //BMP_showFPS(1);
    BMP_flip(1);

}

const fix32 move_speed = 15;

void handle_input() {
    if(joy_button_held(BUTTON_DOWN)) {
        cur_player_y -= angleCos32*move_speed;
        cur_player_x -= angleSin32*move_speed;
        
    } else if (joy_button_held(BUTTON_UP)) {
        cur_player_y += angleCos32*move_speed;
        cur_player_x += angleSin32*move_speed;
    }
    
    if (joy_button_held(BUTTON_LEFT)) {
        cur_player_angle -= 8;
    } else if (joy_button_held(BUTTON_RIGHT)) {
        cur_player_angle += 8;
    }

}

game_mode run_game() {

    angleCos32 = cosFix32(cur_player_angle);
    angleSin32 = sinFix32(cur_player_angle); 
    angleCos16 = cosFix16(cur_player_angle);
    angleSin16 = sinFix16(cur_player_angle); 
    handle_input();

    
    draw_automap();

    return SAME_MODE;
}

void cleanup_game() {
    BMP_end();
    MEM_free(drawn_linedef_bitmap);
    MEM_pack();
}