#include <genesis.h>

#include "game_mode.h"
#include "joy_helper.h"

#include "my_bmp.h"

#include "init.h"


int main() {
	VDP_init();
	MEM_pack();
	KLog_U1("allocated mem at start: ", MEM_getAllocated());

	KLog_U1("free bytes of ram at startup: ", MEM_getFree());
	volatile u32* vp_start_in_game_arr = start_in_game_arr;
	u16 startup_joy = JOY_readJoypad(JOY_1);
	u8 bypass_autoload = (startup_joy & BUTTON_START);

	if(vp_start_in_game_arr[1] && !bypass_autoload) {
		// i don't know why this is required, I guess some VRAM remapping or something
		// without it, fps display doesn't work in-game
		// such a dumb hack

		// basically, it works if you run the INTRO fire mode, which calls these functions
		//bmp_init_horizontal(0, BG_A, PAL1, 0);
	    //bmp_end();
		//MEM_pack();
    	DMA_setBufferSize(2048);
		set_game_mode(IN_GAME);
	} else {
		set_game_mode(INTRO);
	}

	while(1) {        

		update_joy();
		
		run_game_mode();
		
	}
	return (0);
}
