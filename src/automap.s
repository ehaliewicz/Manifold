.global render_blockmap_cell, transform_vert_native
.extern asm_x_shifted, asm_y_shifted, asm_tlx, asm_tly
.global trans_vert_x, trans_vert_y

trans_vert_x:
    dc.w 0
trans_vert_y:
    dc.w 0
d2_stash:
    dc.l 0
d3_stash:
    dc.l 0
d4_stash:
    dc.l 0
d5_stash:
    dc.l 0

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

render_blockmap_cell:


