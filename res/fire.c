#include <genesis.h>
#include "fire.h"

//u8 *fire_buf;

//u8 byte_fire_lut[1024];
u8 table_0[256];
u8 table_1[256];
u8 table_2[256];
u16 table_0_shift[256];
u16 table_1_shift[256];
u16 table_2_shift[256];

//u16 shift_left[256];
//u8 fire_tbl[256];



void init_fire_lut() {
	for(int i = 0; i <= 0xFF; i++) {
		u8 h = (i >> 4) & 0xF;
		u8 l = i & 0xF;
		u8 next_h = (h == 0) ? 0 : h-1;
		u8 next_l = (l == 0) ? 0 : l-1;
		u8 next_fire_0 = (next_h << 4 | next_l);
		u8 next_fire_1 = (next_h << 4 | l);
		u8 next_fire_2 = (h << 4 | next_l);
        u8 retain_fire =      (h << 4 | l);
		
        //fire_tbl[i] = next_fire_0;
        int rand_bits = random() & 0b11;

        //byte_fire_lut[0b00 | i << 2] = retain_fire;

        //byte_fire_lut[0b01 | i << 2] = next_fire_0;
		//byte_fire_lut[0b10 | i << 2] = next_fire_1;
		//byte_fire_lut[0b11 | i << 2] = next_fire_2;
        
        table_0[i] = next_fire_0;
        table_1[i] = next_fire_1;
        table_2[i] = next_fire_2;
        table_0_shift[i] = next_fire_0 << 8;
        table_1_shift[i] = next_fire_1 << 8;
        table_2_shift[i] = next_fire_2 << 8;


	}
}

#define BASE_FRAMEBUFFER_OFFSET (160-FIRE_HEIGHT)
//(160-(FIRE_HEIGHT*2))


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

void init_fire() {
	init_fire_lut();

    //fire_buf = MEM_alloc(FIRE_WIDTH * FIRE_HEIGHT);
	//memset(fire_buf, 0, FIRE_WIDTH*FIRE_HEIGHT);
    //memset(fire_skip_cache, FIRE_HEIGHT-2, FIRE_WIDTH);

	VDP_setPalette(PAL1, fire_cols);
}

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

        
        /*
		*/
        
        /*
        switch(rand_bits & 0b111) { 
            case 0b000: 
                fire_0 = *src_ptr++;
                fire_1 = *src_ptr++;
                *(dst_ptr-1) = fire_0;
                *dst_ptr++ = fire_1;
                dst_ptr++;
                break;
            case 0b001:
                src_ptr++;
                fire_1 = *src_ptr++;
                *dst_ptr++ = fire_1;
                dst_ptr++;
                break;
            case 0b010:
                src_ptr++;
                fire_1 = *src_ptr++;
                dst_ptr++;
                *dst_ptr++ = table_0[fire_1];
                break;
            case 0b011:
                fire_0 = *src_ptr++;
                fire_1 = *src_ptr++;
                dst_ptr++;
                *dst_ptr++ = table_0[fire_1];
                *dst_ptr = table_2[fire_0];
                break;
            case 0b100:
                fire_0 = *src_ptr++;
                fire_1 = *src_ptr++;
                *(dst_ptr-1) = fire_0;
                dst_ptr += 2;
                *dst_ptr = table_1[fire_1];
                break;
            case 0b101:
                fire_0 = *src_ptr++;
                fire_1 = *src_ptr++;
                *dst_ptr++ = table_0[fire_0];
                dst_ptr++;
                *dst_ptr = table_2[fire_1];
                break;
            case 0b110:
                fire_0 = *src_ptr++;
                fire_1 = *src_ptr++;
                dst_ptr++;
                *dst_ptr++ = table_1[fire_0];
                *(dst_ptr+1) = table_2[fire_1];
                break;
            case 0b111:
                fire_0 = *src_ptr++;
                fire_1 = *src_ptr++;
                dst_ptr += 2;
                *((u16*)dst_ptr) = table_2_shift[fire_0] | table_2[fire_1];
                break;
        }*/
        /*
		for(int x = 0; x < FIRE_WIDTH;) {

            u8 fire = *src_ptr++;
            u8 next_fire = 0;
            u8 rand_bits = (r & rand_mask);
            
            // index lookup table with random bits added
            next_fire = byte_fire_lut[(fire<<2) | rand_bits];

            *(dst_ptr+rand_bits) = next_fire;
            dst_ptr++;

								
            r >>= 1;
            bits--;          
            if(bits == 0) {
                r = *rptr++;
                bits = 16;
            }
            x++;
		}
        */
        
        
    

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




void scroll_fire_a() {
	static int scroll_idx = 0;
	int lines = FIRE_HEIGHT*2;

	int pos = scroll_idx;
	int y_pos = 64+32;
	while(lines) {
		int scrolls_remaining = num_scrolls - pos;
		if(lines <= scrolls_remaining) {
			VDP_setHorizontalScrollLine(BG_A, y_pos, &scrolls[pos % num_scrolls], lines, DMA);
			break;
		} else {
			VDP_setHorizontalScrollLine(BG_A, y_pos, &scrolls[pos % num_scrolls], scrolls_remaining, DMA);
			y_pos += scrolls_remaining;
			pos = 0;
			lines -= scrolls_remaining;
		}
	}

	scroll_idx++;
	if(scroll_idx >= num_scrolls) {
		scroll_idx = 0;
	}
}


void scroll_fire_b() {
	static int scroll_idx = 0;
	int lines = FIRE_HEIGHT-8;

	int pos = scroll_idx;
	int y_pos = 64+40;
	while(lines) {
		int scrolls_remaining = num_scrolls - pos;
		if(lines <= scrolls_remaining) {
			VDP_setHorizontalScrollLine(BG_B, y_pos, &scrolls[pos % num_scrolls], lines, DMA);
			break;
		} else {
			VDP_setHorizontalScrollLine(BG_B, y_pos, &scrolls[pos % num_scrolls], scrolls_remaining, DMA);
			y_pos += scrolls_remaining;
			pos = 0;
			lines -= scrolls_remaining;
		}
	}

	scroll_idx++;
	if(scroll_idx >= num_scrolls) {
		scroll_idx = 0;
	}
}