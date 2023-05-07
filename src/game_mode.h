#ifndef GAME_MODE_H
#define GAME_MODE_H


typedef enum {
    SAME_MODE=0,
    RESET_MODE=1,
    INTRO=2,
    MAIN_MENU=3,
    IN_GAME=4,
    PAUSE_MENU=5,
    CREDITS=6
} game_mode;


typedef struct {
    char* name;
    void (*start_up)();
    game_mode (*run)();
    void (*clean_up)();
} mode;

extern game_mode cur_game_mode;
void run_game_mode();
void set_game_mode(game_mode md);

#endif