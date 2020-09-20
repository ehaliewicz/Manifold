#include <genesis.h>
#include "automap.h"
#include "game.h"
#include "level.h"


u8* processed_linedef_bitmap;


void init_processed_linedef_bitmap() {
    int num_linedefs = cur_level->num_linedefs;
    processed_linedef_bitmap = MEM_alloc(num_linedefs/8+1);
}

void clear_linedef_processed_bitmap() {
    int num_linedefs = cur_level->num_linedefs;
    memset(processed_linedef_bitmap, 0, num_linedefs/8+1);
}

void cleanup_automap() {
    MEM_free(processed_linedef_bitmap);
}


Vect2D_s16 transform_vert_native(s16 x, s16 y);
  
Vect2D_s16 inner_transform_vert(s16 x, s16 y) {
    return transform_vert_native(x, y);

    //s16 x_shifted = ((x<<4)/ZOOM); //>>ZOOM_SHIFT);
    //s16 y_shifted = ((y<<4)/ZOOM); //ZOOM_SHIFT);
    s16 x_shifted = x;
    s16 y_shifted = y;
    
    s16 tlx = x_shifted - playerXFrac4;
    s16 tly = y_shifted - playerYFrac4; // scaling factor of 64 (2^6)
    
    fix32 rx = ((tlx)*(angleCosFrac12)) - ((tly)*(angleSinFrac12)); // 12.4 * 1.12 = 13.16? result I know it's 16 bits of fractional precision
    fix32 ry = ((tlx)*(angleSinFrac12)) + ((tly)*(angleCosFrac12));


    s16 trx = ((rx>>16) + (BMP_WIDTH>>1));
    //s16 try = (int_max_y)-((ry>>16) + (BMP_HEIGHT>>1));
    s16 try = ((ry>>16) + (BMP_HEIGHT>>1))-(159);
    Vect2D_s16 vert = {.x = trx, .y = -try};

    
    return vert;
}

Vect2D_s16 inner_transform_vert_with_shift(s16 x, s16 y) {

    s16 x_shifted = ((x<<4)/ZOOM); //>>ZOOM_SHIFT);
    s16 y_shifted = ((y<<4)/ZOOM); //ZOOM_SHIFT);
    //s16 x_shifted = x;
    //s16 y_shifted = y;
    
    s16 tlx = x_shifted - playerXFrac4;
    s16 tly = y_shifted - playerYFrac4; // scaling factor of 64 (2^6)
    
    fix32 rx = ((tlx)*(angleCosFrac12)) - ((tly)*(angleSinFrac12)); // 12.4 * 1.12 = 13.16? result I know it's 16 bits of fractional precision
    fix32 ry = ((tlx)*(angleSinFrac12)) + ((tly)*(angleCosFrac12));


    s16 trx = ((rx>>16) + (BMP_WIDTH>>1));
    //s16 try = (int_max_y)-((ry>>16) + (BMP_HEIGHT>>1));
    s16 try = ((ry>>16) + (BMP_HEIGHT>>1))-(159);
    Vect2D_s16 vert = {.x = trx, .y = -try};

    
    return vert;
}

#define WALL_COL 0x22
#define PORTAL_COL 0x11


void draw_blockmap_cell_native(Line* l, u16* cell_ptr, u32 cur_frame);



void draw_blockmap_cell(u16* cell_ptr) {
    Line lin;

    u16 num_linedefs = *cell_ptr++;

    for(u16 i = 0; i < num_linedefs; i++) {
        u16 linedef_byte_idx = *cell_ptr++;    
        u8 byte = processed_linedef_bitmap[linedef_byte_idx];
        u16 bit_mask_and_is_portal = *cell_ptr++;
        u8 bit_pos = bit_mask_and_is_portal>>8;
        u8 bit_mask = (1 << bit_pos);
        //u8 bit_mask = bit_mask_and_is_portal>>8;
        if(byte == 0 || ((byte & bit_mask) == 0)) {
            processed_linedef_bitmap[linedef_byte_idx] |= bit_mask;
        } else {
        //if(byte == 0xFF || byte & bit_mask) {
            // skip v1_index, v1x, v1y
            cell_ptr += 3;
            // skip v2_index, v2x, v2y
            cell_ptr += 3;

            continue;
        }

        u8 col = bit_mask_and_is_portal&0xFF; 
        u16 v1 = *cell_ptr++;
        
        Vect2D_s16 tv1;

        u16 v1x = *cell_ptr++;
        u16 v1y = *cell_ptr++;
        u16 v2 = *cell_ptr++;

        Vect2D_s16 tv2;

        u16 v2x = *cell_ptr++;
        u16 v2y = *cell_ptr++;

        tv1 = inner_transform_vert(v1x, v1y);
        tv2 = inner_transform_vert(v2x, v2y);

        //last_v2 = v2;
        //last_v1 = v1;

        //u8 col = is_portal ? PORTAL_COL : WALL_COL;

        lin.pt1.x = tv1.x; 
        lin.pt1.y = tv1.y;
        lin.pt2.x = tv2.x;
        lin.pt2.y = tv2.y;
        lin.col = col;


        if(BMP_clipLine(&lin)) {
            //lines_onscreen++;
            //verts_cached+=2;
            //verts_drawn+=2;
            //vertex_transform_cache[v1].cached_frame = cur_frame;
            //vertex_transform_cache[v1].transformed_vert = tv1;
            //vertex_transform_cache[v2].cached_frame = cur_frame;
            //vertex_transform_cache[v2].transformed_vert = tv2;
            //cache_transformed_vertex(v1, tv1);
            //cache_transformed_vertex(v2, tv2);

            BMP_drawLine(&lin);
            
        }
        
    }

}

const Vect2D_s16 playerPolygon[3] = {
    {.x = (BMP_WIDTH-1)/2,     .y = ((BMP_HEIGHT-1)/2)-2},
    {.x = ((BMP_WIDTH-1)/2)+1, .y = ((BMP_HEIGHT-1)/2)+1},
    {.x = ((BMP_WIDTH-1)/2)-1, .y = ((BMP_HEIGHT-1)/2)+1},
};
    


void draw_automap(u32 cur_frame) {
    clear_linedef_processed_bitmap();
    BMP_waitWhileFlipRequestPending();
    BMP_clear();


    // inverse project coordinates

    // inverse project window corners and figure out the min/max x/y blockmap coordinates to render
    
    //const blockmap* blkmap = cur_level->blkmap;
    const render_blockmap* render_blkmap = cur_level->render_blkmap;
    int blockmap_min_x = render_blkmap->x_origin;
    int blockmap_min_y = render_blkmap->y_origin;
    //int blockmap_min_x = big_blkmap->x_origin;
    //int blockmap_min_y = big_blkmap->y_origin;

    //int blockmap_max_x = blockmap_min_x + (blkmap->num_columns*BLOCKMAP_CELL_SIZE);
    //int blockmap_max_y = blockmap_min_y + (blkmap->num_rows*BLOCKMAP_CELL_SIZE);
    

    /* calculate world-space coordinates for all four corners of the map screen 


    the angle from the center of the map to the top and left corners is 58 degrees

    calculate the x and y components of those vectors, scale by half the diagonal length of the screen, as well as the world->screen zoom factor

    mirror the top left vector to get the bottom right vector, and the top right vector to get the bottom left vector

    from this point, we can easily find the min and max x and y blockmap coordinates to iterate over.

    */
    
    int world_half_screen_diag_length = 152 * ZOOM; //<< ZOOM_SHIFT;

    fix32 top_left_angle = (cur_player_angle-ANGLE_58_DEGREES);
    fix32 top_left_angle_sin = sinFix32(top_left_angle);
    fix32 top_left_angle_cos = cosFix32(top_left_angle);

    fix32 top_left_x_off = top_left_angle_sin * world_half_screen_diag_length;
    fix32 top_left_y_off = top_left_angle_cos * world_half_screen_diag_length;

    fix32 world_screen_top_left_x = cur_player_x + top_left_x_off;
    fix32 world_screen_top_left_y = cur_player_y + top_left_y_off;

    fix32 world_screen_bottom_right_x = cur_player_x - top_left_x_off;
    fix32 world_screen_bottom_right_y = cur_player_y - top_left_y_off;


    fix32 top_right_angle = (cur_player_angle+ANGLE_58_DEGREES);
    fix32 top_right_angle_sin = sinFix32(top_right_angle);
    fix32 top_right_angle_cos = cosFix32(top_right_angle);

    fix32 top_right_x_off = top_right_angle_sin * world_half_screen_diag_length;
    fix32 top_right_y_off = top_right_angle_cos * world_half_screen_diag_length;

    fix32 world_screen_top_right_x = cur_player_x + top_right_x_off;
    fix32 world_screen_top_right_y = cur_player_y + top_right_y_off;

    fix32 world_screen_bottom_left_x = cur_player_x - top_right_x_off;
    fix32 world_screen_bottom_left_y = cur_player_y - top_right_y_off;

    

    fix32 min_world_x = min(min(world_screen_top_left_x, world_screen_top_right_x),
                            min(world_screen_bottom_left_x, world_screen_bottom_right_x));

    fix32 max_world_x = max(max(world_screen_top_left_x, world_screen_top_right_x),
                            max(world_screen_bottom_left_x, world_screen_bottom_right_x));


    fix32 min_world_y = min(min(world_screen_top_left_y, world_screen_top_right_y),
                            min(world_screen_bottom_left_y, world_screen_bottom_right_y));

    fix32 max_world_y = max(max(world_screen_top_left_y, world_screen_top_right_y),
                            max(world_screen_bottom_left_y, world_screen_bottom_right_y));



    int min_blockmap_x = fix32ToRoundedInt(min_world_x-intToFix32(blockmap_min_x))/BLOCKMAP_CELL_SIZE;
    int max_blockmap_x = fix32ToRoundedInt(max_world_x-intToFix32(blockmap_min_x))/BLOCKMAP_CELL_SIZE;
    int min_blockmap_y = fix32ToRoundedInt(min_world_y-intToFix32(blockmap_min_y))/BLOCKMAP_CELL_SIZE;
    int max_blockmap_y = fix32ToRoundedInt(max_world_y-intToFix32(blockmap_min_y))/BLOCKMAP_CELL_SIZE;

    int y_start = max(0, min_blockmap_y);
    int y_end = min(max_blockmap_y, render_blkmap->num_rows-1);
    int x_start = max(0, min_blockmap_x);
    int x_end = min(max_blockmap_x, render_blkmap->num_columns-1);

    int num_cols = render_blkmap->num_columns;
    int blockmap_y_off = y_start * num_cols;
    for(int y = y_start; y <= y_end; y++) {
        for(int x = x_start; x <= x_end; x++) {


            int blockmap_offset_idx = blockmap_y_off+x;
            
            int blockmap_table_idx = render_blkmap->offsets_plus_table[blockmap_offset_idx];
            if(blockmap_table_idx == 0) {
                continue;
            }

            const u16* cell_ptr =  &(render_blkmap->offsets_plus_table[blockmap_table_idx]);

            Line l;
            draw_blockmap_cell_native(&l, cell_ptr, cur_frame);
            //draw_blockmap_cell(cell_ptr);
        }
        
        blockmap_y_off += num_cols;

    }

    linedef line0 = cur_level->linedefs[0];
    u8 line0_is_portal = line0.left_sidedef != 0xFFFF && line0.right_sidedef != 0xFFFF;

    u16 line0_v1 = line0.v1;
    u16 line0_v1_x = (cur_level->vertexes[line0_v1].x<<4)/ZOOM;
    u16 line0_v1_y = (cur_level->vertexes[line0_v1].y<<4)/ZOOM;

    u16 line0_v2 = line0.v2;
    u16 line0_v2_x = (cur_level->vertexes[line0_v2].x<<4)/ZOOM;
    u16 line0_v2_y = (cur_level->vertexes[line0_v2].y<<4)/ZOOM;

    Vect2D_s16 l0tv1 = inner_transform_vert(line0_v1_x, line0_v1_y);
    Vect2D_s16 l0tv2 = inner_transform_vert(line0_v2_x, line0_v2_y);

    Line lin;
    lin.pt1 = l0tv1;
    lin.pt2 = l0tv2;
    lin.col = line0_is_portal ? PORTAL_COL : WALL_COL;
    if(BMP_clipLine(&lin)) { BMP_drawLine(&lin); }


    lin.col = 0xFF;
   // draw border
    memset(bmp_buffer_write, 0xFF, 128);
    memset(bmp_buffer_write+((BMP_HEIGHT-1)*128), 0xFF, 128);
    lin.pt1.x = 0; lin.pt1.y = 0;
    lin.pt2.x = 0; lin.pt2.y = (BMP_HEIGHT-1);
    BMP_drawLine(&lin);
    lin.pt1.x = (BMP_WIDTH-1); lin.pt1.y = 0;
    lin.pt2.x = (BMP_WIDTH-1); lin.pt2.y = (BMP_HEIGHT-1);
    BMP_drawLine(&lin);
    
    
    BMP_drawPolygon(playerPolygon, 3, 0x11);


    BMP_showFPS(1);
    



    BMP_flip(1);

    //u8* bmp = BMP_getWritePointer(0, 0);
    
    //DMA_doDma(DMA_VRAM, bmp, BMP_FB0TILE, (BMP_FB0ENDTILEINDEX-BMP_FB0TILE)/2, 4);
    //DMA_doDma(DMA_VRAM, bmp, BMP_FB0TILE+(2*32), (BMP_FB0ENDTILEINDEX-BMP_FB0TILE)/2, 4);
}