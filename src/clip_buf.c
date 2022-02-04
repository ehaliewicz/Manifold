#include <genesis.h>
#include "clip_buf.h"
#include "debug.h"


clip_buf *clip_buffers;

clip_buf* clip_buf_list_head = NULL; 

static int sp;


void init_clip_buffer_list() {
    clip_buffers = MEM_alloc(sizeof(clip_buf) * NUM_CLIP_BUFS);
    sp = 0;
    clip_buf_list_head = &clip_buffers[0];
    for(int i = 0; i < NUM_CLIP_BUFS; i++) {
        clip_buffers[i].id = i;
    }
}

void free_clip_buffer_list() {
    MEM_free(clip_buffers);
}



clip_buf* alloc_clip_buffer() {
    if(sp >= NUM_CLIP_BUFS) {
        return NULL;
    }
    return &clip_buffers[sp++];
}


void free_clip_buffer(clip_buf* buf) {
    clip_buf* freed = &clip_buffers[--sp];

    if(freed != buf) {
        char sbuf[32];
        sprintf("Freed clip buffer %i but expected %i", buf->id, freed->id);
        die(sbuf);
    }
}