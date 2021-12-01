import imgui

from src.python.editor import map_db
from utils import circle_on_line, draw_list
import math




class Line():
    def __init__(self, v1, v2, idx, params=[]):
        self.v1 = v1
        self.v2 = v2
        self.up_color = 1
        self.low_color = 1
        self.mid_color = 1
        self.idx = idx
        self.params = params
        
    def __str__(self):
        return "v1: {} v2: {}".format(self.v1_idx, self.v2_idx)

    def rough_mid_point(self, map_data):
        v1 = self.v1
        v2 = self.v2
        x1 = v1.x
        y1 = v1.y
        x2 = v2.x
        y2 = v2.y
        dx = x2 - x1
        dy = y2 - y1
        midx = int(v1.x + (0.9*dx/2))
        midy = int(v1.y + (0.9*dy/2))
        return (midx, midy)
        
    def normal(self, map_data):
        v1 = self.v1
        v2 = self.v2
        x1 = v1.x
        y1 = v1.y
        x2 = v2.x
        y2 = v2.y
        dx = x2 - x1
        dy = y2 - y1
        mag = math.sqrt((dx*dx)+(dy*dy))
        
        
        scale = 10/mag
        return int(-dy*scale),int(dx*scale)

    def normal_quadrant(self, map_data):
        nx,ny = self.normal(map_data)
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

        
    def centered_normal(self, map_data):
        (mx,my) = self.rough_mid_point(map_data)
        (nx,ny) = self.normal(map_data)
        return (mx,my), (mx+nx, my+ny)
        
    def point_collides(self, map_data, cx, cy, collide_with_normal=False):
        radius = 10

        v1 = self.v1
        v2 = self.v2
        x1 = v1.x
        y1 = v1.y
        x2 = v2.x
        y2 = v2.y

        if circle_on_line(x1,y1, x2,y2, cx, cy, 5):
            return True

        if not collide_with_normal:
            return False
        
        ((n1x,n1y),(n2x,n2y)) = self.centered_normal(map_data)
        return circle_on_line(n1x,n1y, n2x,n2y, cx, cy, 5)



col_names = [
    'TRANSPARENT',
    'LIGHT_RED',
    'MED_RED',
    'DARK_RED',
    'LIGHT_GREEN',
    'MED_GREEN',
    'DARK_GREEN',
    'LIGHT_BLUE',
    'MED_BLUE',
    'DARK_BLUE',
    'MED_BROWN',
    'DARK_BROWN',
    'UNSET',
    'UNSET',
    'UNSET',
    'BLACK'
]




def draw_line_mode(cur_state):
    
    #if imgui.button("New line") and cur_state.cur_sector is not None and len(cur_state.map_data.vertexes) >= 2:
    #    cur_state.cur_wall = add_new_wall()
            
    if cur_state.cur_wall != -1:
        cur_wall = cur_state.cur_wall
        #imgui.same_line()
        #imgui.text("Line {}".format(cur_wall.index))
        
        vert_opts = ["{}".format(idx) for idx in range(len(cur_state.map_data.vertexes))]

        portal_idx = cur_wall
        wall = map_db.get_line(cur_wall)

        cur_v1 = wall.v1 # cur_state.map_data.walls[wall_v1_idx]
        cur_v2 = wall.v2 # cur_state.map_data.walls[wall_v2_idx]

        def v1_setter(v1_idx):
            map_db.set_line_v1(cur_wall, v1_idx)
        def v2_setter(v2_idx):
            map_db.set_line_v2(cur_wall, v2_idx)

        #color_opts = ["{}".format(col_names[idx]) for idx in range(16)]
        
        
        v1_changed,new_v1_idx = imgui.core.combo("v1", cur_v1.idx, vert_opts)
        
        v2_changed,new_v2_idx = imgui.core.combo("v2", cur_v2.idx, vert_opts)
        
        sector_opts = ["-1"] + ["{}".format(idx) for idx in range(len(cur_state.map_data.sectors))]

        adj_sector_idx = cur_state.map_data.portals[portal_idx]
        adj_changed,new_adj_sector_idx = imgui.core.combo("adj sector", adj_sector_idx+1, sector_opts)

        def portal_setter(adj_sect_idx):
            cur_state.map_data.portals[portal_idx] = adj_sect_idx


        #up_col_changed, new_up_col = imgui.core.combo("upper color", cur_wall.up_color, color_opts)
        #mid_col_changed, new_mid_col = imgui.core.combo("middle color", cur_wall.mid_color, color_opts)
        #low_col_changed, new_low_col = imgui.core.combo("lower color", cur_wall.low_color, color_opts)
        
        if v1_changed and new_v1_idx != cur_v2:
            v1_setter(new_v1_idx)

        if v2_changed and new_v2_idx != cur_v1:
            v2_setter(new_v2_idx)

        if adj_changed:
            portal_setter(new_adj_sector_idx)

        #if up_col_changed:
        #    cur_wall.up_color = new_up_col
        #if mid_col_changed:
        #    cur_wall.mid_color = new_mid_col
        #if low_col_changed:
        #    cur_wall.low_color = new_low_col
            
        
    def set_cur_wall(idx):
        cur_state.cur_wall = idx

        
    def delete_line(wall):
        map_db.delete_line(cur_state.cur_sector, wall)
        if cur_state.cur_wall == wall:
            cur_state.cur_wall = -1


    if cur_state.cur_sector != -1:
        wall_off = map_db.get_sector_constant(cur_state.cur_sector, map_db.WALL_OFFSET_IDX)
        num_walls = map_db.get_sector_constant(cur_state.cur_sector, map_db.NUM_WALLS_IDX)
        line_list = range(wall_off, wall_off+num_walls, 1)

        draw_list(cur_state, "Line", "Line list", line_list, set_cur_wall, delete_line)
    
