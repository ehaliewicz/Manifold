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
        cur_state.cur_sector = map_db.add_new_sector()

    if cur_state.cur_sector != -1:
        cur_sect = cur_state.cur_sector


        #type_getter = lambda: get_sector_type(cur_state, cur_sect)
        #type_setter = lambda val: set_sector_type(cur_state, cur_sect, val)

        #attr_getter = lambda attr: get_sector_attribute(cur_state, cur_sect, attr)
        #attr_setter = lambda attr: lambda val: set_sector_attribute(cur_state, cur_sect, attr, val)

        #param_getter = lambda param: get_sector_param(cur_state, cur_sect, param)
        #param_setter = lambda param: lambda val: set_sector_param(cur_state, cur_sect, param, val)

        imgui.same_line()
        imgui.text("Sector {}".format(cur_sect))

        input_int("Floor height:   ", "##sector{}_floor_height".format(cur_sect),
                  map_db.get_sector_param(cur_sect, map_db.FLOOR_HEIGHT_IDX),
                  map_db.set_sector_param(cur_sect, map_db.FLOOR_HEIGHT_IDX))

        input_int("Floor color:    ", "##sector{}_floor_color".format(cur_sect),
                  map_db.get_sector_param(cur_sect, map_db.FLOOR_COLOR_IDX),
                  map_db.set_sector_param(cur_sect, map_db.FLOOR_COLOR_IDX))

        input_int("Ceiling height: ", "##sector{}_ceil".format(cur_sect),
                  map_db.get_sector_param(cur_sect, map_db.CEIL_HEIGHT_IDX),
                  map_db.set_sector_param(cur_sect, map_db.CEIL_HEIGHT_IDX))

        input_int("Ceiling color:  ", "##sector{}_ceil_color".format(cur_sect),
                  map_db.get_sector_param(cur_sect, map_db.CEIL_COLOR_IDX),
                  map_db.set_sector_param(cur_sect, map_db.CEIL_COLOR_IDX))

        input_select("Sector Type:    ", "##Sector{}_type".format(cur_sect),
                     map_db.SECTOR_TYPE_NAMES,
                     map_db.get_sector_type(cur_sect, 0),
                     map_db.set_sector_type(cur_sect, 0))

        SECTOR_LIGHTS = ["-2", "-1", "0", "1", "2"]
        input_select("Light level:    ", "##sector{}_light".format(cur_sect), SECTOR_LIGHTS,
                     map_db.get_sector_param(cur_sect, map_db.LIGHT_IDX)+2,
                     lambda v: map_db.set_sector_param(cur_sect, map_db.LIGHT_IDX)(v-2))

        SECTOR_STATES = ["CLOSED", "GOING_UP", "OPEN", "GOING_DOWN"]
        input_select("State:          ", "##sector{}_state".format(cur_sect), SECTOR_STATES,
                     map_db.get_sector_param(cur_sect, map_db.STATE_IDX),
                     map_db.set_sector_param(cur_sect, map_db.STATE_IDX))

        input_int("Orig height:    ", "##sector{}_orig_height".format(cur_sect),
                  map_db.get_sector_param(cur_sect, map_db.ORIG_HEIGHT_IDX),
                  map_db.set_sector_param(cur_sect, map_db.ORIG_HEIGHT_IDX))


    def set_cur_sector(idx):
        cur_state.cur_sector = idx

    draw_list(cur_state, "Sectors", "Sector list", map_db.get_all_sectors(), set_cur_sector)
