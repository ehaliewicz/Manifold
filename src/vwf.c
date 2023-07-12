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

// render a string into 8x8 tiles, return number of characters processed
int vwf_render_tiles(char* string, int len, tile* tiles, int num_tiles) {

  int tile_num = 0;
  int pos_in_tile = 0;
  
  tile* cur_tile = tiles;
  
  for(int i = 0; i < len; i++) {
    char c = string[i];
    const char_entry* centr = &(charmap[c-32]);
    
    int char_width = centr->width;
    tile til = centr->bitmap;

    if(tile_num == num_tiles-1 && pos_in_tile + char_width > PAIRS_IN_TILE) {
      // can't render this, return number of characters rendered
      return i;
    }

    // render 1 byte, 2 pixel pair at once
    for(int pair_x = 0; pair_x < char_width; pair_x++) {
      for(int y = 0; y < 8; y++) {
        // clear only pixels with font
        //cur_tile->bytes[y][pos_in_tile] &= til.bytes[y][pair_x];
        //cur_tile->bytes[y][pos_in_tile] |= til.bytes[y][pair_x];

        cur_tile->bytes[y][pos_in_tile] = til.bytes[y][pair_x];
      }
      pos_in_tile += 1;
    
      if (pos_in_tile == PAIRS_IN_TILE) {
        tile_num += 1;
        cur_tile++;
        pos_in_tile = 0;
      }
    }
  
  }
  
  return len;
}

int vwf_render_to_separate_tiles(char* string, int len, tile* tiles, int num_tiles) {
 
  int tile_num = 0;
  int pos_in_tile = 0;
  
  tile* cur_tile = tiles;
  
  for(int i = 0; i < len; i++) {
    char c = string[i];
    const char_entry* centr = &(charmap[c-32]);
    
    int char_width = centr->width;
    tile til = centr->bitmap;

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
