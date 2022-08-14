# generate tables to update in-RAM sprite scaling routine
names = []
MAX_SIZE = 512
for output_size in range(MAX_SIZE+1): #513):
    name = "sprite_scale_{}_table".format(output_size)
    names.append(name)

    print("const u16 {}[{}] = ".format(name, output_size) + "{", end='')

    scale = output_size/64
    if output_size == 0:
        texels_per_pixel = 0
    else:
        texels_per_pixel = 64/output_size

    cur_texel = 0
    for i in range(output_size):
        if i % 32 == 0:
            print("")
        print("{},".format(int(cur_texel)), end='')
        cur_texel += texels_per_pixel
        
    print("\n};")

print("const u16* sprite_scale_routine_pointer_lut[{}] = ".format(len(names)) + "{", end="")
for y in range(MAX_SIZE+1):
    if y % 4 == 0:
        print("")
    print("{}, ".format(names[y]), end='')
print("};")

