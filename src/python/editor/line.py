import imgui
import utils 
import math
import numpy as np
import undo
import vertex


def line_formula(p, q):
    a = q[1] - p[1]
    b = p[0] - q[0]
    c = a * (p[0]) + b * (p[1])
    return a,b,c



class Wall():
    def __init__(self, v1: vertex.Vertex, v2: vertex.Vertex, sector_idx, adj_sector_idx, default_tex): #, index):
        self.v1 = v1
        self.v2 = v2
        self.sector_idx = sector_idx
        self.adj_sector_idx = adj_sector_idx

        self.texture_file = default_tex
        #self.index = index
        self.up_color = 0
        self.low_color = 0
        self.mid_color = 0

        self.output_idx = -1


    

        
    def __str__(self):
        return "v1: {} v2: {}".format(self.v1.index, self.v2.index)
    

    def calc_texture_repetitions(self):
        # ingame_x1
        ig_x1 = self.v1.x  * utils.ENGINE_X_SCALE
        ig_y1 = self.v1.y * utils.ENGINE_Y_SCALE
        ig_x2 = self.v2.x * utils.ENGINE_X_SCALE
        ig_y2 = self.v2.y * utils.ENGINE_Y_SCALE

        sqx = (ig_x2-ig_x1)**2
        sqy = (ig_y2-ig_y1)**2

        dist = math.sqrt(sqx+sqy)

        TEX_REPEAT_DIST = 64
        ireps = dist // TEX_REPEAT_DIST
        ret = int(max([1, ireps]))
        return ret

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
        
        
        scale = 10/mag if mag != 0 else 0
        return int(-dy*scale),int(dx*scale)
    
    def portal_out_normal(self):
        v1 = self.v1
        v2 = self.v2
        dx = v2.x - v1.x
        dy = v2.y - v1.y
        mag = math.sqrt((dx*dx)+(dy*dy))
        
        
        scale = 10/mag
        return int(dy*scale),int(-dx*scale)

    def normal_quadrant_int(self):
        quad = self.normal_quadrant()
        tbl = {
            "QUADRANT_0":0,
            "QUADRANT_1":1,
            "QUADRANT_2":2,
            "QUADRANT_3":3,
            "FACING_UP":4,
            "FACING_LEFT":5,
            "FACING_DOWN":6,
            "FACING_RIGHT":7
        }
        return tbl[quad]

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
    

    def get_collision_line_verts(self, world_unit_size):
        (norm_x,norm_y) = self.normal()
        len = math.sqrt((norm_x*norm_x)+(norm_y*norm_y))
        
        nnx = norm_x/len
        nny = norm_y/len
        scaled_nnx = nnx * world_unit_size
        scaled_nny = nny * world_unit_size
        cv1x = self.v1.x + scaled_nnx
        cv1y = self.v1.y + scaled_nny
        cv2x = self.v2.x + scaled_nnx 
        cv2y = self.v2.y + scaled_nny

        return ((cv1x, cv1y), (cv2x, cv2y))
    
    def get_intersecting_vert(self, world_unit_size, cl2):   
        """ 
        Returns the point of intersection of the lines passing through a2,a1 and b2,b1.
        a1: [x, y] a point on the first line
        a2: [x, y] another point on the first line
        b1: [x, y] a point on the second line
        b2: [x, y] another point on the second line
    """
        cl1 = self.get_collision_line_verts(world_unit_size)
        (a1,a2) = cl1
        (b1,b2) = cl2
        s = np.vstack([a1,a2,b1,b2])        # s for stacked
        h = np.hstack((s, np.ones((4, 1)))) # h for homogeneous
        l1 = np.cross(h[0], h[1])           # get first line
        l2 = np.cross(h[2], h[3])           # get second line
        x, y, z = np.cross(l1, l2)          # point of intersection
        if z == 0:                          # lines are parallel
            #assert a2[0] == b1[0] and a2[1] == b1[1] 
            return a2 
            #return (float('inf'), float('inf'))
        return (x/z, y/z)

    #def get_intersecting_vert(self, world_unit_size, cl2):
    #    cl1 = self.get_collision_line_verts(world_unit_size)

    #    ((cl1v1x, cl1v1y), (cl1v2x, cl1v2y)) = cl1
    #    ((cl2v1x, cl2v1y), (cl2v2x, cl2v2y)) = cl2
        
    #    dy1 = cl1v2y - cl1v1y 
    #    dx1 = cl1v2x - cl1v1x 
    #    dy2 = cl2v2y - cl2v1y 
    #    dx2 = cl2v2x - cl2v1x

    #    assert dy1 * dx2 != dy2 * dx1, "Lines are parallel!"
        
    #    if dx1 == 0:
    #        # use dx2 instead 
    #    else:
    #        x = ((cl2v1y - cl1v1y) * dx1 * dx2 + dy1 * dx2 * cl1v1x - dy2 * dx1 * cl2v1x) / (dy1 * dx2 - dy2 * dx1)
    #        y = cl1v1y + (dy1 / dx1) * (x - cl1v1x)

    #    return (x, y)


        
    def point_collides(self, cx, cy, collide_with_normal=False):
        radius = 10

        x1 = self.v1.x
        y1 = self.v1.y
        x2 = self.v2.x
        y2 = self.v2.y

        if utils.circle_on_line(x1,y1, x2,y2, cx, cy, 5):
            return True

        if not collide_with_normal:
            return False
        
        ((n1x,n1y),(n2x,n2y)) = self.centered_normal()
        return utils.circle_on_line(n1x,n1y, n2x,n2y, cx, cy, 5)


    def find_adjacent_wall(self, map_data):
        # returns True, wall_ref, sector_ref
        # or False,None
        for sector in map_data.sectors:
            if self.sector_idx == sector.index:
                continue
            for wall in sector.walls:
                if wall == self:
                    continue
                if wall.v1 == self.v2 and wall.v2 == self.v1:
                    return True, wall
        return False, None

    def link_with_adjacent_wall_if_exists(self, map_data):
        has_shared_wall, adj_wall = self.find_adjacent_wall(map_data)
        if has_shared_wall:
            self.adj_sector_idx = adj_wall.sector_idx
            adj_wall.adj_sector_idx = self.sector_idx
        return has_shared_wall, adj_wall
            
        
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
            
    if cur_state.cur_wall is not None:
        cur_wall = cur_state.cur_wall
        #imgui.same_line()
        #imgui.text("Line {}".format(cur_wall.index))
        
        vert_opts = ["{}".format(idx) for idx in range(len(cur_state.map_data.vertexes))]
        v1_idx = cur_wall.v1.index
        v2_idx = cur_wall.v2.index

        
        color_opts = ["{}".format(idx) for idx in range(16)]
        
        
        v1_changed,new_v1_idx = imgui.core.combo("v1", cur_wall.v1.index, vert_opts)
        
        v2_changed,new_v2_idx = imgui.core.combo("v2", cur_wall.v2.index, vert_opts)
        
        sector_opts = ["-1"] + ["{}".format(idx) for idx in range(len(cur_state.map_data.sectors))]
        
        adj_changed,new_adj_sector_idx = imgui.core.combo("adj sector", cur_wall.adj_sector_idx+1, sector_opts)

        find_adjacent = imgui.button("Find other side for portal")
        if find_adjacent:
            undo.push_state(cur_state)
            cur_wall.link_with_adjacent_wall_if_exists(cur_state.map_data)
            
            
        up_col_changed, new_up_col = imgui.core.combo("upper color", cur_wall.up_color, color_opts)
        mid_col_changed, new_mid_col = imgui.core.combo("middle color", cur_wall.mid_color, color_opts)
        low_col_changed, new_low_col = imgui.core.combo("lower color", cur_wall.low_color, color_opts)


        tex_files = utils.get_texture_files(cur_state)

        def set_tex_file(f):
            undo.push_state(cur_state)
            cur_wall.texture_file = f
        utils.file_selector("texture", cur_wall.texture_file, tex_files, set_tex_file)


        if v1_changed and new_v1_idx != v2_idx:
            undo.push_state(cur_state)
            cur_wall.v1 = cur_state.map_data.vertexes[new_v1_idx]

        if v2_changed and new_v2_idx != v1_idx:
            undo.push_state(cur_state)
            cur_wall.v2 = cur_state.map_data.vertexes[new_v2_idx]
            
        if adj_changed:
            undo.push_state(cur_state)
            cur_wall.adj_sector_idx = new_adj_sector_idx-1

        if up_col_changed:
            undo.push_state(cur_state)
            cur_wall.up_color = new_up_col
        if mid_col_changed:
            undo.push_state(cur_state)
            cur_wall.mid_color = new_mid_col
        if low_col_changed:
            undo.push_state(cur_state)
            cur_wall.low_color = new_low_col

        
    def set_cur_wall(idx):
        cur_state.cur_wall = cur_state.cur_sector.walls[idx]

        
    def delete_line(wall):
        # find wall in sectors
        # delete it from that sector
        # if this is the currently selected wall, unselect the wall
        if cur_state.cur_wall == wall:
            cur_state.cur_wall = None

    
        for sector in cur_state.map_data.sectors:
            for idx,cur_wall in enumerate(sector.walls):
                if cur_wall is wall:
                    undo.push_state(cur_state)
                    del sector.walls[idx]
                    break


    if cur_state.cur_sector is not None:
        utils.draw_list(cur_state, "Lines", "Line list", cur_state.cur_sector.walls, set_cur_wall,
                  delete_callback=delete_line)
    
