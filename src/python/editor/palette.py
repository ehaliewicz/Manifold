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
 
def hex_color_to_rgb(hex):
    r = ((hex>>16)&0xFF)/256
    g = ((hex>>8)&0xFF)/256
    b = ((hex>>0)&0xFF)/256
    return [r,g,b]

def vdp_color_to_rgb(vdpcolor):
    r = col_lut[((vdpcolor>>1)&7)]/255
    g = col_lut[((vdpcolor>>5)&7)]/255
    b = col_lut[((vdpcolor>>9)&7)]/255
    return [r,g,b]

DEFAULT_PALETTE = [hex_color_to_rgb(rgb) for rgb in [
    0xE31C79, #0x000000,
    0x9b5d5d,
    0xd99b7c,
    0xf8ba9b,
    0x7cf89b,
    0x3e5d5d,
    0x001f1f,
    0x3e7c00,
    0x3e3e3e,
    0xd93e00,
    0x1f001f,
    0x9b003e,
    0x3e9bba,
    0x5d5dba,
    0xd9ba00,
    0x000000,
]]

def rgb24_to_vdp_color(color):
    r = (((color + 0x100000 if ((color + 0x100000) < 0xFF0000) else  0xFF0000) >> (20)) & VDPPALETTE_REDMASK) 
    g = ((((color & 0xff00) + 0x1000 if (((color & 0xff00) + 0x1000) < 0xFF00) else 0xFF00) >> ((1 * 4) + 4)) & VDPPALETTE_GREENMASK)
    b = (((((color & 0xff) + 0x10) if (((color & 0xff) + 0x10) < 0xFF) else 0xFF) << 4) & VDPPALETTE_BLUEMASK)
    return (r | g | b)

def rgb_to_vdp_color(r,g,b):
    return rgb24_to_vdp_color((b & 0xFF)|((g & 0xFF)<<8)|((r & 0xFF) << 16))


def draw_palette_mode(cur_state):
    for idx in range(16):
        changed, val = imgui.color_edit3("{}:".format(idx), *cur_state.map_data.palette[idx])
        if changed:
            cur_state.map_data.palette[idx] = val 

