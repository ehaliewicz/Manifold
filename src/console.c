#include <genesis.h>
#include "graphics_res.h"
#include "vwf.h"

#define CONSOLE_MAX_MSG_SIZE 49
#define CONSOLE_NUM_MESSAGES 4
#define CONSOLE_NUM_MESSAGES_MASK 0b111

#define NUM_TILES 32

tile* tile_buf;

static uint16_t start_vram_addr;

static int rendered_tiles_are_dirty = 0;

static int tiles_to_draw;


typedef struct {
    int len;
    uint16_t ticks_left;
    char msg[CONSOLE_MAX_MSG_SIZE];
} console_msg;


console_msg message;

static s16 rendered_sprites_idx;
static u16 used_sprites;


uint16_t console_init(uint16_t start_addr) {

    u16 available_sprites = VDP_getAvailableSprites();
    used_sprites = 80 - available_sprites;
    rendered_sprites_idx = VDP_allocateSprites(32);

    rendered_tiles_are_dirty = 0;
    tiles_to_draw = 0;


    //read = write = 0;
    //messages = MEM_alloc(sizeof(console_msg)*CONSOLE_NUM_MESSAGES);
    message.ticks_left = 0;
    message.msg[0] = 0;

    tile_buf = MEM_alloc(sizeof(tile)*NUM_TILES);


    start_vram_addr = start_addr;
    
    //VDP_setWindowAddress(VDP_getAPlanAddress()); 
    //VDP_setWindowVPos(TRUE, 21); 
    
    //memset(messages, 0, sizeof(console_msg)*CONSOLE_NUM_MESSAGES);
    return start_vram_addr+NUM_TILES;
}

void console_cleanup() {
    //MEM_free(messages);
    MEM_free(tile_buf);
}

void copy_console_message(char *msg, int len, uint16_t ticks) {
    if(len > CONSOLE_MAX_MSG_SIZE) {
        len = CONSOLE_MAX_MSG_SIZE;
    }
    strcpy(message.msg, msg);
    message.len = len;
    message.ticks_left = ticks;
    rendered_tiles_are_dirty = 1;
}


void console_push_message(char *msg, int len, uint16_t ticks) {
    if(message.ticks_left != 0) {
        return;
    }
    copy_console_message(msg, len, ticks);
}


void console_push_message_high_priority(char *msg, int len, uint16_t ticks) {
    copy_console_message(msg, len, ticks);
}

#define CONSOLE_BASE_X 4
#define CONSOLE_BASE_Y 4
//20
//21

void console_render() {
    //tiles_to_draw = vwf_count_tiles(message.msg, message.len);
    tiles_to_draw = message.len;

    if(tiles_to_draw > NUM_TILES) {
        tiles_to_draw = NUM_TILES;
    }

    // clean up the last tile we will be using, in case it has garbage (because we might only render to the left half of it)
    memset(tile_buf[tiles_to_draw-1].rows, 0, 32);
    //for(int i = 0; i < tiles_to_draw; i++) {
    //    memcpy(tile_buf+(i*32), skybox_gradient.tileset->tiles, 32);
    //}

    //vwf_render_tiles(message.msg, message.len, tile_buf, tiles_to_draw);
    vwf_render_to_separate_tiles(message.msg, message.len, tile_buf, tiles_to_draw);

    VDP_loadTileData(&(tile_buf[0].rows[0]), start_vram_addr, tiles_to_draw, DMA_QUEUE);
    
    

    int spr_idx = rendered_sprites_idx;

    u16 cur_x_pos = CONSOLE_BASE_X*8;

    for(int i = 0; i < tiles_to_draw; i++) {
        u16 char_width = charmap[message.msg[i]-32].width*2;

        VDP_setSprite(
            spr_idx,
            //CONSOLE_BASE_X*8+(i*8),
            cur_x_pos,
            CONSOLE_BASE_Y*8,
            SPRITE_SIZE(1,1),
            TILE_ATTR_FULL(3, 1, 0, 0, start_vram_addr+i)
        );
        if(i == tiles_to_draw-1) {
            VDP_setSpriteLink(spr_idx, 0);
        } else {
            VDP_setSpriteLink(spr_idx, spr_idx+1);
        }
        KLog_U1("cur x pos: ", cur_x_pos);
        cur_x_pos += char_width;

        spr_idx++;

    }

    VDP_setSpriteLink(used_sprites-1, rendered_sprites_idx);
    //VDP_setSpriteLink
    //VDP_linkSprites(0, used_sprites+tiles_to_draw-1);

    VDP_updateSprites(used_sprites+tiles_to_draw, DMA_QUEUE);
    // load rendered tiles into VRAM
    //const u16 base_tile = TILE_ATTR_FULL(3, 1, 0, 0, start_vram_addr);
    //VDP_fillTileMapRectInc(BG_B, base_tile, CONSOLE_BASE_X, CONSOLE_BASE_Y, tiles_to_draw, 1);

    // clear unused tiles
    //int clear_tiles = NUM_TILES-tiles_to_draw;
    //const u16 clear_base_tile = TILE_ATTR_FULL(3, 0, 0, 0, 0x390); //0x39E);
    //VDP_fillTileMapRect(BG_B, clear_base_tile, CONSOLE_BASE_X+tiles_to_draw, CONSOLE_BASE_Y, clear_tiles, 1);

    

    rendered_tiles_are_dirty = 0;

}

const sin_table[32] = {
    0x14,0x18,0x1c,0x1f,0x22,0x25,0x26,0x28,
0x28,0x28,0x26,0x25,0x22,0x1f,0x1c,0x18,
0x14,0x10,0xc,0x9,0x6,0x3,0x2,0x0,
0x0,0x0,0x2,0x3,0x6,0x9,0xc,0x10,
};
/*
const sin_table[32*8] = {
    0x10,0x10,0x11,0x11,0x12,0x12,0x12,0x13,
0x13,0x14,0x14,0x14,0x15,0x15,0x15,0x16,
0x16,0x16,0x17,0x17,0x18,0x18,0x18,0x19,
0x19,0x19,0x1a,0x1a,0x1a,0x1a,0x1b,0x1b,
0x1b,0x1c,0x1c,0x1c,0x1c,0x1d,0x1d,0x1d,
0x1d,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1f,
0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x1f,0x1f,0x1f,0x1f,0x1f,
0x1f,0x1f,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,
0x1d,0x1d,0x1d,0x1d,0x1c,0x1c,0x1c,0x1c,
0x1b,0x1b,0x1b,0x1a,0x1a,0x1a,0x1a,0x19,
0x19,0x19,0x18,0x18,0x18,0x17,0x17,0x16,
0x16,0x16,0x15,0x15,0x15,0x14,0x14,0x14,
0x13,0x13,0x12,0x12,0x12,0x11,0x11,0x10,
0x10,0x10,0xf,0xf,0xe,0xe,0xe,0xd,
0xd,0xc,0xc,0xc,0xb,0xb,0xb,0xa,
0xa,0xa,0x9,0x9,0x8,0x8,0x8,0x7,
0x7,0x7,0x6,0x6,0x6,0x6,0x5,0x5,
0x5,0x4,0x4,0x4,0x4,0x3,0x3,0x3,
0x3,0x2,0x2,0x2,0x2,0x2,0x2,0x1,
0x1,0x1,0x1,0x1,0x1,0x1,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x1,0x1,0x1,0x1,0x1,
0x1,0x1,0x2,0x2,0x2,0x2,0x2,0x2,
0x3,0x3,0x3,0x3,0x4,0x4,0x4,0x4,
0x5,0x5,0x5,0x6,0x6,0x6,0x6,0x7,
0x7,0x7,0x8,0x8,0x8,0x9,0x9,0xa,
0xa,0xa,0xb,0xb,0xb,0xc,0xc,0xc,
0xd,0xd,0xe,0xe,0xe,0xf,0xf,0x10,
};
*/
u16 sin_table_idx = 0;

void swizzle_sprites() {
    u16 cur_idx = sin_table_idx;
    for(int i = 0; i < tiles_to_draw; i++) {
        vdpSpriteCache[rendered_sprites_idx+i].y = CONSOLE_BASE_Y*8 + sin_table[cur_idx%32] + 0x80;
        cur_idx++;
        //VDP_setSpritePosition() rendered_sprites_idx+i;
    }
    sin_table_idx++;
    VDP_updateSprites(used_sprites+tiles_to_draw, DMA_QUEUE);
}


void console_tick() {
    if(message.ticks_left) {
        if(rendered_tiles_are_dirty) {
            console_render();
        }
        
        if(message.ticks_left-- == 1) {
            //const u16 clear_base_tile = TILE_ATTR_FULL(3, 0, 0, 0, 0x390);
            //VDP_fillTileMapRect(BG_B, clear_base_tile, CONSOLE_BASE_X, CONSOLE_BASE_Y, tiles_to_draw, 1);
            
            //VDP_linkSprites(0, used_sprites);
            VDP_setSpriteLink(used_sprites-1, 0);
            VDP_updateSprites(used_sprites+tiles_to_draw, DMA_QUEUE);
            //VDP_releaseSprites(rendered_sprites_idx, 1);

            message.ticks_left = 0;
            message.len = 0;
        } else {
            
            swizzle_sprites();
        }
    }
}

