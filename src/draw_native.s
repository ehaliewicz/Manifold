.globl draw_native_vertical_line, draw_native_vertical_line_unrolled_inner, vline_native_dither_movep

 .macro STACK_REF_ARG_LONG num
    4+(4*num)(sp)
 .endm

 .macro STACK_REF_ARG_WORD num 
    4+2+(4*num)(sp)
 .endm

 .macro STACK_REF_ARG_BYTE num 
    4+3+(4*num)(sp)
 .endm


| args y0, y1, col, col_ptr

draw_native_vertical_line:
    move.b  4+3(%sp), %d0    | d0 = y0
    move.b  8+3(%sp), %d1    | d1 = y1
    sub.b %d0, %d1           | d1 = y1-y0 = cnt

    asl.w #7, %d0            | d0 = y0*2
    move.l 16(%sp), %a0      | a0 = col_ptr
    add.l %d0, %a0           | a0 = col_ptr + y0*2

    move.b 12+3(%sp), %d0
    subq #1, %d1
draw_native_vertical_line_lp:
    move.b %d0, (%a0)
    add.l #128, %a0
    dbeq %d1, draw_native_vertical_line_lp
    rts


vline_native_dither_movep:
	| vline_native_dither_movep(u8* buf, s16 dy, u32 col1_col2) 
	move.l 4(%sp), %a0		| load pointer
    moveq.l #0, %d0
    move.w 10(%sp), %d0
    move.l 12(%sp), %d1

    andi.w #3, %d0
    beq after_edge_loop
    subq.b #1, %d0
edge_loop:
    move.b %d1, (%a0)
    addq.l #2, %a0
    ror.l #8, %d1
    dbeq %d0, edge_loop

after_edge_loop:
    
    moveq.l #0, %d0
    move.w 10(%sp), %d0
    andi.w #65532, %d0
    beq exit

    lsr.l #2, %d0
    neg.w %d0
    add.w #36, %d0
    lsl.w #2, %d0
    jmp dither_draw_table_movep(%pc, %d0.W)


dither_draw_table_movep:
    | movep.l %d1, 312(%a0)
    | movep.l %d1, 304(%a0)
    | movep.l %d1, 296(%a0)
    | movep.l %d1, 288(%a0)
    movep.l %d1, 280(%a0)
    movep.l %d1, 272(%a0)
    movep.l %d1, 264(%a0)
    movep.l %d1, 256(%a0)
    movep.l %d1, 248(%a0)
    movep.l %d1, 240(%a0)
    movep.l %d1, 232(%a0)
    movep.l %d1, 224(%a0)
    movep.l %d1, 216(%a0)
    movep.l %d1, 208(%a0)
    movep.l %d1, 200(%a0)
    movep.l %d1, 192(%a0)
    movep.l %d1, 184(%a0)
    movep.l %d1, 176(%a0)
    movep.l %d1, 168(%a0)
    movep.l %d1, 160(%a0)
    movep.l %d1, 152(%a0)
    movep.l %d1, 144(%a0)
    movep.l %d1, 136(%a0)
    movep.l %d1, 128(%a0)
    movep.l %d1, 120(%a0)
    movep.l %d1, 112(%a0)
    movep.l %d1, 104(%a0)
    movep.l %d1, 96(%a0)
    movep.l %d1, 88(%a0)
    movep.l %d1, 80(%a0)
    movep.l %d1, 72(%a0)
    movep.l %d1, 64(%a0)
    movep.l %d1, 56(%a0)
    movep.l %d1, 48(%a0)
    movep.l %d1, 40(%a0)
    movep.l %d1, 32(%a0)
    movep.l %d1, 24(%a0)
    movep.l %d1, 16(%a0)
    movep.l %d1, 8(%a0)
    movep.l %d1, 0(%a0)
exit:
	rts
    
    | draw_native_vertical_line_unrolled_inner(jump_table_offset, col, col_ptr);
draw_native_vertical_line_unrolled_inner:


    move.w 4+2(%sp), %d1
    move.b 8+3(%sp), %d0
    move.l 12(%sp), %a0
    jmp jump_table(%pc, %d1.w)

jump_table:
    move.b %d0, 159*2(%a0)
    move.b %d0, 158*2(%a0)
    move.b %d0, 157*2(%a0)
    move.b %d0, 156*2(%a0)
    move.b %d0, 155*2(%a0)
    move.b %d0, 154*2(%a0)
    move.b %d0, 153*2(%a0)
    move.b %d0, 152*2(%a0)
    move.b %d0, 151*2(%a0)
    move.b %d0, 150*2(%a0)
    move.b %d0, 149*2(%a0)
    move.b %d0, 148*2(%a0)
    move.b %d0, 147*2(%a0)
    move.b %d0, 146*2(%a0)
    move.b %d0, 145*2(%a0)
    move.b %d0, 144*2(%a0)
    move.b %d0, 143*2(%a0)
    move.b %d0, 142*2(%a0)
    move.b %d0, 141*2(%a0)
    move.b %d0, 140*2(%a0)
    move.b %d0, 139*2(%a0)
    move.b %d0, 138*2(%a0)
    move.b %d0, 137*2(%a0)
    move.b %d0, 136*2(%a0)
    move.b %d0, 135*2(%a0)
    move.b %d0, 134*2(%a0)
    move.b %d0, 133*2(%a0)
    move.b %d0, 132*2(%a0)
    move.b %d0, 131*2(%a0)
    move.b %d0, 130*2(%a0)
    move.b %d0, 129*2(%a0)
    move.b %d0, 128*2(%a0)
    move.b %d0, 127*2(%a0)
    move.b %d0, 126*2(%a0)
    move.b %d0, 125*2(%a0)
    move.b %d0, 124*2(%a0)
    move.b %d0, 123*2(%a0)
    move.b %d0, 122*2(%a0)
    move.b %d0, 121*2(%a0)
    move.b %d0, 120*2(%a0)
    move.b %d0, 119*2(%a0)
    move.b %d0, 118*2(%a0)
    move.b %d0, 117*2(%a0)
    move.b %d0, 116*2(%a0)
    move.b %d0, 115*2(%a0)
    move.b %d0, 114*2(%a0)
    move.b %d0, 113*2(%a0)
    move.b %d0, 112*2(%a0)
    move.b %d0, 111*2(%a0)
    move.b %d0, 110*2(%a0)
    move.b %d0, 109*2(%a0)
    move.b %d0, 108*2(%a0)
    move.b %d0, 107*2(%a0)
    move.b %d0, 106*2(%a0)
    move.b %d0, 105*2(%a0)
    move.b %d0, 104*2(%a0)
    move.b %d0, 103*2(%a0)
    move.b %d0, 102*2(%a0)
    move.b %d0, 101*2(%a0)
    move.b %d0, 100*2(%a0)
    move.b %d0, 99*2(%a0)
    move.b %d0, 98*2(%a0)
    move.b %d0, 97*2(%a0)
    move.b %d0, 96*2(%a0)
    move.b %d0, 95*2(%a0)
    move.b %d0, 94*2(%a0)
    move.b %d0, 93*2(%a0)
    move.b %d0, 92*2(%a0)
    move.b %d0, 91*2(%a0)
    move.b %d0, 90*2(%a0)
    move.b %d0, 89*2(%a0)
    move.b %d0, 88*2(%a0)
    move.b %d0, 87*2(%a0)
    move.b %d0, 86*2(%a0)
    move.b %d0, 85*2(%a0)
    move.b %d0, 84*2(%a0)
    move.b %d0, 83*2(%a0)
    move.b %d0, 82*2(%a0)
    move.b %d0, 81*2(%a0)
    move.b %d0, 80*2(%a0)
    move.b %d0, 79*2(%a0)
    move.b %d0, 78*2(%a0)
    move.b %d0, 77*2(%a0)
    move.b %d0, 76*2(%a0)
    move.b %d0, 75*2(%a0)
    move.b %d0, 74*2(%a0)
    move.b %d0, 73*2(%a0)
    move.b %d0, 72*2(%a0)
    move.b %d0, 71*2(%a0)
    move.b %d0, 70*2(%a0)
    move.b %d0, 69*2(%a0)
    move.b %d0, 68*2(%a0)
    move.b %d0, 67*2(%a0)
    move.b %d0, 66*2(%a0)
    move.b %d0, 65*2(%a0)
    move.b %d0, 64*2(%a0)
    move.b %d0, 63*2(%a0)
    move.b %d0, 62*2(%a0)
    move.b %d0, 61*2(%a0)
    move.b %d0, 60*2(%a0)
    move.b %d0, 59*2(%a0)
    move.b %d0, 58*2(%a0)
    move.b %d0, 57*2(%a0)
    move.b %d0, 56*2(%a0)
    move.b %d0, 55*2(%a0)
    move.b %d0, 54*2(%a0)
    move.b %d0, 53*2(%a0)
    move.b %d0, 52*2(%a0)
    move.b %d0, 51*2(%a0)
    move.b %d0, 50*2(%a0)
    move.b %d0, 49*2(%a0)
    move.b %d0, 48*2(%a0)
    move.b %d0, 47*2(%a0)
    move.b %d0, 46*2(%a0)
    move.b %d0, 45*2(%a0)
    move.b %d0, 44*2(%a0)
    move.b %d0, 43*2(%a0)
    move.b %d0, 42*2(%a0)
    move.b %d0, 41*2(%a0)
    move.b %d0, 40*2(%a0)
    move.b %d0, 39*2(%a0)
    move.b %d0, 38*2(%a0)
    move.b %d0, 37*2(%a0)
    move.b %d0, 36*2(%a0)
    move.b %d0, 35*2(%a0)
    move.b %d0, 34*2(%a0)
    move.b %d0, 33*2(%a0)
    move.b %d0, 32*2(%a0)
    move.b %d0, 31*2(%a0)
    move.b %d0, 30*2(%a0)
    move.b %d0, 29*2(%a0)
    move.b %d0, 28*2(%a0)
    move.b %d0, 27*2(%a0)
    move.b %d0, 26*2(%a0)
    move.b %d0, 25*2(%a0)
    move.b %d0, 24*2(%a0)
    move.b %d0, 23*2(%a0)
    move.b %d0, 22*2(%a0)
    move.b %d0, 21*2(%a0)
    move.b %d0, 20*2(%a0)
    move.b %d0, 19*2(%a0)
    move.b %d0, 18*2(%a0)
    move.b %d0, 17*2(%a0)
    move.b %d0, 16*2(%a0)
    move.b %d0, 15*2(%a0)
    move.b %d0, 14*2(%a0)
    move.b %d0, 13*2(%a0)
    move.b %d0, 12*2(%a0)
    move.b %d0, 11*2(%a0)
    move.b %d0, 10*2(%a0)
    move.b %d0, 9*2(%a0)
    move.b %d0, 8*2(%a0)
    move.b %d0, 7*2(%a0)
    move.b %d0, 6*2(%a0)
    move.b %d0, 5*2(%a0)
    move.b %d0, 4*2(%a0)
    move.b %d0, 3*2(%a0)
    move.b %d0, 2*2(%a0)
    move.b %d0, 1*2(%a0)
    move.b %d0, 0*2(%a0)
    rts

