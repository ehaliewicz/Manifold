import configparser
import os

import map 
from modes import Mode 
import utils

class State(object):
    def __init__(self, cur_path):
        self.mode = Mode.SECTOR
        self.cur_sector = None
        self.cur_wall = None
        self.cur_vertex = None
        self.cur_thing = None
        self.cur_sector_group = None
        self.cur_sector_pvs = None

        self.last_sector_inside = None

        self.camera_x = 0
        self.camera_y = 0
        self.zoom = 0
        

        self.emulator_path = ""
        self.xgmtool_path = ""
        self.textures_path = "textures/"
        self.music_tracks_path = "music/"
        self.sprites_path = "sprites/"

        self.load_config(cur_path)
        tex_files = utils.get_texture_files(self)
        sprite_files = utils.get_sprite_files(self)

        self.hovered_item = None

        self.default_up_color = 3
        self.default_low_color = 3
        self.default_mid_color = 0
        self.default_floor_color = 4
        self.default_ceil_color = 5
        self.default_texture_file = tex_files[0]
        self.default_sprite_file = sprite_files[0]
        self.default_thing_type = 0

        self.map_data: map.Map = map.Map(self.default_sprite_file)

    def load_config(self, cur_path):
        conf_file_path = os.path.join(cur_path, "conf.ini")
        conf_exists = os.path.exists(conf_file_path)
        if not conf_exists:
            print("No configuration file, using defaults!")
            return 
        config = configparser.ConfigParser()
        config.read(conf_file_path)
        self.emulator_path = config.get("Default Settings", "emulator-path")
        self.xgmtool_path = config.get("Default Settings", "xgmtool-path")
        self.textures_path = config.get("Default Settings", "textures-path")
        self.sprites_path = config.get("Default Settings", "sprites-path")
        self.music_tracks_path = config.get("Default Settings", "music-tracks-path")
        self.megalink_path = config.get("Default Settings", "megalink-path")
