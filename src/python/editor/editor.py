#import glfw
import atexit
import ctypes
import pickle
import sys
import tkinter as tk
from enum import Enum
from tkinter import filedialog

import OpenGL.GL as gl
import imgui
# from imgui.integrations.glfw import GlfwRenderer
from imgui.integrations.sdl2 import SDL2Renderer
from sdl2 import *

import line
import render_3d
import script
import sector
import texture
import trigger
import vertex


# commands

class Mode(Enum):
    SECTOR = 'Sector'
    LINE = 'Line'
    VERTEX = 'Vertex'
    SCRIPT = 'Script'
    TRIGGER = 'Trigger'
    TEXTURE = 'Texture'
    PREVIEW = 'Preview'


    


class Map():
    def __init__(self, name="placeholder name", sectors=None, vertexes=None):
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

        NUM_SECTOR_ATTRIBUTES = 8

        res = """#include <genesis.h>
#include "colors.h"
#include "portal_map.h"
#include "vertex.h"

"""
        
                
        res += "static const s16 sectors[{}] =".format(num_sectors * NUM_SECTOR_ATTRIBUTES) + "{\n"

        wall_offset = 0
        portal_offset = 0
        for sect in self.sectors:
            sect_num_walls = len(sect.walls)
            res += "    {}, {}, {}, {}<<4, {}<<4, {}, {}, {},\n".format(wall_offset, portal_offset, sect_num_walls,
                                                                    sect.floor_height, sect.ceil_height,
                                                                    sect.floor_color, sect.ceil_color, sect.flags)
            
            wall_offset += sect_num_walls+1
            portal_offset += sect_num_walls
            
        res += "};\n\n"
            
        

        # we can have up to 65536 vertexes
        res += "static const u16 walls[{}]".format(num_walls+num_sectors) + " = {\n"
        for sect in self.sectors:
            prev_v2 = None
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
        

        res += "static const wall_col wall_colors[{}] =".format(num_walls) + "{\n"
        for sect in self.sectors:
            for wall in sect.walls:
                mcol = "0x{}{}".format(wall.mid_color, wall.mid_color)
                ucol = "0x{}{}".format(wall.up_color, wall.up_color)
                dcol = "0x{}{}".format(wall.low_color, wall.low_color)
                if wall.adj_sector_idx == -1:
                    res += "    {" + ".mid_col = {}".format(mcol) + "},\n"
                else:
                    res += "    {" + ".upper_col = {}, .lower_col = {}".format(ucol, dcol) + "},\n"
                    
        res += "};\n\n"

        res += "#define VERT(x1,y1) { .x = (x1 * 2), .y = ((-y1) * 2) } \n"
        
        res += "static const vertex vertexes[{}]".format(num_vertexes) + " = {\n"
        for vert in self.vertexes:
            res += "    VERT({},{}),\n".format(vert.x, vert.y)
        res += "};\n"
        
        
        res += "const portal_map {} ".format(self.name.replace(" ", "_")) + " = {\n"
        res += "    .num_sectors = {},\n".format(num_sectors)
        res += "    .num_walls = {},\n".format(num_walls)
        res += "    .num_verts = {},\n".format(num_vertexes)
        res += "    .sectors = sectors,\n"
        res += "    .walls = walls,\n"
        res += "    .portals = portals,\n"
        res += "    .vertexes = vertexes,\n"
        res += "    .wall_colors = wall_colors,\n"
        res += "    .wall_norm_quadrants = wall_normal_quadrants\n"
        res += "};"
                
        
        return res


    
def main_sdl2():
    def pysdl2_init():
        width, height = 1280, 800
        window_name = "Map Edit"
        if SDL_Init(SDL_INIT_EVERYTHING) < 0:
            print("Error: SDL could not initialize! SDL Error: " + SDL_GetError())
            exit(1)
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
            exit(1)
        gl_context = SDL_GL_CreateContext(window)
        if gl_context is None:
            print("Error: Cannot create OpenGL Context! SDL Error: " + SDL_GetError())
            exit(1)
        SDL_GL_MakeCurrent(window, gl_context)
        if SDL_GL_SetSwapInterval(1) < 0:
            print("Warning: Unable to set VSync! SDL Error: " + SDL_GetError())
            exit(1)
        return window, gl_context

    window, gl_context = pysdl2_init()

    renderer = SDL2Renderer(window)

    running = True

    event = SDL_Event()

    render_3d.init_sdl_window()

    
    while running:
        while SDL_PollEvent(ctypes.byref(event)) != 0:
            if event.type == SDL_QUIT:
                running = False
                break
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
        

        self.hovered_item = None

        
cur_state = State()

        
def add_new_wall(v1, v2):
    num_walls = len(cur_state.cur_sector.walls)
    new_wall = line.Wall(v1=v1, v2=v2, sector_idx=cur_state.cur_sector.index, adj_sector_idx=-1)
    cur_state.cur_sector.walls.append(new_wall)
    return new_wall
    
def add_new_vertex(x,y):
    num_verts = len(cur_state.map_data.vertexes)
    new_vert = vertex.Vertex(x, y, index=num_verts)
    cur_state.map_data.vertexes.append(new_vert)
    return new_vert
    



MODE_DRAW_FUNCS = {
    Mode.SECTOR: sector.draw_sector_mode,
    Mode.PREVIEW: render_3d.draw_preview,
    Mode.LINE: line.draw_line_mode,
    Mode.VERTEX: vertex.draw_vert_mode,
    Mode.SCRIPT: script.draw_script_mode,
    Mode.TRIGGER: trigger.draw_trigger_mode,
    Mode.TEXTURE: texture.draw_texture_mode
}


def draw_mode():
    
    changed, text_val = imgui.input_text("Name: ", cur_state.map_data.name, buffer_length=64)
    if changed:
        cur_state.map_data.name = text_val
        
        
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



def draw_map_vert(draw_list, vert, highlight=False):
    if highlight:
        color = vert_highlight
    else:
        color = vert_default

    cam_x = cur_state.camera_x
    cam_y = cur_state.camera_y
    draw_list.add_circle_filled(vert.x-cam_x, vert.y-cam_y, 2, imgui.get_color_u32_rgba(*color), num_segments=12)


    
def draw_map_wall(draw_list, wall, highlight=False):
    is_portal = wall.adj_sector_idx != -1
    tbl = [
        # not highlighted
        wall_default,
        portal_default,
        # highlighted
        wall_highlight,
        portal_highlight]
           
    
    color = tbl[(highlight<<1 | is_portal)]

        
    v1 = wall.v1
    v2 = wall.v2

    ((n1x,n1y),(n2x,n2y)) = wall.centered_normal()
    
    cam_x = cur_state.camera_x
    cam_y = cur_state.camera_y
     
    draw_list.add_line(v1.x-cam_x, v1.y-cam_y, v2.x-cam_x, v2.y-cam_y, imgui.get_color_u32_rgba(*color), 1.0)
    draw_list.add_line(n1x-cam_x, n1y-cam_y, n2x-cam_x, n2y-cam_y, imgui.get_color_u32_rgba(*color), 1.0)
    

def draw_sector(draw_list, sector, highlight=False):
    for wall in sector.walls:
        wall_selected = cur_state.mode == Mode.LINE and cur_state.cur_wall == wall
        wall_hovered = cur_state.hovered_item == wall

        draw_map_wall(draw_list, wall, (highlight or wall_selected or wall_hovered))
        

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
                "Quit", 'Cmd+Q', False, True
            )
            if clicked_quit:
                exit(1)
                
            clicked_load, selected_load = imgui.menu_item(
                "Load", "", False, True
            )
            if selected_load:
                load_map()

                
            clicked_save, selected_save = imgui.menu_item(
                "Save", "", False, True
            )
            if selected_save:
                save_map()
                
            clicked_export, selected_export = imgui.menu_item(
                "Export", "", False, True
            )
            if selected_export:
                export_map()

            clicked_reset, selected_reset = imgui.menu_item(
                "Reset", "", False, True
            )
            if selected_reset:
                cur_state = State()
            
            
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
        print("clicked left {} right {}".format(left_button_clicked, right_button_clicked))

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
        print("interpreting click at {},{}".format(x,y))
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
        
        
def load_map_from_file(f):
    global cur_state
    old_state = pickle.load(f)
    #cur_state = old_state
    old_map = old_state.map_data

    new_sectors = [sector.Sector(index=s.index, walls=s.walls,
                                 floor_height=s.floor_height, ceil_height=s.ceil_height)
                   for s in old_map.sectors]

    new_map = Map(name=old_map.name,
                  sectors=new_sectors,
                  vertexes=old_map.vertexes)                          

    cur_state = old_state
    cur_state.map_data = new_map

    cur_state.cur_sector = None
    cur_state.cur_vertex = None
    cur_state.cur_wall = None
    
        

def save_map():
    f = filedialog.asksaveasfile(mode="wb")
    if f is not None:
        pickle.dump(cur_state, f)
        f.close()
    print(f)

    
def export_map():
    f = filedialog.asksaveasfile(mode="w")
    if f is not None:
        f.write(cur_state.map_data.generate_c_from_map())
        f.close()

        
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
