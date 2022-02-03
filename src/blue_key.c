#include <genesis.h>

#define TRANS 0 
#define LBLUE 6
#define MBLUE 10

/*
blue_key_card[] = {
    7, // num columns
    1, // 1 8x8 sprite tile wide
    16, // num rows
    2, // 2 8x8 sprite tiles tall

    // 0th column
    2, // skip 2
    4, // span of length 4
    (MBLUE<<4|LBLUE),
    (LBLUE<<4|DBLUE),
}
*/