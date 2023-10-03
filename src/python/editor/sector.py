import typing
import imgui
import undo
from utils import draw_list, ENGINE_X_SCALE, ENGINE_Y_SCALE
import sector_group
import shapely.geometry
import line


class Sector():
    def __init__(self, index, walls: typing.Optional[typing.List[line.Wall]]=None, sect_group_index=0):
        self.sector_group_idx = sect_group_index


        self.is_convex_memo = False
        self.convex_calculated = None
        
        if walls is None:
            self.walls = []
        else:
            self.walls = walls
        
        self.index = index

        self.calced_collision_hull = None
        self.calced_collision_hull_key = None
        self.recalc_collision_hull = False

    def calc_sorted_pvs(self):
        verts = self.get_vertexes()
        min_x = min(v.x for v in verts)
        min_y = min(v.y for v in verts)
        max_x = max(v.x for v in verts)
        max_y = max(v.y for v in verts)


        for i in range(min_x, max_x+1):
            for ang in range(0,1024):
                pass

    def inside(self,x,y):
        verts = []
        if len(self.walls) < 3:
            return False 
        
        for i, wall in enumerate(self.walls):
            if i == 0:
                verts.append((wall.v1.x,wall.v1.y))
            verts.append((wall.v2.x,wall.v2.y))

        ls = shapely.geometry.LineString(verts)
        point = shapely.geometry.Point(x,y)
        polygon = shapely.geometry.Polygon(ls)

        return polygon.contains(point)

    def __str__(self):
        return "Sector {}: Group: {}".format(self.index, self.sector_group_idx) #"F: {} C: {}".format(self.floor_height, self.ceil_height)

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
    
    def get_collision_hull(self, map_data, world_unit_size):

        key = [(line.v1.x, line.v1.y, line.v2.x, line.v2.y) for line in self.walls]
        if key != self.calced_collision_hull_key:
            self.calced_collision_hull = self.get_collision_hull_inner(map_data, world_unit_size)
            self.calced_collision_hull_key = key
            # mark neighbor sectors 

            # if we had to re-calc, but the vertex key was still valid
            for wall in self.walls:
                if wall.adj_sector_idx != -1:
                    # cause neighbor sector to recalc the collision hull, but not in a way that triggers recalc in it's neighbor sectors (which would cause an avalanche of re-processing)
                    map_data.sectors[wall.adj_sector_idx].recalc_collision_hull = True

        elif self.recalc_collision_hull:
            # only recalc, our vertexes haven't moved, but one of our neighbor's did
            self.calced_collision_hull = self.get_collision_hull_inner(map_data, world_unit_size)
            self.recalc_collision_hull = False
        
        return self.calced_collision_hull
    
    def get_collision_hull_inner(self, map_data, world_unit_size):
        hull = []
        for idx,line in enumerate(self.walls):
            collision_line = line.get_collision_line_verts(world_unit_size)

            next_line = self.walls[0] if idx == len(self.walls)-1 else self.walls[idx+1]
            prev_line = self.walls[-1] if idx == 0 else self.walls[idx-1]

            (res_x1, res_y1), (res_x2, res_y2) = collision_line

            if next_line.adj_sector_idx != -1:
                # slightly more complicated case :)
                # find wall in next sector which uses this v2 as it's v1 
                adj_sector = map_data.sectors[next_line.adj_sector_idx]
                attached_wall = None
                for other_wall in adj_sector.walls:
                    if other_wall.v1 == line.v2:
                        # found it baby :)
                        attached_wall = other_wall
                        break 
                if attached_wall is not None: # "Could not find attached wall in other sector!"
                    other_collision_line = attached_wall.get_collision_line_verts(world_unit_size) 
                    res_x2, res_y2 = line.get_intersecting_vert(world_unit_size, other_collision_line)
            elif next_line != line:
                next_collision_line = next_line.get_collision_line_verts(world_unit_size)
                res_x2, res_y2 = line.get_intersecting_vert(world_unit_size, next_collision_line)


            if prev_line.adj_sector_idx != -1:
                # slightly more complicated case :)
                # find wall in next sector which uses this v1 as it's v2
                adj_sector = map_data.sectors[prev_line.adj_sector_idx]
                attached_wall = None
                for other_wall in adj_sector.walls:
                    if other_wall.v2 == line.v1:
                        # found it baby :)
                        attached_wall = other_wall
                        break 
                if attached_wall is not None: # "Could not find attached wall in other sector!"
                    res_x1, res_y1 = attached_wall.get_intersecting_vert(world_unit_size, collision_line)

            elif prev_line != line:
                # find collision point between this line and the next and previous lines 
                res_x1, res_y1 = prev_line.get_intersecting_vert(world_unit_size, collision_line)
                #pass

            hull.append(((res_x1, res_y1), (res_x2, res_y2)))


        return hull


def add_new_sector(cur_state):
    undo.push_state(cur_state)
    num_sects = len(cur_state.map_data.sectors)
    new_sect = Sector(num_sects)
    cur_state.map_data.sectors.append(new_sect)
    if len(cur_state.map_data.sector_groups) == 0:
        sector_group.add_new_sector_group(cur_state)
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

        sect_group_options = ["{}".format(idx) for idx in range(len(cur_state.map_data.sector_groups))]
        group_changed, new_group_idx = imgui.core.combo("Sector group:  ", cur_sect.sector_group_idx, sect_group_options)
        if group_changed:
            cur_sect.sector_group_idx = new_group_idx

        #set_floor_height = lambda v: set_sector_attr(cur_state, cur_sect, 'floor_height', v)
        #set_floor_color = lambda v: set_sector_attr(cur_state, cur_sect, 'floor_color', v)
        #set_ceil_height = lambda v: set_sector_attr(cur_state, cur_sect, 'ceil_height', v)
        #set_ceil_color = lambda v: set_sector_attr(cur_state, cur_sect, 'ceil_color', v)

        #input_int("Floor height:   ", "##sector{}_floor_height".format(cur_sect.index), cur_sect.floor_height, set_floor_height) #lambda v: setattr(cur_sect, 'floor_height', v))
        #input_int("Floor color:    ", "##sector{}_floor_color".format(cur_sect.index), cur_sect.floor_color, set_floor_color) #lambda v: setattr(cur_sect, 'floor_color', v))
        
        #input_int("Ceiling height: ", "##sector{}_ceil".format(cur_sect.index), cur_sect.ceil_height, set_ceil_height)
        #input_int("Ceiling color:  ", "##sector{}_ceil_color".format(cur_sect.index), cur_sect.ceil_color, set_ceil_color)

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
            # patch all thing references?
            # if things are in this sector, do we delete them?
            # nah, if they are in this sector, just leave them at the same index
            # if they are past this sector, reduce their index by 1
            for obj in cur_state.map_data.things:
                if obj.sector_num == deleted_idx:
                    # do nothing
                    pass
                elif obj.sector_num > deleted_idx:
                    obj.sector_num = obj.sector_num - 1




    draw_list(cur_state, "Sectors", "Sector list", 
              cur_state.map_data.sectors, 
              set_cur_sector, delete_sector)
