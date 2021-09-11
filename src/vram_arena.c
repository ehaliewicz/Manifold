#include <genesis.h>

typedef struct {
    const uint16_t vram_addr;
    uint16_t cur_addr;
} arena;

arena new_arena(uint16_t base_addr) {
    return (arena){.vram_addr = base_addr};
}

uint16_t alloc_chunk(arena* a, uint16_t bytes) {
    uint16_t res = a->cur_addr;
    a->cur_addr += bytes;
    return res;
}

void reset_arena(arena* a) {
    a->cur_addr = a->vram_addr;
}