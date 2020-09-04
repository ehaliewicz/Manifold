#include <genesis.h>
#include "game_mode.h"
#include "menu_helper.h"
#include "sfx.h"
#include "music.h"

int launch_new_mode = 0;
game_mode target_mode;

void go_to_credits() {
    launch_new_mode = 1;
    target_mode = CREDITS; 
}

void go_to_new_game() {
    launch_new_mode = 1;
    target_mode = IN_GAME;
}

void go_to_e1m1() {
    launch_new_mode = 1;
    target_mode = IN_GAME;
}

menu load_game_menu = {
    .header_text = "Load game",
    .num_items = 1,
    .items = {
        {.text = "No saves found", .submenu = NULL, .select = NULL}
    }
};

menu level_select_menu = {
    .header_text = "Level select",
    .num_items = 1,
    .items = {
        {.text = "E1M1", .submenu = NULL, .select = go_to_e1m1},
    }
};

char* draw_sfx_state() {
    return (sfx_on ? "ON " : "OFF");
}

char* draw_music_state() {
    return (music_on ? "ON " : "OFF");
}

menu options_menu = {
    .header_text = "Options",
    .num_items = 2,
    .items = {
        {.text = "Sound Effects: ", .submenu = NULL, .select = &toggle_sfx, .render = &draw_sfx_state},
        {.text = "Music: ", .submenu = NULL, .select = &toggle_music, .render = &draw_music_state}
    }
};

menu main_menu = {
    .header_text = "",
    .num_items = 5,
    .items = {
        {.text = "New game", .submenu = NULL, .select = &go_to_new_game},
        {.text = "Level select", .submenu = &level_select_menu, .select = NULL},
        {.text = "Load game", .submenu = &load_game_menu, .select = NULL},
        {.text = "Options", .submenu = &options_menu, .select = NULL},
        {.text = "Credits", .submenu = NULL, .select = &go_to_credits},
    }
};

menu_state main_menu_state;

void init_main_menu() {
    init_menu_state(&main_menu, &main_menu_state);
}


game_mode run_main_menu() {
    run_menu(&main_menu_state);
    if(launch_new_mode) {
        return target_mode;
    }
    return SAME_MODE;
}

void cleanup_main_menu() {

}