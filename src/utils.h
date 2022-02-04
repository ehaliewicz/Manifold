#ifndef UTILS_H
#define UTILS_H
#include <genesis.h>

#define SCREEN_WIDTH BMP_PITCH
#define SCREEN_HEIGHT 144 // BMP_HEIGHT   // H

s16 divs_32_by_16(s32 num, s16 denom);
u16 divu_32_by_16(u32 num, u16 denom);

void die(char* s);

#endif