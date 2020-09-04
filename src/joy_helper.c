#include <genesis.h>

static u16 last_joy = 0;


int joy_button_newly_pressed(u16 button) {
	return (JOY_readJoypad(JOY_1) & button) && (!(button & last_joy));
}



void update_joy() {
    last_joy = JOY_readJoypad(JOY_1);
}