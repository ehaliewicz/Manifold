SHRINK_HEADER_TOP_CLIP = """

"""
SHRINK_HEADER_BOT_CLIP = """
scale_64_{}:	
	move.l a2, -(%sp) ; stash a2 
	
	move.l 20(%sp), %a0         | a0 = clip_bot
	move.l %a0, %a2             | a0 and a2 are clip_bot
	add.l  16(%sp), %a2         | a2 is clip_bot + clip_top
	lsl.l #2, %a2               | a2 is (clip_bot+clip_top)<<2
	moveq #0, %d0			    | d0 = 0
	move.l 12(%sp), %a0 	    | a0 = tex_column
	move.b skip_64_32(%a0), %d0 | d0 = skip_64_32[clip_bot]
	sub.l %d0, %a0			    | tex_column -= skip_64_32[clip_bot]
	
    move.l 8(%sp), %a1          | a1 = col_ptr
	jmp jump_stuff(%a2)
"""
SCALE_HEADER_TOP_CLIP = """

"""
SCALE_HEADER_BOT_CLIP = """

"""


for y in range(1,513):
  du_per_dy = 64/y
  scaled = du_per_dy <= 1
  cur_u = 0
  
  print(".globl scale_64_{}_top_clip".format(y))
  print(".globl scale_64_{}_bot_clip".format(y))
  
  if scaled:
    print("scale_64_{}_top_clip:")
    print(SCALE)
    

  print("scale_64_{}_jump:".format(y))
  if scaled:

    
    for yy in range(y):
      int_u = int(cur_u)
      cur_u += du_per_dy
      next_int_u = int(cur_u)
      if next_int_u > int_u+1:
        raise Exception("wtf")
      elif next_int_u == int_u+1:
        print("    move.w (%a0)+, (%a1)+")
      else:
        print("    move.w 0(%a0), (%a1)+")
  else:
    
    for yy in range(y):
      int_u = int(cur_u)
      cur_u += du_per_dy
      print("move.w {}(%a0), (%a1)+".format(2*int(skip*du_dy)))
  print("move.l %a1, %d0")
  print("rts")

"""
  print(".global skip_64_{}".format(y))
  print("skip_64_{}:".format(y))
  for yy in range(y):
    int_u = int(cur_u)
    cur_u += du_per_dy
    print("dc.w {}".format(int_u))
"""

"""
for y in range(1,513):  
    print("const u8 skip_64_{}[{}] = ".format(y, y) + "{")
    du_dy = 64/y
    for skip in range(y):
        print("{}, ".format(2*int(skip*du_dy)), end="")
        if skip % 40 == 0:
            print("")
    print("};")
"""

#for y in range(1, 513):
#    print("skip_64_{}, ".format(y), end="")
#
#    if y % 20 == 0:
#        print("")
