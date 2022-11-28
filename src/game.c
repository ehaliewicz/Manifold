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
#include "sector_group.h"
#include "sys.h"
#include "utils.h"
#include "vwf.h"

#include "graphics_res.h"
#include "sprites_res.h"


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

}

int cur_frame;


void showStats(u16 float_display)
{
    char str[32];
    u16 y = 5;

    if (float_display)
    {
        fix32 fps = SYS_getFPSAsFloat();
        fix32ToStr(fps, str, 1);
        VDP_clearTextBG(BG_B, 1, y, 4);
    }
    else
    {
        uintToStr(SYS_getFPS(), str, 1);
        VDP_clearTextBG(BG_B, 1, y, 2);
    }

    // display FPS
    VDP_drawTextBG(BG_B, str, 1, y++);

    //sprintf(str, "%i ", vints);
    //vints = 0;
    //VDP_drawTextBG(BG_B, str, 1, 6); 

    //sprintf(str, "s: %i ", cur_player_pos.cur_sector);
    //VDP_drawTextBG(BG_B, str, 1, y++);
    
    //KLog_U1("sector: ", cur_player_pos.cur_sector);
    //KLog_S3("px: ", cur_player_pos.x, " py: ", cur_player_pos.y, " pz: ", cur_player_pos.z);
    //KLog_U1("ang: ", cur_player_pos.ang);
    
    //sprintf(str, "sll: %i ", get_sector_group_light_level(sector_group(cur_player_pos.cur_sector, cur_portal_map)));
    //VDP_drawTextBG(BG_B, str, 1, 8);
    //sprintf(str, "sg: %i", sector_group(cur_player_pos.cur_sector, cur_portal_map));
    //VDP_drawTextBG(BG_B, str, 1, y++);





}




void draw_3d_view(u32 cur_frame) {
    
    // for fading, it might be cheaper to just always clear the screen, and not draw faded/transparent pixels
    //if(1) { 
    if (render_mode == RENDER_WIREFRAME || JOY_readJoypad(JOY_1) & BUTTON_A) {
        bmp_vertical_clear();
    }
    // clear clipping buffers
    clear_2d_buffers();

    // clear portal graph visited cache
    clear_portal_cache();

    // recursively render portal graph
    portal_rend(cur_player_pos.cur_sector, cur_frame);


    // display fps
    showStats(1);

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



//Sprite* shotgun_spr;
#define BASE_GUN_X 128
#define BASE_GUN_Y 120

int shotgun_spr_idx_start, shotgun_num_sprites;


void set_gun_pos() {
    u8 idx = (gun_bob_idx>>4) % 20;

    s16 bobOffX = gun_bobs[idx].x<<1;
    s16 bobOffY = gun_bobs[idx].y<<1;

    
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

u32 init_shotgun(u32 tile_loc) {

    VDP_loadTileSet(shotgun.animations[0]->frames[0]->tileset, tile_loc, DMA);
    //u8 shotgun_w = shotgun.animations[0]->frames[0]->w/8;
    //u8 shotgun_h = shotgun.animations[0]->frames[0]->h/8;
    AnimationFrame *frame1 = shotgun.animations[0]->frames[0];

    VDP_resetSprites();
    s16 spr_idx = VDP_allocateSprites(frame1->numSprite);
    shotgun_num_sprites = frame1->numSprite;
    shotgun_spr_idx_start = spr_idx;

    u16 tile_idx = tile_loc;

    FrameVDPSprite** f = frame1->frameInfos[0].frameVDPSprites;
    for(int i = 0; i < frame1->numSprite; i++) {
        FrameVDPSprite* spr = f[i];

        VDP_setSprite(
            spr_idx+i,
            BASE_GUN_X+spr->offsetX, BASE_GUN_Y+spr->offsetY,
            spr->size,
            TILE_ATTR_FULL(PAL0, 0, 0, 0, tile_idx)
        );
        tile_idx += spr->numTile;

    }
    tile_loc += shotgun.maxNumTile;
    VDP_linkSprites(0, frame1->numSprite);
    VDP_updateSprites(frame1->numSprite, DMA);

    return tile_loc;
}


int holding_down_move = 0;

u8 render_mode;

void handle_input() {
    int strafe = joy_button_pressed(BUTTON_C);

    //int pressed_b = joy_button_pressed(BUTTON_B);


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

    u16 sect_group = sector_group(cur_player_pos.cur_sector, cur_portal_map);
    if(moved) {

        //collision_result collision = check_for_collision(curx, cury, newx, newy, cur_player_pos.cur_sector);
        collision_result collision = check_for_collision_radius(curx, cury, newx, newy, PLAYER_COLLISION_DISTANCE, cur_player_pos.cur_sector);
        cur_player_pos.x = collision.pos.x;
        cur_player_pos.y = collision.pos.y;
        if(collision.new_sector != cur_player_pos.cur_sector) {
            cur_player_pos.cur_sector = collision.new_sector;
            sect_group = sector_group(collision.new_sector, cur_portal_map);
            activate_sector_group_enter_trigger(sect_group);
            
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

    s16 cur_sector_height = get_sector_group_floor_height(sect_group);
    cur_player_pos.z = (cur_sector_height<<(FIX32_FRAC_BITS-4)) + FIX32(50);   

    fix32 run_bob_amt = bobs[run_bob_idx] * FIX32(1.5);
    fix32 idle_bob_amt = bobs[idle_bob_idx] * FIX32(.2);

    cur_player_pos.z += run_bob_amt;
    cur_player_pos.z += idle_bob_amt;
    

    u8 move_button = joy_button_pressed(BUTTON_UP) ? BUTTON_UP : (joy_button_pressed(BUTTON_DOWN) ? BUTTON_DOWN : 0);
    
    if(!holding_down_move && move_button) {
        holding_down_move = move_button;
    } else if (holding_down_move && (holding_down_move == move_button)) {
        step_gun_bob();
    } else {
        holding_down_move = 0;
        reset_gun_bob();
    }
    
    return;


    pause_game = joy_button_pressed(BUTTON_START);

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


void init_sector_0_jump_position() {
    sector_centers = malloc(sizeof(Vect2D_f32) * cur_portal_map->num_sectors,
                            "sector center table");
    
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
        break; 
    }
}


void init_player_pos() {
    cur_player_pos.x = sector_centers[0].x;
    cur_player_pos.y = sector_centers[0].y;
    cur_player_pos.cur_sector = 0;

    u16 sect_group = sector_group(cur_player_pos.cur_sector, cur_portal_map);

    cur_player_pos.z = (get_sector_group_floor_height(sect_group)<<(FIX32_FRAC_BITS-4)) + FIX32(50);


    cur_player_pos.ang = 0;
}


void do_vint_flip();


u16 free_tile_loc = 0x390;




void init_game() {
    render_mode = RENDER_SOLID;

    
    //VDP_clearPlane(BG_B, 1); clears the fps display somehow
    //VDP_clearPlane(BG_A, 1);

    //DMA_doVRamFill(0x0390, 0x0170, 0x00, 1);
    VDP_fillTileData(0x00, 0x0390, 0x170, 1);
        
    XGM_stopPlay();
    //SYS_setVIntCallback(do_vint_flip);
    VDP_setVerticalScroll(BG_B, 0);
    
    #ifdef H32_MODE
    VDP_setScreenWidth256();
    #else 
    VDP_setScreenWidth320();
    #endif
    //VDP_setScreenHeight240();



    holding_down_move = 0;

    cur_frame = 1;
    
	VDP_setBackgroundColor(10);
    volatile u32* vp_start_in_game_arr = start_in_game_arr;
    KLog_U1("free bytes of RAM before init: ", MEM_getFree());

    if(vp_start_in_game_arr[1]) {
        volatile u32* vp_init_load_level_arr = instant_load_level_array;
        volatile int init_level = vp_init_load_level_arr[1];
        load_portal_map((portal_map*)map_table[2+init_level]);
    } else {
        load_portal_map((portal_map*)map_table[2+init_load_level]);
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
        init_sector_0_jump_position();



    }


    init_clip_buffer_list();

    cur_palette = two_light_levels_pal.data;

    // palette 0 is weapon palette
    PAL_setPalette(PAL0, shotgun.palette->data);

    // palette 1 is 3d palette
    //PAL_setPalette(PAL1, cur_palette);
    if(cur_portal_map->palette != NULL) {
        PAL_setPalette(PAL1, cur_portal_map->palette);
    } else {
        PAL_setPalette(PAL1, sprite_pal.data);
    }

    // palette 2 is HUD palette, set in inventory_init
    // palette 3 is sprite palette
    PAL_setPalette(PAL3, sprite_pal.data);
    VDP_setTextPalette(PAL3);
    
    clear_menu();

    // skybox is PAL3
    bmp_init_vertical(1, BG_A, PAL1, 0);

    VDP_waitVSync();

    //u16 skybox_gradient_basetile = TILE_ATTR_FULL(PAL3, 0, 0, 0, free_tile_loc);
	//VDP_drawImageEx(BG_B, &skybox_gradient, skybox_gradient_basetile, 4, 4, 0, 0);
    //PAL_setPalette(PAL3, skybox_gradient.palette->data);
    //free_tile_loc += skybox_gradient.tileset->numTile;
    //VDP_waitVSync();
    
    // palette 2 is HUD palette
    
    //free_tile_loc = inventory_init(free_tile_loc);
    //inventory_draw();
    
    VDP_waitVSync();


    init_sprite_draw_cache();
    init_2d_buffers();

    init_portal_renderer();

    KLog_U1("num sectors: ", cur_portal_map->num_sectors);
    init_object_lists(cur_portal_map->num_sectors);

    free_tile_loc = init_shotgun(free_tile_loc);

    //KLog("allocating object?");
    s16 obj_sect_group = sector_group(0, cur_portal_map);

    int got_player_thing = 0;
    map_object* player_thing = NULL;

    KLog_U1("allocating objects: ", cur_portal_map->num_things);

    for(int i = 0; i < cur_portal_map->num_things; i++) {
        map_object* thg = &cur_portal_map->things[i];
        KLog_U1("thing type: ", thg->type);
        volatile object_template* typ = &object_types[thg->type];
        if(thg->type == 0) { //typ->is_player) {
            KLog("is player?");
            got_player_thing = 1;
            player_thing = thg;
        } else {

            alloc_object_in_sector(
                i&1, // either 0 or 1, to spread the load
                thg->sector_num,
                thg->x<<FIX32_FRAC_BITS, 
                thg->y<<FIX32_FRAC_BITS, 
                thg->z<<FIX32_FRAC_BITS, 
                thg->type
            );
        }
    }

    init_player_pos();

    vwf_init();
    free_tile_loc = console_init(free_tile_loc);

    //obj_sprite_init(free_tile_loc);

    //const char* init_str = "game initialized!";
    console_push_message("game initialized!", 17, 30);

    MEM_pack();

    u16 free_bytes = MEM_getFree();
    //while(1) {}
    KLog_U1("free bytes of RAM after init: ", free_bytes);
    MEM_dump();

    if(cur_portal_map->xgm_track != NULL) {
        XGM_startPlay(cur_portal_map->xgm_track);
    } 

}

void maybe_set_palette(u16* new_palette) {
    if(cur_palette != new_palette) {
        PAL_setPalette(PAL1, new_palette);
        cur_palette = new_palette;
    }
}

game_mode run_game() {

    u32 start_ticks = getTick();
    
    process_all_objects(cur_frame);

    console_tick();
    //inventory_draw();
    run_sector_group_processes();
    calc_movement_speeds();
    handle_input();

    angleCos32 = cosFix32(cur_player_pos.ang);
    angleSin32 = sinFix32(cur_player_pos.ang); 
    angleCos16 = cosFix16(cur_player_pos.ang);
    angleSin16 = sinFix16(cur_player_pos.ang); 

    playerZ12Frac4 = cur_player_pos.z >> (FIX32_FRAC_BITS-4);
    playerXInt = cur_player_pos.x>>FIX32_FRAC_BITS;
    playerYInt = cur_player_pos.y>>FIX32_FRAC_BITS;


    maybe_set_palette(two_light_levels_pal.data);

    // hack for door in overlapping room test map
    //if(init_load_level == OVERLAPPING_ROOMS) {
    //    update_wall_vertex();
    //}
    draw_3d_view(cur_frame);

    //SPR_update();
    u32 end_ticks = getTick();
    last_frame_ticks = end_ticks - start_ticks;

    cur_frame++;
    return SAME_MODE;
}


void cleanup_game() { 
    bmp_end();
    free(sector_centers, "sector center table");
    cleanup_portal_renderer();
    free_clip_buffer_list();
    release_2d_buffers();
    clear_object_lists();
    clean_sector_parameters();

    //console_cleanup();
    vwf_cleanup();

    MEM_pack();
}