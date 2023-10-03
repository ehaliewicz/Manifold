#ifndef WEAPON_SPRITES

#include <genesis.h>

#define WEAPON_SPRITES 

typedef enum {
    SHOTGUN_IDLE_ANIM=0,
    SHOTGUN_RELOAD_ANIM=1,
} weapon_sprite_anim;

u32 init_weapon_sprites(u32 tile_loc);

void set_weapon_anim(weapon_sprite_anim anim_idx, TransferMethod transfer_type);
void set_weapon_sprite_position_offset(s16 offX, s16 offY);
void step_weapon_anim(TransferMethod transfer_type);

#endif 