#ifndef VIS_RANGE_H
#define VIS_RANGE_H

// 9 bytes per wall (10 with alignment?)


// TODO: store only one range, but
// 1 byte that describes whether the range means maybe-visible or definitely-not-visible

typedef struct {
  u8 two_ranges;
  // up two four angles
  // if only one range is present
  // the first two represent the min and max angle this wall is visible between
  // if there are two ranges, the second represent another min and max angle this wall is visible between

  // we need two ranges in case the real visible range wraps around 360 degrees
  // 360 degree range is broken up into 256 chunks
  uint16_t angles[4];
} vis_range;

#endif
