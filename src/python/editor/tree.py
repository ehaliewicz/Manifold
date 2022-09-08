import imgui
import numpy as np



def draw_tree_mode(cur_state):
    disabled = all([sect.is_convex() for sect in cur_state.map_data.sectors])
    if not disabled:
        if imgui.button("Split concave sectors"):
            print("Do splitting here")

            
