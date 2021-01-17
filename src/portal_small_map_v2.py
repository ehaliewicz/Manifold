import itertools
import math

sectors = [
    # sector 0 
    0, 6, -80, 216,
    # sector 1
    6, 6, -80, 216,
    # sector 2
    12, 6, -80, 216,
    # sector 3
    18, 6, -80, 216,
    # sector 4
    24, 6, -80, 216,
    # sector 5
    30, 6, -80, 216,
    # sector 6
    36, 4, -80, 216,
    # sector 7
    40, 4, -80, 216,
    # sector 8
    44, 4, -80, 216,
    # sector 9
    48, 4, -80, 216
]


walls = [
    # sector 0 walls 
    0, 1, 9, 8, 7, 6, 0,
    # sector 1 walls
    1, 2, 12, 11, 10, 9, 1,
    # sector 2 walls
    2, 3, 15, 14, 13, 12, 2,
    # sector 3 walls
    3, 4, 18, 17, 16, 15, 3,
    # sector 4 walls
    4, 5, 21, 20, 19, 18, 4,
    # sector 5 walls
    5, 0, 6, 23, 22, 21, 5,
    # sector 6 walls
    7, 8, 17, 16, 7,
    # sector 7 walls
    25, 13, 14, 26, 25,
    # sector 8 walls
    23, 24, 27, 22, 23,
    # sector 9 walls
    10, 11, 19, 20, 10
]

def VERT(y,x):
    return (x*30),((110-y)*40)

vertexes = [
    VERT(14,46),			      
    VERT(13,90),
    VERT(29,110),
    VERT(46,96),
    VERT(46,48),
    VERT(27,35),
    VERT(18,55),
    VERT(18,67),
    VERT(18,76),
    VERT(18,84),
    VERT(21,87),
    VERT(25,91),
    VERT(30,97),
    VERT(32,95),
    VERT(38,91),
    VERT(41,88),
    VERT(41,76),
    VERT(41,67),
    VERT(41,55),
    VERT(35,52),
    VERT(31,49),
    VERT(27,46),
    VERT(26,47),
    VERT(20,53),
    VERT(20,56),
    VERT(32,92),
    VERT(38,86),
    VERT(26,50),
]

inside_sect_0 = VERT(15,69)
outside_sect_0 = VERT(11,69)

inside_sect_1 = VERT(21,94)

def degrees_to_radians(degs):
    return (degs * math.pi) / 180

def radians_to_degrees(rads):
    return rads*180/math.pi

def degrees_to_binary_degrees(degs):
    return (degs * 1024) // 360

def degrees_to_quadrant(degs):
    return int(degs//90)
        

#   0   0
# 256 360


def iterate_angles():
    # angle
    num_angs = 1024
    for i in range(num_angs): #1024):
        yield (i*360)/num_angs 


def normalize(x, y):
    csqr = (x*x)+(y*y)
    mag = math.sqrt(csqr)
    return (x/mag, y/mag)


def vector_to_degrees(x, y):
    degs = radians_to_degrees(math.atan2(-y, x))
    if degs < 0:
        return 360+degs
    else:
        return degs
    
def degrees_to_vector(degrees):
    rads = degrees_to_radians(degrees)
    x = math.cos(rads)
    y = -math.sin(rads)
    
    #print("{} -> {}".format(degrees, degrees_to_radians(degrees)))
    
    #print("{},{}".format(x, y))
    return normalize(x,y)


def get_normal_from_vertices(v1, v2):
    (x1,y1) = v1
    (x2,y2) = v2
    
    dx = x2-x1
    dy = y2-y1
    
    return normalize(-dy, dx)


def backfacing(ang, v1, v2):
    (wx, wy) = get_normal_from_vertices(v1, v2)
    (ax, ay) = degrees_to_vector(ang)
    

    dot = ((ax * wx) + (ay * wy))
    backfacing = dot > 0
    
    #if ang >= 225:
    #print("angle {}".format(ang))
    #print("- v1 {} v2 {}".format(v1, v2))
    #print("- wall normal {},{}".format(wx, wy))
    #print("- cam vector {},{}".format(ax, ay))
    #print("- dot product {}".format(dot))
    #print("backfacing {}".format(backfacing))
    #exit()
    
    return backfacing

def maybe_visible(ang, v1, v2):
    return (not backfacing(ang, v1, v2))

def line_intersection(line1, line2):
    xdiff = (line1[0][0] - line1[1][0], line2[0][0] - line2[1][0])
    ydiff = (line1[0][1] - line1[1][1], line2[0][1] - line2[1][1])

    def det(a, b):
        return a[0] * b[1] - a[1] * b[0]

    div = det(xdiff, ydiff)
    if div == 0:
       raise Exception('lines do not intersect')

    d = (det(*line1), det(*line2))
    x = det(d, xdiff) / div
    y = det(d, ydiff) / div
    return x, y

def group_into_similar_chunks(similar_test, lst):
    cur_lst = []
    chunks = []
    for item in lst:
        if len(cur_lst) == 0:
            cur_lst.append(item)
        elif similar_test(item, cur_lst[0]):
            cur_lst.append(item)
        else:
            chunks.append(cur_lst)
            cur_lst = [item]

    if len(cur_lst) > 0:
        chunks.append(cur_lst)

    return chunks


def find_outside_bbox(sector):
    (woff,wnum, f, c) = sector
    

    verts = []
    vert_indexes = range(woff, woff+wnum)
    for i,widx in enumerate(vert_indexes):
        
        nwidx = widx+1
        
        vidx = walls[widx]
        nvidx = walls[nwidx]
        
        v1 = vertexes[vidx]
        v2 = vertexes[nvidx]
        
        verts.append(v1)
        verts.append(v2)

    xs = [x for (x,y) in verts]
    ys = [y for (x,y) in verts]

    min_x = min(xs)
    max_x = max(xs)
    min_y = min(ys)
    max_y = max(ys)
    return ((min_x,min_y), (max_x, max_y))


def is_valid_candidate(candidate):
    ((min_x,min_y),(max_x,max_y)) = candidate
    return min_x < max_x and min_y < max_y


def vxs(x1,y1, x2,y2):
    return ((x1)*(y2) - (x2)*(y1))
     

def point_side(px,py, v1x,v1y, v2x,v2y):
    return vxs((v2x)-(v1x), (v2y)-(v1y), (px)-(v1x), (py)-(v1y))

def on_left(x,y,v1,v2):
    (v1x,v1y) = v1
    (v2x,v2y) = v2
    return point_side(x,y, v1x,v1y, v2x,v2y) < 0 


def is_outside(x,y,sector):
    
    (woff, wnum, f, c) = sector
    vert_indexes = range(woff, woff+wnum)
    
    for i,widx in enumerate(vert_indexes):
        
        nwidx = widx+1
        
        vidx = walls[widx]
        nvidx = walls[nwidx]
        
        v1 = vertexes[vidx]
        v2 = vertexes[nvidx]

        if not on_left(x,y,v1,v2):
            return True

    return False


    
def main():
    sectors2 = []
    for sidx in range(0, len(sectors), 4):
        sectors2.append([sectors[i] for i in range(sidx, sidx+4)])


    
    inside_0_x,inside_0_y = inside_sect_0
    outside_0_x,outside_0_y = outside_sect_0

    inside_1_x,inside_1_y = inside_sect_1
    
    print(is_outside(inside_0_x,inside_0_y, sectors2[0]))
    print(is_outside(outside_0_x,outside_0_y, sectors2[0]))
    print(is_outside(inside_1_x,  inside_1_y,  sectors2[0]))
    
    print(is_outside(inside_0_x,  inside_0_y,  sectors2[1]))
    print(is_outside(outside_0_x, outside_0_y, sectors2[1]))
    print(is_outside(inside_1_x,  inside_1_y,  sectors2[1]))
    
    
    exit()
    
        
    wall_angle_ranges = []
    print("vis_range angle_ranges[NUM_WALLS] = {")
    
    for (woff, wnum, f, c) in sectors2:
        vert_indexes = range(woff, woff+wnum)
        for i,widx in enumerate(vert_indexes):

            nwidx = widx+1

            vidx = walls[widx]
            nvidx = walls[nwidx]
                        
            v1 = vertexes[vidx]
            v2 = vertexes[nvidx]
            
            
            nx,ny = get_normal_from_vertices(v1, v2)
            norm_angle = vector_to_degrees(nx,ny)
            
            print(degrees_to_quadrant(norm_angle))
            continue
        
            tbl = [(ang, maybe_visible(ang, v1, v2)) for ang in iterate_angles()]
            
            def similar_test(a, b):
                (aang, avis) = a
                (bang, bvis) = b
                return avis == bvis

            chunked = group_into_similar_chunks(similar_test, tbl)
            backfacing_chunks = filter(lambda chk: chk[0][1], chunked)
            mapped = list(map(lambda chk: (degrees_to_binary_degrees(chk[0][0]),
                                           degrees_to_binary_degrees(chk[-1][0])), backfacing_chunks))
            
            
            
            
            formatted = ','.join(map(lambda chk: "{},{}".format(int(chk[0]), int(chk[1])), mapped))
            
            #wall_angle_ranges.append(mapped)

            assert (len(mapped) in [1,2])
            if len(mapped) == 2:
                print("{.two_ranges = 1, .angles = { " + str(formatted) + " } },")
            else:
                print("{.two_ranges = 0, .angles = { " + str(formatted) + " } }, ")
            #return wall_angle_ranges

    print("};")

if __name__ == '__main__':

    
    main()
    

    
    #  256 =  90
    #  512 = 180
    #  768 = 270
    # 1024 = 360
