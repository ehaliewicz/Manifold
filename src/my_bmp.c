#include <genesis.h>
#include "utils.h"
/*

    **---- bitmap mode module ----**

    - copied with modifications from SGDK

*/

VDPPlane bmp_plane;
u8 bmp_pal;
u8 bmp_prio;
u16* bmp_plane_addr;
u8 bmp_doublebuffer;


u8* bmp_buffer_0;
u8* bmp_buffer_1;
//u8* bmp_buffer_read;
//u8* bmp_buffer_write;

static vu16 state;
static vs16 phase;

static u8 start_blit_cell;

#define BMP_PLAN_ADR            (*bmp_plane_addr)

#define BMP_FB0TILEMAP_BASE     BMP_PLAN_ADR

#define BMP_FB1TILEMAP_BASE     (BMP_PLAN_ADR + ((BMP_PLANWIDTH * (BMP_PLANHEIGHT / 2)) * 2))

#define BMP_FBTILEMAP_OFFSET    (((BMP_PLANWIDTH * BMP_CELLYOFFSET) + BMP_CELLXOFFSET) * 2)
#define GET_YOFFSET             ((bmp_doublebuffer && READ_IS_FB1)?((BMP_PLANHEIGHT / 2) + 4):4)


// !! this is divided by 2 because our vertical framebuffer buffer #2 reuses half of the tiles of the first framebuffer!
#define BMP_VERTICAL_FB1TILEINDEX        (BMP_BASETILEINDEX + (BMP_CELLWIDTH * BMP_CELLHEIGHT / 2))

#define READ_IS_FB0             (bmp_buffer_read == bmp_buffer_0)
#define READ_IS_FB1             (bmp_buffer_read == bmp_buffer_1)
#define WRITE_IS_FB0            (bmp_buffer_write == bmp_buffer_0)
#define WRITE_IS_FB1            (bmp_buffer_write == bmp_buffer_1)


#define BMP_STAT_FLIPPING       (1 << 0)
#define BMP_STAT_BLITTING       (1 << 1)
#define BMP_STAT_FLIPWAITING    (1 << 2)

#define NTSC_TILES_BW           7
#define PAL_TILES_BW            10


#define BMP_VERTICAL_CELLHEIGHT     18
#define BMP_VERTICAL_HEIGHT         (BMP_VERTICAL_CELLHEIGHT * BMP_YPIXPERTILE)

u8* bmp_get_write_pointer(u16 x, u16 y) {
    const u16 off = (y * BMP_PITCH) + (x >> 1);
    // return write address
    return bmp_buffer_write + off;
}

u8* bmp_get_read_pointer(u16 x, u16 y) {
    const u16 off = (y * BMP_PITCH) + (x >> 1);
    // return read address
    return bmp_buffer_read + off;
}


u16 bmp_get_dma_write_offset(u16 x, u16 y) {
  u16 x_col_offset = x & 1;
  u16 base_offset = 0;
  if(x & 0b10) {
    // use right half of framebuffer
    base_offset = (SCREEN_WIDTH/2)*BMP_VERTICAL_HEIGHT;
  }
  u16 y_offset = y * 2;
  u16 x_num_pair_cols_offset = x >> 2;
  u16 x_cols_offset = x_num_pair_cols_offset * BMP_VERTICAL_HEIGHT * 2;
  return base_offset + y_offset + x_col_offset + x_cols_offset; // + 16;
}


void bmp_reset_phase() {
    phase = 0;
}


extern void bmp_clear_vertical_bitmap_buffer(u8 *bmp_buffer);
extern void bmp_clear_bitmap_buffer(u8 *bmp_buffer);

void bmp_vertical_clear() {
	bmp_clear_vertical_bitmap_buffer(bmp_buffer_write);
}

void bmp_clear() {
    bmp_clear_bitmap_buffer(bmp_buffer_write);
}


static void bmp_clear_vertical_vram_buffer(u16 num) {
    if (num) DMA_doVRamFill(BMP_FB1TILE, BMP_PITCH * BMP_VERTICAL_HEIGHT, 0, 1);
    else DMA_doVRamFill(BMP_FB0TILE, BMP_PITCH * BMP_VERTICAL_HEIGHT, 0, 1);
    VDP_waitDMACompletion();
}

static void bmp_clear_vram_buffer(u16 num)
{
    if (num) DMA_doVRamFill(BMP_FB1TILE, BMP_PITCH * BMP_HEIGHT, 0, 1);
    else DMA_doVRamFill(BMP_FB0TILE, BMP_PITCH * BMP_HEIGHT, 0, 1);
    VDP_waitDMACompletion();
}



static void bmp_init_tilemap(u16 num)
{
    vu32 *plctrl;
    vu16 *pwdata;
    u16 tile_ind;
    u32 addr_tilemap;
    u16 i, j;

    VDP_setAutoInc(2);

    // calculated
    const u32 offset = BMP_FBTILEMAP_OFFSET;

    if (num == 0)
    {
        addr_tilemap = BMP_FB0TILEMAP_BASE + offset;
        tile_ind = TILE_ATTR_FULL(bmp_pal, bmp_prio, 0, 0, BMP_FB0TILEINDEX);
    }
    else
    {
        addr_tilemap = BMP_FB1TILEMAP_BASE + offset;
        tile_ind = TILE_ATTR_FULL(bmp_pal, bmp_prio, 0, 0, BMP_FB1TILEINDEX);
    }

    // point to vdp port
    plctrl = (u32 *) GFX_CTRL_PORT;
    pwdata = (u16 *) GFX_DATA_PORT;

    i = BMP_CELLHEIGHT;

    while(i--)
    {
        // set destination address for tilemap
        *plctrl = GFX_WRITE_VRAM_ADDR(addr_tilemap);

        // write tilemap line to VDP
        j = BMP_CELLWIDTH >> 3;

        while(j--)
        {
            *pwdata = tile_ind++;
            *pwdata = tile_ind++;
            *pwdata = tile_ind++;
            *pwdata = tile_ind++;
            *pwdata = tile_ind++;
            *pwdata = tile_ind++;
            *pwdata = tile_ind++;
            *pwdata = tile_ind++;
        }
		

        addr_tilemap += BMP_PLANWIDTH * 2;
    }
}

static void bmp_init_tilemap_vertical(u16 num) { 
	vu32 *plctrl;
    vu16 *pwdata;
    u16 tile_ind;
    u32 addr_tilemap;
    u16 i, j;

    VDP_setAutoInc(2);

    // calculated
    const u32 offset = BMP_FBTILEMAP_OFFSET;

    if (num == 0)
    {
        addr_tilemap = BMP_FB0TILEMAP_BASE + offset;
        tile_ind = TILE_ATTR_FULL(bmp_pal, bmp_prio, 0, 0, BMP_FB0TILEINDEX);
    }
    else
    {
        addr_tilemap = BMP_FB1TILEMAP_BASE + offset;
        tile_ind = TILE_ATTR_FULL(bmp_pal, bmp_prio, 0, 0, BMP_VERTICAL_FB1TILEINDEX);
    }

    // point to vdp port
    plctrl = (u32 *) GFX_CTRL_PORT;
    pwdata = (u16 *) GFX_DATA_PORT;

    i = BMP_VERTICAL_CELLHEIGHT;

    u16 base_tile_ind = tile_ind;

    while(i--)
    {
        // set destination address for tilemap
        *plctrl = GFX_WRITE_VRAM_ADDR(addr_tilemap);

        // write tilemap line to VDP
        j = BMP_CELLWIDTH >> 3;

    tile_ind = base_tile_ind;
    while(j--)
        {
      *pwdata = tile_ind; tile_ind += 18;//20;
      *pwdata = tile_ind; tile_ind += 18;
      *pwdata = tile_ind; tile_ind += 18;
      *pwdata = tile_ind; tile_ind += 18;
      *pwdata = tile_ind; tile_ind += 18;
      *pwdata = tile_ind; tile_ind += 18;
      *pwdata = tile_ind; tile_ind += 18;
      *pwdata = tile_ind; tile_ind += 18;
    }
    base_tile_ind++;
    /*
    while(j--)
        {
      *pwdata = tile_ind++;
      *pwdata = tile_ind++;
      *pwdata = tile_ind++;
      *pwdata = tile_ind++;
      *pwdata = tile_ind++;
      *pwdata = tile_ind++;
      *pwdata = tile_ind++;
      *pwdata = tile_ind++;
        }
    */

        addr_tilemap += BMP_PLANWIDTH * 2;
    } 
}


void flip_buffer()
{
    if (READ_IS_FB0)
    {
        bmp_buffer_read = bmp_buffer_1;
        bmp_buffer_write = bmp_buffer_0;
    }
    else
    {
        bmp_buffer_read = bmp_buffer_0;
        bmp_buffer_write = bmp_buffer_1;
    }

}

#define TRANSFER(x)                                     \
    *pldata = src[((BMP_PITCH * 0) / 4) + (x)]; \
    *pldata = src[((BMP_PITCH * 1) / 4) + (x)]; \
    *pldata = src[((BMP_PITCH * 2) / 4) + (x)]; \
    *pldata = src[((BMP_PITCH * 3) / 4) + (x)]; \
    *pldata = src[((BMP_PITCH * 4) / 4) + (x)]; \
    *pldata = src[((BMP_PITCH * 5) / 4) + (x)]; \
    *pldata = src[((BMP_PITCH * 6) / 4) + (x)]; \
    *pldata = src[((BMP_PITCH * 7) / 4) + (x)];

#define TRANSFER8(x)                \
    TRANSFER((8 * x) + 0)   \
    TRANSFER((8 * x) + 1)   \
    TRANSFER((8 * x) + 2)   \
    TRANSFER((8 * x) + 3)   \
    TRANSFER((8 * x) + 4)   \
    TRANSFER((8 * x) + 5)   \
    TRANSFER((8 * x) + 6)   \
    TRANSFER((8 * x) + 7)


static u16 do_blit() {
    static u16 pos_i;
    vu32 *plctrl;
    vu32 *pldata;
    u32 *src;
    u32 addr_tile;
    u16 i;

	
    VDP_setAutoInc(2);

    src = (u32 *) bmp_buffer_read;

    if (bmp_doublebuffer && READ_IS_FB1) {
        addr_tile = BMP_FB1TILE;
    } else {
        addr_tile = BMP_FB0TILE;
	}
		
    /* point to vdp ctrl port */
    plctrl = (u32 *) GFX_CTRL_PORT;

    // previous blit not completed ?
    if (state & BMP_STAT_BLITTING)
    {
        // adjust tile address
        addr_tile += pos_i * BMP_CELLWIDTH * 32;
        // adjust src pointer
        src += pos_i * (BMP_YPIXPERTILE * (BMP_PITCH / 4));

        // set destination address for tile
        *plctrl = GFX_WRITE_VRAM_ADDR(addr_tile);
    }
    else
    {
        // start blit
        state |= BMP_STAT_BLITTING;
        pos_i = start_blit_cell; //12;
        src += pos_i * (BMP_YPIXPERTILE * (BMP_PITCH / 4));
        addr_tile += pos_i * BMP_CELLWIDTH * 32;

        // set destination address for tile
        *plctrl = GFX_WRITE_VRAM_ADDR(addr_tile);
    }

    const u16 remain = BMP_CELLHEIGHT - pos_i;

    if (IS_PALSYSTEM)
    {
        if (remain < PAL_TILES_BW) i = remain;
        else i = PAL_TILES_BW;
    }
    else
    {
        if (remain < NTSC_TILES_BW) i = remain;
        else i = NTSC_TILES_BW;
    }

    // save position
    pos_i += i;

    /* point to vdp data port */
    pldata = (u32 *) GFX_DATA_PORT;

    while(i--)
    {
        // send it to VRAM
        TRANSFER8(0)
        TRANSFER8(1)
        TRANSFER8(2)
        TRANSFER8(3)

        src += (8 * BMP_PITCH) / 4;
    }

    // blit not yet done
    if (pos_i < BMP_CELLHEIGHT) return 0;

    // blit done
    state &= ~BMP_STAT_BLITTING;

    return 1;
}



static void do_flip()
{
    // wait for DMA completion if used otherwise VDP writes can be corrupted
    VDP_waitDMACompletion();

    // copy tile buffer to VRAM
	
    if (do_blit())
    {
        // at this point copy to VRAM is done !

        // flip displayed buffer
        if (bmp_doublebuffer)
        {
            u16 vscr;

            // switch displayed buffer
            if (READ_IS_FB0) vscr = 0;
            else vscr = (BMP_PLANHEIGHT * 8) / 2;

            VDP_setVerticalScroll(bmp_plane, vscr);
        }

        // get bitmap state
        u16 s = state;

        // we had a pending flip request ?
        if (s & BMP_STAT_FLIPWAITING)
        {
            // flip buffers
            flip_buffer();
            // clear only pending flag and process new flip
            s &= ~BMP_STAT_FLIPWAITING;
        }
        else
            // flip done
            s &= ~BMP_STAT_FLIPPING;

        // save back bitmap state
        state = s;
    }
}

void bmp_do_hblank_process_vertical() {
    // vborder low
    if (phase == 0)
    {
        const u16 vcnt = GET_VCOUNTER;
        const u16 scrh = screenHeight;
		
		
		const u16 vborder = (scrh - 177) >> 1; 
        //const u16 vborder = (scrh - BMP_HEIGHT) >> 1; //BMP_HEIGHT) >> 1;

		//KLog_U1("vborder: ", vborder);
		//KLog_U1("vborder 2: ", vborder2);
        // enable VDP
        VDP_setEnable(1);
        // prepare hint to disable VDP and doing blit process
		//u8 hint_counter = VDP_getHIntCounter();
		//KLog_U1("next target to disabled: ", (scrh - vborder) - (hint_counter + vcnt + 3));
        VDP_setHIntCounter((scrh - vborder) - (VDP_getHIntCounter() + vcnt + 3));
        // update phase
        phase = 1;
    }
    // in active screen
    else if (phase == 1)
    {
        phase = 2;
    }
    // vborder high
    else if (phase == 2)
    {
        // disable VDP
        VDP_setEnable(0);
        // prepare hint to re enable VDP
		//KLog_U1("enabled vdp, next target: ", ((screenHeight-144)>>1) - 1);
		//KLog_U1("enabled vdp, old next target: ", ((screenHeight-BMP_HEIGHT)>>1) - 1);
        //VDP_setHIntCounter(((screenHeight - 180) >> 1) - 1);
        VDP_setHIntCounter(((screenHeight - BMP_HEIGHT) >> 1) - 1);
        //VDP_setHIntCounter(((screenHeight - BMP_VERTICAL_HEIGHT) >> 1) - 1);
        // update phase
        phase = 3;

        // flip requested or not complete ? --> start / continu flip
        //if (state & BMP_STAT_FLIPPING) do_flip();
    }
}

void bmp_do_hblank_process_horizontal() {
    // vborder low
    if (phase == 0)
    {
        const u16 vcnt = GET_VCOUNTER;
        const u16 scrh = screenHeight;
		
		
		const u16 vborder = (scrh - 177) >> 1; 
        //const u16 vborder = (scrh - BMP_HEIGHT) >> 1; //BMP_HEIGHT) >> 1;

		//KLog_U1("vborder: ", vborder);
		//KLog_U1("vborder 2: ", vborder2);
        // enable VDP
        VDP_setEnable(1);
        // prepare hint to disable VDP and doing blit process
		//u8 hint_counter = VDP_getHIntCounter();
		//KLog_U1("next target to disabled: ", (scrh - vborder) - (hint_counter + vcnt + 3));
        VDP_setHIntCounter((scrh - vborder) - (VDP_getHIntCounter() + vcnt + 3));
        // update phase
        phase = 1;
    }
    // in active screen
    else if (phase == 1)
    {
        phase = 2;
    }
    // vborder high
    else if (phase == 2)
    {
        // disable VDP
        VDP_setEnable(0);
        // prepare hint to re enable VDP
		//KLog_U1("enabled vdp, next target: ", ((screenHeight-144)>>1) - 1);
		//KLog_U1("enabled vdp, old next target: ", ((screenHeight-BMP_HEIGHT)>>1) - 1);
        //VDP_setHIntCounter(((screenHeight - 180) >> 1) - 1);
        VDP_setHIntCounter(((screenHeight - BMP_HEIGHT) >> 1) - 1);
        // update phase
        phase = 3;

        // flip requested or not complete ? --> start / continu flip
        if (state & BMP_STAT_FLIPPING) do_flip();
    }
}

void do_vint_flip();

void bmp_reset_vertical() {
    // better to disable ints here
    // FIXME: for some reason disabling interrupts generally break BMP init :-/
//    SYS_disableInts();

    // cancel bitmap interrupt processing
	//HIntProcess &= ~PROCESS_BITMAP_TASK;
    //VIntProcess &= ~PROCESS_BITMAP_TASK;
    SYS_setVIntCallback(NULL);
    SYS_setHIntCallback(NULL);

	// disable H-Int
	VDP_setHInterrupt(0);
	// re enabled VDP if it was disabled because of extended blank
	VDP_setEnable(1);
	// reset hint counter
	VDP_setHIntCounter(255);
	
    // allocate bitmap buffer if needed
	if (!bmp_buffer_0) {
		bmp_buffer_0 = MEM_alloc(BMP_PITCH * BMP_VERTICAL_HEIGHT * sizeof(u8));
	}
	if (!bmp_buffer_1) {
		bmp_buffer_1 = MEM_alloc(BMP_PITCH * BMP_VERTICAL_HEIGHT * sizeof(u8));
	}
	
    // need 64x64 cells sized plane
    VDP_setPlaneSize(64, 64, TRUE);
    // clear plane (complete tilemap)
    VDP_clearPlane(bmp_plane, TRUE);

    // reset state and phase
    state = 0;
    phase = -1;


    // default
    bmp_buffer_read = bmp_buffer_0;
    bmp_buffer_write = bmp_buffer_1;

    // prepare tilemap
	bmp_init_tilemap_vertical(0);
	bmp_clear_vertical_vram_buffer(0);
	
    if (bmp_doublebuffer)
    {
		bmp_init_tilemap_vertical(1);
		bmp_clear_vertical_vram_buffer(1);

    }

    // clear both buffer in memory
	bmp_clear_vertical_bitmap_buffer(bmp_buffer_0);
	bmp_clear_vertical_bitmap_buffer(bmp_buffer_1);
	
	
    // set back vertical scroll to 0
    VDP_setVerticalScroll(bmp_plane, 0);
	
    // prepare hint for extended blank on next frame
    VDP_setHIntCounter(((screenHeight - BMP_VERTICAL_HEIGHT) >> 1) - 1);
    // enabled bitmap interrupt processing
    
    //HIntProcess |= PROCESS_BITMAP_TASK;
    //VIntProcess |= PROCESS_BITMAP_TASK;
    SYS_setHIntCallback(bmp_do_hblank_process_vertical);
    SYS_setVIntCallback(do_vint_flip);

    VDP_setHInterrupt(1);
}



void bmp_reset_horizontal() {
    // better to disable ints here
    // FIXME: for some reason disabling interrupts generally break BMP init :-/
//    SYS_disableInts();

    // cancel bitmap interrupt processing
    SYS_setVIntCallback(NULL);
    SYS_setHIntCallback(NULL);

	// disable H-Int
	VDP_setHInterrupt(0);
	// re enabled VDP if it was disabled because of extended blank
	VDP_setEnable(1);
	// reset hint counter
	VDP_setHIntCounter(255);
	
    // allocate bitmap buffer if needed
	if (!bmp_buffer_0) {
		bmp_buffer_0 = MEM_alloc(BMP_PITCH * BMP_HEIGHT * sizeof(u8));
	}
	if (!bmp_buffer_1) {
		bmp_buffer_1 = MEM_alloc(BMP_PITCH * BMP_HEIGHT * sizeof(u8));
	}
	
    // need 64x64 cells sized plane
    VDP_setPlaneSize(64, 64, TRUE);
    // clear plane (complete tilemap)
    VDP_clearPlane(bmp_plane, TRUE);

    // reset state and phase
    state = 0;
    phase = -1;


    // default
    bmp_buffer_read = bmp_buffer_0;
    bmp_buffer_write = bmp_buffer_1;

    // prepare tilemap
	bmp_init_tilemap(0);
	bmp_clear_vram_buffer(0);
	
    if (bmp_doublebuffer)
    {
		bmp_init_tilemap(1);
		bmp_clear_vram_buffer(1);

    }

    // clear both buffer in memory
	bmp_clear_bitmap_buffer(bmp_buffer_0);
	bmp_clear_bitmap_buffer(bmp_buffer_1);
	
	
    // set back vertical scroll to 0
    VDP_setVerticalScroll(bmp_plane, 0);
	
    // prepare hint for extended blank on next frame
    VDP_setHIntCounter(((screenHeight - BMP_HEIGHT) >> 1) - 1);
    // enabled bitmap interrupt processing
    
    //HIntProcess |= PROCESS_BITMAP_TASK;
    //VIntProcess |= PROCESS_BITMAP_TASK;
    SYS_setHIntCallback(bmp_do_hblank_process_horizontal);
    SYS_setVIntCallback(bmp_reset_phase);

    VDP_setHInterrupt(1);
}

void bmp_init(u16 double_buffer, VDPPlane plane, u16 palette, u16 priority, u8 vertical) {
    bmp_doublebuffer = double_buffer;
    bmp_plane = plane;
    bmp_pal = palette & 3;
    bmp_prio = priority & 1;

    switch(plane)
    {
        default:
        case BG_B:
            bmp_plane_addr = &bgb_addr;
            break;

        case BG_A:
            bmp_plane_addr = &bga_addr;
            break;

        case WINDOW:
            bmp_plane_addr = &window_addr;
            break;
    }

    bmp_buffer_0 = NULL;
    bmp_buffer_1 = NULL;

    if(vertical) {
        bmp_reset_vertical();
    } else {
        bmp_reset_horizontal();
    }
}


vu8 vint_flipping;
vu8 vint_flip_requested;

void bmp_init_vertical(u16 double_buffer, VDPPlane plane, u16 palette, u16 priority) {
    
    vint_flip_requested = 0;
    vint_flipping = 0;
    bmp_init(double_buffer, plane, palette, priority, 1);
}
void bmp_init_horizontal(u16 double_buffer, VDPPlane plane, u16 palette, u16 priority) {
    bmp_init(double_buffer, plane, palette, priority, 0);
}


void bmp_end()
{
    // better to disable ints here
    // FIXME: for some reason disabling interrupts generally break BMP init :-/
//    SYS_disableInts();

    // cancel interrupt processing
    SYS_setVIntCallback(NULL);
    SYS_setHIntCallback(NULL);

    //HIntProcess &= ~PROCESS_BITMAP_TASK;
    //VIntProcess &= ~PROCESS_BITMAP_TASK;

    // disable H-Int
    VDP_setHInterrupt(0);
    // re enabled VDP if it was disabled because of extended blank
    VDP_setEnable(1);
    // reset hint counter
    VDP_setHIntCounter(255);

    // reset back vertical scroll to 0
    VDP_setVerticalScroll(bmp_plane, 0);

    // release memory
    if (bmp_buffer_0)
    {
        MEM_free(bmp_buffer_0);
        bmp_buffer_0 = NULL;
    }
    if (bmp_buffer_1)
    {
        MEM_free(bmp_buffer_1);
        bmp_buffer_1 = NULL;
    }

    // try to pack memory free blocks (before to avoid memory fragmentation)
    MEM_pack();

    // we can re enable ints
    // FIXME: for some reason disabling interrupts generally break BMP init :-/
//    SYS_enableInts();
}

u16 bmp_has_flip_request_pending()
{
    if (state & BMP_STAT_FLIPWAITING) return 1;

    return 0;
}

void bmp_wait_while_flip_request_pending()
{
    while (bmp_has_flip_request_pending());
}

u16 bmp_has_flip_in_progress()
{
    if (state & BMP_STAT_FLIPPING) return 1;

    return 0;
}

void bmp_wait_flip_complete()
{
    while (bmp_has_flip_in_progress());
}



u16 bmp_flip(u16 async)
{
    // wait until pending flip is processed
    bmp_wait_while_flip_request_pending();

    // currently flipping ?
    if (state & BMP_STAT_FLIPPING)
    {
        // set a pending flip
        state |= BMP_STAT_FLIPWAITING;

        // wait completion
        if (!async) bmp_wait_flip_complete();

        return 1;
    }

    // better to disable ints here
    // FIXME: for some reason disabling interrupts generally break BMP init :-/
//    SYS_disableInts();

    // flip bitmap buffer
    flip_buffer();
    // flip started (will be processed in blank period --> BMP_doBlankProcess)
    state |= BMP_STAT_FLIPPING;

    // we can re enable ints
//    SYS_enableInts();

    // wait completion
    if (!async) bmp_wait_flip_complete();

    return 0;
}


u16 bmp_flip_partial(u16 async, u8 start_cell) {
    // wait until pending flip is processed
    bmp_wait_while_flip_request_pending();

    // currently flipping ?
    if (state & BMP_STAT_FLIPPING)
    {
        // set a pending flip
        state |= BMP_STAT_FLIPWAITING;

        // wait completion
        if (!async) bmp_wait_flip_complete();

        return 1;
    }

    // better to disable ints here
    // FIXME: for some reason disabling interrupts generally break BMP init :-/
	//SYS_disableInts();

	
    // flip bitmap buffer
    flip_buffer();
    // flip started (will be processed in blank period --> BMP_doBlankProcess)
    state |= BMP_STAT_FLIPPING;
	start_blit_cell = start_cell;

    // we can re enable ints
	//SYS_enableInts();
	
    // wait completion
    if (!async) bmp_wait_flip_complete();

    return 0;
}

void bmp_show_fps(u16 float_display) {
    char str[16];
    const u16 y = GET_YOFFSET + 1;

    if (float_display)
    {
        fix32ToStr(SYS_getFPSAsFloat(), str, 1);
        VDP_clearTextBG(bmp_plane, 2, y, 5);
    }
    else
    {
        uintToStr(SYS_getFPS(), str, 1);
        VDP_clearTextBG(bmp_plane, 2, y, 2);
    }

    // display FPS
    VDP_drawTextBG(bmp_plane, str, 1, y);
}


volatile u32 vram_copy_dst;
volatile u8* vram_copy_src;


volatile u32 in_use_vram_copy_dst;
volatile u8* in_use_vram_copy_src;

#define FULL_BYTES (SCREEN_WIDTH*SCREEN_HEIGHT)
#define FULL_WORDS (FULL_BYTES/2)
#define HALF_WORDS (FULL_WORDS/2)
#define HALF_BYTES (FULL_BYTES/2)
#define QUARTER_WORDS (HALF_WORDS/2)
#define QUARTER_BYTES (HALF_BYTES/2)

u8* bmp_buffer_0;
u8* bmp_buffer_1;

void after_flip_vscroll_adjustment() {
    u16 vscr;
    if(vram_copy_src == bmp_buffer_1) {
        vscr = (BMP_PLANHEIGHT * 8) / 2;
    } else {
        vscr = 0;
    }
    VDP_setVerticalScroll(BG_A, vscr);
}

void copy_quarter_words(u8* src, u32 dst) {
    DMA_doDma(DMA_VRAM,
        src,
        dst,
        QUARTER_WORDS, 4);
}

u16 vints = 0;
void do_vint_flip() {
    phase = 0;
    vints++;
    if(vint_flipping == 1) {

        // not finished
        // complete second half here

        // second half for framebuffer 0
        // first half for framebuffer 1

        
        if(in_use_vram_copy_src == bmp_buffer_0) {
            // draw second quarter of framebuffer to third quarter of VRAM framebuffer

            copy_quarter_words(
                (u8*)in_use_vram_copy_src+QUARTER_BYTES, 
                in_use_vram_copy_dst+HALF_BYTES);
            copy_quarter_words(
                (u8*)in_use_vram_copy_src+QUARTER_BYTES + HALF_BYTES,
                in_use_vram_copy_dst+2+HALF_BYTES);
            //copy_quarter_words(
            //    (u8*)in_use_vram_copy_src+QUARTER_BYTES,
            //    in_use_vram_copy_dst+2+HALF_BYTES);
        } else {
            copy_quarter_words( 
                (u8*)in_use_vram_copy_src, 
                in_use_vram_copy_dst);
            //copy_quarter_words(
            //    (u8*)in_use_vram_copy_src,
            //    in_use_vram_copy_dst+2);
            copy_quarter_words(
                (u8*)in_use_vram_copy_src+HALF_BYTES,
                in_use_vram_copy_dst+2);
        }
        

        after_flip_vscroll_adjustment();

        vint_flipping = 0;
    } else if(vint_flip_requested) {
        vint_flipping = 1;
        vint_flip_requested = 0;
        in_use_vram_copy_dst = vram_copy_dst;
        in_use_vram_copy_src = vram_copy_src;


        // first half for framebuffer 1
        // second half for framebuffer 0
        if(in_use_vram_copy_src == bmp_buffer_0) {
            copy_quarter_words(
                (u8*)in_use_vram_copy_src, 
                in_use_vram_copy_dst);
            //copy_quarter_words(
            //    (u8*)in_use_vram_copy_src,
            //    in_use_vram_copy_dst+2);
            copy_quarter_words(
                (u8*)in_use_vram_copy_src+HALF_BYTES,
                in_use_vram_copy_dst+2);
        } else { 
            copy_quarter_words(
                (u8*)in_use_vram_copy_src+QUARTER_BYTES, 
                in_use_vram_copy_dst+HALF_BYTES);
            //copy_quarter_words(
            //    (u8*)in_use_vram_copy_src+QUARTER_BYTES, 
            //    in_use_vram_copy_dst+2+HALF_BYTES);
            copy_quarter_words(
                (u8*)in_use_vram_copy_src+QUARTER_BYTES+HALF_BYTES,
                in_use_vram_copy_dst+2+HALF_BYTES);
        }
    }


}


#define FB0MIDDLEINDEX (BMP_BASETILEINDEX + (BMP_CELLWIDTH/2 * BMP_CELLHEIGHT))
#define FB0MIDDLE (FB0MIDDLEINDEX*32)

void request_flip() {
    while(vint_flip_requested || vint_flipping) {
        // vblank is behind one request
        // wait until it has started, and then we can safely flip to the next framebuffer
        //return;
    }

    if(bmp_buffer_write == bmp_buffer_0) {
        vram_copy_src = bmp_buffer_0;
        vram_copy_dst = BMP_FB0TILE;
        bmp_buffer_write = bmp_buffer_1;
        bmp_buffer_read = bmp_buffer_0;
    } else {
        vram_copy_src = bmp_buffer_1;
        vram_copy_dst = FB0MIDDLE; 
        bmp_buffer_write = bmp_buffer_0;
        bmp_buffer_read = bmp_buffer_1;
    }
    vint_flip_requested = 1;
}
