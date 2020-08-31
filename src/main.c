#include <genesis.h>

#include "music.h"
#include "graphics.h"
#include "fire.h"
#include "sfx.h"

const u8* songs[2] = { xgm_e2m2, xgm_e1m4 };
char* song_names[2] = {
	"E2M2",
	"E1M4"
};	

int cur_song = 1;

u16 normal_tempo;
u16 fast_tempo;
u16 cur_tempo;
u16 slow_tempo;

void next_song() {
	if(XGM_isPlaying()) {
		XGM_stopPlay();
	}
	if(cur_song == 0) { cur_song = 1; }
	else { cur_song = 0; }
	XGM_startPlay(songs[cur_song]);
	normal_tempo = XGM_getMusicTempo();
	fast_tempo = normal_tempo + (normal_tempo>>2);
	cur_tempo = normal_tempo;
	slow_tempo = normal_tempo>>1;
}

#define SPEED_UP BUTTON_A
#define SLOW_DOWN BUTTON_B
#define NEXT_SONG BUTTON_C

u16 last_joy;

int is_button_held(u16 button) {
	return (JOY_readJoypad(JOY_1) & button);
}

int is_button_pressed(u16 button) {
	return (JOY_readJoypad(JOY_1) & button) && (!(button & last_joy));
}


void reset_scroll() {
	for(int i = 0; i < 240; i++) {
		s16 scroll = 0;
		VDP_setHorizontalScrollLine(BG_B, i, &scroll, 1, CPU);
		VDP_setHorizontalScrollLine(BG_A, i, &scroll, 1, CPU);
	}
}



int main() {

	VDP_setBackgroundColor(3);

	next_song();
	char buf[40];
	int cur_scroll_fixed = -220<<2;
	int dscroll = 0b100;

	VDP_setScrollingMode(HSCROLL_LINE, VSCROLL_PLANE);
	
	VDP_setVerticalScroll(BG_B, cur_scroll_fixed>>2);
	
	VDP_setVerticalScroll(BG_A, 0);

	

	BMP_init(0, BG_A, PAL1, 0);
	VDP_drawImageEx(BG_B, &doom_logo, 0x0300, 8, 0, 1, 1);


	init_fire();

	start_fire_source();
	BMP_setBufferCopy(0);

	reset_scroll();

	int run_fire = 1;
	int frame = 0;
	//XGM_setPCM(0, sfx_select, sizeof(sfx_select));

	while(1) {        

		// ensure previous flip buffer request has been started
		//BMP_showFPS(1);
		spread_and_draw_fire_byte();
		BMP_waitWhileFlipRequestPending();
		BMP_flipPartial(1, 12);
		copy_fire_buffer_portion();
			
		
		frame++;

		cur_scroll_fixed += dscroll;
		int cur_scroll = cur_scroll_fixed>>2;
		
		if(cur_scroll < -45) {
			VDP_setVerticalScroll(BG_B, cur_scroll);
		} else if (run_fire) {
			clear_fire_source();
			run_fire = 0;
		} else if (!run_fire) {
			
			clear_fire_source();
		}

		if(is_button_pressed(NEXT_SONG)) {
			//next_song();
		} 
		
		if(is_button_pressed(BUTTON_START)) {
			clear_fire_source();
		}

		if(is_button_pressed(BUTTON_UP)) {
			//select
			//XGM_startPlayPCM(1, 0, 0);
		}

		if (is_button_held(SPEED_UP)) {
			if(XGM_getMusicTempo() != fast_tempo) {
				//XGM_setMusicTempo(fast_tempo);
			}
		} else if (is_button_held(SLOW_DOWN)) {
			if(XGM_getMusicTempo() != slow_tempo) {
				//XGM_setMusicTempo(slow_tempo);
			}
		} else if (XGM_getMusicTempo() != normal_tempo) {
			//XGM_setMusicTempo(normal_tempo);
		}

		last_joy = JOY_readJoypad(JOY_1);
		 
	}
	return (0);
}
