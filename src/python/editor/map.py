import palette 
import things 

class Map():
    def __init__(self, 
    default_sprite,
    name="placeholder name", 
    sectors=None, 
    vertexes=None,
    music_path=""):
        self.bsp = False
        if not sectors:
            self.sectors = []
        else:
            self.sectors = sectors
        #self.walls = []

        if not vertexes:
            self.vertexes = []
        else:
            self.vertexes = vertexes

        self.name: str = name
        self.music_path: str = music_path
        self.palette = palette.DEFAULT_PALETTE
        self.thing_defs = [things.ThingDef(default_sprite) for i in range(31)]
        self.things = []
        self.sector_groups = []
