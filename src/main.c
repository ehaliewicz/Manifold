#include <genesis.h>

#include "game_mode.h"
#include "joy_helper.h"


int main() {


	char buf[40];

	//set_game_mode(INTRO);
	set_game_mode(MAIN_MENU);

	while(1) {        

		run_game_mode();
		
		update_joy();
	}
	return (0);
}
