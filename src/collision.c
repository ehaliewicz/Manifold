#include <genesis.h>
#include "game.h"
#include "collision.h"
#include "level.h"

fix32 vxs16(fix16 x0, fix16 y0, fix16 x1, fix16 y1) {
    return x0*y1 - x1*y0;
}

fix32 vxs32(fix32 x0, fix32 y0, fix32 x1, fix32 y1) {
    return fix32Mul(x0, y1) - fix32Mul(x1, y0);
}

#define PointSide16(px,py, x0,y0, x1,y1) vxs16((x1)-(x0), (y1)-(y0), (px)-(x0), (py)-(y0))
#define PointSide32(px,py, x0,y0, x1,y1) vxs32((x1)-(x0), (y1)-(y0), (px)-(x0), (py)-(y0)) 

#define Overlap(a0,a1,b0,b1) (min(a0,a1) <= max(b0,b1) && min(b0,b1) <= max(a0,a1))
#define IntersectBox(x0,y0, x1,y1, x2,y2, x3,y3) (Overlap(x0,x1,x2,x3) && Overlap(y0,y1,y2,y3))

s8 point_sign_int_vert(fix32 x, fix32 y, s16 v1_x, s16 v1_y, s16 v2_x, s16 v2_y) {
    fix32 left = (v2_x-v1_x)*(y-intToFix32(v1_y));
    fix32 right = (v2_y-v1_y)*(x-intToFix32(v1_x));
    fix32 res = left - right;
    return (res > 0 ? 1 : (res < 0 ? -1 : 0));
}

int within_sector(fix32 x, fix32 y, u16 sector) {
    u16 wall_off = sector_wall_offset(sector, (portal_map*)cur_portal_map);
    u16 num_walls = sector_num_walls(sector, (portal_map*)cur_portal_map);

    int sign = 0;
    int got_sign = 0;


    for(int i = 0; i < num_walls; i++) {
        u16 wall_idx = wall_off+i;
        u16 v1_idx = cur_portal_map->walls[wall_idx];
        u16 v2_idx = cur_portal_map->walls[wall_idx+1];
        vertex v1 = cur_portal_map->vertexes[v1_idx];
        vertex v2 = cur_portal_map->vertexes[v2_idx];
        int cur_sign = point_sign_int_vert(x, y, v1.x, v1.y, v2.x, v2.y);
        if(got_sign) {
         if(sign != cur_sign) { return 0; }
         
        } else {
            sign = cur_sign;
            got_sign = 1;
        }
    }

    return 1;
}

fix32 sq_shortest_dist_to_point(fix32 px, fix32 py, vertex a, vertex b) {
    s16 dx = b.x - a.x;
    s16 dy = b.y - a.y;
    s32 dr2 = (dx * dx + dy * dy); // 32.0
    fix32 fixAx = intToFix32(a.x); // 22.10
    fix32 fixAy = intToFix32(a.y);

    fix32 lerp = ((px - fixAx) * dx + (py - fixAy) * dy) / dr2; // 22.10 fractional bits here
    if (lerp < 0) {
        lerp = 0;
    } else if (lerp > FIX32(1)) {
        lerp = FIX32(1);
    }

    fix32 testX = lerp * dx + fixAx;
    fix32 testY = lerp * dy + fixAy;

    fix32 _dx = testX - px;
    fix32 _dy = testY - py;
    fix32 square_dist = fix32Mul(_dx,_dx) + fix32Mul(_dy,_dy);
    return square_dist>>FIX32_FRAC_BITS;
}

int within_sector_radius(fix32 x, fix32 y, u16 radius, u16 sector) {
    u16 wall_off = sector_wall_offset(sector, (portal_map*)cur_portal_map);
    u16 portal_off = sector_portal_offset(sector, (portal_map*)cur_portal_map);
    u16 num_walls = sector_num_walls(sector, (portal_map*)cur_portal_map);

    int maybe_inside = within_sector(x, y, sector);
    if(!maybe_inside) { return 0; }


    //s32 dmax;
    //s32 dmin;
    //int got_dist = 0;

    s32 radius_sqr = radius*radius;

    s32 xInt = fix32ToInt(x);
    s32 yInt = fix32ToInt(y);

    for(int i = 0; i < num_walls-1; i++) {
        u16 vert_idx = wall_off+i;
        u16 v_idx = cur_portal_map->walls[vert_idx];
        vertex v = cur_portal_map->vertexes[v_idx];
        s32 distsqr = ((v.x - xInt)*(v.x - xInt)) + ((v.y - yInt)*(v.y - yInt));
        if(radius_sqr > distsqr) { return 0; }
        //if(!got_dist) {
        //    dmin = distsqr;
        //    got_dist = 1;
        //} else {
        //    //dmax = max(distsqr, dmax);
        //    dmin = min(distsqr, dmin);
        //}
    }

     
    //if(radius_sqr > dmin) { return 0; }

    // otherwise, it might be inside.

    // now check against walls

    // if the wall is a portal, skip it
    for(int i = 0; i < num_walls; i++) {
        u16 wall_idx = wall_off+i;
        u16 portal_idx = portal_off+i;
        s16 portal_sect = cur_portal_map->portals[portal_idx];
        if(portal_sect != -1) {
            continue;
        }
        u16 v1_idx = cur_portal_map->walls[wall_idx];
        u16 v2_idx = cur_portal_map->walls[wall_idx+1];
        vertex v1 = cur_portal_map->vertexes[v1_idx];
        vertex v2 = cur_portal_map->vertexes[v2_idx];
        fix32 dst = sq_shortest_dist_to_point(x, y, v1, v2);
        if(radius_sqr > dst) {
            return 0;
        }
    }

    return 1;

}


collision_result check_for_collision(fix32 curx, fix32 cury, fix32 newx, fix32 newy, u16 cur_sector) {
    if(within_sector(newx, cury, cur_sector)) {
        // no x collision, just move x 
        curx = newx;
    } else {

        // check if traversed to new sector
        u16 portal_off = sector_portal_offset(cur_sector, (portal_map*)cur_portal_map);
        u16 num_walls = sector_num_walls(cur_sector, (portal_map*)cur_portal_map);
        for(int i = 0; i < num_walls; i++) {
            u16 portal_idx = portal_off+i;
            s16 portal_sect = cur_portal_map->portals[portal_idx];
            if(portal_sect == -1) {
                continue;
            }

            if(within_sector(newx, cury, portal_sect)) {
                cur_sector = portal_sect;
                curx = newx;
                break;
            }
        }
        // if we didn't move to a new sector, curx hasn't been changed

    }

    if(within_sector(curx, newy, cur_sector)) {
        cury = newy;
    } else {
        // check if traversed to new sector
        u16 portal_off = sector_portal_offset(cur_sector, (portal_map*)cur_portal_map);
        u16 num_walls = sector_num_walls(cur_sector, (portal_map*)cur_portal_map);
        for(int i = 0; i < num_walls; i++) {
            u16 portal_idx = portal_off+i;
            s16 portal_sect = cur_portal_map->portals[portal_idx];
            if(portal_sect == -1) {
                continue;
            }

            if(within_sector(curx, newy, portal_sect)) {
                cur_sector = portal_sect;
                cury = newy;
                break;
            }
        }
        // if we didn't move to a new sector, curx hasn't been changed
    }

    collision_result res;
    res.new_sector = cur_sector;
    res.pos.x = curx;
    res.pos.y = cury;

    return res;

}

collision_result check_for_collision_radius(fix32 curx, fix32 cury, fix32 newx, fix32 newy, u16 radius, u16 cur_sector) {
    if(within_sector_radius(newx, cury, radius, cur_sector)) {
        // no x collision, just move x 
        curx = newx;
    } else {

        // check if traversed to new sector
        u16 portal_off = sector_portal_offset(cur_sector, (portal_map*)cur_portal_map);
        u16 num_walls = sector_num_walls(cur_sector, (portal_map*)cur_portal_map);
        for(int i = 0; i < num_walls; i++) {
            u16 portal_idx = portal_off+i;
            s16 portal_sect = cur_portal_map->portals[portal_idx];
            if(portal_sect == -1) {
                continue;
            }

            if(within_sector_radius(newx, cury, radius, portal_sect)) {
                cur_sector = portal_sect;
                curx = newx;
                break;
            }
        }
        // if we didn't move to a new sector, curx hasn't been changed

    }

    if(within_sector_radius(curx, newy, radius, cur_sector)) {
        cury = newy;
    } else {
        // check if traversed to new sector
        u16 portal_off = sector_portal_offset(cur_sector, (portal_map*)cur_portal_map);
        u16 num_walls = sector_num_walls(cur_sector, (portal_map*)cur_portal_map);
        for(int i = 0; i < num_walls; i++) {
            u16 portal_idx = portal_off+i;
            s16 portal_sect = cur_portal_map->portals[portal_idx];
            if(portal_sect == -1) {
                continue;
            }

            if(within_sector_radius(curx, newy, radius, portal_sect)) {
                cur_sector = portal_sect;
                cury = newy;
                break;
            }
        }
        // if we didn't move to a new sector, curx hasn't been changed
    }

    collision_result res;
    res.new_sector = cur_sector;
    res.pos.x = curx;
    res.pos.y = cury;

    return res;

}


u16 find_sector(object_pos cur_player_pos) {
    // test current sector first
    
    u16 orig_sector = cur_player_pos.cur_sector;
    if(within_sector(cur_player_pos.x, cur_player_pos.y, orig_sector)) {
        return orig_sector;
    }

    // otherwise test connected sectors
    u16 orig_sector_num_walls = sector_num_walls(orig_sector, (portal_map*)cur_portal_map);
    u16 portal_offset = sector_portal_offset(orig_sector, (portal_map*)cur_portal_map);
    for(int i = 0; i < orig_sector_num_walls; i++) {
        s16 dest_sector = cur_portal_map->portals[portal_offset+i];
        if(dest_sector == -1) {
            continue;
        }
        if(within_sector(cur_player_pos.x, cur_player_pos.y, dest_sector)) {
            return dest_sector;
        }
    }    
    
    //for(int i = 0; i < cur_portal_map->num_sectors; i++) {
    //    if(in_sector(cur_player_pos, i)) {
    //        return i;
    //    }
    //}

    return cur_player_pos.cur_sector;

}
