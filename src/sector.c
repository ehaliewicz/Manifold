#include <genesis.h>
#include "game.h"
#include "level.h"
#include "portal_map.h"
 

s8 get_sector_light_level(s16 sect_idx) {
    sector_param param = cur_portal_map->sector_params[sect_idx];
    return param.light;
}

s16 get_flashing_sector_ticks_left(s16 sect_idx) {
    sector_param param = cur_portal_map->sector_params[sect_idx];
    return param.par2;
}

s8 get_stash_param(s16 sect_idx) {
    return cur_portal_map->sector_params[sect_idx].stash;
}

void set_sector_light_level(s16 sect_idx, s8 light_level) {
    cur_portal_map->sector_params[sect_idx].light = light_level;
}

void set_flashing_sector_ticks_left(s16 sect_idx, u8 ticks_left) {
    cur_portal_map->sector_params[sect_idx].par2 = ticks_left;
}

void set_stash_param(s16 sect_idx, s8 stash) {
    cur_portal_map->sector_params[sect_idx].stash = stash;
}

void run_sector_processes() {
    for(int sect_idx = 0; sect_idx < cur_portal_map->num_sectors; sect_idx++) {

        sector_type typ = cur_portal_map->sector_types[sect_idx];
        //u32 param = cur_portal_map->sector_params[sect_idx];

        switch(typ) {
            case NO_TYPE:
                continue;
            case FLASHING: do {
                s8 light_level = get_sector_light_level(sect_idx);
                s16 ticks_left = get_flashing_sector_ticks_left(sect_idx);
                //u8 ticks_til_flash = get_flashing_sector_ticks_til_flash(sect_idx);
                //u8 flash_length = get_flashing_sector_flash_length(sect_idx);
                ticks_left -= last_frame_ticks;

                switch(light_level) {
                    case 2:
                        // figure out when to switch back
                        ticks_left -= last_frame_ticks;
                        if(ticks_left <= 0) {
                            s8 diff = abs(ticks_left);
                            u16 ticks_til_flash = random() & 63;
                            ticks_left = ticks_til_flash - diff;
                            light_level = 0;
                        }
                        break;
                    default:
                        // figure out when to flash
                        ticks_left -= last_frame_ticks;
                        if(ticks_left <= 0) {
                            s8 diff = abs(ticks_left);
                            u16 flash_length = random() & 255;
                            ticks_left = flash_length - diff;
                            light_level = 2;
                        }

                        break;

                }

                set_sector_light_level(sect_idx, light_level);
                set_flashing_sector_ticks_left(sect_idx, ticks_left);
            } while(0);
            break;
        }
    }
}