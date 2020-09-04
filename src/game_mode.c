#include <genesis.h>
#include "fire.h"
#include "game_mode.h"
#include "main_menu.h"

void print_invalid_mode();

mode modes[6] = {
    {.name = "invalid mode", .start_up = &print_invalid_mode},
    {.name = "INTRO", .start_up = &init_fire, .clean_up = &cleanup_fire, .run = &run_fire},
    {.name = "MAIN_MENU", .start_up = &init_main_menu, .clean_up = &cleanup_main_menu, .run = &run_main_menu},
    {.name = "IN_GAME", .start_up = &print_invalid_mode},
    {.name = "PAUSE_MENU", .start_up = &print_invalid_mode},
    {.name = "CREDITS", .start_up = &print_invalid_mode},
};

void print_invalid_mode() {
    char buf[32];
    sprintf(buf, "Invalid mode '%s'", modes[cur_game_mode].name);
    while(1) {
        VDP_drawText(buf, 10, 10);
    }
}

game_mode cur_game_mode;

void set_game_mode(game_mode md) {
    cur_game_mode = md;
    modes[md].start_up();
}

void run_game_mode() {
    game_mode next_mode = modes[cur_game_mode].run();
    if(next_mode != SAME_MODE) {
        modes[cur_game_mode].clean_up();
        set_game_mode(next_mode);
    }
}
