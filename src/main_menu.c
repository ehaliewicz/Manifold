#include <genesis.h>
#include "game.h"
#include "game_mode.h"
#include "init.h"
#include "menu_helper.h"
#include "portal_map.h"
#include "sfx.h"
#include "music.h"

static int launch_new_mode = 0;
game_mode target_mode;

void go_to_credits() {
    launch_new_mode = 1;
    target_mode = CREDITS; 
}

void go_to_new_game() {
    launch_new_mode = 1;
    target_mode = IN_GAME;
}


#define TEXT_ITEM(str) {.text = (str), .submenu = NULL, .select = NULL, .render = NULL, .selectable = 0}

const menu load_game_menu = {
    .header_text = "Load game",
    .num_items = 1,
    .items = {
        TEXT_ITEM("No saves found")
    }
};

#include "map_table.h"

void select_map(int menu_idx) {
    launch_new_mode = 1;
    init_load_level = menu_idx; 
    target_mode = IN_GAME;
}

menu level_select_menu = {
    .header_text = "Level select",
    .num_items = 4,
    .items = {
        {.text = "Slime room test map", .submenu = NULL, .select = &select_map, .selectable=1},
        {.text = "Overlapping rooms test map", .submenu = NULL, .select = &select_map, .selectable=1},
        {.text = "Building test map", .submenu = NULL, .select = &select_map, .selectable=1},
        {.text = "NO MAP", .submenu = NULL, .select = &select_map, .selectable=0},

    }
};


void populate_level_select() {
    volatile u32* vmap_table = map_table;

    volatile uint32_t num_maps = map_table[1] >= 6 ? 6 : vmap_table[1];
    KLog_U1("num maps: ", num_maps);
    for(u32 i = 0; i < num_maps; i++) { 
        KLog_U1("setting map for index: ", i);
        portal_map* pm = (portal_map*)vmap_table[2+i];
        level_select_menu.items[i].text = pm->name;
        KLog_U1("pointer to name is : ", (u32)(level_select_menu.items[i].text));
        level_select_menu.items[i].selectable = 1;
    }
}

char* draw_sfx_state(int menu_idx) {
    return (sfx_on ? "ON " : "OFF");
    
}

char* draw_music_state(int menu_idx) {
    return (music_on ? "ON " : "OFF");
}

const menu options_menu = {
    .header_text = "Options",
    .num_items = 2,
    .items = {
        {.text = "Sound Effects: ", .submenu = NULL, .select = &toggle_sfx, .render = &draw_sfx_state, .selectable=1},
        {.text = "Music: ", .submenu = NULL, .select = &toggle_music, .render = &draw_music_state, .selectable=1}
    }
};


const menu credits_menu = {
    .header_text = "Credits",
    .num_items = 1,
    .items = {
        TEXT_ITEM("...."),
    }
};


const menu main_menu = {
    .header_text = "",
    .num_items = 5,
    .items = {
        {.text = "New game", .submenu = NULL, .select = &go_to_new_game, .selectable=1},
        {.text = "Level select", .submenu = &level_select_menu, .select = populate_level_select, .selectable=1},
        {.text = "Load game", .submenu = &load_game_menu, .select = NULL, .selectable=1},
        {.text = "Options", .submenu = &options_menu, .select = NULL, .selectable=1},
        {.text = "Credits", .submenu = &credits_menu, .select = NULL, .selectable=1} // .select = &go_to_credits},
    }
};

menu_state main_menu_state;

void init_main_menu() {
    VDP_setScreenWidth320();
    VDP_setScreenHeight240();
    launch_new_mode = 0;
    init_menu_state(&main_menu, &main_menu_state);
}


game_mode run_main_menu() {
    launch_new_mode = 0;
    run_menu(&main_menu_state);
    if(launch_new_mode) {
        return target_mode;
    }
    return SAME_MODE;
}

void cleanup_main_menu() {

}