.globl draw_native_vertical_line_unrolled_inner, vline_native_dither_movep

.global vline_native_movel


| void vline_native_movel(s16 dy, u8 col, u8* col_ptr);
vline_native_movel:
    move.l #79, %d0
    sub.l 4(%sp), %d0    | 
    move.l 8(%sp), %d1
    move.l 12(%sp), %a0
    lsl.l #1, %d0

    jmp movel_draw_table(%pc, %d0.W)

movel_draw_table:
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)+
    move.l %d1, (%a0)
    rts



vline_native_dither_movep:
	| vline_native_dither_movep(u8* buf, u8 extra_pix, s16 jump_table_offset, u32 col1_col2) 
	move.l 4(%sp), %a0		| load pointer
    move.l 16(%sp), %d1     | load color

    moveq.l #0, %d0
    move.b 11(%sp), %d0
    beq.b after_edge_loop
    subq.b #1, %d0
edge_loop:
    move.b %d1, (%a0)
    addq.l #2, %a0
    ror.l #8, %d1
    dbeq %d0, edge_loop
after_edge_loop:


    move.w 14(%sp), %d0      | load jump table offset
    | beq exit

    jmp dither_draw_table_movep(%pc, %d0.W)


dither_draw_table_movep:
    movep.l %d1, 312(%a0)
    movep.l %d1, 304(%a0)
    movep.l %d1, 296(%a0)
    movep.l %d1, 288(%a0)
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
    