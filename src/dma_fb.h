#ifndef DMA_FB_H
#define DMA_FB_H


#define PIXEL_RIGHT_STEP 1
#define PIXEL_DOWN_STEP 4
//#define PIXEL_DOWN_STEP 2
#define TILE_RIGHT_STEP 4*SCREEN_HEIGHT //640


u16 getDMAWriteOffset(u16 x, u16 y);
void do_vint_flip();
void request_dma_flip();
void reset_dma_framebuffer();

#endif