import imgui
import undo
import utils
from utils import input_int, draw_list

NO_TYPE = 0
FLASHING = 1
DOOR = 2
LIFT = 3
STAIRS = 4


SECTOR_GROUP_EFFECT_TYPES = ["No effect", "Flashing", "Door", "Lift", "Stairs", "Lowering stairs"] 




NO_TRIGGER = 0
SET_SECTOR_DARK = 1
SET_SECTOR_LIGHT = 2
START_STAIRS = 3

SECTOR_GROUP_TRIGGER_TYPES = ["No trigger", "Set dark", "Set light", "Start stairs", "End level"]
SECTOR_GROUP_NUM_TRIGGERS = 7 # four pairs of (target_sector_group, target_sector_group_action)

CLOSED=0
GOING_UP=1
OPEN=2
GOING_DOWN=3

DOOR_LIFT_STATES = ["Closed","Going up", "Open", "Going down"]

STAIRS_LOWERED = 0
STAIRS_MOVING = 1
STAIRS_RAISED = 2
STAIR_STATES = ["Stairs lowered", "Stairs moving", "Stairs raised"]



class SectorGroup():
    def regen_color(self):
        self.color = utils.random_bright_color()

    def add_trigger_target(self, tgt):
        if len(self.triggers) == SECTOR_GROUP_NUM_TRIGGERS+1:
            return 
        
        self.triggers.append(tgt)
    
    def __init__(self, index, type, light_level=0, orig_height=0, ticks_left=0, state=0, 
                 floor_height=0, ceil_height=100, floor_color=1, ceil_color=1, triggers=None):
        self.regen_color()
        if triggers is None:
            triggers = [0,-1]
        
        
            

        self.index = index

        self.type = type

        self.light_level = light_level
        self.orig_height = orig_height 
        self.ticks_left = ticks_left 
        self.state = state
        self.floor_height = floor_height
        self.floor_color = floor_color
        self.ceil_height = ceil_height
        self.ceil_color = ceil_color

        self.triggers = triggers

    
def add_new_sector_group(cur_state):
    undo.push_state(cur_state)
    num_sect_groups = len(cur_state.map_data.sector_groups)
    new_sect_group = SectorGroup(num_sect_groups, NO_TYPE)
    new_sect_group.ceil_color = cur_state.default_ceil_color
    new_sect_group.floor_color = cur_state.default_floor_color
    cur_state.map_data.sector_groups.append(new_sect_group)

def set_sector_group_attr(cur_state, cur_sect, attr, v):
    undo.push_state(cur_state)
    setattr(cur_sect, attr, v)

def draw_sector_group_mode(cur_state):
    if imgui.button("New sector group"):
        cur_state.cur_sector_group = add_new_sector_group(cur_state)

    if cur_state.cur_sector_group is not None:
        cur_sect_group = cur_state.cur_sector_group
        
        imgui.text("Sector Group: {}".format(cur_sect_group.index))

        set_orig_height = lambda v: set_sector_group_attr(cur_state, cur_sect_group, 'orig_height', v)
        set_ticks_left = lambda v: set_sector_group_attr(cur_state, cur_sect_group, 'ticks_left', v)

        set_floor_height = lambda v: set_sector_group_attr(cur_state, cur_sect_group, 'floor_height', v)
        set_floor_color = lambda v: set_sector_group_attr(cur_state, cur_sect_group, 'floor_color', v)
        set_ceil_height = lambda v: set_sector_group_attr(cur_state, cur_sect_group, 'ceil_height', v)
        set_ceil_color = lambda v: set_sector_group_attr(cur_state, cur_sect_group, 'ceil_color', v)

        type_changed, new_type_idx = imgui.core.combo("Type:   ", cur_sect_group.type, SECTOR_GROUP_EFFECT_TYPES)
        if type_changed:
            cur_sect_group.type = new_type_idx


        light_level_options = ["-2","-1","0","1","2"]
        light_level_changed, new_light_level_idx = imgui.core.combo("Light level:  ", cur_sect_group.light_level+2, light_level_options)
        if light_level_changed:
            cur_sect_group.light_level = new_light_level_idx-2

        input_int("Original height:    ", "##sector{}_orig_height".format(cur_sect_group.index), cur_sect_group.orig_height, set_orig_height)
        
        input_int("Ticks left: ", "##sector{}_ticks_left".format(cur_sect_group.index), cur_sect_group.ticks_left, set_ticks_left)
        state_options = ["No state"]
        state_label = "No state"
        if cur_sect_group.type == DOOR:
            state_label = "Door state"
            state_options = DOOR_LIFT_STATES
        elif cur_sect_group.type == FLASHING:
            state_label = "Flicker base light level"
            state_options = ["-2", "-1", "0", "1", "2"]
        elif cur_sect_group.type == LIFT:
            state_label = "Lift state"
            state_options = DOOR_LIFT_STATES
        elif cur_sect_group.type == STAIRS:
            state_label = "Stair state"
            state_options = STAIR_STATES


        if cur_sect_group.type == FLASHING:
            state_changed, new_state_idx = imgui.core.combo(state_label, cur_sect_group.state+2, state_options)
        else:
            state_changed, new_state_idx = imgui.core.combo(state_label, cur_sect_group.state, state_options)

        if state_changed:
            if cur_sect_group.type == FLASHING:
                cur_sect_group.state = new_state_idx-2
            else:
                cur_sect_group.state = new_state_idx


        input_int("Floor height:   ", "##sector{}_floor_height".format(cur_sect_group.index), cur_sect_group.floor_height, set_floor_height)
        input_int("Floor color:    ", "##sector{}_floor_color".format(cur_sect_group.index), cur_sect_group.floor_color, set_floor_color)
        
        input_int("Ceiling height: ", "##sector{}_ceil".format(cur_sect_group.index), cur_sect_group.ceil_height, set_ceil_height)
        input_int("Ceiling color:  ", "##sector{}_ceil_color".format(cur_sect_group.index), cur_sect_group.ceil_color, set_ceil_color)


        trigger_type_changed, new_trigger_type_index = imgui.core.combo("Trigger type:  ", cur_sect_group.triggers[0], SECTOR_GROUP_TRIGGER_TYPES)
        if trigger_type_changed:
            cur_sect_group.triggers[0] = new_trigger_type_index

        if len(cur_sect_group.triggers) < SECTOR_GROUP_NUM_TRIGGERS+1:
            
            if imgui.button("New trigger"):
                cur_sect_group.add_trigger_target(-1)
        
        sect_group_options = ["-1"] + ["{}".format(sg.index) for sg in cur_state.map_data.sector_groups]
        for idx,tgt in enumerate(cur_sect_group.triggers[1:]):
            tgt_changed, new_tgt_idx = imgui.core.combo("Tgt {}:  ".format(idx), tgt+1, sect_group_options)
            if tgt_changed:
                cur_sect_group.triggers[idx+1] = new_tgt_idx-1 

            

    delete_sector_group = lambda : None
    def set_cur_sector_group(idx):
        cur_state.cur_sector_group = cur_state.map_data.sector_groups[idx]

    draw_list(cur_state, "Sector Groups", "Sector Group list", 
              cur_state.map_data.sector_groups, 
              set_cur_sector_group, delete_sector_group)
