#include "sfx.h"

int sfx_on = TRUE;

void toggle_sfx(int menu_idx) {
    sfx_on = ! sfx_on;
}