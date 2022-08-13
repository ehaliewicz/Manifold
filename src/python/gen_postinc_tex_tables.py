HEADER_TOP_CLIP = """
    | tex_column in a0, col_ptr in a1, top_clip in d0
    lsl.l #2, %d0
    jmp scale_64_{}_jump(%pc, %d0.w)
"""
HEADER_BOT_CLIP = """
    | tex_column in a0, col_ptr in a1, top_clip in d0, bot_clip in d1
    add.l %d1, %d0 | d0 = clip_top+clip_bot
    lsl.l #2, %d0 | d0 = (clip_top+clip_bot)<<=2
    move.l %a0, -(%sp) | stash tex_column for now
    move.l #skip_64_{}, %a0
    move.b 0(%a0, %d1.w), %d1 | d1 = texels to skip backwards
    ext.w %d1 |ext.w %d1 | andi.w #255, %d1
    move.l (%sp)+, %a0
    sub.l %d1, %a0
    jmp scale_64_{}_jump(%pc, %d0.w)

"""


"""
for y in range(513):
  print(".globl scale_64_{}_top_clip".format(y))
  print(".globl scale_64_{}_bot_clip".format(y))

  
  print("scale_64_{}_top_clip:".format(y))
  if y == 0:
    print("rts")
  else:
    print(HEADER_TOP_CLIP.format(y))

  print("scale_64_{}_bot_clip:".format(y))
  if y == 0:
    print("rts")
  else:
    print(HEADER_BOT_CLIP.format(y, y))

  if y == 0:
    continue  

  du_per_dy = 64/y
  scaled = du_per_dy <= 1
  cur_u = 0

  print("scale_64_{}_jump:".format(y))
  if 0: #scaled:

    
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
      if int_u == 0:
        print("dc.w 0b0011001011101000, 0b0000000000000000")
      else:
        print("move.w {}(%a0), (%a1)+".format(2*int_u))
  print("rts")

"""

  #print(".global skip_64_{}".format(y))
  #print("skip_64_{}:".format(y))
  #for yy in range(y):
  #  int_u = int(cur_u)
  #  cur_u += du_per_dy
  #  print("dc.w {}".format(int_u))


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

#for y in range(513):
#  print("extern void scale_64_{}_top_clip(void);".format(y))
#  print("extern void scale_64_{}_bot_clip(void);".format(y))


#print("jump_table_top_clip_lut[513] = {")
#for y in range(1, 513):
#  print("scale_64_{}_top_clip, ".format(y), end="")
#  if y % 8 == 0:
#    print("")
#print("}")

print("jump_table_bot_clip_lut[513] = {")
for y in range(1, 513):
  print("scale_64_{}_bot_clip, ".format(y), end="")
  if y % 8 == 0:
    print("")
print("}")