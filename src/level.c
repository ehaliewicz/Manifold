#include <genesis.h>
#include "e1m1.h"
#include "level.h"
#include "portal_map.h"
#include "portal_small_map.h"

const level* cur_level = NULL;
const portal_map* cur_portal_map = NULL;



void set_level(level* l) {
    cur_level = l;
}


void set_portal_map(portal_map* l) {
    cur_portal_map = l;
}

