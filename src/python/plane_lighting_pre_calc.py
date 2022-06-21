SCREEN_WIDTH = 64  #256
SCREEN_HEIGHT = 144 #224
RENDER_WIDTH = SCREEN_WIDTH/4
RENDER_HEIGHT = SCREEN_HEIGHT

CONST3 = SCREEN_HEIGHT/2

DARK = 'DARK'
MID = 'MID'
LIGHT = 'LIGHT'

MID_LIGHT_DIST = 110
MID_DIST = 150
MID_DARK_DIST = 250
DARK_DIST = 350
FADE_DIST = 400

def slopes():
    slopes = []
    for y in range(SCREEN_HEIGHT):
        dy = abs((y-SCREEN_HEIGHT/2)+0.5)
        slopes.append((RENDER_WIDTH/2)/dy)
    return slopes

yslopes = slopes()

#print(yslopes)



def gen_floor_slopes():
    total_bytes = 0
    for plane_height in range(10,11): #10):
        #print("PLANE HEIGHT {}".format(plane_height))
        for y in range(RENDER_HEIGHT):
            #print("-- start y: {}".format(y))

            min_y = 0
            max_rows = y
            s = max_rows

            pixels = []

            if max_rows >= 1:
                s += (max_rows-1)*2
                #print("start y: {} has {} bytes".format(y, s))



                cur_dist = 0
                #print("slope {}".format(slope_at_y))
                #print("max rows {}".format(max_rows))


                #inv_dist = 1/dist
                #inv_dist_per_y = inv_dist/max_rows
                for up_y in range(y,0, -1):
                    slope_at_y = yslopes[y]
                    cur_dist += slope_at_y * plane_height
                    if cur_dist >= DARK_DIST:
                        pixels.append(DARK)
                    elif cur_dist >= MID_DARK_DIST:
                        pixels.append(MID)
                    else:
                        pixels.append(LIGHT)
                    #if up_y == y:
                    #    dist_for_row = 0
                    #else:
                    #    inv_dist_for_row = (y - up_y) * inv_dist_per_y
                    #    dist_for_row = 1.0/inv_dist_for_row
                    #print("at y {}, dist is {}".format(up_y, cur_dist)) #dist_for_row))
                #print(pixels)

            total_bytes += s

    return total_bytes

gen_floor_slopes()


screen_height = 144
half_screen_height = screen_height/2
y_scale = 72


NEAR_Z = 10



def reverse_project_screen_y_and_floor_y(screen_y, rel_floor_y):
    b = (143 - screen_y) - half_screen_height
    z = (y_scale * rel_floor_y) / (((screen_height-1) - screen_y) - half_screen_height)
    return z

def project_z_and_floor_y(z, rel_floor_y):
    #if z <= NEAR_Z:
    #    return SCREEN_HEIGHT-1

    screen_y = (screen_height-1) - (half_screen_height + y_scale * rel_floor_y / z)
    return screen_y



def calc_y_positions_for_floor_height(rel_floor_height):
    print("rel floor height: {}".format(rel_floor_height))
    #top_y_pos = 76
    top_y_pos = 72
    bot_y_pos = 143


    top_z = reverse_project_screen_y_and_floor_y(top_y_pos, rel_floor_height)
    bot_z = reverse_project_screen_y_and_floor_y(bot_y_pos, rel_floor_height)
    print("top z: {}".format(top_z))
    print("bot z: {}".format(bot_z))

    if top_z < NEAR_Z:
        top_z = NEAR_Z
    if bot_z < NEAR_Z:
        bot_z = NEAR_Z

    # table lookup
    if top_z == bot_z:
        return [-1,-1,-1,-1]


    # table lookup
    inv_top_z = 1/top_z
    inv_bot_z = 1/bot_z

    inv_dz = (inv_bot_z-inv_top_z)/(bot_y_pos-top_y_pos)
    # division
    inv_dy_per_dz = 1 / inv_dz

    #print("inv dz per dy {}".format(inv_dz))
    #print("inv dy per dz {}".format(inv_dy_per_dz))


    light_inv_dists = [1/DARK_DIST, 1/MID_DARK_DIST, 1/MID_DIST, 1/MID_LIGHT_DIST]# , 1/NEAR_Z]


    res = [] #rel_floor_height]
    # [ start_z, start_y_pos, rel_floor_height ]
    for d in light_inv_dists:
        inv_z_diff = d - inv_top_z

        # multiplication
        y_diff = inv_dy_per_dz * inv_z_diff
        new_y = top_y_pos + y_diff
        #print("dist {} is reached at y {}".format(1/d, new_y))
        if new_y >= screen_height:
            res.append(screen_height)
        else:
            res.append(int(new_y))

    return res


def calc_y_positions_for_ceil_height(rel_ceil_height):
    print("rel ceil height: {}".format(rel_ceil_height))
    #top_y_pos = 76
    top_y_pos = 0
    bot_y_pos = 70

    top_z = reverse_project_screen_y_and_floor_y(top_y_pos, rel_ceil_height)
    bot_z = reverse_project_screen_y_and_floor_y(bot_y_pos, rel_ceil_height)
    print("top z: {}".format(top_z))
    print("bot z: {}".format(bot_z))

    if top_z < NEAR_Z:
        top_z = NEAR_Z
    if bot_z < NEAR_Z:
        bot_z = NEAR_Z

    # table lookup
    if top_z == bot_z:
        return [-1,-1,-1,-1]


    # table lookup
    inv_top_z = 1/top_z
    inv_bot_z = 1/bot_z

    inv_dz = (inv_bot_z-inv_top_z)/(bot_y_pos-top_y_pos)
    # division
    inv_dy_per_dz = 1 / inv_dz

    #print("inv dz per dy {}".format(inv_dz))
    #print("inv dy per dz {}".format(inv_dy_per_dz))


    light_inv_dists = [1/MID_LIGHT_DIST, 1/MID_DIST, 1/MID_DARK_DIST, 1/DARK_DIST]# , 1/NEAR_Z]


    res = [] #rel_floor_height]
    # [ start_z, start_y_pos, rel_floor_height ]
    for d in light_inv_dists:
        inv_z_diff = d - inv_top_z

        # multiplication
        y_diff = inv_dy_per_dz * inv_z_diff
        new_y = top_y_pos + y_diff
        #print("dist {} is reached at y {}".format(1/d, new_y))
        if new_y >= screen_height:
            res.append(screen_height)
        else:
            res.append(int(new_y))

    return res

# rel_floor_height will be 10 bits of index
# screen_y will be 8 bits?
# so an 18 bit index lol
# thats fine

# relative floor height
if __name__ == '__main__':
    screen_y = 143
    rel_floor_y = -40
    tbl = []
    #print(calc_y_positions_for_start_height_and_floor_height(70, 40))

    #print(calc_y_positions_for_start_height_and_floor_height(74, -40))
    #print(calc_y_positions_for_start_height_and_floor_height(74, -39))

    for rel_y in range(0, 512): #512, -1):
        #res = calc_y_positions_for_ceil_height(rel_y)
        res =  calc_y_positions_for_floor_height(-rel_y)
        print(res)
        tbl += res
    print(tbl)

    #for rel_y in range(512, 0, -1):
    #    res =  calc_y_positions_for_floor_height(rel_y)
    #    print(res)
    #    tbl += res
    #print(tbl)
        #print([143-y for y in calc_y_positions_for_start_height_and_floor_height(rel_y)])

#print(calc_y_positions_for_start_height_and_floor_height(-40))
    #print(calc_y_positions_for_start_height_and_floor_height(30, -40))

    #for rel_floor_y in range(-512, 512):
    #    for screen_y_start in range(144):
    # tbl.append(calc_y_positions_for_start_height_and_floor_height(screen_y, rel_floor_y))
    #print(tbl[(-40-512)*144 + 0])


