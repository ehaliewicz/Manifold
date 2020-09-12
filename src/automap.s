.global render_blockmap_cell, transform_vert_native




transform_vert_native:
	movm.l %d2-%d4,-(%sp)
	move.w 12+4+2(%sp),%d0
	move.w 12+8+2(%sp),%d2
	lsl.w #2,%d0
	lsl.w #2,%d2
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

	.macro PUSHL reg
	move.l \reg, -(%sp)
	.endm

	.macro POPL reg
	move.l (%sp)+, \reg
	.endm
	
	.macro TRANSFORM_VERT
	PUSH %d5
	| expects vertex x in d1, and vertex y in d3
	| leaves transformed vertex in d1 (transformed_x<<16 | transformed_y)
	lsl.w #2, %d1
	lsl.w #2, %d3
	sub.w playerXFrac4,%d1
	sub.w playerYFrac4,%d3
	move.w %d1,%d2		| d1 = d2 = tlx
	move.w %d3,%d4		| d3 = d4 = tly
	move.w angleCosFrac12,%d5
	muls.w %d5,%d1		| d1 = tlx * angleCosFrac12
	muls.w %d5,%d4		| d3 = tly * angleCosFrac12
	move.w angleSinFrac12,%d4
	muls.w %d5,%d2		| d2 = tlx * angleSinFrac12
	muls.w %d5,%d3		| d3 = tly * angleSinFrac12

	sub.l %d3,%d1 		| d1 = (tlx*angleCosFrac12)-(tly*angleSinFrac12)
	add.w %d4,%d2		| d2 = (tlx*angleSinFrac12)+(tly*angleCosFrac12)

	swap %d1
	add.w #128, %d1
	swap %d1
	swap %d2
	moveq #70,%d3
	sub.w %d2, %d3
	move.w %d3, %d1

	POP %d5
	.endm
	

line:
line_pt1_x:
	dc.w 0
line_pt1_y:
	dc.w 0
line_pt2_x:
	dc.w 0
line_pt2_y:
	dc.w 0
line_col:
	dc.b 0

	| a0 is pointer to cell information
draw_blockmap_cell:
	| load number of linedefs
	move.w (%a0)+, %d0

	move #$FFFF, %d5  	| load invalid vertex indexes into d5 (vertex idx register)
	moveq #0, %d6		| clear prev vertex register
	moveq #0, %d7		| clear prev prev vertex register
	lea #processed_linedef_bitmap, %a1
	
draw_blockmap_linedef_loop:

	move.w (%a0)+, %d1		| d1 is linedef byte idx
	move.b (%a0)+, %d2		| d2 is bit mask
	and.b 0(%a1,%d1.w), %d2 	| test bit in byte within linedef_processed_bitmap
	beq.b byte_or_bit_was_zero

skip_linedef:
	addq.l #6	| skip past v1_idx, v1_x, v1_y
	addq.l #6	| skip past v2_idx, v2_x, v2_y
	bra.b draw_blockmap_linedef_loop_test

byte_or_bit_was_zero:
	or.b %d2, 0(%a1,%d1.w) | 18 cycles, or bitmask into byte for this linedef (in linedef_processed_bitmap)

	move.b (%a0)+, (line_col)	| move line color into color slot in line struct
	
	

draw_blockmap_linedef_loop_test
	dbf %d0, draw_blockmap_linedef_loop
	

