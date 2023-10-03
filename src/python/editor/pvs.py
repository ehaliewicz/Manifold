import math
import typing
import shapely.geometry
from typing import List, NewType, Optional, Set, Tuple
import utils
import imgui
import line
import sector


#Portal = NewType('Portal', Tuple[Point, Point, int])

#FrustumOrigin = NewType('FrustumOrigin', Point)



#FrustumLeftLine = NewType('FrustumLeftLine', Segment2)
#FrustumRightLine = NewType('FrustumRightLine', Segment2)
#FrustumPolygon = NewType('FrustumPolygon', Polygon)


#PortalFrustum  = NewType('PortalFrustum', Tuple[Portal,
#                                                Optional[FrustumOrigin],
#                                                Optional[FrustumLeftLine],
#                                                Optional[FrustumRightLine],
#                                                Optional[FrustumPolygon]])


def length(vec):
    (x,y) = vec
    return math.sqrt(x*x + y*y)

def normalize(vec):
    x,y = vec
    lgth = length(vec)
    return x/lgth, y/lgth

def dot_product(v1, v2):
    res = (v1[0]*v2[0])
    res += (v1[1]*v2[1])
    return res

def wall_to_linestring(wall):
    return shapely.LineString([(wall.v1.x, wall.v1.y), (wall.v2.x, wall.v2.y)])

def within_frustum(wall, frustum, debug=False):
    if debug:
        pass
    wall_ls = wall_to_linestring(wall)
    intersection = shapely.intersection(wall_ls, frustum)
    if intersection.is_empty or intersection.geom_type == "Point" or intersection.length < 1:
        return False 
        
    return True
    #return not (intersection.is_empty)


def draw_frustum(draw_list, cur_state, frustum):
    try:
        xx,yy = frustum.exterior.coords.xy 
    except Exception as e:
        print(e)
        raise e
    x = xx.tolist()
    y = yy.tolist()
    
    cam_x = cur_state.camera_x
    cam_y = cur_state.camera_y
    for i in range(len(x)):
        ni = i+1 if i+1 < len(x) else 0 
        v1x,v1y = (x[i],y[i])
        v2x,v2y = (x[ni],y[ni])

        draw_list.add_line(v1x-cam_x, v1y-cam_y, v2x-cam_x, v2y-cam_y, imgui.get_color_u32_rgba(0,0,1,1), 2.0)

def sign(x):
    if x == 0:
        return 0
    if x > 0:
        return 1
    return -1

draw_frustums = False

def create_new_frustum(draw_list, cur_state, cur_frustum, start_frustum_portal, portal):
    pv1x, pv1y = (portal.v1.x, portal.v1.y)
    pv2x, pv2y = (portal.v2.x, portal.v2.y)
    sfv1x, sfv1y = (start_frustum_portal.v1.x, start_frustum_portal.v1.y)
    sfv2x, sfv2y = (start_frustum_portal.v2.x, start_frustum_portal.v2.y)
    sfv1 = (sfv1x, sfv1y)
    sfv2 = (sfv2x, sfv2y)

    right_dx = pv1x - sfv2x
    right_dy = pv1y - sfv2y
    right_dy_sign = sign(right_dy)

    if right_dx == 0:
        scaled_pv1 = pv1x, pv1y + right_dy_sign*32768
    else:
        right_slope = right_dy/right_dx
        # make sure to scale slope by right_dx, to get the sign right
        scaled_pv1 = (pv1x + 32768*(sign(right_dx))), (pv1y + (sign(right_dx) * right_slope * 32768))
    right_line = shapely.LineString((sfv2, scaled_pv1))

    left_dx = (pv2x - sfv1x)
    left_dy = pv2y - sfv1y
    left_dy_sign = sign(left_dy)

    if left_dx == 0:
        scaled_pv2 = pv2x, (pv2y + left_dy_sign*32768)
    else:
        left_slope = left_dy/left_dx
        # make sure to scale slope by left_dx, to get the sign right
        scaled_pv2 = (pv2x + 32768*(sign(left_dx))), (pv2y + (sign(left_dx) * left_slope * 32768))
    left_line = shapely.LineString((sfv1, scaled_pv2))

    if pv1x == sfv1x and pv1y == sfv1y:
        intersection = shapely.Point((pv1x, pv1y))
    elif pv2x == sfv2x and pv2y == sfv2y:
        intersection = shapely.Point((pv2x, pv2y))
    else:

        cam_x = cur_state.camera_x
        cam_y = cur_state.camera_y
        #col = utils.random_bright_color()
        
        #draw_list.add_line(sfv1x-cam_x, sfv1y-cam_y, pv2x-cam_x, pv2y-cam_y, imgui.get_color_u32_rgba(0,1,0,1), 2.0)
        #draw_list.add_line(sfv1x-cam_x, sfv1y-cam_y, scaled_pv2[0]-cam_x, scaled_pv2[1]-cam_y, imgui.get_color_u32_rgba(0,1,0,0.5), 2.0)

        #draw_list.add_line(sfv2x-cam_x, sfv2y-cam_y, pv1x-cam_x, pv1y-cam_y, imgui.get_color_u32_rgba(1,0,0,1), 2.0)
        #draw_list.add_line(sfv2x-cam_x, sfv2y-cam_y, scaled_pv1[0]-cam_x, scaled_pv1[1]-cam_y, imgui.get_color_u32_rgba(1,0,0,0.5), 2.0)

        intersection = shapely.intersection(left_line, right_line)
        if intersection.geom_type != 'Point':
            return None #"Failed creating frustum between {} and {}".format(start_frustum_portal, portal)

    

    new_frustum = shapely.Polygon((intersection, scaled_pv1, scaled_pv2))

    if cur_frustum is None:
        return new_frustum
    else:
        #return cur_frustum + [new_frustum]
        result_frustum = shapely.intersection(cur_frustum, new_frustum)
        if result_frustum.is_empty:
            return None
        
        intersection_with_portal = shapely.intersection(result_frustum, wall_to_linestring(portal))
        if (intersection_with_portal.geom_type == 'Point' or (
            intersection_with_portal.geom_type == 'LineString' and intersection_with_portal.length < 1)):
            return None

        return result_frustum
        #return new_frustum

def can_merge_front(b1: line.Wall, b2: line.Wall):
    last_of_b1 = b1[-1]
    first_of_b2 = b2[0]
    if last_of_b1.sector_idx != first_of_b2.sector_idx:
        return False 
    
    return last_of_b1.v2 == first_of_b2.v1

def can_merge_back(b1, b2):
    last_of_b2 = b2[-1]
    first_of_b1 = b1[0]
    
    if last_of_b2.sector_idx != first_of_b1.sector_idx:
        return False 
    
    return last_of_b2.v2 == first_of_b1.v1

def sort_bunch(bunch, map_data):
    bunch_list = []
    sector = map_data.sectors[bunch[0].sector_idx]

    return sorted(bunch, key=lambda wall: sector.walls.index(wall))

def merge_bunches(bunches, map_data):

    merged_at_least_one = True 
    bunches = sorted(bunches, key=lambda b: b[0].sector_idx)

    while merged_at_least_one: 
        merged_at_least_one = False
        idx = 0
        while idx < len(bunches):
            candidate_bunch = bunches[idx]
            other_bunches = bunches[0:idx] + bunches[idx+1:]

            # check if it can be merged with either the front or end of the bunch we're testing against

            for test_idx in range(len(other_bunches)):
                test_bunch = other_bunches[test_idx]

                if can_merge_front(candidate_bunch, test_bunch):
                    merged_at_least_one = True 
                    
                    other_bunches[test_idx] = candidate_bunch + test_bunch
                    bunches = other_bunches
                    break

                if can_merge_back(candidate_bunch, test_bunch):
                    merged_at_least_one = True 
                    other_bunches[test_idx] =  test_bunch + candidate_bunch
                    bunches = other_bunches
                    break


            idx += 1

    for idx in range(len(bunches)):
        bunches[idx] = sort_bunch(bunches[idx], map_data)

    return bunches

def split_bunches_by_output_idx(bunches):
    res_bunches = []
    for bunch in bunches:
        cur_bunch = [bunch[0]]
        for i in range(1, len(bunch)):
            cwall = bunch[i]
            pwall = bunch[i-1]
            if cwall.output_idx != pwall.output_idx+1:
                res_bunches.append(cur_bunch)
                cur_bunch = []
            cur_bunch.append(cwall)

        if len(cur_bunch) > 0:
            res_bunches.append(cur_bunch)
    return res_bunches


Bunch = typing.List[line.Wall]
BunchList = typing.List[Bunch]

PVS = typing.Set[line.Wall]

def recursive_pvs(cur_sector, cur_state, map_data) -> typing.Tuple[typing.Dict[int, PVS], BunchList]:
    
    pvs = {}
    bunches: BunchList = []
    cur_bunch: Bunch = []

    def output_bunch():
        nonlocal cur_bunch
        if len(cur_bunch) > 0:
            bunches.append(cur_bunch)
            cur_bunch = []

    def add_wall_to_cur_bunch(w: line.Wall):
        nonlocal cur_bunch
        cur_bunch.append(w)


    pvs[cur_sector.index] = set()
    cur_sect_pvs = pvs[cur_sector.index]

    draw_list = utils.get_root_window_draw_list()
    def recursive_pvs_inner(cur_sector: sector.Sector, cur_frustum, 
                            start_frustum_portal, depth, last_traversed_portal):
        if depth >= 100:
            return 

        #group = []
        next_calls = []


        if cur_sector.index not in pvs:
            pvs[cur_sector.index] = set()
        cur_sect_pvs = pvs[cur_sector.index]

        for idx, wall in enumerate(cur_sector.walls):
            
            portal_sector = wall.adj_sector_idx
            # don't go back through portals we just went through

            if wall.v1 == last_traversed_portal.v2 and wall.v2 == last_traversed_portal.v1:
                # don't traverse back through portals
                output_bunch()
                continue
            #if portal_sector == last_sector_idx:
            #    continue

            
            #if cur_sector.index == 3: # and idx == 1:
            #    pass
            if cur_frustum is not None:
                if not within_frustum(wall, cur_frustum, debug=cur_sector.index==5):
                    output_bunch()
                    continue
            
            dp = dot_product(normalize(wall.normal()), normalize(start_frustum_portal.portal_out_normal()))
            if dp == 1:  # TODO: test if this should be >0 or >=0
                #print("wall {} is backfacing".format(idx))
                output_bunch()
                continue # this wall isn't visible through the start_frustum_portal

            add_wall_to_cur_bunch(wall)
            cur_sect_pvs.add(wall)
    
            if portal_sector == -1:
                # line from v1 of start frustum to v2 of new frustum, and vice versa
                output_bunch()
                continue


            # if there is no frustum, it means we're in a sector directly attached to the root sector
            # so we don't test against the dot product in those cases

            next_frustum = create_new_frustum(draw_list, cur_state, cur_frustum, start_frustum_portal, wall)

            #continue
            if next_frustum is None:
                continue
            #if wall.adj_sector_idx == 5:
            #if next_frustum.is_empty:
            #    print("no next frustum, bailing out")
            #    continue

            nsect = map_data.sectors[portal_sector]
            next_calls.append([nsect, next_frustum, wall])
            
        output_bunch()

        for next_call in next_calls:
            (nsect, nfrust, last_traversed_portal) = next_call         
            if draw_frustums:  
                draw_frustum(draw_list, cur_state, nfrust)
            recursive_pvs_inner(nsect, nfrust, start_frustum_portal, depth+1, last_traversed_portal=last_traversed_portal)


    for wall in cur_sector.walls:
        add_wall_to_cur_bunch(wall)
        cur_sect_pvs.add(wall)

        portal_sector = wall.adj_sector_idx
        if portal_sector == -1:
            continue

        start_frustum_portal = wall
        # we can see all walls in directly attached sectors
        next_sect = map_data.sectors[portal_sector]
        output_bunch()
        recursive_pvs_inner(next_sect, None, start_frustum_portal, depth=1, last_traversed_portal= wall)


    if len(cur_bunch) > 0:
        output_bunch()

    return pvs, bunches



def draw_pvs_mode(cur_state):
    global draw_frustums
        
    def set_cur_sector(idx):
        cur_state.cur_sector = cur_state.map_data.sectors[idx]

    if imgui.button("frustums## Toggle Frustums"):
        draw_frustums = not draw_frustums



    if cur_state.cur_sector is not None:
        pvs,bunches = recursive_pvs(cur_state.cur_sector, cur_state, cur_state.map_data)
        cur_state.cur_sector_pvs = pvs 

        cur_state.cur_sector_pvs_bunches = bunches
        cur_state.cur_sector_pvs_merged_bunches = merge_bunches(bunches, cur_state.map_data)
        imgui.text("Sector {}".format(cur_state.cur_sector.index))
        imgui.text("Num bunches: {}".format(len(cur_state.cur_sector_pvs_bunches)))
        imgui.text("Num merged bunches: {}".format(len(cur_state.cur_sector_pvs_merged_bunches)))
        imgui.text("Num walls: {}".format(sum(len(b) for b in cur_state.cur_sector_pvs_bunches)))

    

    utils.draw_list(cur_state, "Sectors", "Sector list", 
              cur_state.map_data.sectors, 
              set_cur_sector, lambda idx: None)