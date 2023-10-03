#include "utils.h"
#include "vwf.h"

#define DIVIDE_ROUND_UP(a,b) (((a) + ((b)-1))/(b))



void vwf_init() {
}

void vwf_cleanup() {
}

int vwf_count_tiles(char* string, int len) {
  int num_pairs = 0;
  for(int i = 0; i < len; i++) {
    char c = string[i];
    int cur_num_pairs = charmap[c-32].width;
    if(cur_num_pairs == 0 || cur_num_pairs > PAIRS_IN_TILE) { 
      
      char buf[32]; sprintf(buf, "wtf %i/%c %i", c, c, cur_num_pairs);
      KLog(buf);
      die(buf); } //  die("wtf"); }
    

    num_pairs += cur_num_pairs;
  }
  int int_tiles = num_pairs / PAIRS_IN_TILE;
  if ((int_tiles * PAIRS_IN_TILE) != num_pairs) {
    int_tiles++;
  }
  return int_tiles;

  //return DIVIDE_ROUND_UP(num_pairs, PAIRS_IN_TILE);
}


u8 dummy_col[9] = {
  0b0000000,
  0b0000000,
  0b0000000,
  0b0000000,
  0b0000000,
  0b0000000,
  0b0000000,
  0b0000000,
  0b0000000,
};

const u8 multicolor_shadow_lut[256] = {
  0,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,224,
238,238,238,238,238,238,238,238,238,238,238,238,238,238,238,224,
238,238,238,238,238,238,238,238,238,238,238,238,238,238,238,224,
238,238,238,238,238,238,238,238,238,238,238,238,238,238,238,224,
238,238,238,238,238,238,238,238,238,238,238,238,238,238,238,224,
238,238,238,238,238,238,238,238,238,238,238,238,238,238,238,224,
238,238,238,238,238,238,238,238,238,238,238,238,238,238,238,224,
238,238,238,238,238,238,238,238,238,238,238,238,238,238,238,224,
238,238,238,238,238,238,238,238,238,238,238,238,238,238,238,224,
238,238,238,238,238,238,238,238,238,238,238,238,238,238,238,224,
238,238,238,238,238,238,238,238,238,238,238,238,238,238,238,224,
238,238,238,238,238,238,238,238,238,238,238,238,238,238,238,224,
238,238,238,238,238,238,238,238,238,238,238,238,238,238,238,224,
238,238,238,238,238,238,238,238,238,238,238,238,238,238,238,224,
238,238,238,238,238,238,238,238,238,238,238,238,238,238,238,224,
238,238,238,238,238,238,238,238,238,238,238,238,238,238,238,
};

// render a string into 8x8 tiles, return number of characters processed
static int cnt;
int vwf_render_tiles(char* string, int len, tile* tiles, int num_tiles) {
  cnt++;
  if(cnt > 3) { cnt = 0; }


  int tile_num = 0;
  int pos_in_tile = 0;
  
  tile* cur_tile = tiles;
  
  int i;
  for(i = 0; i < len; i++) {
    char c = string[i];
    const char_entry* centr = &(charmap[c-32]);
    
    int char_width = centr->width;
    two_pix_col_tile til = centr->bitmap;

    if(tile_num == num_tiles-1 && pos_in_tile + char_width > PAIRS_IN_TILE) {
      // can't render this, return number of characters rendered
      break;
    }


    // render 1 byte, 2 pixel pair at once

    //this is with y then x ordering
    u8* output_ptr = &cur_tile->bytes[0][pos_in_tile];
    u8* input_ptr = til.bytes;//[0][0];
    u16 pix_mask = 0xFF;//0b00010001;


    u8* above_left_ptr = &dummy_col[0];

    for(int pair_x = 0; pair_x < char_width; pair_x++) {
      u8* col_output_ptr = output_ptr;
      u8 above_byte = 0;
      u8* col_input_ptr = input_ptr;
      u8 filled_pixel_mask; u8 cur_byte;

      // fill in top two pix
      FETCH_INC_BYTE(cur_byte, col_input_ptr);
      WRITE_BYTE_QINC_4(cur_byte, col_output_ptr);
      above_byte = cur_byte;


      for(int y = 1; y < 8; y++) {

        // clear only pixels with font

        // shadow from above 
        // the ordering of this is not good.
      
        
        // shadow for single color font
        u8 above_left_byte; //*above_left_ptr++;
        FETCH_INC_BYTE(above_left_byte, above_left_ptr);
        above_left_byte <<= 4;
        u8 shadow_pix = above_byte; //(above_byte >> 4) | (above_left_byte << 4);
        shadow_pix >>= 4;
        shadow_pix |= above_left_byte;

        //shadow_pix = multicolor_shadow_lut[shadow_pix];
        __asm volatile(
          "\t\n\
          and.w %2, %0\t\n\
          move.b (%1, %0.w), %0\t\n\
          "
          : "+d" (shadow_pix)
          : "a" (multicolor_shadow_lut), "d" (pix_mask)
        );

        FETCH_INC_BYTE(cur_byte, col_input_ptr);

        shadow_pix |= cur_byte;
        WRITE_BYTE_QINC_4(shadow_pix, col_output_ptr);

        above_byte = cur_byte;
        
        
      }

      output_ptr++;
      above_left_ptr = input_ptr;
      input_ptr += 8;

      pos_in_tile += 1;
    
      if (pos_in_tile == PAIRS_IN_TILE) {
        tile_num += 1;
        cur_tile++;
        pos_in_tile = 0;
        output_ptr = (u8*)cur_tile; //&cur_tile->bytes[0][0];
      }
    }
    
  
  }
  
  return i;
}

int vwf_render_to_separate_tiles(char* string, int len, tile* tiles, int num_tiles) {
  
  // NOTE: this no longer works as ive changed the text to be column based!
  int tile_num = 0;
  int pos_in_tile = 0;
  
  tile* cur_tile = tiles;
  
  for(int i = 0; i < len; i++) {
    char c = string[i];
    const char_entry* centr = &(charmap[c-32]);
    
    int char_width = centr->width;
    two_pix_col_tile til = centr->bitmap;

    if(tile_num == num_tiles-1 && pos_in_tile + char_width > PAIRS_IN_TILE) {
      // can't render this, return number of characters rendered
      return i;
    }

    memcpy(cur_tile->bytes, &til.bytes, sizeof(tile));
    tile_num += 1;
    cur_tile++;
  
  }
  
  return len;
}
