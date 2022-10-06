import os 
import sys 

print(os.getcwd())
print(sys.executable)

if "editor.exe" in sys.executable:
    cur_path = ".\\"
    os.environ['PYSDL2_DLL_PATH'] = cur_path #".\\" 
else:
    
    cur_path = os.path.join(os.getcwd(), "src", "python", "editor")
    print("cur path: ", cur_path)
#else:
#    current_path = ".\src\\python\\editor\\"


from sdl2 import *
import ctypes

import imgui
#from imgui.integrations.glfw import GlfwRenderer
from imgui.integrations.sdl2 import SDL2Renderer

import OpenGL.GL as gl
from enum import Enum

import tkinter as tk
from tkinter import filedialog, messagebox

import atexit
import pickle


import line
import script
import sector
import trigger
import texture
import tree
import undo
import vertex

import struct 
import subprocess
import configparser

# commands

class Mode(Enum):
    SECTOR = 'Sector'
    LINE = 'Line'
    VERTEX = 'Vertex'
    SCRIPT = 'Script'
    TRIGGER = 'Trigger'
    TEXTURE = 'Texture'
    TREE = 'Tree'


class Map():
    def __init__(self, 
    name="placeholder name", 
    sectors=None, 
    vertexes=None):
        self.bsp = False
        if not sectors:
            self.sectors = []
        else:
            self.sectors = sectors
        #self.walls = []

        if not vertexes:
            self.vertexes = []
        else:
            self.vertexes = vertexes

        self.name = name

    def generate_c_from_map(self):
        num_sectors = len(self.sectors)
        num_vertexes = len(self.vertexes)
        num_walls = sum(len(sect.walls) for sect in self.sectors)

        NUM_SECTOR_ATTRIBUTES = 7

        res = """#include <genesis.h>
#include "colors.h"
#include "portal_map.h"
#include "vertex.h"

"""
        
                
        res += "static const s16 sectors[{}*SECTOR_SIZE] = ".format(num_sectors) + "{\n"

        wall_offset = 0
        portal_offset = 0
        for sect in self.sectors:
            sect_num_walls = len(sect.walls)
            res += "    {}, {}, {}, {},\n".format(
                wall_offset, portal_offset, 
                sect_num_walls, sect.index)
            
            if len(sect.walls) == 0:
                continue
            wall_offset += sect_num_walls+1
            portal_offset += sect_num_walls
            
        res += "};\n\n"


        # TODO: implement sector groups w/ all of these flags
        res += "static const s16 sector_group_params[{}*NUM_SECTOR_PARAMS] = ".format(num_sectors) + "{\n"
        for sect in self.sectors:
            # light_level, orig_height, ticks_left, state,
            # floor_height, ceil_height, floor_color, ceil_color
            res += "{},{},{},{},{}<<4,{}<<4,{},{},\n".format(
                0, 0, 0, 0, sect.floor_height, sect.ceil_height, sect.floor_color, sect.ceil_color
            )
        res += "};\n\n"

        res += "static const s16 sector_group_triggers[{}*8] = ".format(num_sectors) + "{\n"
        for sect in self.sectors:
            res += "0,0,0,0,0,0,0,0,\n"
        res += "};\n\n"

        res += "static const u8 sector_group_types[{}] = ".format(num_sectors) + "{\n"
        for sect in self.sectors:
            res += "NO_TYPE,\n"
        res += "};\n\n"
            
        

        # we can have up to 65536 vertexes
        res += "static const u16 walls[{}]".format(num_walls+num_sectors) + " = {\n"
        for sect in self.sectors:
            prev_v2 = None
            if len(sect.walls) == 0:
                res += "// sector {} is empty\n".format(sect.index)
                continue
            first_v1 = sect.walls[0].v1
            res += "    "
            for wall in sect.walls:
                if prev_v2 is not None:
                    assert prev_v2 == wall.v1

                res += "{}, ".format(wall.v1.index)

                prev_v2 = wall.v2

            res += "{}, \n".format(first_v1.index)

        res += "};\n"

        res += "static const s16 portals[{}] =".format(num_walls) + "{\n"
        for sect in self.sectors:
            res += "    "
            for wall in sect.walls:
                res += "{}, ".format(wall.adj_sector_idx)
                
            res += "\n"
        res += "};\n\n"


        res += "static const u8 wall_normal_quadrants[{}] =".format(num_walls) + "{\n"
        for sect in self.sectors:
            for wall in sect.walls:
                res += "    {},\n".format(wall.normal_quadrant())
        res += "};\n\n"
        

        res += "static const u8 wall_colors[{}*4] =".format(num_walls) + "{\n"
        for sect in self.sectors:
            if sect.index == 19:
                ## TODO: wtf why?
                ## was this some empty sector issue with a test map?
                pass
            for wall in sect.walls:
                res += "{}, {}, {}, {},\n".format(
                    wall.texture_idx, 
                    wall.up_color, 
                    wall.low_color, 
                    wall.mid_color
                    #randrange(3, 12)
                    )
                    
        res += "};\n\n"

        # !!!! this defines the scale of the map. 
        # it should be settable within the editor itself
        # as it generally needs to be tweaked for every map
        res += "#define VERT(x1,y1) { .x = (x1 * 1.3), .y = ((-y1) * 1.3) } \n"
        
        res += "static const vertex vertexes[{}]".format(num_vertexes) + " = {\n"
        for vert in self.vertexes:
            res += "    VERT({},{}),\n".format(vert.x, vert.y)
        res += "};\n"


        
        
        res += "const portal_map {} ".format(self.name.replace(" ", "_")) + " = {\n"
        res += "    .num_sectors = {},\n".format(num_sectors)
        res += "    .num_sector_groups = {},\n".format(num_sectors)
        res += "    .num_walls = {},\n".format(num_walls)
        res += "    .num_verts = {},\n".format(num_vertexes)
        res += "    .sectors = sectors,\n"
        res += "    .sector_group_types = sector_group_types,\n"
        res += "    .sector_group_params = sector_group_params,\n"
        res += "    .sector_group_triggers = sector_group_triggers,\n"
        res += "    .walls = walls,\n"
        res += "    .portals = portals,\n"
        res += "    .vertexes = vertexes,\n"
        res += "    .wall_colors = wall_colors,\n"
        res += "    .wall_norm_quadrants = wall_normal_quadrants,\n"
        res += "    .has_pvs = 0\n"
        res += "};"
                
        
        return res



def set_cur_state(cs):
    global cur_state
    cur_state = cs

def main_sdl2():
    def pysdl2_init():
        width, height = 1280, 800
        window_name = "portal editor"
        if SDL_Init(SDL_INIT_EVERYTHING) < 0:
            print("Error: SDL could not initialize! SDL Error: " + SDL_GetError())
            sys.exit(1)
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1)
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24)
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8)
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1)
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1)
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE)
        SDL_SetHint(SDL_HINT_MAC_CTRL_CLICK_EMULATE_RIGHT_CLICK, b"1")
        SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, b"1")
        window = SDL_CreateWindow(window_name.encode('utf-8'),
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                width, height,
                                SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE)
        if window is None:
            print("Error: Window could not be created! SDL Error: " + SDL_GetError())
            sys.exit(1)
        gl_context = SDL_GL_CreateContext(window)
        if gl_context is None:
            print("Error: Cannot create OpenGL Context! SDL Error: " + SDL_GetError())
            sys.exit(1)
        SDL_GL_MakeCurrent(window, gl_context)
        if SDL_GL_SetSwapInterval(1) < 0:
            print("Warning: Unable to set VSync! SDL Error: " + SDL_GetError())
            sys.exit(1)
        return window, gl_context
    window, gl_context = pysdl2_init()
    renderer = SDL2Renderer(window)

    running = True

    event = SDL_Event()

    shortcut_tab = {
        SDLK_z: lambda: undo.undo(cur_state, set_cur_state),
        SDLK_x: lambda: undo.redo(cur_state, set_cur_state),
        SDLK_s: save_map,
        SDLK_l: load_map,
        SDLK_e: lambda: export_map_to_rom(False),
        SDLK_r: lambda: export_and_launch(True),
        SDLK_q: lambda: sys.exit(1)
    }
    while running:
        while SDL_PollEvent(ctypes.byref(event)) != 0:
            if event.type == SDL_QUIT:
                running = False
                break
            elif event.type == SDL_KEYUP:
                mod = event.key.keysym.mod
                sym = event.key.keysym.sym
                if mod & KMOD_LCTRL:
                    if sym in shortcut_tab:
                        shortcut_tab[sym]()
            renderer.process_event(event)
        renderer.process_inputs()
        imgui.new_frame()
        on_frame()
        gl.glClearColor(1., 1., 1., 1)
        gl.glClear(gl.GL_COLOR_BUFFER_BIT)
        imgui.render()
        renderer.render(imgui.get_draw_data())
        SDL_GL_SwapWindow(window)
    renderer.shutdown()
    SDL_GL_DeleteContext(gl_context)
    SDL_DestroyWindow(window)
    SDL_Quit()



class State(object):
    def __init__(self):
        self.mode = Mode.SECTOR
        self.map_data = Map()
        self.cur_sector = None
        self.cur_wall = None
        self.cur_vertex = None

        self.camera_x = 0
        self.camera_y = 0
        self.zoom = 0
        self.emulator_path = ""
        

        self.hovered_item = None

def load_config():
    global cur_state
    conf_file_path = os.path.join(cur_path, "conf.ini")
    conf_exists = os.path.exists(conf_file_path)
    if not conf_exists:
        print("No configuration file, using defaults!")
        return 
    config = configparser.ConfigParser()
    config.read(conf_file_path)
    cur_state.emulator_path = config.get("Default Settings", "emulator-path")


def reset_state():
    global cur_state, last_exported_rom_file, last_saved_map_file
    last_exported_rom_file = None
    last_saved_map_file = None
    cur_state = State()
    load_config()
    
reset_state()

def add_new_wall(v1, v2):
    undo.push_state(cur_state)
    new_wall = line.Wall(v1=v1, v2=v2, sector_idx=cur_state.cur_sector.index, adj_sector_idx=-1)
    cur_state.cur_sector.add_wall(new_wall)

    new_wall.link_with_adjacent_wall_if_exists(cur_state.map_data)
    
    
    return new_wall
    
def add_new_vertex(x,y):
    undo.push_state(cur_state)
    num_verts = len(cur_state.map_data.vertexes)
    new_vert = vertex.Vertex(x, y, index=num_verts)
    cur_state.map_data.vertexes.append(new_vert)

    return new_vert
    



MODE_DRAW_FUNCS = {
    Mode.SECTOR: sector.draw_sector_mode,
    Mode.LINE: line.draw_line_mode,
    Mode.VERTEX: vertex.draw_vert_mode,
    Mode.SCRIPT: script.draw_script_mode,
    Mode.TRIGGER: trigger.draw_trigger_mode,
    Mode.TEXTURE: texture.draw_texture_mode,
    Mode.TREE: tree.draw_tree_mode
}


def draw_mode():
    
    changed, text_val = imgui.input_text("Name: ", cur_state.map_data.name, buffer_length=64)
    if changed:
        cur_state.map_data.name = text_val

    chg_emu, emu_val = imgui.input_text("Emulator: ", cur_state.emulator_path, buffer_length=128)
    if chg_emu:
        cur_state.emulator_path = emu_val
        
        
    imgui.text("{} mode".format(cur_state.mode.value))

    for k in MODE_DRAW_FUNCS.keys():
        #if can_switch_to(k.value):
        if imgui.radio_button(k.value, cur_state.mode == k):
            cur_state.mode = k
    
        
    MODE_DRAW_FUNCS[cur_state.mode](cur_state)


vert_default = (1,1,1,1)
vert_highlight = (1,0,1,1)
vert_select = (1,1,0,1)

portal_default = (1,0,0,0.3)
portal_highlight = (1,1,0,0.3)
wall_default = (1,0,0,1)
wall_highlight = (1,0,1,1)
wall_selected = (1,1,0,1)

concave_wall = (1,0,0,1)
concave_portal = (1,0,0,.3)
convex_wall = (0,0,1,1)
convex_portal = (0,0,1,.3)

def draw_map_vert(draw_list, vert, highlight=False):
    if highlight:
        color = vert_highlight
    else:
        color = vert_default

    cam_x = cur_state.camera_x
    cam_y = cur_state.camera_y
    draw_list.add_circle_filled(vert.x-cam_x, vert.y-cam_y, 2, imgui.get_color_u32_rgba(*color), num_segments=12)


    
def draw_map_wall(draw_list, wall, highlight=False, tree_mode=False, concave_sector=False):
    is_portal = wall.adj_sector_idx != -1
    tbl = [
        # not highlighted
        wall_default,
        portal_default,
        # highlighted
        wall_highlight,
        portal_highlight,
    ]
           
    
    color = tbl[(highlight<<1 | is_portal)]

        
    v1 = wall.v1
    v2 = wall.v2

    ((n1x,n1y),(n2x,n2y)) = wall.centered_normal()
    
    cam_x = cur_state.camera_x
    cam_y = cur_state.camera_y
     

    if tree_mode:
        color = None
        if concave_sector:
            color = concave_portal if is_portal else concave_wall
        else:
            color = convex_portal if is_portal else convex_wall 
            
        draw_list.add_line(v1.x-cam_x, v1.y-cam_y, v2.x-cam_x, v2.y-cam_y, imgui.get_color_u32_rgba(*color), 1.0)
        draw_list.add_line(n1x-cam_x, n1y-cam_y, n2x-cam_x, n2y-cam_y, imgui.get_color_u32_rgba(*color), 1.0)
    else:
        draw_list.add_line(v1.x-cam_x, v1.y-cam_y, v2.x-cam_x, v2.y-cam_y, imgui.get_color_u32_rgba(*color), 1.0)
        draw_list.add_line(n1x-cam_x, n1y-cam_y, n2x-cam_x, n2y-cam_y, imgui.get_color_u32_rgba(*color), 1.0)

    

def draw_sector(draw_list, sector, highlight=False):
    tree_mode = cur_state.mode == Mode.TREE

    concave_sector = False
    if tree_mode:
        concave_sector = not sector.is_convex()  
        
    for wall in sector.walls:
        wall_selected = cur_state.mode == Mode.LINE and cur_state.cur_wall == wall
        wall_hovered = cur_state.hovered_item == wall
        
        draw_map_wall(draw_list, wall, (highlight or wall_selected or wall_hovered), tree_mode, concave_sector)

        
# returns true if hovered
def draw_map():
    draw_list = imgui.get_window_draw_list()

    for vertex in cur_state.map_data.vertexes:
        
        draw_map_vert(draw_list, vertex, highlight = ((cur_state.mode == Mode.VERTEX and vertex == cur_state.cur_vertex) or vertex == cur_state.hovered_item))
        
        
    #for wall in cur_state.map_data.walls:
    #    draw_map_wall(draw_list, wall, highlight = ((cur_state.mode == Mode.LINE and wall==cur_state.cur_wall) or wall == cur_state.hovered_item))
    
    for sect in cur_state.map_data.sectors:
        is_selected = cur_state.mode == Mode.SECTOR and sect == cur_state.cur_sector
        is_hovered = cur_state.hovered_item == sect
        draw_sector(draw_list, sect, highlight = is_selected or is_hovered)

    if (cur_state.mode == Mode.SECTOR and cur_state.cur_sector is not None):
        draw_sector(draw_list, cur_state.cur_sector, highlight=True)
    if cur_state.hovered_item and isinstance(cur_state.hovered_item, sector.Sector):
        draw_sector(draw_list, cur_state.hovered_item, highlight=True)

LEFT_BUTTON = 0
RIGHT_BUTTON = 1




def interpret_click(x,y,button):
    #global cur_vertex, cur_wall, cur_mode
    ix = int(x)
    iy = int(y)

    
    prev_cur = cur_state.cur_vertex
    clicked_vertex = None
    for vert in cur_state.map_data.vertexes:
        if vert.point_collides(x, y):
            cur_state.cur_vertex = vert
            
            if button == LEFT_BUTTON:
                cur_state.mode = Mode.VERTEX
                return
            elif button == RIGHT_BUTTON:
                clicked_vertex = vert
                break
            
    # then walls
    if button == LEFT_BUTTON:
        for sector in cur_state.map_data.sectors:
            for wall in sector.walls:
                if wall.point_collides(x, y, collide_with_normal=True):
                    cur_state.cur_wall = wall
                    cur_state.mode = Mode.LINE
                    # find sector
                    cur_state.cur_sector = cur_state.map_data.sectors[cur_state.cur_wall.sector_idx]
                    return
        return

        
    if not cur_state.cur_sector:
        return


    # add a new vertex and line to the sector
    

    if clicked_vertex is None:
        ix -= ix%10
        iy -= iy%10
        cur_state.cur_vertex = add_new_vertex(ix, iy)
        

    if prev_cur is not None and prev_cur != cur_state.cur_vertex:
        cur_state.cur_wall = add_new_wall(prev_cur, cur_state.cur_vertex)
        

last_frame_x = None
last_frame_y = None
got_hold_last_frame = False



def on_frame():
    global cur_state, last_frame_x, last_frame_y, got_hold_last_frame

    global last_exported_rom_file, last_saved_map_file

    imgui.set_next_window_position(0, 0)
    io = imgui.get_io()
    imgui.set_next_window_size(io.display_size.x, io.display_size.y)
    imgui.begin("Map",
                flags=imgui.WINDOW_NO_TITLE_BAR|imgui.WINDOW_NO_RESIZE|imgui.WINDOW_NO_COLLAPSE|imgui.WINDOW_NO_BRING_TO_FRONT_ON_FOCUS)

    map_hovered = imgui.is_window_hovered()
        
    draw_map()
    imgui.end()
    
    if imgui.begin_main_menu_bar():
        if imgui.begin_menu("File", True):
            clicked_quit, selected_quit = imgui.menu_item(
                "Quit", 'Ctrl+Q', False, True
            )
            if clicked_quit:
                sys.exit(1)
                
            _, selected_load = imgui.menu_item(
                "Load", "Ctrl-l", False, True
            )
            if selected_load:
                load_map()

                
            _, selected_save = imgui.menu_item(
                "Save", "Ctrl-s", False, True
            )
            if selected_save:
                save_map()
            
            _, selected_save_as = imgui.menu_item(
                "Save as", "", False, True
            )
            if selected_save_as:
                last_saved_map_file = None
                save_map()

            _, selected_export_c = imgui.menu_item(
                "Export to C", "", False, True
            )
            if selected_export_c:
                export_map_to_c()

            _,selected_export_rom = imgui.menu_item(
                "Export to ROM", "Ctrl-e", False, True
            )
            if selected_export_rom:
                export_map_to_rom()

            _, selected_export_as = imgui.menu_item(
                "Export to ROM as", "", False, True)
            if selected_export_as:
                last_exported_rom_file = None
                export_map_to_rom()
            

            _,selected_export_launch = imgui.menu_item(
                "Export to ROM and launch", "Ctrl-r", False, True
            )
            if selected_export_launch:
                export_and_launch(set_launch_flags=True)

            _, selected_export_launch_as = imgui.menu_item(
                "Export to ROM and launch as", "", False, True
            )
            if selected_export_launch_as:
                last_exported_rom_file = None
                export_and_launch(set_launch_flags=True)


            _, selected_reset = imgui.menu_item(
                "Reset", "", False, True
            )
            if selected_reset:    
                undo.push_state(cur_state)
                reset_state()


            _, selected_undo = imgui.menu_item(
                "Undo", "", False, undo.can_undo()
            )
            if selected_undo:
                undo.undo(cur_state, set_cur_state)

            _, selected_redo = imgui.menu_item(
                "Redo", "", False, undo.can_redo()
            )     
            if selected_redo:
                undo.redo(cur_state, set_cur_state)   



            imgui.end_menu()
        imgui.end_main_menu_bar()
    
        
    imgui.begin("Tools", False)
    tools_hovered = imgui.is_window_hovered()
    draw_mode()
    

    left_button_clicked = imgui.is_mouse_clicked(button=0)
    right_button_clicked = imgui.is_mouse_clicked(button=1)

    

    mouse_button_clicked = (left_button_clicked or right_button_clicked)
    left_button_down = imgui.is_mouse_down(button=0)
    if mouse_button_clicked:
        pass
        #print("clicked left {} right {}".format(left_button_clicked, right_button_clicked))

    #else:
    #    print("clearing down_x and down_y")
    #    start_down_x = None
    #    start_down_y = None
        
    if got_hold_last_frame:
        cur_x,cur_y = imgui.get_mouse_pos()
        moved_cam_x = last_frame_x - cur_x
        moved_cam_y = last_frame_y - cur_y
        cur_state.camera_x += moved_cam_x
        cur_state.camera_y += moved_cam_y


    
    if map_hovered and not tools_hovered and mouse_button_clicked:
        x,y = imgui.get_mouse_pos()
        #print("interpreting click at {},{}".format(x,y))
        interpret_click(x+cur_state.camera_x,y+cur_state.camera_y, LEFT_BUTTON if left_button_clicked else RIGHT_BUTTON)
    elif map_hovered and left_button_down:
        got_hold_last_frame = True
        last_frame_x, last_frame_y = imgui.get_mouse_pos()
    elif not left_button_down:
        got_hold_last_frame = False

        
    
    imgui.end()


def load_map():
    f = filedialog.askopenfile(mode="rb")
    if f is not None:
        load_map_from_file(f)
        f.close()

tbl = {}
def old_vertex_to_new_vertex(v):
    key = (v.x,v.y,v.index)
    if key not in tbl:
        tbl[key] = vertex.Vertex(v.x, v.y, v.index)
    return tbl[key]

def old_vertexes_to_new_vertexes(vertexes):
    return [old_vertex_to_new_vertex(v) for v in vertexes]

def old_wall_to_new_wall(wall):
    nw = line.Wall(old_vertex_to_new_vertex(wall.v1), 
    old_vertex_to_new_vertex(wall.v2), wall.sector_idx, wall.adj_sector_idx)
    nw.low_color = wall.low_color
    nw.mid_color = wall.mid_color
    nw.up_color = wall.up_color
    nw.texture_idx = wall.texture_idx
    return nw


def old_walls_to_new_walls(walls):
    return [old_wall_to_new_wall(w) for w in walls]


def old_sectors_to_new_sectors(sectors):
    return [sector.Sector(
                index=s.index, walls=old_walls_to_new_walls(s.walls), 
                floor_height=s.floor_height, ceil_height=s.ceil_height,
                floor_color=s.floor_color, ceil_color=s.ceil_color) for s in sectors]
        
        
def load_map_from_file(f):
    global cur_state
    old_state = pickle.load(f)
    old_map = old_state.map_data

    

    new_map = Map(name=old_map.name,
                  sectors=old_sectors_to_new_sectors(old_map.sectors),
                  vertexes=old_vertexes_to_new_vertexes(old_map.vertexes))                      

    reset_state()
    cur_state.map_data = new_map

    cur_state.cur_sector = None
    cur_state.cur_vertex = None
    cur_state.cur_wall = None
    
        
last_saved_map_file = None

def save_map():
    global last_saved_map_file
    if last_saved_map_file is None:
        f = filedialog.asksaveasfile(mode="wb")
    else:
        f = open(last_saved_map_file, mode="wb")

    if f is not None:
        last_saved_map_file = os.path.realpath(f.name)
        pickle.dump(cur_state, f)
        f.close()

def save_map_as():
    f = filedialog.asksaveasfile(mode="wb")
    if f is not None:
        last_saved_map_file = os.path.realpath(f.name)
        save_map()



    
def export_map_to_c():
    f = filedialog.asksaveasfile(mode="w")
    if f is not None:
        f.write(cur_state.map_data.generate_c_from_map())
        f.close()

def _read_longword_(f, signed):
    return int.from_bytes(f.read(4), "big", signed)
def read_s32(f):
    return _read_longword_(f, True)
def read_u32(f):
    return _read_longword_(f, False)

def _write_longword_(f, lw, signed):
    off = f.tell()
    f.write(lw.to_bytes(4, byteorder="big", signed=signed))
    return off 

def write_s32(f, lw):
    return _write_longword_(f, lw, True)
def write_u32(f, lw):
    return _write_longword_(f, lw, False)


def _read_word_(f, signed):
    return int.from_bytes(f.read(2), "big", signed)

def read_s16(f):
    return _read_word_(f, True)
def read_u16(f):
    return _read_word_(f, False)

def _write_word_(f, w, signed):
    off = f.tell()
    f.write(w.to_bytes(2, byteorder="big", signed=signed))
    return off 

def write_u16(f, w):
    _write_word_(f, w, False)
def write_s16(f, w):
    _write_word_(f, w, True)


def _read_byte_(f, signed):
    return int.from_bytes(f.read(1), byteorder="big", signed=signed)
def _write_byte_(f, b, signed):
    off = f.tell()
    f.write(b.to_bytes(1, byteorder="big", signed=signed))
    return off

def read_s8(f):
    return _read_byte_(f, True)
def read_u8(f):
    return _read_byte_(f, False)
def write_s8(f, b):
    return _write_byte_(f, b, True)
def write_u8(f, b):
    return _write_byte_(f, b, False)


def pointer_placeholder(f):
    return write_u32(f, 0xDEADBEEF)

def patch_pointer_to_current_offset(f, ptr_off):
    ptr = f.tell()
    f.seek(ptr_off)
    write_u32(f, ptr)
    f.seek(ptr)

def align(f):
    if f.tell() & 0b1:
        f.seek(f.tell()+1)

launched_emu_process = None 
def launch_emulator(emu_path, rom_path):
    global launched_emu_process
    if emu_path == "":
        return
    if rom_path == "":
        return
    if launched_emu_process is not None:
        print("process previously launched")
        if launched_emu_process.poll() is None:
            print("killing process")
            launched_emu_process.kill()
            print("killed")
        launched_emu_process = None
    cmd = "{} \"{}\"".format(emu_path, rom_path)
    print("Executing command {}".format(cmd))
    try:
        launched_emu_process = subprocess.Popen([emu_path, rom_path])
    except Exception as e:
        print("error launching {}".format(e))
        return

last_exported_rom_file = None

def export_map_to_rom(set_launch_flags=False):
    global last_exported_rom_file
    if last_exported_rom_file is None:
        f = filedialog.asksaveasfile(mode="r")
    else:
        f = open(last_exported_rom_file, "r")
    if f is None:
        return 

    last_exported_rom_file = os.path.realpath(f.name)


    f.close()

    with open(last_exported_rom_file, "rb+") as f:
        s = f.read()
        map_table_base = s.find(b'\xDE\xAD\xBE\xEF') 
        #s.find(b'mapt') 
        num_maps_offset = map_table_base + 4
        map_pointer_offset =  map_table_base + 20
        if map_table_base == -1:
            messagebox.showerror(
                title="Error",
                message="Couldn't find map table, is this a correct ROM file?"
            )
            return
        map_data_base = s.find(b'WAD?')
        if map_data_base == -1:
            messagebox.showerror(
                title="Error",
                message="Couldn't find map data table, is this a correct ROM file?"
            )
            return

        map_struct_offset = map_data_base + 4

        start_in_game_flag_offset = s.find(b'\xFE\xED\xBE\xEF')
        if start_in_game_flag_offset == -1:
            messagebox.showerror(
                title="Error",
                message="Couldn't find init flag table, is this a correct ROM file?"
            )
            return
        start_in_game_flag_offset += 4 
        init_load_level_off = s.find(b'\xBE\xEF\xFE\xED')
        if init_load_level_off == -1:
            messagebox.showerror(
                title="Error",
                message="Couldn't find init load table, is this a correct ROM file?"
            )
            return

        data = cur_state.map_data
        for sect in data.sectors:
            if not sect.is_convex():
                messagebox.showerror(
                    title="Error",
                    message="Sector {} is not convex".format(sect.index)
                )


        init_load_level_off += 4
        if set_launch_flags:
            f.seek(start_in_game_flag_offset)
            write_u32(f, 1)

            f.seek(init_load_level_off)
            write_u32(f, 3)
        else:
            f.seek(start_in_game_flag_offset)
            write_u32(f, 0)

            f.seek(init_load_level_off)
            write_u32(f, 0)

        

        # write number of maps (HARDCODED to 4 in this case)
        f.seek(num_maps_offset)
        write_u32(f, 4)

        # write address of map struct (in map data table)
        f.seek(map_pointer_offset)
        write_u32(f, map_struct_offset)

        
        num_sectors = len(data.sectors)
        num_vertexes = len(data.vertexes)
        num_walls = sum(len(sect.walls) for sect in data.sectors)

        f.seek(map_struct_offset)
        # write num_sector_groups, currently the same as num sectors (no groups supported in editor yet)
        write_u16(f, num_sectors)
        # write num_sectors, num_walls, and num_verts
        write_u16(f, num_sectors)
        write_u16(f, num_walls)
        write_u16(f, num_vertexes)
        
        sectors_ptr_offset = pointer_placeholder(f)
        sector_group_types_ptr_offset = pointer_placeholder(f)
        sector_group_params_ptr_offset = pointer_placeholder(f)
        sector_group_triggers_ptr_offset = pointer_placeholder(f)
        walls_ptr_offset = pointer_placeholder(f)
        portals_ptr_offset = pointer_placeholder(f)
        wall_colors_ptr_offset = pointer_placeholder(f)
        vertexes_ptr_offset = pointer_placeholder(f)
        wall_norm_quads_ptr_offset = pointer_placeholder(f)
        # write has_pvs (HARDCODED to false right now)
        write_u16(f, 0)
        # write pvs pointers(HARDCODED to NULL)
        pvs_ptr_offset = write_u32(f, 0)
        raw_pvs_ptr_offset = write_u32(f, 0) 
        name_ptr_offset = pointer_placeholder(f)

        patch_pointer_to_current_offset(
            f, sectors_ptr_offset
        )
        
        wall_offset = 0
        portal_offset = 0
        for sect in data.sectors:
            sect_num_walls = len(sect.walls)
            f.write(struct.pack(
                ">hhhh",
                wall_offset, portal_offset,
                sect_num_walls, sect.index
            ))
            if len(sect.walls) == 0:
                continue
            wall_offset += sect_num_walls+1
            portal_offset += sect_num_walls

        patch_pointer_to_current_offset(
            f, sector_group_types_ptr_offset
        )
        for sect in data.sectors:
            f.write(b'\x00')
        align(f)

        patch_pointer_to_current_offset(
            f, sector_group_params_ptr_offset
        )

        for sect in data.sectors:
            f.write(struct.pack(
                # light level, orig_height, ticks_left, state
                # floor_height, ceil_height, floor_color, ceil_color
                ">hhhhhhhh",
                0, 0, 0, 0, 
                sect.floor_height*16, sect.ceil_height*16,
                sect.floor_color, sect.ceil_color
            ))
        
        patch_pointer_to_current_offset(
            f, sector_group_triggers_ptr_offset
        )
        for sect in data.sectors:
            f.write(struct.pack(
                ">hhhhhhhh", 0,0,0,0,0,0,0,0
            ))

        patch_pointer_to_current_offset(
            f, walls_ptr_offset
        )
        for sect in data.sectors:
            prev_v2 = None
            if len(sect.walls) == 0:
                continue
            first_v1 = sect.walls[0].v1
            for wall in sect.walls:
                if prev_v2 is not None:
                    if prev_v2 != wall.v1:
                        messagebox.showerror(
                            title="Error",
                            message="Sector {} is not complete".format(sect.index)
                        )
                    
                write_u16(f, wall.v1.index)
                prev_v2 = wall.v2 
            write_u16(f, first_v1.index)

        patch_pointer_to_current_offset(
            f, portals_ptr_offset
        )
        for sect in data.sectors:
            for wall in sect.walls:
                write_s16(f, wall.adj_sector_idx)

        patch_pointer_to_current_offset(
            f, wall_colors_ptr_offset
        )
        for sect in data.sectors:
            for wall in sect.walls:
                f.write(struct.pack(
                    ">BBBB", 
                    wall.texture_idx, 
                    wall.up_color,
                    wall.low_color,
                    wall.mid_color
                ))
        align(f)
        
        patch_pointer_to_current_offset(
            f, vertexes_ptr_offset
        )
        for vert in data.vertexes:
            write_s16(f, int(vert.x*1.3))
            write_s16(f, int((-vert.y)*1.3))


        patch_pointer_to_current_offset(
            f, wall_norm_quads_ptr_offset
        )
        for sect in data.sectors:
            for wall in sect.walls:
                write_u8(f, wall.normal_quadrant_int())
        align(f)

        patch_pointer_to_current_offset(
            f, name_ptr_offset
        )
        for char in data.name:
            f.write(str.encode(char))
        # null terminate name
        f.write(b'\x00')

    return last_exported_rom_file



def export_and_launch(set_launch_flags):
    rom_name = export_map_to_rom(set_launch_flags=set_launch_flags)
    launch_emulator(cur_state.emulator_path, rom_name)
                

if __name__ == '__main__':
    
    root = tk.Tk()
    root.withdraw()

    imgui.create_context()
    io = imgui.get_io()

    if len(sys.argv) > 1:
        with open(sys.argv[1], "rb") as f:
            load_map_from_file(f)
            f.close()

    main_sdl2()
    
    atexit.register(root.destroy)
