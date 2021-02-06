.globl draw_native_vertical_line, draw_native_vertical_line_unrolled_inner

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

    asl.w #7, %d0            | d0 = y0*128
    move.l 16(%sp), %a0      | a0 = col_ptr
    add.l %d0, %a0           | a0 = col_ptr + y0*128

    move.b 12+3(%sp), %d0
    subq #1, %d1
draw_native_vertical_line_lp:
    move.b %d0, (%a0)
    add.l #128, %a0
    dbeq %d1, draw_native_vertical_line_lp
    rts


    
    | draw_native_vertical_line_unrolled_inner(jump_table_offset, col, col_ptr);
draw_native_vertical_line_unrolled_inner:

    | move.b  4+3(%sp), %d0    | d0 = y0
    | move.b #127, %d1         | d1 = max_y
    | sub.b  8+3(%sp), %d1     | d1 = max_y-y1
    | add.b %d0, %d1           | d1 = max_y-y1+y0 = start 
    | sub.b %d0, %d1           | d1 = y1-y0 = len
    | sub.b %d1, %d0           | d0 = y0-start = offset
    | lsr.w #7, %d0            | d0 = offset * 128
    | move.l 16(%sp), %a0      | a0 = col_ptr
    | add.l %d0, %a0           | offset pointer

    
    | lsl.w #2, %d1            | scale index by 4 since each instruction is 4 bytes
    | move.b 12+3(%sp), %d0    | d0 = color

    move.w 4+2(%sp), %d1
    move.b 8+3(%sp), %d0
    move.l 12(%sp), %a0
    jmp jump_table(%pc, %d1.w)



jump_table:    
    move.b %d0, 159*128(%a0)
    move.b %d0, 158*128(%a0)
    move.b %d0, 157*128(%a0)
    move.b %d0, 156*128(%a0)
    move.b %d0, 155*128(%a0)
    move.b %d0, 154*128(%a0)
    move.b %d0, 153*128(%a0)
    move.b %d0, 152*128(%a0)
    move.b %d0, 151*128(%a0)
    move.b %d0, 150*128(%a0)
    move.b %d0, 149*128(%a0)
    move.b %d0, 148*128(%a0)
    move.b %d0, 147*128(%a0)
    move.b %d0, 146*128(%a0)
    move.b %d0, 145*128(%a0)
    move.b %d0, 144*128(%a0)
    move.b %d0, 143*128(%a0)
    move.b %d0, 142*128(%a0)
    move.b %d0, 141*128(%a0)
    move.b %d0, 140*128(%a0)
    move.b %d0, 139*128(%a0)
    move.b %d0, 138*128(%a0)
    move.b %d0, 137*128(%a0)
    move.b %d0, 136*128(%a0)
    move.b %d0, 135*128(%a0)
    move.b %d0, 134*128(%a0)
    move.b %d0, 133*128(%a0)
    move.b %d0, 132*128(%a0)
    move.b %d0, 131*128(%a0)
    move.b %d0, 130*128(%a0)
    move.b %d0, 129*128(%a0)
    move.b %d0, 128*128(%a0)
    move.b %d0, 127*128(%a0)
    move.b %d0, 126*128(%a0)
    move.b %d0, 125*128(%a0)
    move.b %d0, 124*128(%a0)
    move.b %d0, 123*128(%a0)
    move.b %d0, 122*128(%a0)
    move.b %d0, 121*128(%a0)
    move.b %d0, 120*128(%a0)
    move.b %d0, 119*128(%a0)
    move.b %d0, 118*128(%a0)
    move.b %d0, 117*128(%a0)
    move.b %d0, 116*128(%a0)
    move.b %d0, 115*128(%a0)
    move.b %d0, 114*128(%a0)
    move.b %d0, 113*128(%a0)
    move.b %d0, 112*128(%a0)
    move.b %d0, 111*128(%a0)
    move.b %d0, 110*128(%a0)
    move.b %d0, 109*128(%a0)
    move.b %d0, 108*128(%a0)
    move.b %d0, 107*128(%a0)
    move.b %d0, 106*128(%a0)
    move.b %d0, 105*128(%a0)
    move.b %d0, 104*128(%a0)
    move.b %d0, 103*128(%a0)
    move.b %d0, 102*128(%a0)
    move.b %d0, 101*128(%a0)
    move.b %d0, 100*128(%a0)
    move.b %d0, 99*128(%a0)
    move.b %d0, 98*128(%a0)
    move.b %d0, 97*128(%a0)
    move.b %d0, 96*128(%a0)
    move.b %d0, 95*128(%a0)
    move.b %d0, 94*128(%a0)
    move.b %d0, 93*128(%a0)
    move.b %d0, 92*128(%a0)
    move.b %d0, 91*128(%a0)
    move.b %d0, 90*128(%a0)
    move.b %d0, 89*128(%a0)
    move.b %d0, 88*128(%a0)
    move.b %d0, 87*128(%a0)
    move.b %d0, 86*128(%a0)
    move.b %d0, 85*128(%a0)
    move.b %d0, 84*128(%a0)
    move.b %d0, 83*128(%a0)
    move.b %d0, 82*128(%a0)
    move.b %d0, 81*128(%a0)
    move.b %d0, 80*128(%a0)
    move.b %d0, 79*128(%a0)
    move.b %d0, 78*128(%a0)
    move.b %d0, 77*128(%a0)
    move.b %d0, 76*128(%a0)
    move.b %d0, 75*128(%a0)
    move.b %d0, 74*128(%a0)
    move.b %d0, 73*128(%a0)
    move.b %d0, 72*128(%a0)
    move.b %d0, 71*128(%a0)
    move.b %d0, 70*128(%a0)
    move.b %d0, 69*128(%a0)
    move.b %d0, 68*128(%a0)
    move.b %d0, 67*128(%a0)
    move.b %d0, 66*128(%a0)
    move.b %d0, 65*128(%a0)
    move.b %d0, 64*128(%a0)
    move.b %d0, 63*128(%a0)
    move.b %d0, 62*128(%a0)
    move.b %d0, 61*128(%a0)
    move.b %d0, 60*128(%a0)
    move.b %d0, 59*128(%a0)
    move.b %d0, 58*128(%a0)
    move.b %d0, 57*128(%a0)
    move.b %d0, 56*128(%a0)
    move.b %d0, 55*128(%a0)
    move.b %d0, 54*128(%a0)
    move.b %d0, 53*128(%a0)
    move.b %d0, 52*128(%a0)
    move.b %d0, 51*128(%a0)
    move.b %d0, 50*128(%a0)
    move.b %d0, 49*128(%a0)
    move.b %d0, 48*128(%a0)
    move.b %d0, 47*128(%a0)
    move.b %d0, 46*128(%a0)
    move.b %d0, 45*128(%a0)
    move.b %d0, 44*128(%a0)
    move.b %d0, 43*128(%a0)
    move.b %d0, 42*128(%a0)
    move.b %d0, 41*128(%a0)
    move.b %d0, 40*128(%a0)
    move.b %d0, 39*128(%a0)
    move.b %d0, 38*128(%a0)
    move.b %d0, 37*128(%a0)
    move.b %d0, 36*128(%a0)
    move.b %d0, 35*128(%a0)
    move.b %d0, 34*128(%a0)
    move.b %d0, 33*128(%a0)
    move.b %d0, 32*128(%a0)
    move.b %d0, 31*128(%a0)
    move.b %d0, 30*128(%a0)
    move.b %d0, 29*128(%a0)
    move.b %d0, 28*128(%a0)
    move.b %d0, 27*128(%a0)
    move.b %d0, 26*128(%a0)
    move.b %d0, 25*128(%a0)
    move.b %d0, 24*128(%a0)
    move.b %d0, 23*128(%a0)
    move.b %d0, 22*128(%a0)
    move.b %d0, 21*128(%a0)
    move.b %d0, 20*128(%a0)
    move.b %d0, 19*128(%a0)
    move.b %d0, 18*128(%a0)
    move.b %d0, 17*128(%a0)
    move.b %d0, 16*128(%a0)
    move.b %d0, 15*128(%a0)
    move.b %d0, 14*128(%a0)
    move.b %d0, 13*128(%a0)
    move.b %d0, 12*128(%a0)
    move.b %d0, 11*128(%a0)
    move.b %d0, 10*128(%a0)
    move.b %d0, 9*128(%a0)
    move.b %d0, 8*128(%a0)
    move.b %d0, 7*128(%a0)
    move.b %d0, 6*128(%a0)
    move.b %d0, 5*128(%a0)
    move.b %d0, 4*128(%a0)
    move.b %d0, 3*128(%a0)
    move.b %d0, 2*128(%a0)
    move.b %d0, 1*128(%a0)
    move.b %d0, 0*128(%a0)
    rts

