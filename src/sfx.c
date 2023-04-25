#include "sfx.h"

int sfx_on = FALSE; //TRUE;

#define SFX_CHANNELS 3

u8 pcm_id[SFX_CHANNELS] = {0,0,0};
u8 channels_in_use[SFX_CHANNELS] = {0,0,0}; // 0 means not in use, 1 means in use priority 0, 2 means 1, etc
u8 sfx_channels[SFX_CHANNELS] = { SOUND_PCM_CH2, SOUND_PCM_CH3, SOUND_PCM_CH4 };
void toggle_sfx(int menu_idx) {
    sfx_on = ! sfx_on;
}

void play_sfx(u8 id, u8 priority) {
    if(!sfx_on) { return; }
    for(int i = 0; i < SFX_CHANNELS; i++) {
        u8 ch_used = channels_in_use[i];
        s8 pcm_equal = pcm_id[i] - id;
        s8 prio_diff = priority - ch_used; 
        
        if((ch_used == 0) || ((prio_diff >= 0) && (pcm_equal != 0))) {
        //if(priority >= channels_in_use[i] && pcm_id[i] != id) { // sadly, we need to check this to make sure a sfx gets played if possible
            XGM_startPlayPCM(id, priority, sfx_channels[i]);
            channels_in_use[i] = priority+1;
            pcm_id[i] = id;
            return;
        }
    }
    // dropped sfx sadly
    KLog_U1("dropped sfx :( %i", id);
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