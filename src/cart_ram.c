#include <genesis.h>
#include "cart_ram.h"

uint16_t ram_get(uint32_t idx) {
    uint16_t* base = (uint16_t*)RAM_START;
    return *(base+idx);
}

void ram_set(uint32_t idx, uint16_t val) {
    uint16_t* base = (uint16_t*)RAM_START;
    *(base+idx) = val;
}


void unlock_ram() {
    __asm volatile("move.b  #1,(0xA130F1).l");
}

void lock_ram() {
    __asm volatile("move.b  #0,(0xA130F1).l");
}

