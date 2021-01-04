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
    0, 1, 9, 8, 7, 6,
    # sector 1 walls
    1, 2, 12, 11, 10, 9,
    # sector 2 walls
    2, 3, 15, 14, 13, 12,
    # sector 3 walls
    3, 4, 18, 17, 16, 15,
    # sector 4 walls
    4, 5, 21, 20, 19, 18,
    # sector 5 walls
    5, 0, 6, 23, 22, 21,
    # sector 6 walls
    7, 8, 17, 16,
    # sector 7 walls
    25, 13, 14, 26,
    # sector 8 walls
    23, 24, 27, 22,
    # sector 9 walls
    10, 11, 19, 20
]


vertexes = [
    (14,46),			      
    (13,90),
    (29,110),
    (46,96),
    (46,48),
    (27,35),
    (18,55),
    (18,67),
    (18,76),
    (18,84),
    (21,87),
    (25,91),
    (30,97),
    (32,95),
    (38,91),
    (41,88),
    (41,76),
    (41,67),
    (41,55),
    (35,52),
    (31,49),
    (27,46),
    (26,47),
    (20,53),
    (20,56),
    (32,92),
    (38,86),
    (26,50)
]


def degrees_to_radians(degs):
    return (degs * math.pi) / 180

def radians_to_degrees(rads):
    return rads*180/math.pi
    

#   0   0
# 256 360


def iterate_angles():
    # angle
    num_angs = 32
    for i in range(num_angs): #1024):
        yield degrees_to_radians((i*360)/num_angs) 


def radians_to_vector(rad):
    x = math.cos(rad)
    y = math.sin(rad)
    return (x,y)


def get_normal_from_vertices(v1, v2):
    (x1,y1) = v1
    (x2,y2) = v2
    
    dx = x2-x1
    dy = y2-y1
    
    return (-dy, dx)


def backfacing(ang, v1, v2):
    (wx, wy) = get_normal_from_vertices(v1, v2)
    (ax, ay) = radians_to_vector(ang)

    return ((ax * wx) + (ay * wy)) >= 0

def maybe_visible(ang, v1, v2):
    return (not backfacing(ang, v1, v2))

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


    
            
    
def main():
    sectors2 = []
    for sidx in range(0, len(sectors), 4):
        sectors2.append([sectors[i] for i in range(sidx, sidx+4)])


    wall_angle_ranges = []
    print("vis_range angle_ranges[NUM_WALLS] = {")
    
    for (woff, wnum, f, c) in sectors2:
        vert_indexes = range(woff, woff+wnum)
        for i,widx in enumerate(vert_indexes):

            last_wall_for_sector = i == len(vert_indexes)-1
            nwidx = woff if last_wall_for_sector else widx+1
            #print("i1 {} i2 {}".format(widx, nwidx))
            
            vidx = walls[widx]
            nvidx = walls[nwidx]
            #print("vi1 {} vi2 {}".format(vidx, nvidx))
            
            v1 = vertexes[vidx]
            v2 = vertexes[nvidx]
            
            # walls are defined clockwise
            #tbl = []
            #print((v1,v2))
            tbl = [(ang, maybe_visible(ang, v1, v2)) for ang in iterate_angles()]

            def similar_test(a, b):
                (aang, avis) = a
                (bang, bvis) = b
                return avis == bvis

            chunked = group_into_similar_chunks(similar_test, tbl)

            filtered = filter(lambda chk: chk[0][1], chunked)
            mapped = map(lambda chk: (radians_to_degrees(chk[0][0]), radians_to_degrees(chk[-1][0])), filtered)

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
    

