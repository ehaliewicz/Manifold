#include <genesis.h>

#include "utils.h"

#define SUB_16_16(a, b) do {    \
    __asm volatile(             \
        "sub.w %1, %0"          \
        : "+d" (a)              \
        : "d" (b)               \
    );                          \
} while(0);


 u16 sub_16_16(u16 a, u16 b) {
    __asm volatile(             
        "sub.w %1, %0"          
        : "+d" (a)              
        : "d" (b)               
    );                          
    return a;
}

u32 fastLength(s32 dx, s32 dy) {
    u32 adx = abs(dx);
    u32 ady = abs(dy);
    return (adx > ady) ? (adx + (ady >> 1)) : (ady + (adx >> 1));
}


void die(char* msg) {
    while(1) {
        VDP_drawTextBG(BG_B, msg, 2, 12);
        VDP_waitVInt();
    }
}

static char buf[80];
void* malloc(u16 size, const char* thing) {
    sprintf(buf, "allocating %i bytes for %s", size, thing);
    KLog(buf);
    void* p = MEM_alloc(size);
    KLog_U1("allocated pointer: ", (u32)p);
    return p;
}

void free(void* thing, const char* thing2) {
    sprintf(buf, "freeing %s", thing2);
    KLog(buf);
    MEM_free(thing);
}