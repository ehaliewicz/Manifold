#include <genesis.h>
#include "console.h"
#include "game.h"
#include "level.h"
#include "portal_map.h"
#include "portal.h"
#include "random.h"
#include "sector_group.h"
#include "sfx.h"


s16* live_sector_group_parameters;


s8 get_sector_group_light_level_inner(s16* params) {
    s16* field_ptr = &params[SECTOR_GROUP_PARAM_LIGHT_FLOOR_CEIL_COLOR_IDX];
    light_and_floor_ceil_color* cast_ptr = (light_and_floor_ceil_color*)field_ptr;
    return cast_ptr->light_level;
}

s8 get_sector_group_light_level(u16 sect_group) {
    return get_sector_group_light_level_inner(&live_sector_group_parameters[(sect_group<<NUM_SECTOR_GROUP_PARAMS_SHIFT)]);
}

void set_sector_group_light_level_inner(s16* params, s8 light_level) {
    s16* field_ptr = &params[SECTOR_GROUP_PARAM_LIGHT_FLOOR_CEIL_COLOR_IDX];
    light_and_floor_ceil_color* cast_ptr = (light_and_floor_ceil_color*)field_ptr;
    cast_ptr->light_level = light_level;
}

void set_sector_group_light_level(u16 sect_group, s8 light_level) {
    set_sector_group_light_level_inner(
        &live_sector_group_parameters[(sect_group<<NUM_SECTOR_GROUP_PARAMS_SHIFT)], light_level);
}

u16 get_sector_group_floor_color_inner(s16* params) {
    s16* field_ptr = &params[SECTOR_GROUP_PARAM_LIGHT_FLOOR_CEIL_COLOR_IDX];
    light_and_floor_ceil_color* cast_ptr = (light_and_floor_ceil_color*)field_ptr;
    return (cast_ptr->floor_ceil_color)&0xF;
}

u16 get_sector_group_floor_color(u16 sect_group) {
    return get_sector_group_floor_color_inner(&live_sector_group_parameters[sect_group<<NUM_SECTOR_GROUP_PARAMS_SHIFT]);
}

void set_sector_group_floor_color_inner(s16* params, u8 floor_color) {
    s16* field_ptr = &params[SECTOR_GROUP_PARAM_LIGHT_FLOOR_CEIL_COLOR_IDX];
    light_and_floor_ceil_color* cast_ptr = (light_and_floor_ceil_color*)field_ptr;
    u8 colors = cast_ptr->floor_ceil_color;
    colors &= 0xF0; // clear floor color
    colors |= floor_color; // set floor color
    cast_ptr->floor_ceil_color = colors;
}

u16 get_sector_group_ceil_color_inner(s16* params) {
    s16* field_ptr = &params[SECTOR_GROUP_PARAM_LIGHT_FLOOR_CEIL_COLOR_IDX];
    light_and_floor_ceil_color* cast_ptr = (light_and_floor_ceil_color*)field_ptr;
    return (cast_ptr->floor_ceil_color)>>4;
}

u16 get_sector_group_ceil_color(u16 sect_group) {
    return get_sector_group_ceil_color_inner(&live_sector_group_parameters[(sect_group<<NUM_SECTOR_GROUP_PARAMS_SHIFT)]);
}

void set_sector_group_ceil_color_inner(s16* params, u8 ceil_color) {
    s16* field_ptr = &params[SECTOR_GROUP_PARAM_LIGHT_FLOOR_CEIL_COLOR_IDX];
    light_and_floor_ceil_color* cast_ptr = (light_and_floor_ceil_color*)field_ptr;
    u8 colors = cast_ptr->floor_ceil_color;
    colors &= 0x0F; // clear ceil color
    colors |= (ceil_color<<4); // set ceil color
    cast_ptr->floor_ceil_color = colors;
}

s16* get_sector_group_pointer(u16 sect_group) {
    return &live_sector_group_parameters[(sect_group<<NUM_SECTOR_GROUP_PARAMS_SHIFT)];
}

s16 get_sector_group_orig_height(u16 sect_group) {
    return live_sector_group_parameters[(sect_group<<NUM_SECTOR_GROUP_PARAMS_SHIFT)+SECTOR_GROUP_PARAM_ORIG_HEIGHT_IDX];
}

u16 get_sector_group_ticks_left(u16 sect_group) {
    return live_sector_group_parameters[(sect_group<<NUM_SECTOR_GROUP_PARAMS_SHIFT)+SECTOR_GROUP_PARAM_TICKS_LEFT_IDX];
}

void set_sector_group_ticks_left(u16 sect_group, u16 ticks_left) {
    live_sector_group_parameters[(sect_group<<NUM_SECTOR_GROUP_PARAMS_SHIFT)+SECTOR_GROUP_PARAM_TICKS_LEFT_IDX] = ticks_left;
}

u16 get_sector_group_state(u16 sect_group) {
    return live_sector_group_parameters[(sect_group<<NUM_SECTOR_GROUP_PARAMS_SHIFT)+SECTOR_GROUP_PARAM_STATE_IDX];
}

void set_sector_group_state(u16 sect_group, u16 state) {
    live_sector_group_parameters[(sect_group<<NUM_SECTOR_GROUP_PARAMS_SHIFT)+SECTOR_GROUP_PARAM_STATE_IDX] = state;
}

s16 get_sector_group_floor_height(u16 sect_group) {
    return live_sector_group_parameters[(sect_group<<NUM_SECTOR_GROUP_PARAMS_SHIFT) + SECTOR_GROUP_PARAM_FLOOR_HEIGHT_IDX];
}

void set_sector_group_floor_height(u16 sect_group, s16 height) {
    live_sector_group_parameters[(sect_group<<NUM_SECTOR_GROUP_PARAMS_SHIFT) + SECTOR_GROUP_PARAM_FLOOR_HEIGHT_IDX] = height;
}

s16 get_sector_group_ceil_height(u16 sect_group) {
    return live_sector_group_parameters[(sect_group<<NUM_SECTOR_GROUP_PARAMS_SHIFT) + SECTOR_GROUP_PARAM_CEIL_HEIGHT_IDX];
}

void set_sector_group_ceil_height(u16 sect_group, s16 height) {
    live_sector_group_parameters[(sect_group<<NUM_SECTOR_GROUP_PARAMS_SHIFT) + SECTOR_GROUP_PARAM_CEIL_HEIGHT_IDX] = height;
}

void run_door(s16* params, u16 sect_group) {
    door_lift_state state = params[SECTOR_GROUP_PARAM_STATE_IDX];
    s16 cur_height = params[SECTOR_GROUP_PARAM_CEIL_HEIGHT_IDX];
    s16 floor_height = params[SECTOR_GROUP_PARAM_FLOOR_HEIGHT_IDX];
    s16 orig_door_height = params[SECTOR_GROUP_PARAM_ORIG_HEIGHT_IDX];

    switch(state) {
        case CLOSED: 
            break;

        case GOING_UP: do {
            if(cur_height == floor_height) {
                play_sfx(SFX_OPEN_DOOR_ID, 8);
            }
            cur_height += 180; //160;
            params[SECTOR_GROUP_PARAM_CEIL_HEIGHT_IDX] = cur_height;
            if(cur_height >= orig_door_height) {
                params[SECTOR_GROUP_PARAM_CEIL_HEIGHT_IDX] = orig_door_height;
                params[SECTOR_GROUP_PARAM_STATE_IDX] = OPEN;
                params[SECTOR_GROUP_PARAM_TICKS_LEFT_IDX] = 30;
            }
        } while(0);
        break;

        case OPEN: do {
            u16 ticks_left = params[SECTOR_GROUP_PARAM_TICKS_LEFT_IDX];

            if(ticks_left == 0) {
                //play_sfx(SFX_CLOSE_DOOR_ID, 1);
                propagate_sfx_from_sect_group(SFX_CLOSE_DOOR_ID, 8, sect_group, cur_player_pos.cur_sector);
                params[SECTOR_GROUP_PARAM_STATE_IDX] = GOING_DOWN;
            } else {
                params[SECTOR_GROUP_PARAM_TICKS_LEFT_IDX]--;
            }
        } while(0);
            break;

        case GOING_DOWN: do {
            cur_height -= 180; //160;
            params[SECTOR_GROUP_PARAM_CEIL_HEIGHT_IDX] = cur_height;
            if(cur_height <= floor_height) {
                params[SECTOR_GROUP_PARAM_CEIL_HEIGHT_IDX] = floor_height;
                params[SECTOR_GROUP_PARAM_STATE_IDX] = CLOSED;
                params[SECTOR_GROUP_PARAM_TICKS_LEFT_IDX] = 30;
            }
        } while(0);
        break;
    }
}


void run_lift(s16* params, u16 sect_group) {
    door_lift_state state = params[SECTOR_GROUP_PARAM_STATE_IDX];

    s16 cur_height = params[SECTOR_GROUP_PARAM_FLOOR_HEIGHT_IDX];
    s16 ceil_height = params[SECTOR_GROUP_PARAM_CEIL_HEIGHT_IDX];
    s16 orig_lift_height = params[SECTOR_GROUP_PARAM_ORIG_HEIGHT_IDX];
    s16 reset_height = params[SECTOR_GROUP_PARAM_SCRATCH_ONE_IDX];
    switch(state) {
        case CLOSED: 
            params[SECTOR_GROUP_PARAM_SCRATCH_ONE_IDX] = cur_height;
            break;

        case GOING_DOWN: do {
                if(cur_height == reset_height) {
                    propagate_sfx_from_sect_group(SFX_OPEN_DOOR_ID, 8, sect_group, cur_player_pos.cur_sector);
                }
                cur_height -= 128;
                params[SECTOR_GROUP_PARAM_FLOOR_HEIGHT_IDX] = cur_height;
                if(cur_height <= orig_lift_height) {
                        params[SECTOR_GROUP_PARAM_FLOOR_HEIGHT_IDX] = orig_lift_height;
                        params[SECTOR_GROUP_PARAM_STATE_IDX] = OPEN;
                        params[SECTOR_GROUP_PARAM_TICKS_LEFT_IDX] = 30;
                }
            } while(0);
            break;

        case OPEN: 
        do {
            u16 ticks_left = params[SECTOR_GROUP_PARAM_TICKS_LEFT_IDX];
            if(ticks_left == 0) {
                propagate_sfx_from_sect_group(SFX_LIFT_GO_UP_ID, 8, sect_group, cur_player_pos.cur_sector);
                params[SECTOR_GROUP_PARAM_STATE_IDX] = GOING_UP;
            } else {
                params[SECTOR_GROUP_PARAM_TICKS_LEFT_IDX]--;
            }
        } while(0);
            break;

        case GOING_UP: do {
                cur_height += 128;
                params[SECTOR_GROUP_PARAM_FLOOR_HEIGHT_IDX] = cur_height;
                if(cur_height >= reset_height) {
                    params[SECTOR_GROUP_PARAM_FLOOR_HEIGHT_IDX] = reset_height;
                    params[SECTOR_GROUP_PARAM_STATE_IDX] = CLOSED;
                    params[SECTOR_GROUP_PARAM_TICKS_LEFT_IDX] = 30;
                }
            } while(0);
            break;
    }
}


void run_stairs(s16* params, u16 sect_group) {
    stair_state state = params[SECTOR_GROUP_PARAM_STATE_IDX];

    s16 cur_height = params[SECTOR_GROUP_PARAM_FLOOR_HEIGHT_IDX];
    s16 orig_stairs_height = params[SECTOR_GROUP_PARAM_ORIG_HEIGHT_IDX];
    //s16 ticks = params[SECTOR_PARAM_TICKS_LEFT_IDX]++; // ticks no longer necessary, just increase until we're at the right height.
    switch(state) {
        case STAIRS_LOWERED: do {
        } while(0);
            break;

        case STAIRS_MOVING: do {
                cur_height += 32;
                //ticks++;
                //params[SECTOR_PARAM_TICKS_LEFT_IDX] = ticks;
                
                // TODO: floor color hack!
                set_sector_group_floor_color_inner(params, 12);
                
                params[SECTOR_GROUP_PARAM_FLOOR_HEIGHT_IDX] = cur_height;
                if(cur_height >= orig_stairs_height) {
                    params[SECTOR_GROUP_PARAM_FLOOR_HEIGHT_IDX] = orig_stairs_height;
                    params[SECTOR_GROUP_PARAM_STATE_IDX] = STAIRS_RAISED;
                }
            } while(0);
            break;
        

        case STAIRS_RAISED: do {
        } while(0);
        break;
    }
}

void run_lowering_stairs(s16* params, u16 sect_group) {
    stair_state state = params[SECTOR_GROUP_PARAM_STATE_IDX];

    s16 cur_height = params[SECTOR_GROUP_PARAM_FLOOR_HEIGHT_IDX];
    s16 orig_stairs_height = params[SECTOR_GROUP_PARAM_ORIG_HEIGHT_IDX];
    //s16 ticks = params[SECTOR_PARAM_TICKS_LEFT_IDX]++; // ticks no longer necessary, just increase until we're at the right height.
    switch(state) {
        case STAIRS_LOWERED: do {
        } while(0);
            break;

        case STAIRS_MOVING: do {
                cur_height -= 32;
                //ticks++;
                //params[SECTOR_PARAM_TICKS_LEFT_IDX] = ticks;
                
                // TODO: floor color hack!
                set_sector_group_floor_color_inner(params, 12);
                
                params[SECTOR_GROUP_PARAM_FLOOR_HEIGHT_IDX] = cur_height;
                if(cur_height <= orig_stairs_height) {
                    params[SECTOR_GROUP_PARAM_FLOOR_HEIGHT_IDX] = orig_stairs_height;
                    params[SECTOR_GROUP_PARAM_STATE_IDX] = STAIRS_LOWERED;
                }
            } while(0);
            break;
        

        case STAIRS_RAISED: do {
        } while(0);
        break;
    }
}



void run_flash(s16* params) {
    s8 light_level = get_sector_group_light_level_inner(params);

    s8 orig_light_level = params[SECTOR_GROUP_PARAM_STATE_IDX];
    s16 ticks_left = params[SECTOR_GROUP_PARAM_TICKS_LEFT_IDX];
    //KLog_S1("Light level: ", light_level);
    //KLog_S1("orig light level: ", orig_light_level);
    //KLog_S1("ticks left: ", ticks_left);
    ticks_left -= last_frame_ticks;
    s8 diff;

    if(ticks_left <= 0) {

        switch(light_level) {
            case 2:
                // figure out when to switch back
                diff = abs(ticks_left);
                u8 ticks_til_flash = fxrandom();
                ticks_left = ticks_til_flash - diff;
                light_level = orig_light_level;
                break;
            default:
                diff = abs(ticks_left);
                u8 flash_length = fxrandom();
                ticks_left = flash_length - diff;
                orig_light_level = light_level;
                light_level = 2;
                break;
        }
    }

    set_sector_group_light_level_inner(params, light_level);
    params[SECTOR_GROUP_PARAM_TICKS_LEFT_IDX] = ticks_left;
    params[SECTOR_GROUP_PARAM_STATE_IDX] = orig_light_level;
}

void run_sector_group_process(u16 sect_group) {
    //KLog_U1("running sector group process for sector group: ", sect_group);
    u8 typ = GET_SECTOR_GROUP_TYPE(cur_portal_map->sector_group_types[sect_group]);
    //KLog_U1("type: ", typ);
    s16* params = &live_sector_group_parameters[sect_group<<NUM_SECTOR_GROUP_PARAMS_SHIFT];

    switch(typ) {
        case NO_TYPE:
            break;

        case DOOR: do {
            run_door(params, sect_group);
        } while(0);
            break;

        case FLASHING: do {
            run_flash(params);  
        } while(0);
            break;

        case LIFT: do {
            run_lift(params, sect_group);
        } while(0);
            break;

        case STAIRS: do {
            run_stairs(params, sect_group);
        } while(0);
            break;

        case LOWERING_STAIRS: do {
            run_lowering_stairs(params, sect_group);
        } while(0);
            break;
            
    }
}

void run_sector_group_processes() {
    for(int sect_group = 0; sect_group < cur_portal_map->num_sector_groups; sect_group++) {
        run_sector_group_process(sect_group);
    }
}

#define SECTOR_TRIGGER_SIZE 8
#define NUM_SECTOR_TRIGGER_TARGETS 7
#define SECTOR_TRIGGER_SHIFT 3

u16 get_sector_group_trigger_type(u16 sector_group) {
    return cur_portal_map->sector_group_triggers[sector_group<<SECTOR_TRIGGER_SHIFT];
}
s16 get_sector_group_trigger_target(u16 sector_group, u8 tgt_idx) {
    return cur_portal_map->sector_group_triggers[(sector_group<<SECTOR_TRIGGER_SHIFT)+1+tgt_idx];
}

trigger_type activate_sector_group_enter_trigger(u16 sector_group) {
    s16 type = get_sector_group_trigger_type(sector_group);
    switch(type) {
        case NO_TRIGGER:
            break;
        case SET_SECTOR_DARK:
            //return;
            for(int i = 0; i < NUM_SECTOR_TRIGGER_TARGETS; i++) {
                s16 tgt_sector_group = get_sector_group_trigger_target(sector_group, i);
                if(tgt_sector_group == -1) { break; }
                set_sector_group_light_level(tgt_sector_group, -2);
                
            }
            break;
        case SET_SECTOR_LIGHT:
            for(int i = 0; i < NUM_SECTOR_TRIGGER_TARGETS; i++) {
                s16 tgt_sector_group = get_sector_group_trigger_target(sector_group, i);
                if(tgt_sector_group == -1) { break; }
                set_sector_group_light_level(tgt_sector_group, 1);
                
            }
            break;
        case START_STAIRS:
            for(int i = 0; i < NUM_SECTOR_TRIGGER_TARGETS; i++) {
                s16 tgt_sector_group = get_sector_group_trigger_target(sector_group, i);
                if(tgt_sector_group == -1) { break; }
                set_sector_group_state(tgt_sector_group, STAIRS_MOVING);
            }
            break;

        case LEVEL_END: do {
        } while(0);
            break;
    }

    return type;
    
    //char buf[50];
    //int len = sprintf(buf, "trap for sector #%i triggered", sector_group);
    //buf[len] = 0;
    //console_push_message(buf, len, 30);
}
