#include <genesis.h>
#include "colors.h"
#include "collision.h"
#include "console.h"
#include "game.h"
#include "inventory.h"
#include "level.h"
#include "object.h"
#include "portal_map.h"
#include "utils.h"

static int next_obj_id = 0; 


#define MAX_OBJECTS 64

static object* objects;
static object *free_list = NULL; 

static object** sector_lists;

static int num_sector_lists;

object* pop(object** lst) {
  object* head = *lst;
  if(head == NULL) {
    return NULL;
  }

  object* next = head->next;
  if(next != NULL) {
    next->prev = NULL;
  }


  *lst = next; // set list to next

  head->next = NULL;
  head->prev = NULL;
  return head;
}

void push_to_front(object* new_head, object** lst) {
    object* prev_head = *lst;
    new_head->next = prev_head;
    if(prev_head != NULL) {
        prev_head->prev = new_head;
    }
    *lst = new_head;
}

void push(object* new_head, object** lst) {
  object* head = *lst;
  new_head->next = head;
  if(head != NULL) {

    // this handles if we're pushing in the middle of a list
    if(head->prev != NULL) {
      new_head->prev = head->prev;
      head->prev->next = new_head;
    }

    head->prev = new_head;
  }
  *lst = new_head;
}

void swap_with_prev(object* o) {
  object* prev = o->prev;
  object* next = o->next;

  o->prev = NULL;
  o->next = o;


  if(prev != NULL) {
    object* prev_prev = prev_prev;
    o->prev = prev_prev;
    if(prev_prev != NULL) {
      prev->prev->next = o;
    }
    prev->prev = o;
    prev->next = next;
  }


  if(next != NULL) {
    next->prev = prev;
  }
} 

// setup objects
void init_object_lists(int num_sectors) {

    num_sector_lists = num_sectors;
    objects = malloc(sizeof(object)*MAX_OBJECTS, "object list");

    sector_lists = malloc(sizeof(object*)*num_sectors, "sector object lists");
    for(int i = 0; i < MAX_OBJECTS; i++) {
        objects[i].prev = (i == 0 ? NULL : &(objects[i-1]));
        objects[i].next = (i == (MAX_OBJECTS-1) ? NULL : &(objects[i+1]));
    }
    free_list = &objects[0];

    for(int i = 0; i < num_sectors; i++) {
        sector_lists[i] = NULL;
    }
} 


void clear_object_lists() {
    free(objects, "object list");
    free(sector_lists, "sector object lists");
}


const u16 col_9_spans[2] = {34,5};
const u8 col_9_texels[5] = {0x44,0x44,0x44,0x44,0x22};
const u16 col_10_spans[2] = {32,8};
const u8 col_10_texels[8] = {0x44,0x44,0x44,0x44,0x22,0x11,0x22,0x77};
const u16 col_11_spans[2] = {31,10};
const u8 col_11_texels[10] = {0x44,0x44,0x44,0x44,0x22,0x33,0x33,0x11,0x22,0xaa};     
const u16 col_12_spans[2] = {30,11};
const u8 col_12_texels[11] = {0x44,0x55,0x55,0x55,0x44,0x11,0x11,0x11,0x11,0x22,0xaa};
const u16 col_13_spans[2] = {30,11};
const u8 col_13_texels[11] = {0x55,0xaa,0xaa,0x55,0x44,0x33,0x11,0x11,0x33,0x44,0x77};
const u16 col_14_spans[2] = {29,12};
const u8 col_14_texels[12] = {0x22,0xaa,0xaa,0x55,0x55,0x44,0x11,0x11,0x11,0x11,0x44,0x22};
const u16 col_15_spans[2] = {28,13};
const u8 col_15_texels[13] = {0x44,0x22,0x55,0x55,0x44,0x44,0x22,0x11,0x33,0x33,0x11,0x44,0x22};
const u16 col_16_spans[2] = {27,13};
const u8 col_16_texels[13] = {0x44,0x44,0x22,0x44,0x44,0x44,0x22,0x22,0x22,0x11,0x11,0x44,0x22};
const u16 col_17_spans[4] = {21,5,1,15};
const u8 col_17_texels[20] = {0x44,0x44,0x22,0x22,0x77,0x44,0x44,0x22,0x22,0x22,0x22,0x44,0xaa,0x77,0x44,0x22,0x22,0x22,0x22,0x22};
const u16 col_18_spans[4] = {20,14,8,1};
const u8 col_18_texels[15] = {0x44,0x44,0x44,0x44,0x22,0x44,0x77,0x44,0x22,0xaa,0x22,0x44,0x44,0x22,0x55};
const u16 col_19_spans[4] = {8,22,12,1};
const u8 col_19_texels[23] = {0x55,0x88,0x88,0x55,0x88,0x55,0x55,0x55,0x55,0x55,0x44,0x44,0xaa,0x55,0x44,0x44,0x44,0x22,0x44,0x22,0x77,0x22,0x55};
const u16 col_20_spans[4] = {7,22,13,1};
const u8 col_20_texels[23] = {0x55,0xaa,0xaa,0xaa,0x88,0xaa,0x88,0x55,0xaa,0x55,0x55,0x55,0x44,0x55,0x55,0x44,0x44,0x44,0x22,0x22,0x22,0x44,0xaa};
const u16 col_21_spans[2] = {6,36};
const u8 col_21_texels[36] = {0x55,0xaa,0xff,0xff,0xaa,0xaa,0x88,0x55,0x88,0x55,0x88,0xaa,0x55,0x44,0x55,0x44,0x44,0x99,0xbb,0xbb,0x99,0x44,0x44,0x22,0x22,0x22,0x22,0x22,0x22,0x77,0x77,0x55,0x55,0xaa,0xaa,0xaa};
const u16 col_22_spans[2] = {6,24};
const u8 col_22_texels[24] = {0xaa,0xff,0xff,0xff,0xaa,0xaa,0x88,0xaa,0x55,0x88,0x55,0x55,0x44,0x44,0x44,0x44,0x99,0xbb,0xbb,0xbb,0xbb,0x99,0x44,0x44};
const u16 col_23_spans[4] = {6,28,9,3};
const u8 col_23_texels[31] = {0xaa,0xff,0xff,0xaa,0xaa,0x88,0x88,0x88,0x88,0x55,0x88,0x55,0x44,0x44,0x44,0x99,0xbb,0xdd,0xbb,0xbb,0xbb,0x99,0x99,0x44,0x22,0x44,0x44,0x22,0xcc,0xcc,0x11};
const u16 col_24_spans[8] = {6,29,2,5,1,3,4,5};
const u8 col_24_texels[42] = {0x55,0xaa,0xaa,0xaa,0x88,0xaa,0x88,0x88,0x88,0xaa,0x88,0x55,0x44,0x11,0x99,0x99,0xdd,0x99,0x44,0xff,0xbb,0xbb,0x99,0x44,0x22,0xaa,0x55,0x44,0x22,0x22,0x22,0x22,0x22,0x22,0xcc,0x11,0x11,0x44,0x77,0x77,0x77,0x44};
const u16 col_25_spans[4] = {7,39,2,9};
const u8 col_25_texels[48] = {0x55,0xaa,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x55,0x44,0x11,0x11,0xbb,0x99,0xff,0x44,0x99,0xff,0xbb,0xbb,0x99,0x44,0x22,0x22,0x22,0x22,0x44,0x22,0x44,0x22,0x44,0x22,0x11,0x22,0x22,0x11,0x11,0xcc,0x44,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x44};      
const u16 col_26_spans[6] = {7,1,1,37,1,12};
const u8 col_26_texels[50] = {0x66,0x55,0xbb,0xbb,0xbb,0xbb,0xbb,0x99,0x55,0x11,0x11,0x99,0xbb,0x99,0xff,0x44,0xbb,0xbb,0xbb,0xbb,0x99,0x44,0x44,0x44,0x44,0x44,0x22,0x22,0x55,0x22,0x44,0x22,0x11,0x22,0x22,0x11,0xcc,0xcc,0x44,0x77,0x77,0xaa,0xaa,0xaa,0xaa,0x77,0x77,0x77,0x77,0x44};
const u16 col_27_spans[4] = {6,1,2,51};
const u8 col_27_texels[52] = {0x66,0xbb,0xdd,0xdd,0xdd,0x99,0xbb,0xbb,0x44,0x99,0x44,0xbb,0xbb,0x44,0x44,0x44,0x44,0x44,0xff,0x99,0x44,0x44,0x55,0x44,0x44,0x55,0xff,0xaa,0xaa,0x55,0x44,0x77,0x11,0x22,0x22,0xcc,0xcc,0x11,0x44,0x77,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0x77,0x77,0x44};
const u16 col_28_spans[2] = {3,57};
const u8 col_28_texels[57] = {0x66,0x66,0x66,0x66,0x66,0x99,0xdd,0xff,0xff,0xdd,0x99,0xbb,0x55,0x44,0x55,0x99,0x77,0x99,0xbb,0xdd,0x44,0xbb,0x99,0x99,0x44,0x44,0x55,0xaa,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x77,0x11,0x11,0x77,0xcc,0x11,0x11,0x77,0xaa,0xaa,0xaa,0xff,0xff,0xff,0xff,0xff,0xaa,0xaa,0xaa,0x77,0x77};
const u16 col_29_spans[2] = {2,60};
const u8 col_29_texels[60] = {0x66,0xbb,0xcc,0xbb,0x66,0x66,0x66,0x66,0x66,0x66,0xdd,0xff,0xff,0x44,0x44,0x44,0xbb,0x77,0x77,0x99,0xbb,0x44,0x99,0x99,0x22,0x22,0x22,0xaa,0x44,0x22,0x44,0x22,0x44,0x22,0x44,0x22,0xaa,0x11,0x11,0x11,0xaa,0x11,0x11,0xcc,0x77,0xaa,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xaa,0xaa,0xaa,0xaa,0x77,0x77};
const u16 col_30_spans[2] = {3,59};
const u8 col_30_texels[59] = {0x66,0xbb,0xcc,0xcc,0xcc,0xbb,0x66,0x66,0x66,0xdd,0xbb,0xbb,0x44,0x44,0x44,0xbb,0x77,0x77,0x99,0xbb,0x44,0x99,0x99,0x22,0x22,0x22,0xff,0x44,0x22,0x55,0x22,0xaa,0x22,0x55,0x22,0xaa,0x11,0x11,0x11,0xaa,0x11,0xcc,0xcc,0x77,0xaa,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xaa,0xaa,0xaa,0xaa,0x77,0x77};
const u16 col_31_spans[2] = {3,57};
const u8 col_31_texels[57] = {0x66,0x66,0x66,0x66,0x66,0x99,0xdd,0xff,0xff,0xdd,0x99,0xbb,0x55,0x44,0x55,0x99,0x77,0x99,0xbb,0xdd,0x44,0xbb,0x99,0x99,0x44,0x44,0x55,0x55,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x77,0x11,0x11,0x77,0xcc,0xcc,0x11,0x77,0xaa,0xaa,0xaa,0xff,0xff,0xff,0xff,0xff,0xaa,0xaa,0xaa,0x77,0x77};
const u16 col_32_spans[6] = {4,1,1,1,2,51};
const u8 col_32_texels[53] = {0x66,0x66,0xbb,0xdd,0xdd,0xdd,0x99,0xbb,0xbb,0x44,0x99,0x44,0xbb,0xbb,0xdd,0xff,0x44,0xff,0xbb,0xbb,0x99,0x44,0x44,0x55,0x44,0x44,0x55,0xff,0xaa,0xaa,0x55,0x44,0x77,0x11,0x22,0x22,0xcc,0x11,0x11,0x44,0x77,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0x77,0x77,0x44};
const u16 col_33_spans[6] = {3,1,6,36,1,12};
const u8 col_33_texels[49] = {0x66,0xbb,0xbb,0xbb,0xbb,0xbb,0x99,0x44,0x11,0x11,0x99,0xbb,0x99,0xff,0x99,0x44,0xff,0xbb,0xbb,0x99,0x44,0x44,0x44,0x44,0x44,0x22,0x22,0x55,0x22,0x44,0x22,0x11,0x22,0x22,0x11,0x11,0xcc,0x44,0x77,0x77,0xaa,0xaa,0xaa,0xaa,0x77,0x77,0x77,0x77,0x44}; 
const u16 col_34_spans[4] = {12,34,2,9};
const u8 col_34_texels[43] = {0x44,0xaa,0x55,0x44,0x22,0x22,0x11,0x11,0xbb,0x99,0xff,0xff,0x99,0x44,0x99,0xff,0x99,0x44,0x22,0x22,0x22,0x22,0x44,0x22,0x44,0x22,0x44,0x22,0x11,0x22,0x22,0x11,0xcc,0xcc,0x44,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x44};
const u16 col_35_spans[8] = {12,23,2,5,1,3,4,5};
const u8 col_35_texels[36] = {0x44,0x55,0xaa,0x55,0x44,0x22,0x22,0x11,0x99,0x99,0xdd,0xff,0xdd,0x99,0x99,0xff,0x99,0x44,0x22,0xaa,0x55,0x44,0x22,0x22,0x22,0x22,0x22,0x22,0xcc,0xcc,0x11,0x44,0x77,0x77,0x77,0x44};
const u16 col_36_spans[4] = {13,21,9,3};
const u8 col_36_texels[24] = {0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x99,0xbb,0xdd,0xbb,0xbb,0xbb,0x99,0x99,0x44,0x22,0x44,0x44,0x22,0xcc,0x11,0x11};
const u16 col_37_spans[2] = {19,11};
const u8 col_37_texels[11] = {0x44,0x44,0x44,0x99,0xbb,0xbb,0xbb,0xbb,0x99,0x44,0x44};
const u16 col_38_spans[2] = {19,10};
const u8 col_38_texels[10] = {0x44,0x55,0x44,0x44,0x99,0xbb,0xbb,0x99,0x44,0x44};
const u16 col_39_spans[2] = {17,12};
const u8 col_39_texels[12] = {0x55,0x44,0x44,0x55,0x55,0x44,0x44,0x44,0x22,0x22,0x77,0x22};
const u16 col_40_spans[2] = {15,16};
const u8 col_40_texels[16] = {0x55,0xaa,0xaa,0x55,0x44,0xaa,0x55,0x44,0x44,0x44,0x22,0x44,0x22,0xaa,0x44,0x22};
const u16 col_41_spans[4] = {20,13,7,2};
const u8 col_41_texels[15] = {0x44,0x44,0x44,0x44,0x22,0x44,0x77,0x44,0x22,0x77,0x22,0x22,0x22,0x22,0x22};
const u16 col_42_spans[6] = {19,7,1,7,5,4};
const u8 col_42_texels[18] = {0x55,0x44,0x44,0x44,0x22,0x22,0x77,0x44,0x44,0x22,0x22,0x44,0x22,0x22,0x22,0x55,0x77,0x22};
const u16 col_43_spans[6] = {19,2,6,8,3,6};
const u8 col_43_texels[16] = {0xaa,0x55,0x44,0x44,0x22,0x44,0x44,0x77,0x22,0x22,0x22,0x44,0x44,0x55,0x44,0x22};
const u16 col_44_spans[6] = {18,2,8,8,1,8};
const u8 col_44_texels[18] = {0xaa,0x55,0x44,0x22,0x55,0x44,0x77,0xaa,0x22,0x22,0x22,0x44,0x44,0x44,0x22,0x22,0x44,0x22};
const u16 col_45_spans[6] = {18,1,10,12,2,2};
const u8 col_45_texels[15] = {0x55,0x55,0x55,0x55,0x44,0x77,0xaa,0x22,0x22,0x22,0xaa,0x44,0x22,0x22,0x44};
const u16 col_46_spans[4] = {29,11,4,1};
const u8 col_46_texels[12] = {0x55,0xaa,0x55,0x55,0x44,0x77,0x22,0x22,0xaa,0x44,0x22,0x22};
const u16 col_47_spans[2] = {29,11};
const u8 col_47_texels[11] = {0x55,0xff,0x55,0x55,0x55,0x22,0x22,0x22,0xff,0x44,0x22};
const u16 col_48_spans[4] = {29,10,6,1};
const u8 col_48_texels[11] = {0x55,0xaa,0xff,0x55,0x22,0x55,0x44,0x22,0xaa,0x22,0x44};
const u16 col_49_spans[4] = {28,11,6,1};
const u8 col_49_texels[12] = {0x88,0x55,0x55,0xaa,0x22,0xff,0xaa,0x77,0x44,0x55,0x22,0x44};
const u16 col_50_spans[4] = {27,12,5,2};
const u8 col_50_texels[14] = {0x88,0xaa,0x55,0x55,0x55,0x22,0xff,0xaa,0xaa,0x77,0x22,0x22,0x44,0x22};
const u16 col_51_spans[4] = {26,14,2,3};
const u8 col_51_texels[17] = {0x88,0xaa,0x55,0x88,0x55,0x44,0x22,0xaa,0x55,0xaa,0x77,0x55,0x44,0x22,0x22,0x44,0x44};
const u16 col_52_spans[4] = {25,6,1,13};
const u8 col_52_texels[19] = {0x88,0xff,0xaa,0x88,0x55,0x88,0x22,0x44,0x22,0x55,0xaa,0x55,0x55,0x44,0x44,0x44,0x55,0x55,0x44};
const u16 col_53_spans[4] = {24,6,3,12};
const u8 col_53_texels[18] = {0x88,0xff,0xaa,0x88,0x55,0x88,0x22,0x22,0x55,0xaa,0xff,0x55,0x55,0x55,0x55,0xaa,0x55,0x22};
const u16 col_54_spans[4] = {24,5,5,10};
const u8 col_54_texels[15] = {0xaa,0xaa,0x88,0x88,0x88,0x22,0x44,0x55,0xaa,0xff,0xff,0xaa,0xaa,0x55,0x44};
const u16 col_55_spans[4] = {24,4,7,9};
const u8 col_55_texels[13] = {0x88,0xaa,0x88,0x88,0x22,0x44,0x55,0xaa,0xaa,0xaa,0x55,0x44,0x22};
const u16 col_56_spans[4] = {25,2,9,7};
const u8 col_56_texels[9] = {0x88,0x88,0x44,0x44,0x44,0x44,0x44,0x44,0x44};
const u16 col_57_spans[2] = {37,5};
const u8 col_57_texels[5] = {0x22,0x44,0x44,0x44,0x22};
const rle_sprite claw_guy_sprite = {
.num_columns = 64,
.columns = {
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 1, .spans = col_9_spans, .texels = col_9_texels},
{.num_spans = 1, .spans = col_10_spans, .texels = col_10_texels},
{.num_spans = 1, .spans = col_11_spans, .texels = col_11_texels},
{.num_spans = 1, .spans = col_12_spans, .texels = col_12_texels},
{.num_spans = 1, .spans = col_13_spans, .texels = col_13_texels},
{.num_spans = 1, .spans = col_14_spans, .texels = col_14_texels},
{.num_spans = 1, .spans = col_15_spans, .texels = col_15_texels},
{.num_spans = 1, .spans = col_16_spans, .texels = col_16_texels},
{.num_spans = 2, .spans = col_17_spans, .texels = col_17_texels},
{.num_spans = 2, .spans = col_18_spans, .texels = col_18_texels},
{.num_spans = 2, .spans = col_19_spans, .texels = col_19_texels},
{.num_spans = 2, .spans = col_20_spans, .texels = col_20_texels},
{.num_spans = 1, .spans = col_21_spans, .texels = col_21_texels},
{.num_spans = 1, .spans = col_22_spans, .texels = col_22_texels},
{.num_spans = 2, .spans = col_23_spans, .texels = col_23_texels},
{.num_spans = 4, .spans = col_24_spans, .texels = col_24_texels},
{.num_spans = 2, .spans = col_25_spans, .texels = col_25_texels},
{.num_spans = 3, .spans = col_26_spans, .texels = col_26_texels},
{.num_spans = 2, .spans = col_27_spans, .texels = col_27_texels},
{.num_spans = 1, .spans = col_28_spans, .texels = col_28_texels},
{.num_spans = 1, .spans = col_29_spans, .texels = col_29_texels},
{.num_spans = 1, .spans = col_30_spans, .texels = col_30_texels},
{.num_spans = 1, .spans = col_31_spans, .texels = col_31_texels},
{.num_spans = 3, .spans = col_32_spans, .texels = col_32_texels},
{.num_spans = 3, .spans = col_33_spans, .texels = col_33_texels},
{.num_spans = 2, .spans = col_34_spans, .texels = col_34_texels},
{.num_spans = 4, .spans = col_35_spans, .texels = col_35_texels},
{.num_spans = 2, .spans = col_36_spans, .texels = col_36_texels},
{.num_spans = 1, .spans = col_37_spans, .texels = col_37_texels},
{.num_spans = 1, .spans = col_38_spans, .texels = col_38_texels},
{.num_spans = 1, .spans = col_39_spans, .texels = col_39_texels},
{.num_spans = 1, .spans = col_40_spans, .texels = col_40_texels},
{.num_spans = 2, .spans = col_41_spans, .texels = col_41_texels},
{.num_spans = 3, .spans = col_42_spans, .texels = col_42_texels},
{.num_spans = 3, .spans = col_43_spans, .texels = col_43_texels},
{.num_spans = 3, .spans = col_44_spans, .texels = col_44_texels},
{.num_spans = 3, .spans = col_45_spans, .texels = col_45_texels},
{.num_spans = 2, .spans = col_46_spans, .texels = col_46_texels},
{.num_spans = 1, .spans = col_47_spans, .texels = col_47_texels},
{.num_spans = 2, .spans = col_48_spans, .texels = col_48_texels},
{.num_spans = 2, .spans = col_49_spans, .texels = col_49_texels},
{.num_spans = 2, .spans = col_50_spans, .texels = col_50_texels},
{.num_spans = 2, .spans = col_51_spans, .texels = col_51_texels},
{.num_spans = 2, .spans = col_52_spans, .texels = col_52_texels},
{.num_spans = 2, .spans = col_53_spans, .texels = col_53_texels},
{.num_spans = 2, .spans = col_54_spans, .texels = col_54_texels},
{.num_spans = 2, .spans = col_55_spans, .texels = col_55_texels},
{.num_spans = 2, .spans = col_56_spans, .texels = col_56_texels},
{.num_spans = 1, .spans = col_57_spans, .texels = col_57_texels},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL},
{.num_spans = 0, .spans = NULL, .texels = NULL}
}
};


const object_template object_types[32+1] = {
    // first object type is player
    {.is_player = 1, .name = "player_object_type"},
    {.init_state = 1,
     .name = "claw guy", .sprite=&claw_guy_sprite,
    .from_floor_draw_offset = 10<<4, .width=46, .height=80<<4},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    {.init_state = 0, .is_player = 0},
    //{.init_state = 2,
    // .name = "blue keycard", 
    //.size = 20, .from_floor_draw_offset = 10<<4, .width=30, .height=46<<4},
    //{.init_state = 1,
    // .name = "red cube",
    //.size = 20, .from_floor_draw_offset = 20<<4, .width=12, .height=20<<4},
    //{.init_state = 1,
    // .name = "first enemy type ",
    //.size = 20, .from_floor_draw_offset = 10<<4, .width=46, .height=80<<4},
};

int look_for_player(object* cur_obj, uint16_t cur_sector);
int follow_player(object* cur_obj, uint16_t cur_sector);
int maybe_get_picked_up(object* cur_obj, uint16_t cur_sector);
int idle(object* cur_obj, uint16_t cur_sector);

obj_state object_state_machines[] = {
    {.next_state = 0, .ticks = 3, .action = &look_for_player, },
    {.next_state = 1, .ticks = 1, .action = &follow_player, },
    {.next_state = 2, .ticks = 1, .action = &maybe_get_picked_up, },
    {.next_state = 3, .ticks = 3, .action = &idle, },
};


fix32 dist_sqr(object_pos posa, object_pos origin) {
    //fix16Sqrt();
    fix16 dx1 = fix32ToFix16(posa.x - origin.x);
    fix16 dy1 = fix32ToFix16(posa.y - origin.y);
    return (dx1*dx1)+(dy1*dy1);
} 

// returns -1 if point 1 is closer than point 2, +1 if opposite, or 0 is they are the same distance
int is_closer(object_pos pos1, object_pos pos2, object_pos origin) {
  fix32 d1 = dist_sqr(pos1, origin);
  fix32 d2 = dist_sqr(pos2, origin);
  if(d1 == d2) {
    return 0;
  } else if (d1 < d2) {
    return -1;
  } else {
    return 1;
  }
} 


// inserts object in sorted position?
// not important
// first parameter used to be cur_player_pos
object* alloc_object_in_sector(u32 activate_tick, int sector_num, fix32 x, fix32 y, fix32 z, uint8_t object_type) {
    object* res = pop(&free_list);
    if(res == NULL) {
        KLog("free list pop is null? wtf");
        return NULL;
    }


    res->pos.cur_sector = sector_num;
    res->pos.x = x;
    res->pos.y = y;
    res->pos.z = z;
    res->id = next_obj_id++;
    volatile object_template* tmpl = &object_types[object_type];
    res->current_state = tmpl->init_state; // object_types[object_type].init_state;
    res->object_type = object_type;
    res->activate_tick = activate_tick;
    res->pos.cur_sector = sector_num;

    object** sector_list_obj = &sector_lists[sector_num];
    /*
    while((*sector_list_obj) != NULL) {
        break; // TODO why does this cause such a performance hit if it's executed just once?
        object* cur_sect_obj = *sector_list_obj;
        if(is_closer(res->pos, cur_sect_obj->pos, cur_player_pos) == -1) {
        // this object is the closest
            break;
        }
        sector_list_obj = &(cur_sect_obj->next);
    }
    */
    //return NULL;

  push_to_front(res, sector_list_obj);
  return res;
} 


void move_object_to_sector(object* obj, u16 next_sector) {

    int old_sector = obj->pos.cur_sector;


    object* next = obj->next;
    object* prev = obj->prev;
    obj->prev = NULL;
    obj->next = NULL;
    obj->pos.cur_sector = next_sector;
    
    if(next != NULL) {
        next->prev = prev;
    }
    if(prev != NULL) {
        prev->next = next;
    }

    if(prev == NULL) {
        sector_lists[old_sector] = next;
    }

    obj->next = sector_lists[next_sector];
    if(obj->next != NULL) {
        obj->next->prev = obj;
    }
    sector_lists[next_sector] = obj;

    //char buf[32];
    //sprintf(buf, "move obj to sect %i ", next_sector);
    //VDP_drawTextBG(BG_B, buf, 10, 10);
    //sprintf(buf, "old sect %i",  )
    //die(buf);
}


/*
void free_object(object* obj) {
    int old_sector = obj->pos.cur_sector;
    object* next = obj->next;
    object* prev = obj->prev;
    if(next != NULL) {
        next->prev = prev;
    }
    if(prev != NULL) {
        prev->next = next;
    } else {    
        sector_lists[old_sector] = next;
    }
    object* next_free = free_object_list;
    if(next_free != NULL) {
        next_free->prev = obj;
    }
    obj->next = next_free;
    free_object_list = obj;
}
*/

object* objects_in_sector(int sector_num) {
    return sector_lists[sector_num];
}

int idle(object* cur_obj, uint16_t cur_sector) {
    return 1; // stays alive
}

int look_for_player(object* cur_obj, uint16_t cur_sector) {
    if(cur_player_pos.cur_sector == cur_sector) {
        //cur_obj->object_type = 1;
        cur_obj->current_state++;
        cur_obj->tgt.sector = cur_sector;
        cur_obj->tgt.x = cur_player_pos.x;
        cur_obj->tgt.y = cur_player_pos.y;
        cur_obj->tgt.z = cur_player_pos.z;
    } else {
        //KLog("player not found");
    }
    return 1;
}



int maybe_get_picked_up(object* cur_obj, uint16_t cur_sector) {  
    object_pos pos = cur_obj->pos;
    int dx = fix32ToInt(cur_player_pos.x - pos.x);
    int dy = fix32ToInt(cur_player_pos.y - pos.y);
    
    u32 dist = fastLength(dx, dy);
    
    if(dist < 32) { 
        if(inventory_add_item(BLUE_KEY)) {
            char buf[50];
            int len = sprintf(buf, "picked up the %s!", object_types[cur_obj->object_type].name);
            console_push_message_high_priority(buf, len, 30);
        }

        return 0;
    }
    return 1;
}

int follow_player(object* cur_obj, uint16_t cur_sector) {
    object_pos pos = cur_obj->pos;
    // follow player
    //KLog("following player");

    int dx = fix32ToInt(cur_player_pos.x - pos.x);
    int dy = fix32ToInt(cur_player_pos.y - pos.y);


    //u32 dist = getApproximatedDistance(dx, dy);
    //if(dist < 8) { c }
    //int norm_dx = dx/dist;
    //int norm_dy = dy/dist;
    //dx = 7*norm_dx;
    //dy = 7*norm_dy;

    //int dz = fix32ToInt(cur_player_pos.z - pos.z);

    volatile object_template* obj_type = &object_types[cur_obj->object_type];

    s16 speed = obj_type->speed;
    if(dx > 0) {
        dx = min(speed, dx);
    } else if (dx < 0) {
        dx = max(-speed, dx);
    }

    if(dy > 0) {
        dy = min(speed, dy);
    } else {
        dy = max(-speed, dy);
    }
    

    /*
    int dist_sqr = (dx*dx)+(dy*dy);

    while (dist_sqr > 2) {
        dx >>= 1;
        dy >>= 1;
        dist_sqr = (dx*dx)+(dy*dy);
        KLog_U1("dist sqr: ", dist_sqr);
    }
    */

    fix32 dx32 = intToFix32(dx);
    fix32 dy32 = intToFix32(dy);

    //collision_result move_res = check_for_collision_radius(pos.x, pos.y, pos.x+dx32, pos.y+dy32, 8, cur_sector); // radius of 8 originally
    
    
    cur_obj->pos.x = pos.x+dx32;//move_res.pos.x;
    cur_obj->pos.y = pos.y+dy32;//move_res.pos.y;

    
    /*

    if(move_res.new_sector != cur_sector) {
        move_object_to_sector(cur_obj, move_res.new_sector);
        u16 sect = cur_obj->pos.cur_sector;
        u16 sect_group = sector_group(sect, cur_portal_map);
        cur_obj->pos.z = get_sector_group_floor_height(sect_group);
    }
    //u16 sect = cur_obj->pos.cur_sector;
    //u16 sect_group = sector_group(sect, cur_portal_map);
    //cur_obj->pos.z = get_sector_group_floor_height(sect_group);
    */
    return 1;
}



void process_all_objects(uint32_t cur_frame) {
    /*
    for(int sect = 0; sect < num_sector_lists; sect++) {
        object* cur_object = sector_lists[sect];
        if(cur_object != NULL) {
            KLog_U2("got object: ", cur_object->id, " in sector: ", sect);
            KLog_U2("object reported sector: ", cur_object->pos.cur_sector, ", ", 0);
        }
    }
    */
    for(int sect = 0; sect < num_sector_lists; sect++) {
        object* cur_object = sector_lists[sect];
        while(cur_object != NULL) {
            if(cur_object->activate_tick != 0) {
                cur_object->activate_tick--;
            
            } else {
                KLog("processing object");
                uint16_t state_idx = cur_object->current_state;
                int live = object_state_machines[state_idx].action(cur_object, sect);
                if(!live) { 
                    // TODO: make work with more than one object
                    // this will only work with a single object
                    push_to_front(cur_object, &free_list);
                    sector_lists[sect] = NULL;
                } else {
                    if(object_state_machines[state_idx].action == &idle) {
                        cur_object->activate_tick = 128;
                    } else {
                        cur_object->activate_tick = 1;
                    }
                }
            }
            cur_object = cur_object->next;
            //break;
        }
    }
    KLog("done processing objects");
}