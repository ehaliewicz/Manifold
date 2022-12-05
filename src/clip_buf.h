#ifndef CLIP_BUF_H
#define CLIP_BUF_H

#include <genesis.h>

// 128 bytes * 2 = 256 bytes
#define NUM_CLIP_BUFS 6
typedef struct clip_buf clip_buf;

// we could be really fancy and create just one big buffer, and allocate chunks out of that as needed
// but i doubt we'll ever want to draw four of these on top of each other
struct clip_buf {
    // we should probably just 
    u8 y_clip_buffer[128];
    u8 id;
};




void init_clip_buffer_list();
void free_clip_buffer_list();

clip_buf* clip_buffers;

clip_buf* alloc_clip_buffer();
void free_clip_buffer(clip_buf* buf);


#endif