import imgui
import map_db
from utils import input_int, draw_list, input_select, delay

def draw_sector_group_mode(cur_state):

    if imgui.button("New sector group"):
        cur_state.cur_sector = map_db.add_new_sector_group(cur_state.map_data)

    cur_sect_group = cur_state.cur_sector_group
    if cur_sect_group != -1:

        imgui.text("Sector Group {}".format(cur_sect_group))

        input_int("Floor height:   ", "##sector_group{}_floor_height".format(cur_sect_group),
                  map_db.get_sector_group_param(cur_state.map_data, cur_sect_group, map_db.FLOOR_HEIGHT_IDX),
                  delay(map_db.set_sector_group_param, cur_state.map_data, cur_sect_group, map_db.FLOOR_HEIGHT_IDX))

        input_int("Floor color:    ", "##sector_group{}_floor_color".format(cur_sect_group),
                  map_db.get_sector_group_param(cur_state.map_data, cur_sect_group, map_db.FLOOR_COLOR_IDX),
                  delay(map_db.set_sector_group_param, cur_state.map_data, cur_sect_group, map_db.FLOOR_COLOR_IDX))

        input_int("Ceiling height: ", "##sector_group{}_ceil".format(cur_sect_group),
                  map_db.get_sector_group_param(cur_state.map_data, cur_sect_group, map_db.CEIL_HEIGHT_IDX),
                  delay(map_db.set_sector_group_param, cur_state.map_data, cur_sect_group, map_db.CEIL_HEIGHT_IDX))

        input_int("Ceiling color:  ", "##sector_group{}_ceil_color".format(cur_sect_group),
                  map_db.get_sector_group_param(cur_state.map_data, cur_sect_group, map_db.CEIL_COLOR_IDX),
                  delay(map_db.set_sector_group_param, cur_state.map_data, cur_sect_group, map_db.CEIL_COLOR_IDX))


        input_select("Type:    ", "##sector_group{}_type".format(cur_sect_group),
                     map_db.SECTOR_TYPE_NAMES,
                     map_db.get_sector_group_type(cur_state.map_data, cur_sect_group, 0),
                     delay(map_db.set_sector_group_type, cur_state.map_data, cur_sect_group, 0))

        SECTOR_LIGHTS = ["-2", "-1", "0", "1", "2"]
        input_select("Light level:    ", "##sector{}_light".format(cur_sect_group), SECTOR_LIGHTS,
                     map_db.get_sector_group_param(cur_state.map_data, cur_sect_group, map_db.LIGHT_IDX) + 2,
                     lambda v: map_db.set_sector_group_param(cur_state.map_data, cur_sect_group, map_db.LIGHT_IDX, v - 2))

        SECTOR_STATES = ["CLOSED", "GOING_UP", "OPEN", "GOING_DOWN"]
        input_select("State:          ", "##sector{}_state".format(cur_sect_group), SECTOR_STATES,
                     map_db.get_sector_group_param(cur_state.map_data, cur_sect_group, map_db.STATE_IDX),
                     delay(map_db.set_sector_group_param, cur_state.map_data, cur_sect_group, map_db.STATE_IDX))

        input_int("Orig height:    ", "##sector{}_orig_height".format(cur_sect_group),
                  map_db.get_sector_group_param(cur_state.map_data, cur_sect_group, map_db.ORIG_HEIGHT_IDX),
                  delay(map_db.set_sector_group_param, cur_state.map_data, cur_sect_group, map_db.ORIG_HEIGHT_IDX))

    def set_cur_sector_group(idx):
        cur_state.cur_sector_group = idx

    draw_list(cur_state, "Sector Group", "Sector group list", map_db.get_all_sector_groups(cur_state.map_data),
                set_cur_sector_group)