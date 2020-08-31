.global fire_native, fire_native_dbl, fire_native_quad
.global copy_fire_native

copy_fire_native:
    | d0,d1,a0,d1 already available
    | a0 = src
    | a1 = dstr_ptr
    move.l 4(%sp), %a0 
    move.l 8(%sp), %a1 
    movm.l %d2-%d7/%a2-%a6,-(%sp)

    | 7040 bytes
    lea 7040(%a1),%a1          | a1 = dest end
    lea 7040-48(%a0), %a0      | a0 = src end - 56 bytes

    move.l #72, %d0            | 73 iterations, unrolled once

    
copy_fire_loop:
    
    movm.l (%a0),%d1-%d7/%a2-%a6
    lea    -48(%a0),%a0 
    movm.l %d1-%d7/%a2-%a6,-(%a1)       | 216 cycles for 48 bytes copy
    
    movm.l (%a0),%d1-%d7/%a2-%a6
    lea    -48(%a0),%a0 
    movm.l %d1-%d7/%a2-%a6,-(%a1)       | 216 cycles for 48 bytes copy

    dbf %d0, copy_fire_loop

copy_thirty_two_more_bytes:
    movm.l (%a0), %d1-%d7/%a2
    lea   -32(%a0),%a0
    movm.l %d1-%d7/%a2,-(%a1)

    movm.l (%sp)+, %d2-%d7/%a2-%a6
    rts


fire_native_quad:

    | a0 is src_ptr
    | a1 is dst_ptr

    | a2 is rand_ptr
    | a3 is table_0
    | a4 is table_1
    | a5 is table_2


    | d0 is y 
    | d1 is x 

    | d2 is r
    | d3 is bits
    | d4 is fire
    | d4 is rand_bits
    | d6 is rand_bits_mask
    | d7 is tmp

    movm.l %d2-%d7/%a2-%a5,-(%sp)

    | add 44
    move.l 44(%sp), %a0 
    move.l 48(%sp), %a1 
    move.l 52(%sp), %a2 
    move.l 56(%sp), %a3 
    move.l 60(%sp), %a4 
    move.l 64(%sp), %a5

    moveq.l #0, %d5
    moveq.l #0, %d4 
    moveq.l #0, %d7


    | u16 r = *rptr++;
    | u8 bits = 16;
    move.w (%a2)+, %d2      | prime random bits
    move.b #16, %d3         | set random bits counter

    | const u8 rand_mask = 0b01111;
    move.b #15, %d6          | d6 is rand_bits mask

    move.w #53, %d0         | y counter
    
fire_quad_y_loop:

    move.w #31, %d1         | x counter

fire_quad_x_loop:
    move.b %d2, %d5
    and.b %d6, %d5          | d5 is r & 0b0111111
    
    lsl.w #2, %d5
    jmp quad_jmp_table(%pc, %d5.w)
   

quad_jmp_table:
    bra.w qcase_0
    bra.w qcase_1
    bra.w qcase_2
    bra.w qcase_3
    bra.w qcase_4
    bra.w qcase_5
    bra.w qcase_6
    bra.w qcase_7
    bra.w qcase_8
    bra.w qcase_9
    bra.w qcase_10
    bra.w qcase_11
    bra.w qcase_12
    bra.w qcase_13
    bra.w qcase_14
    bra.w qcase_15


                |case 0b00000:
                |    fire_0 = *src_ptr++;
                |    fire_1 = *src_ptr++;
                |    fire_2 = *src_ptr++;
                |    fire_3 = *src_ptr++;
                |    *(dst_ptr-1) = fire_0;
                |    *((u16*)dst_ptr) = (fire_1 << 8)|fire_2;
                |    dst_ptr += 2; // dst_ptr = 2
                |    *dst_ptr++ = fire_3; // dst_ptr = 3
                |    dst_ptr += 1;
                |    break;
qcase_0:
    subq.l #1, %a1
    move.b (%a0)+, (%a1)+ | *(dst_ptr-1)++ = *src_ptr++    
    move.b (%a0)+, (%a1)+ | extra 
    move.b (%a0)+, (%a1)+ | extra 
    move.b (%a0)+, (%a1)+
    addq.l #1, %a1
    bra.w quad_after_cases
    


                |case 0b00001:
                |    src_ptr += 1;
                |    fire_1 = *src_ptr++;
                |    fire_2 = *src_ptr++;
                |    fire_3 = *src_ptr++;
                |    *((u16*)dst_ptr) = (fire_1 << 8)|fire_2;
                |    dst_ptr += 2; // dst_ptr = 2
                |    *dst_ptr++ = fire_3; // dst_ptr = 3
                |    dst_ptr += 1;
                |    break;
qcase_1:
    addq.l #1, %a0
    move.b (%a0)+, (%a1)+
    move.b (%a0)+, (%a1)+ | extra
    move.b (%a0)+, (%a1)+ | extra

    addq.l #1, %a1
    bra.w quad_after_cases

                |case 0b00010:
                |    src_ptr += 2;
                |    fire_2 = *src_ptr++;
                |    fire_3 = *src_ptr++;
                |    dst_ptr += 1; // dst_ptr = 1
                |    *dst_ptr++ = fire_2; // dst_ptr = 2
                |    *dst_ptr++ = fire_3; // dst_ptr = 3
                |    dst_ptr += 1;
                |    break;

qcase_2:
    addq.l #2, %a0
    addq.l #1, %a1
    move.b (%a0)+, (%a1)+
    move.b (%a0)+, (%a1)+
    addq.l #1, %a1
    bra.w quad_after_cases


                |case 0b00011:
                |    src_ptr += 3;
                |    fire_3 = *src_ptr++;
                |    fire_2 = *src_ptr++;
                |    dst_ptr += 1; // dst_ptr = 1
                |    *dst_ptr++ = fire_2; // dst_ptr = 2
                |    *dst_ptr++ = fire_3; // dst_ptr = 3
                |    dst_ptr += 1;
                |    src_ptr -= 1;
                |    break;

qcase_3:
    addq.l #3, %a0
    move.b (%a0)+, %d4    | "fire_3"
    addq.l #1, %a1 
    move.b (%a0), (%a1)+  | copy "fire_2"
    move.b %d4, (%a1)+     | copy "fire_3"
    addq.l #1, %a1
    bra.w quad_after_cases

    | addq.l #2, %a0
    | addq.l #1, %a1
    | move.b (%a0)+, (%a1)+
    | move.b (%a0)+, (%a1)+
    | addq.l #1, %a1
    | bra.w quad_after_cases


                |case 0b00100:
                |    fire_0 = *src_ptr++;
                |    src_ptr += 2;
                |    fire_3 = *src_ptr++;
                |    *(dst_ptr-1) = fire_0;
                |    dst_ptr += 2; // dst_ptr = 2
                |    *dst_ptr++ = fire_3; // dst_ptr = 3
                |    dst_ptr += 1;
                |    break;
qcase_4:
    subq.l #1, %a1
    move.b (%a0)+, (%a1)+
    addq.l #2, %a0
    addq.l #2, %a1
    move.b (%a0)+, (%a1)+
    addq.l #1, %a1
    bra.w quad_after_cases

                |case 0b00101:
                |    fire_0 = *src_ptr++;
                |    src_ptr += 2;
                |    fire_3 = *src_ptr++;
                |    *dst_ptr++ = table_0[fire_0]; // dst_ptr = 1
                |    dst_ptr += 1; // dst_ptr = 2
                |    *dst_ptr++ = fire_3; // dst_ptr = 3
                |    dst_ptr += 1;
                |    break;

qcase_5:
    move.b (%a0)+, %d4
    move.b 0(%a3, %d4.w), (%a1)+
    addq.l #1, %a1
    addq.l #2, %a0    
    move.b (%a0)+, (%a1)+
    addq.l #1, %a1
    bra.w quad_after_cases

                |case 0b00110:
                |    fire_0 = *src_ptr++;
                |    fire_1 = *src_ptr++;
                |    src_ptr += 1;
                |    fire_3 = *src_ptr++;
                |    dst_ptr += 1; // dst_ptr = 1
                |    *dst_ptr++ = table_1[fire_0]; // dst_ptr = 2
                |    *((u16*)dst_ptr) = (fire_3 << 8)|table_2[fire_1];
                |    dst_ptr += 2;
                |    break;
qcase_6:
    move.b (%a0)+, %d4
    addq.l #1, %a1
    move.b 0(%a4, %d4.w), (%a1)+
    move.b (%a0)+, %d4       | d4 is fire 1
    addq.l #1, %a0
    move.b (%a0)+, (%a1)+    | *dst_ptr++ = fire_3
    move.b 0(%a4, %d4.w), (%a1)+ 
    bra.w quad_after_cases

                |case 0b00111:
                |    src_ptr += 1;
                |    fire_1 = *src_ptr++;
                |    src_ptr += 1
                |    fire_3 = *src_ptr++;
                |    dst_ptr += 2; // dst_ptr = 2
                |    *((u16*)dst_ptr) = (fire_3 << 8)|table_2[fire_1];
                |    dst_ptr += 2;
                |    break;
qcase_7:
                | TODO wtf is going on here with this??
    addq.l #3, %a0
    addq.l #2, %a1 
    move.b (%a0)+, (%a1)+  | copy "fire_3"
    move.b (%a0), %d7     | d7 is "fire_1"
    move.b 0(%a5, %d7.w), (%a1)+  | *dst_ptr++ = table_2[fire_1]
    bra.w quad_after_cases


                |case 0b01000:
                |    fire_0 = *src_ptr++;
                |    fire_1 = *src_ptr++;
                |    src_ptr += 1;
                |    fire_3 = *src_ptr++;
                |    *(dst_ptr-1) = fire_0;
                |    *dst_ptr++ = fire_1; // dst_ptr = 1
                |    dst_ptr += 2; // dst_ptr = 3
                |    *dst_ptr++ = table_0[fire_3]; // dst_ptr = 4
                |    break;
qcase_8:
    subq.l #1, %a1
    move.b (%a0)+, (%a1)+ | copy fire_0
    move.b (%a0)+, (%a1)+ | copy fire_1
    addq.l #1, %a0
    addq.l #2, %a1
    move.b (%a0)+, %d4
    move.b 0(%a3, %d4.w), (%a1)+
    bra.w quad_after_cases




                |case 0b01001:
                |    src_ptr += 3;
                |    fire_3 = *src_ptr++;
                |    src_ptr += 1;
                |    fire_1 = *src_ptr;
                |    *dst_ptr++ = fire_1; // dst_ptr = 1
                |    dst_ptr += 2; // dst_ptr = 3
                |    *dst_ptr++ = table_0[fire_3]; // dst_ptr = 4
                |    break;
qcase_9:
    addq.l #1, %a0
    move.b (%a0)+, (%a1)+
    addq.l #2, %a1
    addq.l #1, %a0
    move.b (%a0)+, %d4
    move.b 0(%a3, %d4.w), (%a1)+
    bra.w quad_after_cases


                |case 0b01010:
                |    src_ptr += 1;
                |    fire_1 = *src_ptr++;
                |    src_ptr += 1;
                |    fire_3 = *src_ptr++;
                |    dst_ptr += 1; // dst_ptr = 1
                |    *dst_ptr++ = table_0[fire_1]; // dst_ptr = 2
                |    dst_ptr += 1; // dst_ptr = 3
                |    *dst_ptr++ = table_0[fire_3]; // dst_ptr = 4
                |    break;

qcase_10:
    addq.l #1, %a0
    move.b (%a0)+, %d4              | d4 is fire_1
    addq.l #1, %a0
    move.b (%a0)+, %d7              | d7 is fire_3
    addq.l #1, %a1
    move.b 0(%a3, %d4.w), (%a1)+
    addq.l #1, %a1
    move.b 0(%a3, %d7.w), (%a1)+
    bra.w quad_after_cases

                |case 0b01011:
                |    fire_0 = *src_ptr++;
                |    fire_1 = *src_ptr++;
                |    src_ptr += 1;
                |    fire_3 = *src_ptr++;
                |    dst_ptr += 1; // dst_ptr = 1
                |    *dst_ptr++ = table_0[fire_1]; // dst_ptr = 2
                |    *dst_ptr++ = table_2[fire_0]; // dst_ptr = 3
                |    *dst_ptr++ = table_0[fire_3]; // dst_ptr = 4
                |    break;
qcase_11:
    move.b (%a0)+, %d4       | d4 is fire_0
    addq.l #1, %a0
    move.b (%a0)+, %d7       | d7 is fire_1
    addq.l #1, %a1
    move.b 0(%a3, %d7.w), (%a1)+
    move.b 0(%a5, %d4.w), (%a1)+
    move.b (%a0)+, %d4
    move.b 0(%a3, %d4.w), (%a1)+
    bra.w quad_after_cases

                |case 0b01100:
                |    fire_0 = *src_ptr++;
                |    fire_1 = *src_ptr++;
                |    fire_2 = *src_ptr++;
                |    fire_3 = *src_ptr++;
                |    *(dst_ptr-1) = fire_0;
                |    dst_ptr += 2; // dst_ptr = 2
                |    *((u16*)dst_ptr) = table_1[fire_1]<<8|table_0[fire_3];
                |    dst_ptr += 2; // dst_ptr = 4
                |    *dst_ptr = table_2[fire_2];
                |    break;
qcase_12:
    subq #1, %a1
    move.b (%a0)+, (%a1)+   | copy fire_0, src_ptr = 1
    addq.l #2, %a1          | dst_ptr = 2
    move.b (%a0)+, %d4      | src_ptr = 2
    move.b 0(%a4, %d4.w), (%a1)+  | dst_ptr = 3
    move.b (%a0)+, %d4      | d4 is fire_2, src_ptr = 3
    move.b (%a0)+, %d7      | d7 is fire_3, src_ptr = 4
    move.b 0(%a3, %d7.w), (%a1)+  | dst_ptr = 4
    move.b 0(%a5, %d4.w), (%a1)
    bra.w quad_after_cases


                |case 0b01101:
                |    fire_0 = *src_ptr++;
                |    fire_1 = *src_ptr++;
                |    fire_2 = *src_ptr++;
                |    fire_3 = *src_ptr++;
                |    *dst_ptr++ = table_0[fire_0]; // dst_ptr = 1
                |    dst_ptr += 1; // dst_ptr = 2
                |    *((u16*)dst_ptr) = table_1_shift[fire_1]|table_0[fire_3];
                |    dst_ptr += 2; // dst_ptr = 4
                |    *dst_ptr = table_2[fire_2];
                |    break;
qcase_13:
    move.b (%a0)+, %d4              | d4 is fire_0
    move.b 0(%a3, %d4.w), (%a1)+   | *dstr_ptr = table_0[fire_0]
    addq.l #1, %a1
    move.b (%a0)+, %d4
    move.b 0(%a4, %d4.w), (%a1)+
    move.b (%a0)+, %d4              | d4 is fire_2
    move.b (%a0)+, %d7              | d7 is fire_3
    move.b 0(%a3, %d7.w), (%a1)+
    move.b 0(%a5, %d4.w), (%a1)
    bra.w quad_after_cases

                |case 0b01110:
                |    fire_0 = *src_ptr++;   // src_ptr = 1
                |    src_ptr += 2;          // src_ptr = 3
                |    fire_3 = *src_ptr++;   // src_ptr = 4
                |    fire_2 = *src_ptr;   // src_ptr = 4
                |    dst_ptr += 1; // dst_ptr = 1
                |    *dst_ptr++ = table_1[fire_0]; // dst_ptr = 2
                |    dst_ptr += 1; // dst_ptr = 3
                |    *dst_ptr++ = table_0[fire_3]; // dst_ptr = 4
                |    *dst_ptr = table_2[fire_2]; // dst_ptr = 4
                |    break;
qcase_14:
    move.b (%a0)+, %d4           | d4 is fire_0, src_ptr = 1
    addq.l #1, %a1               | dst_ptr = 1
    move.b 0(%a4, %d4.w), (%a1)+ | dst_ptr = 2
    addq.l #2, %a0               | src_ptr = 3
    move.b (%a0)+, %d4           | d4 is "fire_3", src_ptr = 4
    addq.l #1, %a1               | dst_ptr = 3
    move.b 0(%a3, %d4.w), (%a1)+ | dst_ptr = 4
    move.b (%a0), %d4            | d4 = "fire_2"
    move.b 0(%a5, %d4.w), (%a1)
    bra.w quad_after_cases



                |case 0b01111:
                |    fire_0 = *src_ptr++;
                |    src_ptr += 2;
                |    fire_3 = *src_ptr++;
                |    fire_2 = *src_ptr;
                |    dst_ptr += 2; // dst_ptr = 2
                |    *((u16*)dst_ptr) = table_2_shift[fire_0]|table_0[fire_3];
                |    dst_ptr += 2; // dst_ptr = 4
                |    *dst_ptr = table_2[fire_2];
                |    break;
qcase_15:
    move.b (%a0)+, %d4             | d4 is fire_0
    addq.l #2, %a0                 | src_ptr += 2
    move.b (%a0)+, %d7             | d7 is fire_3
    addq.l #2, %a1                 | dst_ptr = 2
    move.b 0(%a5, %d4.w), (%a1)+   | *dst_ptr++ = table_2[fire_0]  dst_ptr = 3
    move.b 0(%a3, %d7.w), (%a1)+   | *dst_ptr++ = table_0[fire_3]  dst_ptr = 4
    move.b (%a0), %d4             | d4 is "fire_2"
    move.b 0(%a3, %d4.w), (%a1)   | table_0[fire_3]

quad_after_cases:
    lsr.w #4, %d2             | shift off one random bit
    subq.b #4, %d3

    | r >>= 4;
    | bits -= 4;         
    | if(bits == 0) {
    |   r = *rptr++;
    |   bits = 16;
    | }

    bne.b quad_dont_get_new_rand
quad_get_new_rand:
    move.w (%a2)+, %d2
    move.b #16, %d3

quad_dont_get_new_rand:
    dbf %d1, fire_quad_x_loop


    dbf %d0, fire_quad_y_loop

quad_exit:
    movm.l (%sp)+,%d2-%d7/%a2-%a5

    rts

        


fire_native_dbl:

    | a0 is src_ptr
    | a1 is dst_ptr

    | a2 is rand_ptr
    | a3 is table_0
    | a4 is table_1
    | a5 is table_2


    | d0 is y 
    | d1 is x 

    | d2 is r
    | d3 is bits
    | d4 is fire
    | d4 is rand_bits
    | d6 is rand_bits_mask
    | d7 is tmp

    movm.l %d2-%d7/%a2-%a5,-(%sp)
    | add 44
    move.l 44(%sp), %a0 
    move.l 48(%sp), %a1 
    move.l 52(%sp), %a2 
    move.l 56(%sp), %a3 
    move.l 60(%sp), %a4 
    move.l 64(%sp), %a5

    moveq.l #0, %d5
    moveq.l #0, %d4 
    moveq.l #0, %d7


    move.w (%a2)+, %d2      | prime random bits
    move.b #16, %d3         | set random bits counter

    move.b #3, %d6          | d6 is rand_bits mask

    move.w #53, %d0         | y counter
    
fire_dbl_y_loop:

    move.w #63, %d1         | x counter

fire_dbl_x_loop:
    move.b %d2, %d5
    and.b %d6, %d5          | d5 is r & 0b011
    | bne.b case_1_2_or_3
    
    lsl.w #1, %d5
    jmp dbl_jmp_table(%pc, %d5.w)
   

dbl_jmp_table:
    bra.b case_0
    bra.b case_1
    bra.b case_2
    bra.b case_3

    
    | switch(rand_bits & 0b011) {
    |case 0:
    |    fire_0 = *src_ptr++;
    |    fire_1 = *src_ptr++;
    |    *(dst_ptr-1) = fire_0;
    |    *dst_ptr++ = fire_1; // dst_ptr = 1
    |    dst_ptr += 1;
    |    break;
case_0:
    subq.l #1, %a1
    move.b (%a0)+, (%a1)+
    move.b (%a0)+, (%a1)+
    addq.l #1, %a1
    bra.b after_cases


case_1_2_or_3:
    subq.b #1, %d5
    bne.b case_2_or_3 

    |case 1:
    |    src_ptr += 1;
    |    fire_1 = *src_ptr++;
    |    *dst_ptr++ = fire_1; // dst_ptr = 1
    |    dst_ptr += 1;
    |    break;


case_1:
    addq.l #1, %a0
    move.b (%a0)+, (%a1)+
    addq.l #1, %a1
    bra.b after_cases

case_2_or_3: 
    subq.b #1, %d5
    bne.b case_3

    |case 2:
    |    src_ptr += 1;
    |    fire_1 = *src_ptr++;
    |    dst_ptr += 1; // dst_ptr = 1
    |    *dst_ptr++ = table_0[fire_1]; // dst_ptr = 2
    |    break;
case_2:
    add.l #1, %a0
    move.b (%a0)+, %d4
    add.l #1, %a1
    move.b 0(%a3, %d4.w), (%a1)+
    bra.b after_cases

    |case 3:
    |    fire_0 = *src_ptr++;
    |    fire_1 = *src_ptr++;
    |    dst_ptr += 1; // dst_ptr = 1
    |    *dst_ptr++ = table_0[fire_1]; // dst_ptr = 2
    |    *dst_ptr++ = table_2[fire_0]; // dst_ptr = 3
    |    dst_ptr += -1;
    |    break;
case_3:
    move.b (%a0)+, %d4
    move.b (%a0)+, %d7
    addq.l #1, %a1
    move.b 0(%a3, %d7.w), (%a1)+
    move.b 0(%a3, %d4.w), (%a1)


after_cases:
    lsr.w #2, %d2             | shift off one random bit
    subq.b #2, %d3


    bne.b dbl_dont_get_new_rand
dbl_get_new_rand:
    move.w (%a2)+, %d2
    move.b #16, %d3

dbl_dont_get_new_rand:
    dbf %d1, fire_dbl_x_loop


    dbf %d0, fire_dbl_y_loop

dbl_exit:
    movm.l (%sp)+,%d2-%d7/%a2-%a5

    rts





fire_native:

    | a0 is src_ptr
    | a1 is dst_ptr

    | a2 is bmp_ptr

    | a3 is rand_ptr
    | a4 is byte_fire_lut

    | d0 is y 
    | d1 is x 

    | d2 is r
    | d3 is bits
    | d4 is fire 
    | d5 is rand_bits
    | d6 is rand_bits_mask
    | d7 is 128

    movm.l %d2-%d7/%a2-%a4,-(%sp)
    | add 40 
    move.l 40(%sp), %a0 
    move.l 44(%sp), %a1 
    move.l 48(%sp), %a2 
    move.l 52(%sp), %a3 
    move.l 56(%sp), %a4 

    moveq.l #0, %d5 




    move.w (%a3)+, %d2      | prime random bits
    move.b #16, %d3         | set random bits counter

    move.b #3, %d6          | d6 is rand_bits mask
    move.l #128, %d7        | load framebuffer stride

    move.w #43, %d0         | y counter
fire_y_loop:

    move.w #127, %d1         | x counter
fire_x_loop:

    | d4 is fire/next_fire
    moveq #0, %d4            | clear any leftover bits from shifter
    move.b (%a0)+, %d4       | fire = *src_ptr++
    lsl.w #2, %d4            | fire <<= 2

    move.b %d2, %d5           | rand_bits = r.b 
    and.b %d6, %d5            | rand_bits &= 0b011

    or.b %d5, %d4             | fire = fire<<2 | rand_bits
    move.b 0(%a4, %d4.w), %d4 | next_fire = byte_fire_lut[(fire<<2) | rand_bits]

    move.b %d4, 0(%a1, %d5.w) | *dst_ptr+dst_offset = next_fire
    addq.l #1, %a1            | dst_ptr++

    move.b %d4,  0(%a2,%d5.w) | *framebuffer_ptr++ = next_fire
    addq.l #1, %a2
    move.b %d4, 127(%a2,%d5.w)| *next_line_framebuffer = next_fire

    lsr.w #1, %d2             | shift off one random bit
    subq.b #1, %d3


    bne.b dont_get_new_rand
get_new_rand:
    move.w (%a3)+, %d2
    move.b #16, %d3

dont_get_new_rand:
    dbf %d1, fire_x_loop

    add.l %d7, %a2
    dbf %d0, fire_y_loop









    movm.l (%sp)+,%d2-%d7/%a2-%a4

    rts

