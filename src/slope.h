#ifndef SLOPE_H

typedef enum {
    NO_SLOPE,
    SLOPE_PORTAL, // the portal whose bottom portion matches the connecting sector's height, rather than the source sector's height
    SLOPE_TRANSITION_RIGHT, // the portal/wall whose bottom/top-right portion matches the height of the wall to the right's neighbor sector
    SLOPE_TRANSITION_LEFT, // the portal/wall whose bottom/top-left portion matches the height of the wall to the left's neighbor sector
} slope_type;

#endif