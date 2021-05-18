#include <genesis.h>
#include "colors.h"
#include "draw.h"


/*   .. .* *. ..
     .. ** ** ..
     .* ** ** *.
     ** ** ** **
     ** ** ** **
     .* ** ** *.
     .. ** ** ..
     .. .* *. .. */

#define RED ((RED_IDX<<4)|RED_IDX)
#define RIGHT_RED ((TRANSPARENT_IDX << 4) | RED_IDX)
#define LEFT_RED ((RED_IDX<<4) | TRANSPARENT_IDX)

const uint8_t small_ball_sprite[17] = {
    // 8x8
    4, // num columns

    // column 0
    1, // 1 runs
    2, 6, RED,

    // column 1
    // 3 runs
    1,
    0, 10, RED,

    // column 2
    1,
    0, 10, RED,

    // column 3
    1, // 3 runs
    2, 6, RED,

};

uint8_t med_sprite[] = {

};


uint8_t large_sprite[] = {

};