#include <genesis.h>
#include "active_sectors.h"
#include "portal_map.h"
#include "utils.h"


static u32* active_sectors;

void init_active_sectors(u16 num_sect_groups) {
    // 64 bytes
    if(num_sect_groups > MAX_SECTOR_GROUPS) {
        die("Too many sector groups!");
    }
    active_sectors = malloc((num_sect_groups + (8-1))/8, "active_sector_bitmap");
}

void cleanup_active_sectors() {
    free(active_sectors, "active_sector_bitmap");
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
    for(u16 lw_index = 0; lw_index < MAX_SECTOR_GROUPS/32; lw_index++) {
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