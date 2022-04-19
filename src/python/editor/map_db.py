from src.python.editor import vertex, line

NUM_SECTOR_CONSTANTS = 4
NUM_SECTOR_GROUP_PARAMS = 8

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
SECT_GROUP_IDX = 3

LIGHT_IDX = 0
ORIG_HEIGHT_IDX = 1
STATE_IDX = 2
TICKS_LEFT_IDX = 3
FLOOR_HEIGHT_IDX = 4
CEIL_HEIGHT_IDX = 5
FLOOR_COLOR_IDX = 6
CEIL_COLOR_IDX = 7

NUM_LINE_PARAMS = 4

WALL_TEXTURE_IDX = 0
WALL_HIGH_COLOR_IDX = 1
WALL_LOW_COLOR_IDX = 2
WALL_SOLID_COLOR_IDX = 3


NUM_VERTEX_PARAMS = 2


class WorldData(object):
    def __init__(self):
        self.name = 'Default name'
        self.num_sectors = 0
        self.num_sector_groups = 0
        self.num_walls = 0
        self.num_vertexes = 0
        self.sectors = []
        self.sector_group_params = []
        self.sector_group_types = []
        self.lines = []
        self.line_params = []
        self.portals = []
        self.vertexes = []

    def generate_c(self):
        raise Exception("Unimplemented")



def add_new_vertex(world_data, x, y):
    world_data.vertexes.append(vertex.Vertex(x, y, world_data.num_vertexes))
    ret = world_data.num_vertexes
    world_data.num_vertexes += 1
    return ret


def add_line_to_sector(world_data, sector_index, v1, v2):  # needs to increment all later sectors wall and portal offsets
    num_sectors = world_data.num_sectors
    wall_base = get_sector_constant(world_data, sector_index, WALL_OFFSET_IDX)
    portal_base = get_sector_constant(world_data, sector_index, PORTAL_OFFSET_IDX)
    num_walls = get_sector_constant(world_data, sector_index, NUM_WALLS_IDX)

    wall_idx = wall_base + num_walls
    portal_idx = portal_base + num_walls

    set_sector_constant(world_data, sector_index, NUM_WALLS_IDX, num_walls + 1)
    ret = wall_idx
    if num_walls == 0:
        world_data.lines.insert(wall_idx, v1)
    world_data.lines.insert(wall_idx + 1, v2)
    world_data.portals.insert(portal_idx, -1)
    for i in range(NUM_LINE_PARAMS):
        world_data.line_params.append(0)

    for sect_idx in range(sector_index + 1, num_sectors):
        woff = get_sector_constant(world_data, sect_idx, WALL_OFFSET_IDX)
        set_sector_constant(world_data, sect_idx, WALL_OFFSET_IDX, woff + 1)
        poff = get_sector_constant(world_data, sect_idx, PORTAL_OFFSET_IDX)
        set_sector_constant(world_data, sect_idx, PORTAL_OFFSET_IDX, poff + 1)

    world_data.num_walls += 1
    return ret

def get_last_or_create_sector_group(world_data):
    if world_data.num_sector_groups == 0:
        return add_new_sector_group(world_data)
    return world_data.num_sector_groups-1

def add_new_sector_group(world_data):
    world_data.sector_group_types.append(0)
    world_data.num_sector_groups += 1
    for i in range(NUM_SECTOR_GROUP_PARAMS):
        world_data.sector_group_params.append(0)

    return world_data.num_sector_groups - 1

def add_new_sector(world_data):
    if world_data.num_sectors == 0:
        world_data.sectors.append(0)
    else:
        world_data.sectors.append(world_data.num_walls+world_data.num_sectors)
    # portal offset
    world_data.sectors.append(world_data.num_walls)
    # num walls
    world_data.sectors.append(0)

    sector_group = get_last_or_create_sector_group(world_data)
    # sector_group
    world_data.sectors.append(sector_group)

    ret = world_data.num_sectors
    world_data.num_sectors += 1
    return ret


def gen_funcs(field, scaler):
    def getter(world_data, base_idx, attr_idx):
        return getattr(world_data, field)[base_idx * scaler + attr_idx]

    def setter(world_data, base_idx, attr_idx, val):
        getattr(world_data, field)[base_idx * scaler + attr_idx] = val

    return getter, setter


# sector constants
# wall_offset, portal_offset, num_walls
get_sector_constant, set_sector_constant = gen_funcs('sectors', NUM_SECTOR_CONSTANTS)

get_sector_group_param, set_sector_group_param = gen_funcs('sector_group_params', NUM_SECTOR_GROUP_PARAMS)

get_sector_group_type, set_sector_group_type = gen_funcs('sector_group_types', 1)

# line -> upper and lower color, texture, vertexes
get_line_param, set_line_param = gen_funcs('line_params', NUM_LINE_PARAMS)

get_vertex_attr, set_vertex_attr = gen_funcs('vertexes', NUM_VERTEX_PARAMS)


def get_all_lines_for_sector(world_data, sector_idx):
    lines = []
    base_offset = get_sector_constant(world_data, sector_idx, WALL_OFFSET_IDX)
    num_walls = get_sector_constant(world_data, sector_idx, NUM_WALLS_IDX)


    for i in range(num_walls):
        idx = base_offset + i
        params = []
        v1_idx = world_data.lines[idx]
        v2_idx = world_data.lines[idx + 1]
        for param_idx in range(NUM_LINE_PARAMS):
            params.append(get_line_param(world_data, idx-sector_idx, param_idx))
        lines.append(line.Line(world_data.vertexes[v1_idx], world_data.vertexes[v2_idx], idx, sector_idx, params))
    return lines


def get_all_lines(world_data):
    res = []
    for i in range(world_data.num_sectors):
        res += get_all_lines_for_sector(world_data, i)

    return res


def get_line(world_data, sector_idx, wall_idx):
    v1_idx = world_data.lines[wall_idx]
    v2_idx = world_data.lines[wall_idx + 1]
    return line.Line(world_data.vertexes[v1_idx], world_data.vertexes[v2_idx], wall_idx, sector_idx)

def delete_line(world_data, sect_idx, wall_idx):
    del world_data.lines[wall_idx+1]
    nwalls = get_sector_constant(world_data, sect_idx, NUM_WALLS_IDX)
    set_sector_constant(world_data, sect_idx, NUM_WALLS_IDX, nwalls-1)

def set_line_v1(world_data, wall_idx, v1):
    world_data.lines[wall_idx] = v1

def set_line_v2(world_data, wall_idx, v2):
    world_data.lines[wall_idx + 1] = v2


def get_all_sectors(world_data):
    return list(range(world_data.num_sectors))

def get_all_sector_groups(world_data):
    return list(range(world_data.num_sector_groups))


def get_all_vertexes_for_sector(world_data, sector_idx):
    verts = []
    base_offset = get_sector_constant(world_data, sector_idx, WALL_OFFSET_IDX)
    num_walls = get_sector_constant(world_data, sector_idx, NUM_WALLS_IDX)
    if num_walls == 0:
        return []
    for i in range(num_walls + 1):
        v_idx = world_data.lines[base_offset + i]
        vert = world_data.vertexes[v_idx]
        verts.append(vert.copy_to_new_sector(sector_idx))
    return verts


def get_all_vertexes(world_data):
    return world_data.vertexes


def get_sect_for_wall(world_data, wall_idx):
    for sect in range(1, world_data.num_sectors):

        walls_up_to = get_sector_constant(world_data, sect, WALL_OFFSET_IDX)
        if wall_idx < walls_up_to:
            return sect-1
    return world_data.num_sectors-1

def get_portal_for_wall(world_data, wall_idx):
    sect = get_sect_for_wall(world_data, wall_idx)
    return wall_idx - sect