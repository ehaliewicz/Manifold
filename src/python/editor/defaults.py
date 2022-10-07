import imgui

def draw_defaults_mode(cur_state):
    color_opts = ["{}".format(idx) for idx in range(16)]
    texture_idxs = ["{}".format(i) for i in range(32)]
    up_col_changed, new_up_col = imgui.core.combo("upper color", cur_state.default_up_color, color_opts)
    mid_col_changed, new_mid_col = imgui.core.combo("middle color", cur_state.default_mid_color, color_opts)
    low_col_changed, new_low_col = imgui.core.combo("lower color", cur_state.default_low_color, color_opts)
    tex_idx_changed, new_tex_idx = imgui.core.combo("texture idx", cur_state.default_texture_idx, texture_idxs)

    if up_col_changed:
        cur_state.default_up_color = new_up_col
    if mid_col_changed:
        cur_state.default_mid_color = new_mid_col
    if low_col_changed:
        cur_state.default_low_color = new_low_col
    if tex_idx_changed:
        cur_state.default_texture_idx = new_tex_idx
