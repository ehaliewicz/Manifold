#ifndef LEVEL_H
#define LEVEL_H

#include <genesis.h>

#include "blockmap.h"
#include "linedef.h"
#include "node.h"
#include "sector.h"
#include "seg.h"
#include "sidedef.h"
#include "ssector.h"
#include "thing.h"
#include "vertex.h"



typedef struct {
    const int num_linedefs;
    const linedef* linedefs;
    const node* nodes;
    const sector* sectors;
    const int num_segs;
    const seg* segs;
    const sidedef* sidedefs;
    const ssector* ssectors;
    const thing* things;
    const int num_vertexes;
    const vertex* vertexes;
    const blockmap* blkmap;
    const render_blockmap* render_blkmap;
} level;

extern const level* cur_level;

#endif