#include <genesis.h>
#include "hud.h"
#include "inventory.h"

#define MAX_ITEMS 8 


static int num_items;
static int dirty;

static u16 hud_tile_loc;

static u16 *inventory;

static u16 *item_vram_addresses; //[NUM_ITEM_TYPES]; 


int inventory_full() {
    return (num_items == MAX_ITEMS);
}

int inventory_add_item(item_type item) {
    if(inventory_full()) {
        return 0;
    }
    inventory[num_items++] = item;
    dirty = 1;
    return 1;
}

void inventory_draw() {
    if(!dirty) {
        return;
    }



    int ix,iy;
    ix = 0;
    iy = 0;
    for(int i = 0; i < num_items; i++) {
        u16 tile_addr = item_vram_addresses[inventory[i]];


        VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(2, 1, 0, 0, tile_addr),
                         ix*2+2, iy+22);
        ix++;
        if(ix >= 5) {
            iy++;
            ix = 0;
        }
    }
    dirty = 0;
}



void inventory_reset() {
    //num_items = 0;
    num_items = 5;
    inventory[0] = HEALTH_RECHARGE;
    inventory[1] = ARMOR_CHARGE;
    inventory[2] = ARMOR_CHARGE;
    inventory[3] = SHIELD_CHARGE;
    inventory[4] = BULLET_CHARGE;
    dirty = 1;
}

u32 load_item(const Image* img, item_type type, u32 tile_loc) {
    VDP_loadTileSet(img->tileset, tile_loc, CPU);
    item_vram_addresses[type] = tile_loc;
    tile_loc += img->tileset->numTile;
    return tile_loc;
}

u32 load_inventory_items_to_vram(u32 tile_loc) {    
    hud_tile_loc = tile_loc;
    VDP_loadTileSet(hud.tileset, hud_tile_loc, CPU);

    PAL_setPalette(PAL2, hud.palette->data);
    tile_loc += hud.tileset->numTile;

    tile_loc = load_item(&armor_inv, ARMOR_CHARGE, tile_loc);
    tile_loc = load_item(&blue_key_inv, BLUE_KEY, tile_loc);
    tile_loc = load_item(&bullet_inv, BULLET_CHARGE, tile_loc);
    tile_loc = load_item(&heart_inv, HEALTH_RECHARGE, tile_loc);
    tile_loc = load_item(&shield_inv, SHIELD_CHARGE, tile_loc);


    return tile_loc;

}

u32 inventory_init(u32 free_tile_loc) {
    inventory = MEM_alloc(sizeof(item_type) * MAX_ITEMS);
    item_vram_addresses = MEM_alloc(sizeof(u16) * NUM_ITEM_TYPES);
    u32 new_free_tile_loc = load_inventory_items_to_vram(free_tile_loc);

    for(int y = 0; y < hud.tilemap->h; y++) {
        u16 y_off = y * hud.tilemap->w;
        for(int x = 0; x < hud.tilemap->w; x++) {
            u16 tile_attr = hud.tilemap->tilemap[y_off+x];
            tile_attr += hud_tile_loc;
            tile_attr |= (1<<TILE_ATTR_PRIORITY_SFT);
            tile_attr |= (PAL2<<TILE_ATTR_PALETTE_SFT);

            VDP_setTileMapXY(BG_B, tile_attr, x, y+20);
        }
    }

    inventory_reset();
    return new_free_tile_loc;

}

void inventory_cleanup() {
    MEM_free(inventory);
    MEM_free(item_vram_addresses);
}