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
