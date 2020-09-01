import math, pygame, sys

import bsp, draw, wad, span_buffer
import cProfile

DEFAULT_WADFILE = "./doom.wad"

screen = None
topdown_draw_surf = None
persp_draw_surf = None
scale_surf = None

BLIT_WIDTH = draw.DRAW_WIDTH*2
BLIT_HEIGHT = draw.DRAW_HEIGHT*2

SCREEN_WIDTH = BLIT_WIDTH*2
SCREEN_HEIGHT = BLIT_HEIGHT

PROJECTED_WIDTH = 256
PROJECTED_HEIGHT = 144



def init():
    global screen, topdown_draw_surf, persp_draw_surf, scale_surf
    pygame.init()
    pygame.font.init()
    screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
    pygame.display.set_caption('WAD loader')
    
    topdown_draw_surf = pygame.Surface((draw.DRAW_WIDTH, draw.DRAW_HEIGHT), flags=pygame.SRCALPHA)
    persp_draw_surf = pygame.Surface((draw.DRAW_WIDTH, draw.DRAW_HEIGHT), flags=pygame.SRCALPHA)
    scale_surf = pygame.Surface((BLIT_WIDTH, BLIT_HEIGHT), flags=pygame.SRCALPHA)
    
    

RED   = (255,   0,   0, 255)
GREEN = (  0, 255,   0, 255)
TRANS_GREEN = (  0, 255,   0, 100)
BLUE  = (  0,   0, 255, 255)
GRAY  = (140, 140, 140, 255)
WHITE = (255, 255, 255, 255)
BLACK = (  0,   0,   0, 255)




def flip(stats_func=lambda : None):
    pygame.transform.scale(topdown_draw_surf,
                           (BLIT_WIDTH, BLIT_HEIGHT),
                           scale_surf)

    
    stats_func()
    screen.fill(BLACK)
    screen.blit(scale_surf, (0, 0))

    
    pygame.transform.scale(persp_draw_surf,
                           (BLIT_WIDTH, BLIT_HEIGHT),
                           scale_surf)
    screen.blit(scale_surf, (BLIT_WIDTH, 0))
    
    # update the screen
    pygame.display.flip()

level = None
    
def main():
    global directory, level
    
    if len(sys.argv) == 1:
        wadfile = DEFAULT_WADFILE
    else:
        wadfile = sys.argv[1]

    if len(sys.argv) > 2:
        level = sys.argv[2]

    directory, is_doom_two = wad.read_wadfile(wadfile)

    raw_level = input("Enter map to load > ")
    level = raw_level.strip()
    
    #if not level:
    #    if is_doom_two:
    #        level = 'MAP24'
    #    else:
    #        level = 'E1M2'

    
    level_data = directory[level+'_DATA']
    things = level_data['THINGS']
    player_thing = next(t for t in things if t.thing_type == 1)
        
    draw.cam_x = player_thing.x_pos
    draw.cam_y = player_thing.y_pos
    
    draw.flip_func = flip
    init()
    
    clock = pygame.time.Clock()
    font = pygame.font.Font('freesansbold.ttf',14)
    
    while(True):

        span_buffer.reset(PROJECTED_WIDTH)
        
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            elif event.type == pygame.KEYUP:
                if event.key == pygame.K_z:
                    draw.zoom += 1
                elif event.key == pygame.K_x:
                    draw.zoom -= 1
                elif event.key == pygame.K_b:
                    draw.backface_culling = not draw.backface_culling
                elif event.key == pygame.K_f:
                    tbl = {(False,False): (False,True),
                           (False,True): (True,False),
                           (True,False): (True,True),
                           (True,True): (False,False)}
                    draw.bsp_node_frustum_culling, draw.seg_frustum_culling = (
                        tbl[(draw.bsp_node_frustum_culling, draw.seg_frustum_culling)]
                    )

                elif event.key == pygame.K_p:
                    draw.pvs_check = not draw.pvs_check
                elif event.key == pygame.K_o:
                    draw.draw_outline = not draw.draw_outline
                elif event.key == pygame.K_t:
                    draw.show_render_traversal = not draw.show_render_traversal
                    
                if draw.zoom <= 0:
                    draw.zoom = 1
        
        keys=pygame.key.get_pressed()
        if keys[pygame.K_LEFT]:
            draw.cam_angle -= .08
        elif keys[pygame.K_RIGHT]:
            draw.cam_angle += .08


        move = False

        if keys[pygame.K_UP]:
            move = True
            factor = 10
        elif keys[pygame.K_DOWN]:
            move = True
            factor = -10

        if move:
            draw.cam_x += (math.sin(draw.cam_angle) * factor)
            draw.cam_y += (math.cos(draw.cam_angle) * factor)
            
        
        
        # erase the screen
        topdown_draw_surf.fill(BLACK)
        persp_draw_surf.fill(BLACK)
        
        
        #col = (255,255,255)

        


        draw.draw_stats.reset()

        
        
        nodes_traversed = 0
        def node_callback(node, level, on_left):
            nonlocal nodes_traversed
            nodes_traversed += 1
            #draw.draw_bsp_node(node, level, topdown_draw_surf, RED, draw_left=on_left, draw_right=(not on_left))
        ssect_callback = lambda ssect: None
        

        cur_subsector = bsp.find_subsector_for_position(level_data,
                                                        draw.cam_x, draw.cam_y,
                                                        node_callback)

        cur_sector_idx = wad.get_subsector_sector_idx(cur_subsector, level_data)
        cur_sector = level_data['SECTORS'][cur_sector_idx]

        #print("current sector: {}".format(cur_sector_idx))
        target_height = cur_sector.floor_height + draw.EYE_HEIGHT
        if draw.cam_z > target_height:
            draw.cam_z -= 25
        elif draw.cam_z < target_height:
           draw.cam_z = target_height
            
        #draw.cam_z = cur_sector.floor_height + draw.EYE_HEIGHT
        
        

        def draw_node_callback(node, visiting_left, visiting_right):
            draw.draw_stats.nodes_traversed += 1
            if draw.show_render_traversal:
                draw.draw_bsp_node(node, level, topdown_draw_surf, TRANS_GREEN, draw_left=visiting_left, draw_right=visiting_right)

            
        
        def bound_func(ssect):
            draw.ssect_draw_visible_segs(level_data, topdown_draw_surf, persp_draw_surf, ssect)


        draw.clear_col_buffer()
        span_buffer.clear_span_buffer()
        if not draw.draw_outline:
            draw.fill_floors(persp_draw_surf)
        bsp.traverse_bsp_front_to_back(level_data,
                                       draw.cam_x, draw.cam_y, cur_subsector,
                                       draw_node_callback,
                                       ssect_callback = bound_func)

                

        draw.draw_player(topdown_draw_surf, color=RED)
        

        def stats_func():
            line = 0
            
            def draw_font_line(st):
                nonlocal line
                text_surf = font.render(st, True, GREEN)
                scale_surf.blit(text_surf, (0, line))
                line += 15

            fps = clock.get_fps()
            draw_stats = draw.draw_stats
            draw_font_line("drawing map {}".format(level))
            draw_font_line("{} nodes traversed".format(draw_stats.nodes_traversed))
            draw_font_line("pvs {}".format("enabled" if draw.pvs_check else "disabled")) 
            draw_font_line("backface culling {}".format("enabled" if draw.backface_culling else "disabled"))
            draw_font_line("bsp frustum culling {}".format("enabled" if draw.bsp_node_frustum_culling else "disabled"))
            draw_font_line("seg frustum culling {}".format("enabled" if draw.seg_frustum_culling else "disabled"))
            
            draw_font_line("{} subsectors processed".format(draw_stats.ssectors_processed))
            draw_font_line("{} segs backface culled".format(draw_stats.segs_backface_culled))
            draw_font_line("{} segs transformed".format(draw_stats.segs_transformed))
            draw_font_line("{} segs fully clipped".format(draw_stats.segs_fully_clipped))
            
            draw_font_line("{} segs drawn".format(draw_stats.segs_drawn))
            draw_font_line("{} ms".format(int(1000/max(1, fps))))

            """
            print("------------------")
            print("seg v1: {}".format(draw_stats.seg_v1))
            print("seg v2: {}".format(draw_stats.seg_v2))
            print("player: {}".format((draw.cam_x, draw.cam_y)))
            print("player angle: {}".format(math.degrees(draw.cam_angle)))
            print("player mod angle: {}".format(math.degrees(draw.cam_angle) % 360))
            print("player adjusted angle: {}".format(math.degrees(draw_stats.adjusted_angle)))
            print("player vector: {}".format(draw_stats.player_vector))

            print("seg dx dy: {},{}".format(draw_stats.seg_dx, draw_stats.seg_dy))
            print("seg normal: {}".format(draw_stats.seg_normal))
            
            print("seg normal angle: {}".format(draw_stats.seg_normal_angle))

                                      
            print("seg player dot product: {}".format(draw_stats.seg_dot_product))
            
            print("vec from player to seg v1: {}".format(draw_stats.seg_dv1))
            print("vec from player to seg v1: {}".format(draw_stats.seg_dv2))

            print("angle from player to seg v1: {}".format(draw_stats.seg_v1_angle))
            print("angle from player to seg v2: {}".format(draw_stats.seg_v2_angle))

            print("rotated v1: {}".format(draw_stats.seg_r1))
            print("rotated v2: {}".format(draw_stats.seg_r2))
            
            print("angle from player to rot seg v1: {}".format(draw_stats.seg_rot_v1_angle))
            print("angle from player to rot seg v2: {}".format(draw_stats.seg_rot_v2_angle))

            print("v1 fov pos: {}".format(draw_stats.v1_fov_pos))
            print("v2 fov pos: {}".format(draw_stats.v2_fov_pos))
            
            print("------------------")
            """
            #print("max_y_depth: {}".format(draw_stats.max_y_depth))
            
        flip(stats_func)
        
        clock.tick(60)
        

        
    


if __name__ == '__main__':
    #cProfile.run("main()")
    main()
