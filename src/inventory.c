#include <genesis.h>
#include "hud.h"
#include "inventory.h"

#define MAX_ITEMS 8 


int num_items;

item_type *inventory;

void draw_inventory() {
    //for(int i = 0)
    for(int i = 0; i < num_items; i++) {

    }
}



void reset_inventory() {
    //num_items = 0;
    num_items = 5;
    inventory[0] = BLUE_KEY;
    inventory[1] = HEALTH_RECHARGE;
    inventory[2] = ARMOR_CHARGE;
    inventory[3] = SHIELD_CHARGE;
    inventory[4] = BULLET_CHARGE;
}

void load_inventory_items_to_vram(u32 tile_loc) {
    VDP_load_
    extern const Image hud;
    extern const Image armor_inv;
    extern const Image blue_key_inv;
    extern const Image bullet_inv;
    extern const Image heart_inv;
    extern const Image shield_inv;
    
    u16 hud_base_tile = TILE_ATTR_FULL(PAL2, 1, 0, 0, tile_loc);
    VDP_drawImageEx(BG_B, &hud, hud_base_tile, 0, 20, 0, 1);
    PAL_setPalette(PAL2, hud.palette->data);
    tile_loc += hud.tileset->numTile;

}

u32 init_inventory(u32 free_tile_loc) {
    inventory = MEM_alloc(sizeof(item_type) * MAX_ITEMS);
    u32 new_free_tile_loc = load_inventory_items_to_vram(free_tile_loc);


    reset_inventory();
    return new_free_tile_loc;

}

void free_inventory() {
    MEM_free(inventory);
}