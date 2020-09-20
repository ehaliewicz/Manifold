#ifndef AUTOMAP_H
#define AUTOMAP_H

#include <genesis.h>


#define ZOOM 6

//const int zoom = 4;
//#define ZOOM 8
//#define ZOOM_SHIFT 3

//#define ZOOM 4
//#define ZOOM_SHIFT 2


void draw_automap(u32 cur_frame);

extern u8* processed_linedef_bitmap;


void init_processed_linedef_bitmap();

void clear_linedef_processed_bitmap();

void cleanup_automap();



#endif