#ifndef JOY_HELPER_H
#define JOY_HELPER_H

#include <genesis.h>


int joy_button_newly_pressed(u16 btn);

int joy_button_held(u16 button);

void update_joy();


#endif