#include <genesis.h>

#include "game_mode.h"
#include "joy_helper.h"

const u32 start_in_game_arr[2] = {
	0xFEEDBEEF,
	0
};

int main() {
	VDP_init();
	volatile u32* vp_start_in_game_arr = start_in_game_arr;
	if(vp_start_in_game_arr[1]) {
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
