
import imgui

import undo
import utils 

def draw_music_mode(cur_state):
    chg_xgm, xgm_val = imgui.input_text("XGM Tool path: ", cur_state.xgmtool_path, buffer_length=128)
    if chg_xgm:
        undo.push_state(cur_state)
        cur_state.xgmtool_path = xgm_val
        
    music_files = ["No track"] + utils.get_music_files(cur_state)   
    def set_music_track(new_track):
        undo.push_state(cur_state)
        if new_track == "No track":
            cur_state.map_data.music_path = ""
        else:
            cur_state.map_data.music_path = new_track

    if cur_state.map_data.music_path == "":
        utils.file_selector("Music track: ", "No track", music_files, set_music_track)
    else:
        utils.file_selector("Music track: ", cur_state.map_data.music_path, music_files, set_music_track)
