import math
from PIL import Image

avail_colors = [
    (154,93,93),
    (216,154,124),
    (247,185,154),
    (124,247,154),
    (62,93,93),
    (0,31,31),
    (62,124,0),
    (62,62,62),
    (216,62,0),
    (31,0,31),
    (154,0,64),
    (62,154,185),
    (93,93,185),
    (216,185,0),
    (0,0,0)
]

def clamp(c):
    return 255 if c > 255 else 0 if c < 0 else c

def lighten(c):
    return scale_color(c, 1.3)
    
def darken(c):
    return scale_color(c, .7)
    
def scale_color(c, amount):
    (r,g,b) = c
    return (clamp(int(r*amount)),clamp(int(g*amount)),clamp(int(b*amount)))
    
def dist(c1, c2):
    (r1,g1,b1) = c1
    (r2,g2,b2) = c2
    rmean = int((r1+r2)/2)
    r = int(r1-r2)
    g = int(g1-g2)
    b = int(b1-b2)
    return math.sqrt((((512+rmean)*r*r)>>8)+(4*g*g)+(((767-rmean)*b*b)>>8))
    
def find_closest_idx(col):
    dists = [dist(c2,col) if c2 != col else 10000000000000 for c2 in avail_colors]
    #print(dists)
    min_dist = min(dists)
    #print(min_dist)
    closest_color_idx = dists.index(min_dist)
    return closest_color_idx
    
#for idx,c in enumerate(avail_colors):
#    
#    # near colors
#    dark = find_closest_idx(darken(c))
#    extra_dark = find_closest_idx(darken(darken(c)))
#    light = find_closest_idx(lighten(c))
#    extra_light = find_closest_idx(lighten(lighten(c)))
#    
#    print(extra_dark+1, end=', ')
#    print(dark+1, end=', ')
#    print(idx+1, end=', ')
#    print(light+1, end=', ')
#    print(extra_light+1, end=',\n')
    
tables = [
    # far
        [
            lambda c:find_closest_idx(darken(darken(c)))+1,
            lambda c:find_closest_idx(darken(darken(c)))+1,
            lambda c:find_closest_idx(darken(c))+1,
            lambda c:avail_colors.index(c)+1,
            lambda c:find_closest_idx(lighten(lighten(c)))+1
        ],
    # near
        [
            lambda c:find_closest_idx(darken(darken(c)))+1,
            lambda c:find_closest_idx(darken(c))+1,
            lambda c:avail_colors.index(c)+1,
            lambda c:find_closest_idx(lighten(c))+1,
            lambda c:find_closest_idx(lighten(lighten(c)))+1
        ]
]

#img = Image.new(mode="RGB",size=(10,15))
#pixels = img.load()

#for y,base_col in enumerate(avail_colors):
#    for dist_idx in range(2):
#        base_x = dist_idx*5
#        light_levels = tables[1-dist_idx]
#        for idx,light_level in enumerate(light_levels):
#            x = base_x + idx
#            pixels[x,y] = avail_colors[light_level(base_col)-1]

#with open("C:\\Users\\Erik\\code\\genesis\\DOOM\\src\\python\\editor\\light_remapping_table_autogen.png", "wb") as fp:
#    img.save(fp)


with NEAR_FILE
for idx in range(2):
    light_levels = tables[idx]
    print("// {} table".format(["far", "near"][idx]))
    for idx,light_level in enumerate(light_levels):
        print("  // {} light level".format(idx-2))
        print("    0", end=', ')
        
        for base_col in avail_colors:
            print("PIX({}), ".format(light_level(base_col)), end='')
        print()

    