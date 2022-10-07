import imgui
#define RGB24_TO_VDPCOLOR(color)    
# (((((color + 0x100000) < 0xFF0000 ? color + 0x100000 : 0xFF0000) >> (20)) & VDPPALETTE_REDMASK) | (((((color & 0xff00) + 0x1000) < 0xFF00 ? (color & 0xff00) + 0x1000 : 0xFF00) >> ((1 * 4) + 4)) & VDPPALETTE_GREENMASK) | (((((color & 0xff) + 0x10) < 0xFF ? (color & 0xff) + 0x10 : 0xFF) << 4) & VDPPALETTE_BLUEMASK))
#define RGB8_8_8_TO_VDPCOLOR(r, g, b) 
# 
# RGB24_TO_VDPCOLOR(((((b) << 0) & 0xFF) | (((g) & 0xFF) << 8) | (((r) & 0xFF) << 16)))


VDPPALETTE_REDMASK   = 0x000E
VDPPALETTE_GREENMASK = 0x00E0
VDPPALETTE_BLUEMASK  = 0x0E00
VDPPALETTE_COLORMASK = 0x0EEE

col_lut = [0, 36, 72, 108, 144, 180, 216, 252]
 
def vdp_color_to_rgb(vdpcolor):
    r = col_lut[((vdpcolor>>1)&7)]/255
    g = col_lut[((vdpcolor>>5)&7)]/255
    b = col_lut[((vdpcolor>>9)&7)]/255
    return [r,g,b]

DEFAULT_PALETTE = [vdp_color_to_rgb(rgb) for rgb in [
    3598,
    0,
    0,
    520,
    546,
    1090,
    44,
    2628,
    98,
    1096,
    2690,
    1676,
    172,
    2222,
    0,
    2278
    #[255,  0,247],
    #[  0,  0,  0],
    #[0,   31, 31],
    #[155,  0, 62],
    #[ 62, 62, 62],
    #[ 62, 93, 93],
    #[217, 62,  0],
    #[ 93, 93,186],
    #[ 62,124,  0],
    #[155, 93, 93],
    #[ 62,155,186],
    #[217,155,124],
    #[217,186,  0],
    #[248,186,155],
    #[  0,  0,  0],
    #[124,248,155],
]]
print(DEFAULT_PALETTE)

def rgb24_to_vdp_color(color):
    r = (((color + 0x100000 if ((color + 0x100000) < 0xFF0000) else  0xFF0000) >> (20)) & VDPPALETTE_REDMASK) 
    g = ((((color & 0xff00) + 0x1000 if (((color & 0xff00) + 0x1000) < 0xFF00) else 0xFF00) >> ((1 * 4) + 4)) & VDPPALETTE_GREENMASK)
    b = (((((color & 0xff) + 0x10) if (((color & 0xff) + 0x10) < 0xFF) else 0xFF) << 4) & VDPPALETTE_BLUEMASK)
    return (r | g | b)

def rgb_to_vdp_color(r,g,b):
    return rgb24_to_vdp_color((b & 0xFF)|((g & 0xFF)<<8)|((r & 0xFF) << 16))


def draw_palette_mode(cur_state):
    imgui.begin_child("blah")
    for idx in range(16):
        changed, val = imgui.color_edit3("{}:".format(idx), *cur_state.map_data.palette[idx])
        if changed:
            cur_state.map_data.palette[idx] = val 
    imgui.end_child()    

