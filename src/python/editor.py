import glfw
import imgui
from imgui.integrations.glfw import GlfwRenderer
import math
import OpenGL.GL as gl
from enum import Enum
import sys

import tkinter as tk
from tkinter import filedialog

import atexit
import pickle

# commands





def dist(x1,y1, x2,y2):
    dx = x2-x1
    dy = y2-y1
    return math.sqrt((dx*dx)+(dy*dy))
    
def point_circle(px,py,cx,cy,r):
    dx = px-cx
    dy = py-cy
    
    if abs(dx) > r or abs(dy) > r:
        return False

    dst = dist(px,py, cx,cy)
    return dst <= r


def line_point(x1, y1, x2, y2, px, py):

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

def line_circle(x1,y1, x2,y2, cx, cy, r):
    inside1 = point_circle(x1,y1, cx,cy, r)
    inside2 = point_circle(x2,y2, cx,cy, r)
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

    onSegment = line_point(x1,y1,x2,y2, closestX,closestY);
    if not onSegment:
        return False

    #distX = closestX - cx;
    #distY = closestY - cy;
    dst = dist(closestX,closestY, cx, cy)

    return dst <= r
 


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

        NUM_SECTOR_ATTRIBUTES = 7

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
            res += "    {}, {}, {}, {}, {}, {}, {},\n".format(wall_offset, portal_offset, sect_num_walls,
                                                             sect.floor_height, sect.ceil_height,
                                                             sect.floor_color, sect.ceil_color)
            
            wall_offset += sect_num_walls+1
            portal_offset += sect_num_walls
            
        res += "};\n\n"
            
        

        # we can have up to 65536 vertexes
        res += "static const u16 walls[{}] =".format(num_walls+num_sectors) + "{\n"
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
                if wall.adj_sector_idx == -1:
                    res += "    {" + ".mid_col = {}".format(wall.mid_color) + "},\n"
                else:
                    res += "    {" + ".upper_col = {}, .lower_col = {}".format(wall.up_color, wall.low_color) + "},\n"
                    
        res += "};\n\n"

        res += "#define VERT(x1,y1) { .x = (x1 * 6), .y = (y1 * 6) } \n"
        
        res += "static const vertex vertexes[{}] =".format(num_vertexes) + " = {\n"
        for vert in self.vertexes:
            res += "    VERT({},{}),\n".format(vert.x, vert.y)
        res += "};\n"
        
        
        res += "const portal_map {} ".format(self.name.replace(" ", "_")) + "{\n"
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

class Sector():
    def __init__(self, index, walls=None, floor_height=0, ceil_height=100, floor_color=1, ceil_color=1):
        self.floor_height = floor_height
        self.ceil_height = ceil_height
        self.floor_color = floor_color
        self.ceil_color = ceil_color

        if walls is not None:
            self.walls = walls
        else:
            self.walls = []
        
        self.index = index

    def __str__(self):
        return "F: {} C: {}".format(self.floor_height, self.ceil_height)


class Wall():
    def __init__(self, v1, v2, sector_idx, adj_sector_idx): #, index):
        self.v1 = v1
        self.v2 = v2
        self.sector_idx = sector_idx
        self.adj_sector_idx = adj_sector_idx
        #self.index = index
        self.up_color = 1
        self.low_color = 1
        self.mid_color = 1
        
    def __str__(self):
        return "v1: {} v2: {}".format(self.v1.index, self.v2.index)

    def rough_mid_point(self):
        v1 = self.v1
        v2 = self.v2
        dx = v2.x - v1.x
        dy = v2.y - v1.y
        midx = int(v1.x + (0.9*dx/2))
        midy = int(v1.y + (0.9*dy/2))
        return (midx, midy)
        
    def normal(self):
        v1 = self.v1
        v2 = self.v2
        dx = v2.x - v1.x
        dy = v2.y - v1.y
        mag = math.sqrt((dx*dx)+(dy*dy))

        scale = 10/mag
        return int(-dy*scale),int(dx*scale)

    def normal_quadrant(self):
        nx,ny = self.normal()
        rads = math.atan2(-ny, nx)
        ang = rads * 180/math.pi
        if ang < 0:
            ang += 360

        if ang == 0:
            return "FACING_RIGHT"
        elif ang < 90:
            return "QUADRANT_0"
        elif ang == 90:
            return "FACING_UP"
        elif ang < 180:
            return "QUADRANT_1"
        elif ang == 180:
            return "FACING_LEFT"
        elif ang < 270:
            return "QUADRANT_2"
        elif ang == 270:
            return "FACING_DOWN"
        else:
            return "QUADRANT_3"

        
    def centered_normal(self):
        (mx,my) = self.rough_mid_point()
        (nx,ny) = self.normal()
        return (mx,my), (mx+nx, my+ny)
        
    def point_collides(self, cx, cy, collide_with_normal=False):
        radius = 10

        x1 = self.v1.x
        y1 = self.v1.y
        x2 = self.v2.x
        y2 = self.v2.y

        if line_circle(x1,y1, x2,y2, cx, cy, 5):
            return True

        if not collide_with_normal:
            return False
        
        ((n1x,n1y),(n2x,n2y)) = self.centered_normal()
        return line_circle(n1x,n1y, n2x,n2y, cx, cy, 5)
            
        
class Vertex():
    def __init__(self, x, y, index):
        self.x = x
        self.y = y
        self.index = index

    def __str__(self):
        return "x: {} y: {}".format(self.x, self.y)

    def point_collides(self,x,y):
        radius = 5
        x1 = self.x
        y1 = self.y
        x2 = x
        y2 = y

        return point_circle(x1,y1,x2,y2,radius)

        

def main():
    def glfw_init():
        width, height = 1280, 800
        window_name = "Map Editor"
        if not glfw.init():
            print("Could not initialize OpenGL context")
            exit(1)
        # OS X supports only forward-compatible core profiles from 3.2
        glfw.window_hint(glfw.CONTEXT_VERSION_MAJOR, 3)
        glfw.window_hint(glfw.CONTEXT_VERSION_MINOR, 3)
        glfw.window_hint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE)
        glfw.window_hint(glfw.OPENGL_FORWARD_COMPAT, gl.GL_TRUE)
        # Create a windowed mode window and its OpenGL context
        window = glfw.create_window(
            int(width), int(height), window_name, None, None
        )
        glfw.make_context_current(window)
        if not window:
            glfw.terminate()
            print("Could not initialize Window")
            exit(1)
        return window
    window = glfw_init()
    impl = GlfwRenderer(window)
    while not glfw.window_should_close(window):
        glfw.poll_events()
        impl.process_inputs()
        imgui.new_frame()
        on_frame()
        gl.glClearColor(1., 1., 1., 1)
        gl.glClear(gl.GL_COLOR_BUFFER_BIT)
        imgui.render()
        impl.render(imgui.get_draw_data())
        glfw.swap_buffers(window)
    impl.shutdown()
    glfw.terminate()


class Mode(Enum):
    SECTOR = 'Sector'
    LINE = 'Line'
    VERTEX = 'Vertex'


class State(object):
    def __init__(self):
        self.mode = Mode.SECTOR
        self.map_data = Map()
        self.cur_sector = None
        self.cur_wall = None
        self.cur_vertex = None

        self.hovered_item = None

        
cur_state = State()






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


def draw_list(id_str, label, items, select_item, delete_callback = None):
    
    imgui.begin_child(id_str)
    imgui.text(label)

    cur_state.hovered_item = None
    
    for idx,item in enumerate(items):
        cur_id_str = "##list_{}_{}".format(id_str, idx)
        sel_btn_id  = "Select {} {}".format(idx, cur_id_str)
        del_btn_id = "X{}".format(cur_id_str)
        
        imgui.begin_group()
        if imgui.button(sel_btn_id):
            print("clicked {}".format(idx))
            select_item(idx)
        imgui.same_line()
        
        imgui.text(str(item))
        if delete_callback is not None:
            imgui.same_line()
            if imgui.button(del_btn_id):
                delete_callback(item)
                print("delete!!")
        
        
        imgui.end_group()

        if imgui.is_item_hovered():
            cur_state.hovered_item = item
            
    imgui.end_child()


def add_new_sector():
    num_sects = len(cur_state.map_data.sectors)
    new_sect = Sector(num_sects)
    cur_state.map_data.sectors.append(new_sect)
    return new_sect
        
def add_new_wall(v1, v2):
    num_walls = len(cur_state.cur_sector.walls)
    new_wall = Wall(v1=v1, v2=v2, sector_idx=cur_state.cur_sector.index, adj_sector_idx=-1)
    cur_state.cur_sector.walls.append(new_wall)
    return new_wall
    
def add_new_vertex(x,y):
    num_verts = len(cur_state.map_data.vertexes)
    new_vert = Vertex(x, y, index=num_verts)
    cur_state.map_data.vertexes.append(new_vert)
    return new_vert
    
def draw_sector_mode():
    
    if imgui.button("New sector"):
        cur_state.cur_sector = add_new_sector()
        
    
    if cur_state.cur_sector is not None:
        cur_sect = cur_state.cur_sector
        imgui.same_line()
        imgui.text("Sector {}".format(cur_sect.index))

        input_int("Floor height:   ", "##sector{}_floor_height".format(cur_sect.index), cur_sect.floor_height, lambda v: setattr(cur_sect, 'floor_height', v))
        input_int("Floor color:    ", "##sector{}_floor_color".format(cur_sect.index), cur_sect.floor_color, lambda v: setattr(cur_sect, 'floor_color', v))
        
        input_int("Ceiling height: ", "##sector{}_ceil".format(cur_sect.index), cur_sect.ceil_height, lambda v: setattr(cur_sect, 'ceil_height', v))
        input_int("Ceiling color:  ", "##sector{}_ceil_color".format(cur_sect.index), cur_sect.ceil_color, lambda v: setattr(cur_sect, 'ceil_color', v))

    def set_cur_sector(idx):
        cur_state.cur_sector = cur_state.map_data.sectors[idx]

    draw_list("Sectors", "Sector list", cur_state.map_data.sectors, set_cur_sector)


def delete_line(wall):
    cur_state.cur_sector
    # find wall in sectors
    # delete it from that sector
    # if this is the currently selected wall, unselect the wall
    if cur_state.cur_wall == wall:
        cur_state.cur_wall = None

    
    for sector in cur_state.map_data.sectors:
        for idx,cur_wall in enumerate(sector.walls):
            if cur_wall is wall:
                del sector.walls[idx]
                break


def draw_line_mode():
    
    #if imgui.button("New line") and cur_state.cur_sector is not None and len(cur_state.map_data.vertexes) >= 2:
    #    cur_state.cur_wall = add_new_wall()
            
    if cur_state.cur_wall is not None:
        cur_wall = cur_state.cur_wall
        #imgui.same_line()
        #imgui.text("Line {}".format(cur_wall.index))
        
        #vert_opts = ["{}".format(idx) for idx in range(len(cur_state.map_data.vertexes))]
        vert_opts = ["{}".format(idx) for idx in range(len(cur_state.map_data.vertexes))]
                     
        color_opts = ["{}".format(idx) for idx in range(16)]
        
        v1_changed,new_v1_idx = imgui.core.combo("v1", cur_wall.v1.index, vert_opts)
        
        v2_changed,new_v2_idx = imgui.core.combo("v2", cur_wall.v2.index, vert_opts)
        
        sector_opts = ["-1"] + ["{}".format(idx) for idx in range(len(cur_state.map_data.sectors))]
        
        adj_changed,new_adj_sector_idx = imgui.core.combo("adj sector", cur_wall.adj_sector_idx+1, sector_opts)

        up_col_changed, new_up_col = imgui.core.combo("upper color", cur_wall.up_color, color_opts)
        mid_col_changed, new_mid_col = imgui.core.combo("middle color", cur_wall.mid_color, color_opts)
        low_col_changed, new_low_col = imgui.core.combo("lower color", cur_wall.low_color, color_opts)
        
        if v1_changed:
            cur_wall.v1 = cur_state.map_data.vertexes[new_v1_idx]
        if v2_changed:
            print(new_v2_idx)
            cur_wall.v2 = cur_state.map_data.vertexes[new_v2_idx]
            
        if adj_changed:
            cur_wall.adj_sector_idx = new_adj_sector_idx-1

        if up_col_changed:
            cur_wall.up_color = new_up_col
        if mid_col_changed:
            cur_wall.mid_color = new_mid_col
        if low_col_changed:
            cur_wall.low_color = new_low_col
            
        
    def set_cur_wall(idx):
        global cur_state
        cur_state.cur_wall = cur_state.cur_sector.walls[idx]

    if cur_state.cur_sector is not None:
        draw_list("Lines", "Line list", cur_state.cur_sector.walls, set_cur_wall,
                  delete_callback=delete_line)
    

def draw_vert_mode():
    
    #if imgui.button("New vertex"):
    #    cur_state.cur_vertex = add_new_vertex()
    
    
    if cur_state.cur_vertex is not None:

        cur_vertex = cur_state.cur_vertex
        imgui.text("Vertex {}".format(cur_vertex.index))

        def set_xy(xy):
            (x,y) = xy
            cur_state.cur_vertex.x = x
            cur_state.cur_vertex.y = y
            
        input_int2("x,y: ", "##vert{}_xy".format(cur_vertex.index), (cur_vertex.x, cur_vertex.y),
                   set_xy)                  
    
    

    def set_cur_vertex(idx):
        cur_state.cur_vertex = cur_state.map_data.vertexes[idx]

    draw_list("Vertexes", "Vertex list", cur_state.map_data.vertexes, set_cur_vertex)

   
def draw_mode():
    
    draw_funcs = {
        Mode.SECTOR: draw_sector_mode,
        Mode.LINE: draw_line_mode,
        Mode.VERTEX: draw_vert_mode
    }
        
    changed, text_val = imgui.input_text("Name: ", cur_state.map_data.name, buffer_length=64)
    if changed:
        cur_state.map_data.name = text_val
        
        
    imgui.text("{} mode".format(cur_state.mode.value))

    for k in draw_funcs.keys():
        #if can_switch_to(k.value):
        if imgui.radio_button(k.value, cur_state.mode == k):
            cur_state.mode = k
    
        
    draw_funcs[cur_state.mode]()


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

    draw_list.add_circle_filled(vert.x, vert.y, 2, imgui.get_color_u32_rgba(*color), num_segments=12)

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
     
    draw_list.add_line(v1.x, v1.y, v2.x, v2.y, imgui.get_color_u32_rgba(*color), 1.0)
    draw_list.add_line(n1x, n1y, n2x, n2y, imgui.get_color_u32_rgba(*color), 1.0)
    

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
    
    for sector in cur_state.map_data.sectors:
        is_selected = cur_state.mode == Mode.SECTOR and sector == cur_state.cur_sector
        is_hovered = cur_state.hovered_item == sector
        draw_sector(draw_list, sector, highlight = is_selected or is_hovered)

    if (cur_state.mode == Mode.SECTOR and cur_state.cur_sector is not None):
        draw_sector(draw_list, cur_state.cur_sector, highlight=True)
    if cur_state.hovered_item and isinstance(cur_state.hovered_item, Sector):
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
        


def on_frame():

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
            
            
            imgui.end_menu()
        imgui.end_main_menu_bar()
    
        
    imgui.begin("Tools", False)
    tools_hovered = imgui.is_window_hovered()
    draw_mode()
    

    left_button_released = imgui.is_mouse_released(button=0)
    right_button_released = imgui.is_mouse_released(button=1)
    
    mouse_button_released = (left_button_released or right_button_released)
    
    if map_hovered and not tools_hovered and mouse_button_released:
        x,y = imgui.get_mouse_pos()
        interpret_click(x,y, LEFT_BUTTON if left_button_released else RIGHT_BUTTON)
            
    
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

    new_sectors = [Sector(index=s.index, walls=s.walls,
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

    main()
    
    atexit.register(root.destroy)
