#ifndef LEVEL_H
#define LEVEL_H

#include "portal_map.h"

extern portal_map* cur_portal_map;

extern u16 *wall_tex_repetitions;



void load_portal_map(portal_map* l);
void clean_portal_map();

#endif