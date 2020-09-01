import math, pygame
import span_buffer, util
import wad

cam_x = 0
cam_y = 0
cam_z = 0
cam_angle = .0

EYE_HEIGHT = 41

#draw_mode = TOPDOWN_DRAW_ALL_SEGS

backface_culling = False
bsp_node_frustum_culling = False
seg_frustum_culling = False
pvs_check = False
draw_outline = False
show_render_traversal = False

class DrawStats():
    def __init__(self):
        self.reset()

    def reset(self):
        self.nodes_traversed = 0
        self.ssectors_processed = 0
        self.segs_backface_culled = 0
        self.segs_transformed = 0
        self.segs_fully_clipped = 0
        self.segs_partially_clipped = 0
        self.segs_drawn = 0

        self.seg_v1 = None
        self.seg_v2 = None
        self.seg_dx = None
        self.seg_dy = None
        self.seg_dv1 = None
        self.seg_dv2 = None
        self.seg_r1 = None
        self.seg_r2 = None
        self.seg_normal = None
        self.seg_normal_angle = None
        
        self.adjusted_angle = None
        self.player_vector = None
        self.seg_dot_product = None
        self.seg_v1_angle = None
        self.seg_v2_angle = None

        self.seg_rot_v1_angle = None
        self.seg_rot_v2_angle = None

        self.v1_fov_pos = None
        self.v2_fov_pos = None



def calc_dist(x1,y1,x2,y2):  
    dist = math.sqrt((x2 - x1)**2 + (y2 - y1)**2)  
    return dist  

draw_stats = DrawStats()
flip_func = None

ssector_depth_table = {}

def ssect_draw_visible_segs(level_data, topdown_draw_surf, persp_draw_surf, ssect):
    start_seg = ssect.first_seg
    num_segs = ssect.num_segs
    segs = level_data['SEGS']

    ssect_key = (ssect.num_segs, ssect.first_seg)
    if ssect_key in ssector_depth_table:
        avg_dist = ssector_depth_table[ssect_key]
    else:
        
        avg_dist = 0
        for i in range(start_seg, start_seg+num_segs):
            seg = segs[i]

            v1 = level_data['VERTEXES'][seg.begin_vert]
            v2 = level_data['VERTEXES'][seg.end_vert]
            (v1x,v1y) = v1.x, v1.y
            (v2x,v2y) = v2.x, v2.y
            v1_dist = calc_dist(cam_x, cam_y, v1x, v1y)
            v2_dist = calc_dist(cam_x, cam_y, v2x, v2y)
            avg = (v1_dist + v2_dist)/2
            avg_dist += avg


        avg_dist = avg_dist / num_segs
        ssector_depth_table[ssect_key] = avg_dist
    
            
    
    draw_stats.ssectors_processed += 1
    
    for i in range(start_seg, start_seg+num_segs):
        seg = segs[i]
        
        #if draw_stats.segs_backface_culled >= 1 or draw_stats.segs_drawn >= 1 or draw_stats.segs_fully_clipped >= 1:
        #    return

        res = transform_seg(level_data, seg, topdown_draw_surf, persp_draw_surf, avg_dist)
        if res == BACKFACE_CULLED:
            draw_stats.segs_backface_culled += 1
        else:
            draw_stats.segs_transformed += 1

        if res == FULLY_CLIPPED:
            draw_stats.segs_fully_clipped += 1
            
            #draw_stats.segs_drawn += 1
        if res == DRAWN:
            draw_stats.segs_drawn += 1
            #pygame.draw.line(topdown_draw_surf, col, v1, v2) #(255,255,255)
            #flip_func()
            #pygame.time.wait(2)
            


DRAW_WIDTH = 400
DRAW_HEIGHT = 300


column_buffer = []
y_buffer = []
def clear_col_buffer():
    global column_buffer, y_buffer
    column_buffer = [0 for x in range(DRAW_WIDTH)]
    y_buffer = [(-1,DRAW_HEIGHT) for x in range(DRAW_WIDTH)]
    



    

                    


BACKFACE_CULLED = 'bface'
FULLY_CLIPPED = 'fully_clipped'
DRAWN = 'drawn'

WHITE = (255,255,255)
GREEN = (  0,255,  0)







depth_col_map = {}
def bsp_depth_color(depth):
    global depth_col_map
    if depth not in depth_col_map:
        depth_col_map[depth] = util.rand_color()
    return depth_col_map[depth]


def translate_point(point):
    (x, y) = point
    return (x-cam_x, y-cam_y)
    
    
def rotate_point(point):
    (x, y) = point

    s = math.sin(cam_angle)
    c = math.cos(cam_angle)
    rot_x = x * c - y * s
    rot_y = x * s + y * c

    return (rot_x, rot_y)

FOV = 90
ASPECT_RATIO = DRAW_WIDTH / DRAW_HEIGHT
SCALE = 1 / math.tan(.5 * (math.radians(90)))


CONST_1 = .5 * DRAW_WIDTH
CONST_2 = .5 * DRAW_WIDTH * SCALE / min(1, ASPECT_RATIO)
CONST_3 = .5 * DRAW_HEIGHT
CONST_4 = .5 * DRAW_HEIGHT * SCALE * max(1, ASPECT_RATIO)

def project_vertex(x3d, y3d, z3d):
        
    x2d = CONST_1 + CONST_2 * x3d / z3d
    y2d = CONST_3 + CONST_4 * y3d / z3d
    
    return (x2d, y2d)


def project_vertex_x(x3d, z3d):
    return CONST_1 + CONST_2 * x3d / z3d

def project_vertex_y(y3d, z3d):
    return CONST_3 + CONST_4 * y3d / z3d

def project_y(y3d, z3d):
    #y_scale = SCALE * y3d
    y_scale = CONST_4 * y3d
    if y_scale > 0:
        return CONST_3 - (y_scale / z3d) #(.5 * DRAW_HEIGHT) - (y_scale / z3d) 
    else:
        return CONST_3 + (y_scale / z3d) #(y_scale / z3d) + (.5 * DRAW_HEIGHT)
    

zoom = 12

def adjust_to_display(point):
    global zoom
    (x, y) = point
    return ((x/zoom)+(DRAW_WIDTH/2)), ((-y/zoom)+(DRAW_HEIGHT/2))
    
def transform_point(point):
    (x, y) = point
    #trans_x, trans_y = ((x-cam_x), ((-y)-cam_y))
    trans_x, trans_y = ((x-cam_x), ((y)-cam_y))
    rot_x = (math.cos(cam_angle) * trans_x) - (math.sin(cam_angle) * trans_y)
    rot_y = (math.sin(cam_angle) * trans_x) + (math.cos(cam_angle) * trans_y)
    return ((rot_x/12)+(DRAW_WIDTH/2)), ((-rot_y/12)+(DRAW_HEIGHT/2))


def draw_level_vertexes(level_data, surf, color):
    vertexes = level_data['VERTEXES']
    for vert in vertexes:
        trans_x, trans_y = transform_point((vert.x, vert.y))
        pygame.draw.rect(surf, color, (trans_x, trans_y, 1, 1), 1)


WITHIN_FOV = "within"
LEFT_OF_FOV = "left"
RIGHT_OF_FOV = "right"

def angle_fov_position(angle):
    if angle <= 180:
        if angle > 135:
            return LEFT_OF_FOV
        elif angle < 45:
            return RIGHT_OF_FOV
        else:
            return WITHIN_FOV
    else:
        if angle < 270:
            return LEFT_OF_FOV
        else:
            return RIGHT_OF_FOV
        
    
def seg_fully_clipped(rot_v1, rot_v2):
    (r1x, r1y) = rot_v1
    (r2x, r2y) = rot_v2
    
    ang_v1 = math.degrees(math.atan2(r1y, r1x)) % 360
    ang_v2 = math.degrees(math.atan2(r2y, r2x)) % 360

    
    
    v1_pos = angle_fov_position(ang_v1)
    v2_pos = angle_fov_position(ang_v2)

    draw_stats.v1_fov_pos = v1_pos
    draw_stats.v2_fov_pos = v2_pos

    if ang_v1 >= 180 and ang_v2 >= 180:
        return True
    
    if v1_pos == WITHIN_FOV or v2_pos == WITHIN_FOV:
        return False
    elif v1_pos == LEFT_OF_FOV and v2_pos == RIGHT_OF_FOV:
        return False
    else:
        return True
    

def bbox_on_screen(v1,v2,v3,v4):
    
    (vv1_x,vv1_y) = rotate_point(translate_point(v1))
    (vv2_x,vv2_y) = rotate_point(translate_point(v2))
    (vv3_x,vv3_y) = rotate_point(translate_point(v3))
    (vv4_x,vv4_y) = rotate_point(translate_point(v4))
    
    #nvv1_y = 1 if (vv1_y <= 0) else vv1_y
    #nvv2_y = 1 if (vv2_y <= 0) else vv2_y
    #nvv3_y = 1 if (vv3_y <= 0) else vv3_y
    #nvv4_y = 1 if (vv4_y <= 0) else vv4_y
    
    vv1 = (vv1_x, vv1_y)
    vv2 = (vv2_x, vv2_y)
    vv3 = (vv3_x, vv3_y)
    vv4 = (vv4_x, vv4_y)

        
    clipped = (seg_fully_clipped(vv1,vv2) and
               seg_fully_clipped(vv2,vv3) and
               seg_fully_clipped(vv3,vv4) and
               seg_fully_clipped(vv4,vv1))
    
    return not clipped



def dot_product(v1, v2):
    (v1x,v1y) = v1
    (v2x,v2y) = v2

    return (v1x*v2x + v1y*v2y)

def angle_diff(a, b):
    phi = abs(b-a) % 360
    return 360 - phi if phi > 180 else phi


def lerp(a, b, percentage):
  return (1 - percentage) * a + percentage * b;

FAR_COLOR = 60
NEAR_COLOR = 180
MAX_DEPTH = 2048
def depth_color(depth):
    return lerp(NEAR_COLOR,FAR_COLOR, depth/MAX_DEPTH)

def fill_floors(persp_draw_surf):
    grad_size = int(DRAW_HEIGHT/2)
    
    
    for y in range(0, int(DRAW_HEIGHT/2)+1):
        depth = lerp(0, MAX_DEPTH, y/grad_size)
        col = depth_color(depth)
        pygame.draw.line(persp_draw_surf, (col,col,col), (0, y), (DRAW_WIDTH, y))
        
    for step,y in enumerate(range(DRAW_HEIGHT-1, int(DRAW_HEIGHT/2), -1)):
        depth = lerp(0, MAX_DEPTH, step/grad_size)
        col = depth_color(depth)
        pygame.draw.line(persp_draw_surf, (col,col,col), (0, y), (DRAW_WIDTH, y))


def check_ceiling_floor(front_sector, back_sector):
    
    if back_sector is None:
        return (True, True)

    
    update_ceiling = back_sector.ceiling_height != front_sector.ceiling_height

    update_floor = back_sector.floor_height != front_sector.floor_height
    

    
    if ((back_sector.ceiling_height <= front_sector.floor_height) or
        (back_sector.floor_height >= front_sector.ceiling_height)):
        # closed door, or elevator that's above the current ceiling
        update_floor = True
        update_ceiling = True

    if front_sector.ceiling_height <= cam_z:
        # below view plane
        # ceiling is too low to see
        update_ceiling = False

    if front_sector.floor_height >= cam_z:
        # above view plane
        # floor is too high to see
        update_floor = False

#def calc_wall_height(seg, v1x, v2x, v1ang, v2ang):
    
        

def transform_seg(level_data, seg, topdown_draw_surf, persp_draw_surf, avg_sector_dist): #, color):
    
    segs = level_data['SEGS']
    vertexes = level_data['VERTEXES']
    linedefs = level_data['LINEDEFS']
    v1 = vertexes[seg.begin_vert]
    v2 = vertexes[seg.end_vert]

    adjusted_angle = math.radians(360 - math.degrees(cam_angle))
    #player_vector = (math.cos(cam_angle), math.sin(cam_angle))
    draw_stats.adjusted_angle = adjusted_angle
    player_vector = (math.cos(adjusted_angle), math.sin(adjusted_angle))
    
    
    #wall_normal
    seg_dx = v2.x - v1.x
    seg_dy = v2.y - v1.y

    
    seg_normal = (seg_dy,-seg_dx) # real (-seg_dy,seg_dx)
    seg_normal_angle = math.atan2(seg_dy, -seg_dx)
    #else:
    #    seg_normal = (-seg_dy,seg_dx) # real (seg_dy,-seg_dx)

        
    #transf_v1 = transform_point((v1.x, v1.y))
    #transf_v2 = transform_point((v2.x, v2.y))

    trans_v1 = translate_point((v1.x, v1.y))
    trans_v2 = translate_point((v2.x, v2.y))
    rot_v1 = rotate_point(trans_v1)
    rot_v2 = rotate_point(trans_v2)

    seg_dv1 = ((v1.x - cam_x), (v1.y - cam_y))
    seg_dv2 = ((v2.x - cam_x), (v2.y - cam_y))
    
    draw_stats.seg_v1 = v1
    draw_stats.seg_v2 = v2
    draw_stats.seg_dx = seg_dx
    draw_stats.seg_dy = seg_dy
    draw_stats.seg_dv1 = seg_dv1
    draw_stats.seg_dv2 = seg_dv2
    draw_stats.seg_r1 = rot_v1
    draw_stats.seg_r2 = rot_v2
    draw_stats.seg_normal = seg_normal
    draw_stats.seg_normal_angle = seg_normal_angle

    draw_stats.player_vector = player_vector
    draw_stats.seg_dot_product = dot_product(player_vector, seg_normal) 

    (dv1x,dv1y) = seg_dv1
    (dv2x,dv2y) = seg_dv2
    draw_stats.seg_v1_angle = math.degrees(math.atan2(dv1y, dv1x))
    draw_stats.seg_v2_angle = math.degrees(math.atan2(dv2y, dv2x))

    (r1x,r1y) = rot_v1
    (r2x,r2y) = rot_v2
    
    draw_stats.seg_rot_v1_angle = math.degrees(math.atan2(r1y, r1x)) % 360
    draw_stats.seg_rot_v2_angle = math.degrees(math.atan2(r2y, r2x)) % 360

    

    seg_ang = math.degrees(math.atan2(seg_dy, -seg_dx)) % 360
    player_ang = math.degrees(cam_angle) % 360

    dang = angle_diff(seg_ang, player_ang)

    
    res = DRAWN

    backface_culled = False
    fully_clipped = False
    if dang < 40: #dang < 90:
        backface_culled = True
        
    if backface_culling and backface_culled:
        return BACKFACE_CULLED


    if seg_fully_clipped(rot_v1, rot_v2):
        fully_clipped = True
    
    if seg_frustum_culling and fully_clipped:
        return FULLY_CLIPPED
    
        

    
    disp_v1 = adjust_to_display(rot_v1)
    disp_v2 = adjust_to_display(rot_v2)
    
    
    col = WHITE
    
    (disp_v1_x,disp_v1_y) = disp_v1
    (disp_v2_x,disp_v2_y) = disp_v2

    
    #if backface_culling:
    #    if (disp_v1_x > disp_v2_x):
    #        assert False, "This should never happen anymore!"
    #        return BACKFACE_CULLED
        
    pygame.draw.line(topdown_draw_surf, col, disp_v1, disp_v2)


    linedef_idx = seg.linedef
    linedef = level_data['LINEDEFS'][linedef_idx]
    
    
    sector_idx = wad.seg_sector_idx(seg, level_data)
    sector = level_data['SECTORS'][sector_idx]

    
    portal = False
    if linedef.right_sidedef != -1 and linedef.left_sidedef != -1:
        # todo: handle portals
        portal = True
        #return DRAWN

        other_sector_idx = wad.portal_seg_other_sector_idx(seg, level_data)
        
        other_sector = level_data['SECTORS'][other_sector_idx]
    
        

    if r1y <= 0 and r2y <= 0:
        return FULLY_CLIPPED
    
    near_z = 0.01
    if r1y <= 0:
        
        # clip r1y and heights?
        # somehow clip
        dx = r2x - r1x
        dy = r2y - r1y
        if dy == 0: # how the fuck??
            return FULLY_CLIPPED
        dx_over_dy = dx / dy
        
        how_much_dy = near_z - r1y # how much y to move
        how_much_dx = dx_over_dy * dy 
        r1y = near_z
        r1x = r1x + how_much_dx

    elif r2y <= 0:
        dx = r2x - r1x
        dy = r2y - r1y
        if dy == 0:
            return FULLY_CLIPPED
        dx_over_dy = dx/dy

        how_much_dy = near_z - r2y
        how_much_dx = dx_over_dy * dy
        r2y = near_z
        r2x = r2x - how_much_dx
        

    floor_brg = depth_color(avg_sector_dist)
    floor_col = (floor_brg, floor_brg, floor_brg)

    handle_other_floor = False
    handle_other_ceil = False

    sect_floor = sector.floor_height
    sect_ceil = sector.ceiling_height

    if portal:
        other_sect_floor = other_sector.floor_height
        other_sect_ceil = other_sector.ceiling_height
        
    if (not backface_culled) and (not fully_clipped):
        
        v1_proj_x = project_vertex_x(r1x, r1y)
        v2_proj_x = project_vertex_x(r2x, r2y)
        
        trans_floor_height = (sector.floor_height - cam_z)
        trans_ceil_height = (sector.ceiling_height - cam_z)
        
        v1_proj_top_y = project_vertex_y(trans_ceil_height, r1y)
        v1_proj_bot_y = project_vertex_y(trans_floor_height, r1y)

        v2_proj_top_y = project_vertex_y(trans_ceil_height, r2y)
        v2_proj_bot_y = project_vertex_y(trans_floor_height, r2y)
        
        if portal:
            
            
            
            
            if other_sect_floor > sect_floor:
                handle_other_floor = True
                trans_other_floor_height = (other_sector.floor_height - cam_z)
                v1_proj_back_bot_y = project_vertex_y(trans_other_floor_height, r1y)
                v2_proj_back_bot_y = project_vertex_y(trans_other_floor_height, r2y)
            
            if other_sect_ceil < sect_ceil:
                handle_other_ceil = True
                trans_other_ceil_height = (other_sector.ceiling_height - cam_z)
                v1_proj_back_top_y = project_vertex_y(trans_other_ceil_height, r1y)
                v2_proj_back_top_y = project_vertex_y(trans_other_ceil_height, r2y)
                
            
        
        

        
        
        x_steps = int(v2_proj_x - v1_proj_x)

        if x_steps <= 0:
            return DRAWN

        #return
        one_over_r1y = 1/r1y
        one_over_r2y = 1/r2y

        left_x = int(v1_proj_x)
        right_x = int(v2_proj_x)


        clipped_left_x = max(left_x, 0)
        clipped_right_x = min(right_x, DRAW_WIDTH)
        
        for step,x in enumerate(range(clipped_left_x, clipped_right_x)):

            if x >= DRAW_WIDTH:
                break
                        

            progress = (x-left_x)/(right_x-left_x)
            #progress = step/x_steps

            
            one_over_depth = lerp(one_over_r1y, one_over_r2y, progress)
            depth = 1/one_over_depth
            brg = depth_color(depth)

            top_height = DRAW_HEIGHT - lerp(v1_proj_top_y, v2_proj_top_y, progress)
            bot_height = DRAW_HEIGHT - lerp(v1_proj_bot_y, v2_proj_bot_y, progress)

            if portal:
                if handle_other_ceil:
                    back_top_height = DRAW_HEIGHT - lerp(v1_proj_back_top_y, v2_proj_back_top_y, progress)

                if handle_other_floor:
                    back_bot_height = DRAW_HEIGHT - lerp(v1_proj_back_bot_y, v2_proj_back_bot_y, progress)                
            
                
            col = (brg,brg,brg)
            if brg <= 0:
                continue
            
            
            
            if column_buffer[x] == 0:

                if not portal:
                    column_buffer[x] = 1
                
                min_y,max_y = y_buffer[x]
                
                
                clipped_top_y = max(min_y+1, top_height)
                clipped_bot_y = min(max_y-1, bot_height)

                y_clip = [clipped_top_y, clipped_bot_y]
                


                if handle_other_ceil:
                    clipped_back_top_y = max(min_y+1, back_top_height)
                    y_clip[0] = clipped_back_top_y 
                    
                if handle_other_floor:
                    clipped_back_bot_y = min(max_y-1, back_bot_height)
                    y_clip[1] = clipped_back_bot_y

                y_buffer[x] = tuple(y_clip)
                

                if y_clip[0] >= y_clip[1]:
                    column_buffer[x] = 1
                    continue
                
                #if not portal:
                

                if portal and draw_outline:
                    col = (255,0,0)
                

                # DRAW outline
                if draw_outline:
                    # draw top and bottom
                    pygame.draw.line(persp_draw_surf, col, (x, clipped_top_y), (x, clipped_top_y))
                    pygame.draw.line(persp_draw_surf, col, (x, clipped_bot_y), (x, clipped_bot_y))

                    if handle_other_ceil:                   
                        pygame.draw.line(persp_draw_surf, col, (x, clipped_back_top_y), (x, clipped_back_top_y))

                        
                    if handle_other_floor:
                        pygame.draw.line(persp_draw_surf, col, (x, clipped_back_bot_y), (x, clipped_back_bot_y))

                    #not portal
                    if step == 0 or x == clipped_right_x-1:
                        # draw left and right sides for walls
                        if not portal:
                            pygame.draw.line(persp_draw_surf, col, (x, clipped_top_y), (x, clipped_bot_y))

                        if handle_other_ceil:
                            pygame.draw.line(persp_draw_surf, col, (x, clipped_back_top_y), (x, clipped_top_y))                            
                        if handle_other_floor:
                            pygame.draw.line(persp_draw_surf, col, (x, clipped_back_bot_y), (x, clipped_bot_y))
                        
                            
                else:
                    if portal:
                        if handle_other_ceil:
                            pygame.draw.line(persp_draw_surf, col, (x, clipped_top_y), (x, clipped_back_top_y)) 
                        if handle_other_floor:
                            pygame.draw.line(persp_draw_surf, col, (x, clipped_bot_y), (x, clipped_back_bot_y)) 
                    else:
                        # draw middle wall portion
                            
                        pygame.draw.line(persp_draw_surf, col, (x, clipped_top_y), (x, clipped_bot_y))

                    
                    if floor_brg > 0:
                        # draw floor
                        floor_col = (128,128,128)
                        pygame.draw.line(persp_draw_surf, floor_col, (x, min_y+1), (x, clipped_top_y))
                        pygame.draw.line(persp_draw_surf, floor_col, (x, clipped_bot_y), (x, max_y-1))
                    
                
                
    
    
    return DRAWN


def draw_level_segs(level_data, surf, color):
    segs = level_data['SEGS']

    for seg_idx,seg in enumerate(segs):
        draw_seg(level_data, surf, seg, color)
        
        
def draw_level_linedefs(level_data, surf, color):
    linedefs = level_data['LINEDEFS']
    vertexes = level_data['VERTEXES']
    
    for linedef in linedefs:
        v1 = vertexes[linedef.begin_vert]
        v2 = vertexes[linedef.end_vert]
        trans_v1 = transform_point((v1.x, v1.y))
        trans_v2 = transform_point((v2.x, v2.y))
        
        pygame.draw.line(surf, color, trans_v1, trans_v2)

def draw_bounding_box(v1, v2, v3, v4, surf, color):
    tv1 = transform_point(v1)
    tv2 = transform_point(v2)
    tv3 = transform_point(v3)
    tv4 = transform_point(v4)

    pygame.draw.line(surf, color, tv1, tv2)
    pygame.draw.line(surf, color, tv2, tv3)
    pygame.draw.line(surf, color, tv3, tv4)
    pygame.draw.line(surf, color, tv4, tv1)
    

def draw_bsp_node(node, node_depth, surf, color=None, draw_left=True, draw_right=True):
    if not color:
        color = bsp_depth_color(node_depth)
    lv1 = (node.left_box_left, node.left_box_top)
    lv2 = (node.left_box_right, node.left_box_top)
    lv3 = (node.left_box_right, node.left_box_bottom)
    lv4 = (node.left_box_left, node.left_box_bottom)

    
    rv1 = (node.right_box_left, node.right_box_top)
    rv2 = (node.right_box_right, node.right_box_top)
    rv3 = (node.right_box_right, node.right_box_bottom)
    rv4 = (node.right_box_left, node.right_box_bottom)

    # disabled: draw bounding boxes
    if draw_left:
        draw_bounding_box(lv1, lv2, lv3, lv4, surf, color)
    if draw_right:
        draw_bounding_box(rv1, rv2, rv3, rv4, surf, color)


    part_v1 = node.partition_x_coord, node.partition_y_coord
    part_v2 = node.partition_x_coord+node.dx, node.partition_y_coord+node.dy

    # disabled: draw split plane
    #tpv1 = transform_point(part_v1)
    #tpv2 = transform_point(part_v2)
    #pygame.draw.line(surf, (0, 255, 0), tpv1, tpv2)


def draw_player(draw_surf, color):
    x, y = DRAW_WIDTH/2, DRAW_HEIGHT/2

    top = (x, y-1)
    left = (x-2, y+2)
    right = (x+2, y+2)


    pygame.draw.polygon(draw_surf, color, [top, right, left]) 
    
    
    
