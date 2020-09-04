#include <genesis.h>
#include "game_mode.h"
#include "joy_helper.h"
#include "level.h"

fix32 cur_player_x, cur_player_y;
fix32 cur_player_angle;


void init_game() {
    BMP_init(0, BG_A, PAL0, 0);
    cur_player_x = intToFix32(cur_level->things[0].x);
    cur_player_y = intToFix32(cur_level->things[0].y);
    cur_player_angle = 0;
}

game_mode run_game() {
    BMP_waitWhileFlipRequestPending();
    BMP_clear();

    const fix32 move_speed = FIX32(100);
    if(joy_button_held(BUTTON_DOWN)) {
        cur_player_y += move_speed;
    } else if (joy_button_held(BUTTON_UP)) {
        cur_player_y -= move_speed;
    } else if (joy_button_held(BUTTON_LEFT)) {
        cur_player_angle += 10;
    } else if (joy_button_held(BUTTON_RIGHT)) {
        cur_player_angle -= 1;
    }

    /*
    for(int i = 0; i < cur_level->num_vertexes; i++) {
        vertex v = cur_level->vertexes[i];
        s16 tvx = intToFix32(v) - cur_player_x
    }
    */
    
    Line l = {.col = 0xFF};

    for(int i = 0; i < cur_level->num_segs; i++) {
        seg cur_seg = cur_level->segs[i];
        vertex v1 = cur_level->vertexes[cur_seg.begin_vert];
        vertex v2 = cur_level->vertexes[cur_seg.end_vert];

        fix32 tl1x = (intToFix32(v1.x) - cur_player_x);
        fix32 tl1y = (intToFix32(v1.y) - cur_player_y);
        fix32 tl2x = (intToFix32(v2.x) - cur_player_x);
        fix32 tl2y = (intToFix32(v2.y) - cur_player_y);

        fix32 s = sinFix32(cur_player_angle);
        fix32 c = cosFix32(cur_player_angle);
        
        fix32 r1x = fix32Mul(tl1x, c) - fix32Mul(tl1y, s);
        fix32 r1y = fix32Mul(tl1x, s) + fix32Mul(tl1y, c);
        fix32 r2x = fix32Mul(tl2x, c) - fix32Mul(tl2y, s);
        fix32 r2y = fix32Mul(tl2x, s) + fix32Mul(tl2y, c);

        s16 tr1x = fix32ToInt(r1x>>3 + intToFix32(BMP_WIDTH/2));
        s16 tr1y = fix32ToInt(r1y>>3 + intToFix32(BMP_HEIGHT/2));
        s16 tr2x = fix32ToInt(r2x>>3 + intToFix32(BMP_WIDTH/2));
        s16 tr2y = fix32ToInt(r2y>>3 + intToFix32(BMP_HEIGHT/2));
        //s16 tr1x = fix32ToInt(tl1x) + BMP_WIDTH/2;
        //s16 tr1y = fix32ToInt(tl1y) + BMP_HEIGHT/2;
        //s16 tr2x = fix32ToInt(tl2x) + BMP_WIDTH/2;
        //s16 tr2y = fix32ToInt(tl2y) + BMP_HEIGHT/2;

        /*        
        if(tr1x > tr2x) {
            SWAP_s16(tr1x,tr2x);
            SWAP_s16(tr1y,tr2y);
        }
        if(tr2x <= 0 || tr1x >= BMP_WIDTH) {
            continue;
        }
        if((tr1y < 0 && tr2y < 0) ||
           (tr1y >= BMP_HEIGHT && tr2y >= BMP_HEIGHT)) {
               continue;
        }
        */

        l.pt1.x = tr1x; l.pt1.y = tr1y;
        l.pt2.x = tr2x; l.pt2.y = tr2y;


        if(BMP_clipLine(&l)) {
            BMP_drawLine(&l);
        }
    }
    
    
    BMP_showFPS(1);
    BMP_flip(1);
    return SAME_MODE;
}

void cleanup_game() {
    BMP_end();
    MEM_pack();
}