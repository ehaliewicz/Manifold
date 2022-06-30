#include <genesis.h>
#include "vwf.h"

#define CONSOLE_MAX_MSG_SIZE 79
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

uint16_t console_init(uint16_t start_addr) {
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
#define CONSOLE_BASE_Y 21
#define CONSOLE_BASE_Y2 53


void render_console() {
    tiles_to_draw = vwf_count_tiles(message.msg, message.len);
    if(tiles_to_draw > NUM_TILES) {
        tiles_to_draw = NUM_TILES;
    }

    // clean up the last tile we will be using, in case it has garbage (because we might only render to the left half of it)
    memset(tile_buf[tiles_to_draw-1].rows, 0, 32);
    vwf_render_tiles(message.msg, message.len, tile_buf, tiles_to_draw);
    VDP_loadTileData(&(tile_buf[0].rows[0]), start_vram_addr, tiles_to_draw, CPU); //DMA_QUEUE);
    

    const u16 base_tile = TILE_ATTR_FULL(3, 1, 0, 0, start_vram_addr);
    VDP_fillTileMapRectInc(BG_B, base_tile, CONSOLE_BASE_X, CONSOLE_BASE_Y, tiles_to_draw, 1);
    //for(int i = 0; i < tiles_to_draw; i++) {
    //    VDP_setTileMapXY(BG_B, base_tile+i, CONSOLE_BASE_X+i, CONSOLE_BASE_Y);
    //}

    int clear_tiles = NUM_TILES-tiles_to_draw;
    const u16 clear_base_tile = TILE_ATTR_FULL(3, 0, 0, 0, 0x39E);
    VDP_fillTileMapRect(BG_B, clear_base_tile, CONSOLE_BASE_X+tiles_to_draw, CONSOLE_BASE_Y, clear_tiles, 1);


    rendered_tiles_are_dirty = 0;

}


void console_tick() {
    if(message.ticks_left) {
        if(rendered_tiles_are_dirty) {
            render_console();
        }
        
        if(message.ticks_left-- == 1) {
            const u16 clear_base_tile = TILE_ATTR_FULL(3, 0, 0, 0, 0x39E);
            VDP_fillTileMapRect(BG_B, clear_base_tile, CONSOLE_BASE_X, CONSOLE_BASE_Y, tiles_to_draw, 1);
            message.ticks_left = 0;
            message.len = 0;
        }
    }
}

