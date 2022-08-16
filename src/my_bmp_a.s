	.globl bmp_clear_vertical_bitmap_buffer
	.type  bmp_clear_vertical_bitmap_buffer, @function
bmp_clear_vertical_bitmap_buffer:
	move.l 4(%sp),%a0           | a0 = buffer
    lea 18432(%a0),%a0          | a0 = buffer end

    movm.l %d2-%d7/%a2-%a6,-(%sp)

    | the function consumes about 36000? cycles to clear the whole bitmap buffer

    moveq #0,%d1
    move.l %d1,%d2
    move.l %d1,%d3
    move.l %d1,%d4
    move.l %d1,%d5
    move.l %d1,%d6
    move.l %d1,%d7
    move.l %d1,%a1
    move.l %d1,%a2
    move.l %d1,%a3
    move.l %d1,%a4
    move.l %d1,%a5
    move.l %d1,%a6

    moveq #34,%d0

.L000:
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    movm.l %d1-%d7/%a1-%a6,-(%a0) 
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    dbra %d0,.L000
	
	
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    movm.l %d1-%d7/%a1-%a6,-(%a0)
	
	movm.l %d1-%d6,-(%a0)

    movm.l (%sp)+,%d2-%d7/%a2-%a6
    rts

.globl bmp_clear_bitmap_buffer
.type  bmp_clear_bitmap_buffer, @function
bmp_clear_bitmap_buffer:
    move.l 4(%sp),%a0           | a0 = buffer
    lea 20480(%a0),%a0          | a0 = buffer end

    movm.l %d2-%d7/%a2-%a6,-(%sp)

    | the function consume about 43200 cycles to clear the whole bitmap buffer

    moveq #0,%d1
    move.l %d1,%d2
    move.l %d1,%d3
    move.l %d1,%d4
    move.l %d1,%d5
    move.l %d1,%d6
    move.l %d1,%d7
    move.l %d1,%a1
    move.l %d1,%a2
    move.l %d1,%a3
    move.l %d1,%a4
    move.l %d1,%a5
    move.l %d1,%a6

    moveq #38,%d0

.L01:
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    dbra %d0,.L01

    movm.l %d1-%d7/%a1-%a6,-(%a0)
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    movm.l %d1-%d7/%a1-%a6,-(%a0)
    movm.l %d1-%d7/%a1-%a4,-(%a0)

    movm.l (%sp)+,%d2-%d7/%a2-%a6
    rts