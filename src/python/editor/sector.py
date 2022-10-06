import imgui
import undo
from utils import input_int, draw_list



class Sector():
    def __init__(self, index, walls=None, floor_height=0, ceil_height=100, floor_color=1, ceil_color=1):
        self.floor_height = floor_height
        self.ceil_height = ceil_height
        self.floor_color = floor_color
        self.ceil_color = ceil_color
        self.sector_group_idx = 0

        self.is_convex_memo = False
        self.convex_calculated = None
        
        if walls is not None:
            self.walls = walls
        else:
            self.walls = []
        
        self.index = index


    def __str__(self):
        return "F: {} C: {}".format(self.floor_height, self.ceil_height)

    def get_vertexes(self):
        return [wall.v2 for wall in self.walls]

    def add_wall(self, wall):
        self.convex_calculated = False
        self.walls.append(wall)

    def is_convex(self):
        if not self.convex_calculated:
            self.is_convex_memo = self.is_convex_inner()
            self.convex_calculated = True
            
        return self.is_convex_memo

    
    def is_convex_inner(self):
        verts = self.get_vertexes()
        num_verts = len(verts)
        if num_verts < 3:
            return False
        
        res = 0

        
        for idx in range(num_verts):
            p = verts[idx]
            tmp = verts[(idx+1) % num_verts] # get next wall
        
            vx = tmp.x - p.x;
            vy = tmp.y - p.y;

            u = verts[(idx+2) % num_verts]
            if idx == 0: # in first loop direction is unknown, so save it in res
                res = u.x * vy - u.y * vx + vx * p.y - vy * p.x
            else:
                newres = u.x * vy - u.y * vx + vx * p.y - vy * p.x
                if ( (newres > 0 and res < 0) or (newres < 0 and res > 0) ):
                    return False
        
            
        return True

def add_new_sector(cur_state):
    undo.push_state(cur_state)
    num_sects = len(cur_state.map_data.sectors)
    new_sect = Sector(num_sects)
    cur_state.map_data.sectors.append(new_sect)
    return new_sect

def set_sector_attr(cur_state, cur_sect, attr, v):
    undo.push_state(cur_state)
    setattr(cur_sect, attr, v)

def draw_sector_mode(cur_state):
    
    if imgui.button("New sector"):
        cur_state.cur_sector = add_new_sector(cur_state)
        
    
    if cur_state.cur_sector is not None:
        cur_sect = cur_state.cur_sector
        imgui.same_line()
        imgui.text("Sector {}".format(cur_sect.index))

        set_floor_height = lambda v: set_sector_attr(cur_state, cur_sect, 'floor_height', v)
        set_floor_color = lambda v: set_sector_attr(cur_state, cur_sect, 'floor_color', v)
        set_ceil_height = lambda v: set_sector_attr(cur_state, cur_sect, 'ceil_height', v)
        set_ceil_color = lambda v: set_sector_attr(cur_state, cur_sect, 'ceil_color', v)

        input_int("Floor height:   ", "##sector{}_floor_height".format(cur_sect.index), cur_sect.floor_height, set_floor_height) #lambda v: setattr(cur_sect, 'floor_height', v))
        input_int("Floor color:    ", "##sector{}_floor_color".format(cur_sect.index), cur_sect.floor_color, set_floor_color) #lambda v: setattr(cur_sect, 'floor_color', v))
        
        input_int("Ceiling height: ", "##sector{}_ceil".format(cur_sect.index), cur_sect.ceil_height, set_ceil_height)
        input_int("Ceiling color:  ", "##sector{}_ceil_color".format(cur_sect.index), cur_sect.ceil_color, set_ceil_color)

    def set_cur_sector(idx):
        cur_state.cur_sector = cur_state.map_data.sectors[idx]

    def delete_sector(sect):
        if cur_state.cur_sector == sect:
            cur_state.cur_sector = None
        deleted_idx = None
        for idx,sector in enumerate(cur_state.map_data.sectors):
            if sect == sector:
                undo.push_state(cur_state)
                deleted_idx = idx
                del cur_state.map_data.sectors[idx] 
                break
        if deleted_idx is not None:
            for idx, sector in enumerate(cur_state.map_data.sectors):
                # patch sector indexes
                sector.index = idx
                # patch portal references to the deleted sector,
                # or any references to sectors after the deleted sector
                for wall in sector.walls:
                    wall.sector_idx = idx
                    if wall.adj_sector_idx == deleted_idx:
                        wall.adj_sector_idx = -1 
                        # remove portal references to the deleted sector
                    elif wall.adj_sector_idx > deleted_idx:
                        # decrement references to sectors after the deleted sector
                        wall.adj_sector_idx = wall.adj_sector_idx - 1 



    draw_list(cur_state, "Sectors", "Sector list", 
              cur_state.map_data.sectors, 
              set_cur_sector, delete_sector)

    
