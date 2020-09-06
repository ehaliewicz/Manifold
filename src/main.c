#include <genesis.h>

#include "game_mode.h"
#include "joy_helper.h"


int main() {

	set_game_mode(INTRO);

	while(1) {        

		run_game_mode();
		
		update_joy();
	}
	return (0);
}
