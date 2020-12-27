import skgeom
from skgeom import Point2, Ray2, Segment2
from shapely.geometry import LineString, Point, Polygon
from typing import List, NewType, Optional, Set, Tuple
import bsp, wad
import functools

Portal = NewType('Portal', Tuple[Point, Point, int])

FrustumOrigin = NewType('FrustumOrigin', Point)



FrustumLeftLine = NewType('FrustumLeftLine', Segment2)
FrustumRightLine = NewType('FrustumRightLine', Segment2)
FrustumPolygon = NewType('FrustumPolygon', Polygon)


PortalFrustum  = NewType('PortalFrustum', Tuple[Portal,
                                                Optional[FrustumOrigin],
                                                Optional[FrustumLeftLine],
                                                Optional[FrustumRightLine],
                                                Optional[FrustumPolygon]])



def vertexToPoint2(vert):
    return Point2(vert.x, vert.y)

def point2ToPoint(pt):
    return Point([pt.x(), pt.y()])

def make_empty_portal_frustum_from_portal(portal):
    return (portal, None, None, None, None)

def are_portals_coplanar(new_portal: Portal, portal_frustum: PortalFrustum):
    (old_portal, _, _, _, _)    = portal_frustum
    (old_pt1, old_pt2, _) = old_portal
    (new_pt1, new_pt2, _) = new_portal

    
    old_seg = skgeom.Segment2(old_pt1, old_pt2)
    
    old_line = old_seg.supporting_line()
    

    return old_line.has_on(new_pt1) and old_line.has_on(new_pt2)

def clip_frustum_against_level_bounds(left_side, right_side, bound_lines):

    INSIDE = 0
    OUTSIDE = 1
    
    cur_lines = [left_side, right_side]

    state = INSIDE
    
    for line in level_bound_lines:
        
        intersection = skgeom.intersection(left_ray, line)
        
            
        print(intersection)
        
    

def generate_frustum_between_portals(last_portal_frustum, new_portal, level_bounds):
    (last_portal, _, _, _, _)     = last_portal_frustum
    (last_pt1, last_pt2, _) = last_portal

    (min_x, max_x, min_y, max_y) = level_bounds
    bounds_width = max_x - min_x
    bounds_height = max_y - min_y

    max_bounds_dim = max(bounds_width, bounds_height)
    
    
    (new_pt1, new_pt2, _) = new_portal
    
    l1 = Segment2(last_pt1, new_pt2)
    l2 = Segment2(last_pt2, new_pt1)

    l1_vec = l1.to_vector()
    l2_vec = l2.to_vector()
    
    
    intersection = skgeom.intersection(l1, l2)
    #print("Got intersection: ", intersection)

    if not isinstance(intersection, Point2):
        return None
                
              
    assert isinstance(intersection, Point2)

    
    scale = skgeom.Transformation2(skgeom.SCALING, max_bounds_dim*2)
    scaled_l1_vec = l1_vec.transform(scale)
    scaled_l2_vec = l2_vec.transform(scale)

    scaled_l1_seg = Segment2(intersection, intersection + scaled_l1_vec)
    scaled_l2_seg = Segment2(intersection, intersection + scaled_l2_vec)

    poly = Polygon([point2ToPoint(intersection),
                    point2ToPoint(scaled_l1_seg.point(1)),
                    point2ToPoint(scaled_l2_seg.point(1))])

    bounds_poly = Polygon([Point([min_x, min_y]),
                           Point([max_x, min_y]),
                           Point([max_x, max_y]),
                           Point([min_x, max_y])])

    
    #clipped_poly = poly.intersection(bounds_poly)

    #assert isinstance(clipped_poly, Polygon)
    
    return (new_portal,
            FrustumOrigin(intersection),
            FrustumLeftLine(scaled_l1_seg),
            FrustumRightLine(scaled_l2_seg),
            FrustumPolygon(poly)) #clipped_poly))
    

def are_frustums_overlapping(frustum_1, frustum_2):
    (_, _, _, _, frustum_1_poly) = frustum_1
    (_, _, _, _, frustum_2_poly) = frustum_2

    if frustum_2_poly is None:
        return True

    
    
    intersects = frustum_1_poly.intersects(frustum_2_poly)
    touches = frustum_1_poly.touches(frustum_2_poly)

    return intersects #and (not touches)


    
    
    
    
    

def get_segs_on_other_side_of_seg(seg, linedef_idx_to_seg_idxs_map, seg_idx_to_ssect_idx_map, level_data):
    linedef_idx = seg.linedef
    linedef = level_data['LINEDEFS'][linedef_idx]


    vertexes = level_data['VERTEXES']
    
    src_direction = seg.direction
    seg_idxs_on_linedef = linedef_idx_to_seg_idxs_map[linedef_idx]

    seg_v1 = vertexes[seg.begin_vert]
    seg_v2 = vertexes[seg.end_vert]
    
    

    res = []

    all_segs = level_data['SEGS']

    other_side_seg_idxs = [si for si in seg_idxs_on_linedef if all_segs[si].direction != src_direction]
    other_side_segs = [(si,all_segs[si]) for si in other_side_seg_idxs]
    
    sorted_other_side_segs = sorted(other_side_segs, key=lambda s: s[1].begin_vert, reverse=True)

    idx_to_point2 = {}
    cnt_to_idx = {}
    cnt = 0

    idx_to_point2[seg.begin_vert] = Point2(cnt, 0)
    cnt_to_idx[cnt] = seg.begin_vert
    cnt += 1
    
    
    for (si,other_seg) in sorted_other_side_segs:
        ov1_idx = other_seg.begin_vert
        ov2_idx = other_seg.end_vert
        if ov2_idx not in idx_to_point2:
            idx_to_point2[ov2_idx] = Point2(cnt, 0)
            cnt_to_idx[cnt] = ov2_idx
            cnt += 1
            
        if ov1_idx not in idx_to_point2:
            idx_to_point2[ov1_idx] = Point2(cnt, 0)
            cnt_to_idx[cnt] = ov1_idx
            cnt += 1

    if seg.end_vert not in idx_to_point2:
        idx_to_point2[seg.end_vert] = Point2(cnt, 0)
        cnt_to_idx[cnt] = seg.end_vert
        cnt += 1
        
    #print(idx_to_point2)
    
    
    #src_seg2 = Segment2(vertexToPoint2(seg_v1), vertexToPoint2(seg_v2))
    src_seg2 = Segment2(idx_to_point2[seg.begin_vert], idx_to_point2[seg.end_vert])
    #src_seg = LineString([idx_to_point2[seg.begin_vert], idx_to_point2[seg.end_vert]])
    
    for (other_seg_idx, other_seg) in other_side_segs:
        #other_seg = level_data['SEGS'][other_seg_idx]
        #if other_seg.direction == src_direction:
        #    continue

        #other_v1 = vertexes[other_seg.begin_vert]
        #other_v2 = vertexes[other_seg.end_vert]

        other_seg2 = Segment2(idx_to_point2[other_seg.begin_vert], idx_to_point2[other_seg.end_vert])
        #other_seg = LineString([idx_to_point2[other_seg.begin_vert], idx_to_point2[other_seg.end_vert]])
        
        intersection = skgeom.intersection(src_seg2, other_seg2)
        #intersection = src_seg.intersection(other_seg)

        #print("Got intersection {}".format(intersection))
        
        if isinstance(intersection, Segment2): 
        #if isinstance(intersection, LineString) and len(intersection.coords) > 0:
            
            tgt_ssect_idx = seg_idx_to_ssect_idx_map[other_seg_idx]
            cnts = [intersection.point(0).x(), intersection.point(1).x()]
            sorted_cnts = sorted(cnts)
            cnt1 = sorted_cnts[0]
            cnt2 = sorted_cnts[1]
            idx1 = cnt_to_idx[float(cnt1)]
            idx2 = cnt_to_idx[float(cnt2)]
                        
            p1 = vertexToPoint2(vertexes[idx1])
            p2 = vertexToPoint2(vertexes[idx2])
            res.append((p1, p2, tgt_ssect_idx))

    return res
        
    
    

def gather_ssect_portal_data(ssect, level_data, linedef_idx_to_seg_idxs_map, seg_idx_to_ssect_idx_map) -> List[Portal]:
    fst_seg = ssect.first_seg
    num_segs = ssect.num_segs
    #ssect_segs = level_data['SEGS'][fst_seg:fst_seg+num_segs]

    res = []

    for seg_idx in range(fst_seg, fst_seg+num_segs):
        seg = level_data['SEGS'][seg_idx]
        linedef_idx = seg.linedef
        linedef = level_data['LINEDEFS'][linedef_idx]
        if wad.linedef_is_portal(linedef):
            v1_idx = seg.begin_vert
            v2_idx = seg.end_vert
            v1 = level_data['VERTEXES'][v1_idx]
            v2 = level_data['VERTEXES'][v2_idx]

            
            # get all segs on this linedef
            #possible_seg_idxs = range(len(level_data['SEGS']))
            possible_seg_idxs = linedef_idx_to_seg_idxs_map[linedef_idx]

            matching_segs = get_segs_on_other_side_of_seg(seg,
                                                          linedef_idx_to_seg_idxs_map,
                                                          seg_idx_to_ssect_idx_map,
                                                          level_data)

            
            if len(matching_segs) == 0:
                print("On linedef: {}".format(linedef_idx))
                print("Couldn't find segs on other side of seg {}:{}".format(seg_idx, level_data['SEGS'][seg_idx]))
                print("Has segs: {}".format([level_data['SEGS'][si] for si in possible_seg_idxs]))
                
                raise Exception("Couldn't find any segs?!?")
            #print("Got {} segs on other side of seg!".format(len(matching_segs)))
            #print(matching_segs)
            res.extend(matching_segs)
            
                  

    return res
            

def gather_all_ssect_portal_data(level_data) -> List[List[Portal]]:
    
    linedef_idx_to_seg_idxs_map = {}
    for seg_idx,seg in enumerate(level_data['SEGS']):
        line_num = seg.linedef
        st = linedef_idx_to_seg_idxs_map.get(line_num, set())
        st.add(seg_idx)
        linedef_idx_to_seg_idxs_map[line_num] = st

    seg_idx_to_ssect_idx_map = {}
    for ssect_idx,ssect in enumerate(level_data['SSECTORS']):
        fst_seg = ssect.first_seg
        num_segs = ssect.num_segs
        for seg_idx in range(fst_seg, fst_seg+num_segs):
            seg_idx_to_ssect_idx_map[seg_idx] = ssect_idx
    
        
    
    return [gather_ssect_portal_data(ssect, level_data,
                                     linedef_idx_to_seg_idxs_map,
                                     seg_idx_to_ssect_idx_map) for ssect in level_data['SSECTORS']]



def recursive_pvs(src_ssect_num, ssect_portal_data, level_bounds, level_data):
    
    def recur(cur_ssect_num: int,
              portal_frustum_trail: List[PortalFrustum]) -> None:
        # get walls for subsector

        nsegs = level_data['SSECTORS'][cur_ssect_num].num_segs
        portals = ssect_portal_data[cur_ssect_num]

        """
        print("Found {} portals in subsector {} with {} segs".format(
            len(portals), cur_ssect_num, nsegs            
        ))
        """

        last_portal_frustum = portal_frustum_trail[-1]
        
        for portal in portals:

            (v1, v2, portal_tgt) = portal

            if portal_tgt in pvs:
                #print("found portal to sector {} already in pvs, skipping".format(portal_tgt))
                continue

            
            is_coplanar_with_new_portal = functools.partial(are_portals_coplanar, portal)
            any_coplanar_portals = any((is_coplanar_with_new_portal(old_portal) for old_portal in portal_frustum_trail))

            
            if any_coplanar_portals:
                #print("found portal coplanar to new portal, skipping")
                continue
            
            #print("generating path from sector {} to {}".format(cur_ssect_num, portal_tgt))
            
            
            portal_frustum = generate_frustum_between_portals(last_portal_frustum, portal, level_bounds)
            if portal_frustum is None:
                #print("No valid frustum found")
                continue
            
            
            is_frustum_overlapping = functools.partial(are_frustums_overlapping, portal_frustum)
            within_all_prev_frustums = all((is_frustum_overlapping(old_portal_frustum) for old_portal_frustum in portal_frustum_trail))


            if not within_all_prev_frustums:
                #print("new portal isn't within all previous frustums, skipping")
                continue

            #new_frustums = cur_frustums + frustum
            new_portal_frustums = portal_frustum_trail + [portal_frustum] 
            pvs.add(cur_ssect_num)
            recur(portal_tgt, portal_frustum_trail)
                

            
    pvs = {src_ssect_num}
        
    start_portals = ssect_portal_data[src_ssect_num]
    #print("found {} portals for ssect {}".format(len(start_portals), src_ssect_num))
    
    for portal in start_portals:
        portal_frustum_trail = [make_empty_portal_frustum_from_portal(portal)]
        cur_frustums = []
        (v1,v2,portal_tgt) = portal
        pvs.add(portal_tgt)
        recur(portal_tgt, portal_frustum_trail)

    return pvs
        
            
def calc_pvs(draw: bool, level_data):
    
    ssect_portal_data = gather_all_ssect_portal_data(level_data)
    #print(ssect_portal_data)

    level_bounds = bsp.get_bsp_tree_bounds(level_data)
    #print("level bounds {}".format(level_bounds))

    global_pvs = {}

    for i in range(len(level_data['SSECTORS'])):
        global_pvs[i] = recursive_pvs(i, ssect_portal_data, level_bounds, level_data)

    return global_pvs
    
    
    #return pvs
