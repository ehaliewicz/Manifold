#include <genesis.h>
#include "span_buf.h"

static s16 min_drawable_x;
static s16 max_drawable_x;

typedef struct {
    u8 x1, x2;
    u8 prev, next;
} span_entry;

#define MAX_SPAN_ENTRIES 128


void init_span_buf() {
    min_drawable_x = 0;
    max_drawable_x = BMP_WIDTH-1;
}

int point_intersects_with_drawable_screen(u8 x) {
    return (x >= min_drawable_x && x <= max_drawable_x);
}

int span_intersects_with_drawable_screen(u8 x1, u8 x2) {
    if(x2 < min_drawable_x) {
        return 0;
    }
    if(x1 > max_drawable_x) {
        return 0;
    }
    return 1;

    // TODO: implement actual span buffering here

}
