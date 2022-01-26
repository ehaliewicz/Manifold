import imgui

from src.python.editor import map_db
from utils import input_int, draw_list, input_select


"""
def add_new_sector(cur_state):
    for i in range(NUM_SECTOR_ATTRIBUTES):
        cur_state.map_data.sectors.append(0)

    for i in range(NUM_SECTOR_PARAMS):
        cur_state.map_data.sector_params.append(0)

    cur_state.map_data.sector_types.append(0)

    cur_state.map_data.num_sectors += 1
    return cur_state.map_data.num_sectors-1


class SectorParams(object):
    def __init__(self, light, orig_height, ticks_left, state):
        self.light = light
        self.orig_height = orig_height
        self.ticks_left = ticks_left
        self.state = state


class Sector(object):
    def __init__(self, walls=None, floor_height=0, ceil_height=100, floor_color=1, ceil_color=1, flags=0,
                 type=NO_TYPE, params=None):
        self.floor_height = floor_height
        self.ceil_height = ceil_height
        self.floor_color = floor_color
        self.ceil_color = ceil_color
        self.flags = flags
        self.type = type
        if params is None:
            self.params = SectorParams(0, 0, 0, 0)
        else:
            self.params = params

        if walls is not None:
            self.walls = walls
        else:
            self.walls = []

        self.index = None

    def get_index(self, cur_state):
        if self.index is None:
            for idx, sect in cur_state.map_data.sectors:
                if sect == self:
                    self.index = idx
                    return self.index
            raise Exception("Couldn't find sector to get index!")
        return self.index

    def __str__(self):
        return "F: {} C: {}".format(self.floor_height, self.ceil_height)


def set_sector_attribute(cur_state, sector_idx, attr_idx, val):
    cur_state.map_data.sectors[sector_idx * NUM_SECTOR_ATTRIBUTES + attr_idx] = val


def get_sector_attribute(cur_state, sector_idx, attr_idx):
    return cur_state.map_data.sectors[sector_idx * NUM_SECTOR_ATTRIBUTES + attr_idx]


def set_sector_param(cur_state, sector_idx, param_idx, val):
    cur_state.map_data.sector_params[sector_idx * NUM_SECTOR_PARAMS + param_idx] = val


def get_sector_param(cur_state, sector_idx, param_idx):
    return cur_state.map_data.sector_params[sector_idx * NUM_SECTOR_PARAMS + param_idx]

def get_sector_type(cur_state, sector_idx):
    return cur_state.map_data.sector_types[sector_idx]

def set_sector_type(cur_state, sector_idx, new_type):
    cur_state.map_data.sector_types[sector_idx] = new_type

def get_sector_indexes(cur_state):
    return [i for i in range(cur_state.map_data.num_sectors)]

def get_wall_vertex_indexes(cur_state, sector_idx):
    num_walls = get_sector_attribute(cur_state, sector_idx, NUM_WALLS_IDX)
    start_offset = get_sector_attribute(cur_state, sector_idx, WALL_OFFSET_IDX)
    return [i+start_offset for i in range(num_walls)]
"""


def draw_sector_mode(cur_state):
    if imgui.button("New sector"):
        cur_state.cur_sector = map_db.add_new_sector(cur_state.map_data)

    cur_sect = cur_state.cur_sector
    if cur_sect != -1:
        imgui.text("Sector {}".format(cur_sect))

        set_sector_group = lambda val: map_db.set_sector_constant(cur_state.map_data, cur_sect, map_db.SECT_GROUP_IDX, val)

        input_int("Sector group:  ", "##sector{}_".format(cur_sect),
                  map_db.get_sector_constant(cur_state.map_data, cur_sect, map_db.SECT_GROUP_IDX),
                  set_sector_group)



    #type_getter = lambda: get_sector_type(cur_state, cur_sect)
    #type_setter = lambda val: set_sector_type(cur_state, cur_sect, val)

    #attr_getter = lambda attr: get_sector_attribute(cur_state, cur_sect, attr)
    #attr_setter = lambda attr: lambda val: set_sector_attribute(cur_state, cur_sect, attr, val)

    #param_getter = lambda param: get_sector_param(cur_state, cur_sect, param)
    #param_setter = lambda param: lambda val: set_sector_param(cur_state, cur_sect, param, val)

    imgui.same_line()
    imgui.text("Sector {}".format(cur_sect))



    def set_cur_sector(idx):
        cur_state.cur_sector = idx

    draw_list(cur_state, "Sector", "Sector list", map_db.get_all_sectors(cur_state.map_data), set_cur_sector)
