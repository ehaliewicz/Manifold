#include <genesis.h>

void die(char* msg) {
    while(1) {
        VDP_drawTextBG(BG_B, msg, 2, 12);
        VDP_waitVInt();
    }
}