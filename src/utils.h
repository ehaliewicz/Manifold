#ifndef UTILS_H
#define UTILS_H
#include <genesis.h>

#define SCREEN_WIDTH BMP_PITCH
#define SCREEN_HEIGHT 144 // BMP_HEIGHT   // H

//s16 divs_32_by_16(s32 num, s16 denom);

//u16 divu_32_by_16(u32 num, u16 denom);
//u32 mulu_16_by_16(u16 a, u16 b);

//s32 muls_16_by_16(u16 a, u16 b);
u16 sub_16_16(u16 a, u16 b);


#define FETCH_INC_BYTE(var, ptr) do {\
    __asm volatile(                 \
        "move.b (%1)+, %0"          \
        : "=d" (var), "+a" (ptr)    \
    );                              \
} while(0);

#define FETCH_INC_WORD(var, ptr) do {\
    __asm volatile(                 \
        "move.w (%1)+, %0"          \
        : "=d" (var), "+a" (ptr)    \
    );                              \
} while(0);

#define WRITE_WORD_INC(var, ptr) do {\
    __asm volatile(                 \
        "move.w %1, (%0)+"          \
        : "+a" (ptr)                \
        : "d" (var)                 \
    );                              \
} while(0);

#define WRITE_BYTE_INC(var, ptr) do {\
    __asm volatile(                 \
        "move.b %1, (%0)+"          \
        : "+a" (ptr)                \
        : "d" (var)                 \
    );                              \
} while(0);

inline u16 divu_32_by_16(u32 num, u16 denom) {
     __asm volatile(
        "divu.w %1, %0"
        : "+d" (num) // output
        : "d" (denom)
    );

    s16 res = num;
    return res;
}


inline u32 mulu_16_by_16(u16 a, u16 b) {
    u32 a32 = a;
     __asm volatile(
        "mulu.w %1, %0"
        : "+d" (a32) // output
        : "d" (b)
    );
    return a32;
}

inline s16 divs_32_by_16(s32 num, s16 denom) {
     __asm volatile(
        "divs.w %1, %0"
        : "+d" (num) // output
        : "d" (denom)
    );

    s16 res = num;
    return res;
}


inline s32 muls_16_by_16(s16 a, s16 b) {
    s32 a32 = a;
     __asm volatile(
        "muls.w %1, %0"
        : "+d" (a32) // output
        : "d" (b)
    );
    return a32;
}


void die(char* s);

u32 fastLength(s32 dx, s32 dy);



inline u16 fastLength16(s16 dx, s16 dy) {
    u16 adx = abs(dx);
    u16 ady = abs(dy);
    return (adx > ady) ? (adx + (ady >> 1)) : (ady + (adx >> 1));
    /*

    __asm volatile(
        "tst.w %0\t\n\
         bpl.b to_there%=\t\n\
         neg.w %0\t\n\
         to_there%=:\n"
         : "+d" (dx) : : "cc"
    );
    __asm volatile(
        "tst.w %0\t\n\
         bpl.b to_there%=\t\n\
         neg.w %0\t\n\
         to_there%=:\n"
         : "+d" (dy) : : "cc"
    );
    return (dx > dy) ? (dx + (dy >> 1)) : (dy + (dx >> 1));
    */
}

void die(char* msg);

#define clamp(a, mi,ma)      min(max(a,mi),ma)

void* malloc(u16 size, const char* thing);
void free(void* thing, const char* thing2);

#endif