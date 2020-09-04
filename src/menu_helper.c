#include <genesis.h>
#include "menu_helper.h"

int cur_line;

void reset_menu_frame() {
    cur_line = 0;
}

void draw_line(char* txt) { 
    VDP_drawTextBG(BG_A, txt, 10, cur_line);
    cur_line += 2;
}

void init_menu_state(menu* m, menu_state* s) {
    s->cur_menu = m;
    s->cur_item = 0;
}

void clear_menu(menu* m) {
    reset_menu_frame();
    draw_line("                                        ");
    for(int i = 0; i < m->num_items; i++) {
        draw_line("                                        ");
    }
    reset_menu_frame();
}

void run_menu(menu_state* st) {

    cur_line = 0;

    menu* cur_menu = st->cur_menu;
    int cur_item = st->cur_item;
    menu* prev_menu = st->prev_menu;


    reset_menu_frame();
    draw_line(cur_menu->header_text);

    char buf[40];

    for(int i = 0; i < cur_menu->num_items; i++) {
        menu_item* rend_item = &(cur_menu->items[i]);
        char* rend_str = (rend_item->render == NULL ? "" : rend_item->render());
        if(cur_item == i) {
            sprintf(buf, "> %s %s", rend_item->text, rend_str);
        } else {
            sprintf(buf, "  %s %s", cur_menu->items[i].text, rend_str);
        }
        draw_line(buf);
    }


    if(joy_button_newly_pressed(BUTTON_UP)) {
        cur_item = ((st->cur_item == 0) ? cur_item : cur_item-1);
    } else if (joy_button_newly_pressed(BUTTON_DOWN)) {
        cur_item = ((st->cur_item == cur_menu->num_items-1) ? cur_item : st->cur_item+1);
    } else if(joy_button_newly_pressed(BUTTON_A) || joy_button_newly_pressed(BUTTON_START)) {
        // either select or submenu
        if(cur_menu->items[cur_item].select != NULL) {
            cur_menu->items[cur_item].select();
        } else if (cur_menu->items[cur_item].submenu != NULL) {
            clear_menu(cur_menu);
            prev_menu = cur_menu;
            cur_menu = cur_menu->items[cur_item].submenu;
            cur_item = 0;
        }
    } else if(joy_button_newly_pressed(BUTTON_B)) {
        if(prev_menu != NULL) {
            clear_menu(cur_menu);
            cur_menu = prev_menu;
            prev_menu = NULL; // CAN ONLY NEST ONCE!
            cur_item = 0;
        }
    }

    st->cur_item = cur_item;
    st->cur_menu = cur_menu;
    st->prev_menu = prev_menu;
}

