import imgui

import utils

def draw_defaults_mode(cur_state):
    color_opts = ["{}".format(idx) for idx in range(16)]
    up_col_changed, new_up_col = imgui.core.combo("upper color", cur_state.default_up_color, color_opts)
    mid_col_changed, new_mid_col = imgui.core.combo("middle color", cur_state.default_mid_color, color_opts)
    low_col_changed, new_low_col = imgui.core.combo("lower color", cur_state.default_low_color, color_opts)
    
    tex_files = utils.get_texture_files(cur_state)
    
    def set_default_tex_file(f):
        cur_state.default_texture_file = f
    utils.file_selector("texture", cur_state.default_texture_file, tex_files, set_default_tex_file)


    floor_col_changed, new_floor_col = imgui.core.combo("floor color", cur_state.default_floor_color, color_opts)
    ceil_col_changed, new_ceil_col = imgui.core.combo("ceil color ", cur_state.default_ceil_color, color_opts)

    if up_col_changed:
        cur_state.default_up_color = new_up_col
    if mid_col_changed:
        cur_state.default_mid_color = new_mid_col
    if low_col_changed:
        cur_state.default_low_color = new_low_col
    if floor_col_changed:
        cur_state.default_floor_color = new_floor_col
    if ceil_col_changed:
        cur_state.default_ceil_color = new_ceil_col
