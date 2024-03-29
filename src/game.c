#include <genesis.h>
#include "my_bmp.h"
#include "collision.h"
#include "console.h"
#include "clip_buf.h"
#include "colors.h"
#include "config.h"
#include "game.h"
#include "game_mode.h"
#include "init.h"
#include "inventory.h"
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
#include "real_timer.h"
#include "sector_group.h"
#include "sfx.h"
#include "sfx_res.h"
#include "sys.h"
#include "utils.h"
#include "vwf.h"

#include "weapon_sprites.h"
#include "graphics_res.h"
#include "sprites_res.h"


player_pos cur_player_pos;

fix16 playerXFrac4, playerYFrac4;

s16 playerXInt, playerYInt;

fix32 angleCos32, angleSin32;
fix16 angleCos16, angleSin16;
fix16 angleSinFrac12, angleCosFrac12;
s16 playerZCam12Frac4;


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

}

int cur_frame;

u16 smiley_addr;
u16 frown_addr;
u16 pause_checker_addr;


u16 last_vints_arr[4] = { 0, 0, 0, 0 };
u8 last_vints_idx = 0;

u16 vints_avg = 0;

u16 last_vints = 0;
void showStats() {
    char str[32];
    u16 y = 5;

    
    /*
    u8 fps_good = 0;
    u16 cur_vints = vints;
    u16 delta_vtimer = cur_vints - last_vints;

    
    
    vints_avg += delta_vtimer;
    vints_avg -= last_vints_arr[last_vints_idx];
    last_vints_arr[last_vints_idx++] = delta_vtimer;
    if(last_vints_idx >= 4) { last_vints_idx = 0; }
    u16 frames = vints_avg >> 2;
    last_vints = vints;
    fps_good = frames < 5; 
    */

    s32 frames = SYS_getFPSAsFloat();
    //last_vtimer = cur_vtimer;
    //KLog_U1("FRAMES: ", frames);

    //uintToStr(frames, str, 2);
    fix32ToStr(frames, str, 1);
    //return;

    // display FPS


    //u16 base_tile_addr = 0x05A0;//fps_good ? smiley_addr : frown_addr;
    //for(int ty = y; ty < y+4; ty++) {
    //    for(int tx = 1; tx < 5; tx++) {
    //        VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, 1, 0, 0, base_tile_addr), tx, ty);
    //        base_tile_addr++;
    //    }
    //}
    
    char* sptr = str;
    u16 base = 0x05A0;
    u8 x = 2;
    while(*sptr) {
        VDP_setTileMapXY(BG_B, 
            TILE_ATTR_FULL(PAL3, 1, 0, 0,
                        base+(*sptr++)-32),
            x++,
            y);
    }
    //VDP_setTextPalette(1);
    //VDP_clearTextBG(BG_B, 1, y, 4);
    //VDP_drawTextBG(BG_B, "blah", 1, y++);
    /*
    if(fps_good) {
        
        VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, 1, 0, 0, smiley_addr), 1, y);
        VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, 1, 0, 0, smiley_addr+1), 2, y);
        VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, 1, 0, 0, smiley_addr+2), 1, y+1);
        VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, 1, 0, 0, smiley_addr+3), 2, y+1);
    } else {
        VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, 1, 0, 0, frown_addr), 1, y);
        VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, 1, 0, 0, frown_addr+1), 2, y);
        VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, 1, 0, 0, frown_addr+2), 1, y+1);
        VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, 1, 0, 0, frown_addr+3), 2, y+1);
    }
    */
}




void draw_3d_view(u32 cur_frame) {
    
    // for fading, it might be cheaper to just always clear the screen, and not draw faded/transparent pixels
    //if(1) { 
    if (render_mode == RENDER_WIREFRAME) { //} || JOY_readJoypad(JOY_1) & BUTTON_A) {
        bmp_vertical_clear();
    }
    
    reset_sprite_hit_info();

    // clear clipping buffers
    clear_2d_buffers();

    // clear portal graph visited cache
    clear_portal_cache();

    // recursively render portal graph
    //KLog("START RENDER");
    portal_rend(cur_player_pos.cur_sector, cur_frame);
    //KLog("END RENDER");

    // display fps
    showStats(0);

    // request a flip when vsync process is idle (almost always, as the software renderer is much slower than the framebuffer DMA process)
    request_flip();
    debug_draw_cleared = 1;

    return;
}


u16 last_joy = 0;


Vect2D_f32 *sector_centers = NULL;

//static int last_pressed_b = 0;
//u8 do_collision = 0;

int run_bob_idx;
int wait_idle_bob_idx;
int idle_bob_idx;
s16 gun_bob_idx;
const fix32 bobs[28] = { // 32
    -1,-2,-3,-4,-5,-6,-7,//-8,
    //-7,
    -6,-5,-4,-3,-2,-1,-0,
    1,2,3,4,5,6,7,//8,
    //7,
    6,5,4,3,2,1,0,
};


const Vect2D_s16 gun_bobs[20] = {
    {0,0},
    {-1,0},
    {-2,0},
    {-3,-1},
    {-4,-2},
    {-5,-3},
    {-4,-2},
    {-3,-1},
    {-2,0},
    {-1,0},
    {0,0},
    {1,0},
    {2,0},
    {3,-1},
    {4,-2},
    {5,-3},
    {4,-2},
    {3,-1},
    {2,0},
    {1,0}
};

void apply_bob() {
    fix32 run_bob_amt = bobs[run_bob_idx] * FIX32(1.5);
    fix32 idle_bob_amt = bobs[idle_bob_idx] * FIX32(.2);
    cur_player_pos.z += run_bob_amt + idle_bob_amt;
}

void unapply_bob() {
    fix32 run_bob_amt = bobs[run_bob_idx] * FIX32(1.5);
    fix32 idle_bob_amt = bobs[idle_bob_idx] * FIX32(.2);
    cur_player_pos.z -= run_bob_amt + idle_bob_amt;
}



//Sprite* shotgun_spr;

int shotgun_spr_idx_start, shotgun_num_sprites;


void set_gun_pos() {
    
    u8 idx = (gun_bob_idx>>4) % 20;

    s16 bobOffX = gun_bobs[idx].x<<1;
    s16 bobOffY = gun_bobs[idx].y<<1;
    set_weapon_sprite_position_offset(bobOffX, bobOffY);

    /*
    AnimationFrame *frame1 = shotgun.animations[0]->frames[0];
    FrameVDPSprite** f = frame1->frameInfos[0].frameVDPSprites;
    for(int i = 0; i < shotgun_num_sprites; i++) {
        FrameVDPSprite* spr = f[i];
        VDP_setSpritePosition(i+shotgun_spr_idx_start,
            BASE_GUN_X+spr->offsetX+bobOffX,
            BASE_GUN_Y+spr->offsetY+bobOffY
        );
    }
    VDP_updateSprites(shotgun_num_sprites, DMA_QUEUE);
    */
   
}

void step_gun_bob() {
    set_gun_pos();
    s16 inc = last_frame_ticks * 1; //(.20 << 2);
    gun_bob_idx += inc; //(1 << 2);
}
void reset_gun_bob() {
    gun_bob_idx = 0;
    set_gun_pos();
}


void init_shotgun() {
    set_weapon_anim(SHOTGUN_IDLE_ANIM, DMA);
}


int holding_down_move = 0;

u8 render_mode;

static u8 prev_start_pressed = 0;

static u8 paused = 0;

#define PAUSE_SCREEN 5
#define UNPAUSE_SCREEN 6

int handle_input() {

    //int pressed_b = joy_button_pressed(BUTTON_B);
    int start_pressed = joy_button_pressed(BUTTON_START);
    if (start_pressed && !prev_start_pressed) {
        if(paused) {
            paused = 0;
            prev_start_pressed = start_pressed;
            return UNPAUSE_SCREEN;
        } else {
            paused = 1;
            prev_start_pressed = start_pressed;
            return PAUSE_SCREEN;
        }
    }

    prev_start_pressed = start_pressed;
    if(paused) {
        return 0;
    }


    int strafe_left = joy_button_pressed(BUTTON_X);
    int strafe_right = joy_button_pressed(BUTTON_Z);
    int jump_pressed = joy_button_pressed(BUTTON_Y);

    int moved = 0;
    fix32 curx = cur_player_pos.x;
    fix32 cury = cur_player_pos.y;
    fix32 newx, newy;
    u16 newang = cur_player_pos.ang;


    if (joy_button_pressed(BUTTON_LEFT)) {

        newang += rot_speed;
        if(newang > 1023) {
            newang -= 1024;
        }
    } else if (joy_button_pressed(BUTTON_RIGHT)) {
        if(newang < rot_speed) {
            newang = 1024 - (rot_speed-newang);
        } else {
            newang -= rot_speed;
        }

    }   
    if(strafe_left) {
        moved = 1;
        u16 leftAngle = (cur_player_pos.ang+ANGLE_90_DEGREES);
        fix32 leftDy = sinFix32(leftAngle);
        fix32 leftDx = cosFix32(leftAngle);
        newy = cury + leftDy*move_speed;
        newx = curx + leftDx*move_speed;
    } 
    
    if(strafe_right) {
        moved = 1;
        u16 rightAngle = (cur_player_pos.ang-ANGLE_90_DEGREES);
        fix32 rightDy = sinFix32(rightAngle);
        fix32 rightDx = cosFix32(rightAngle);
        newy = cury + rightDy*move_speed;
        newx = curx + rightDx*move_speed;
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

    unapply_bob();

    u16 sect_group = sector_group(cur_player_pos.cur_sector, cur_portal_map);
    if(moved) {

        
        s16 cur_player_z = cur_player_pos.z >> (FIX32_FRAC_BITS-4);
        collision_result collision = check_for_collision_radius(curx, cury, cur_player_z, newx, newy, FIX32(PLAYER_COLLISION_DISTANCE_SQR), cur_player_pos.cur_sector);
        cur_player_pos.x = collision.pos.x;
        cur_player_pos.y = collision.pos.y;
        if(collision.new_sector != cur_player_pos.cur_sector) {
            u16 old_sect_group = sector_group(cur_player_pos.cur_sector, cur_portal_map);
            cur_player_pos.cur_sector = collision.new_sector;
            sect_group = sector_group(collision.new_sector, cur_portal_map);
            if(sect_group != old_sect_group) {
                if(activate_sector_group_enter_trigger(sect_group) == LEVEL_END) {
                    return LEVEL_END;
                }
            }
        }
        
        idle_bob_idx = 27;
        wait_idle_bob_idx = 16;

        run_bob_idx++;
        if(run_bob_idx >= 28) {
            run_bob_idx = 0;
        }
    } else {
        run_bob_idx = 27;
        
        if(wait_idle_bob_idx == 0) {
            idle_bob_idx++;
            if(idle_bob_idx >= 28) {
                idle_bob_idx = 0;
            }
        } else {
            wait_idle_bob_idx--;
        }
    }

    static int jumping = 0;
    static int pressing = 0;
    static int shooting = 0;

    
    s16 cur_sector_height = get_sector_group_floor_height(sect_group);
    s32 rest_player_pos = (cur_sector_height<<(FIX32_FRAC_BITS-4));// + FIX32(50);
    if(cur_player_pos.z > rest_player_pos) {
        cur_player_pos.z -= FIX32(8);
    } else if (cur_player_pos.z < rest_player_pos) {
        cur_player_pos.z = rest_player_pos;
    }

    //cur_player_pos.z = (cur_sector_height<<(FIX32_FRAC_BITS-4)) + FIX32(50);   

    apply_bob();
    step_weapon_animation();

    u8 move_button = joy_button_pressed(BUTTON_UP) ? BUTTON_UP : (joy_button_pressed(BUTTON_DOWN) ? BUTTON_DOWN : 0);
    
    if(!holding_down_move && move_button) {
        holding_down_move = move_button;
    } else if (holding_down_move && (holding_down_move == move_button)) {
        step_gun_bob();
    } else {
        holding_down_move = 0;
        reset_gun_bob();
    }
    
    
    if(shooting) { 
        shooting--;
        if(shooting == 4) { set_weapon_anim(SHOTGUN_RELOAD_ANIM, DMA_QUEUE); }
        if(shooting == 0 && joy_button_pressed(BUTTON_A)) { shooting = 1;}
    }
    if(pressing) { 
        pressing--;
        if(pressing == 0 && joy_button_pressed(BUTTON_B)) { pressing = 1;}
    }

    if(jumping) {
        if(jumping >= 6) { cur_player_pos.z += FIX32(16); }
        jumping--;
        if(jumping == 0) {
            play_sfx(SFX_JUMP2_ID, 15);
        }
    }


    if(joy_button_pressed(BUTTON_A) && !shooting) {  
        //play_sfx(SFX_PEASHOOTER_ID, 1);
        reset_gun_bob();
        play_sfx(SFX_SHOTGUN_ID, 15);
        run_in_phs(cur_player_pos.cur_sector, wake_enemies_in_sector, cur_portal_map);

        shooting = 12;
        //KLog_U2("drawn to center: ", drawn_to_center_cols, " sprite idX: ", sprite_on_center_col);
        if(drawn_to_center_cols == OBJECT && sprite_on_center_col != NULL_OBJ_LINK) {
            if(OBJ_LINK_DEREF(sprite_on_center_col).current_state != MAYBE_GET_PICKED_UP_STATE) {
                //KLog("object rendered to center of screen");
                // destroy the object
                free_object(sprite_on_center_col, center_object_sector);
            }
        }
    }

    if(joy_button_pressed(BUTTON_B) && !pressing) {
        //play_sfx(SFX_SELECT_ID, 1);
        pressing = 2;
        if(!check_trigger_switch(&cur_player_pos)) {        
            play_sfx(SFX_JUMP1_ID, 15);
        }

    }

    if(jump_pressed && !jumping) {
        play_sfx(SFX_JUMP1_ID, 15);
        jumping = 10;
    }



    return 0;


    u16 joy = JOY_readJoypad(JOY_1);



    last_joy = joy;

    if (joy & BUTTON_Z) {
        render_mode = (render_mode == RENDER_SOLID ? RENDER_WIREFRAME : RENDER_SOLID);
    }
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





void do_vint_flip();


void set_palette(u16* new_palette) {
    PAL_setPalette(PAL1, new_palette);
    cur_palette = new_palette;
}

void maybe_set_palette(u16* new_palette) {
    if(cur_palette == new_palette) {
        return;
    }
    PAL_setPalette(PAL1, new_palette);
    cur_palette = new_palette;
}


u16 free_tile_loc;


void init_game() {
    free_tile_loc = 0x390;
    cur_palette = NULL;
    render_mode = RENDER_SOLID;

    
    //VDP_clearPlane(BG_B, 1); clears the fps display somehow
    //VDP_clearPlane(BG_A, 1);

    //DMA_doVRamFill(0x0390, 0x0170, 0x00, 1);
    VDP_fillTileData(0x00, 0x0390, 0x170, 1);
        
    XGM_stopPlay();
    //SYS_setVIntCallback(do_vint_flip);
    VDP_setVerticalScroll(BG_B, -1); // scroll down by 1 pixel to show the top of console messages :)
    
    #ifdef H32_MODE
    VDP_setScreenWidth256();
    #else 
    VDP_setScreenWidth320();
    #endif
    VDP_setScreenHeight240();



    holding_down_move = 0;

    cur_frame = 1;
    
	VDP_setBackgroundColor(10);
    volatile u32* vp_start_in_game_arr = start_in_game_arr;
    KLog_U1("free bytes of RAM before init: ", MEM_getFree());



    quit_game = 0;


    init_clip_buffer_list();


    // palette 0 is weapon palette
    PAL_setPalette(PAL0, shotgun.palette->data);

    
    static int loaded_into_game = 0;
    if(vp_start_in_game_arr[1] && !loaded_into_game) {
        volatile u32* vp_init_load_level_arr = instant_load_level_array;
        volatile int init_level = vp_init_load_level_arr[1];
        init_load_level = init_level;
        loaded_into_game = 1;
    }

    load_portal_map((portal_map*)map_table[2+init_load_level]);

    // palette 1 is 3d palette
    //PAL_setPalette(PAL1, cur_palette);
    //static int init = 0;
    //if(!init) {
        if(cur_portal_map->palette != NULL) {
            set_palette(cur_portal_map->palette);
        } else {
            set_palette(sprite_pal.data);
        }
        //init = 1;
    //}

    // palette 2 is HUD palette, set in inventory_init
    // palette 3 is sprite palette
    PAL_setPalette(PAL3, sprite_pal.data);
    VDP_setTextPalette(PAL3);
    init_color_table();

    clear_menu();

    // skybox is PAL3
    bmp_init_vertical(1, BG_A, PAL1, 0);

    //u16 skybox_gradient_basetile = TILE_ATTR_FULL(PAL3, 0, 0, 0, free_tile_loc);
	//VDP_drawImageEx(BG_B, &skybox_gradient, skybox_gradient_basetile, 4, 4, 0, 0);
    //PAL_setPalette(PAL3, skybox_gradient.palette->data);
    //free_tile_loc += skybox_gradient.tileset->numTile;
    //VDP_waitVSync();
    
    // palette 2 is HUD palette
    
    free_tile_loc = inventory_init(free_tile_loc);
    //inventory_draw();
    //inventory_reset();
    
    VDP_waitVSync();

    XGM_setPCM(SFX_SHOTGUN_ID, sfx_shotgun, sizeof(sfx_shotgun));
    XGM_setPCM(SFX_SELECT_ID, sfx_select, sizeof(sfx_select));
    XGM_setPCM(SFX_JUMP1_ID, sfx_jump1, sizeof(sfx_jump1));
    XGM_setPCM(SFX_JUMP2_ID, sfx_jump2, sizeof(sfx_jump2));
    XGM_setPCM(SFX_LIFT_GO_UP_ID, sfx_lift_go_up, sizeof(sfx_lift_go_up));
    XGM_setPCM(SFX_OPEN_DOOR_ID, sfx_door_open, sizeof(sfx_door_open));
    XGM_setPCM(SFX_CLOSE_DOOR_ID, sfx_door_close, sizeof(sfx_door_close));
    XGM_setPCM(SFX_ENEMY_A_WAKE_ID, sfx_enemy_a_wake, sizeof(sfx_enemy_a_wake));

    init_sprite_draw_cache();
    init_2d_buffers();

    init_portal_renderer();

    free_tile_loc = init_weapon_sprites(free_tile_loc);
    init_shotgun(free_tile_loc);

    //VDP_loadTileData(smile.tiles, free_tile_loc, 16, DMA);
    //smiley_addr = free_tile_loc; free_tile_loc += 16;
    //VDP_loadTileData(frown.tiles, free_tile_loc, 16, DMA);
    //frown_addr = free_tile_loc; free_tile_loc += 16;
    VDP_loadTileData(pause_checker.tiles, free_tile_loc, 1, DMA);
    pause_checker_addr = free_tile_loc; free_tile_loc += 1;

    //KLog("allocating object?");

    vwf_init();
    free_tile_loc = console_init(free_tile_loc);


    // this stuff is entirely disabled
    //obj_sprite_init(free_tile_loc);

    //const char* init_str = "game initialized!";
    //console_push_message("game initialized!", 17, 30);
    MEM_pack();

    u16 free_bytes = MEM_getFree();
    //while(1) {}
    KLog_U1("free bytes of RAM after init: ", free_bytes);
    MEM_dump();


}

void cleanup_level() {
    clean_object_lists();
    clean_portal_map();
}


game_mode run_game() {
    console_push_message_high_priority("game initialized!", 17, 2);

    u32 start_ticks = getTick();
    update_real_timer();
    calc_movement_speeds();

    int input_res = handle_input();
    if(input_res == LEVEL_END) {
        VDP_setHilightShadow(0);
        const int num_maps = map_table[1];
        init_load_level+= 1;
        if(init_load_level >= num_maps) {
            init_load_level = 0;
        }
        KLog_U1("going to level: ", init_load_level);
        return RESET_MODE;
    } else if (input_res == PAUSE_SCREEN) {
        VDP_setHilightShadow(1);
        for(int ty = 0; ty < 30; ty++) {
            for(int tx = 0; tx < 40; tx++) {
                // priority 0 for shadowing
                VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, 0, 0, 0, pause_checker_addr), tx, ty);
                //base_tile_addr++;
            }
        }
    } else if (input_res == UNPAUSE_SCREEN) {
        VDP_setHilightShadow(0);
        for(int ty = 0; ty < 30; ty++) {
            for(int tx = 0; tx < 40; tx++) {
                // priority 0 for shadowing
                VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, 1, 0, 0, 0), tx, ty);
                //base_tile_addr++;
            }
        }

    }

    if(paused) {
        return SAME_MODE;
    }

    process_all_objects(cur_frame);
    
    // temp disable stuff for debugging tearing
    console_tick();
    inventory_draw();
    update_sfx();
    run_sector_group_processes();

    angleCos32 = cosFix32(cur_player_pos.ang);
    angleSin32 = sinFix32(cur_player_pos.ang); 
    angleCos16 = cosFix16(cur_player_pos.ang);
    angleSin16 = sinFix16(cur_player_pos.ang); 

    // add 50 here 
    playerZCam12Frac4 = (cur_player_pos.z + FIX32(50)) >> (FIX32_FRAC_BITS-4);
    playerXInt = cur_player_pos.x>>FIX32_FRAC_BITS;
    playerYInt = cur_player_pos.y>>FIX32_FRAC_BITS;

    //if(cur_portal_map->palette != NULL) {
    //    maybe_set_palette(cur_portal_map->palette);
    //} else {       
    //    maybe_set_palette(sprite_pal.data); //two_light_levels_pal.data);
    //}
    // hack for door in overlapping room test map
    //if(init_load_level == OVERLAPPING_ROOMS) {
    //    update_wall_vertex();
    //}
    
    //KLog_F2("px: ", cur_player_pos.x, " py: ", cur_player_pos.y);
    //KLog_U1("ang: ", cur_player_pos.ang);
    //KLog_U1("sp: ", cur_player_pos.cur_sector);

    draw_3d_view(cur_frame);
    //SPR_update();
    u32 end_ticks = getTick();
    last_frame_ticks = end_ticks - start_ticks;

    //KLog_U1("ms since last frame: ", ms_since_last_frame());
    //KLog_U1("total vints: ", total_vints());
    cur_frame++;
    return SAME_MODE;
}

void cleanup_game() { 
    bmp_end();
    cleanup_portal_renderer();
    free_clip_buffer_list();
    release_2d_buffers();

    clean_object_lists();
    clean_portal_map();

    console_cleanup();
    vwf_cleanup();

    MEM_pack();
}