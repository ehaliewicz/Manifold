#include "span_buf.h"
#include "common.h"


typedef struct span span;

// represents a portion of the screen,
//from and including x1, and up to but not including x2
struct span {
    s16 x1, x2;
    span* next;
};

static span* all_spans; //[MAX_SPANS];

span* free_span_pointer = NULL;

span* span_buf_head = NULL;


static span* alloc_span() {
    if(free_span_pointer != NULL) {
        span* sp = free_span_pointer;
        free_span_pointer = sp->next;
        return sp;
    }
    while(1) {
        VDP_drawTextBG(VDP_PLAN_A, "Out of spans!", 10, 20);
    }
    return NULL;
}

static void free_span(span* sp) {
    sp->x1 = -1;
    sp->x2 = -1;
    sp->next = free_span_pointer;
    free_span_pointer = sp;
}

int num_render_spans;
render_span* render_spans_for_wall; //[MAX_SPANS];

void init_span_buffer() {
  //all_spans = MEM_alloc(sizeof(span) * MAX_SPANS);
  //render_spans_for_wall = MEM_alloc(sizeof(render_span) * MAX_SPANS);
}

void cleanup_span_buffer() {
  //MEM_free(all_spans);
  //MEM_free(render_spans_for_wall);
}

void enqueue_render_span(s16 clip_x1, s16 clip_x2) {
  render_spans_for_wall[num_render_spans].clip_x1 = clip_x1;
  render_spans_for_wall[num_render_spans++].clip_x2 = clip_x2;
}

// each span represents an undrawn portion of screen
// when you insert a span, if it intersects with a span in the buffer
// draw the portion of the wall that is intersecting, and remove the
// intersecting portion from the buffer

// the span buffer starts off with a single span that covers the whole screen
// and as we project and draw spans, we remove chunks of that span,
// modifying, removing, and creating entries in the buffer

// eventually, when we have drawn all of the screen, the span buffer
// will be completely empty, and at that point we know we can stop
// trying to draw walls

// the advantage of this approach (versus, e.g. a byte per column
// marking if we have drawn this column of pixels)
// is that we pay this cost only once per span, instead of once per
// each column of pixels

// it also allows us to short-circuit and stop traversing the bsp tree once
// the screen has been drawn.

// this process is slower than checking a single byte in the column
// drawing loop, so it can be slower if there are many small spans,
// but in general, walls are large enough that it significantly improves performance.


/*

cases

     *----*
*--*

resolution: continue

  *-----*
*---*

resolution: draw overlapped portion, update old span, update inserting span, continue



*-----*
*--*

resolution: draw overlapped portion, delete old span, update inserting span, continue



*-----*
 *--*

resolution: draw overlapped portion, delete old span, update inserting span, continue


*-----*
   *--*

resolution: draw overlapped portion, delete old span, update inserting span, exit


*-----*
    *----*

resolution: draw overlapped portion, update old span, exit


*-----*
         *----*

resolution: exit
*/
int insert_span(s16 x1, s16 x2, u8 insert) {
  s16 orig_x1 = x1;
  s16 orig_x2 = x2;
  span *old,*current, *n;
  u8 update_vert_clipping = !insert;

  num_render_spans = 0;

  if(span_buf_head == NULL) { return 1; }

  // fast bail-out for offscreen polygons
  if(x2 <= 0 || x1 >= W) {
    return 0;
  }
  

  for (old = NULL, current = span_buf_head;
       current != NULL;
       old = current, current = current->next) {
      
    if(current->x2 == x2 && current->x1 == x1) {
      n=current->next;
      enqueue_render_span(x1, x2);
      if(insert) {
          free_span(current);
          if (old) {
              old->next=n;
          } else {
              span_buf_head = n;
              if(span_buf_head == NULL) { return 1; }
          }
      }
      return 0;
    }
    if (current->x2 <= x1) { // case 1
      continue;
    }
    
    if (current->x1 < x1) {
      if (current->x2 <= x2) { // case 2
        enqueue_render_span(x1, current->x2);
        if(insert) {
            current->x2 = x1;
        } 
        if(current->x2 == x2) { return 0; }
      }
      else { // case 3
        enqueue_render_span(x1, x2);
        if(insert) {
            n = alloc_span();
            n->next = current->next;
            current->next = n;
            n->x2 = current->x2;
            current->x2 = x1;
            n->x1 = x2;
        }
        return 0;

      }
    } else {
      if (current->x1>=x2) { // case 6
        return 0;
      }
      
      if (current->x2<=x2) { // case 4
        enqueue_render_span(current->x1, current->x2);

        s16 cx2 = current->x2;
        n=current->next;
        if(insert) {
            free_span(current);
            
            if (old) { 
                old->next=n;
            } else {
                span_buf_head = n;
                if(span_buf_head == NULL) { return 1; }
            }
        }
        if(cx2 == x2) { return 0; }
        current=n;
        if (current==NULL) {
            return 0;
        }
      } else { // case 5
        enqueue_render_span(current->x1, x2);
        if(insert) {
            current->x1 = x2;
        }
      }
    }
  }
  return 0;
}


void clear_span_buffer() {
    for(int i = 0; i < MAX_SPANS-1; i++) {
        all_spans[i].next = &(all_spans[i+1]);
    }
    all_spans[MAX_SPANS-1].next = NULL;

    free_span_pointer = &(all_spans[0]);

    // initialize span buffer to contain the whole screen
    span_buf_head = alloc_span();
    span_buf_head->x1 = 0;
    span_buf_head->x2 = W-1;
    span_buf_head->next = NULL;
}
