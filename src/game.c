#include <genesis.h>
#include "automap.h"
#include "bsp.h"
#include "game.h"
#include "game_mode.h"
#include "graphics_res.h"
#include "joy_helper.h"
#include "level.h"
#include "math3d.h"
#include "music.h"
#include "music_res.h"
#include "menu_helper.h"
#include "span_buf.h"

fix32 cur_player_x, cur_player_y;
fix16 playerXFrac4, playerYFrac4;
fix32 cur_player_angle;

fix32 angleCos32, angleSin32;
fix16 angleCos16, angleSin16;
fix16 angleSinFrac12, angleCosFrac12;


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




u32* vertex_cache_frames;
u32* cached_vertexes;


void init_vertex_cache() {
    int num_vertexes = cur_level->num_vertexes;
    vertex_cache_frames = MEM_alloc((num_vertexes+4) * sizeof(u32));
    cached_vertexes = MEM_alloc((num_vertexes+4) * sizeof(Vect2D_s16));
    memsetU32(vertex_cache_frames, 0xFFFFFFFF, num_vertexes);
}


typedef enum {
    INSIDE = 0b00000,
    LEFT   = 0b00010,
    RIGHT  = 0b00100,
    BOTTOM = 0b01000,
    TOP    = 0b10000
} outcode;


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




static int pause_game = 0;
static int quit_game = 0;




const fix32 move_speed = 4*ZOOM; //15;

int cur_frame;
int automap_mode;
int draw_solid;



void draw_3d_view(u32 cur_frame) {

    linedef line = cur_level->linedefs[42];

    vertex v1 = cur_level->vertexes[line.v1];
    vertex v2 = cur_level->vertexes[line.v2];



    char buf[32];


    BMP_waitWhileFlipRequestPending();
    BMP_clear();

    traverse_bsp_nodes_front_to_back(cur_player_x, cur_player_y);

    //if(BMP_clipLine(&l)) {
    //    BMP_drawLine(&l);
    //}


    

    Vect2D_s32 trans_v1 = transform_map_vert(v1.x, v1.y);
    Vect2D_s32 trans_v2 = transform_map_vert(v2.x, v2.y);

    // clip against near z plane if necessary
    clip_result clipped = clip_map_vertex(&trans_v1, &trans_v2);

    if(clipped != OFFSCREEN) {
        // project map vertex with height attributes
        transformed_vert screen_v1 = project_and_adjust_3d(trans_v1, -20, 20);
        transformed_vert screen_v2 = project_and_adjust_3d(trans_v2, -20, 20);
        
        u8 col = clipped != UNCLIPPED ? 0x11 : 0x22;
        
        Line l3d_ceil = {.pt1 = {.x = screen_v1.x, .y = screen_v1.yceil}, .pt2 = {.x = screen_v2.x, .y = screen_v2.yceil}, .col = col};
        Line l3d_floor = {.pt1 = {.x = screen_v1.x, .y = screen_v1.yfloor}, .pt2 = {.x = screen_v2.x, .y = screen_v2.yfloor}, .col = col};

        Line l3d_left = {.pt1 = {.x = screen_v1.x, .y = screen_v1.yceil}, .pt2 = {.x = screen_v1.x, .y = screen_v1.yfloor}, .col = col};
        Line l3d_right = {.pt1 = {.x = screen_v2.x, .y = screen_v2.yceil}, .pt2 = {.x = screen_v2.x, .y = screen_v2.yfloor}, .col = col};



        if(draw_solid) {
            Vect2D_s16 pts[4] = {
                {screen_v1.x, screen_v1.yceil},
                {screen_v2.x, screen_v2.yceil},
                {screen_v2.x, screen_v2.yfloor},
                {screen_v1.x, screen_v1.yfloor}
            };

            BMP_drawPolygon(pts, 4, col);

        } else {
            if(BMP_clipLine(&l3d_ceil)) {
                BMP_drawLine(&l3d_ceil);
            }
            if(BMP_clipLine(&l3d_floor)) {
                BMP_drawLine(&l3d_floor);
            }
            if(BMP_clipLine(&l3d_left)) {
                BMP_drawLine(&l3d_left);
            }
            if(BMP_clipLine(&l3d_right)) {
                BMP_drawLine(&l3d_right);
            }
        }
        

    }
    




    //BMP_drawPolygon(playerPolygon, 3, 0x11);

    //sprintf(buf, "screen x %i,%i     ", scrn_x1, scrn_x2);
    //BMP_drawText(buf, 4, 11);


    BMP_showFPS(1);
    BMP_flip(1);
    return;
}



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
            fix32 leftAngle = (cur_player_angle-ANGLE_90_DEGREES);
            fix32 leftAngleSin = sinFix32(leftAngle);
            fix32 leftAngleCos = cosFix32(leftAngle);
            cur_player_y += leftAngleCos*move_speed;
            cur_player_x += leftAngleSin*move_speed;
        } else {
            cur_player_angle -= 10;
        }
    } else if (joy_button_pressed(BUTTON_RIGHT)) {
        if(strafe) {
            fix32 leftAngle = (cur_player_angle+ANGLE_90_DEGREES);
            fix32 leftAngleSin = sinFix32(leftAngle);
            fix32 leftAngleCos = cosFix32(leftAngle);
            cur_player_y += leftAngleCos*move_speed;
            cur_player_x += leftAngleSin*move_speed;
        } else {
            cur_player_angle += 10;
        }
    }

    pause_game = joy_button_pressed(BUTTON_START);

    if(!automap_mode) {
        automap_mode = JOY_readJoypad(JOY_1) & BUTTON_Y;
    } else if (JOY_readJoypad(JOY_1) & BUTTON_B) {
        automap_mode = 0;
    }
    draw_solid = JOY_readJoypad(JOY_1) & BUTTON_Z;
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
    draw_solid = 0;
    automap_mode = 0;
    cur_frame = 0;
    SYS_disableInts();
    //VDP_setScreenHeight224();
    SYS_enableInts();
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
    init_vertex_cache();
    init_span_buf();

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

    
    playerXFrac4 = ((cur_player_x/ZOOM)>>(FIX32_FRAC_BITS-4)); // scaling factor is now 16 (2^4) instead of 1024 (2^10)
    playerYFrac4 = ((cur_player_y/ZOOM)>>(FIX32_FRAC_BITS-4));

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
    if(automap_mode) {
        draw_automap(cur_frame);
    } else {
        draw_3d_view(cur_frame);
    }
    cur_frame++;

    return SAME_MODE;
}


void cleanup_game() {
    BMP_end();
    cleanup_automap();
    MEM_free(vertex_cache_frames);
    MEM_free(cached_vertexes);
    MEM_pack();
}