#include <genesis.h>

s16 divs_32_by_16(s32 num, s16 denom) {
     __asm volatile(
        "divs.w %1, %0"
        : "+d" (num) // output
        : "d" (denom)
    );

    s16 res = num;
    return res;
}

u16 divu_32_by_16(u32 num, u16 denom) {
     __asm volatile(
        "divu.w %1, %0"
        : "+d" (num) // output
        : "d" (denom)
    );

    s16 res = num;
    return res;
}


u32 mulu_16_by_16(u16 a, u16 b) {
     __asm volatile(
        "mulu.w %1, %0"
        : "+d" (a) // output
        : "d" (b)
    );
    return a;
}

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