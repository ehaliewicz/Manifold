#ifndef SPAN_BUF
#define SPAN_BUF

#include <genesis.h>
#include "common.h"

#define MAX_SPANS W

typedef struct {
  s16 clip_x1;
  s16 clip_x2;
} render_span;

extern int num_render_spans;
extern render_span* render_spans_for_wall; //[MAX_SPANS];

// inserts a span into the span buffer data structure
// drawing wall spans as necessary
int insert_span(s16 x1, s16 x2, u8 insert);

void init_span_buffer();
void clear_span_buffer();
void cleanup_span_buffer();

#endif