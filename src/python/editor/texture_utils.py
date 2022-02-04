from PIL import Image


def dpix(p):
    return (p << 4) | p


TRANSPARENT_IDX = 0x0
LIGHT_GREEN_IDX = 0x1
LIGHT_RED_IDX = 0x2
LIGHT_BLUE_IDX = 0x3
LIGHT_BROWN_IDX = 0x4
LIGHT_STEEL_IDX = 0x5
GREEN_IDX = 0x6
RED_IDX = 0x7
BLUE_IDX = 0x8
BROWN_IDX = 0x9
STEEL_IDX = 0xA
DARK_GREEN_IDX = 0xB
DARK_RED_IDX = 0xC
DARK_BLUE_IDX = 0xD
DARK_BROWN_IDX = 0xE
DARK_STEEL_IDX = 0xF

dark_col_lut = [
    0, GREEN_IDX, RED_IDX, BLUE_IDX, BROWN_IDX, STEEL_IDX,
    DARK_GREEN_IDX, DARK_RED_IDX, DARK_BLUE_IDX, DARK_BROWN_IDX, DARK_STEEL_IDX,
    DARK_GREEN_IDX, DARK_RED_IDX, DARK_BLUE_IDX, DARK_BROWN_IDX, DARK_STEEL_IDX
]

light_col_lut = [
    0, LIGHT_GREEN_IDX, LIGHT_RED_IDX, LIGHT_BLUE_IDX, LIGHT_BROWN_IDX, LIGHT_STEEL_IDX,
    LIGHT_GREEN_IDX, LIGHT_RED_IDX, LIGHT_BLUE_IDX, LIGHT_BROWN_IDX, LIGHT_STEEL_IDX,
    GREEN_IDX, RED_IDX, BLUE_IDX, BROWN_IDX, STEEL_IDX,
]


def mpix(l, r, next_nib):
    return ((l << 4 | r) << 8) | (r << 4 | next_nib)


def gen_mip_image(name, light_file, mid_file, dark_file):
    conv = []
    lconv = []
    dconv = []

    near_conv = []
    near_lconv = []
    near_dconv = []

    far_conv = []
    far_lconv = []
    far_dconv = []

    mx, my = None, None

    def add_fine_pix(arr, ll, l, r, rr):
        arr.append((ll << 12) | (l << 8) | (r << 4) | rr)

    def add_pix(arr, l, r):
        arr.append((dpix(l) << 8) | dpix(r))

    light_img = Image.open(light_file)
    mid_img = Image.open(mid_file)
    dark_img = Image.open(dark_file)

    light_pix = light_img.load()
    mid_pix = mid_img.load()
    dark_pix = dark_img.load()

    def pix(x, y):
        return light_pix[x, y], mid_pix[x, y], dark_pix[x, y]

    mx, my = light_img.size
    assert light_img.size == mid_img.size
    assert mid_img.size == dark_img.size

    for x in range(0, mx, 4):
        llx, lx, rx, rrx, = x, x + 1, x + 2, x + 3
        for y in range(my):
            light_ll, mid_ll, dark_ll = pix(llx, y)
            light_l, mid_l, dark_l = pix(lx, y)
            light_r, mid_r, dark_r = pix(rx, y)
            light_rr, mid_rr, dark_rr = pix(rrx, y)

            add_fine_pix(far_lconv, light_ll, light_l, light_r, light_rr)
            add_fine_pix(far_conv, mid_ll, mid_l, mid_r, mid_rr)
            add_fine_pix(far_dconv, dark_ll, dark_l, dark_r, dark_rr)

    for x in range(0, mx, 2):
        lx, rx = x, x + 1
        for y in range(my):
            light_l, mid_l, dark_l = pix(lx, y)
            light_r, mid_r, dark_r = pix(rx, y)

            add_pix(lconv, light_l, light_r)
            add_pix(conv, mid_l, mid_r)
            add_pix(dconv, dark_l, dark_r)

    for x in range(mx):
        for y in range(my):
            l, m, d = pix(x, y)

            add_pix(near_lconv, l, l)
            add_pix(near_conv, m, m)
            add_pix(near_dconv, d, d)

    names = []
    for (key, tbl) in [('light', lconv), ('mid', conv), ('dark', dconv),
                       #('light_near', near_lconv), ('mid_near', near_conv), ('dark_near', near_dconv),
                       #('light_far', far_lconv), ('mid_far', far_conv), ('dark_far', far_dconv)
                       ]:
        gen_name = "raw_{}_{}".format(name, key)
        names.append(gen_name)
        print("const u16 {}[{}*{}]".format(gen_name, len(tbl) // 64, 64) + " = {")
        for i, col in enumerate(tbl):
            print("0x{:02x}, ".format(col), end='')
            if i != 0 and i % 64 == 0:
                print("")
        print("\n};\n\n")

    dname = names[2]
    mname = names[1]
    lname = names[0]
    for (level,d,m,l) in [("dark", dname, dname, dname),
                          ("mid_dark", dname, dname, mname),
                          ("mid", dname, mname, lname),
                          ("mid_light", mname, lname, lname),
                          ("light", lname, lname, lname)]:

        print("const lit_texture {}_{} = {}".format(name, level, '{'))
        print("  .dark = {}, .mid = {}, .light = {}".format(d, m, l))
        print("};")



def convert_image(name, file, extra_detail=False):
    conv = []
    lconv = []
    dconv = []

    mx, my = None, None
    with Image.open(file) as img:
        pix = img.load()
        mx, my = img.size
        for x in range(0, mx, 2):
            lx, rx, rrx = x, x + 1, x + 2
            for y in range(my):
                light_l = pix[lx, y]
                mid_l = dark_col_lut[light_l]
                dark_l = dark_col_lut[mid_l]

                light_r = pix[rx, y]
                mid_r = dark_col_lut[light_r]
                dark_r = dark_col_lut[mid_r]
                if rrx >= mx:
                    light_rr = light_r
                else:
                    light_rr = pix[rrx, y]

                mid_rr = dark_col_lut[light_rr]
                dark_rr = dark_col_lut[mid_rr]

                if extra_detail:
                    conv.append(mpix(mid_l, mid_r, mid_rr))
                    dconv.append(mpix(dark_l, dark_r, dark_rr))
                    lconv.append(mpix(light_l, light_r, light_rr))
                else:
                    conv.append((dpix(mid_l) << 8) | dpix(mid_r))
                    dconv.append((dpix(dark_l) << 8) | dpix(dark_r))
                    lconv.append((dpix(light_l) << 8) | dpix(light_r))

    for (key, tbl) in [('light', lconv), ('mid', conv), ('dark', dconv)]:
        print("const u16 {}_{}[{}*{}] = ".format(name, key, mx // 2, my) + " {")
        for i, col in enumerate(tbl):
            print("0x{:02x}, ".format(col), end='')
            if i != 0 and i % 64 == 0:
                print("")
        print("\n};\n\n")


def gen_byte_tables(max_scaled_y, tex_size, asm_file, c_file):
    tbl_size = 0
    jump_tables = {}
    for scaled in range(0, max_scaled_y + 1):
        if scaled != 0:
            dv_dy = tex_size / scaled
        jump_tables[scaled] = []
        prev_v = 0
        for y in range(scaled):
            cur_v = int(dv_dy * y)
            if cur_v == 0:
                s = "dc.w 0b0001000000101000, 0b0000000000000000"
            else:
                s = "move.b {}(%a0), %d0".format(cur_v * 2)
            jump_tables[scaled].append(s)

            s = "move.b %d0, (%a1)+"
            jump_tables[scaled].append(s)
            jump_tables[scaled].append(s)
            # tbl_size += 4
            prev_v = cur_v
    # print("full size in bytes: {}".format(tbl_size))
    with open(asm_file, "w") as f:
        for i in range(0, max_scaled_y + 1):
            f.write("\n.globl byte_scale_{}_{}\n".format(tex_size, i))
            f.write("\nbyte_scale_{}_{}:\n".format(tex_size, i))
            f.write("\n".join(jump_tables[i]))
            f.write("\nrts\n")
        f.write(".globl end_byte_tables")
        f.write("\nend_byte_tables:\n")
    with open(c_file, "w") as f:
        for i in range(0, max_scaled_y + 1):
            f.write("extern void byte_scale_{}_{}(void);\n".format(tex_size, i))
        f.write("void* bytes_jump_table_lut[{}]".format(max_scaled_y + 1) + " = {\n")
        for i in range(0, max_scaled_y + 1):
            f.write("byte_scale_{}_{}, ".format(tex_size, i))
            if i % 5 == 0:
                f.write("\n")
        f.write("\n};\n")


def gen_tables(max_scaled_y, tex_size, asm_file, c_file):
    tbl_size = 0
    jump_tables = {}
    for scaled in range(0, max_scaled_y + 1):
        if scaled != 0:
            dv_dy = tex_size / scaled
        jump_tables[scaled] = []
        prev_v = 0
        for y in range(scaled):
            cur_v = int(dv_dy * y)
            if cur_v == 0:
                s = "dc.w 0b0011001011101000, 0b0000000000000000"
            else:
                s = "move.w {}(%a0), (%a1)+".format(cur_v * 2)
            jump_tables[scaled].append(s)
            tbl_size += 4
            prev_v = cur_v
    # print("full size in bytes: {}".format(tbl_size))
    with open(asm_file, "w") as f:
        for i in range(0, max_scaled_y + 1):
            f.write("\n.globl scale_{}_{}\n".format(tex_size, i))
            f.write("\nscale_{}_{}:\n".format(tex_size, i))
            f.write("\n".join(jump_tables[i]))
            f.write("\nrts\n")
        f.write(".globl end_tables")
        f.write("\nend_tables:\n")
    with open(c_file, "w") as f:
        for i in range(0, max_scaled_y + 1):
            f.write("extern void scale_{}_{}(void);\n".format(tex_size, i))
        f.write("void* jump_table_lut[{}]".format(max_scaled_y + 1) + " = {\n")
        for i in range(0, max_scaled_y + 1):
            f.write("scale_{}_{}, ".format(tex_size, i))
            if i % 5 == 0:
                f.write("\n")
        f.write("\n};\n")


# Press the green button in the gutter to run the script.
if __name__ == '__main__':

    #norm_f = "C:\\Users\\Erik\\code\\genesis\\DOOM\\res\\textures\\WALL_A.png"
    #mid_f = "C:\\Users\\Erik\\code\\genesis\\DOOM\\res\\textures\\WALL_A_DARKER.png"
    #dark_f = "C:\\Users\\Erik\\code\\genesis\\DOOM\\res\\textures\\WALL_A_DARKEST.png"
    #name = "wall_a"

    #norm_f = "C:\\Users\\Erik\\code\\genesis\\DOOM\\res\\textures\\DOOR.png"
    #mid_f = norm_f
    #dark_f = mid_f
    #name = "door"


    #norm_f = "C:\\Users\\Erik\\code\\genesis\\DOOM\\res\\textures\\WALL_B.png"
    #mid_f = "C:\\Users\\Erik\\code\\genesis\\DOOM\\res\\textures\\WALL_B_DARKER.png"
    #dark_f = "C:\\Users\\Erik\\code\\genesis\\DOOM\\res\\textures\\WALL_B_DARKEST.png"
    #name = "wall_b"
    #norm_f = "C:\\Users\\Erik\\code\\genesis\\DOOM\\res\\textures\\WALL_C.png"
    #mid_f = "C:\\Users\\Erik\\code\\genesis\\DOOM\\res\\textures\\WALL_C_DARKER.png"
    #dark_f = "C:\\Users\\Erik\\code\\genesis\\DOOM\\res\\textures\\WALL_C_DARKEST.png"
    #name = "wall_c"

    norm_f = "C:\\Users\\Erik\\code\\genesis\\DOOM\\res\\textures\\WALL_A_FOG.png"
    mid_f = "C:\\Users\\Erik\\code\\genesis\\DOOM\\res\\textures\\WALL_A_FOG_DARKER.png"
    dark_f = "C:\\Users\\Erik\\code\\genesis\\DOOM\\res\\textures\\WALL_A_FOG_DARKEST.png"
    name = "wall_a_fog"

    norm_f = "C:\\Users\\Erik\\code\\genesis\\DOOM\\res\\textures\\KEY.png"
    mid_f = "C:\\Users\\Erik\\code\\genesis\\DOOM\\res\\textures\\KEY.png"
    dark_f = "C:\\Users\\Erik\\code\\genesis\\DOOM\\res\\textures\\KEY.png"
    name = "key"

    norm_f = "C:\\Users\\Erik\\code\\genesis\\DOOM\\res\\textures\\KEY_32_32.png"
    mid_f = "C:\\Users\\Erik\\code\\genesis\\DOOM\\res\\textures\\KEY_32_32.png"
    dark_f = "C:\\Users\\Erik\\code\\genesis\\DOOM\\res\\textures\\KEY_32_32.png"
    name = "key_32_32"

    gen_mip_image(name, norm_f, mid_f, dark_f)

    # gen_tables(512, 64, "C:\\Users\\Erik\\Desktop\\tables.s", "C:\\Users\\Erik\\Desktop\\tables_table.c")
    # gen_byte_tables(512, 64, "C:\\Users\\Erik\\Desktop\\byte_tables.s", "C:\\Users\\Erik\\Desktop\\byte_tables_table.c")

    # for y in range(1,1024):
    #    print("{}, ".format((32*256)//y), end='')
    #    if y % 5 == 0:
    #        print("")

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
"""
pixels_to_output = 0
for i in range(1, 512):
    if i/64 == i//64:
        pixels_to_output += i
    else:
        for bot_clip in range(1, i):
            rem_clippable = i-(bot_clip-1)
            pixels_to_output += rem_clippable
            #for top_clip in range(0, rem_clippable):
            #    pixels_to_output += 1
        #for clip in range(0, i):
        #    pixels_to_output += i-clip
print("{}".format(pixels_to_output*1))
"""
