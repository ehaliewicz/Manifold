import imgui
import enum
from utils import input_int, draw_list, input_select



SECTOR_TYPE_NAMES = [
    "NO TYPE",
    "FLASHING",
    "DOOR",
    "LIFT"
]
NO_TYPE = 0
FLASHING = 1
DOOR = 2
LIFT = 3


class SectorParams(object):
    def __init__(self, light, orig_height, ticks_left, state):
        self.light = light
        self.orig_height = orig_height
        self.ticks_left = ticks_left
        self.state = state


class Sector(object):
    def __init__(self, index, walls=None, floor_height=0, ceil_height=100, floor_color=1, ceil_color=1, flags=0,
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

        self.index = index


    def __str__(self):
        return "F: {} C: {}".format(self.floor_height, self.ceil_height)


def add_new_sector(cur_state):
    num_sects = len(cur_state.map_data.sectors)
    new_sect = Sector(num_sects)
    cur_state.map_data.sectors.append(new_sect)
    return new_sect




def draw_sector_mode(cur_state):
    if imgui.button("New sector"):
        cur_state.cur_sector = add_new_sector(cur_state)

    if cur_state.cur_sector is not None:
        cur_sect = cur_state.cur_sector
        imgui.same_line()
        imgui.text("Sector {}".format(cur_sect.index))

        input_int("Floor height:   ", "##sector{}_floor_height".format(cur_sect.index), cur_sect.floor_height,
                  lambda v: setattr(cur_sect, 'floor_height', v))
        input_int("Floor color:    ", "##sector{}_floor_color".format(cur_sect.index), cur_sect.floor_color,
                  lambda v: setattr(cur_sect, 'floor_color', v))

        input_int("Ceiling height: ", "##sector{}_ceil".format(cur_sect.index), cur_sect.ceil_height,
                  lambda v: setattr(cur_sect, 'ceil_height', v))
        input_int("Ceiling color:  ", "##sector{}_ceil_color".format(cur_sect.index), cur_sect.ceil_color,
                  lambda v: setattr(cur_sect, 'ceil_color', v))

        input_select("Sector Type:    ", "##Sector{}_type".format(cur_sect.index),
                     SECTOR_TYPE_NAMES, cur_sect.type, lambda v: setattr(cur_sect, 'type', v))

        SECTOR_LIGHTS = ["-2", "-1", "0", "1", "2"]
        input_select("Light level:    ", "##sector{}_light".format(cur_sect.index), SECTOR_LIGHTS, cur_sect.params.light+2, lambda v: setattr(cur_sect.params, 'light', v-2))

        SECTOR_STATES = ["CLOSED", "GOING_UP", "OPEN", "GOING_DOWN"]
        input_select("State:          ", "##sector{}_state".format(cur_sect.index), SECTOR_STATES, cur_sect.params.state, lambda v: setattr(cur_sect.params, 'state', v))

        input_int("Orig height:    ", "##sector{}_orig_height".format(cur_sect.index),
                  cur_sect.params.orig_height, lambda v: setattr(cur_sect.params, 'orig_height', v))



    def set_cur_sector(idx):
        cur_state.cur_sector = cur_state.map_data.sectors[idx]

    draw_list(cur_state, "Sectors", "Sector list", cur_state.map_data.sectors, set_cur_sector)
