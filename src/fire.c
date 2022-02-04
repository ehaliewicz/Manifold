#include <genesis.h>
#include "my_bmp.h"
#include "fire.h"
#include "game_mode.h"
#include "graphics_res.h"
#include "joy_helper.h"
#include "music_res.h"


static u8* lookup_table_base;
static u8* table_0;
static u8* table_1;
static u8* table_2;
//static u16* table_0_shift;
//static u16* table_1_shift;
//static u16* table_2_shift;


void reset_scroll() {
	for(int i = 0; i < 240; i++) {
		s16 scroll = 0;
		VDP_setHorizontalScrollLine(BG_B, i, &scroll, 1, CPU);
		VDP_setHorizontalScrollLine(BG_A, i, &scroll, 1, CPU);
	}
}



void init_fire_lut() {
    // allocate 2.3 KB
    lookup_table_base = MEM_alloc(256*3); //9);
    table_0 = lookup_table_base+(256*0);
    table_1 = lookup_table_base+(256*1);
    table_2 = lookup_table_base+(256*2);
    //table_0_shift = (u16*)(lookup_table_base+(256*3));
    //table_1_shift = (u16*)(lookup_table_base+(256*5));
    //table_1_shift = (u16*)(lookup_table_base+(256*7));

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
        //table_0_shift[i] = next_fire_0 << 8;
        //table_1_shift[i] = next_fire_1 << 8;
        //table_2_shift[i] = next_fire_2 << 8;


	}
}



void start_fire_source() {	
    u8* ptr = bmp_get_write_pointer(0, BASE_FRAMEBUFFER_OFFSET+FIRE_HEIGHT-1);
    memset(ptr, 0xFF, 128);
}

void clear_fire_source() {	
    u8* ptr = bmp_get_write_pointer(0, BASE_FRAMEBUFFER_OFFSET+FIRE_HEIGHT-1);
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




void fire_native_quad(u8* src_ptr, u8* dst_ptr, u16* rand_ptr, u8* table_0_ptr, u8* table_1_ptr, u8* table_2_ptr);

void copy_fire_native(u32* src_ptr, u32* dst_ptr);

void copy_fire_buffer_portion() {
    u32* src_ptr = (u32*)bmp_get_read_pointer(0, BASE_FRAMEBUFFER_OFFSET);
    u32* dst_ptr = (u32*)bmp_get_write_pointer(0, BASE_FRAMEBUFFER_OFFSET);
    copy_fire_native(src_ptr, dst_ptr);
}


void spread_and_draw_fire_byte() {
    
    #define NUM_RANDS ((FIRE_WIDTH*FIRE_HEIGHT)>>4)+1
    //882 bytes
    u16 rands[NUM_RANDS]; //[NUM_RANDS];

    for(int i = 0; i < NUM_RANDS; i++) {
        rands[i] = random();
    }

    
    u8* src_ptr = bmp_get_read_pointer(0,  BASE_FRAMEBUFFER_OFFSET+1); //&fire_buf[FIRE_WIDTH];
    u8* dst_ptr = bmp_get_write_pointer(0,  BASE_FRAMEBUFFER_OFFSET+0); //&fire_buf[0]-1;


    u16* rptr = rands;  


    fire_native_quad(src_ptr, dst_ptr, rptr, table_0, table_1, table_2);

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


int end;
void init_fire() {
    end = 0;
    //XGM_startPlay(xgm_e2m2);
    //intro_wav;

    //SND_startPlay_PCM(intro_wav, sizeof(intro_wav), SOUND_RATE_13400, SOUND_PAN_CENTER, 1);

	SYS_disableInts();
    //VDP_setScreenWidth320();
    //VDP_setScreenHeight224();
	VDP_setBackgroundColor(3);
	cur_scroll_fixed = -220<<2;

	VDP_setScrollingMode(HSCROLL_LINE, VSCROLL_PLANE);
	VDP_setVerticalScroll(BG_B, cur_scroll_fixed>>2);
	VDP_setVerticalScroll(BG_A, 0);


    DMA_setBufferSize(2048);
    MEM_pack();    

	bmp_init_horizontal(0, BG_A, PAL1, 0);



    VDP_setBackgroundColor(0);


	start_fire_source();
	//BMP_setBufferCopy(0);
	reset_scroll();

	VDP_setPalette(PAL1, fire_cols);
	SYS_enableInts();


	init_fire_lut();
    fire_running = 1;
    fire_frame = 0;

    //SPR_update();

}
int end = 0;

game_mode run_fire() {
    bmp_show_fps(1);
    fire_frame++;
    if(fire_running) {

        spread_and_draw_fire_byte();
        bmp_flip_partial(1, 12);
        bmp_wait_while_flip_request_pending();
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
    } else if ((fire_running && !fire_hidden) || skip) {
        clear_fire_source();
        fire_hidden = 1;
    }

    if(fire_hidden && skip) {
        end = 1;
    }


    if(end == 1) {
        fire_running = 0;
        bmp_clear();
        bmp_flip_partial(0, 12);
        end = 2;
    } else if (end == 2) {
        return MAIN_MENU;
    }

    return SAME_MODE;
}

void cleanup_fire() {
    MEM_free(lookup_table_base);
    bmp_end();
    VDP_clearPlane(BG_B, 1);
    MEM_pack();
}