#include <genesis.h>
#include "clip_buf.h"
#include "utils.h"


clip_buf *clip_buffers;

clip_buf* clip_buf_list_head = NULL; 

static int sp;

void init_clip_buffer_list() {
    clip_buffers = malloc(sizeof(clip_buf) * NUM_CLIP_BUFS, "silhouette clip buffers");
    sp = 0;
    clip_buf_list_head = &clip_buffers[0];
    for(int i = 0; i < NUM_CLIP_BUFS; i++) {
        clip_buffers[i].id = i;
    }
}

void free_clip_buffer_list() {
    free(clip_buffers, "silhouette clip buffers");
}

clip_buf* alloc_clip_buffer() {
    //KLog("allocating clip buffer");
    if(sp >= NUM_CLIP_BUFS) {
        die("no more clip bufs");
        return NULL;
    }
    return &clip_buffers[sp++];
}

void free_clip_buffer(clip_buf* buf) {
    clip_buf* freed = &clip_buffers[--sp];
    //KLog("free clip buffer");
    if(freed != buf) {
        char sbuf[64];
        sprintf(sbuf, "Freed clip buffer %i but expected %i", buf->id, freed->id);
        die(sbuf);
    }
}