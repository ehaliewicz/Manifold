from tkinter import filedialog, messagebox
import os
import struct 
import subprocess
import time

import palette
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
            messagebox.showerror(
                title="Error",
                message="Couldn't find map table, is this a correct ROM file?"
            )
            return
        map_data_base = s.find(b'WAD?')
        if map_data_base == -1:
            messagebox.showerror(
                title="Error",
                message="Couldn't find map data table, is this a correct ROM file?"
            )
            return

        map_struct_offset = map_data_base + 4

        start_in_game_flag_offset = s.find(b'\xFE\xED\xBE\xEF')
        if start_in_game_flag_offset == -1:
            messagebox.showerror(
                title="Error",
                message="Couldn't find init flag table, is this a correct ROM file?"
            )
            return
        start_in_game_flag_offset += 4 
        init_load_level_off = s.find(b'\xBE\xEF\xFE\xED')
        if init_load_level_off == -1:
            messagebox.showerror(
                title="Error",
                message="Couldn't find init load table, is this a correct ROM file?"
            )
            return

        texture_table_off = s.find(b'\xBE\xEF\xF0\x0F')
        if texture_table_off == -1:
            messagebox.showerror(
                title="Error",
                message="Couldn't find texture table, is this ROM file correct?"
            )
            return
        texture_table_off += 4
        

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
        print("current offset {}".format(f.tell()))
        patch_pointer_to_current_offset(f, palette_ptr_offset)
        for idx in range(16):
            clr = cur_state.map_data.palette[idx]
            (r,g,b) = clr
            col = palette.rgb_to_vdp_color(int(r*255),
                                           int(g*255),
                                           int(b*255))
            print("writing color {}".format(col))
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
        f.seek(texture_table_off)
        for ptr in tex_pointers_to_write:
            write_u32(f, ptr)


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