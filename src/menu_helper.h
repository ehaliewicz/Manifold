#ifndef MENU_HELPER_H
#define MENU_HELPER_H

// a menu is a list of items which are either text, or text + another submenu you can enter

typedef struct menu menu;

typedef struct {
    char* text;
    const menu* submenu;
    void (*select)();
    char* (*render)();
    int selectable;
} menu_item;

struct menu {
    char* header_text;
    int num_items;
    const menu_item items[];
};

typedef struct {
    const menu* cur_menu;
    int cur_item;
    const menu* prev_menu;
} menu_state;

void init_menu_state(const menu* m, menu_state* st);

void run_menu(menu_state* st);

void clear_menu();

#define MAX_MENU_ITEMS 14


#endif