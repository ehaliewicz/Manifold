#ifndef PORTAL_H
#define PORTAL_H

#include <genesis.h>
#include "common.h"
#include "game.h"
#include "portal_map.h"
#include "vertex.h"

void init_portal_renderer();

void clear_portal_cache();

void portal_rend(u16 src_sector, u32 cur_frame);

#endif