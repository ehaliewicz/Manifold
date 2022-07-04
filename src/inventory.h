#ifndef INVENTORY_H
#define INVENTORY_H

#include <genesis.h>

typedef enum {
      RED_KEY,
    GREEN_KEY,
     BLUE_KEY,
     SPEED_UP,
     POWER_UP,
     HEALTH_RECHARGE,
     SHIELD_CHARGE,
     BULLET_CHARGE,
     ARMOR_CHARGE,
} item_type;

#define NUM_ITEM_TYPES 9

void inventory_draw();
void inventory_reset();
int inventory_add_item(item_type item);
int inventory_full();
int inventory_has_item(item_type item);
void inventory_use_item(item_type item);


u32 inventory_init(u32 free_tile_loc);
void inventory_cleanup();

#endif