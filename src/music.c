#include <genesis.h>
#include "music_res.h"

int music_on = TRUE;

const u8* songs[2] = { 
	//xgm_e2m2, xgm_e1m4
	NULL, NULL,
	 };
const char const* song_names[2] = {
	"E2M2",
	"E1M4"
};	

void toggle_music(int menu_idx) {
	music_on = !music_on;
}