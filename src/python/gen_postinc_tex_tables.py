for y in range(1,513):
  du_per_dy = 64/y
  optimized = du_per_dy <= 1
  cur_u = 0
  """
  if optimized:

    #print(".globl scale_64_{}".format(y))
    #print("scale_64_{}:".format(y))
    
    for yy in range(y):
      int_u = int(cur_u)
      cur_u += du_per_dy
      next_int_u = int(cur_u)
      if next_int_u > int_u+1:
        raise Exception("wtf")
      elif next_int_u == int_u+1:
        #print("    move.w (%a0)+, (%a1)+")
        print("dc.w {}".format(int_u))
      else:
        #print("    move.w 0(%a0), (%a1)+")
    """

"""
  print(".global skip_64_{}".format(y))
  print("skip_64_{}:".format(y))
  for yy in range(y):
    int_u = int(cur_u)
    cur_u += du_per_dy
    print("dc.w {}".format(int_u))
"""

for y in range(1,513):  
    print("const u8 skip_64_{}[{}] = ".format(y, y) + "{")
    du_dy = 64/y
    for skip in range(y):
        print("{}, ".format(int(skip*du_dy)), end="")
        if skip % 40 == 0:
            print("")
    print("};")

#for y in range(1, 513):
#    print("skip_64_{}, ".format(y), end="")
#
#    if y % 20 == 0:
#        print("")
