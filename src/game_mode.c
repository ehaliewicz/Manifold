#include <genesis.h>
#include "fire.h"
#include "game.h"
#include "game_mode.h"
#include "main_menu.h"

void print_invalid_mode();

mode modes[7] = {
    {.name = "invalid mode", .start_up = &print_invalid_mode},
    {.name = "invalid mode", .start_up = &print_invalid_mode},
    {.name = "INTRO", .start_up = &init_fire, .clean_up = &cleanup_fire, .run = &run_fire},
    {.name = "MAIN_MENU", .start_up = &init_main_menu, .clean_up = &cleanup_main_menu, .run = &run_main_menu},
    {.name = "IN_GAME", .start_up = &init_game, .clean_up = &cleanup_game, .run = &run_game},
    {.name = "PAUSE_MENU", .start_up = &print_invalid_mode},
    {.name = "CREDITS", .start_up = &print_invalid_mode},
};

game_mode cur_game_mode;

void print_invalid_mode() {
    char buf[32];
    sprintf(buf, "Invalid mode %i/'%s'", cur_game_mode, modes[cur_game_mode].name);
    while(1) {
        VDP_drawText(buf, 10, 10);
    }
}


void set_game_mode(game_mode md) {
    if(md != RESET_MODE) {
        cur_game_mode = md;
    }
    KLog("going to game mode: ");
    KLog(modes[cur_game_mode].name);
    modes[cur_game_mode].start_up();
}

void run_game_mode() {
    
    game_mode next_mode = modes[cur_game_mode].run();

    if(next_mode != SAME_MODE) {
        modes[cur_game_mode].clean_up();
        set_game_mode(next_mode);
    }
}
