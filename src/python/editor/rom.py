import io
from tkinter import filedialog, messagebox
import os
import struct 
import subprocess
import time
import typing

import palette
import pvs
import sector_group
import sprite_utils
import state
import texture_utils
import utils

from file_utils import write_u8, write_s8, write_u16, write_s16, write_u32

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
    if rom_path == "" or rom_path is None:
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
    
launched_megalink_process = None
def launch_hardware(megalink_path, rom_path):
    global launched_megalink_process
    if megalink_path == "":
        return
    if rom_path == "" or rom_path is None:
        return 
    
    if launched_megalink_process is not None:
        if launched_megalink_process.poll() is None:
            launched_megalink_process.kill()
        launched_megalink_process = None
    try:
        launched_megalink_process = subprocess.Popen([megalink_path, rom_path])
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

def export_map_to_rom(cur_path, cur_state: state.State, set_launch_flags=False, texture_indexes=None, music_indexes=None):
    global last_exported_rom_file
    if last_exported_rom_file is None:
        f = filedialog.asksaveasfile(mode="r")
    else:
        f = open(last_exported_rom_file, "r")
    if f is None:
        return 

    last_exported_rom_file = os.path.realpath(f.name)


    f.close()


    try:
        with open(last_exported_rom_file, "rb+") as f:
            s = f.read()
            map_table_base = s.find(b'\xDE\xAD\xBE\xEF')
            #s.find(b'mapt') 
            num_maps_offset = map_table_base + 4
            map_pointer_offset =  map_table_base + 20
            if map_table_base == -1:
                return err("Couldn't find map table, is this a valid ROM file?")
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

            # gather live sectors and remap indexes
            #live_sector_group_map_tbl = {} # maps original sector group idx to current sector group idx
            #new_sector_group_idx = 0
            #live_sector_groups: typing.List[sector_group.SectorGroup] = []
            #for sector in data.sectors:
            #    sect_group_idx = sector.sector_group_idx 
            ##    if sect_group_idx not in live_sector_group_map_tbl:
            #        live_sector_group_map_tbl[sect_group_idx] = new_sector_group_idx
            #        live_sector_groups.append(data.sector_groups[sect_group_idx])
            #        new_sector_group_idx += 1
            

            num_sectors = len(data.sectors)
            num_sector_groups = len(data.sector_groups)
            #num_sector_groups = len(live_sector_group_map_tbl)

            num_vertexes = len(data.vertexes)
            num_walls = sum(len(sect.walls) for sect in data.sectors)

            f.seek(map_struct_offset)
            # write num_sector_groups, currently the same as num sectors (no groups supported in editor yet)
            write_u16(f, num_sector_groups)
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
            wall_tex_repetitions_ptr_offset = pointer_placeholder(f)
            vertexes_ptr_offset = pointer_placeholder(f)
            collision_vertexes_ptr_offset = pointer_placeholder(f)

            wall_norm_quads_ptr_offset = pointer_placeholder(f)
            # write has_pvs (HARDCODED to false right now)
            write_u16(f, 0)
            # write pvs pointers(HARDCODED to NULL)
            pvs_ptr_offset = write_u32(f, 0)
            raw_pvs_ptr_offset = write_u32(f, 0) 
            
            pvs_sector_offsets_ptr_offset = write_u32(f, 0)
            pvs_sector_list_offsets_ptr_offset = write_u32(f, 0)
            wall_pvs_ptr_offset = write_u32(f, 0)

            pvs_bunch_groups_ptr_offset = write_u32(f, 0)
            pvs_bunch_entries_ptr_offset = write_u32(f, 0)

            sector_pvs_offsets_ptr_offset = write_u32(f, 0)
            sector_pvs_entries_ptr_offset = write_u32(f, 0)

            sector_phs_offsets_ptr_offset = write_u32(f, 0)
            sector_phs_entries_ptr_offset = write_u32(f, 0)

            name_ptr_offset = pointer_placeholder(f)
            music_ptr_offset = pointer_placeholder(f, 0) # define to NULL
            palette_ptr_offset = pointer_placeholder(f, 0) # default to NULL

            write_u16(f, len(cur_state.map_data.things)) # num things
            thing_ptr_offset = pointer_placeholder(f, 0) # things pointer, NULL for now

            patch_pointer_to_current_offset(
                f, sectors_ptr_offset
            )

            
            # write sectors, wall offsets, portal offsets, num walls, and group indexes
            wall_offset = 0
            portal_offset = 0
            for sect in data.sectors:
                sect_num_walls = len(sect.walls)
                f.write(struct.pack(
                    ">hhhh",
                    wall_offset, portal_offset,
                    sect_num_walls, sect.sector_group_idx
                ))
                if len(sect.walls) == 0:
                    continue
                wall_offset += sect_num_walls+1
                portal_offset += sect_num_walls

            patch_pointer_to_current_offset(
                f, sector_group_types_ptr_offset
            )

            # write sector group types for LIVE (aka used) sector groups only
            print("Checking sector group params...")
            for sect_group in data.sector_groups:
                sect_group_key = sect_group.key if hasattr(sect_group, 'key') else 0
                f.write(struct.pack('>B', sect_group.type | (sect_group_key<<5)))    
                if (sect_group.type != sector_group.DOOR and
                    # check neighbor sectors
                    sect_group.type != sector_group.LIFT):
                    continue
                
                if sect_group.type == sector_group.DOOR:
                    assert sect_group.orig_height >= sect_group.ceil_height, "Sector group {}'s ceiling height is higher than it's original (open) height.".format(sect_group.index)
                elif sect_group.type == sector_group.LIFT:
                    assert sect_group.orig_height <= sect_group.floor_height, "Sector group {}'s floor height is lower than it's original (lowered) height".format(sect_group.index)
                else:
                    continue

                for sector in data.sectors:
                    if sector.sector_group_idx != sect_group.index:
                        continue
                    # find neighbors to this sector
                    for wall in sector.walls:
                        if wall.adj_sector_idx == -1:
                            continue
                        adj_sector = data.sectors[wall.adj_sector_idx]
                        adj_sector_group = data.sector_groups[adj_sector.sector_group_idx]
                        if sect_group.type == sector_group.DOOR:
                            assert adj_sector_group.ceil_height <= sect_group.orig_height, "Sector group {}'s ceiling height is higher than neighboring door sector group {}'s original height, this is not supported.".format(adj_sector_group.index, sect_group.index)
                        elif sect_group.type == sector_group.LIFT:
                            assert adj_sector_group.floor_height >= sect_group.orig_height, "Sector group {}'s floor height is lower than neighboring lift sector group {}'s original height, this is not supported.".format(adj_sector_group.index, sect_group.index)

            print("Sector group params valid!")

            #for sect_group in data.sector_groups:
            #    f.write(struct.pack('>B', sect_group.type))
            
            align(f)

            patch_pointer_to_current_offset(
                f, sector_group_params_ptr_offset
            )

            # write sector group params for LIVE/used sector groups only

            for sect_group in data.sector_groups:
                f.write(struct.pack(
                    # light level:3|ceil_color:4|floor_color:4, orig_height, ticks_left, state
                    # floor_height, ceil_height, scratch, scratch
                    ">hhhhhhhh",
                    (sect_group.light_level<<8)|(sect_group.ceil_color<<4)|(sect_group.floor_color), 
                    sect_group.orig_height*16, sect_group.state, sect_group.ticks_left, 
                    sect_group.floor_height*16, sect_group.ceil_height*16,
                    sect_group.scratch_one if hasattr(sect_group, 'scratch_one') else 0, sect_group.scratch_two if hasattr(sect_group, 'scratch_two') else 0
                ))
            
            # write sector group triggers
            patch_pointer_to_current_offset(
                f, sector_group_triggers_ptr_offset
            )
            for sect_group in data.sector_groups: #data.sector_groups:
                cnt = 0
                #f.write(struct.pack(">hhhhhhhh", *[0,0,0,0,0,0,0,0]))
                
                for i in range(len(sect_group.triggers)):
                    write_s16(f, sect_group.triggers[i])
                    cnt += 1
                while cnt < 8:
                    write_s16(f, -1)
                    cnt += 1
                

            patch_pointer_to_current_offset(
                f, walls_ptr_offset
            )

            wall_idx = 0
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
                    wall.output_idx = wall_idx
                    wall_idx += 1
                    prev_v2 = wall.v2 
                write_u16(f, first_v1.index)
                wall_idx += 1

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

            # wall colors/textures
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
                f, wall_tex_repetitions_ptr_offset
            )
            # wall texture repetitions
            for sect in data.sectors:
                for wall in sect.walls:
                    write_u8(f, wall.calc_texture_repetitions())

            align(f)
            patch_pointer_to_current_offset(
                f, vertexes_ptr_offset
            )
            for vert in data.vertexes:
                write_s16(f, int(vert.x * utils.ENGINE_X_SCALE))
                write_s16(f, int(vert.y * utils.ENGINE_Y_SCALE))


            patch_pointer_to_current_offset(
                f, collision_vertexes_ptr_offset
            )

            for sect in data.sectors:
                collision_walls = sect.get_collision_hull(cur_state.map_data, utils.PLAYER_COLLISION_SIZE)
                #for wall in collision_walls:
                    #((cv1x,cv1y),(cv2x,cv2y)) = wall
                    #write_s32(f, int((cv1x * 1024) * utils.ENGINE_X_SCALE)) # 10 fractional bits
                    #write_s32(f, int((cv1y * 1024) * utils.ENGINE_Y_SCALE)) # 10 fractional bits
                    #write_s32(f, int((cv2x * 1024) * utils.ENGINE_X_SCALE)) # 10 fractional bits
                    #write_s32(f, int((cv2y * 1024) * utils.ENGINE_Y_SCALE)) # 10 fractional bits
                for wall in sect.walls:
                    ((cv1x,cv1y),(cv2x,cv2y)) = wall.get_collision_line_verts(utils.PLAYER_COLLISION_SIZE)
                    write_s16(f, int(cv1x * utils.ENGINE_X_SCALE))
                    write_s16(f, int(cv1y * utils.ENGINE_Y_SCALE))
                    write_s16(f, int(cv2x * utils.ENGINE_X_SCALE))
                    write_s16(f, int(cv2y * utils.ENGINE_Y_SCALE))



            patch_pointer_to_current_offset(
                f, wall_norm_quads_ptr_offset
            )
            for sect in data.sectors:
                for wall in sect.walls:
                    write_u8(f, wall.normal_quadrant_int())
            align(f)


            

            sector_pvs_map: typing.Dict[int, typing.List[int]] = {} # sector to list of potentially visible sectors
            sector_pvs_bunches: typing.Dict[int, pvs.BunchList] = {}
            for sector in data.sectors:
                sect_pvs, bunches =  pvs.recursive_pvs(sector, cur_state, data)
                merged_bunches = pvs.merge_bunches(bunches, cur_state.map_data)
                split_bunches = pvs.split_bunches_by_output_idx(merged_bunches)
                sector_pvs_map[sector.index] = sect_pvs.keys()
                sector_pvs_bunches[sector.index] = split_bunches
            sector_phs_map: typing.Dict[int, typing.Set[int]] = {}
            for sector in data.sectors:
                cur_phs = set()
                for nsect in sector_pvs_map[sector.index]:
                    cur_phs = cur_phs.union(set(sector_pvs_map[nsect]))
                sector_phs_map[sector.index] = cur_phs
            
            align(f)

            # write pvs bunch entries 
            patch_pointer_to_current_offset(
                f, pvs_bunch_entries_ptr_offset
            )
            # write bunch entries and update a table to write the bunch groups later
            # 
            sector_pvs_bunch_info: typing.Dict[typing.Tuple[int, int]] = {}

            bunch_entries_added = 0
            for sector in data.sectors: 
                bunches = sector_pvs_bunches[sector.index]
                sector_pvs_bunch_info[sector.index] = (bunch_entries_added, len(bunches))

                for bunch in bunches:                
                    write_u16(f, sector.index)
                    # assert that bunch is contiguous in wall output indexes
                    for i in range(1, len(bunch)):
                        cwall = bunch[i]
                        pwall = bunch[i-1]
                        if cwall.output_idx != pwall.output_idx+1:
                            # need to create a new bunch, sadly
                            assert False, "Wall indexes in a bunch must be contiguous! {} vs {}".format(pwall.output_idx, cwall.output_idx)
                                
                    write_u16(f, bunch[0].output_idx)
                    write_u8(f, len(bunch))

                    #write_u16(f, bunch[0].
                
            align(f)

            patch_pointer_to_current_offset(
                f, pvs_bunch_groups_ptr_offset
            )
            for sector in data.sectors:
                offset, num_bunches = sector_pvs_bunch_info[sector.index]
                write_u16(f, offset)
                write_u8(f, num_bunches)

            align(f)

            def output_pvs(pvs, off):
                # sectors
                run_len = 0 
                # negative run length is zeros
                # posiive run length is sectors
                for sect_idx in range(len(data.sectors)):
                    if sect_idx not in pvs:
                        if run_len <= 0: # increment zeros :)
                            run_len -= 1
                        else:
                            # output sector run
                            write_s8(f, run_len)
                            off += 1
                            run_len = -1
                    else:
                        if run_len >= 0:
                            run_len += 1
                        else:
                            # output zero run
                            write_s8(f, run_len)
                            run_len = 1
                            off += 1

                    if run_len == -128 or run_len == 127:
                        write_s8(f, run_len)
                        run_len = 0
                        off += 1
                    

                if run_len != 0:
                    write_s8(f, run_len)
                    off += 1

                # terminate this sector's with a zero
                write_s8(f, 0)
                off += 1
                return off

            align(f)
            # maps from the sector index to the starting index (in the pvs) of it's 
            # zero-run-length encoded pvs
            patch_pointer_to_current_offset(
                f, sector_pvs_entries_ptr_offset
            )

            off = 0
            sector_pvs_offset_map = {} 
            for sector in data.sectors:
                sect_pvs = sector_pvs_map[sector.index]
                sector_pvs_offset_map[sector.index] = off
                off = output_pvs(sect_pvs, off)


            align(f)
            patch_pointer_to_current_offset(
                f, sector_pvs_offsets_ptr_offset
            )
            for sector in data.sectors:
                write_u32(f, sector_pvs_offset_map[sector.index]) 
            print("Write {} bytes for sector pvs".format(len(data.sectors)*4 + off))


            patch_pointer_to_current_offset(
                f, sector_phs_entries_ptr_offset
            )
            off = 0
            sector_phs_offset_map = {} 
            for sector in data.sectors:
                sector_phs_offset_map[sector.index] = off
                sect_phs = sector_phs_map[sector.index]
                off = output_pvs(sect_phs, off)

            align(f)
            patch_pointer_to_current_offset(
                f, sector_phs_offsets_ptr_offset
            )
            for sector in data.sectors:
                write_u32(f, sector_phs_offset_map[sector.index]) 
            print("Write {} bytes for sector phs".format(len(data.sectors)*4 + off))


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
                for idx, column in enumerate(columns):
                    align(f)
                    if len(column) == 0:
                        span_offsets.append(0)
                        texel_offsets.append(0)
                        continue
                    span_offsets.append(f.tell())
                    for (skip,length,_) in column:
                        write_u16(f, skip)
                        write_u16(f, length)
                    texel_offsets.append(f.tell())
                    for (_,_,texels) in column:
                        for texel in texels:
                            write_u8(f, texel)

                align(f)
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
                sz = 0
                write_u32(f, sprite_to_wad_offset_map[thing_def.sprite_file])
                sz += 4
                write_u16(f, thing_def.anchor_draw_offset*16)
                sz += 2
                write_u16(f, thing_def.width)
                sz += 2
                write_u16(f, thing_def.height*16)
                sz += 2
                write_u8(f, thing_def.init_state)
                sz += 1
                write_u16(f, thing_def.speed)
                sz += 2

                write_u8(f, 0) # not player :^)
                sz += 1

                # type of object 
                # let's go with OBJECT rather than DECORATION
                write_u8(f, 0)
                sz += 1

                # anchor top or bottom
                assert thing_def.anchor_top or thing_def.anchor_bottom, "Thing must be anchored to top or bottom"
                flags = (
                    thing_def.anchor_bottom |
                    (thing_def.anchor_top<<1) |
                    (thing_def.key_type<<2))
                write_u8(f, flags)
                #write_u8(f, thing_def.anchor_bottom)
                sz += 1

                assert len(thing_def.name) < 32, "thing def name is too long!"
                for c in thing_def.name:
                    f.write(str.encode(c))
                write_u8(f, 0) # null terminate this string
                sz += 32
                assert sz == 48, "{} is not the right size lol".format(sz)
                f.seek(struct_start + 48) # skip past this struct 

            f.seek(prev_wad_ptr)
            # write map things
            patch_pointer_to_current_offset(f, thing_ptr_offset)
            for thing in cur_state.map_data.things:
                write_u16(f, thing.sector_num)
                write_s16(f, int(thing.x * utils.ENGINE_X_SCALE))
                write_s16(f, int(thing.y * utils.ENGINE_Y_SCALE))
                write_s16(f, thing.z)
                write_u8(f, thing.object_type)


            print("Wrote {} bytes".format(prev_wad_ptr - map_data_base))

    except Exception as e:
        messagebox.showerror(
            title="Error",
            message=e,
        )
        raise e




    return last_exported_rom_file

def export_map_to_rom_as(cur_path, cur_state, set_launch_flags):
    reset_last_exported_rom_file()
    export_map_to_rom(cur_path, cur_state, set_launch_flags)

def export_and_launch(cur_path, cur_state, set_launch_flags):
    try:
        rom_name = export_map_to_rom(cur_path, cur_state, set_launch_flags=set_launch_flags)
        launch_emulator(cur_state.emulator_path, rom_name)
    except:
        pass

def export_and_launch_as(cur_path, cur_state, set_launch_flags):
    try:
        reset_last_exported_rom_file()
        export_and_launch(cur_path, cur_state, set_launch_flags)
    except:
        pass

def export_and_launch_on_hardware(cur_path, cur_state, set_launch_flags):
    try:
        rom_name = export_map_to_rom(cur_path, cur_state, set_launch_flags=set_launch_flags)
        launch_hardware(cur_state.megalink_path, rom_name)
    except:
        pass


def export_and_launch_as_on_hardware(cur_path, cur_state, set_launch_flags):
    try:
        reset_last_exported_rom_file()
        export_and_launch_on_hardware(cur_path, cur_state, set_launch_flags)
    except:
        pass