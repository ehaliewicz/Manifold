#ifndef PORTAL_H
#define PORTAL_H

#include <genesis.h>
#include "game.h"
#include "portal_map.h"
#include "vertex.h"

void init_portal_renderer();
void cleanup_portal_renderer();

void clear_portal_cache();
void portal_rend(u16 src_sector, u32 cur_frame);
void portal_scan(u16 src_sector, u16 window_min, u16 window_max, u32 cur_frame);
u8* sector_rendered_cache;

#endif