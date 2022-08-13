base = 64

cnt = 0

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
