#include <genesis.h>
#include "e1m1.h"
#include "level.h"


const level* cur_level = &e1m1;


void set_level(level* l) {
    cur_level = l;
}