from tkinter import filedialog, messagebox
import os
import struct 
import subprocess
import time

import palette
import sprite_utils
import texture_utils

def _read_longword_(f, signed):
    return int.from_bytes(f.read(4), "big", signed)
def read_s32(f):
    return _read_longword_(f, True)
def read_u32(f):
    return _read_longword_(f, False)

def _write_longword_(f, lw, signed):
    off = f.tell()
    f.write(lw.to_bytes(4, byteorder="big", signed=signed))
    return off 

def write_s32(f, lw):
    return _write_longword_(f, lw, True)
def write_u32(f, lw):
    return _write_longword_(f, lw, False)


def _read_word_(f, signed):
    return int.from_bytes(f.read(2), "big", signed)

def read_s16(f):
    return _read_word_(f, True)
def read_u16(f):
    return _read_word_(f, False)

def _write_word_(f, w, signed):
    off = f.tell()
    f.write(w.to_bytes(2, byteorder="big", signed=signed))
    return off 

def write_u16(f, w):
    _write_word_(f, w, False)
def write_s16(f, w):
    assert w >= -32768 and w < 32768
    _write_word_(f, w, True)


def _read_byte_(f, signed):
    return int.from_bytes(f.read(1), byteorder="big", signed=signed)
def _write_byte_(f, b, signed):
    off = f.tell()
    f.write(b.to_bytes(1, byteorder="big", signed=signed))
    return off

def read_s8(f):
    return _read_byte_(f, True)
def read_u8(f):
    return _read_byte_(f, False)
def write_s8(f, b):
    return _write_byte_(f, b, True)
def write_u8(f, b):
    return _write_byte_(f, b, False)


def pointer_placeholder(f, val=0xDEADBEEF):
    return write_u32(f, val)

def patch_pointer_to_current_offset(f, ptr_off):
    ptr = f.tell()
    f.seek(ptr_off)
    write_u32(f, ptr)
    f.seek(ptr)

def align(f):
    if f.tell() & 0b1:
        f.seek(f.tell()+1)


launched_emu_process = None 
def launch_emulator(emu_path, rom_path):
    global launched_emu_process
    if emu_path == "":
        return
    if rom_path == "":
        return
    if launched_emu_process is not None:
        if launched_emu_process.poll() is None:
            launched_emu_process.kill()
        launched_emu_process = None
    try:
        launched_emu_process = subprocess.Popen([emu_path, rom_path])
    except Exception as e:
        print("error launching {}".format(e))
        return



last_exported_rom_file = None

def reset_last_exported_rom_file():
    global last_exported_rom_file
    last_exported_rom_file = None

def err(msg):
    messagebox.showerror(
        title="Error",
        message=msg,
    )
    return


def export_map_to_rom(cur_path, cur_state, set_launch_flags=False):
    global last_exported_rom_file
    if last_exported_rom_file is None:
        f = filedialog.asksaveasfile(mode="r")
    else:
        f = open(last_exported_rom_file, "r")
    if f is None:
        return 

    last_exported_rom_file = os.path.realpath(f.name)


    f.close()

    with open(last_exported_rom_file, "rb+") as f:
        s = f.read()
        map_table_base = s.find(b'\xDE\xAD\xBE\xEF') 
        #s.find(b'mapt') 
        num_maps_offset = map_table_base + 4
        map_pointer_offset =  map_table_base + 20
        if map_table_base == -1:
            return err("Couldn't find map table, is this a correct ROM file?")
        map_data_base = s.find(b'WAD?')
        if map_data_base == -1:
            return err("Couldn't find map data table, is this a correct ROM file?")

        map_struct_offset = map_data_base + 4

        start_in_game_flag_offset = s.find(b'\xFE\xED\xBE\xEF')
        if start_in_game_flag_offset == -1:
            return err("Couldn't find init flag table, is this a correct ROM file?")
        start_in_game_flag_offset += 4 
        init_load_level_off = s.find(b'\xBE\xEF\xFE\xED')
        if init_load_level_off == -1:
            return err("Couldn't find init load table, is this a correct ROM file?")

        texture_table_off = s.find(b'\xBE\xEF\xF0\x0F')
        if texture_table_off == -1:
            return err("Couldn't find texture table, is this ROM file correct?")
        texture_table_off += 4

        thing_def_table_off = s.find(b"player_object_type")
        if thing_def_table_off == -1:
            return err("Couldn't find thing definition table, is this ROM file correct?")
        
        # skip past name field of object type, which is the last 32 bytes
        thing_def_table_off += 32

        data = cur_state.map_data
        for sect in data.sectors:
            if not sect.is_convex():
                messagebox.showerror(
                    title="Error",
                    message="Sector {} is not convex".format(sect.index)
                )
                return


        init_load_level_off += 4
        if set_launch_flags:
            f.seek(start_in_game_flag_offset)
            write_u32(f, 1)

            f.seek(init_load_level_off)
            write_u32(f, 3)
        else:
            f.seek(start_in_game_flag_offset)
            write_u32(f, 0)

            f.seek(init_load_level_off)
            write_u32(f, 0)

        

        # write number of maps (HARDCODED to 4 in this case)
        f.seek(num_maps_offset)
        write_u32(f, 4)

        # write address of map struct (in map data table)
        f.seek(map_pointer_offset)
        write_u32(f, map_struct_offset)

        
        num_sectors = len(data.sectors)
        num_vertexes = len(data.vertexes)
        num_walls = sum(len(sect.walls) for sect in data.sectors)

        f.seek(map_struct_offset)
        # write num_sector_groups, currently the same as num sectors (no groups supported in editor yet)
        write_u16(f, num_sectors)
        # write num_sectors, num_walls, and num_verts
        write_u16(f, num_sectors)
        write_u16(f, num_walls)
        write_u16(f, num_vertexes)
        
        sectors_ptr_offset = pointer_placeholder(f)
        sector_group_types_ptr_offset = pointer_placeholder(f)
        sector_group_params_ptr_offset = pointer_placeholder(f)
        sector_group_triggers_ptr_offset = pointer_placeholder(f)
        walls_ptr_offset = pointer_placeholder(f)
        portals_ptr_offset = pointer_placeholder(f)
        wall_colors_ptr_offset = pointer_placeholder(f)
        vertexes_ptr_offset = pointer_placeholder(f)
        wall_norm_quads_ptr_offset = pointer_placeholder(f)
        # write has_pvs (HARDCODED to false right now)
        write_u16(f, 0)
        # write pvs pointers(HARDCODED to NULL)
        pvs_ptr_offset = write_u32(f, 0)
        raw_pvs_ptr_offset = write_u32(f, 0) 
        name_ptr_offset = pointer_placeholder(f)
        music_ptr_offset = pointer_placeholder(f, 0) # define to NULL
        palette_ptr_offset = pointer_placeholder(f, 0) # default to NULL

        write_u16(f, len(cur_state.map_data.things)) # num things
        thing_ptr_offset = pointer_placeholder(f, 0) # things pointer, NULL for now

        patch_pointer_to_current_offset(
            f, sectors_ptr_offset
        )

        
        wall_offset = 0
        portal_offset = 0
        for sect in data.sectors:
            sect_num_walls = len(sect.walls)
            f.write(struct.pack(
                ">hhhh",
                wall_offset, portal_offset,
                sect_num_walls, sect.index
            ))
            if len(sect.walls) == 0:
                continue
            wall_offset += sect_num_walls+1
            portal_offset += sect_num_walls

        patch_pointer_to_current_offset(
            f, sector_group_types_ptr_offset
        )
        for sect in data.sectors:
            f.write(b'\x00')
        align(f)

        patch_pointer_to_current_offset(
            f, sector_group_params_ptr_offset
        )

        for sect in data.sectors:
            f.write(struct.pack(
                # light level, orig_height, ticks_left, state
                # floor_height, ceil_height, floor_color, ceil_color
                ">hhhhhhhh",
                0, 0, 0, 0, 
                sect.floor_height*16, sect.ceil_height*16,
                sect.floor_color, sect.ceil_color
            ))
        
        patch_pointer_to_current_offset(
            f, sector_group_triggers_ptr_offset
        )
        for sect in data.sectors:
            f.write(struct.pack(
                ">hhhhhhhh", 0,0,0,0,0,0,0,0
            ))

        patch_pointer_to_current_offset(
            f, walls_ptr_offset
        )
        for sect in data.sectors:
            prev_v2 = None
            if len(sect.walls) == 0:
                continue
            first_v1 = sect.walls[0].v1
            for wall in sect.walls:
                if prev_v2 is not None:
                    if prev_v2 != wall.v1:
                        messagebox.showerror(
                            title="Error",
                            message="Sector {} is not complete".format(sect.index)
                        )
                    
                write_u16(f, wall.v1.index)
                prev_v2 = wall.v2 
            write_u16(f, first_v1.index)

        patch_pointer_to_current_offset(
            f, portals_ptr_offset
        )
        for sect in data.sectors:
            for wall in sect.walls:
                write_s16(f, wall.adj_sector_idx)

        patch_pointer_to_current_offset(
            f, wall_colors_ptr_offset
        )

        
        # gather texture files and map to indexes
        tex_file_to_idx_map = {}
        tex_file_list = []
        cur_tex_idx = 0
        for sect in data.sectors:
            for wall in sect.walls:
                tex_file = wall.texture_file
                if tex_file not in tex_file_to_idx_map:
                    tex_file_to_idx_map[tex_file] = cur_tex_idx
                    tex_file_list.append(tex_file)
                    cur_tex_idx += 1


        for sect in data.sectors:
            for wall in sect.walls:
                tex_idx = tex_file_to_idx_map[wall.texture_file]
                f.write(struct.pack(
                    ">BBBB", 
                    tex_idx, 
                    wall.up_color,
                    wall.low_color,
                    wall.mid_color
                ))
        align(f)
        
        patch_pointer_to_current_offset(
            f, vertexes_ptr_offset
        )
        for vert in data.vertexes:
            write_s16(f, int(vert.x*1.3))
            write_s16(f, int((-vert.y)*1.3))


        patch_pointer_to_current_offset(
            f, wall_norm_quads_ptr_offset
        )
        for sect in data.sectors:
            for wall in sect.walls:
                write_u8(f, wall.normal_quadrant_int())
        align(f)

        patch_pointer_to_current_offset(
            f, name_ptr_offset
        )
        for char in data.name:
            f.write(str.encode(char))
        # null terminate name
        f.write(b'\x00')

        ### write music data if applicable
        align(f)
        if data.music_path != "" and cur_state.xgmtool_path != "":
            patch_pointer_to_current_offset(f, music_ptr_offset)
            # write music data
            output_path = os.path.join(cur_path, "xgm_output.bin")
            full_music_path = os.path.join(cur_state.music_tracks_path, data.music_path)
            cmd = [cur_state.xgmtool_path, full_music_path, output_path]
            
            try:
                pid = subprocess.Popen(cmd)
                while pid.poll() is None:
                    time.sleep(0.01)

                with open(output_path, "rb") as xgm_bin_file:
                    xgm_data = xgm_bin_file.read()
                    f.write(xgm_data)
            except:
                messagebox.showerror(
                    title="Error",
                    message="Error converting or writing music, skipping".format()
                )

        # write palette data
        align(f)
        patch_pointer_to_current_offset(f, palette_ptr_offset)
        for idx in range(16):
            clr = cur_state.map_data.palette[idx]
            (r,g,b) = clr
            col = palette.rgb_to_vdp_color(int(r*255),
                                           int(g*255),
                                           int(b*255))
            write_u16(f, col)

        # write textures to WAD area
        
        align(f)
        tex_pointers_to_write = []
        for tex_file in tex_file_list:
            full_tex_path = os.path.join(cur_state.textures_path, tex_file)
            light_tex_words, dark_tex_words = texture_utils.gen_texture_words_from_file(full_tex_path)

            
            # write actual textures
            light_texture_addr = f.tell()
            for word in light_tex_words:
                write_u16(f, word)
            dark_texture_addr = f.tell()
            for word in dark_tex_words:
                write_u16(f, word)
            

            # dark lit_texture struct
            dark_lit_texture_struct = f.tell()
            write_u32(f, dark_texture_addr)
            write_u32(f, dark_texture_addr)
            write_u32(f, light_texture_addr)
            # mid lit_texture struct
            mid_lit_texture_struct = f.tell()
            write_u32(f, dark_texture_addr)
            write_u32(f, dark_texture_addr)
            write_u32(f, light_texture_addr)
            # light lit_texture struct
            light_lit_texture_struct = f.tell()
            write_u32(f, light_texture_addr)
            write_u32(f, light_texture_addr)
            write_u32(f, light_texture_addr)
            tex_pointers_to_write.append(dark_lit_texture_struct)
            tex_pointers_to_write.append(mid_lit_texture_struct)
            tex_pointers_to_write.append(mid_lit_texture_struct)
            tex_pointers_to_write.append(mid_lit_texture_struct)
            tex_pointers_to_write.append(light_lit_texture_struct)
            tex_pointers_to_write.append(0)
            tex_pointers_to_write.append(0)
            tex_pointers_to_write.append(0)


        # write pointers to texture table
        prev_wad_ptr = f.tell()
        f.seek(texture_table_off)
        for ptr in tex_pointers_to_write:
            write_u32(f, ptr)


        
        #return last_exported_rom_file
        # get list of sprites to write to WAD
        sprite_to_idx_map = {}
        sprite_to_wad_offset_map = {}
        sprite_list = []
        for thing_def in cur_state.map_data.thing_defs:
            if thing_def.sprite_file not in sprite_to_idx_map:
                sprite_to_idx_map[thing_def.sprite_file] = len(sprite_list)
                sprite_list.append(thing_def.sprite_file)



        # generate sprite rle tables
        # write sprite rle data to WAD
        sprite_span_offsets = [] # offsets to span data per column, per sprite
        sprite_texel_offsets = [] # offsets to texel data per column, per sprite
        f.seek(prev_wad_ptr)
        for sprite in sprite_list:
            align(f)

            columns = sprite_utils.compile_image(os.path.join(cur_state.sprites_path, sprite))

            span_offsets = []
            texel_offsets = []
            # write span and texel data for columns
            for column in columns:
                align(f)
                if len(column) == 0:
                    span_offsets.append(0)
                    texel_offsets.append(0)
                    continue
                span_offsets.append(f.tell())
                for (skip,length,_) in column:
                    write_u16(f, skip)
                    write_u16(f, length)
                align(f)
                texel_offsets.append(f.tell())
                for (_,_,texels) in column:
                    for texel in texels:
                        write_u8(f, texel)

            sprite_to_wad_offset_map[sprite] = f.tell()
            # write number of columns as 2 bytes
            write_u16(f, len(columns))

            for idx,column in enumerate(columns):
                span_offset = span_offsets[idx]
                texel_offset = texel_offsets[idx]
                # write number of spans as 2 bytes
                write_u16(f, len(column))
                #spans_ptr = f.tell()+4
                #spans_bytes = len(column)*4
                write_u32(f, span_offset) # kind of a waste, whatever 
                write_u32(f, texel_offset)

        # write thing definitions to thing def table
        prev_wad_ptr = f.tell()
        f.seek(thing_def_table_off)
        for thing_def in cur_state.map_data.thing_defs:
            struct_start = f.tell()
            write_u32(f, sprite_to_wad_offset_map[thing_def.sprite_file])
            write_u16(f, thing_def.floor_draw_offset*16)
            write_u16(f, thing_def.width)
            write_u16(f, thing_def.height*16)
            write_u16(f, thing_def.init_state)
            write_u16(f, thing_def.speed)
            write_u16(f, 0) # not player :^)
            assert len(thing_def.name) <= 32, "thing def name is too long!"
            for c in thing_def.name:
                f.write(str.encode(c))
            f.seek(struct_start + 48) # skip past this struct 

        f.seek(prev_wad_ptr)
        # write map things
        patch_pointer_to_current_offset(f, thing_ptr_offset)
        for thing in cur_state.map_data.things:
            write_u16(f, thing.sector_num)
            write_s16(f, int(thing.x*1.3))
            write_s16(f, int((-thing.y)*1.3))
            write_s16(f, thing.z)
            write_u16(f, thing.object_type)


        print("Wrote {} bytes".format(prev_wad_ptr - map_data_base))

             



    return last_exported_rom_file

def export_map_to_rom_as(cur_path, cur_state, set_launch_flags):
    reset_last_exported_rom_file()
    export_map_to_rom(cur_path, cur_state, set_launch_flags)

def export_and_launch(cur_path, cur_state, set_launch_flags):
    rom_name = export_map_to_rom(cur_path, cur_state, set_launch_flags=set_launch_flags)
    launch_emulator(cur_state.emulator_path, rom_name)
                
def export_and_launch_as(cur_path, cur_state, set_launch_flags):
    reset_last_exported_rom_file()
    export_and_launch(cur_path, cur_state, set_launch_flags)