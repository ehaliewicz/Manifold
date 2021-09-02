#ifndef CART_RAM

#include <genesis.h>

#define RAM_START    0x00200002
#define RAM_END      0x00207D00
#define RAM_LOCK     0x00A130F1

uint16_t ram_get(uint32_t idx);
void ram_set(uint32_t idx, uint16_t val);
void unlock_ram();
void lock_ram();

#endif
