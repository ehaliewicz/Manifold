# import glfw
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
import sector_group
import texture
import trigger
import vertex

# commands
from src.python.editor import map_db


class Mode(Enum):
    SECTOR = 'Sector'
    SECTOR_GROUP = 'Sector Group'
    LINE = 'Line'
    VERTEX = 'Vertex'
    SCRIPT = 'Script'
    TRIGGER = 'Trigger'
    TEXTURE = 'Texture'
    PREVIEW = 'Preview'


class Map():
    def __init__(self, name="placeholder name"):
        self.name = name
        self.num_sectors = 0
        self.num_walls = 0
        self.num_verts = 0
        self.sectors = []
        self.sector_types = []
        self.sector_params = []
        self.walls = []
        self.portals = []
        self.wall_colors = []
        self.vertexes = []
        self.wall_norm_quadrants = []
        self.has_pvs = 0
        self.pvs = []
        self.raw_pvs = []

    def generate_c_from_map(self):
        num_sectors = len(self.sectors)
        num_vertexes = len(self.vertexes)
        num_walls = sum(len(sect.walls) for sect in self.sectors)

        res = """#include <genesis.h>
#include "colors.h"
#include "portal_map.h"
#include "vertex.h"

"""

        res += "static s16 sectors[{}] =".format(num_sectors * sector.NUM_SECTOR_ATTRIBUTES) + "{\n"

        wall_offset = 0
        portal_offset = 0
        for sect in self.sectors:
            sect_num_walls = len(sect.walls)
            res += "    {}, {}, {}, {}<<4, {}<<4, {}, {}, {},\n".format(wall_offset, portal_offset, sect_num_walls,
                                                                        sect.floor_height, sect.ceil_height,
                                                                        sect.floor_color, sect.ceil_color, sect.flags)

            wall_offset += sect_num_walls + 1
            portal_offset += sect_num_walls

        res += "};\n\n"

        # we can have up to 65536 vertexes
        res += "static const u16 walls[{}]".format(num_walls + num_sectors) + " = {\n"
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
                mcol = "0x{}".format(wall.mid_color, wall.mid_color)
                ucol = "0x{}".format(wall.up_color, wall.up_color)
                dcol = "0x{}".format(wall.low_color, wall.low_color)
                if wall.adj_sector_idx == -1:
                    res += "    {" + ".mid_col = {}".format(mcol) + "},\n"
                else:
                    res += "    {" + ".upper_col = {}, .lower_col = {}".format(ucol, dcol) + "},\n"

        res += "};\n\n"

        res += "#define VERT(x1,y1) { .x = (x1 * 2), .y = ((-y1) * 2) } \n"

        res += "static const vertex vertexes[{}]".format(num_vertexes) + " = {\n"
        for vert in self.vertexes:
            res += "    VERT({},{}),\n".format(vert.x, vert.y)
        res += "};\n\n"

        res += "static const sector_type sector_types[{}]".format(num_sectors) + " = {\n"
        for sect in self.sectors:
            res += "{},".format(sect.type)
        res += "};\n\n"

        res += "static sector_param sector_params[{}]".format(num_sectors) + " = {\n"
        for sect in self.sectors:
            res += "{"
            res += ".light={}, .orig_height={}<<4, .ticks_left={}, .state={}".format(
                sect.params.light, sect.params.orig_height, sect.params.ticks_left, sect.params.state
            )
            res += "},\n"
        res += "};\n\n"

        res += "const portal_map {} ".format(self.name.replace(" ", "_")) + " = {\n"
        res += "    .num_sectors = {},\n".format(num_sectors)
        res += "    .num_walls = {},\n".format(num_walls)
        res += "    .num_verts = {},\n".format(num_vertexes)
        res += "    .sectors = sectors,\n"
        res += "    .walls = walls,\n"
        res += "    .portals = portals,\n"
        res += "    .vertexes = vertexes,\n"
        res += "    .wall_colors = wall_colors,\n"
        res += "    .wall_norm_quadrants = wall_normal_quadrants,\n"
        res += "    .sector_params = sector_params,\n"
        res += "    .sector_types = sector_types\n"
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
                                  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE)
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
            elif event.type == SDL_WINDOWEVENT and event.window.event == SDL_WINDOWEVENT_CLOSE:
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
    def __init__(self, map_data):
        self.mode = Mode.SECTOR
        self.map_data = map_data
        self.cur_sector = -1
        self.cur_sector_group = -1
        self.cur_wall = -1
        self.cur_vertex = -1

        self.camera_x = 0
        self.camera_y = 0
        self.zoom = 0

        self.hovered_item = None
        self.hovered_item_type = None


cur_state = None

"""
def add_new_wall(v1, v2):
    sect = cur_state.cur_sector
    if sect == cur_state.map_data.num_sectors-1:
        num_walls = cur_state.map_data.num_walls
        cur_state.map_data.walls.append(v1)
        cur_state.map_data.walls.append(v2)
        cur_state.map_data.num_walls += 1
        cur_state.map_data.portals.append(-1)
        num_walls_idx = sect * sector.NUM_SECTOR_ATTRIBUTES + sector.NUM_WALLS_IDX
        cur_state.map_data.sectors[num_walls_idx] += 1
        return num_walls
    else:
        raise Exception("Adding walls to non-last sectors Unsupported")
    
def add_new_vertex(x,y):
    num_verts = cur_state.map_data.num_verts
    new_vert = vertex.Vertex(x, y)
    cur_state.map_data.vertexes.append(new_vert)
    cur_state.map_data.num_verts += 1
    print(cur_state.map_data.vertexes)
    return num_verts
"""

MODE_DRAW_FUNCS = {
    Mode.SECTOR: sector.draw_sector_mode,
    Mode.SECTOR_GROUP: sector_group.draw_sector_group_mode,
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
        # if can_switch_to(k.value):
        if imgui.radio_button(k.value, cur_state.mode == k):
            cur_state.mode = k

    MODE_DRAW_FUNCS[cur_state.mode](cur_state)


vert_default = (1, 1, 1, 1)
vert_highlight = (1, 0, 1, 1)
vert_select = (1, 1, 0, 1)

portal_default = (1, 0, 0, 0.3)
portal_highlight = (1, 1, 0, 0.3)
wall_default = (1, 0, 0, 1)
wall_highlight = (1, 0, 1, 1)
wall_selected = (1, 1, 0, 1)


def draw_map_vert(draw_list, vert, highlight=False):
    if highlight:
        color = vert_highlight
    else:
        color = vert_default

    cam_x = cur_state.camera_x
    cam_y = cur_state.camera_y
    draw_list.add_circle_filled(vert.x - cam_x, vert.y - cam_y, 2, imgui.get_color_u32_rgba(*color), num_segments=12)



def draw_map_wall(draw_list, wall_idx, highlight=False):
    sect_idx = map_db.get_sect_for_wall(cur_state.map_data, wall_idx)
    portal_idx = map_db.get_portal_for_wall(cur_state.map_data, wall_idx)

    adj_sector_idx = cur_state.map_data.portals[portal_idx]

    is_portal = adj_sector_idx != -1
    tbl = [
        # not highlighted
        wall_default,
        portal_default,
        # highlighted
        wall_highlight,
        portal_highlight]

    color = tbl[(highlight << 1 | is_portal)]

    wall = map_db.get_line(cur_state.map_data, sect_idx, wall_idx)
    v1 = wall.v1
    v2 = wall.v2

    ((n1x, n1y), (n2x, n2y)) = wall.centered_normal(cur_state.map_data)

    cam_x = cur_state.camera_x
    cam_y = cur_state.camera_y

    draw_list.add_line(v1.x - cam_x, v1.y - cam_y, v2.x - cam_x, v2.y - cam_y, imgui.get_color_u32_rgba(*color), 1.0)
    draw_list.add_line(n1x - cam_x, n1y - cam_y, n2x - cam_x, n2y - cam_y, imgui.get_color_u32_rgba(*color), 1.0)


def draw_sector(draw_list, draw_sect, highlight=False):
    #base_idx = draw_sect * map_db.NUM_SECTOR_PARAMS
    portal_base_idx = map_db.get_sector_constant(cur_state.map_data, draw_sect, map_db.PORTAL_OFFSET_IDX)
    #portal_base_idx = cur_state.map_data.sectors[base_idx + map_db.PORTAL_OFFSET_IDX]

    for idx, wall in enumerate(map_db.get_all_lines_for_sector(cur_state.map_data, draw_sect)):
        cur_wall_idx = wall.idx
        cur_portal_idx = portal_base_idx + idx
        wall_selected = cur_state.mode == Mode.LINE and cur_state.cur_wall == cur_wall_idx
        wall_hovered = cur_state.hovered_item == cur_wall_idx and cur_state.hovered_item_type == "Line"

        draw_map_wall(draw_list, cur_wall_idx, (highlight or wall_selected or wall_hovered))


# returns true if hovered
def draw_map(cur_state):
    draw_list = imgui.get_window_draw_list()

    vertexes = map_db.get_all_vertexes(cur_state.map_data)

    for idx, vertex in enumerate(vertexes):
        is_selected =  cur_state.mode == Mode.VERTEX and idx == cur_state.cur_vertex
        is_hovered = vertex == cur_state.hovered_item and cur_state.hovered_item_type == "Vertex"
        draw_map_vert(draw_list, vertex, highlight=(is_selected or is_hovered))


    # for wall in cur_state.map_data.walls:
    #    draw_map_wall(draw_list, wall, highlight = ((cur_state.mode == Mode.LINE and wall==cur_state.cur_wall) or wall == cur_state.hovered_item))

    for sect in range(cur_state.map_data.num_sectors):
        is_selected = cur_state.mode == Mode.SECTOR and sect == cur_state.cur_sector
        is_hovered = cur_state.hovered_item == sect and cur_state.hovered_item_type == "Sector"

        this_sectors_group = map_db.get_sector_constant(cur_state.map_data, sect, map_db.SECT_GROUP_IDX)
        is_group_hovered = cur_state.hovered_item_type == "Sector Group" and cur_state.hovered_item == this_sectors_group

        #is_hovered = cur_state.hovered_item == get_sector
        draw_sector(draw_list, sect, highlight=is_selected or is_hovered or is_group_hovered)

    if (cur_state.mode == Mode.SECTOR and cur_state.cur_sector != -1):
        draw_sector(draw_list, cur_state.cur_sector, highlight=True)

    if (cur_state.mode == Mode.LINE and cur_state.cur_wall != -1):
        draw_map_wall(draw_list, cur_state.cur_wall, True)
    #if cur_state.hovered_item and cur_state.mode == Mode.SECTOR:
    #    # if cur_state.hovered_item and isinstance(cur_state.hovered_item, sector.Sector):
    #    draw_sector(draw_list, cur_state.hovered_item, highlight=True)


LEFT_BUTTON = 0
RIGHT_BUTTON = 1


def interpret_click(x, y, button):
    # global cur_vertex, cur_wall, cur_mode
    ix = int(x)
    iy = int(y)

    prev_cur = cur_state.cur_vertex
    clicked_vertex = -1
    for idx, vert in enumerate(map_db.get_all_vertexes(cur_state.map_data)):
        if vert.point_collides(x, y):
            cur_state.cur_vertex = idx

            if button == LEFT_BUTTON:
                cur_state.mode = Mode.VERTEX
                return
            elif button == RIGHT_BUTTON:
                clicked_vertex = idx

    # then walls
    if button == LEFT_BUTTON:
        for sect_idx in map_db.get_all_sectors(cur_state.map_data):
            for wall in map_db.get_all_lines_for_sector(cur_state.map_data, sect_idx):

            #num_walls = map_db.get_sector_param(sect_idx, map_db.NUM_WALLS_IDX)
            #wall_offset = map_db.get_sector_param(sect_idx, map_db.WALL_OFFSET_IDX)

            #for i in range(num_walls):
            #    wall_v1_idx = wall_offset + i
            #    wall_v2_idx = wall_v1_idx + 1
            #    wall = line.Wall(wall_v1_idx, wall_v2_idx)

                if wall.point_collides(cur_state.map_data, x, y, collide_with_normal=True):
                    cur_state.cur_wall = wall.idx #wall_v1_idx
                    cur_state.mode = Mode.LINE
                    # find sector
                    cur_state.cur_sector = sect_idx
                    return
        return

    if cur_state.cur_sector == -1:
        return

    # add a new vertex and line to the sector

    if clicked_vertex == -1:
        ix -= ix % 10
        iy -= iy % 10
        cur_state.cur_vertex = map_db.add_new_vertex(cur_state.map_data, ix, iy)

    if prev_cur != -1 and prev_cur != cur_state.cur_vertex:
        cur_state.cur_wall = map_db.add_line_to_sector(cur_state.map_data, cur_state.cur_sector, prev_cur, cur_state.cur_vertex)


last_frame_x = None
last_frame_y = None
got_hold_last_frame = False


def on_frame():
    global cur_state, last_frame_x, last_frame_y, got_hold_last_frame

    imgui.set_next_window_position(0, 0)
    io = imgui.get_io()
    imgui.set_next_window_size(io.display_size.x, io.display_size.y)
    imgui.begin("Map",
                flags=imgui.WINDOW_NO_TITLE_BAR | imgui.WINDOW_NO_RESIZE | imgui.WINDOW_NO_COLLAPSE | imgui.WINDOW_NO_BRING_TO_FRONT_ON_FOCUS)

    map_hovered = imgui.is_window_hovered()

    draw_map(cur_state)
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
                cur_state = State(map_db.WorldData())

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

    # else:
    #    print("clearing down_x and down_y")
    #    start_down_x = None
    #    start_down_y = None

    if got_hold_last_frame:
        cur_x, cur_y = imgui.get_mouse_pos()
        moved_cam_x = last_frame_x - cur_x
        moved_cam_y = last_frame_y - cur_y
        cur_state.camera_x += moved_cam_x
        cur_state.camera_y += moved_cam_y

    if map_hovered and not tools_hovered and mouse_button_clicked:
        x, y = imgui.get_mouse_pos()
        print("interpreting click at {},{}".format(x, y))
        interpret_click(x + cur_state.camera_x, y + cur_state.camera_y,
                        LEFT_BUTTON if left_button_clicked else RIGHT_BUTTON)
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
    # cur_state = old_state
    old_map = old_state.map_data

    new_sectors = [sector.Sector(walls=s.walls,
                                 floor_height=s.floor_height, ceil_height=s.ceil_height,
                                 type=getattr(s, 'type', 0),
                                 params=getattr(s, 'params', sector.SectorParams(0, 0, 0, 0)))
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
        f.write(cur_state.map_data.generate_c())
        f.close()


def export_map_to_rom():
    f = filedialog.asksaveasfile(mode="rwb")
    slot = 2
    fileContent = f.read()

    if f is not None:
        f.write(cur_state.map_data.generate_c_from_map())
        f.close()


if __name__ == '__main__':

    root = tk.Tk()
    root.withdraw()
    cur_state = State(map_db.WorldData())
    imgui.create_context()
    io = imgui.get_io()

    if len(sys.argv) > 1:
        with open(sys.argv[1], "rb") as f:
            load_map_from_file(f)
            f.close()

    main_sdl2()

    atexit.register(root.destroy)
