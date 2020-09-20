#ifndef SPAN_BUF_H
#define SPAN_BUF_H

#include <genesis.h>
#include "math3d.h"

typedef struct {
    u8 x1, x2;
    u8 tex_or_color;   // only 256 different textures possible
} wall_draw_span;

typedef struct {
    u8 x1, y2;
    u8 y1a, y1b;
    u8 y2a, y2b;
} portal_draw_span;

#define MAX_WALL_DRAW_SPANS 64
#define MAX_PORTAL_DRAW_SPANS 64

extern wall_draw_span wall_draw_spans[MAX_WALL_DRAW_SPANS];
extern portal_draw_span portal_draw_spans[MAX_WALL_DRAW_SPANS];

void init_span_buf();

int span_intersects_with_drawable_screen(u8 x1, u8 x2);
int point_intersects_with_drawable_screen(u8 x);


// returns number of spans inserted into either wall_draw_spans or portal_draw_spans
int insert_solid_color_wall_span(transformed_vert v1, transformed_vert v2, u8 col);
int insert_textured_wall_span(transformed_vert v1, transformed_vert v2, u8 texture);
int insert_portal_span(transformed_vert v1, transformed_vert v2);


#endif