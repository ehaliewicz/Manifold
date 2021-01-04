#ifndef VIS_RANGE_H

// 6 bytes per wall


typedef struct {
  u8 two_ranges;
  // up two four angles
  // if only one range is present
  // the first two represent the min and max angle this wall is visible between
  // if there are two ranges, the second represent another min and max angle this wall is visible between

  // we need two ranges in case the real visible range wraps around 360 degrees
  // 360 degree range is broken up into 256 chunks
  uint8_t angles[4];
} vis_range;
