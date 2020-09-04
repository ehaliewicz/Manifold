#ifndef REJECT_H
#define REJECT_H

#include <genesis.h>

typedef struct {
    int num_bytes;
    u8 tbl[];
} reject;

#endif