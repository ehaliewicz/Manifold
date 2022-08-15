#ifndef TEXTURES_H
#define TEXTURES_H

#include <genesis.h>
#include "texture.h"

//extern const lit_texture wall_texture;
//extern const lit_texture sci_fi_wall_texture;
extern const lit_texture door_mid;

#define NUM_TEXTURES 5

extern lit_texture* const textures[8*5];

const u16 raw_key_mid[2048];
const u16 raw_key_32_32_mid[2048];

lit_texture* get_texture(u8 tex_idx, s8 light_level);

#endif