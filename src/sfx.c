#include "level.h"
#include "sfx.h"

int sfx_on = TRUE;

#define NUM_SFX_CHANNELS 3

u8 pcm_id[NUM_SFX_CHANNELS] = {0,0,0};
u8 channels_in_use[NUM_SFX_CHANNELS] = {0,0,0}; // 0 means not in use, 1 means in use priority 0, 2 means 1, etc
u8 sfx_channels[NUM_SFX_CHANNELS] = { SOUND_PCM_CH2, SOUND_PCM_CH3, SOUND_PCM_CH4 };
void toggle_sfx(int menu_idx) {
    sfx_on = ! sfx_on;
}

void play_sfx(u8 id, u8 priority) {
    if(!sfx_on) { return; }
    //KLog_U2("playing sfx: ", id, " with priority: ", priority);

    // check for a free channel first
    for(int i = 0; i < NUM_SFX_CHANNELS; i++) {
        u8 ch_used = channels_in_use[i];
        s8 old_priority = ch_used-1;
        s8 pcm_equal = pcm_id[i] - id;
        s8 prio_diff = priority - (ch_used-1);  // if 0, that means it's actually higher priority
        
        if(ch_used == 0) {
        //if(priority >= channels_in_use[i] && pcm_id[i] != id) { // sadly, we need to check this to make sure a sfx gets played if possible
            //KLog_U1("playing on unused channel: ", sfx_channels[i]);
            XGM_startPlayPCM(id, priority, sfx_channels[i]);
            //KLog_S2("playing id: ", id, " on channel: ", i+1);
            channels_in_use[i] = priority+1;
            //KLog_S3("ch used: ", ch_used, " prev priority: ", old_priority, " cur priority: ", priority);
            pcm_id[i] = id;
            return;
        }
    }
    // if no free channels, check for a lower priority channel
    for(int i = 0; i < NUM_SFX_CHANNELS; i++) {
        u8 ch_used = channels_in_use[i];
        s8 old_priority = ch_used-1;
        s8 pcm_equal = pcm_id[i] - id;
        s8 prio_diff = priority - (ch_used-1);  // if 0, that means it's actually higher priority
        
        if((priority > old_priority) && (pcm_equal != 0)) {
        //if(priority >= channels_in_use[i] && pcm_id[i] != id) { // sadly, we need to check this to make sure a sfx gets played if possible
            //KLog_U1("playing on lower priority channel: ", sfx_channels[i]);
            XGM_startPlayPCM(id, priority, sfx_channels[i]);
            //KLog_S2("playing id: ", id, " on channel: ", i+1);
            channels_in_use[i] = priority+1;
            //KLog_S3("ch used: ", ch_used, " prev priority: ", old_priority, " cur priority: ", priority);
            pcm_id[i] = id;
            return;
        }
    }
    // dropped sfx sadly
    KLog_U1("dropped sfx :( %i", id);
}

u8 sound_hearable(u16 sound_src_sector_group, u16 dst_sector) {
    portal_map* map = (portal_map*)cur_portal_map;


    // if src_sector is reachable from the set of rendered sectors via up to one max sector between, play it
    for(int sect = 0; sect < cur_portal_map->num_sectors; sect++) {
        if(sector_group(sect, map) != sound_src_sector_group) {
            continue;
        }
        if(!sector_in_phs(dst_sector, sect, map)) {
            continue;
        }
        return 1;
    }
    return 0;
}

void propagate_sfx_from_sect_group(u8 id, u8 priority, u16 src_sector_group, u16 dst_sector) {
    if(sound_hearable(src_sector_group, dst_sector)) {
        play_sfx(id, priority);
    }
}

void update_sfx() {
    u8 playing_status = (XGM_isPlayingPCM(SOUND_PCM_CH2_MSK | SOUND_PCM_CH3_MSK | SOUND_PCM_CH4_MSK))>>1;
    //KLog_U3("bitmap ", playing_status&1, "", playing_status&2, "", playing_status&4);
    switch(playing_status)  {
        case 0b000:
            channels_in_use[0] = 0;
            channels_in_use[1] = 0;
            channels_in_use[2] = 0;
            break;
        case 0b001:
            channels_in_use[1] = 0;
            channels_in_use[2] = 0;
            break;
        case 0b010:
            channels_in_use[0] = 0;
            channels_in_use[2] = 0;
            break;
        case 0b011:
            channels_in_use[2] = 0;
            break;
        case 0b100:
            channels_in_use[0] = 0;
            channels_in_use[1] = 0;
            break;
        case 0b101:
            channels_in_use[1] = 0;
            break;
        case 0b110:
            channels_in_use[0] = 0;
            break;
        case 0b111:
            break;
    }
}