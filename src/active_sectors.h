#ifndef ACTIVE_SECTORS
#define ACTIVE_SECTORS

#include <genesis.h>

typedef void(*active_sector_callback)(u16 sect_group);


void init_active_sectors();

void cleanup_active_sectors();

void register_sect_group_as_active(u16 sect_group);

void register_sect_group_as_inactive(u16 sect_group);

void iterate_active_sectors(active_sector_callback cb);

#endif