base = 64

cnt = 0

for i in range(512):
  scale_factor = (i+1)/64
  for run_length in range(32):
    scaled_run_length = int((run_length+1)*65536*scale_factor)
    cnt += 1
    print(scaled_run_length, end=", ")
    if cnt % 16 == 0:
      print("")