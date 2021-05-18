#include <genesis.h>
#include "portal_map.h"

//const level* cur_level = NULL;
const portal_map* cur_portal_map = NULL;



void set_portal_map(portal_map* l) {
    cur_portal_map = l;
}

