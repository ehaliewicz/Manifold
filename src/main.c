#include <genesis.h>

#include "game_mode.h"
#include "joy_helper.h"


int main() {
	VDP_init();

	set_game_mode(INTRO);
	//set_game_mode(IN_GAME);
	while(1) {        

		update_joy();
		
		run_game_mode();
		
	}
	return (0);
}
