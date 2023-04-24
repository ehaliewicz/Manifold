#ifndef SFX_H
#define SFX_H

#include <genesis.h>

extern int sfx_on;
void toggle_sfx();
void update_sfx();
void play_sfx(u8 id, u8 priority);

#endif