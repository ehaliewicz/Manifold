#include <genesis.h>

#include "game_mode.h"
#include "joy_helper.h"


int main() {

	set_game_mode(INTRO);

	while(1) {        

		update_joy();
		
		run_game_mode();
		
	}
	return (0);
}
