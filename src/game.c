#include <genesis.h>
#include "collision.h"
#include "colors.h"
#include "clip_buf.h"
#include "game.h"
#include "game_mode.h"
#include "graphics_res.h"
#include "joy_helper.h"
#include "level.h"
#include "math3d.h"
#include "music.h"
#include "music_res.h"
#include "menu_helper.h"
#include "portal.h"
#include "portal_map.h"
#include "portal_maps.h"
#include "sector.h"

object_pos cur_player_pos;

fix16 playerXFrac4, playerYFrac4;

s16 playerXInt, playerYInt;

fix32 angleCos32, angleSin32;
fix16 angleCos16, angleSin16;
fix16 angleSinFrac12, angleCosFrac12;
s16 playerZ12Frac4;


void init_3d_palette() {
    for(int i = 0; i < 16; i++) {
        //u16 col = RGB24_TO_VDPCOLOR((random() << 8) | random());
        //u16 col = RGB24_TO_VDPCOLOR(((random() & 0xFF)<<16) |( (random()&0xFF)<<8) | (random()&0xFF));
        //threeDPalette[i] = col;
    }
}

u16 mapPalette[16] = {
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



static int pause_game = 0;
static int quit_game = 0;



u32 last_frame_ticks = 10;

u16 rot_speed;
u32 move_speed;

void calc_movement_speeds() {


    // base speed is for 30fps
    //KLog_S1("ticks from last frame: ", last_frame_ticks);

    const fix32 base_rot_speed = FIX16(.75);
    const fix32 base_move_speed = FIX16(.70); //24; 

    // 10 ticks for a 30fps frame
    move_speed = (base_move_speed * last_frame_ticks) >> FIX16_FRAC_BITS;
    rot_speed = (base_rot_speed * last_frame_ticks) >> FIX16_FRAC_BITS;

    //KLog_S2("move speed: ", move_speed, " rot: ", rot_speed);
    // multiply by how many ticks
}


int cur_frame;
draw_mode render_mode;
int debug_draw;
int subpixel;
int bob_idx;


void showFPS(u16 float_display)
{
    char str[16];
    const u16 y = 5;

    if (float_display)
    {
        fix32ToStr(SYS_getFPSAsFloat(), str, 1);
        VDP_clearTextBG(BG_B, 2, y, 5);
    }
    else
    {
        uintToStr(SYS_getFPS(), str, 1);
        VDP_clearTextBG(BG_B, 2, y, 2);
    }

    // display FPS
    VDP_drawTextBG(BG_B, str, 1, y);
}



volatile u32 vram_copy_dst;
volatile u8* vram_copy_src;

vu8 vint_flipping;
vu8 vint_flip_requested;

volatile u32 in_use_vram_copy_dst;
volatile u8* in_use_vram_copy_src;

#define FULL_BYTES (SCREEN_WIDTH*SCREEN_HEIGHT)
#define FULL_WORDS (FULL_BYTES/2)
#define HALF_WORDS (FULL_WORDS/2)
#define HALF_BYTES (FULL_BYTES/2)
#define QUARTER_WORDS (HALF_WORDS/2)
#define QUARTER_BYTES (HALF_BYTES/2)

void after_flip_vscroll_adjustment() {
    u16 vscr;
    if(vram_copy_src == bmp_buffer_1) {
        vscr = (BMP_PLANHEIGHT * 8) / 2;
    } else {
        vscr = 0;
    }
    VDP_setVerticalScroll(BG_A, vscr);
}

void copy_quarter_words(u8* src, u32 dst) {
    DMA_doDma(DMA_VRAM,
        src,
        dst,
        QUARTER_WORDS, 4);
}

void do_vint_flip() {
    
    if(vint_flipping == 1) {

        // not finished
        // complete second half here

        // second half for framebuffer 0
        // first half for framebuffer 1

        
        if(in_use_vram_copy_src == bmp_buffer_0) {
            // draw second quarter of framebuffer to third quarter of VRAM framebuffer

            copy_quarter_words(
                (u8*)in_use_vram_copy_src+QUARTER_BYTES, 
                in_use_vram_copy_dst+HALF_BYTES);
            copy_quarter_words(
                (u8*)in_use_vram_copy_src+QUARTER_BYTES + HALF_BYTES,
                in_use_vram_copy_dst+2+HALF_BYTES);
            //copy_quarter_words(
            //    (u8*)in_use_vram_copy_src+QUARTER_BYTES,
            //    in_use_vram_copy_dst+2+HALF_BYTES);
        } else {
            copy_quarter_words( 
                (u8*)in_use_vram_copy_src, 
                in_use_vram_copy_dst);
            //copy_quarter_words(
            //    (u8*)in_use_vram_copy_src,
            //    in_use_vram_copy_dst+2);
            copy_quarter_words(
                (u8*)in_use_vram_copy_src+HALF_BYTES,
                in_use_vram_copy_dst+2);
        }
        

        after_flip_vscroll_adjustment();

        vint_flipping = 0;
    } else if(vint_flip_requested) {
        vint_flipping = 1;
        vint_flip_requested = 0;
        in_use_vram_copy_dst = vram_copy_dst;
        in_use_vram_copy_src = vram_copy_src;


        // first half for framebuffer 1
        // second half for framebuffer 0
        if(in_use_vram_copy_src == bmp_buffer_0) {
            copy_quarter_words(
                (u8*)in_use_vram_copy_src, 
                in_use_vram_copy_dst);
            //copy_quarter_words(
            //    (u8*)in_use_vram_copy_src,
            //    in_use_vram_copy_dst+2);
            copy_quarter_words(
                (u8*)in_use_vram_copy_src+HALF_BYTES,
                in_use_vram_copy_dst+2);
        } else { 
            copy_quarter_words(
                (u8*)in_use_vram_copy_src+QUARTER_BYTES, 
                in_use_vram_copy_dst+HALF_BYTES);
            //copy_quarter_words(
            //    (u8*)in_use_vram_copy_src+QUARTER_BYTES, 
            //    in_use_vram_copy_dst+2+HALF_BYTES);
            copy_quarter_words(
                (u8*)in_use_vram_copy_src+QUARTER_BYTES+HALF_BYTES,
                in_use_vram_copy_dst+2+HALF_BYTES);
        }
    }


}


#define FB0MIDDLEINDEX (BMP_BASETILEINDEX + (BMP_CELLWIDTH/2 * BMP_CELLHEIGHT))
#define FB0MIDDLE (FB0MIDDLEINDEX*32)

void request_flip() {
    while(vint_flip_requested || vint_flipping) {
        // vblank is behind one request
        // wait until it has started, and then we can safely flip to the next framebuffer
        //return;
    }


    if(bmp_buffer_write == bmp_buffer_0) {
        vram_copy_src = bmp_buffer_0;
        vram_copy_dst = BMP_FB0TILE;
        bmp_buffer_write = bmp_buffer_1;
        bmp_buffer_read = bmp_buffer_0;
    } else {
        vram_copy_src = bmp_buffer_1;
        vram_copy_dst = FB0MIDDLE; //BMP_FB1TILE;
        bmp_buffer_write = bmp_buffer_0;
        bmp_buffer_read = bmp_buffer_0;
    }
    vint_flip_requested = 1;
}

void draw_3d_view(u32 cur_frame) {

    //BMP_vertical_clear();

    // clear clipping buffers
    clear_2d_buffers();
    // clear portal graph visited cache
    clear_portal_cache();
    // recursively render portal graph
    portal_rend(cur_player_pos.cur_sector, cur_frame);
    // display fps
    //BMP_waitFlipComplete();
    showFPS(1);
    // request a flip when vsync process is idle (almost always, as the software renderer is much slower than the framebuffer DMA process)
    request_flip();
    //BMP_flip(1, 0);

    return;
}


u16 last_joy = 0;


Vect2D_f32 *sector_centers = NULL;

static int last_pressed_b = 0;
u8 do_collision = 0;

void handle_input() {
    int strafe = joy_button_pressed(BUTTON_C);

    int pressed_b = joy_button_pressed(BUTTON_B);
    if(!last_pressed_b && pressed_b) {
        int next_sector = cur_player_pos.cur_sector + 1;
        if(next_sector >= cur_portal_map->num_sectors) {
            next_sector = 0;
        }
        cur_player_pos.x = sector_centers[next_sector].x;
        cur_player_pos.y = sector_centers[next_sector].y;
        cur_player_pos.cur_sector = next_sector;
    }
    last_pressed_b = pressed_b;

    //ssector* cur_ssect = find_subsector_for_position(cur_player_pos.x, cur_player_pos.y, cur_level->num_nodes-1);

    //u16 sector_idx = ssect_sector_idx(cur_ssect);
    //sector* cur_sector = &(cur_level->sectors[sector_idx]);

    //cur_player_pos.z = cur_sector->floor_height;
    
    //player_pos new_player_pos = cur_player_pos;

    int moved = 0;
    fix32 curx = cur_player_pos.x;
    fix32 cury = cur_player_pos.y;
    fix32 newx, newy;
    u16 newang = cur_player_pos.ang;

    if (joy_button_pressed(BUTTON_LEFT)) {
        if(strafe) {
            moved = 1;
            u16 leftAngle = (cur_player_pos.ang+ANGLE_90_DEGREES);
            fix32 leftDy = sinFix32(leftAngle);
            fix32 leftDx = cosFix32(leftAngle);
            newy = cury + leftDy*move_speed;
            newx = curx + leftDx*move_speed;
        } else {
            newang += rot_speed;
            if(newang > 1023) {
                newang -= 1024;
            }
        }
    } else if (joy_button_pressed(BUTTON_RIGHT)) {
        if(strafe) {
            moved = 1;
            u16 rightAngle = (cur_player_pos.ang-ANGLE_90_DEGREES);
            fix32 rightDy = sinFix32(rightAngle);
            fix32 rightDx = cosFix32(rightAngle);
            newy = cury + rightDy*move_speed;
            newx = curx + rightDx*move_speed;
        } else {
            if(newang < rot_speed) {
                newang = 1024 - (rot_speed-newang);
            } else {
                newang -= rot_speed;
            }
        }
    }    
    if(joy_button_pressed(BUTTON_DOWN)) {   
        newy = cury - angleSin32*move_speed;
        newx = curx - angleCos32*move_speed;
        moved = 1;
    } else if (joy_button_pressed(BUTTON_UP)) {
        newy = cury + angleSin32*move_speed;
        newx = curx + angleCos32*move_speed;
        moved = 1;
    }
    

    cur_player_pos.ang = newang;

    if(moved) {

        collision_result collision = check_for_collision(curx, cury, newx, newy, cur_player_pos.cur_sector, do_collision);
        cur_player_pos.x = collision.pos.x;
        cur_player_pos.y = collision.pos.y;
        cur_player_pos.cur_sector = collision.new_sector;

        cur_player_pos.z = (sector_floor_height(cur_player_pos.cur_sector, (portal_map*)cur_portal_map)<<(FIX32_FRAC_BITS-4)) + FIX32(40);



    }

    const fix32 bobs[32] = {FIX32(0.1), FIX32(0.1), FIX32(0.1), FIX32(0.1), FIX32(0.1), FIX32(0.1), FIX32(0.1), FIX32(0.1), 
                            FIX32(0.1), FIX32(0.1), FIX32(0.1), FIX32(0.1), FIX32(0.1), FIX32(0.1), FIX32(0.1), FIX32(0.1), 
                            FIX32(-0.1), FIX32(-0.1), FIX32(-0.1), FIX32(-0.1), FIX32(-0.1), FIX32(-0.1), FIX32(-0.1), FIX32(-0.1),
                            FIX32(-0.1), FIX32(-0.1), FIX32(-0.1), FIX32(-0.1), FIX32(-0.1), FIX32(-0.1), FIX32(-0.1), FIX32(-0.1)};
    cur_player_pos.z += bobs[bob_idx>>1]/2;
    bob_idx++;
    if(bob_idx >= 64) {
        bob_idx = 0;
    }


    pause_game = joy_button_pressed(BUTTON_START);

    u16 joy = JOY_readJoypad(JOY_1);



    last_joy = joy;


    /*
    if(joy & BUTTON_Y) {
        if(render_mode == AUTOMAP) {
            render_mode = GAME_WIREFRAME;
        } else {
            render_mode = AUTOMAP;
        }
    } else if (joy & BUTTON_Z) {
        render_mode = (render_mode == GAME_SOLID ? GAME_WIREFRAMEGAME_SOLID;
    } else {
        render_mode = GAME_WIREFRAME;
    }
    if(joy & BUTTON_X) {
        debug_draw = !debug_draw;
    }
    */
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

u16* cur_palette = NULL;


void init_sector_jump_positions() {
    sector_centers = MEM_alloc(sizeof(Vect2D_f32) * cur_portal_map->num_sectors);
    for(int i = 0; i < cur_portal_map->num_sectors; i++) {

        int avg_sect_x = 0;
        int avg_sect_y = 0;
        int num_walls = sector_num_walls(i, (portal_map*)cur_portal_map);
        int wall_offset = sector_wall_offset(i, (portal_map*)cur_portal_map);

        for(int j = 0; j < num_walls; j++) {
            int vidx = cur_portal_map->walls[wall_offset+j];
            avg_sect_x += cur_portal_map->vertexes[vidx].x;
            avg_sect_y += cur_portal_map->vertexes[vidx].y;
        }
        avg_sect_x /= num_walls;
        avg_sect_y /= num_walls;
        sector_centers[i].x = intToFix32(avg_sect_x);
        sector_centers[i].y = intToFix32(avg_sect_y);
    }
}

void do_vint_flip();

void init_game() {
    vint_flip_requested = 0;
    vint_flipping = 0;

    //XGM_stopPlay();
    SYS_setVIntCallback(do_vint_flip);
    VDP_setVerticalScroll(BG_B, 0);
    
    VDP_setScreenWidth256();
    //VDP_setScreenWidth320();
    //VDP_setScreenHeight240();


    //request_flip();

    render_mode = GAME_WIREFRAME;
    bob_idx = 0;
    subpixel = 1;

    cur_frame = 1;
    debug_draw = 0;
    //PAL3
    //VDP_setBackgroundColor(LIGHT_BLUE_IDX);
    //VDP_setVerticalScroll(BG_B, 0);
    //VDP_drawImageEx(BG_B, &bg_img,   TILE_ATTR_FULL(PAL3, 0, 0, 0, 0x0360), 8, 0, 1, 1);
	//VDP_drawImageEx(BG_B, &doom_logo, 0x0360, 8, 0, 1, 1);


    //set_portal_map((portal_map*)&portal_level_1);
    set_portal_map((portal_map*)&overlapping_map);




    quit_game = 0;
    if(pause_game) {
        pause_game = 0;
    } else {
        init_sector_jump_positions();
        
        //cur_player_pos.x = 133430;
        cur_player_pos.x = sector_centers[0].x;
        //cur_player_pos.y = -200904; 
        cur_player_pos.y = sector_centers[0].y; 
        cur_player_pos.cur_sector = 0;
        //cur_player_pos.cur_sector = 0;

        cur_player_pos.z = (sector_floor_height(cur_player_pos.cur_sector, (portal_map*)cur_portal_map)<<(FIX32_FRAC_BITS-4)) + FIX32(40);

        cur_player_pos.ang = 0;
        //cur_player_pos.ang = 904;
    }

    init_3d_palette();
    init_clip_buffer_list();

    //cur_palette = (u16*)threeDPalette;
    cur_palette = pal.data;
    VDP_setPalette(PAL1, cur_palette);
    

    clear_menu();


    BMP_init(1, BG_A, PAL1, 0, 1);
    
    init_2d_buffers();

    init_portal_renderer();

    if(music_on) {
        //XGM_startPlay(xgm_e1m4);
    }
}

void maybe_set_palette(u16* new_palette) {
    if(cur_palette != new_palette) {
        VDP_setPalette(PAL1, new_palette);
        cur_palette = new_palette;
    }
}



/*
const char tex[16] = {
    LIGHT_BLUE_IDX,
    MED_BLUE_IDX,
    DARK_BLUE_IDX,
    LIGHT_GREEN_IDX,
    MED_GREEN_IDX,
    DARK_GREEN_IDX,
    LIGHT_RED_IDX,
    MED_RED_IDX,
    DARK_RED_IDX,
    MED_BROWN_IDX,
    DARK_BROWN_IDX,
    BLACK_IDX,
    DARK_BROWN_IDX,
    MED_BROWN_IDX,
    DARK_RED_IDX,
    MED_RED_IDX
};
*/

game_mode run_game() {
    u32 start_ticks = getTick();
    
    run_sector_processes();
    calc_movement_speeds();
    handle_input();

    angleCos32 = cosFix32(cur_player_pos.ang);
    angleSin32 = sinFix32(cur_player_pos.ang); 
    angleCos16 = cosFix16(cur_player_pos.ang);
    angleSin16 = sinFix16(cur_player_pos.ang); 
    playerZ12Frac4 = cur_player_pos.z >> (FIX32_FRAC_BITS-4);
    playerXInt = cur_player_pos.x>>FIX32_FRAC_BITS;
    playerYInt = cur_player_pos.y>>FIX32_FRAC_BITS;

    //JOY_waitPress
    /*
    if(joy_button_newly_pressed(BUTTON_START)) { ////pause_game) {
        while(1) {}
        return PAUSE_MENU;
    } else if (quit_game) {
        return MAIN_MENU;
    }
    */

   /*
   static u8 cur_x = 0;
   static u8 cur_y0 = 0;
   static u8 cur_y1 = 128;
   static u8 cur_clip_y0 = 0;
   static u8 cur_clip_y1 = 0;
   
    u16 joy = JOY_readJoypad(JOY_1);
    if(joy & BUTTON_A) {
        if(joy & BUTTON_UP) {
            if(cur_y0 > 0) {
                cur_y0--;
            }
        } else if (joy & BUTTON_A && joy & BUTTON_DOWN) {
            if(cur_y0 < cur_y1) {
                cur_y0++;
            }
        }
    } else if (joy & BUTTON_B) {
      if(joy & BUTTON_UP) {
          if(cur_y1 > cur_y0) {
              cur_y1--;
          }
      }  else if (joy & BUTTON_DOWN) {
          if(cur_y1 < SCREEN_HEIGHT-1) {
              cur_y1++;
          }
      }
    } else if(joy & BUTTON_UP) {
        if(cur_y0 > 0) {
            cur_y0--;
            cur_y1--;
        }        
    } else if (joy & BUTTON_DOWN) {
        if(cur_y1 < SCREEN_HEIGHT-1) {
            cur_y1++;
            cur_y0++;
        }
    } else if (joy & BUTTON_LEFT) {
        if(cur_x > 0) {
            cur_x--;
        }
    } else if (joy & BUTTON_RIGHT) {
        if(cur_x < SCREEN_WIDTH-1) {
            cur_x++;
        }
    }

   col_params params = {
       .x = cur_x,
       .y0 = cur_y0,
       .y1 = cur_y1,
       .clip_y0 = cur_clip_y0,
       .clip_y1 = cur_clip_y1,
       .bmp = &door
   };
    */

    switch(render_mode) {
        case GAME_WIREFRAME:
        case GAME_SOLID:
            maybe_set_palette(pal.data);
            // probably not necessary
            //VDP_waitVInt();
            
            /* 3d render code */

            draw_3d_view(cur_frame);

            
            /* texture map code */
            
            //BMP_vertical_clear();
            //showFPS(1);
            //draw_tex_column(params);
            //sprintf(buf, "w: %i, h: %i", door.w, door.h);
            //VDP_drawTextBG(BG_B, buf, 7, 5);
            //request_flip();
            //maybe_set_palette(door.palette->data);
            


    }
    u32 end_ticks = getTick();
    last_frame_ticks = end_ticks - start_ticks;

    cur_frame++;
    return SAME_MODE;
}


void cleanup_game() { 
    BMP_end();
    MEM_free(sector_centers);
    cleanup_portal_renderer();
    free_clip_buffer_list();
    release_2d_buffers();
    MEM_pack();
}