#include <genesis.h>
#include "menu_helper.h"
#include "joy_helper.h"

int cur_line;

void reset_menu_frame() {
    cur_line = 0;
}

void draw_line(char* txt) { 
    int num_chars = strlen(txt);
    int diff = 40 - num_chars;

    VDP_drawTextBG(BG_A, txt, (diff/2)-1, cur_line); // 12
    cur_line += 2;
}

int find_first_selectable_item(menu* m) {
    for(int i = 0; i < m->num_items; i++) {
        if(m->items[i].selectable) {
            return i;
        }
    }
    return -1;
}

void init_menu_state(const menu* m, menu_state* s) {
    s->cur_menu = m;
    s->cur_item = find_first_selectable_item(m);
}

void clear_menu() {
    cur_line = 0;
    for(int i = 0; i < 240/8; i++) {
        VDP_drawText("                                        ", 0, i);
    }
}

void run_menu(menu_state* st) {

    const menu* cur_menu = st->cur_menu;
    int cur_item = st->cur_item;
    const menu* prev_menu = st->prev_menu;


    int max_lines = 30;
    int num_lines = cur_menu->num_items*2;
    int diff = max_lines - num_lines;
    int start_off = diff/2;

    cur_line = start_off;

    draw_line(cur_menu->header_text);

    char buf[40];

    for(int i = 0; i < cur_menu->num_items; i++) {
        const menu_item* rend_item = &(cur_menu->items[i]);
        char* rend_str = (rend_item->render == NULL ? "" : rend_item->render());
        char* cursor = (cur_item == i) ? ">" : " ";
       
        sprintf(buf, "%s %s %s", cursor, rend_item->text, rend_str);
        draw_line(buf);
    }


    if(joy_button_newly_pressed(BUTTON_UP) && (cur_item != -1))  {
        cur_item = ((st->cur_item == 0) ? cur_item : cur_item-1);
    } else if (joy_button_newly_pressed(BUTTON_DOWN) && (cur_item != -1)) {
        cur_item = ((st->cur_item == cur_menu->num_items-1) ? cur_item : st->cur_item+1);
    } else if(joy_button_newly_pressed(BUTTON_A) || joy_button_newly_pressed(BUTTON_START)) {

        if(cur_menu->items[cur_item].select != NULL) {
            cur_menu->items[cur_item].select();
        }

        if (cur_menu->items[cur_item].submenu != NULL) {
            clear_menu(cur_menu);
            prev_menu = cur_menu;
            cur_menu = cur_menu->items[cur_item].submenu;
            cur_item = find_first_selectable_item(cur_menu);
        }

    } else if(joy_button_newly_pressed(BUTTON_B) && (prev_menu != NULL)) {
        clear_menu(cur_menu);
        cur_menu = prev_menu;
        prev_menu = NULL; // CAN ONLY NEST ONCE!
        cur_item = find_first_selectable_item(cur_menu);
        
    }

    st->cur_item = cur_item;
    st->cur_menu = cur_menu;
    st->prev_menu = prev_menu;
}

