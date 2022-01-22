#include <genesis.h>
#include "game.h"
#include "level.h"
#include "portal_map.h"
#include "sector.h"

s16* live_sector_parameters;


s8 get_sector_light_level(u16 sect_idx) {
    return live_sector_parameters[(sect_idx<<NUM_SECTOR_PARAMS_SHIFT)+SECTOR_PARAM_LIGHT_IDX];
}

void set_sector_light_level(u16 sect_idx, s8 light_level) {
    live_sector_parameters[(sect_idx<<NUM_SECTOR_PARAMS_SHIFT)+SECTOR_PARAM_LIGHT_IDX] = light_level;
}


s16 get_sector_orig_height(u16 sect_idx) {
    return live_sector_parameters[(sect_idx<<NUM_SECTOR_PARAMS_SHIFT)+SECTOR_PARAM_ORIG_HEIGHT_IDX];
}

u16 get_sector_ticks_left(u16 sect_idx) {
    return live_sector_parameters[(sect_idx<<NUM_SECTOR_PARAMS_SHIFT)+SECTOR_PARAM_TICKS_LEFT_IDX];
}

void set_sector_ticks_left(u16 sect_idx, u16 ticks_left) {
    live_sector_parameters[(sect_idx<<NUM_SECTOR_PARAMS_SHIFT)+SECTOR_PARAM_TICKS_LEFT_IDX] = ticks_left;
}

u16 get_sector_state(u16 sect_idx) {
    return live_sector_parameters[(sect_idx<<NUM_SECTOR_PARAMS_SHIFT)+SECTOR_PARAM_STATE_IDX];
}

void set_sector_state(u16 sect_idx, u16 state) {
    live_sector_parameters[(sect_idx<<NUM_SECTOR_PARAMS_SHIFT)+SECTOR_PARAM_STATE_IDX] = state;
}

s16 get_sector_floor_height(u16 sect_idx) {
    return live_sector_parameters[(sect_idx<<NUM_SECTOR_PARAMS_SHIFT) + SECTOR_PARAM_FLOOR_HEIGHT_IDX];
}

void set_sector_floor_height(u16 sect_idx, s16 height) {
    live_sector_parameters[(sect_idx<<NUM_SECTOR_PARAMS_SHIFT) + SECTOR_PARAM_FLOOR_HEIGHT_IDX] = height;
}

s16 get_sector_ceil_height(u16 sect_idx) {
    return live_sector_parameters[(sect_idx<<NUM_SECTOR_PARAMS_SHIFT) + SECTOR_PARAM_CEIL_HEIGHT_IDX];
}

void set_sector_ceil_height(u16 sect_idx, s16 height) {
    live_sector_parameters[(sect_idx<<NUM_SECTOR_PARAMS_SHIFT) + SECTOR_PARAM_CEIL_HEIGHT_IDX] = height;
}

s16 get_sector_floor_color(u16 sect_idx) {
    return live_sector_parameters[(sect_idx<<NUM_SECTOR_PARAMS_SHIFT) + SECTOR_PARAM_FLOOR_COLOR_IDX];
}

s16 get_sector_ceil_color(u16 sect_idx) {
    return live_sector_parameters[(sect_idx<<NUM_SECTOR_PARAMS_SHIFT) + SECTOR_PARAM_CEIL_COLOR_IDX];
}

void run_door(u16 sect_idx, s16* params) {
    door_lift_state state = params[SECTOR_PARAM_STATE_IDX];
    s16 cur_height = get_sector_ceil_height(sect_idx);
    s16 orig_door_height = params[SECTOR_PARAM_ORIG_HEIGHT_IDX];

    switch(state) {
        case CLOSED: do {
                u16 ticks_left = params[SECTOR_PARAM_TICKS_LEFT_IDX];
                if(ticks_left == 0) {
                    params[SECTOR_PARAM_STATE_IDX] = GOING_UP;
                } else {
                    params[SECTOR_PARAM_TICKS_LEFT_IDX]--;
                }
            } while(0);
            break;

        case GOING_UP: do {
            cur_height += 128;
            set_sector_ceil_height(sect_idx, cur_height);
            if(cur_height >= orig_door_height) {
                set_sector_ceil_height(sect_idx, orig_door_height);
                params[SECTOR_PARAM_STATE_IDX] = OPEN;
                params[SECTOR_PARAM_TICKS_LEFT_IDX] = 30;
            }
        } while(0);
        break;

        case OPEN: do {
            u16 ticks_left = params[SECTOR_PARAM_TICKS_LEFT_IDX];

            if(ticks_left == 0) {
                params[SECTOR_PARAM_STATE_IDX] = GOING_DOWN;
            } else {
                params[SECTOR_PARAM_TICKS_LEFT_IDX]--;
            }
        } while(0);
            break;

        case GOING_DOWN: do {
            s16 floor_height = get_sector_floor_height(sect_idx);
            cur_height -= 128;
            set_sector_ceil_height(sect_idx, cur_height);
            if(cur_height <= floor_height) {
                set_sector_ceil_height(sect_idx, floor_height);
                params[SECTOR_PARAM_STATE_IDX] = CLOSED;
                params[SECTOR_PARAM_TICKS_LEFT_IDX] = 30;
            }
        } while(0);
        break;
    }
}


void run_lift(u16 sect_idx, s16* params) {
    door_lift_state state = params[SECTOR_PARAM_STATE_IDX];
    s16 cur_height = get_sector_floor_height(sect_idx);
    s16 ceil_height = get_sector_ceil_height(sect_idx);
    s16 orig_lift_height = params[SECTOR_PARAM_ORIG_HEIGHT_IDX];
    switch(state) {
        case CLOSED: do {
            u16 ticks_left = params[SECTOR_PARAM_TICKS_LEFT_IDX];
            if(ticks_left == 0) {
                params[SECTOR_PARAM_STATE_IDX] = GOING_DOWN;
            } else {
                params[SECTOR_PARAM_TICKS_LEFT_IDX]--;
            }
        } while(0);
        break;

        case GOING_DOWN: do {
                s16 cur_height = get_sector_floor_height(sect_idx);

                cur_height -= 128;
                set_sector_floor_height(sect_idx, cur_height);
                if(cur_height <= orig_lift_height) {
                        set_sector_floor_height(sect_idx, orig_lift_height);
                        params[SECTOR_PARAM_STATE_IDX] = OPEN;
                        params[SECTOR_PARAM_TICKS_LEFT_IDX] = 30;
                }
            } while(0);
            break;

        case OPEN: do {
            u16 ticks_left = params[SECTOR_PARAM_TICKS_LEFT_IDX];
            if(ticks_left == 0) {
                params[SECTOR_PARAM_STATE_IDX] = GOING_UP;
            } else {
                params[SECTOR_PARAM_TICKS_LEFT_IDX]--;
            }
        } while(0);
            break;

        case GOING_UP: do {
                cur_height += 128;
                set_sector_floor_height(sect_idx, cur_height);
                if(cur_height >= ceil_height) {
                    set_sector_floor_height(sect_idx, ceil_height);
                    params[SECTOR_PARAM_STATE_IDX] = CLOSED;
                    params[SECTOR_PARAM_TICKS_LEFT_IDX] = 30;
                }
            } while(0);
            break;
    }
}


void run_flash(u16 sect_idx, s16* params) {
    s8 light_level = params[SECTOR_PARAM_LIGHT_IDX];
    s16 ticks_left = params[SECTOR_PARAM_TICKS_LEFT_IDX];

    ticks_left -= last_frame_ticks;

    switch(light_level) {
        case 2:
            // figure out when to switch back
            ticks_left -= last_frame_ticks;
            if(ticks_left <= 0) {
                s8 diff = abs(ticks_left);
                u8 ticks_til_flash = fxrandom();
                ticks_left = ticks_til_flash - diff;
                light_level = 0;
            }
            break;
        default:
            // figure out when to flash
            ticks_left -= last_frame_ticks;
            if(ticks_left <= 0) {
                s8 diff = abs(ticks_left);
                u8 flash_length = fxrandom();
                ticks_left = flash_length - diff;
                light_level = 2;
            }

            break;

    }

    params[SECTOR_PARAM_LIGHT_IDX] = light_level;
    params[SECTOR_PARAM_TICKS_LEFT_IDX] = ticks_left;
}

void run_sector_processes() {
    for(int sect_idx = 0; sect_idx < cur_portal_map->num_sectors; sect_idx++) {

        u8 typ = cur_portal_map->sector_types[sect_idx];
        s16* params = &live_sector_parameters[sect_idx<<NUM_SECTOR_PARAMS_SHIFT];

        switch(typ) {
            case NO_TYPE:
                continue;

            case DOOR: do {
                run_door(sect_idx, params);
            } while(0);
            break;

            case FLASHING: do {
                run_flash(sect_idx, params);  
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