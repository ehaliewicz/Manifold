#ifndef SLOPE_H

typedef enum {
    NO_SLOPE,
    SLOPE_PORTAL, // the portal whose bottom portion matches the connecting sector's height, rather than the source sector's height
    SLOPE_RIGHT_WALL, // the portal/wall whose bottom-right portion matches the height of the wall to the right's neighbor sector
    SLOPE_LEFT_WALL, // the portal/wall whose bottom-left portion matches the height of the wall to the left's neighbor sector
} slope_type;

#endif