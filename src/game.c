#include <genesis.h>
#include "cart_ram.h"
#include "collision.h"
#include "colors.h"
#include "config.h"
#include "clip_buf.h"
#include "debug.h"
#include "game.h"
#include "game_mode.h"
#include "graphics_res.h"
#include "joy_helper.h"
#include "level.h"
#include "maps.h"
#include "map_table.h"
#include "math3d.h"
#include "music.h"
#include "music_res.h"
#include "menu_helper.h"
#include "object.h"
#include "portal.h"
#include "portal_map.h"
#include "sector.h"

object_pos cur_player_pos;

fix16 playerXFrac4, playerYFrac4;

s16 playerXInt, playerYInt;

fix32 angleCos32, angleSin32;
fix16 angleCos16, angleSin16;
fix16 angleSinFrac12, angleCosFrac12;
s16 playerZ12Frac4;


static int pause_game = 0;
static int quit_game = 0;



u32 last_frame_ticks = 10;

u16 rot_speed;
u32 move_speed;

void calc_movement_speeds() {


    // base speed is for 30fps
    //KLog_S1("ticks from last frame: ", last_frame_ticks);

    const fix32 base_rot_speed = FIX16(1);
    const fix32 base_move_speed = FIX16(.70); //24; 

    // 10 ticks for a 30fps frame
    move_speed = (base_move_speed * last_frame_ticks) >> FIX16_FRAC_BITS;
    rot_speed = (base_rot_speed * last_frame_ticks) >> FIX16_FRAC_BITS;

    //KLog_S2("move speed: ", move_speed, " rot: ", rot_speed);
    // multiply by how many ticks
}

int cur_frame;


u32 vints = 0;
void showFPS(u16 float_display)
{
    char str[32];
    const u16 y = 5;

    if (float_display)
    {
        fix32ToStr(SYS_getFPSAsFloat(), str, 1);
        VDP_clearTextBG(BG_B, 1, y, 4);
    }
    else
    {
        uintToStr(SYS_getFPS(), str, 1);
        VDP_clearTextBG(BG_B, 1, y, 2);
    }

    // display FPS
    VDP_drawTextBG(BG_B, str, 1, y);

    sprintf(str, "%i ", vints);
    vints = 0;
    VDP_drawTextBG(BG_B, str, 1, 6); 

    //sprintf(str, "s: %i ", cur_player_pos.cur_sector);
    //VDP_drawTextBG(BG_B, str, 1, 7);
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
    vints++;
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
        vram_copy_dst = FB0MIDDLE; 
        bmp_buffer_write = bmp_buffer_0;
        bmp_buffer_read = bmp_buffer_0;
    }
    vint_flip_requested = 1;
}

void draw_3d_view(u32 cur_frame) {

    if(JOY_readJoypad(JOY_1) & BUTTON_A) {
        BMP_vertical_clear();
    }
    // clear clipping buffers
    clear_2d_buffers();

    // clear portal graph visited cache
    clear_portal_cache();

    // recursively render portal graph
    portal_rend(cur_player_pos.cur_sector, cur_frame);


    // display fps
    showFPS(1);

    // request a flip when vsync process is idle (almost always, as the software renderer is much slower than the framebuffer DMA process)
    request_flip();
    debug_draw_cleared = 1;

    return;
}


u16 last_joy = 0;


Vect2D_f32 *sector_centers = NULL;

static int last_pressed_b = 0;
//u8 do_collision = 0;

int bob_idx;
u8 gun_bob_idx;
const fix32 bobs[32] = {FIX32(0.1), FIX32(0.1), FIX32(0.1), FIX32(0.1), FIX32(0.1), FIX32(0.1), FIX32(0.1), FIX32(0.1), 
                        FIX32(0.1), FIX32(0.1), FIX32(0.1), FIX32(0.1), FIX32(0.1), FIX32(0.1), FIX32(0.1), FIX32(0.1), 
                        FIX32(-0.1), FIX32(-0.1), FIX32(-0.1), FIX32(-0.1), FIX32(-0.1), FIX32(-0.1), FIX32(-0.1), FIX32(-0.1),
                        FIX32(-0.1), FIX32(-0.1), FIX32(-0.1), FIX32(-0.1), FIX32(-0.1), FIX32(-0.1), FIX32(-0.1), FIX32(-0.1)};


const Vect2D_s16 gun_bobs[16] = {
    {-1,-1},{-2,-2},{-3,-3},{-4,-4},{-3,4},{-2,4},{-1,4},{0,4},{1,4},{2,4},{3,4},{4,4},{3,3},{2,2},{1,1},{0,0}
};



Sprite* shotgun_spr;
#define BASE_GUN_X 128
#define BASE_GUN_Y 110

void set_gun_pos() {
    u8 idx = (gun_bob_idx>>4) % 0xF;

    SPR_setPosition(shotgun_spr, BASE_GUN_X+(gun_bobs[idx].x<<1), BASE_GUN_Y+(gun_bobs[idx].y<<1));
}

void step_gun_bob() {
    set_gun_pos();
    gun_bob_idx++;
}
void reset_gun_bob() {
    gun_bob_idx = 0;
    set_gun_pos();
}


int holding_down_move = 0;

void handle_input() {
    int strafe = joy_button_pressed(BUTTON_C);

    int pressed_b = joy_button_pressed(BUTTON_B);


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

        //collision_result collision = check_for_collision(curx, cury, newx, newy, cur_player_pos.cur_sector);
        collision_result collision = check_for_collision_radius(curx, cury, newx, newy, PLAYER_COLLISION_DISTANCE, cur_player_pos.cur_sector);
        cur_player_pos.x = collision.pos.x;
        cur_player_pos.y = collision.pos.y;
        cur_player_pos.cur_sector = collision.new_sector;
        s16 cur_sector_height = get_sector_floor_height(cur_player_pos.cur_sector);

        
        cur_player_pos.z = (cur_sector_height<<(FIX32_FRAC_BITS-4)) + FIX32(50);
        


    }

    //cur_player_pos.z += bobs[bob_idx>>1]/2;
    //bob_idx++;
    //if(bob_idx >= 64) {
    //    bob_idx = 0;
    //}

    u8 move_button = joy_button_pressed(BUTTON_UP) ? BUTTON_UP : (joy_button_pressed(BUTTON_DOWN) ? BUTTON_DOWN : 0);

    step_gun_bob();
    /*
    if(!holding_down_move && move_button) {
        holding_down_move = move_button;
    } else if (holding_down_move && (holding_down_move == move_button)) {
        step_gun_bob();
    } else {
        holding_down_move = 0;
        reset_gun_bob();
    }
    */


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

#include "tex_tables_lookup.h"
void copy_texture_tables() {
    #ifdef RAM_TEXTURE
    u32 ram_position = RAM_START;
    //u32 slot = 0;
    //return;
    for(int i = 1; i < 500; i++) {
        u16* table_start = (u16*)(jump_table_lut[i]);
        u32 words_to_copy = i*2 + 1; // each instruction in the table is 2 words, plus 1 extra for the rts
        u32 bytes_to_copy = words_to_copy*2;

        
        unlock_ram();
        memcpy((void*)ram_position, (void*)(jump_table_lut[i]), bytes_to_copy);
        lock_ram();
        
        jump_table_lut[i] = ram_position;
        ram_position += bytes_to_copy;
    }        
    #endif
}

u16 free_tile_loc = 0x390;


selected_level init_load_level = SLIME_ROOM;

void init_game() {
    copy_texture_tables();
    
    VDP_setTextPalette(PAL3);

    //DMA_doVRamFill(0x0390, 0x0170, 0x00, 1);
    VDP_fillTileData(0x00, 0x0390, 0x170, 1);
    
    //ram_set(100, 0);
    vint_flip_requested = 0;
    vint_flipping = 0;
    

    //SPR_init();
    //shotgun_spr = SPR_addSprite(&shotgun, BASE_GUN_X, BASE_GUN_Y, TILE_ATTR_FULL(PAL0, 0, 0, 0, free_tile_loc));
    //SPR_setVRAMTileIndex(shotgun_spr, free_tile_loc);
    free_tile_loc += shotgun.maxNumTile;
    reset_gun_bob();
    
    PAL_setPalette(PAL0, shotgun.palette->data);
    SPR_update();

    //XGM_stopPlay();
    SYS_setVIntCallback(do_vint_flip);
    VDP_setVerticalScroll(BG_B, 0);
    
    #ifdef H32_MODE
    VDP_setScreenWidth256();
    #else 
    VDP_setScreenWidth320();
    #endif
    //VDP_setScreenHeight240();



    holding_down_move = 0;

    cur_frame = 1;
    
	VDP_setBackgroundColor(1);


    //load_portal_map(&overlapping_map);
    //load_portal_map(&editor_test_map);

    switch(init_load_level) {
        case SLIME_ROOM:
            load_portal_map((portal_map*)map_table[4]);
            break;
        case OVERLAPPING_ROOMS:
            load_portal_map((portal_map*)map_table[3]);
            break;
    }
    //load_portal_map((portal_map*)map_table[3]);
    /*
    KLog_U1("map table address: ", map_table);
    KLog_U1("portal map pointer: ", map_table[3]);
    portal_map* map = (portal_map*)map_table[3];
    KLog_U1("num_sectors address: ", &map->num_sectors);
    KLog_U1("num_walls address: ", &map->num_walls);
    KLog_U1("num_verts address: ", &map->num_verts);
    KLog_U1("sectors address: ", &map->sectors);
    KLog_U1("sector_types address: ", &map->sector_types);
    KLog_U1("sector_params address: ", &map->sector_params);
    KLog_U1("walls address: ", &map->walls);
    KLog_U1("portals address: ", &map->portals);
    KLog_U1("wall_colors address: ", &map->wall_colors);
    KLog_U1("vertexes address: ", &map->vertexes);
    KLog_U1("wall_norm_quadrants address: ", &map->wall_norm_quadrants);
    //while(1) {}
    */



    quit_game = 0;
    if(pause_game) {
        pause_game = 0;
    } else {
        init_sector_jump_positions();
        
        cur_player_pos.x = sector_centers[0].x;
        cur_player_pos.y = sector_centers[0].y; 
        cur_player_pos.cur_sector = 0;

        cur_player_pos.z = (get_sector_floor_height(cur_player_pos.cur_sector)<<(FIX32_FRAC_BITS-4)) + FIX32(50);

        cur_player_pos.ang = 0;
    }

    init_clip_buffer_list();

    cur_palette = pal.data;
    VDP_setPalette(PAL1, cur_palette);
    

    clear_menu();


    BMP_init(1, BG_A, PAL1, 0, 1);
    


    u16 skybox_gradient_basetile = TILE_ATTR_FULL(PAL3, 0, 0, 0, free_tile_loc);
	VDP_drawImageEx(BG_B, &skybox_gradient, skybox_gradient_basetile, 4, 4, 0, 1);
    
    
    PAL_setPalette(PAL3, skybox_gradient.palette->data);

    free_tile_loc += skybox_gradient.tileset->numTile;

    //u16 hud_base_tile = TILE_ATTR_FULL(PAL2, 1, 0, 0, free_tile_loc);
    //VDP_drawImageEx(BG_B, &hud, hud_base_tile, 0, 20, 0, 1);
    //PAL_setPalette(PAL2, hud.palette->data);
    
    init_2d_buffers();

    init_portal_renderer();

    init_object_lists(cur_portal_map->num_sectors);

    //object* red_cube = alloc_object_in_sector(cur_player_pos, 0, sector_centers[0].x, sector_centers[0].y, get_sector_floor_height(0), 0);
    


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

game_mode run_game() {


    u32 start_ticks = getTick();
    //process_all_objects(cur_frame);

    
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


    maybe_set_palette(pal.data);
    draw_3d_view(cur_frame);

    //SPR_update();
    u32 end_ticks = getTick();
    last_frame_ticks = end_ticks - start_ticks;

    cur_frame++;
    return SAME_MODE;
}


void cleanup_game() { 
    BMP_end();
    //SPR_releaseSprite(shotgun_spr);
    //SPR_end();
    MEM_free(sector_centers);
    cleanup_portal_renderer();
    free_clip_buffer_list();
    release_2d_buffers();
    clear_object_lists();
    clean_sector_parameters();
    
    MEM_pack();
}