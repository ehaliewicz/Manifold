.global project_and_adjust_x
.globl project_and_adjust_y_fix

transform_map_vert_blah:
	move.l 4(%sp), %d0	| d0 is x (16-bit int)
	move.l 8(%sp), %d1 	| d1 is y (16-bit int)
	move.l %d2, -(%sp)
	move.l %d3, -(%sp)
	| move.w sinFix16, %d4
	| move.w cosFix16, %d5 
	sub.w playerXInt, %d0	| d0 is tlx (16-bit int)
	sub.w playerYInt, %d1	| d1 is tly (16-bit int)
	move.w %d0, %d2			| d0, d2 is tlx
	move.w %d1, %d3 		| d1, d3 is tly

	muls.w angleSin16, %d0	| d0 is tlx*sinFix16
	muls.w angleSin16, %d1	| d1 is tly*sinFix16
	muls.w angleCos16, %d2	| d2 is tlx*cosFix16
	muls.w angleCos16, %d3	| d3 is tly*cosFix16


	move.l (%sp)+, %d3
	move.l (%sp)+, %d2
	rts

project_and_adjust_x:
	move.l 4(%sp), %d0  | d0 is rx (16.0)
	move.l 8(%sp), %d1 	| d1 is rz (16.0)
	| muls.w #60,%d0      | d0 is rx*80, 32 bits
    muls.w #32, %d0 	
	divs.w %d1, %d0     | d0 here is 16rem:16:quo of (rx*80)/rz
    | add.w #64, %d0
    add.w #32, %d0
	rts

project_and_adjust_y_fix:
	move.l 4(%sp), %d0
	move.l 8(%sp), %d1
	sub.w playerZ12Frac4, %d0 
	muls.w #72, %d0					| d0 = 80(wall_y-player_y) (12.4)
	divs.w %d1, %d0					| d0 = 16.mod/16.quo y/z (12.4 in lower word)
	move.w #1136, %d1
	sub.w %d0, %d1
	move.w %d1, %d0
	rts