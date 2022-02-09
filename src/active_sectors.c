#include <genesis.h>
#include "active_sectors.h"
#include "portal_map.h"


u32* active_sectors;
u16 num_longwords;

void init_active_sectors() {
    active_sectors = MEM_alloc(MAX_SECTOR_GROUPS/8);
    num_longwords = MAX_SECTOR_GROUPS/32;
}

void cleanup_active_sectors() {
    MEM_free(active_sectors);
}

void register_sect_group_as_active(u16 sect_group) {
    u16 lw_index = sect_group>>5;
    u8 bit_index = sect_group & 0b111;
    active_sectors[lw_index] |= (1 << bit_index);
}

void register_sect_group_as_inactive(u16 sect_group) {
    u16 lw_index = sect_group>>5;
    u8 bit_index = sect_group & 0b111;
    active_sectors[lw_index] &= ~(1 << bit_index);
}


void iterate_active_sectors(active_sector_callback cb) {
    for(u16 lw_index = 0; lw_index < num_longwords; lw_index++) {
        u32 lw = active_sectors[lw_index];
        if(lw == 0) { continue; }

        u16 base_index = lw_index<<5;

        for(u32 bit_index = 0; bit_index < 32; bit_index++) {
            if(lw & (1<<bit_index)) {
                u16 sect_group_index = base_index+bit_index;
                cb(sect_group_index);
            }
        }
    }
}