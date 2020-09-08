import copy, math, os, pickle, re, sys, struct
from dataclasses import dataclass
import BitVector
from typing import List

wad_data = None
num_directory_entries = None
directory_index = None

directory = {}

def read_wad_data(filename, chunksize=8192):
    global wad_data
    with open(filename, "rb") as f:
        wad_data = f.read()
        
        
def read_byte_at(idx):
    return wad_data[idx]

def read_fixed_len_string(idx,length):
    return ''.join(chr(b) for b in wad_data[idx:idx+length])

def read_terminated_string(idx, maxlen=8):
    chrs = ''
    while True:
        b = wad_data[idx]
        if len(chrs) == maxlen or b == 0:
            break
        chrs += chr(b)
        idx += 1

    return chrs

def read_ints(idx, num=1):
    """Reads a signed 4-byte little endian integer"""
    fmt = '<' + ('l' * num)
    return struct.unpack_from(fmt, wad_data, idx)

def read_shorts(idx, num=1):
    fmt = '<' + ('h' * num)
    return struct.unpack_from(fmt, wad_data, idx)

    
def read_wad_header():
    global num_directory_entries,directory_index
    assert read_fixed_len_string(0, 4) == 'IWAD'
    num_directory_entries,directory_index = read_ints(4, num=2)
    #print("num directory_entries: {}".format(num_directory_entries))


episodes = '1234'
missions = '123456789'

is_doom_two = None

def is_level_name(nm):
    global is_doom_two
    if is_doom_two is None:
        if len(nm) >= 4:
            if nm[0:3] == 'MAP' and nm[4] in missions:
                is_doom_two = True
            elif nm[0] == 'E' and nm[1] in episodes and nm[2] == 'M' and nm[3] in missions:
                is_doom_two = False

    
    if is_doom_two:
        return nm[0:3] == 'MAP'
    else:
        if len(nm) != 4:
            return False
        if nm[0] != 'E':
            return False
        if nm[2] != 'M':
            return False
        if nm[1] not in episodes:
            return False
        if nm[3] not in missions:
            return False
    
        return True


DIRECTORY_ENTRY_SIZE = 16
DIRECTORY_ENTRY_FORMAT = """
  ptr int ;
  size int ; 
  name char[8]
"""
@dataclass
class Directory:
    ptr: int
    size: int
    name: str
    

LINEDEF_SIZE = 14

# could save 3 bytes here
LINEDEF_FORMAT = """
  begin_vert    unsigned_short ; 
  end_vert      unsigned_short ;
  flags         unsigned_short ; 
  line_type     unsigned_short ;
  sector_tag    unsigned_short ;
  right_sidedef short ;
  left_sidedef  short
 """

@dataclass
class Linedef:
    begin_vert: int
    end_vert: int
    flags: int
    line_type: int
    sector_tag: int
    right_sidedef: int
    left_sidedef: int

    def write_c(self):
        return ("{" +
                """
                .v1 = {}, .v2 = {}, 
                .flags = {}, .line_type = {}, .sector_tag = {}, 
                .right_sidedef = {}, .left_sidedef = {}""".format(
                    self.begin_vert, self.end_vert, self.flags, self.line_type,
                    self.sector_tag, self.right_sidedef, self.left_sidedef
        ) + "}")
    

SIDEDEF_SIZE = 30
SIDEDEF_FORMAT = """
  x_off          short ;
  y_off          short ;
  upper_texture  char[8] ;
  lower_texture  char[8] ;
  middle_texture char[8] ;
  sector_ref     unsigned_short
"""

@dataclass
class Sidedef:
    x_off: int
    y_off: int
    upper_texture: str
    lower_texture: str
    middle_texture: str
    sector_ref: int

    def write_c(self):
        return ("{" +
                """
                .x_off = {}, .y_off = {}, 
                .upper_texture = "{}", .lower_texture = "{}", .middle_texture = "{}",
                .sector_ref = {}""".format(
                    self.x_off, self.y_off,
                    self.upper_texture, self.lower_texture, self.middle_texture,
                    self.sector_ref
                    ) + "}")
        
VERTEX_SIZE = 4
VERTEX_FORMAT = """
  x short ;
  y short 
"""

@dataclass
class Vertex:
    x: int
    y: int
    def write_c(self):
        return ("{" +
                ".x = {}, .y = {}".format(self.x, self.y) + 
                "}")

SEG_SIZE = 12
# could save one byte here
SEG_FORMAT = """
  begin_vert unsigned_short ;
  end_vert   unsigned_short ; 
  angle      short ;
  linedef    unsigned_short ;
  direction  short ;
  offset     short
"""

@dataclass
class Seg:
    begin_vert: int
    end_vert: int
    angle: int
    linedef: int
    direction: int
    offset: int
    def write_c(self):
        return ("{" +
                """
                .begin_vert = {}, .end_vert = {},
                .angle = {}, .linedef = {},
                .direction = {}, .offset = {}
                """.format(
                    self.begin_vert, self.end_vert,
                    self.angle, self.linedef,
                    self.direction, self.offset                    
                ) + 
                "}")
    
    
SSECTOR_SIZE = 4
# num_segs can likely be a single byte here, saving a byte
SSECTOR_FORMAT = """
  num_segs  short ;
  first_seg short
"""

@dataclass
class SSector:
    num_segs: int
    first_seg: int
    def write_c(self):
        return (
            "{" +
            ".num_segs = {}, .first_seg = {}".format(self.num_segs, self.first_seg)
            + "}"
            )


NODE_SIZE = 28
# this is pretty big..
# if nodes were sorted, I think we could save 2 bytes here
NODE_FORMAT = """
  partition_x_coord    short ;
  partition_y_coord    short ;
  dx                   short ;
  dy                   short ;
  right_box_top        short ;
  right_box_bottom     short ;
  right_box_left       short ;
  right_box_right      short ;
  left_box_top         short ;
  left_box_bottom      short ;
  left_box_left        short ;
  left_box_right       short ;
  right_child unsigned_short ;
  left_child  unsigned_short
"""

@dataclass
class Node:
    partition_x_coord: int
    partition_y_coord: int
    dx: int
    dy: int
    right_box_top: int
    right_box_bottom: int
    right_box_left: int
    right_box_right: int
    left_box_top: int
    left_box_bottom: int
    left_box_left: int
    left_box_right: int
    right_child: int
    left_child: int

    def write_c(self):
        return (
            "{" +
            """
            .split_x = {}, .split_y = {},
            .split_dx = {}, .split_dy = {},
            .right_box_top = {}, .right_box_bottom = {},
            .right_box_left = {}, .right_box_right = {},
            .left_box_top = {}, .left_box_bottom = {},
            .left_box_left = {}, .left_box_right = {},
            .right_child = {}, .left_child = {}
            """.format(
                self.partition_x_coord, self.partition_y_coord,
                self.dx, self.dy,
                self.right_box_top, self.right_box_bottom,
                self.right_box_left, self.right_box_right,
                self.left_box_top, self.left_box_bottom,
                self.left_box_left, self.left_box_right,
                self.right_child, self.left_child
            )
            + "}"
        )

SECTOR_SIZE = 26
# could save a byte in light_level and sector_special
SECTOR_FORMAT = """
  floor_height            short ;
  ceiling_height          short ;
  floor_texture         char[8] ;
  ceil_texture          char[8] ;
  light_level             short ;
  sector_special unsigned_short ; 
  sector_tag     unsigned_short
"""

@dataclass
class Sector:
    floor_height: int
    ceiling_height: int
    floor_texture: str
    ceil_texture: str
    light_level: int
    sector_special: int
    sector_tag: int
    def write_c(self):
        return (
            "{" +
            """
            .floor_height = {}, .ceil_height = {},
            .floor_texture = "{}", .ceil_texture = "{}",
            .light_level = {}, .sector_special = {},
            .sector_tag = {}
            """.format(self.floor_height, self.ceiling_height,
                       self.floor_texture, self.ceil_texture,
                       self.light_level, self.sector_special,
                       self.sector_tag)
            + "}"
        )
    

THING_SIZE = 10
# could save a byte on flags, and maybe save a byte on thing type
# (are there more then 256 thing types?) 
THING_FORMAT = """
  x_pos       short ;
  y_pos       short ;
  angle       short ;
  thing_type  short ;
  flags       short
"""

@dataclass
class Thing:
    x_pos: int
    y_pos: int
    angle: int
    thing_type: int
    flags: int

    def write_c(self):
        return ("{" +
                ".x = {}, .y = {}, .angle = {}, .type = {}, .flags = {} ".format(
                    self.x_pos, self.y_pos, self.angle, self.thing_type, self.flags
                ) + "}")
                
#@dataclass
#class BigBlockmap():
    
    
def calculate_big_blockmap(blkmap):
    # cell size 256x256
    num_cols = 0
    num_rows = 0
    num_offset_vals = 0
    
    table = []
    offsets = []

    for x in range(0, blkmap.num_columns, 2):
        num_cols += 1
    for y in range(0, blkmap.num_rows,2):
        for x in range(0, blkmap.num_columns, 2):
            num_offset_vals += 2
        num_rows += 1
            
    for y in range(0, blkmap.num_rows,2):
        for x in range(0, blkmap.num_columns, 2):
            b0 = index_blockmap(x, y, blkmap)
            b1 = index_blockmap(x+1, y, blkmap)
            b2 = index_blockmap(x, y+1, blkmap)
            b3 = index_blockmap(x+1, y+1, blkmap)
            new_cell = b0 | b1 | b2 | b3

            
            empty_cell = False
            if len(new_cell) == 0:
                empty_cell = True
            elif len(new_cell) == 1 and 0 in new_cell:
                empty_cell = True
            
            offsets.append(num_offset_vals+len(table))
            if empty_cell:
                offsets.append(0)
            else:
                offsets.append(len(new_cell))
                for idx in new_cell:
                    table.append(idx)
                
            
    #return (blkmap.num_columns, blkmap.num_rows, blkmap.offsets, blkmap.table)
    #return (num_cols, num_rows, offsets, table)
    return Blockmap(x_origin = blkmap.x_origin,
                    y_origin = blkmap.y_origin,
                    num_columns = num_cols,
                    num_rows = num_rows,
                    num_offsets = num_offset_vals,
                    offsets = offsets,
                    table = table)

def index_blockmap(x_cell, y_cell, blockmap):
    if x_cell >= blockmap.num_columns:
        return set()
    if y_cell >= blockmap.num_rows:
        return set()

    
    offset_idx = (y_cell * blockmap.num_columns + x_cell)
    table_idx = blockmap.offsets[offset_idx]
    
    vals = []

    while True:
        val = blockmap.table[table_idx-blockmap.num_offsets]
        if val == 65535:
            break
        #if val != 0:
        vals.append(val)
        table_idx += 1
        

    return set(vals)
        



    
@dataclass
class Blockmap:
    x_origin: int
    y_origin: int
    num_columns: int
    num_rows: int
    num_offsets: int
    offsets: List[int]
    table: List[int]
        
    def write_c(self):
        num_cols = self.num_columns
        num_offs = self.num_offsets
        
        chunked_offsets = [self.offsets[i:i+num_cols] for i in range(0, num_offs, num_cols)]
        chunked_str_offsets = [", ".join([str(off) for off in chk]) for chk in chunked_offsets] 
        chunked_table = [self.table[i:i+10] for i in range(0, len(self.table), 10)]
        chunked_str_table = [", ".join([str(entry) for entry in chk]) for chk in chunked_table]
        
        joined_offsets = ",\n".join(chunked_str_offsets)
        joined_table = ",\n".join(chunked_str_table)
        
        return ("{" +
                """.x_origin = {}, .y_origin = {}, 
                .num_columns = {}, .num_rows = {}, .num_offsets = {},
                .offsets_plus_table = """.format(
                    self.x_origin, self.y_origin,
                    self.num_columns, self.num_rows, self.num_offsets,
                ) + "{\n" +
                joined_offsets + ",\n\n" + joined_table + 
                "\n}" + 
                "}")

def read_blockmap(blockmap_dir, wad_data):
    print("reading blockmap from wad")
    print(blockmap_dir)
    

    cur_idx = blockmap_dir.ptr
    (x_org, y_org, num_cols, num_rows) = struct.unpack_from('hhHH', wad_data, cur_idx)
    cur_idx += 8
    
    num_offs = num_cols*num_rows
    offsets_bytes = num_offs*2
    
    blockmap_table_bytes = blockmap_dir.size - (num_offs*2)
    blockmap_table_entries = blockmap_table_bytes//2
    
    offsets_fmt_string = ('H' * num_offs)

    offsets = [off-4 for off in (struct.unpack_from(offsets_fmt_string, wad_data, cur_idx))]
    cur_idx += offsets_bytes

    table_fmt_string = ('H' * blockmap_table_entries)
    table = list(struct.unpack_from(table_fmt_string, wad_data, cur_idx))

    return Blockmap(x_origin=x_org, y_origin=y_org,
                    num_columns=num_cols, num_rows=num_rows,
                    num_offsets=num_offs, offsets=offsets,
                    table=table)
    
    
                
def strip_zeros(s):
    i = s.find(b'\x00')
    if i == -1:
        return s.decode('ascii')
    return s[:i].decode('ascii')
    
    
    
    
counter = 0
def get_new_int():
    global counter
    res = counter
    counter += 1
    return res

def compile_format_string(fmt):
    
    
    struct_fmt = '<'
    format_map = {
        'short': ('h', 2),
        'unsigned_short': ('H', 2),
        'int':  ('i', 4),
        'unsigned_int': ('I', 4),
        'char[8]': ('8s', 8)}
    
    type_map = {'short': int,
                'unsigned_short': int,
                'int': int,
                'unsigned_int': int,
                'char[8]': str}
    
    parts = list(re.split('\s+', part.strip()) for part in fmt.split(';'))
    part_names = [part[0] for part in parts]
    total_size = 0

    for (name,typ) in parts:
        fmt,size = format_map[typ]
        struct_fmt += fmt
        total_size += size

        
    func_name = "parse_{}".format(get_new_int())
    
    func_code = "def {}(cur_idx, klass):\n".format(func_name)

    func_code += "    results = {}\n"
    func_code += "    parsed = struct.unpack_from(\"{}\", wad_data, cur_idx)\n".format(struct_fmt)
    for idx,part in enumerate(parts):
        name = part[0]
        typ = part[1]
        if 'char' in typ:
            func_code += "    {} = strip_zeros(parsed[{}])\n".format(
                name, idx)
        else:
            func_code += "    {} = parsed[{}]\n".format(name,idx)
    func_code += "    obj = klass({})\n".format(",".join(part_names))
    func_code += "    return obj, cur_idx+{}\n".format(total_size)

    
    
    exec(func_code)
    return eval(func_name)


parse_things = [
    (  'THINGS',   Thing,   THING_FORMAT,   THING_SIZE),
    ('LINEDEFS', Linedef, LINEDEF_FORMAT, LINEDEF_SIZE),
    ('SIDEDEFS', Sidedef, SIDEDEF_FORMAT, SIDEDEF_SIZE),
    ('VERTEXES',  Vertex,  VERTEX_FORMAT,  VERTEX_SIZE),
    (    'SEGS',     Seg,     SEG_FORMAT,     SEG_SIZE),
    ('SSECTORS', SSector, SSECTOR_FORMAT, SSECTOR_SIZE),
    (   'NODES',    Node,    NODE_FORMAT,    NODE_SIZE),
    ( 'SECTORS',  Sector,  SECTOR_FORMAT,  SECTOR_SIZE),
    #('  REJECT',  REJECT_FORMAT,  REJECT_SIZE)
    #('BLOCKMAP', BLOCKMAP_FORMAT, BLOCKMAP_SIZE)
]

compiled_parse_things = []

for (name,klass,format_string, size) in parse_things:
    parse_func = compile_format_string(format_string)
    compiled_parse_things.append((name, parse_func, klass, size))


def maybe_line_of_sight(src_sector_idx, dest_sector_idx, level):
    num_sectors = len(level['SECTORS'])
    bit_idx = (src_sector_idx * num_sectors) + dest_sector_idx

    byte_idx = int(bit_idx / 8)

    
    bit_pos = bit_idx % 8
    
    return (level['REJECT'][byte_idx] & (1 << bit_pos) == 0)


def get_subsector_sector_idx(subsector, level):
    first_seg = level['SEGS'][subsector.first_seg]
    return seg_sector_idx(first_seg, level)


def seg_sector_idx(seg, level):
    linedef = level['LINEDEFS'][seg.linedef]
    if seg.direction == 0:
        sidedef = level['SIDEDEFS'][linedef.right_sidedef]
    else:
        sidedef = level['SIDEDEFS'][linedef.left_sidedef]

    return sidedef.sector_ref

def portal_seg_other_sector_idx(seg, level):
    linedef = level['LINEDEFS'][seg.linedef]
    if seg.direction == 0:
        sidedef = level['SIDEDEFS'][linedef.left_sidedef]
    else:
        sidedef = level['SIDEDEFS'][linedef.right_sidedef]

    return sidedef.sector_ref

    

def read_level_data(level_dir):

    #blockmap   = level_dat['BLOCKMAP']

    results = {}
    for (key,compiled_parse_func,klass,thing_size) in compiled_parse_things:
        data = level_dir[key]
        cur_idx = data.ptr
        total_size = data.size
        
        num_things = total_size/float(thing_size)

        assert num_things == int(num_things)
        num_things = int(num_things)
        
        list_of_things = []
        for i in range(num_things):
            thing,cur_idx = compiled_parse_func(cur_idx, klass)
            list_of_things.append(thing)

        results[key] = list_of_things

    # read reject table
    num_sectors = len(results['SECTORS'])
    reject_table_num_bits = num_sectors*num_sectors
    reject_table_num_bytes = math.ceil(reject_table_num_bits/8.0)
    
    
    idx = level_dir['REJECT'].ptr
    reject_data = wad_data[idx:idx+reject_table_num_bytes]

    
    results['REJECT'] = reject_data

    #level_dir['BLOCKMAP']
    blkmap = read_blockmap(level_dir['BLOCKMAP'], wad_data)
    results['BLOCKMAP'] = blkmap
    results['BIG_BLOCKMAP'] = calculate_big_blockmap(blkmap)

    
    #sys.exit(1)
    
    return results

@dataclass(frozen=True, eq=True, repr=True)
class BaseSidedef():
    x_off: int
    y_off: int
    upper_texture: str
    lower_texture: str
    middle_texture: str

@dataclass(frozen=True, eq=True, repr=True)
class OptSidedef():
    base_sidedef_idx: int
    sector_ref: int

def copy_sidedef_without_sector_ref(sidedef):
    return BaseSidedef(sidedef.x_off,
                       sidedef.y_off,
                       sidedef.upper_texture,
                       sidedef.lower_texture,
                       sidedef.middle_texture)

            
            

def optimize_sidedefs(sidedefs):
    base_sidedefs_map = {}
    base_sidedefs_list = []
    
    idx = 0
    opt_sidedefs = set()
    
    for sidedef in sidedefs:
        base_sidedef = copy_sidedef_without_sector_ref(sidedef) #copy.copy(sidedef)
        sector_ref = sidedef.sector_ref
                
        if base_sidedef not in base_sidedefs_map:
            base_sidedefs_map[base_sidedef] = idx
            base_sidedefs_list.append(base_sidedef)
            idx += 1

        base_idx = base_sidedefs_map[base_sidedef]
        opt_sidedefs.add(OptSidedef(base_idx, sector_ref))

    
    #unique_sidedefs = set(opt_sidedefs)
    #print("num opt sidedefs {}".format(len(opt_sidedefs)))
    #print("num unique opt sidedefs {}".format(len(unique_sidedefs)))
    #sys.exit()
    base_size = 28 * len(base_sidedefs_list)
    unique_sidedefs_size = 4 * len(set(opt_sidedefs))

    return base_sidedefs_list, opt_sidedefs, (base_size + unique_sidedefs_size)

def level_names():
    res = []
    if is_doom_two:

        for i in range(1, 33):
            if i < 10:
                yield 'MAP0{}'.format(i)
                
            else:
                yield 'MAP{}'.format(i)
    else:
        for episode in episodes:
            for mission in missions:
                yield 'E{}M{}'.format(episode, mission)




parse_directory_entry = compile_format_string(DIRECTORY_ENTRY_FORMAT)


def read_directory():
    cur_idx = directory_index

    reading_level_data = False
    cur_level_data = {}
    
    for i in range(num_directory_entries):
        dir_entry,cur_idx = parse_directory_entry(cur_idx, Directory)
        name = dir_entry.name
                
        if is_level_name(name):
            reading_level_data = name
            cur_level_data = {}
        elif reading_level_data:
            cur_level_data[name] = dir_entry
            if name == 'BLOCKMAP':
                directory[reading_level_data] = cur_level_data
                reading_level_data = False
        else:
            directory[name] = dir_entry


def get_cachefile_name(wadfile):
    f = wadfile.split(".wad")[0]
    return "{}.cache".format(f)
    
                
def read_wadfile(wadfile):
    cachefile = get_cachefile_name(wadfile)

    cachefile_exists = os.path.exists(cachefile) 
    if cachefile_exists:
        print("Loading previously dumped WAD {}".format(wadfile))
        with open(cachefile, "rb") as f:
            return pickle.load(f)
            

    
    
    
    read_wad_data(wadfile)
    read_wad_header()
    read_directory()
    
    total_unoptimized_size = 0
    total_optimized_size = 0
    
    print("Reading level data", end='', flush=True)
        
    for level_name in level_names():
        level_directory = directory[level_name]
        level_data = read_level_data(level_directory)
        
        full_size = sum(d.size for d in level_directory.values())

        level_sidedefs = level_data['SIDEDEFS']
        base_sidedefs, opt_sidedefs, size = optimize_sidedefs(level_sidedefs)
        optimized_size = full_size
        optimized_size -= level_directory['SIDEDEFS'].size
        optimized_size += size
        
        total_unoptimized_size += full_size

        directory["{}_DATA".format(level_name)] = level_data
        #print("{} unoptimized size {}".format(level_name, full_size))
        #print("{} optimized size {}".format(level_name, optimized_size))
        
        
        total_optimized_size += optimized_size
        print('.', end='', flush=True)
    print("")

    print("total unoptimized size: {}".format(total_unoptimized_size))
    print("total optimized size: {}".format(total_optimized_size))
    
    if not cachefile_exists:
        print("Saving WAD")
        with open(cachefile, "wb") as f:
            pickle.dump((directory,is_doom_two), f)
    
    return directory,is_doom_two

def dump_level_data(output, level_data):
    if '.c' in output:
        output_level_name = output.split(".c")[0]
    else:
        output_level_name = output
        
    with open(output, 'w') as f:
        f.write("#include \"level.h\"\n")
        size = 0
        for thing in parse_things:
            (name,typ,_,obj_size) = thing
            objs = level_data[name]
            num_objs = len(objs)
            size += obj_size * num_objs
            
            type_name = name[0:-2] if name == 'VERTEXES' else name[0:-1]
            f.write("static const {} {}[{}] = ".format(type_name.lower(), name.lower(), num_objs))
            f.write("{\n")
            for obj in objs:
                f.write("    {},\n ".format(obj.write_c()))
                
            f.write("};\n\n")

        f.write("static const blockmap blkmap = " + level_data['BLOCKMAP'].write_c() + "\n};\n")
        f.write("static const blockmap big_blkmap = " + level_data['BIG_BLOCKMAP'].write_c() + "\n};\n")
        

        level_def = ("const level {}".format(output_level_name) + " = {\n" +
                     """
                     .num_linedefs = {}
                     .linedefs = linedefs,
                     .nodes = nodes,
                     .sectors = sectors,
                     .num_segs = {},
                     .segs = segs,
                     .sidedefs = sidedefs,
                     .ssectors = ssectors,
                     .things = things,
                     .num_vertexes = {},
                     .vertexes = vertexes
                     """.format(
                         len(level_data['LINEDEFS']),
                         len(level_data['SEGS']),
                         len(level_data['VERTEXES'])
                     ) + "};\n")

        f.write(level_def)
        size += len(level_data['REJECT'])
        
    print("dumped level with {} bytes to {}".format(size, output))
            
            
    
    
