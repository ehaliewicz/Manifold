import os 
import sys

print(os.getcwd())
print(sys.executable)

is_nuitka = "__compiled__" in globals()

if "editor.exe" in sys.executable or is_nuitka:
    print("running executable")
    cur_path = ".\\"
    os.environ['PYSDL2_DLL_PATH'] = cur_path #".\\" 
else:
    print("running through python")
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
from tkinter import filedialog,messagebox

import atexit
import pickle


import defaults
import line
import music
import palette
import pvs
import rom
import script
import sector
import sector_group
import things
import trigger
import tree
import undo
import utils
import vertex

import configparser

# commands

class Mode(Enum):
    SECTOR = 'Sector'
    SECTOR_GROUP = 'Sector Groups'
    LINE = 'Line'
    VERTEX = 'Vertex'
    SCRIPT = 'Script'
    TRIGGER = 'Trigger'
    TEXTURE = 'Texture'
    TREE = 'Tree'
    PVS = 'PVS'
    MUSIC = 'Music'
    PALETTE = 'Palette'
    DEFAULTS = 'Defaults'
    THING_DEFS = 'Thing Defs'
    THINGS = 'Things'


class Map():
    def __init__(self, 
    default_sprite,
    name="placeholder name", 
    sectors=None, 
    vertexes=None,
    music_path=""):
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
        self.music_path = music_path
        self.palette = palette.DEFAULT_PALETTE
        self.thing_defs = [things.ThingDef(default_sprite) for i in range(31)]
        self.things = []
        self.sector_groups = []

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
        width, height = 1600, 900
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
        SDLK_e: lambda: rom.export_map_to_rom(cur_path, cur_state, False),
        SDLK_r: lambda: rom.export_and_launch(cur_path, cur_state, True),
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
        #utils.set_root_window_draw_list(imgui.get_window_draw_list())
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
        self.cur_sector = None
        self.cur_wall = None
        self.cur_vertex = None
        self.cur_thing = None
        self.cur_sector_group = None
        self.cur_sector_pvs = None

        self.camera_x = 0
        self.camera_y = 0
        self.zoom = 0
        

        self.emulator_path = ""
        self.xgmtool_path = ""
        self.textures_path = "textures/"
        self.music_tracks_path = "music/"
        self.sprites_path = "sprites/"

        self.load_config()
        tex_files = utils.get_texture_files(self)
        sprite_files = utils.get_sprite_files(self)

        self.hovered_item = None

        self.default_up_color = 3
        self.default_low_color = 3
        self.default_mid_color = 0
        self.default_floor_color = 4
        self.default_ceil_color = 5
        self.default_texture_file = tex_files[0]
        self.default_sprite_file = sprite_files[0]
        self.default_thing_type = 0

        self.map_data = Map(self.default_sprite_file)

    def load_config(self):
        conf_file_path = os.path.join(cur_path, "conf.ini")
        conf_exists = os.path.exists(conf_file_path)
        if not conf_exists:
            print("No configuration file, using defaults!")
            return 
        config = configparser.ConfigParser()
        config.read(conf_file_path)
        self.emulator_path = config.get("Default Settings", "emulator-path")
        self.xgmtool_path = config.get("Default Settings", "xgmtool-path")
        self.textures_path = config.get("Default Settings", "textures-path")
        self.sprites_path = config.get("Default Settings", "sprites-path")
        self.music_tracks_path = config.get("Default Settings", "music-tracks-path")

def reset_state():
    global cur_state, last_saved_map_file
    rom.reset_last_exported_rom_file()

    last_saved_map_file = None
    cur_state = State()
    

def add_new_wall(v1, v2):
    undo.push_state(cur_state)
    new_wall = line.Wall(v1=v1, v2=v2, 
                         sector_idx=cur_state.cur_sector.index, adj_sector_idx=-1, 
                         default_tex=cur_state.default_texture_file)
    new_wall.low_color = cur_state.default_low_color
    new_wall.up_color = cur_state.default_up_color
    new_wall.mid_color = cur_state.default_mid_color
    new_wall.texture_file = cur_state.default_texture_file

    cur_state.cur_sector.add_wall(new_wall)

    new_wall.link_with_adjacent_wall_if_exists(cur_state.map_data)
    
    
    return new_wall
    
def add_new_vertex(x,y):
    undo.push_state(cur_state)
    num_verts = len(cur_state.map_data.vertexes)
    new_vert = vertex.Vertex(x, y, index=num_verts)
    cur_state.map_data.vertexes.append(new_vert)

    return new_vert
    
def add_new_thing(x, y):
    undo.push_state(cur_state)
    num_things = len(cur_state.map_data.things)
    
    new_thing = things.Thing(cur_state.default_thing_type, cur_state.cur_sector.index, x, y, cur_state.cur_sector_group.floor_height, index=num_things)
    cur_state.map_data.things.append(new_thing)

    return new_thing



MODE_DRAW_FUNCS = {
    Mode.SECTOR: sector.draw_sector_mode,
    Mode.SECTOR_GROUP: sector_group.draw_sector_group_mode,
    Mode.LINE: line.draw_line_mode,
    Mode.VERTEX: vertex.draw_vert_mode,
    Mode.SCRIPT: script.draw_script_mode,
    Mode.TRIGGER: trigger.draw_trigger_mode,
    Mode.TREE: tree.draw_tree_mode,
    Mode.PVS: pvs.draw_pvs_mode,
    Mode.MUSIC: music.draw_music_mode,
    Mode.PALETTE: palette.draw_palette_mode,
    Mode.DEFAULTS: defaults.draw_defaults_mode,
    Mode.THING_DEFS: things.draw_thing_defs_mode,
    Mode.THINGS: things.draw_things_mode
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

thing_default = (0,.8,0,.8)
thing_highlight = (0.7,1,0,1)


portal_default = (1,0,0,0.3)
portal_highlight = (1,1,0,0.3)
wall_default = (1,0,0,1)
wall_highlight = (1,1,1,0.5)

concave_wall = (1,0,0,1)
concave_portal = (1,0,0,.3)
convex_wall = (0,0,1,1)
convex_portal = (0,0,1,.3)

def draw_map_vert(draw_list, vert, highlight=False):
    #if highlight:
    #    color = vert_highlight
    #else:
    color = vert_default

    cam_x = cur_state.camera_x
    cam_y = cur_state.camera_y
    if highlight:
        draw_list.add_circle_filled(vert.x-cam_x, vert.y-cam_y, 4, imgui.get_color_u32_rgba(*vert_highlight), num_segments=12)
    draw_list.add_circle_filled(vert.x-cam_x, vert.y-cam_y, 2, imgui.get_color_u32_rgba(*color), num_segments=12)

def draw_thing(draw_list, thing, highlight=False):
    if highlight:
        color = thing_highlight
    else:
        color = thing_default

    cam_x = cur_state.camera_x
    cam_y = cur_state.camera_y
    draw_list.add_circle_filled(thing.x-cam_x, thing.y-cam_y, 5, imgui.get_color_u32_rgba(*color), num_segments=12)


    
def draw_map_wall(draw_list, wall, sect_group_color, highlight=False, tree_mode=False, concave_sector=False, ):
    is_portal = wall.adj_sector_idx != -1
    tbl = [
        # not highlighted
        wall_default,
        portal_default,
        # highlighted
        wall_highlight,
        portal_highlight,
    ]
           
    color = sect_group_color
    (r,g,b,_) = color 
    if is_portal:
        color = (r,g,b,.25)
    #color = tbl[(highlight<<1 | is_portal)]

        
    v1 = wall.v1
    v2 = wall.v2

    ((n1x,n1y),(n2x,n2y)) = wall.centered_normal()
    
    cam_x = cur_state.camera_x
    cam_y = cur_state.camera_y
     

    #if tree_mode:
    #    color = None
    #    if concave_sector:
    #        color = concave_portal if is_portal else concave_wall
    #    else:
    #        color = convex_portal if is_portal else convex_wall 
    #        
    #    draw_list.add_line(v1.x-cam_x, v1.y-cam_y, v2.x-cam_x, v2.y-cam_y, imgui.get_color_u32_rgba(*color), 1.0)
    #    draw_list.add_line(n1x-cam_x, n1y-cam_y, n2x-cam_x, n2y-cam_y, imgui.get_color_u32_rgba(*color), 1.0)
    #else:

    draw_list.add_line(v1.x-cam_x, v1.y-cam_y, v2.x-cam_x, v2.y-cam_y, imgui.get_color_u32_rgba(*color), 1.0)
    draw_list.add_line(n1x-cam_x, n1y-cam_y, n2x-cam_x, n2y-cam_y, imgui.get_color_u32_rgba(*color), 1.0)
    
    if highlight:
        draw_list.add_line(v1.x-cam_x, v1.y-cam_y, v2.x-cam_x, v2.y-cam_y, imgui.get_color_u32_rgba(*wall_highlight), 4.0)
        draw_list.add_line(n1x-cam_x, n1y-cam_y, n2x-cam_x, n2y-cam_y, imgui.get_color_u32_rgba(*wall_highlight), 4.0)


    

def draw_sector(draw_list, sector, sect_group, highlight=False, cur_sector_pvs=None):
    tree_mode = cur_state.mode == Mode.TREE

    concave_sector = False
    
    if tree_mode:
        concave_sector = not sector.is_convex()  
        
    for wall in sector.walls:
        wall_selected = cur_state.mode == Mode.LINE and cur_state.cur_wall == wall
        wall_hovered = cur_state.hovered_item == wall

        wall_in_pvs = cur_state.mode == Mode.PVS and cur_sector_pvs is not None and (wall in (cur_sector_pvs[wall.sector_idx] if wall.sector_idx in cur_sector_pvs else set()))
        
        draw_map_wall(draw_list, wall, sect_group.color, (highlight or wall_selected or wall_hovered or wall_in_pvs), tree_mode, concave_sector)

        
# returns true if hovered
def draw_map():
    draw_list = imgui.get_window_draw_list()


    for thing in cur_state.map_data.things:
        draw_thing(
            draw_list, thing, 
            highlight = (
                (cur_state.mode == Mode.THINGS and thing == cur_state.cur_thing) 
                or (thing == cur_state.hovered_item))
        )
    #for wall in cur_state.map_data.walls:
    #    draw_map_wall(draw_list, wall, highlight = ((cur_state.mode == Mode.LINE and wall==cur_state.cur_wall) or wall == cur_state.hovered_item))
    
    for sect in cur_state.map_data.sectors:
        sect_group = cur_state.map_data.sector_groups[sect.sector_group_idx]
        is_group_selected = (cur_state.mode == Mode.SECTOR_GROUP and sect_group == cur_state.cur_sector_group)
        is_selected = (cur_state.mode == Mode.SECTOR and sect == cur_state.cur_sector) or is_group_selected
        
        is_group_hovered = isinstance(cur_state.hovered_item, sector_group.SectorGroup) and sect.sector_group_idx == cur_state.hovered_item.index
        is_hovered = (cur_state.hovered_item == sect or is_group_hovered)

        draw_sector(draw_list, sect, sect_group, highlight = is_selected or is_hovered, cur_sector_pvs=cur_state.cur_sector_pvs)
    
    for vertex in cur_state.map_data.vertexes:
        
        draw_map_vert(draw_list, vertex, highlight = ((cur_state.mode == Mode.VERTEX and vertex == cur_state.cur_vertex) or vertex == cur_state.hovered_item))
        
    # draw current sector
    #if (cur_state.mode == Mode.SECTOR and cur_state.cur_sector is not None):
    #    sect_group = cur_state.sector
    #    draw_sector(draw_list, cur_state.cur_sector, highlight=True)

    #if cur_state.hovered_item and isinstance(cur_state.hovered_item, sector.Sector):
    #    draw_sector(draw_list, cur_state.hovered_item, highlight=True)

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
                    cur_state.cur_sector_group = cur_state.map_data.sector_groups[cur_state.cur_sector.sector_group_idx]

        for thing in cur_state.map_data.things:
            if thing.point_collides(x, y):
                cur_state.cur_thing = thing
                
                if button == LEFT_BUTTON:
                    cur_state.mode = Mode.THINGS
                    return
        return
        
    if not cur_state.cur_sector:
        return


    # add a new vertex and line to the sector
    

    if clicked_vertex is None:
        ix -= ix%10
        iy -= iy%10
        if cur_state.mode == Mode.THINGS:
            cur_state.cur_thing = add_new_thing(ix, iy)
        else:
            cur_state.cur_vertex = add_new_vertex(ix, iy)
    
        

    if prev_cur is not None and prev_cur != cur_state.cur_vertex:
        cur_state.cur_wall = add_new_wall(prev_cur, cur_state.cur_vertex)
        

last_frame_x = None
last_frame_y = None
got_hold_last_frame = False



def on_frame():
    global cur_state, last_frame_x, last_frame_y, got_hold_last_frame

    global last_saved_map_file

    imgui.set_next_window_position(0, 0)
    io = imgui.get_io()
    imgui.set_next_window_size(io.display_size.x, io.display_size.y)
    imgui.begin("Map",
                flags=imgui.WINDOW_NO_TITLE_BAR|imgui.WINDOW_NO_RESIZE|imgui.WINDOW_NO_COLLAPSE|imgui.WINDOW_NO_BRING_TO_FRONT_ON_FOCUS)

    utils.set_root_window_draw_list(imgui.get_window_draw_list())
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
                rom.export_map_to_rom(cur_path, cur_state)

            _, selected_export_as = imgui.menu_item(
                "Export to ROM as", "", False, True)
            if selected_export_as:
                rom.export_map_to_rom_as(cur_path, cur_state, False)
            

            _,selected_export_launch = imgui.menu_item(
                "Export to ROM and launch", "Ctrl-r", False, True
            )
            if selected_export_launch:
                rom.export_and_launch(cur_path, cur_state, set_launch_flags=True)

            _, selected_export_launch_as = imgui.menu_item(
                "Export to ROM and launch as", "", False, True
            )
            if selected_export_launch_as:
                rom.export_and_launch_as(cur_path, cur_state, set_launch_flags=True)

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

def old_wall_to_new_wall(wall, default_texture):
    nw = line.Wall(
        old_vertex_to_new_vertex(wall.v1), 
        old_vertex_to_new_vertex(wall.v2), 
        wall.sector_idx, wall.adj_sector_idx,
        default_tex=default_texture
    )
    nw.low_color = wall.low_color
    nw.mid_color = wall.mid_color
    nw.up_color = wall.up_color
    #nw.texture_idx = wall.texture_idx
    if hasattr(wall, 'texture_file'):
        nw.texture_file = wall.texture_file
    return nw


def old_walls_to_new_walls(walls, default_texture):
    return [old_wall_to_new_wall(w, default_texture) for w in walls]


def old_sectors_to_new_sectors(sectors, default_texture):
    return [sector.Sector(
                index=s.index, walls=old_walls_to_new_walls(s.walls, default_texture), 
                sect_group_index=s.sector_group_idx if hasattr(s, 'sector_group_idx') else idx ) for idx,s in enumerate(sectors)] 
                
def old_sectors_to_new_sector_groups(sectors):
    return [sector_group.SectorGroup(
        idx, 
        type=0, 
        light_level=0, orig_height=0, ticks_left=0, state=0, 
        floor_height=sect.floor_height, ceil_height=sect.ceil_height, floor_color=sect.floor_color, ceil_color=sect.ceil_color,
        triggers=None) for idx,sect in enumerate(sectors)]
                #floor_height=s.floor_height, ceil_height=s.ceil_height,
                #floor_color=s.floor_color, ceil_color=s.ceil_color) for s in sectors]
        
        
def load_map_from_file(f):
    global cur_state
    reset_state()
    try:
        old_state = pickle.load(f)
    except Exception as e:
       _ = tk.messagebox.showerror("Error!", e)
       return 
    old_map = old_state.map_data


    default_tex = cur_state.default_texture_file
    default_sprite = cur_state.default_sprite_file

    new_map = Map(default_sprite=default_sprite,
                  name=old_map.name,
                  sectors=old_sectors_to_new_sectors(old_map.sectors, default_tex),
                  vertexes=old_vertexes_to_new_vertexes(old_map.vertexes),
                  music_path=old_map.music_path)                      

    if hasattr(old_map, 'palette'):
        new_map.palette = old_map.palette

    if hasattr(old_map, 'thing_defs'):
        new_map.thing_defs = old_map.thing_defs

    if hasattr(old_map, "things"):
        new_map.things = old_map.things

    if hasattr(old_map, "sector_groups"):
        new_map.sector_groups = old_map.sector_groups
        if len(new_map.sector_groups) > 0 and (not hasattr(new_map.sector_groups[0], 'color')):
            for sctg in new_map.sector_groups:
                sctg.color = utils.random_bright_color()
    else:
        new_map.sector_groups = old_sectors_to_new_sector_groups(old_map.sectors)

    cur_state.map_data = new_map

    cur_state.cur_sector = None
    cur_state.cur_vertex = None
    cur_state.cur_wall = None
    cur_state.cur_thing = None

        
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


if __name__ == '__main__':
        
    root = tk.Tk()
    root.withdraw()
    atexit.register(root.destroy)
    try:
        reset_state()
    except Exception as e:
        _ = tk.messagebox.showerror("Error!", e)
        exit()


    imgui.create_context()
    io = imgui.get_io()

    if len(sys.argv) > 1:
        with open(sys.argv[1], "rb") as f:
            load_map_from_file(f)
            f.close()

    main_sdl2()

