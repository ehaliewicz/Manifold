import imgui
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
