import typing
import imgui
import math
import os
import random
import re




ENGINE_X_SCALE = 1.3
ENGINE_Y_SCALE = -1.3

PLAYER_COLLISION_SIZE = 20

def dist(x1,y1, x2,y2):
    dx = x2-x1
    dy = y2-y1
    return math.sqrt((dx*dx)+(dy*dy))
    
def point_in_circle(px,py,cx,cy,r):
    dx = px-cx
    dy = py-cy
    
    if abs(dx) > r or abs(dy) > r:
        return False

    dst = dist(px,py, cx,cy)
    return dst <= r


def point_on_line(x1, y1, x2, y2, px, py):

    # get distance from the point to the two ends of the line
    d1 = dist(px,py, x1,y1)
    d2 = dist(px,py, x2,y2)

    # get the length of the line
    line_len = dist(x1,y1, x2,y2)

    # since floats are so minutely accurate, add
    # a little buffer zone that will give collision
    buf = 0.1    # higher num = less accurate

    # if the two distances are equal to the line's 
    # length, the point is on the line!
    # note we use the buffer here to give a range, 
    # rather than one #
    return (d1+d2 >= line_len-buf and d1+d2 <= line_len+buf)


def circle_on_line(x1,y1, x2,y2, cx, cy, r):
    inside1 = point_in_circle(x1,y1, cx,cy, r)
    inside2 = point_in_circle(x2,y2, cx,cy, r)
    if inside1 or inside2:
        return True

    #dx = x1-x2
    #dy = y1-y2
    dx = x2-x1
    dy = y2-y1
    length = dist(x1,y1, x2, y2)

    dot = ( ((cx-x1)*dx) + ((cy-y1)*dy) ) / math.pow(length,2)
    closestX = x1 + (dot * dx)
    closestY = y1 + (dot * dy)

    onSegment = point_on_line(x1,y1,x2,y2, closestX,closestY);
    if not onSegment:
        return False

    #distX = closestX - cx;
    #distY = closestY - cy;
    dst = dist(closestX,closestY, cx, cy)

    return dst <= r





def draw_list(cur_state, id_str, label, items, select_item, delete_callback = None, print_item=None):
    
    imgui.begin_child(id_str)
    imgui.text(label)

    cur_state.hovered_item = None
    
    for idx,item in enumerate(items):
        cur_id_str = "##list_{}_{}".format(id_str, idx)
        sel_btn_id  = "Select {} {}".format(idx, cur_id_str)
        del_btn_id = "X{}".format(cur_id_str)
        
        imgui.begin_group()
        if imgui.button(sel_btn_id):
            select_item(idx)
        imgui.same_line()
        
        if print_item is not None:
            imgui.text(print_item(item))
        else:
            imgui.text(str(item))
        if delete_callback is not None:
            imgui.same_line()
            if imgui.button(del_btn_id):
                delete_callback(item)
        
        
        imgui.end_group()

        if imgui.is_item_hovered():
            cur_state.hovered_item = item
            
    imgui.end_child()


    
def input_int(label, id_str, input_val, set_val):
    imgui.text(label)
    imgui.same_line()
    changed,new_val = imgui.input_int(id_str, input_val)

    if changed:
        set_val(new_val)

        
def input_int2(label, id_str, input_vals, set_vals):
    imgui.text(label)
    imgui.same_line()
    changed,new_vals = imgui.input_int2(id_str, *input_vals)

    if changed:
        set_vals(new_vals)

def get_files_in_folder(folder_path):
    return [f for f in os.listdir(folder_path) if os.path.isfile(os.path.join(folder_path, f))]
    

def file_selector(label, cur_val, files, changed_cb):
    cur_idx = files.index(cur_val)
    file_changed, new_file_idx = imgui.core.combo(label, cur_idx, files)
    if file_changed:
        changed_cb(files[new_file_idx])

class Texture(object):
    def __init__(self, pfx: str, tex_files: list[str]):
        assert len(tex_files) > 0 and len(tex_files) < 256
        self.prefix = pfx 
        self.tex_files = tex_files

    def __str__(self) -> str:
        return self.prefix 
    
    def gen_textures() -> list[list[int]]:
        # return a list of bytes for textures 
        pass
    
    def gen_anim_texture(self, first_tex_idx: int, frequency: int):
        assert first_tex_idx > 0 and first_tex_idx < 256
        assert frequency > 0 and frequency < 5
        return [len(self.tex_files), frequency, first_tex_idx]


def get_anim_texture_files(cur_state):
    tex_files = [f for f in get_files_in_folder(cur_state.textures_path) if ".png" in f]
    tex_files = sorted(tex_files)
    texs = []
    
    cur_anim_name = None
    cur_anim = []
    for tex_file in tex_files: 
        res = re.match('(.*)_[0-9]*.png', tex_file)
        if res is None:
            if cur_anim_name:
                texs.append(Texture(cur_anim_name, cur_anim))
                cur_anim_name = None
            # push new texture, don't start an animation
            texs.append(Texture(tex_file.split(".png")[0], [tex_file]))
        else:
            # it matches
            # if it matches the current animation, append to it
            if res.group(1) == cur_anim_name:
                cur_anim.append(tex_file)
            else:
                # otherwise, push the current animation if it exists
                if cur_anim_name:
                    texs.append(Texture(cur_anim_name, cur_anim))
                # and start a new animation
                cur_anim = [tex_file]
                cur_anim_name = res.group(1)

    if cur_anim_name:
        texs.append(Texture(cur_anim_name, cur_anim))
        
    return texs

def get_files(path, ext, name, required):
    files = [f for f in get_files_in_folder(path) if ext in f]
    if required:
        assert len(files) > 0, "No {} files provided!".format(name)
    return files


def get_texture_files(cur_state):
    files = [f for f in get_files_in_folder(cur_state.textures_path) if ".png" in f]
    assert len(files) > 0, "No texture files provided!"
    return files


def get_music_files(cur_state):
    return [f for f in get_files_in_folder(cur_state.music_tracks_path) if ".vgm" in f]

def get_sprite_files(cur_state):
    sprite_files = [f for f in get_files_in_folder(cur_state.sprites_path) if ".png" in f]
    assert len(sprite_files) > 0, "No sprite files provided!"
    return sprite_files

def random_bright_color():
    rand_r = random.random()
    rand_g = random.random()
    rand_b = random.random()

    if (rand_r + rand_g + rand_b)/3 < 0.5:
        rand_r *= 1.25
        rand_g *= 1.25
        rand_b *= 1.25 
        rand_r = min(1.0, rand_r)
        rand_g = min(1.0, rand_g)
        rand_b = min(1.0, rand_b)

    return (rand_r, rand_g, rand_b, 1.0)
    


     
     
root_window_draw_list = None 

def set_root_window_draw_list(rw):
    global root_window_draw_list
    root_window_draw_list = rw

def get_root_window_draw_list():
    global root_window_draw_list
    return root_window_draw_list