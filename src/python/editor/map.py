import palette 
import things 
import sector 
import sector_group
import vertex

import typing

class Map():
    def __init__(self, 
    default_sprite,
    name="placeholder name", 
    sectors:typing.List[sector.Sector] | None=None, 
    vertexes:typing.List[vertex.Vertex] | None=None,
    music_path=""):
        self.bsp = False
        if not sectors:
            self.sectors: typing.List[sector.Sector] = []
        else:
            self.sectors: typing.List[sector.Sector] = sectors
        #self.walls = []

        if not vertexes:
            self.vertexes = []
        else:
            self.vertexes = vertexes

        self.name: str = name
        self.music_path: str = music_path
        self.palette = palette.DEFAULT_PALETTE
        self.thing_defs = [things.ThingDef(default_sprite) for i in range(31)]
        self.things: typing.List[things.Thing] = []
        self.sector_groups: typing.List[sector_group.SectorGroup] = []
