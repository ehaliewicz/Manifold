from src.python.editor import vertex, line

NUM_SECTOR_CONSTANTS = 4
NUM_SECTOR_PARAMS = 8

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

WALL_OFFSET_IDX = 0
PORTAL_OFFSET_IDX = 1
NUM_WALLS_IDX = 2
FLAGS_IDX = 3

LIGHT_IDX = 0
ORIG_HEIGHT_IDX = 1
STATE_IDX = 2
TICKS_LEFT_IDX = 3
FLOOR_HEIGHT_IDX = 4
CEIL_HEIGHT_IDX = 5
FLOOR_COLOR_IDX = 6
CEIL_COLOR_IDX = 7

NUM_LINE_PARAMS = 4

NUM_VERTEX_PARAMS = 2


class WorldData(object):
    def __init__(self):
        self.name = 'Default name'
        self.num_sectors = 0
        self.num_walls = 0
        self.num_vertexes = 0
        self.sectors = []
        self.sector_params = []
        self.sector_types = []
        self.lines = []
        self.line_params = []
        self.portals = []
        self.vertexes = []


world_data = WorldData()


def add_new_vertex(x, y):
    world_data.vertexes.append(vertex.Vertex(x, y, world_data.num_vertexes))
    ret = world_data.num_vertexes
    world_data.num_vertexes += 1
    return ret


def add_line_to_sector(sector_index, v1, v2):  # needs to increment all later sectors wall and portal offsets
    num_sectors = world_data.num_sectors
    wall_base = get_sector_constant(sector_index, WALL_OFFSET_IDX)
    portal_base = get_sector_constant(sector_index, PORTAL_OFFSET_IDX)
    num_walls = get_sector_constant(sector_index, NUM_WALLS_IDX)

    wall_idx = wall_base + num_walls
    portal_idx = portal_base + num_walls

    set_sector_constant(sector_index, NUM_WALLS_IDX)(num_walls + 1)
    ret = wall_idx
    if num_walls == 0:
        world_data.lines.insert(wall_idx, v1)
    world_data.lines.insert(wall_idx + 1, v2)
    world_data.portals.insert(portal_idx, -1)
    for i in range(NUM_LINE_PARAMS):
        world_data.line_params.append(0)

    for sect_idx in range(sector_index + 1, num_sectors):
        woff = get_sector_constant(sect_idx, WALL_OFFSET_IDX)
        set_sector_constant(sect_idx, WALL_OFFSET_IDX, woff + 1)
        poff = get_sector_constant(sect_idx, PORTAL_OFFSET_IDX)
        set_sector_constant(sect_idx, PORTAL_OFFSET_IDX, poff + 1)

    world_data.num_walls += 1
    return ret


def add_new_sector():
    if world_data.num_sectors == 0:
        world_data.sectors.append(0)
    else:
        world_data.sectors.append(world_data.num_walls+world_data.num_sectors)
    world_data.sectors.append(world_data.num_walls)
    world_data.sectors.append(0)
    world_data.sectors.append(0)

    world_data.sector_types.append(0)
    for i in range(NUM_SECTOR_PARAMS):
        world_data.sector_params.append(0)
    ret = world_data.num_sectors
    world_data.num_sectors += 1
    return ret


def gen_funcs(field, scaler):
    def getter(base_idx, attr_idx):
        return getattr(world_data, field)[base_idx * scaler + attr_idx]

    def setter(base_idx, attr_idx):
        def setter2(val):
            getattr(world_data, field)[base_idx * scaler + attr_idx] = val

        return setter2

    return getter, setter


# sector constants
# wall_offset, portal_offset, num_walls
get_sector_constant, set_sector_constant = gen_funcs('sectors', NUM_SECTOR_CONSTANTS)

get_sector_param, set_sector_param = gen_funcs('sector_params', NUM_SECTOR_PARAMS)

get_sector_type, set_sector_type = gen_funcs('sector_types', 1)

# line -> upper and lower color, texture, vertexes
get_line_param, set_line_param = gen_funcs('line_params', NUM_LINE_PARAMS)

get_vertex_attr, set_vertex_attr = gen_funcs('vertexes', NUM_VERTEX_PARAMS)


def get_all_lines_for_sector(sector_idx):
    lines = []
    base_offset = get_sector_constant(sector_idx, WALL_OFFSET_IDX)
    num_walls = get_sector_constant(sector_idx, NUM_WALLS_IDX)


    for i in range(num_walls):
        idx = base_offset + i
        params = []
        v1_idx = world_data.lines[idx]
        v2_idx = world_data.lines[idx + 1]
        for param_idx in range(NUM_LINE_PARAMS):
            params.append(get_line_param(idx-sector_idx, param_idx))
        lines.append(line.Line(world_data.vertexes[v1_idx], world_data.vertexes[v2_idx], idx, params))
    return lines


def get_all_lines():
    res = []
    for i in range(world_data.num_sectors):
        res += get_all_lines_for_sector(i)

    return res


def get_line(wall_idx):
    v1_idx = world_data.lines[wall_idx]
    v2_idx = world_data.lines[wall_idx + 1]
    return line.Line(world_data.vertexes[v1_idx], world_data.vertexes[v2_idx], wall_idx)

def delete_line(sect_idx, wall_idx):
    del world_data.lines[wall_idx+1]
    nwalls = get_sector_constant(sect_idx, NUM_WALLS_IDX)
    set_sector_constant(sect_idx, NUM_WALLS_IDX)(nwalls-1)

def set_line_v1(wall_idx, v1):
    world_data.lines[wall_idx] = v1

def set_line_v2(wall_idx, v2):
    world_data.lines[wall_idx + 1] = v2


def get_all_sectors():
    return list(range(world_data.num_sectors))


def get_all_vertexes_for_sector(sector_idx):
    verts = []
    base_offset = get_sector_constant(sector_idx, WALL_OFFSET_IDX)
    num_walls = get_sector_constant(sector_idx, NUM_WALLS_IDX)
    if num_walls == 0:
        return []
    for i in range(num_walls + 1):
        v_idx = world_data.lines[base_offset + i]
        vert = world_data.vertexes[v_idx]
        verts.append(vert.copy_to_new_sector(sector_idx))
    return verts


def get_all_vertexes():
    return world_data.vertexes
    for i in range(world_data.num_sectors):
        res += get_all_vertexes_for_sector(i)
    return res
