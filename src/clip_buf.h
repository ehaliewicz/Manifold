#ifndef CLIP_BUF_H
#define CLIP_BUF_H

#include <genesis.h>

typedef struct clip_buf clip_buf;

struct clip_buf {
    u8 clip_buffer[128];
};




void init_clip_buffer_list();
void free_clip_buffer_list();

clip_buf* clip_buffers;

clip_buf* alloc_clip_buffer();
void free_clip_buffer(clip_buf* buf);


#endif