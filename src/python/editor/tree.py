import imgui
import numpy as np
import sector

def split_concave_sector(sect, cur_state):
    for wall in sect.walls:
        has_adj_wall,adj_wall = wall.find_adjacent_wall(cur_state.map_data)
        if has_adj_wall:
            print("Portal to this sector in sector {} might need to be split".format(adj_wall.sector_idx))
                
        if wall.adj_sector_idx != -1:
            print("Portal to sector {} might need to be split".format(wall.adj_sector_idx))

        # for each vertex in the sector
        # try to draw from that vertex to a new vertex, creating two partial sectors
        # check if both are convex
        # if they are, we're good.

        # if one is still concave, recursively split the extra concave sector
        # if 

        sector_verts = sect.get_vertexes()
        num_verts = len(sector_verts)
        floor_height = sect.floor_height
        ceil_height = sect.ceil_height
        
        for i1 in range(1, num_verts):
            for i2 in range(num_verts):
                if i1 == i2:
                    continue
                if abs(i2-i1) == 1:
                    continue

                v1 = sector_verts[i1]
                v2 = sector_verts[i2]

                fst_sector = sector.Sector(index = sect.index+1, floor_height=floor_height, ceil_height=ceil_height)
                snd_sector = sector.Sector(index = sect.index+2, floor_height=floor_height, ceil_height=ceil_height)
                # how do we split the sector with these vertexes?
                
                # first sector starts at i2 and goes up to i1, then reconnects to i2 
                print("first sector from vert {} to vert {}".format(i2, i1))
                
                # second sector starts at i1 and goes up to i2, then reconnects to i1
                print("second sector from vert {} to vert {}".format(i1, i2))
                
                
                fst_sector.add_wall()
                
def split_concave_sectors(cur_state):
    for sect in cur_state.map_data.sectors:
        if sect.is_convex():
            continue

        split_concave_sector(sect, cur_state)
        
        print("Sector {} is concave, splitting".format(sect.index))
            
                
    # update cur_sector,wall,vertex?
    
        
def draw_tree_mode(cur_state):
    disabled = all([s.is_convex() for s in cur_state.map_data.sectors])

    if not disabled:
        if imgui.button("Split concave sectors"):
            split_concave_sectors(cur_state)

            # any sectors connected to this sector

def determine_wall_order(w1, w2, map_data):
    pass

def calc_pvs(cur_state):
    map_data = cur_state.map_data
    all_walls = []
    for sect in map_data.sectors:
        all_walls += sect.walls

    for sect in map_data.sector:
        sect_walls = sect.walls

        wall_draw_order_table = {}
        # for each pair of two walls, store the order in which they should be drawn
        # if they occlude each other, and the first order doesn't match a later order, report an error
        min_x = int(min([min(w.v1.x, w.v2.x) for w in sect_walls]))
        max_x = int(max([max(w.v1.x, w.v2.x) for w in sect_walls]))
        min_y = int(min([min(w.v1.y, w.v2.y) for w in sect_walls]))
        max_y = int(max([max(w.v1.y, w.v2.y) for w in sect_walls]))
        for x in range(min_x, max_x+1):
            for y in range(min_y, max_y+1):
                if not inside_sector(x, y, sect, map_data):
                    continue

                for w1 in all_walls:
                    for w2 in all_walls:
                        if w1 == w2:
                            continue
                        wall_set = frozenset((w1, w2))    
                        draw_order, occlusion = determine_wall_order(w1, w2, map_data)
                        if wall_set in wall_draw_order_table:
                            (prev_draw_order, prev_occlusion) = wall_draw_order_table[wall_set]
                            if occlusion and prev_occlusion:
                                if draw_order != prev_draw_order:
                                    assert False, "sorted pvs construction error!"
                            elif occlusion:
                                wall_draw_order_table[wall_set] = (draw_order, True)
                            elif prev_occlusion:
                                # do nothing
                                pass
                            else:
                                pass
                        else:
                            wall_draw_order_table[wall_set] = (draw_order, occlusion)
                                #wall_draw_order_table[wall_set] = (draw_order, False)
                

