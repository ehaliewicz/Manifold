#include <genesis.h>
#include "portal_map.h"

s16* sector_data_start(s16 sector_idx, portal_map* mp) {
    return (s16*)(&mp->sectors[sector_idx*SECTOR_SIZE]);
}

s16 sector_wall_offset(s16 sector_idx, portal_map* mp) {
    return mp->sectors[sector_idx*SECTOR_SIZE];
}

s16 sector_portal_offset(s16 sector_idx, portal_map* mp) {
    return mp->sectors[sector_idx*SECTOR_SIZE+1];
}

s16 sector_num_walls(s16 sector_idx, portal_map* mp) {
    return mp->sectors[sector_idx*SECTOR_SIZE+2];
}

u16 sector_group(s16 sector_idx, portal_map* mp) {
    return mp->sectors[sector_idx*SECTOR_SIZE+3];
}


u8 sector_in_pvs_inner(u16 check_sector, s8* entries_for_src_sector) {

    u16 cur_sector = 0;
    s8 el = *entries_for_src_sector++;
    while(el != 0) {
        if(el < 0) {
            // run of zeros
            cur_sector += (-el); // skip past this many sectors
            if(cur_sector > check_sector) {
                return 0;
            }
        } else {
            // run of sectors
            cur_sector += el;
            if(cur_sector > check_sector) {
                return 1;
            }
        }
        el = *entries_for_src_sector++;
    }
    return 0;
}
u8 sector_in_pvs(u16 src_sector, u16 check_sector, portal_map* mp) {
    u32 pvs_offset = mp->sector_pvs_offsets[src_sector];
    s8* entries = &mp->sector_pvs_entries[pvs_offset];
    return sector_in_pvs_inner(check_sector, entries);
}

u8 sector_in_phs(u16 src_sector, u16 check_sector, portal_map* mp) {
    u32 phs_offset = mp->sector_phs_offsets[src_sector];
    s16* entries = &mp->sector_phs_entries[phs_offset];
    return sector_in_pvs_inner(check_sector, entries);
}


void run_in_pvs_inner(u16 src_sector, void (*sect_func)(u16), s8* entries_for_src_sector) {
    u16 cur_sector = 0;
    s8 el = *entries_for_src_sector++;
    while(el != 0) {
        if(el < 0) {
            // run of zeros
            cur_sector += (-el); // skip past this many sectors
        } else {
            // run of sectors
            for(u16 s = cur_sector; s < cur_sector+el; s++) {    
                sect_func(s);
            }
            cur_sector += el;
        }
        el = *entries_for_src_sector++;
    }
}


void run_in_pvs(u16 src_sector, void (*sect_func)(u16), portal_map* mp) {
    u32 pvs_offset = mp->sector_pvs_offsets[src_sector];
    s8* entries = &mp->sector_pvs_entries[pvs_offset];
    run_in_pvs_inner(src_sector, sect_func, entries);
}

void run_in_phs(u16 src_sector, void (*sect_func)(u16), portal_map* mp) {
    u32 pvs_offset = mp->sector_phs_offsets[src_sector];
    s8* entries = &mp->sector_phs_entries[pvs_offset];
    run_in_pvs_inner(src_sector, sect_func, entries);
}

