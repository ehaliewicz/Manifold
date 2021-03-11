#include <genesis.h>
#include "fire.h"
#include "game_mode.h"
#include "graphics_res.h"
#include "joy_helper.h"
#include "music_res.h"


static u8* lookup_table_base;
static u8* table_0;
static u8* table_1;
static u8* table_2;
static u16* table_0_shift;
static u16* table_1_shift;
static u16* table_2_shift;


void reset_scroll() {
	for(int i = 0; i < 240; i++) {
		s16 scroll = 0;
		VDP_setHorizontalScrollLine(BG_B, i, &scroll, 1, CPU);
		VDP_setHorizontalScrollLine(BG_A, i, &scroll, 1, CPU);
	}
}



void init_fire_lut() {
    // allocate 2.3 KB
    lookup_table_base = MEM_alloc(256*9);
    table_0 = lookup_table_base+(256*0);
    table_1 = lookup_table_base+(256*1);
    table_2 = lookup_table_base+(256*2);
    table_0_shift = (u16*)(lookup_table_base+(256*3));
    table_1_shift = (u16*)(lookup_table_base+(256*5));
    table_1_shift = (u16*)(lookup_table_base+(256*7));

	for(int i = 0; i <= 0xFF; i++) {
		u8 h = (i >> 4) & 0xF;
		u8 l = i & 0xF;
		u8 next_h = (h == 0) ? 0 : h-1;
		u8 next_l = (l == 0) ? 0 : l-1;
		u8 next_fire_0 = (next_h << 4 | next_l);
		u8 next_fire_1 = (next_h << 4 | l);
		u8 next_fire_2 = (h << 4 | next_l);
        //u8 retain_fire =      (h << 4 | l);
		
        //int rand_bits = random() & 0b11;

        table_0[i] = next_fire_0;
        table_1[i] = next_fire_1;
        table_2[i] = next_fire_2;
        table_0_shift[i] = next_fire_0 << 8;
        table_1_shift[i] = next_fire_1 << 8;
        table_2_shift[i] = next_fire_2 << 8;


	}
}



void start_fire_source() {	
    u8* ptr = BMP_getWritePointer(0, BASE_FRAMEBUFFER_OFFSET+FIRE_HEIGHT-1);
    memset(ptr, 0xFF, 128);
}

void clear_fire_source() {	
    u8* ptr = BMP_getWritePointer(0, BASE_FRAMEBUFFER_OFFSET+FIRE_HEIGHT-1);
    memset(ptr, 0x00, 128);
}


const u16 fire_cols[16] = {
	RGB24_TO_VDPCOLOR(0x000000),
	RGB24_TO_VDPCOLOR(0x2F0F07),
	RGB24_TO_VDPCOLOR(0x470F07),
	RGB24_TO_VDPCOLOR(0x771F07),
	RGB24_TO_VDPCOLOR(0x8F2707),
	RGB24_TO_VDPCOLOR(0xAF3F07),
	RGB24_TO_VDPCOLOR(0xDF4F07),
	RGB24_TO_VDPCOLOR(0xDF5707),
	RGB24_TO_VDPCOLOR(0xCF6F0F),
	RGB24_TO_VDPCOLOR(0xCF8717),
	RGB24_TO_VDPCOLOR(0xC7971F),
	RGB24_TO_VDPCOLOR(0xBFA727),
	RGB24_TO_VDPCOLOR(0xB7AF2F),
	RGB24_TO_VDPCOLOR(0xCFCF6F),
	RGB24_TO_VDPCOLOR(0xEFEFC7),
	RGB24_TO_VDPCOLOR(0xFFFFFF)
};


#define NUM_RANDS ((FIRE_WIDTH*FIRE_HEIGHT)>>4)+1

u16 rands[NUM_RANDS];


void fire_native(u8* src_ptr, u8* dst_ptr, u8* bmp_ptr, u16* rand_ptr, u8* byte_fire_lut_ptr);

void fire_native_dbl(u8* src_ptr, u8* dst_ptr, u16* rand_ptr, u8* table_0_ptr, u8* table_1_ptr, u8* table_2_ptr);
void fire_native_quad(u8* src_ptr, u8* dst_ptr, u16* rand_ptr, u8* table_0_ptr, u8* table_1_ptr, u8* table_2_ptr);

void copy_fire_native(u32* src_ptr, u32* dst_ptr);

void copy_fire_buffer_portion() {
    u32* src_ptr = (u32*)BMP_getReadPointer(0, BASE_FRAMEBUFFER_OFFSET);
    u32* dst_ptr = (u32*)BMP_getWritePointer(0, BASE_FRAMEBUFFER_OFFSET);
    copy_fire_native(src_ptr, dst_ptr);
    return;
    for(int i = 0; i < FIRE_HEIGHT*(FIRE_WIDTH>>2); i+=8) {
            *dst_ptr++ = *src_ptr++;
            *dst_ptr++ = *src_ptr++;
            *dst_ptr++ = *src_ptr++;
            *dst_ptr++ = *src_ptr++;
            *dst_ptr++ = *src_ptr++;
            *dst_ptr++ = *src_ptr++;
            *dst_ptr++ = *src_ptr++;
            *dst_ptr++ = *src_ptr++;
    }
}


void spread_and_draw_fire_byte() {
    
    for(int i = 0; i < NUM_RANDS; i++) {
        rands[i] = random();
    }

    
    u8* src_ptr = BMP_getReadPointer(0,  BASE_FRAMEBUFFER_OFFSET+1); //&fire_buf[FIRE_WIDTH];
    u8* dst_ptr = BMP_getWritePointer(0,  BASE_FRAMEBUFFER_OFFSET+0); //&fire_buf[0]-1;


    u16* rptr = rands;  


    //fire_native(src_ptr, dst_ptr, bmp_ptr, rptr, &byte_fire_lut[0]);
    //fire_native_dbl(src_ptr, dst_ptr, rptr, table_0, table_1, table_2);
    
    fire_native_quad(src_ptr, dst_ptr, rptr, table_0, table_1, table_2);

    return;

    
    u16 r = *rptr++;
    u8 bits = 16;

    //const u8 rand_mask = 0b011;
	for(int y = 1; y < FIRE_HEIGHT; y++) {
        for(int x = 0; x < FIRE_WIDTH; x += 4) {
            //u8 rand_bits = (r & 0b11111);
            u8 fire_0, fire_1, fire_2, fire_3;
            switch(r & 0b01111) {
                case 0b00000:
                    fire_0 = *src_ptr++;
                    fire_1 = *src_ptr++;
                    fire_2 = *src_ptr++;
                    fire_3 = *src_ptr++;
                    *(dst_ptr-1) = fire_0;
                    *((u16*)dst_ptr) = (fire_1 << 8)|fire_2;
                    dst_ptr += 2; // dst_ptr = 2
                    *dst_ptr++ = fire_3; // dst_ptr = 3
                    dst_ptr += 1;
                    break;
                case 0b00001:
                    src_ptr += 1;
                    fire_1 = *src_ptr++;
                    fire_2 = *src_ptr++;
                    fire_3 = *src_ptr++;
                    *((u16*)dst_ptr) = (fire_1 << 8)|fire_2;
                    dst_ptr += 2; // dst_ptr = 2
                    *dst_ptr++ = fire_3; // dst_ptr = 3
                    dst_ptr += 1;
                    break;
                case 0b00010:
                    src_ptr += 2;
                    fire_2 = *src_ptr++;
                    fire_3 = *src_ptr++;
                    dst_ptr += 1; // dst_ptr = 1
                    *dst_ptr++ = fire_2; // dst_ptr = 2
                    *dst_ptr++ = fire_3; // dst_ptr = 3
                    dst_ptr += 1;
                    break;
                case 0b00011:
                    src_ptr += 3; // modified from +3
                    fire_3 = *src_ptr++;
                    fire_2 = *src_ptr;
                    dst_ptr += 1; // dst_ptr = 1
                    *dst_ptr++ = fire_2; // dst_ptr = 2
                    *dst_ptr++ = fire_3; // dst_ptr = 3
                    //src_ptr += -1;
                    dst_ptr += 1;
                    break;
                case 0b00100:
                    fire_0 = *src_ptr++;
                    src_ptr += 2;
                    fire_3 = *src_ptr++;
                    *(dst_ptr-1) = fire_0;
                    dst_ptr += 2; // dst_ptr = 2
                    *dst_ptr++ = fire_3; // dst_ptr = 3
                    dst_ptr += 1;
                    break;
                case 0b00101:
                    fire_0 = *src_ptr++;
                    src_ptr += 2;
                    fire_3 = *src_ptr++;
                    *dst_ptr++ = table_0[fire_0]; // dst_ptr = 1
                    dst_ptr += 1; // dst_ptr = 2
                    *dst_ptr++ = fire_3; // dst_ptr = 3
                    dst_ptr += 1;
                    break;
                case 0b00110:
                    fire_0 = *src_ptr++;
                    fire_1 = *src_ptr++;
                    src_ptr += 1;
                    fire_3 = *src_ptr++;
                    dst_ptr += 1; // dst_ptr = 1
                    *dst_ptr++ = table_1[fire_0]; // dst_ptr = 2
                    *((u16*)dst_ptr) = (fire_3 << 8)|table_2[fire_1];
                    dst_ptr += 2;
                    break;
                case 0b00111:
                    src_ptr += 3;
                    fire_3 = *src_ptr++;
                    fire_1 = *src_ptr++;
                    dst_ptr += 2; // dst_ptr = 2
                    *((u16*)dst_ptr) = (fire_3 << 8)|table_2[fire_1];
                    src_ptr += -1;
                    dst_ptr += 2;
                    break;
                case 0b01000:
                    fire_0 = *src_ptr++;
                    fire_1 = *src_ptr++;
                    src_ptr += 1;
                    fire_3 = *src_ptr++;
                    *(dst_ptr-1) = fire_0;
                    *dst_ptr++ = fire_1; // dst_ptr = 1
                    dst_ptr += 2; // dst_ptr = 3
                    *dst_ptr++ = table_0[fire_3]; // dst_ptr = 4
                    break;
                case 0b01001:
                    src_ptr += 1;
                    fire_1 = *src_ptr++;
                    src_ptr += 1;
                    fire_3 = *src_ptr++;
                    *dst_ptr++ = fire_1; // dst_ptr = 1
                    dst_ptr += 2; // dst_ptr = 3
                    *dst_ptr++ = table_0[fire_3]; // dst_ptr = 4
                    break;
                case 0b01010:
                    src_ptr += 1;
                    fire_1 = *src_ptr++;
                    src_ptr += 1;
                    fire_3 = *src_ptr++;
                    dst_ptr += 1; // dst_ptr = 1
                    *dst_ptr++ = table_0[fire_1]; // dst_ptr = 2
                    dst_ptr += 1; // dst_ptr = 3
                    *dst_ptr++ = table_0[fire_3]; // dst_ptr = 4
                    break;
                case 0b01011:
                    fire_0 = *src_ptr++;
                    fire_1 = *src_ptr++;
                    src_ptr += 1;
                    fire_3 = *src_ptr++;
                    dst_ptr += 1; // dst_ptr = 1
                    *dst_ptr++ = table_0[fire_1]; // dst_ptr = 2
                    *((u16*)dst_ptr) = table_2_shift[fire_0]|table_0[fire_3];
                    dst_ptr += 2;
                    break;
                case 0b01100:
                    fire_0 = *src_ptr++;
                    fire_1 = *src_ptr++;
                    fire_2 = *src_ptr++;
                    fire_3 = *src_ptr++;
                    *(dst_ptr-1) = fire_0;
                    dst_ptr += 2; // dst_ptr = 2
                    *((u16*)dst_ptr) = table_1_shift[fire_1]|table_0[fire_3];
                    dst_ptr += 2; // dst_ptr = 4
                    *dst_ptr = table_2[fire_2];
                    break;
                case 0b01101:
                    fire_0 = *src_ptr++;
                    fire_1 = *src_ptr++;
                    fire_2 = *src_ptr++;
                    fire_3 = *src_ptr++;
                    *dst_ptr++ = table_0[fire_0]; // dst_ptr = 1
                    dst_ptr += 1; // dst_ptr = 2
                    *((u16*)dst_ptr) = table_1_shift[fire_1]|table_0[fire_3];
                    dst_ptr += 2; // dst_ptr = 4
                    *dst_ptr = table_2[fire_2];
                    break;
                case 0b01110:
                    fire_0 = *src_ptr++;
                    src_ptr += 2;
                    fire_3 = *src_ptr++;
                    fire_2 = *src_ptr;
                    dst_ptr += 1; // dst_ptr = 1
                    *dst_ptr++ = table_1[fire_0]; // dst_ptr = 2
                    dst_ptr += 1; // dst_ptr = 3
                    *dst_ptr++ = table_0[fire_3]; // dst_ptr = 4
                    *dst_ptr = table_2[fire_2]; // dst_ptr = 5
                    break;
                case 0b01111:
                    fire_0 = *src_ptr++;
                    src_ptr += 2;
                    fire_3 = *src_ptr++;
                    fire_2 = *src_ptr;
                    dst_ptr += 2; // dst_ptr = 2
                    *((u16*)dst_ptr) = table_2_shift[fire_0]|table_0[fire_3];
                    dst_ptr += 2; // dst_ptr = 4
                    *dst_ptr = table_2[fire_2];
                    break;
                
            }
 
            r >>= 4;
            bits -= 4;         
            if(bits == 0) {
                r = *rptr++;
                bits = 16;
            }
        }
        
    }
}

        
        
    

int num_scrolls = 128;
const s16 scrolls[128] = {
	0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,
	0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,
	0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,
	0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,
	0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,
	0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,
	0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,
	0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,
};



static int cur_scroll_fixed;
static const int dscroll = 0b100;

static int fire_running = 0;
static int fire_frame = 0;
static int fire_hidden = 0;


static Sprite* fire_spr;
static Sprite* spr2;
static Sprite* spr3;

void init_fire() {
    //XGM_startPlay(xgm_e2m2);

	SYS_disableInts();
    //VDP_setScreenWidth320();
    //VDP_setScreenHeight224();
	VDP_setBackgroundColor(3);
	cur_scroll_fixed = -220<<2;

	VDP_setScrollingMode(HSCROLL_LINE, VSCROLL_PLANE);
	VDP_setVerticalScroll(BG_B, cur_scroll_fixed>>2);
	VDP_setVerticalScroll(BG_A, 0);


    

	BMP_init(0, BG_A, PAL1, 0, 0);
	//SPR_init();
	//SYS_enableInts();
    //return

	//fire_spr = SPR_addSpriteEx(&fire_fixup,       256-32, 183, TILE_ATTR(1, 1, 0, 0), 0, (SPR_FLAG_FAST_AUTO_VISIBILITY | SPR_FLAG_AUTO_SPRITE_ALLOC | SPR_FLAG_AUTO_VRAM_ALLOC));
	//SYS_enableInts();
    //return;
	//spr2 = SPR_addSpriteExSafe(&bottom_line_cover,    70, 184, TILE_ATTR(0, 1, 0, 0), 0, (SPR_FLAG_FAST_AUTO_VISIBILITY | SPR_FLAG_AUTO_SPRITE_ALLOC));
	//spr3 = SPR_addSpriteExSafe(&bottom_line_cover, 70+96, 184, TILE_ATTR(0, 1, 0, 0), 0, (SPR_FLAG_FAST_AUTO_VISIBILITY | SPR_FLAG_AUTO_SPRITE_ALLOC));
	//SPR_setVisibility(fire_spr, VISIBLE);
	//SPR_setVisibility(spr2, VISIBLE);
	//SPR_setVisibility(spr3, VISIBLE);

    // side effect: loads palette!
	VDP_drawImageEx(BG_B, &doom_logo, 0x0360, 8, 0, 1, 1);

	const int fire_fix_vram_addr = 0x300;
	const int bkgd_cover_vram_addr = 0x304;
	//VDP_loadTileSet(fire_fixup.animations[0]->frames[0]->tileset, fire_fix_vram_addr, CPU);
	//VDP_loadTileSet(bottom_line_cover.animations[0]->frames[0]->tileset, bkgd_cover_vram_addr, CPU);

	//SPR_setVRAMTileIndex(fire_spr, fire_fix_vram_addr);
	//SPR_setVRAMTileIndex(spr2, bkgd_cover_vram_addr);
	//SPR_setVRAMTileIndex(spr3, bkgd_cover_vram_addr);

	start_fire_source();
	BMP_setBufferCopy(0);
	reset_scroll();

	VDP_setPalette(PAL1, fire_cols);
	SYS_enableInts();


	init_fire_lut();
    fire_running = 1;
    fire_frame = 0;

    //SPR_update();

}
game_mode run_fire() {
    fire_frame++;
    //return SAME_MODE;
    if(fire_running) {
        BMP_waitWhileFlipRequestPending();

        spread_and_draw_fire_byte();
        BMP_flipPartial(1, 12, 0);
        copy_fire_buffer_portion();
    }

    
    int skip = joy_button_newly_pressed(BUTTON_START);
    if(skip) {
        fire_frame = 180;
        cur_scroll_fixed = -46<<2;
    } else {
        cur_scroll_fixed += dscroll;
    }
    
    int cur_scroll = cur_scroll_fixed>>2;


    if(cur_scroll < -45) {
        VDP_setVerticalScroll(BG_B, cur_scroll);
    } else if (fire_running && !fire_hidden) {
        //SPR_setVisibility(fire_spr, HIDDEN);
        //SPR_update();
        clear_fire_source();
        fire_hidden = 1;
    }

    if(fire_hidden && skip) {
        fire_frame = 290;
    }


    if(fire_frame == 290) {
        fire_running = 0;
        BMP_clear();
        BMP_flipPartial(0, 12, 0);
    } else if (fire_frame == 291) {
        return MAIN_MENU;
    }

    return SAME_MODE;
}

void cleanup_fire() {
    //KLog("cleaning up up fire");
    //SPR_releaseSprite(fire_spr);
    //KLog("cleaning up up fire 2");
    //SPR_releaseSprite(spr2);
    //KLog("cleaning up up fire 3");
    //SPR_releaseSprite(spr3);
    //KLog("cleaning up up fire 5");
    //SPR_end();
    MEM_free(lookup_table_base);
    BMP_end();
    VDP_clearPlane(BG_B, 1);
    MEM_pack();
}