
import imgui
import undo

def draw_music_mode(cur_state):
    chg_xgm, xgm_val = imgui.input_text("XGM Tool path: ", cur_state.xgmtool_path, buffer_length=128)
    if chg_xgm:
        undo.push_state(cur_state)
        cur_state.xgmtool_path = xgm_val
        
    chg_mus, mus_val = imgui.input_text("Music track: ", cur_state.map_data.music_path, buffer_length=128)
    if chg_mus:
        undo.push_state(cur_state)
        cur_state.map_data.music_path = mus_val