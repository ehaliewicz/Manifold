import imgui 

import utils 
import undo

class ThingDef(object):
    def __init__(self, default_spr): 
        self.name = ""
        self.sprite_file = default_spr
        self.width = 64
        self.height = 64
        self.floor_draw_offset = 0
        self.init_state = 0
        self.speed = 3
#def draw_things_mode(cur_state):
#    for i in range(cur_state.map_data.things):

class Thing(object): 
    def __init__(self, sector_num, x, y, z, index):
        self.sector_num = sector_num
        self.object_type = 0
        self.x = x
        self.y = y
        self.z = z
        self.index = index

    def __str__(self):
        return "type: {}, sector: {}, x,y: {},{}".format(self.object_type, self.sector_num, self.x, self.y)

        
    def point_collides(self,x,y):
        radius = 10
        x1 = self.x
        y1 = self.y
        x2 = x
        y2 = y

        return utils.point_in_circle(x1,y1,x2,y2,radius)

def draw_thing_defs_mode(cur_state):
    

    sprite_files = utils.get_sprite_files(cur_state)


    imgui.begin_child("thing defs")
    thing_defs = cur_state.map_data.thing_defs
    for idx,thing_def in enumerate(thing_defs):

        if thing_def.sprite_file == "":
            thing_def.sprite_file = sprite_files[0]
        def set_sprite_file(spr):
            undo.push_state(cur_state)
            thing_defs[idx].sprite_file = spr

        name_changed, new_name = imgui.input_text("name:##obj_name_{} ".format(idx), thing_defs[idx].name, buffer_length=32)

        utils.file_selector("sprite##obj_sprite_{}".format(idx), thing_def.sprite_file, sprite_files, set_sprite_file)

        width_changed, new_width = imgui.input_int("width:##obj_{}_width".format(idx), thing_defs[idx].width)
        height_changed, new_height = imgui.input_int("height:##obj_{}_height".format(idx), thing_defs[idx].height)
        speed_changed, new_speed = imgui.input_int("speed:##obj_{}_speed".format(idx), thing_defs[idx].speed)
        floor_draw_off_changed, new_floor_draw_offset = imgui.input_int("floor draw offset:##obj_floor_draw_off{}".format(idx), thing_defs[idx].floor_draw_offset)

        state_options = ["look for player", "follow player", "maybe get picked up", "idle"]
        init_state_changed, new_init_state = imgui.core.combo("init_state:##obj_{}_init_state".format(idx), thing_def.init_state, state_options)

        if name_changed:
            undo.push_state(cur_state)
            thing_defs[idx].name = new_name

        if width_changed:
            undo.push_state(cur_state)
            thing_defs[idx].width = new_width

        if height_changed:
            undo.push_state(cur_state)
            thing_defs[idx].height = new_height
        
        if speed_changed:
            undo.push_state(cur_state)
            thing_defs[idx].speed = new_speed

        if floor_draw_off_changed:
            undo.push_state(cur_state)
            thing_defs[idx].floor_draw_offset = new_floor_draw_offset

        if init_state_changed:
            undo.push_state(cur_state)
            thing_defs[idx].init_state = new_init_state

        imgui.dummy(0, 10)
    
    imgui.end_child()


def draw_things_mode(cur_state):

    if cur_state.cur_thing is not None:
        cur_thing = cur_state.cur_thing
        type_options = ['player'] + [ thing.name for thing in cur_state.map_data.thing_defs]
        type_changed, new_type = imgui.core.combo("type", cur_thing.object_type, type_options)
        if type_changed:
            cur_thing.object_type = new_type

        x_changed, new_x = imgui.input_int("x:##obj_x", cur_thing.x)
        if x_changed:
            cur_thing.x = new_x
        y_changed, new_y = imgui.input_int("x:##obj_y", cur_thing.y)
        if y_changed:
            cur_thing.y = new_y

        sector_opts = ["-1"] + ["{}".format(idx) for idx in range(len(cur_state.map_data.sectors))]
        
        sect_changed,new_sect = imgui.core.combo("sector", cur_thing.sector_num, sector_opts)
        if sect_changed:
            cur_thing.sector_num = new_sect


    def print_thing(thing):
        if thing.object_type == 0:
            thing_name = ' player'
        else:
            thing_name = cur_state.map_data.thing_defs[thing.object_type-1].name
        return str(thing) + " name: {}".format(thing_name)


    def set_cur_thing(idx):
        cur_state.cur_thing = cur_state.map_data.things[idx]

    def delete_thing(thing):
        if cur_state.cur_thing == thing:
            cur_state.cur_thing = None

        for idx,cur_thing in enumerate(cur_state.map_data.things):
            if thing == cur_thing:
                undo.push_state(cur_state)
                del cur_state.map_data.things[idx]
                break
        
        
    utils.draw_list(cur_state, "Things", "Thing list", cur_state.map_data.things, 
                    set_cur_thing,
                    delete_callback=delete_thing,
                    print_item=print_thing)