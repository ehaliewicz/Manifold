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



void run_door(s16 sect_idx, sector_param params) {
    door_lift_state state = cur_portal_map->sector_params[sect_idx].state;
    s16 cur_height = sector_ceil_height(sect_idx, cur_portal_map);
    s16 orig_door_height = params.orig_height;
    switch(state) {
        case CLOSED: do {
                if(cur_portal_map->sector_params[sect_idx].frames_left-- == 0) {
                    cur_portal_map->sector_params[sect_idx].state = GOING_UP;
                }
            } while(0);
            break;

        case GOING_UP: do {
                cur_height += 128;
                set_sector_ceil_height(sect_idx, cur_portal_map, cur_height);
                if(cur_height >= orig_door_height) {
                    set_sector_ceil_height(sect_idx, cur_portal_map, orig_door_height);
                    cur_portal_map->sector_params[sect_idx].state = OPEN;
                    cur_portal_map->sector_params[sect_idx].frames_left = 30;
                }
            } while(0);
            break;

        case OPEN: do {
            if(cur_portal_map->sector_params[sect_idx].frames_left-- == 0) {
                cur_portal_map->sector_params[sect_idx].state = GOING_DOWN;
            }
        } while(0);
            break;

        case GOING_DOWN: do {
            s16 floor_height = sector_floor_height(sect_idx, cur_portal_map);
            cur_height -= 128;
            set_sector_ceil_height(sect_idx, cur_portal_map, cur_height);
            if(cur_height <= floor_height) {
                    set_sector_ceil_height(sect_idx, cur_portal_map, floor_height);
                    cur_portal_map->sector_params[sect_idx].state = CLOSED;
                    cur_portal_map->sector_params[sect_idx].frames_left = 30;
            }
        } while(0);
        break;
    }
}


void run_lift(s16 sect_idx, sector_param params) {
    door_lift_state state = cur_portal_map->sector_params[sect_idx].state;
    s16 cur_height = sector_floor_height(sect_idx, cur_portal_map);
    s16 ceil_height = sector_ceil_height(sect_idx, cur_portal_map);
    s16 orig_lift_height = params.orig_height;
    switch(state) {
        case CLOSED: do {
                if(cur_portal_map->sector_params[sect_idx].frames_left-- == 0) {
                    cur_portal_map->sector_params[sect_idx].state = GOING_DOWN;
                }
            } while(0);
            break;

        case GOING_DOWN: do {
                s16 cur_height = sector_floor_height(sect_idx, cur_portal_map);

                cur_height -= 128;
                set_sector_floor_height(sect_idx, cur_portal_map, cur_height);
                if(cur_height <= orig_lift_height) {
                        set_sector_floor_height(sect_idx, cur_portal_map, orig_lift_height);
                        cur_portal_map->sector_params[sect_idx].state = OPEN;
                        cur_portal_map->sector_params[sect_idx].frames_left = 30;
                }
            } while(0);
            break;

        case OPEN: do {
                if(cur_portal_map->sector_params[sect_idx].frames_left-- == 0) {
                    cur_portal_map->sector_params[sect_idx].state = GOING_UP;
                }
        } while(0);
            break;

        case GOING_UP: do {
                cur_height += 128;
                set_sector_floor_height(sect_idx, cur_portal_map, cur_height);
                if(cur_height >= ceil_height) {
                    set_sector_floor_height(sect_idx, cur_portal_map, ceil_height);
                    cur_portal_map->sector_params[sect_idx].state = CLOSED;
                    cur_portal_map->sector_params[sect_idx].frames_left = 30;
                }
            } while(0);
            break;
    }
}


void run_flash(s16 sect_idx) {
    
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
}

void run_sector_processes() {
    for(int sect_idx = 0; sect_idx < cur_portal_map->num_sectors; sect_idx++) {

        sector_type typ = cur_portal_map->sector_types[sect_idx];
        //u32 param = cur_portal_map->sector_params[sect_idx];
        sector_param params = cur_portal_map->sector_params[sect_idx];

        switch(typ) {
            case NO_TYPE:
                continue;

            case DOOR: do {
                run_door(sect_idx, params);
            } while(0);
            break;

            case FLASHING: do {
                run_flash(sect_idx);  
            } while(0);
            break;

            case LIFT: do {
                run_lift(sect_idx, params);
            } while(0);
            break;

            break;
        }
    }
}