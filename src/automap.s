.global draw_blockmap_cell_native, transform_vert_native


transform_vert_native:
	movm.l %d2-%d4,-(%sp)
	move.w 12+4+2(%sp),%d0
	move.w 12+8+2(%sp),%d2
	| lsl.w #1,%d0
	| lsl.w #1,%d2
	sub.w playerXFrac4,%d0
	sub.w playerYFrac4,%d2
	move.w %d0, %d1         | d0 = d1 = tlx
	move.w %d2, %d3         | d2 = d3 = tly
	move.w angleCosFrac12,%d4
    muls.w %d4, %d0         | d0 = tlx * angleCosFrac12
    muls.w %d4, %d3         | d3 = tly * angleCosFrac12
    move.w angleSinFrac12, %d4 
    muls.w %d4, %d1         | d1 = tlx * angleSinFrac12
    muls.w %d4, %d2         | d2 = tly * angleSinFrac12

    sub.l %d2, %d0
    add.l %d3, %d1
    swap %d0
    add.w #128, %d0
    swap %d0
    swap %d1 
    moveq #70, %d2
    sub.w %d1, %d2
    move.w %d2, %d0
    movm.l (%sp)+,%d2-%d4
    rts

	.macro TRANSFORM_VERT 
	| expects d0 and d2 to be x,y coords of untransformed vertex
	| trashes d0,d1,d2,d3,d4
	| result x<<16|y in d0

	| move scale coordinates to 12.4 fixed point (but also divide by zoom factor of 4, so it's a left shift of 2)
	
	| lsl.w #2,%d0
	| lsl.w #2,%d2
	| lsl.w #1, %d0		| zoom of 8
	| lsl.w #1, %d2


	| perform translation
	sub.w playerXFrac4,%d0
	sub.w playerYFrac4,%d2
    move.w %d0, %d1         | d0 = d1 = tlx
    move.w %d2, %d3         | d2 = d3 = tly

	| perform rotation
	move.w angleCosFrac12,%d4
    muls.w %d4, %d0         | d0 = tlx * angleCosFrac12
    muls.w %d4, %d3         | d3 = tly * angleCosFrac12
    move.w angleSinFrac12, %d4 
    muls.w %d4, %d1         | d1 = tlx * angleSinFrac12
    muls.w %d4, %d2         | d2 = tly * angleSinFrac12
    sub.l %d2, %d0			| d0 = rotated_x = (tlx*cos) - (tly*sin)   16.16 fixed point
    add.l %d3, %d1			| d1 = rotated_y = (tlx*sin) + (tly*cos)   16.16 fixed point

	| perform adjustment to screen coordinates
    swap %d0				| get integer portion of rx
    add.w #128, %d0			| move to center of screen
    swap %d0				| move rx back to high portion of d0
    swap %d1 				| get integer portion of ry

    moveq #70, %d2			| adjust to center of screen (and flip vertically)
    sub.w %d1, %d2
    move.w %d2, %d0			| move ry into low word of d0

	.endm


draw_blockmap_cell_native:
	movm.l %d2-%d7/%a2-%a6, -(%sp)
	
	move.l 4+44(%sp), %a2		| load pointer to line struct (where we store transformed line data for BMP_clipLine and BMP_drawLine)

	move.l 8+44(%sp), %a3		| load cell pointer into a3
	move.l 12+44(%sp), %d6		| load cur frame in d6
	
	move.l %a2, -(%sp)			| push line pointer onto stack for BMP_clipLine/BMP_drawLine

	move.l (processed_linedef_bitmap), %a4 | load linedef bitmap into a4
	move.l (vertex_cache_frames), %a5	   | load vertex cached frames into a5
	move.l (cached_vertexes), %a6		   | load vertex cache into a6

	move.w (%a3)+, %d5		  | d5 = linedef count
	subq.w #1, %d5
draw_blockmap_cell_loop:
	move.w (%a3)+, %d0		  | d0 = linedef byte index
	move.b (%a3)+, %d2 		  | d2 = bitpos
	bset.b %d2, 0(%a4, %d0.w)
	beq.b draw_linedef
skip_linedef:		      
	add.l #13, %a3				| skip 13 bytes, 6 for each vertex * 2, plus one for color
	bra.w draw_blockmap_linedef_loop_test	
draw_linedef:
	move.b (%a3)+, 9(%a2)		| copy color directly into line struct

	| vertex caching logic for vertex #1
	move.w (%a3)+, %d7			| load vertex idx (prescaled by 4 for fast cache lookup)
	cmp.l 0(%a5, %d7.w), %d6	| check if the current frame matches the one in the vertex cache
	bne.b dont_reuse_v1			| if they're not equal, transform this vertex and cache it
reuse_v1:
	move.l 0(%a6, %d7.w), (%a2) | load vertex from vertex cache and store directly into line struct
	addq.l #4, %a3				| skip past vertex coordinates
	bra.b handle_v2				| and jump to vertex #2

dont_reuse_v1:
	move.w (%a3)+, %d0			| load vertex x component
	move.w (%a3)+, %d2			| load vertex y component
	TRANSFORM_VERT				| transform vertex and place results into d0 (2 16-bit components)
	move.l %d0, (%a2)			| write transformed vertex to line struct
	move.l %d0, 0(%a6, %d7.w)   | write to vertex cache
	move.l %d6, 0(%a5, %d7.w)   | write current frame to vertex cached frame
	
	| vertex caching logic for vertex #2
handle_v2:
	move.w (%a3)+, %d7			 | load vertex idx (prescaled by 4 for fast cache lookup)
	cmp.l 0(%a5, %d7.w), %d6     | check if the current frame matches the one in the vertex cache
	bne.b dont_reuse_v2			 | if they're not equal, transform this vertex and cache it
reuse_v2:
	move.l 0(%a6, %d7.w), 4(%a2) | load vertex from vertex cache and store directly into line struct
	addq.l #4, %a3				 | skip past vertex coordinates
	bra clip_line				 | and jump to vertex #2

dont_reuse_v2:
	move.w (%a3)+, %d0			| load vertex x component
	move.w (%a3)+, %d2			| load vertex y component
	TRANSFORM_VERT				| transform vertex and place results into d0 (2 16-bit components)
	move.l %d0, 4(%a2)			| write transformed vertex to line struct
	move.l %d0, 0(%a6, %d7.w)  | write to vertex cache
	move.l %d6, 0(%a5, %d7.w)  | write current frame to vertex cached frame

clip_line:
	bra.w clipAndMaybeDrawMapLine

after_draw_map_line:

draw_blockmap_linedef_loop_test:
	dbf %d5, draw_blockmap_cell_loop

	addq.l #4, %sp 					| pop line pointer off stack

    movm.l (%sp)+,%d2-%d7/%a2-%a6
	rts



.macro func _name, _align=2
    .section .text.asm.\_name
    .globl  \_name
    .type   \_name, @function
    .align \_align
  \_name:
.endm


func clipAndMaybeDrawMapLine
    movm.l %d2-%d7/%a2-%a5,-(%sp)

    move.l 40(%sp),%a0          | a0 = &line

    movm.w (%a0),%d2-%d5        | d2 = x1, d3 = y1, d4 = x2, d5 = y2

    move.w #255,%d0             | d0 = BMP_WIDTH - 1
    move.w #159,%d1             | d1 = BMP_HEIGHT - 1

    cmp.w %d0,%d2               | if (((u16) x1 < BMP_WIDTH) &&
    jhi .L50
    cmp.w %d0,%d4               |     ((u16) x2 < BMP_WIDTH) &&
    jhi .L50
    cmp.w %d1,%d3               |     ((u16) y1 < BMP_HEIGHT) &&
    jhi .L50
    cmp.w %d1,%d5               |     ((u16) y2 < BMP_HEIGHT))
    jhi .L50

    | movm.w %d2-%d5,(%a0)        |   update line
    | moveq #1,%d0                |   return 1;
    | movm.l (%sp)+,%d2-%d7/%a2-%a5
    bra.w drawMapLine
    | rts

.L60:
    move.w %d4,%d6
    sub.w %d2,%d6               | d6 = dx = x2 - x1;
    move.w %d5,%d7
    sub.w %d3,%d7               | d7 = dy = y2 - y1;

    tst.w %d2                   | if (x1 < 0)
    jge .L38                    | {

    muls.w %d7,%d2              |   y1 -= (x1 * dy) / dx;
    divs.w %d6,%d2
    sub.w %d2,%d3
    moveq #0,%d2                |   x1 = 0;
    jra .L39                    | }

.L38:
    cmp.w %d0,%d2               | else if (x1 >= BMP_WIDTH)
    jle .L39                    | {

    sub.w %d0,%d2
    muls.w %d7,%d2              |   y1 -= ((x1 - (BMP_WIDTH - 1)) * dy) / dx;
    divs.w %d6,%d2
    sub.w %d2,%d3
    move.w %d0,%d2              |   x1 = BMP_WIDTH - 1;
                                | }
.L39:
    tst.w %d4                   | if (x2 < 0)
    jge .L41                    | {

    muls.w %d7,%d4              |   y2 -= (x2 * dy) / dx;
    divs.w %d6,%d4
    sub.w %d4,%d5
    moveq #0,%d4                |   x2 = 0;
    jra .L42                    | }

.L41:
    cmp.w %d0,%d4               | else if (x2 >= BMP_WIDTH)
    jle .L42                    | {

    sub.w %d0,%d4
    muls.w %d7,%d4              |   y2 -= ((x2 - (BMP_WIDTH - 1)) * dy) / dx;
    divs.w %d6,%d4
    sub.w %d4,%d5
    move.w %d0,%d4              |   x2 = BMP_WIDTH - 1;
                                | }
.L42:
    tst.w %d3                   | if (y1 < 0)
    jge .L44                    | {

    muls.w %d6,%d3              |   x1 -= (y1 * dx) / dy;
    divs.w %d7,%d3
    sub.w %d3,%d2
    moveq #0,%d3                |   y1 = 0;
    jra .L45                    | }

.L44:
    cmp.w %d1,%d3               | else if (y1 >= BMP_HEIGHT)
    jle .L45                    | {

    sub.w %d1,%d3
    muls.w %d6,%d3              |   x1 -= ((y1 - (BMP_HEIGHT - 1)) * dx) / dy;
    divs.w %d7,%d3
    sub.w %d3,%d2
    move.w %d1,%d3              |   y1 = BMP_HEIGHT - 1;

.L45:                            | }
    tst.w %d5                   | if (y2 < 0)
    jge .L47                    | {

    muls.w %d6,%d5              |   x2 -= (y2 * dx) / dy;
    divs.w %d7,%d5
    sub.w %d5,%d4
    moveq #0,%d5                |   y2 = 0;
    jra .L48                    | }

.L47:
    cmp.w %d1,%d5               | else if (y2 >= BMP_HEIGHT)
    jle .L48                    | {

    sub.w %d1,%d5
    muls.w %d6,%d5              |   x2 -= ((y2 - (BMP_HEIGHT - 1)) * dx) / dy;
    divs.w %d7,%d5
    sub.w %d5,%d4
    move.w %d1,%d5              |   y2 = BMP_HEIGHT - 1;
                                | }
.L48:
    cmp.w %d0,%d2               | if (((u16) x1 < BMP_WIDTH) &&
    jhi .L50
    cmp.w %d0,%d4               |     ((u16) x2 < BMP_WIDTH) &&
    jhi .L50
    cmp.w %d1,%d3               |     ((u16) y1 < BMP_HEIGHT) &&
    jhi .L50
    cmp.w %d1,%d5               |     ((u16) y2 < BMP_HEIGHT))
    jhi .L50                    | {

    | movm.w %d2-%d5,(%a0)        |   update line
    | moveq #1,%d0                |   return 1;
    | movm.l (%sp)+,%d2-%d7/%a2-%a5
    | rts                         | }
    bra.w drawMapLine

.L50:
    tst.w %d2                   | if (((x1 < 0) && (x2 < 0)) ||
    jge .L53
    tst.w %d4
    jlt .L52

.L53:
    cmp.w %d0,%d2               |     ((x1 >= BMP_WIDTH) && (x2 >= BMP_WIDTH)) ||
    jle .L54
    cmp.w %d0,%d4
    jgt .L52

.L54:
    tst.w %d3                   |     ((y1 < 0) && (y2 < 0)) ||
    jge .L55
    tst.w %d5
    jlt .L52

.L55:
    cmp.w %d1,%d3               |     ((y1 >= BMP_HEIGHT) && (y2 >= BMP_HEIGHT)))
    jle .L60
    cmp.w %d1,%d5
    jle .L60

.L52:
    | moveq #0,%d0                |   return 0;
    movm.l (%sp)+,%d2-%d7/%a2-%a5
	bra.w after_draw_map_line
    | rts







func drawMapLine
    | movem.l %d2-%d7/%a2-%a5,-(%sp)

    | move.l  44(%sp),%a0     | a0 = &line
    | movem.w (%a0)+,%d2-%d6  | d2 = x1, d3 = y1, d4 = x2, d5 = y2, d6 = col
    move.w 8(%a0), %d6

.dl_start:
    moveq   #1,%d0          | d0 = stepx = 1
    move.w  #128,%d1        | d1 = stepy = BMP_PITCH;

    sub.w   %d2,%d4         | d4 = deltax
    jge     .dl_01          | {

    neg.w   %d4             |     deltax = -deltax;
    neg.w   %d0             |     stepx = -stepx;
                            | }
.dl_01:
    sub.w   %d3,%d5         | d5 = deltay
    jge     .dl_02          | {

    neg.w   %d5             |     deltay = -deltay;
    neg.w   %d1             |     stepy = -stepy;
                            | }
.dl_02:                     |
    move.w  %d0,%a2         | a2 = stepx
    move.w  %d1,%a3         | a3 = stepy
    move.w  %d4,%a4         | a4 = dx
    move.w  %d5,%a5         | a5 = dy

    move.l  bmp_buffer_write,%d7
    lsl.w   #7,%d3
    add.w   %d3,%d7         | d7.l = &bmp_buffer_write[y1 * BMP_PITCH]
    move.l  %d7,%a0         | a0 = *dst = &bmp_buffer_write[y1 * BMP_PITCH]

    moveq   #-0x10,%d0      | d0 = mu = 0xF0
    moveq   #0x0F,%d1       | d1 = md = 0x0F
    move.b  %d6,%d7
    and.b   %d0,%d6         | d6 = cu = col & mu
    and.b   %d1,%d7         | d7 = cd = col & md


    cmp.w   %d4,%d5         | if (deltax < deltay)
    jge     .dl_on_dy       | {

.dl_on_dx:
    move.w  %d4,%d3         |   d2 = x
    asr.w   #1,%d3          |   d3 = delta = dx >> 1
                            |   d4 = cnt = dx

.dl_dx_loop:                |   while(cnt--)
    move.w  %d2,%d5         |   {
    lsr.w   #1,%d5          |     d5 = x >> 1
    jcs     .dl_dx_odd      |     if (x & 1)
                            |     {
.dl_dx_even:
    lea     (%a0,%d5.w),%a1 |       a1 = d = dst + (x >> 1)

    move.b  (%a1),%d5
    and.b   %d1,%d5
    or.b    %d6,%d5
    move.b  %d5,(%a1)       |       *d = (*d & md) | cu

    add.w   %a2,%d2         |       x += stepx
    sub.w   %a5,%d3         |       if ((delta -= dy) < 0)
    jpl     .dl_dx_even_1   |       {

    add.w   %a3,%a0         |         dst += stepy; (can be 16 bits as dst is in RAM)
    add.w   %a4,%d3         |         delta += dx;

.dl_dx_even_1:              |       }
    dbra    %d4,.dl_dx_loop |     }

.dl_end:
    movem.l (%sp)+,%d2-%d7/%a2-%a5
	bra.w after_draw_map_line
    | rts
                            |     else
.dl_dx_odd:                 |     {
    lea     (%a0,%d5.w),%a1 |       a1 = d = dst + (x >> 1)

    move.b  (%a1),%d5
    and.b   %d0,%d5
    or.b    %d7,%d5
    move.b  %d5,(%a1)       |       *d = (*d & mu) | cd

    add.w   %a2,%d2         |       x += stepx
    sub.w   %a5,%d3         |       if ((delta -= dy) < 0)
    jpl     .dl_dx_odd_1    |       {

    add.w   %a3,%a0         |           dst += stepy; (can be 16 bits as dst is in RAM)
    add.w   %a4,%d3         |           delta += dx;

.dl_dx_odd_1:               |       }
    dbra    %d4,.dl_dx_loop |     }

    movem.l (%sp)+,%d2-%d7/%a2-%a5
	bra.w after_draw_map_line
    | rts

.dl_on_dy:
    move.w  %d5,%d3         |   d2 = x
    asr.w   #1,%d3          |   d3 = delta = dy >> 1
    move.w  %d5,%d4         |   d4 = cnt = dy

.dl_dy_loop:                |   while(cnt--)
    move.w  %d2,%d5         |   {
    lsr.w   #1,%d5          |     d5 = x >> 1
    jcs     .dl_dy_odd      |     if (x & 1)
                            |     {
.dl_dy_even:
    lea     (%a0,%d5.w),%a1 |       a1 = d = dst + (x >> 1)

    move.b  (%a1),%d5
    and.b   %d1,%d5
    or.b    %d6,%d5
    move.b  %d5,(%a1)       |       *d = (*d & md) | cu

    add.w   %a3,%a0         |       dst += stepy; (can be 16 bits as dst is in RAM)
    sub.w   %a4,%d3         |       if ((delta -= dx) < 0)
    jpl     .dl_dy_even_1   |       {

    add.w   %a2,%d2         |         x += stepx
    add.w   %a5,%d3         |         delta += dy;

.dl_dy_even_1:              |       }
    dbra    %d4,.dl_dy_loop |     }

    movem.l (%sp)+,%d2-%d7/%a2-%a5
	bra.w after_draw_map_line
    | rts
                            |     else
.dl_dy_odd:                 |     {
    lea     (%a0,%d5.w),%a1 |       a1 = d = dst + (x >> 1)

    move.b  (%a1),%d5
    and.b   %d0,%d5
    or.b    %d7,%d5
    move.b  %d5,(%a1)       |       *d = (*d & mu) | cd

    add.w   %a3,%a0         |       dst += stepy; (can be 16 bits as dst is in RAM)
    sub.w   %a4,%d3         |       if ((delta -= dx) < 0)
    jpl     .dl_dy_odd_1    |       {

    add.w   %a2,%d2         |         x += stepx
    add.w   %a5,%d3         |         delta += dy;

.dl_dy_odd_1:               |       }
    dbra    %d4,.dl_dy_loop |     }

    movem.l (%sp)+,%d2-%d7/%a2-%a5
	bra.w after_draw_map_line
    | rts
