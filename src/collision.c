#include <genesis.h>
#include "game.h"
#include "collision.h"
#include "level.h"

fix16 vxs16(fix16 x0, fix16 y0, fix16 x1, fix16 y1) {
    return fix16Mul(x0, y1) - fix16Mul(x1, y0);
}

fix32 vxs32(fix32 x0, fix32 y0, fix32 x1, fix32 y1) {
    return fix32Mul(x0, y1) - fix32Mul(x1, y0);
}

#define PointSide16(px,py, x0,y0, x1,y1) vxs16((x1)-(x0), (y1)-(y0), (px)-(x0), (py)-(y0))
#define PointSide32(px,py, x0,y0, x1,y1) vxs32((x1)-(x0), (y1)-(y0), (px)-(x0), (py)-(y0))

collision_result check_for_collision(player_pos cur_position, player_pos new_position) {

    fix32 oldx = cur_position.x;
    fix32 oldy = cur_position.y;
    u16 old_sector = cur_position.cur_sector;
    
    fix32 curx = oldx;
    fix32 cury = oldy;
    u16 cur_sector = cur_position.cur_sector;

    fix32 newx = new_position.x;
    u16 new_sector = cur_sector;

    int got_x_collision = 0;

    collision_type coll_type = NO_COLLISION;

    int num_walls = sector_num_walls(cur_sector, cur_portal_map);
    int wall_offset = sector_wall_offset(cur_sector, cur_portal_map);

    for(int i = 0; i < num_walls; i++) {
        int v1_idx = cur_portal_map->walls[wall_offset+i];
        int v2_idx = cur_portal_map->walls[(i == num_walls-1) ? (wall_offset+i+1) : (wall_offset)];
        vertex v1 = cur_portal_map->vertexes[v1_idx];
        vertex v2 = cur_portal_map->vertexes[v2_idx];

        fix32 oldside = PointSide32((curx), (cury),
                                    intToFix32(v1.x), intToFix32(v1.y), 
                                    intToFix32(v2.x), intToFix32(v2.y));
        fix32 newside = PointSide32((newx), (cury), 
                                    intToFix32(v1.x), intToFix32(v1.y), 
                                    intToFix32(v2.x), intToFix32(v2.y));
        int signold = (oldside < 0 ? -1 : oldside == 0 ? 0 : 1);
        int signnew = (newside < 0 ? -1 : oldside == 0 ? 0 : 1);
        int portal_sector = cur_portal_map->portals[wall_offset+i];
        int is_portal = (portal_sector != -1);
        if(signold != signnew) {
            if(is_portal) {
                //if(player_collides_vertically(portal_sector)) {
                //    got_x_collision = 1;
                //    break;
                //}
                coll_type = SECTOR_TRANSITION;
                new_sector = portal_sector;
                break;
            } else {
                got_x_collision = 1;
                coll_type = COLLIDED;
                break;
            }
        }
    }

    if(!got_x_collision) {
        //moved = 1;
        curx = newx;
    }

    fix32 newy = new_position.y;
    int got_y_collision = 0;

    for(int i = 0; i < num_walls; i++) {
        int v1_idx = cur_portal_map->walls[wall_offset+i];
        int v2_idx = cur_portal_map->walls[(i == num_walls-1) ? (wall_offset+i+1) : (wall_offset)];
        vertex v1 = cur_portal_map->vertexes[v1_idx];
        vertex v2 = cur_portal_map->vertexes[v2_idx];

        fix32 oldside = PointSide32((curx), (cury), 
                                    intToFix32(v1.x), intToFix32(v1.y), 
                                    intToFix32(v2.x), intToFix32(v2.y));
        fix32 newside = PointSide32((curx), (newy), 
                                    intToFix32(v1.x), intToFix32(v1.y), 
                                    intToFix32(v2.x), intToFix32(v2.y));
        int signold = (oldside < 0 ? -1 : oldside == 0 ? 0 : 1);
        int signnew = (newside < 0 ? -1 : oldside == 0 ? 0 : 1);
        int portal_sector = cur_portal_map->portals[wall_offset+i];
        int is_portal = (portal_sector != -1);
        if(signold != signnew) {
            if(is_portal) {
                //if(player_collides_vertically(portal_sector)) {
                //    got_x_collision = 1;
                //    break;
                //}
                coll_type = SECTOR_TRANSITION;
                new_sector = portal_sector;
                break;
            } else {
                coll_type = COLLIDED;
                got_y_collision = 1;
                break;
            }
        }
    }

    if(!got_y_collision) {
        //moved = 1;
        cury = newy;
    }

    //ply.where.x = curx;
    //ply.where.y = cury;

    collision_result res;
    res.type = coll_type;
    res.new_player_pos.x = newx; 
    res.new_player_pos.y = newy;
    res.new_player_pos.z = cur_position.z;
    res.new_player_pos.ang = new_position.ang;
    res.new_player_pos.cur_sector = new_sector;

    if(new_sector != old_sector) {
        // if we've moved, check for new sector

        //ply.where.z = ply.cur_sector->floor_height + eye_height;
    }
    return res;

}