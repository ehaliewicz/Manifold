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

def gen_textures_from_atlas(name, atlas_file):
    conv = []
    dconv = []

    def add_pix(arr, l, r):
        arr.append((dpix(l) << 8) | dpix(r))

    atlas = Image.open(atlas_file)
    all_pix = atlas.load()

    def pix(x, y):
        return all_pix[x, y], all_pix[x+64, y]
    
    TEX_SIZE = 64
    
    atlas_width, atlas_height = atlas.size
    assert atlas_width == TEX_SIZE*2
    num_textures = atlas_height//TEX_SIZE
    assert num_textures * TEX_SIZE == atlas_height

    for tex in range(num_textures):
        base_y = tex*64

        for x in range(0, TEX_SIZE, 2):
            lx, rx = x, x + 1
            for y in range(TEX_SIZE):
                mid_l, dark_l = pix(lx, y+base_y)
                mid_r, dark_r = pix(rx, y+base_y)
                add_pix(conv, mid_l, mid_r)
                add_pix(dconv, dark_l, dark_r)
        
        names = []
        for (key, tbl) in [('mid', conv), ('dark', dconv),
                        ]:

            gen_name = "raw_{}_tex_{}_{}".format(name, tex, key)
            names.append(gen_name)
            print("const u16 {}[{}*{}]".format(gen_name, len(tbl) // 64, 64) + " = {")
            for i, col in enumerate(tbl):
                print("0x{:02x}, ".format(col), end='')
                if i != 0 and i % 64 == 0:
                    print("")
            print("\n};\n\n")

        mname = names[0]
        dname = names[1]
        for (level,d,m,l) in [("dark", dname, dname, mname),
                            #("mid_dark", dname, mname),
                            ("mid", dname, dname, mname),
                            #("mid_light", mname, mname),
                            ("light", mname, mname, mname)]:

            print("const lit_texture lit_{}_tex_{}_{} = {}".format(name, tex, level, '{'))
            print("  .dark = {}, .mid = {}, .light = {}".format(d, m, l))
            print("};")
        
        conv = []
        dconv = []



def gen_mip_image(name, light_file, mid_file, dark_file):
    conv = []
    lconv = []
    dconv = []


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

    for x in range(0, mx, 2):
        lx, rx = x, x + 1
        for y in range(my):
            light_l, mid_l, dark_l = pix(lx, y)
            light_r, mid_r, dark_r = pix(rx, y)

            add_pix(lconv, light_l, light_r)
            add_pix(conv, mid_l, mid_r)
            add_pix(dconv, dark_l, dark_r)


    names = []
    for (key, tbl) in [('light', lconv), ('mid', conv), ('dark', dconv),
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




if __name__ == '__main__':


    
    #norm_f = "C:\\Users\\Erik\\code\\genesis\\DOOM\\res\\textures\\WALLA_15COL.png"
    #mid_f = "C:\\Users\\Erik\\code\\genesis\\DOOM\\res\\textures\\WALLA_15COL.png"
    #dark_f = "C:\\Users\\Erik\\code\\genesis\\DOOM\\res\\textures\\WALLA_15COL.png"
    #name = "walla_15col"
    #gen_mip_image(name, norm_f, mid_f, dark_f)


    gen_textures_from_atlas("tex_15col", "C:\\Users\\Erik\\code\\genesis\\DOOM\\res\\textures\\texture_atlas.png")