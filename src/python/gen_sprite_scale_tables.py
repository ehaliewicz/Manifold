base = 64

cnt = 0

"""
# size of run lengths of M, for 64-texel bitmap scaled to N pixels
print("{")
for scaled_size in range(0,513):
  scale_factor = (scaled_size)/64
  for run_length in range(64):
    scaled_run_length = int((run_length)*65536*scale_factor)
    cnt += 1
    print(scaled_run_length, end=", ")
    if cnt % 16 == 0:
      print("")
print("}")

#for i in range(512):
#  # calculate texels per pixel
"""

# number of 
print("u32 sprite_tex_per_pix[513] = {", end='')
for scale in range(513):
  if scale % 32 == 0:
    print('')
  if scale == 0:
    tex_per_pix = 0
  else:
    tex_per_pix = int((64*65536)/scale)
  
  print("{},".format(tex_per_pix), end='')
print("};")