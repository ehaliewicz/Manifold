#ifndef LEVEL_H
#define LEVEL_H

#include "portal_map.h"

extern portal_map* cur_portal_map;



void load_portal_map(portal_map* l);
void clean_portal_map();
// returns 1 if a switch or door was triggered, else 0
int check_trigger_switch(player_pos* pos);

#endif