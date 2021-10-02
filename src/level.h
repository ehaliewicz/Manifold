#ifndef LEVEL_H
#define LEVEL_H

#include "portal_map.h"

extern const portal_map* cur_portal_map;




void load_portal_map(portal_map* l);
void clean_sector_parameters();

#endif