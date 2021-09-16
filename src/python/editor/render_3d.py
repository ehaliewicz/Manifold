import ctypes.wintypes

import imgui
import sdl2
import sdl2.ext
import random
import math

RENDER_WIDTH = 256
RENDER_HEIGHT = 140

SCALE = 2

vertexes = [
    (90,90),
    (100,100)
]

walls = [
    (0, 1, (255,0,0)),
]

CEIL_HEIGHT = 100
FLOOR_HEIGHT = 0

surf: sdl2.SDL_Surface
#window: sdl2.ext.Window
renderer: sdl2.ext.Renderer

cam_x = 95
cam_y = 40
cam_z = 20
cam_ang = 0

def init_sdl_window():
    global renderer, surf
    sdl2.ext.init()

    window = sdl2.ext.Window("Live View", size=(RENDER_WIDTH*SCALE, RENDER_HEIGHT*2))
    window.show()

    #surf = sdl2.SDL_CreateRGBSurface(0, RENDER_WIDTH, RENDER_HEIGHT, 32, 0,0,0,0)
    #renderer = sdl2.ext.Renderer(surf, )
    #renderer.present()
    renderer = sdl2.ext.Renderer(window)
    renderer.present()
    #processor = sdl2.ext.TestEventProcessor()
    #processor.run(window)


def draw_preview(current_state):
    global cam_ang
    renderer.clear((0,0,0))
    render_from_position(cam_x, cam_y, cam_z, cam_ang, 0, RENDER_WIDTH)
    renderer.present()

    #tex = sdl2.SDL_CreateTextureFromSurface(renderer.sdlrenderer, surf)

    #sdl2.SDL_LockTexture(tex)

    cam_ang += 0.5
    #sdl2.SDL_GL_BindTexture(tex, None, None)

    #imgui.image(tex, RENDER_WIDTH, RENDER_HEIGHT)
    #sdl2.SDL_UnlockTexture(tex)

    cam_ang = cam_ang % 360

def render_from_position(cam_x, cam_y, cam_z, angle, window_min, window_max):
    #surf: sdl2.SDL_Surface
    #surf = window.get_surface()
    for wall in walls:
        (v1_idx, v2_idx, col) = wall
        v1 = vertexes[v1_idx]
        v2 = vertexes[v2_idx]

        trans_v1_x, trans_v1_z = transform_vert(v1, cam_x, cam_y, angle)
        trans_v2_x, trans_v2_z = transform_vert(v2, cam_x, cam_y, angle)

        if(trans_v1_z <= 0 or trans_v2_z <= 0):
            continue

        print("v1 x,z {},{}".format(trans_v1_x, trans_v1_z))
        print("v2 x,z {},{}".format(trans_v2_x, trans_v2_z))
        lx = int(project_x(trans_v1_x, trans_v1_z))
        rx = int(project_x(trans_v2_x, trans_v2_z))
        print("lx: {}".format(lx))
        print("rx: {}".format(rx))

        x1_ytop = project_y(CEIL_HEIGHT, trans_v1_z, cam_z)
        x1_ybot = project_y(FLOOR_HEIGHT, trans_v1_z, cam_z)
        x2_ytop = project_y(CEIL_HEIGHT, trans_v2_z, cam_z)
        x2_ybot = project_y(FLOOR_HEIGHT, trans_v2_z, cam_z)

        if (lx > window_max):
            print("lx too big")
            continue

        if (rx < window_min):
            print("rx too small")
            continue

        if (lx >= rx):
            print("lx and rx are reversed")
            continue

        print("DRAWING!!!!")

        cur_ytop = x1_ytop
        cur_ybot = x1_ybot

        dx = rx-lx

        dy_top_per_dx = (x2_ytop - x1_ytop) / dx
        dy_bot_per_dx = (x2_ybot - x1_ybot) / dx
        print("drawing from {} to {}".format(lx, rx))
        if lx < window_min:
            skip_x = window_min - lx
            cur_ytop += (dy_top_per_dx * skip_x)
            cur_ybot += (dy_bot_per_dx * skip_x)
            lx = window_min

        for x in range(lx, rx):
            draw_col(col, x, cur_ytop, cur_ybot)
            cur_ytop += dy_top_per_dx
            cur_ybot += dy_bot_per_dx


def draw_col(color: (int,int,int), x: int, ytop: int, ybot: int):
    print("draw x: {}, {} to {}".format(x, ytop, ybot))
    ytop = int(max(0, ytop))
    ybot = int(min(RENDER_HEIGHT-1, ybot))
    renderer.draw_line((x, ytop, x, ybot), color)
    #sdl2.ext.line(surf, color, (x, ytop, x, ybot))






def transform_vert(vert, cx, cy, cang):
    x,y = vert
    tlx = x - cx
    tly = y - cy

    rad_ang = math.radians(cang)
    sin_ang = math.sin(rad_ang)
    cos_ang = math.cos(rad_ang)

    rx = (tlx * sin_ang) - (tly * cos_ang)
    ry = (tlx * cos_ang) + (tly * sin_ang)

    return (rx,ry)

def project_x(x, z):
    return 128 + (x / z)

ASPECT_RATIO = (RENDER_WIDTH / RENDER_HEIGHT)
CONST1 = RENDER_WIDTH/2
CONST2 = ((RENDER_WIDTH/2) * SCALE / min(1, ASPECT_RATIO))
CONST3 = (RENDER_HEIGHT/2)
CONST4 = (RENDER_HEIGHT/2 * SCALE / max(1, ASPECT_RATIO))

def project_y(y, z, player_z):
    rel_y = y - player_z

    scaled_y = (rel_y / z) * CONST4

    adj_y = CONST4 + scaled_y

    return (RENDER_HEIGHT-1) - adj_y




