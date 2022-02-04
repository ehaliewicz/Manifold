#include <genesis.h>


#define BMP_VERTICAL_CELLHEIGHT     18
#define BMP_VERTICAL_HEIGHT         (BMP_VERTICAL_CELLHEIGHT * BMP_YPIXPERTILE)

//VDPPlane bmp_plane;
//u8 bmp_pal;
//u8 bmp_prio;
//u16* bmp_plane_addr;
//u8 bmp_doublebuffer;

/*

    **---- bitmap mode module ----**

    - copied with modifications from SGDK

*/


u8* bmp_buffer_0;
u8* bmp_buffer_1;

u8* bmp_get_write_pointer(u16 x, u16 y);
u8* bmp_get_read_pointer(u16 x, u16 y);

void bmp_reset_phase();

void bmp_vertical_clear();
void bmp_clear();

void bmp_reset();
void bmp_reset_vertical(VoidCallback *vintcallback);

void bmp_init_horizontal(u16 double_buffer, VDPPlane plane, u16 palette, u16 priority);
void bmp_init_vertical(u16 double_buffer, VDPPlane plane, u16 palette, u16 priority, VoidCallback *vintcallback);


void bmp_end();

u16 bmp_flip_partial(u16 async, u8 start_cell);
void bmp_wait_while_flip_request_pending();

void bmp_show_fps(u16 float_display);