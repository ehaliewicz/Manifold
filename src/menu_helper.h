#ifndef MENU_HELPER_H
#define MENU_HELPER_H

// a menu is a list of items which are either text, or text + another submenu you can enter

typedef struct menu menu;

typedef struct {
    char* text;
    menu* submenu;
    void (*select)();
    char* (*render)();
} menu_item;

struct menu {
    char* header_text;
    int num_items;
    menu_item items[];
};

typedef struct {
    menu* cur_menu;
    int cur_item;
    menu* prev_menu;
} menu_state;

void init_menu_state(menu* m, menu_state* st);

void run_menu(menu_state* st);



#endif