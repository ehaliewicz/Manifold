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

	move.l 8+44(%sp), %a3		| load cell pointer into a0
	move.l 12+44(%sp), %d6		| load cur frame in d6
	
	move.l %a2, -(%sp)			| push line pointer onto stack for BMP_clipLine/BMP_drawLine

	move.l (vertex_cache_frames), %a5	   | load vertex cached frames into a5
	move.l (cached_vertexes), %a6		   | load vertex cache into a6
	move.l (processed_linedef_bitmap), %a4 | load linedef bitmap into a4

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
	jsr BMP_clipLine

	| returns a result in d0, 0 if offscreen
	tst.w %d0							
	beq.b skip_draw_line

	| clip line modifies it's argument so we can just call drawLine right away afterwards
	jsr BMP_drawLine
skip_draw_line:

draw_blockmap_linedef_loop_test:
	dbf %d5, draw_blockmap_cell_loop

	addq.l #4, %sp 					| pop line pointer off stack

    movm.l (%sp)+,%d2-%d7/%a2-%a6
	rts
