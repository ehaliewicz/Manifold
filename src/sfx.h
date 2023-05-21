#ifndef SFX_H
#define SFX_H

#include <genesis.h>

extern int sfx_on;
void toggle_sfx();
void update_sfx();
void play_sfx(u8 id, u8 priority);
void propagate_sfx_from_sect_group(u8 id, u8 priority, u8 src_sector_group, u8* rendered_sectors);

#define SFX_OPEN_DOOR_ID 1
#define SFX_CLOSE_DOOR_ID 2
#define SFX_LIFT_GO_UP_ID 3
#define SFX_JUMP1_ID 4
#define SFX_JUMP2_ID 5
#define SFX_SHOTGUN_ID 6
#define SFX_SELECT_ID 7


#endif