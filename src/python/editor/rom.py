import struct

def read_uchar(bytes, idx):
    val = struct.unpack_from(">B", bytes, idx)
    return val[0], idx + 1

def read_schar(bytes, idx):
    val = struct.unpack_from(">b", bytes, idx)
    return val[0], idx + 1

def read_uword(bytes, idx):
    val = struct.unpack_from(">H", bytes, idx)
    return val[0], idx + 2


def read_sword(bytes, idx):
    val = struct.unpack_from(">h", bytes, idx)
    return val[0], idx + 2


def read_ulong(bytes, idx):
    val = struct.unpack_from(">i", bytes, idx)
    return val[0], idx + 4


def read_slong(bytes, idx):
    val = struct.unpack_from(">I", bytes, idx)
    return val[0], idx + 4

def read_array(read_func, bytes, idx, size):
    vals = []
    for i in range(size):
        val,idx = read_func(bytes, idx)
        vals.append(val)

    return vals,idx

class Rom(object):
    def __init__(self, file):
        self.maps = []
        self.read_file(file)

    def read_file(self, f):
        with open(f, "rb") as f:
            bytes = f.read()
        idx = bytes.find(b'\xde\xad\xbe\xef')
        print("founds maps list at: {}".format(idx))
        idx += 4
        num_maps, idx = read_ulong(bytes, idx)
        max_maps, idx = read_ulong(bytes, idx)

        for i in range(num_maps):
            map_addr, idx = read_ulong(bytes, idx)
            self.maps.append(Map(map_addr, bytes))

SECTOR_SIZE = 4
NUM_SECTOR_PARAMS = 8
WALL_COLOR_NUM_PARAMS = 4
NUM_TEXTURES = 2

class Map(object):
    def __init__(self, addr, bytes):
        self.orig_rom_addr = addr
        self.read_map(addr, bytes)

    def read_map(self, addr, bytes):
        idx = addr
        num_sectors, idx = read_uword(bytes, idx)
        num_walls, idx = read_uword(bytes, idx)
        num_verts, idx = read_uword(bytes, idx)

        sector_array_addr, idx = read_ulong(bytes, idx)
        sector_type_addr, idx = read_ulong(bytes, idx)
        sector_param_addr, idx = read_ulong(bytes, idx)
        walls_addr, idx = read_ulong(bytes, idx)
        portals_addr, idx = read_ulong(bytes, idx)
        wall_colors_addr, idx = read_ulong(bytes, idx)
        vertexes_addr, idx = read_ulong(bytes, idx)
        wall_norm_quadrants_addr, idx = read_ulong(bytes, idx)
        textures_addr, idx = read_ulong(bytes, idx)

        self.sectors,_       = read_array(read_sword, bytes, sector_array_addr, size=num_sectors*SECTOR_SIZE)
        self.sector_types,_  = read_array(read_uchar, bytes, sector_type_addr, size=num_sectors)
        self.sector_params,_ = read_array(read_sword, bytes, sector_param_addr, size=num_sectors*NUM_SECTOR_PARAMS)
        self.walls,_         = read_array(read_uword, bytes, walls_addr, size=num_walls)
        self.portals,_       = read_array(read_sword, bytes, portals_addr, size=num_walls)
        self.wall_colors,_   = read_array(read_uchar, bytes, wall_colors_addr, size=num_walls*WALL_COLOR_NUM_PARAMS)
        self.vertexes,_      = read_array(read_uword, bytes, vertexes_addr, size=num_verts*2)
        self.wall_norm_quadrants,_ = read_array(read_uchar, bytes, wall_norm_quadrants_addr, size=num_walls)
        self.textures,_      = read_array(read_ulong, bytes, textures_addr, size=NUM_TEXTURES)


if __name__ == '__main__':
    rom = Rom("C:\\Users\\Erik\\code\\genesis\\DOOM\\out\\rom.bin")
