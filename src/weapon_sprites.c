#include <genesis.h>

#include "weapon_sprites.h"
#include "sprites_res.h"
#include "utils.h"

#define MAX_SPRITE_TILES 97
#define MAX_SPRITES 16

static u32 start_tile_loc;
static u16 cur_num_sprites;
static u16 sprite_idx_start;

// frame index 
// time til next frame

typedef enum {
 SHOTGUN_SPR=0,
 SHOTGUN_RELOAD_SPR=1,
} weapon_sprite;

typedef struct {
    u8 spr;
    s16 time;
    u16 next_anim;
    u8 anim_idx; u8 frame_idx;
} animation_entry;




animation_entry anim_table[] = {
    {
        .spr = SHOTGUN_SPR,
        .time = -1,
        .next_anim = 0,
    },
    {
        .spr = SHOTGUN_RELOAD_SPR,
        .time = 3, // 2 frames
        .next_anim = 2,
        .anim_idx = 0, .frame_idx = 0,
    },
    {
        .spr = SHOTGUN_RELOAD_SPR,
        .time = 3, // 2 frames
        .next_anim = SHOTGUN_IDLE_ANIM, // back to idle
        .anim_idx = 0, .frame_idx = 1,
    }
};

static s16 cur_frame_timer;
static u16 cur_anim_idx;
SpriteDefinition *cur_sprite_def;

typedef enum {
    DONE=0,
    TILES_LOADING = 1,
    SPRITES_LOADING = 2,
} update_state;

u8 transfer_pending = 0;
u8 pre_queue_size = 0;


void update_weapon_sprites(TransferMethod transfer_type);

void step_weapon_animation(TransferMethod transfer_type) {
    if(cur_frame_timer == -1) {
        if(transfer_pending != DONE) {
            if(DMA_getQueueSize() == pre_queue_size) {
                transfer_pending = DONE;
                update_weapon_sprites(transfer_type);
            }
        }
        return;
    }
    if(cur_frame_timer-- <= 0 && transfer_pending == DONE) {
        // move to next frame
        //set_weapon_anim
        u16 next_anim = anim_table[cur_anim_idx].next_anim;
        KLog_U1("going to anim index: ", next_anim);

        pre_queue_size = DMA_getQueueSize();
        set_weapon_anim(next_anim, transfer_type);
        transfer_pending = TILES_LOADING;
    }
    if(transfer_pending != DONE) {
        if(DMA_getQueueSize() == pre_queue_size) {
            transfer_pending = DONE;
            update_weapon_sprites(transfer_type);
        }
    }

}

u32 init_weapon_sprites(u32 tile_loc) {
    VDP_resetSprites();
    start_tile_loc = tile_loc;
    // set load tile location?
    sprite_idx_start = VDP_allocateSprites(MAX_SPRITES);
    KLog_U1_("Allocating ", MAX_SPRITES, " sprites for weapons");
    KLog_U1_("Allocating ", (MAX_SPRITE_TILES*2), " tiles for weapons");
    return tile_loc+(MAX_SPRITE_TILES*2);

}


#define BASE_WEAPON_X 128
#define BASE_WEAPON_Y 120

u32 get_next_tile_loc_and_flip() {
    static u8 flag = 0;

    if(flag == 0) {
        flag = 1;
        return start_tile_loc;
    } else {
        flag = 0;
        return start_tile_loc + MAX_SPRITE_TILES;
    }
}

u32 cur_upload_idx;
void set_weapon_anim(weapon_sprite_anim anim_idx, TransferMethod transfer_type) {
    cur_anim_idx = anim_idx;
    animation_entry entry = anim_table[cur_anim_idx];
    cur_frame_timer = entry.time;
    cur_upload_idx = get_next_tile_loc_and_flip();

    switch(entry.spr) {
        case SHOTGUN_SPR:
            cur_sprite_def = &shotgun;
            break;
        case SHOTGUN_RELOAD_SPR:
            cur_sprite_def = &shotgun_reload;
            break;
        default:
            die("Unknown sprite");
    }
    u8 animation_idx = entry.anim_idx;
    u8 frame_idx = entry.frame_idx;


    //ASSERT(num_tiles <= MAX_SPRITE_TILES)
    KLog_U1("sprites: ", cur_sprite_def->maxNumSprite);
    KLog_U1("sprite tiles: ", cur_sprite_def->maxNumTile);
    assert(cur_sprite_def->maxNumTile <= MAX_SPRITE_TILES, "Weapon has too many sprite tiles");
    assert(cur_sprite_def->maxNumSprite <= MAX_SPRITES, "Weapon has too many sprites");

    AnimationFrame* frame = cur_sprite_def->animations[animation_idx]->frames[frame_idx];
    VDP_loadTileSet(frame->tileset, cur_upload_idx, transfer_type);
}

    
void update_weapon_sprites(TransferMethod transfer_type) {
    animation_entry entry = anim_table[cur_anim_idx];
    u8 animation_idx = entry.anim_idx;
    u8 frame_idx = entry.frame_idx;
    AnimationFrame* frame = cur_sprite_def->animations[animation_idx]->frames[frame_idx];
    //VDP_resetSprites(); // should i always reset?  What if something else uses sprites?
    cur_num_sprites = frame->numSprite;

    u16 tile_idx = cur_upload_idx;

    VDP_clearSprites();
    FrameVDPSprite** f = frame->frameInfos[0].frameVDPSprites;
    for(int i = 0; i < cur_num_sprites; i++) {
        FrameVDPSprite* spr = f[i];

        VDP_setSprite(
            sprite_idx_start+i,
            BASE_WEAPON_X+spr->offsetX, BASE_WEAPON_Y+spr->offsetY,
            spr->size,
            TILE_ATTR_FULL(PAL0, 0, 0, 0, tile_idx)
        );
        tile_idx += spr->numTile;

    }
    VDP_linkSprites(sprite_idx_start, cur_num_sprites);
    VDP_setSpriteLink(sprite_idx_start+cur_num_sprites, 0);
    VDP_updateSprites(cur_num_sprites, transfer_type);
}


void set_weapon_sprite_position_offset(s16 offX, s16 offY) {
    

    
    AnimationFrame *frame1 = cur_sprite_def->animations[0]->frames[0];
    FrameVDPSprite** f = frame1->frameInfos[0].frameVDPSprites;
    for(int i = 0; i < cur_num_sprites; i++) {
        FrameVDPSprite* spr = f[i];
        VDP_setSpritePosition(i+sprite_idx_start,
            BASE_WEAPON_X+spr->offsetX+offX,
            BASE_WEAPON_Y+spr->offsetY+offY
        );
    }
    VDP_updateSprites(cur_num_sprites, DMA_QUEUE);
    
   
}