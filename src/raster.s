.globl interpolate_loop, end_of_interpolate_loop


    |  calculate 16-bit fractional 1/z, and u/z for every two columns
    |  plus 16-bit integer y1, y2, y3, and y4 for each column of a wall that has both lower and upper steps

    |  walls with only lower or upper steps, or none at all use simpler routines, this is the worst case

    |  this is an unrolled loop, so any 1-pixelcolumn or odd-length walls need to be addressed before this point

    |  also, this routine must be in RAM, is it has a couple immediates that need to be patched at runtime

    |  d1 is d(1/z)  (plus low byte of d(u/z) in top byte)
    |  d2 is 1/z     (plus low byte of u/z in top byte)
    |  d3 is d(u/z)  (plus low byte of dy1 in top byte)
    |  d4 is u/z     (plus low byte of y1 in top byte)

interpolate_loop:
    |   the constant here should be patched with ((d(1/z)&0xFF)<<24) | 0xFFFF
    |   0xFFFF here will decrement count of remaining columns in the low word of d0 (pre-divided by 2, since this is unrolled once)
    add.l #0xDEADBEEF, %d0
    tst.w %d0
    beq end_of_interpolate_loop


    addx.l %d1, %d2        |  1/z += d(1/z), plus carry from the previous add of the fractional bits
    addx.l %d3, %d4        |  u/z += d(u/z), plus carry from the previous add of the fractional bits

    move.w %d2, -(%a6)    |  write 1/z to buffer
    move.w %d4, -(%a6)    |  write u/z to buffer (calculate z and u later on, and lerp for second column)

    |  stash low bytes of y1 (plus high bytes of u/z, which are unused after this point)
    move.l %d4, second_loop_y1_low_byte


    exg %d0, %a0             |  stash x into a0, move dy1 into d0
    exg %d1, %a1         |  stash d(1/z) into a0, move dy2 into d1
    exg %d2, %a2        |  stash 1/z into a2, move dy3 into d2
    exg %d3, %a3        |  stash d(u/z) into a3, move dy4 into d3
    exg %d4, %a4        |  stash u/z into a4, move y1 into d4

    |  d0 through d3 are dy1, dy2, dy3, dy4, plus low bytes of dy2,dy3,dy4 in d0, d1, d2
    |  d4 through d7 are y1, y2, y3, y4, plus low bytes of y2,y3,y4 in d4, d5, d6

    addx.l %d0, %d4        | y1 += dy1
    addx.l %d1, %d5        | y2 += dy2
    addx.l %d2, %d6        | y3 += dy3
    addx.l %d3, %d7        | y4 += dy4

    | stash in buffer
    movem.w %d4/%d5/%d6/%d7, -(%a6)

    | the constant here should be the low byte of dy1
    | this sets up the carry from the fractional part of dy1 and fractional part of y1
    | similar to above, but we no longer have a register free
    add.b #0xFF, second_loop_y1_low_byte
    addx.l %d0, %d4        | y1 += dy1
    addx.l %d1, %d5        | y2 += dy2
    addx.l %d2, %d6        | y3 += dy3
    addx.l %d3, %d7        | y4 += dy4

    | stash in buffer
    movem.w %d4/%d5/%d6/%d7, -(%a6)


    | move x, d(1/z), 1/z, d(u/z) and u/z back into d0 through d4
    exg %d0, %a0
    exg %d1, %a1
    exg %d2, %a2
    exg %d3, %a3
    exg %d4, %a4

    | first byte of second_loop_zero_slot needs to go into top byte of d4
    swap %d4
    rol.l #8, %d4
    move.w second_loop_y1_low_byte, %d4
    swap %d4

    bra interpolate_loop

second_loop_y1_low_byte:
    |   first byte of this will be the low byte of y1
    dc.l 0

end_of_interpolate_loop:

