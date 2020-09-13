#include "level.h"
static const thing things[143] = {
    {.x = 1056, .y = -3616, .angle = 90, .type = 1, .flags = 7 },
     {.x = 1008, .y = -3600, .angle = 90, .type = 2, .flags = 7 },
     {.x = 1104, .y = -3600, .angle = 90, .type = 3, .flags = 7 },
     {.x = 960, .y = -3600, .angle = 90, .type = 4, .flags = 7 },
     {.x = 288, .y = -3104, .angle = 90, .type = 48, .flags = 7 },
     {.x = 288, .y = -3360, .angle = 90, .type = 48, .flags = 7 },
     {.x = 528, .y = -3312, .angle = 90, .type = 2028, .flags = 7 },
     {.x = 528, .y = -3152, .angle = 90, .type = 2028, .flags = 7 },
     {.x = 3440, .y = -3472, .angle = 135, .type = 3001, .flags = 15 },
     {.x = 3360, .y = -3504, .angle = 135, .type = 3001, .flags = 12 },
     {.x = 2912, .y = -2816, .angle = 90, .type = 3004, .flags = 15 },
     {.x = 3056, .y = -3584, .angle = 90, .type = 3004, .flags = 14 },
     {.x = 3136, .y = -3408, .angle = 90, .type = 3004, .flags = 12 },
     {.x = 1824, .y = -3280, .angle = 180, .type = 2019, .flags = 7 },
     {.x = 2736, .y = -4256, .angle = 0, .type = 3004, .flags = 12 },
     {.x = 3280, .y = -4256, .angle = 180, .type = 3004, .flags = 12 },
     {.x = 3008, .y = -4416, .angle = 90, .type = 3001, .flags = 15 },
     {.x = 3280, .y = -4304, .angle = 180, .type = 3004, .flags = 12 },
     {.x = 2736, .y = -4304, .angle = 0, .type = 3004, .flags = 12 },
     {.x = 2880, .y = -4416, .angle = 0, .type = 2012, .flags = 15 },
     {.x = 2736, .y = -4160, .angle = 0, .type = 2007, .flags = 15 },
     {.x = 3280, .y = -4160, .angle = 0, .type = 2007, .flags = 15 },
     {.x = 2784, .y = -2944, .angle = 90, .type = 2008, .flags = 23 },
     {.x = 144, .y = -3136, .angle = 90, .type = 2014, .flags = 7 },
     {.x = 144, .y = -3328, .angle = 90, .type = 2014, .flags = 7 },
     {.x = 96, .y = -3392, .angle = 90, .type = 2014, .flags = 7 },
     {.x = 96, .y = -3072, .angle = 90, .type = 2014, .flags = 7 },
     {.x = 432, .y = -3040, .angle = 90, .type = 2015, .flags = 7 },
     {.x = 432, .y = -3424, .angle = 90, .type = 2015, .flags = 7 },
     {.x = 32, .y = -3232, .angle = 90, .type = 2015, .flags = 7 },
     {.x = -32, .y = -3232, .angle = 90, .type = 2015, .flags = 7 },
     {.x = 1312, .y = -3520, .angle = 90, .type = 2014, .flags = 7 },
     {.x = 736, .y = -3520, .angle = 90, .type = 2014, .flags = 7 },
     {.x = 752, .y = -2992, .angle = 90, .type = 2015, .flags = 7 },
     {.x = 976, .y = -2912, .angle = 90, .type = 2015, .flags = 7 },
     {.x = 1312, .y = -3264, .angle = 90, .type = 2035, .flags = 7 },
     {.x = 1152, .y = -2912, .angle = 90, .type = 2035, .flags = 7 },
     {.x = 864, .y = -3328, .angle = 90, .type = 2035, .flags = 7 },
     {.x = 2672, .y = -2752, .angle = 90, .type = 2014, .flags = 7 },
     {.x = 2752, .y = -2640, .angle = 90, .type = 2014, .flags = 7 },
     {.x = 3232, .y = -3040, .angle = 90, .type = 2028, .flags = 7 },
     {.x = 2800, .y = -3040, .angle = 90, .type = 2028, .flags = 7 },
     {.x = 2848, .y = -3584, .angle = 90, .type = 2008, .flags = 7 },
     {.x = 3008, .y = -3968, .angle = 90, .type = 2008, .flags = 7 },
     {.x = 2912, .y = -4176, .angle = 180, .type = 2028, .flags = 15 },
     {.x = 3104, .y = -4176, .angle = 180, .type = 2028, .flags = 15 },
     {.x = 2720, .y = -4352, .angle = 180, .type = 2028, .flags = 15 },
     {.x = 3296, .y = -4352, .angle = 180, .type = 2028, .flags = 15 },
     {.x = 2944, .y = -4320, .angle = 180, .type = 2035, .flags = 15 },
     {.x = 3184, .y = -4224, .angle = 180, .type = 2035, .flags = 15 },
     {.x = 2832, .y = -4240, .angle = 180, .type = 2035, .flags = 15 },
     {.x = 3184, .y = -4176, .angle = 180, .type = 2014, .flags = 15 },
     {.x = 2832, .y = -4176, .angle = 180, .type = 2014, .flags = 15 },
     {.x = 2736, .y = -4192, .angle = 180, .type = 2014, .flags = 15 },
     {.x = 3280, .y = -4192, .angle = 180, .type = 2014, .flags = 12 },
     {.x = 3280, .y = -4448, .angle = 180, .type = 2014, .flags = 15 },
     {.x = 2736, .y = -4448, .angle = 180, .type = 2015, .flags = 15 },
     {.x = 3072, .y = -4832, .angle = 180, .type = 2015, .flags = 7 },
     {.x = 2944, .y = -4832, .angle = 180, .type = 2015, .flags = 7 },
     {.x = 3072, .y = -4704, .angle = 180, .type = 2015, .flags = 7 },
     {.x = 2944, .y = -4704, .angle = 180, .type = 2015, .flags = 7 },
     {.x = 3088, .y = -4528, .angle = 180, .type = 35, .flags = 7 },
     {.x = 2928, .y = -4528, .angle = 180, .type = 35, .flags = 7 },
     {.x = 3008, .y = -4816, .angle = 90, .type = 3001, .flags = 12 },
     {.x = 3072, .y = -4768, .angle = 90, .type = 2011, .flags = 15 },
     {.x = 2304, .y = -4032, .angle = 90, .type = 2012, .flags = 15 },
     {.x = 2176, .y = -4000, .angle = 90, .type = 2049, .flags = 15 },
     {.x = 2432, .y = -4000, .angle = 90, .type = 2048, .flags = 15 },
     {.x = -224, .y = -3232, .angle = 90, .type = 2018, .flags = 7 },
     {.x = -288, .y = -3232, .angle = 0, .type = 11, .flags = 7 },
     {.x = 1056, .y = -3552, .angle = 90, .type = 11, .flags = 7 },
     {.x = 1792, .y = -2944, .angle = 270, .type = 11, .flags = 7 },
     {.x = 3408, .y = -3504, .angle = 135, .type = 11, .flags = 7 },
     {.x = 2784, .y = -4160, .angle = 270, .type = 11, .flags = 7 },
     {.x = 1712, .y = -3264, .angle = 270, .type = 2003, .flags = 23 },
     {.x = 1936, .y = -3264, .angle = 270, .type = 2046, .flags = 23 },
     {.x = 1920, .y = -3712, .angle = 270, .type = 2046, .flags = 23 },
     {.x = 2304, .y = -3712, .angle = 270, .type = 2046, .flags = 23 },
     {.x = 3232, .y = -4160, .angle = 270, .type = 2002, .flags = 23 },
     {.x = 3232, .y = -4192, .angle = 270, .type = 2048, .flags = 23 },
     {.x = 2784, .y = -4192, .angle = 270, .type = 2048, .flags = 23 },
     {.x = 2560, .y = -3904, .angle = 270, .type = 2048, .flags = 23 },
     {.x = 3360, .y = -3536, .angle = 270, .type = 2048, .flags = 23 },
     {.x = 3008, .y = -4768, .angle = 270, .type = 2048, .flags = 23 },
     {.x = 1408, .y = -3584, .angle = 270, .type = 2049, .flags = 23 },
     {.x = 1408, .y = -3456, .angle = 270, .type = 2049, .flags = 23 },
     {.x = 1408, .y = -3328, .angle = 270, .type = 2049, .flags = 23 },
     {.x = 2272, .y = -2432, .angle = 180, .type = 3004, .flags = 15 },
     {.x = 2272, .y = -2352, .angle = 180, .type = 3004, .flags = 14 },
     {.x = 2272, .y = -2512, .angle = 180, .type = 9, .flags = 12 },
     {.x = 2464, .y = -2432, .angle = 180, .type = 9, .flags = 12 },
     {.x = 2224, .y = -2320, .angle = 180, .type = 2015, .flags = 7 },
     {.x = 2224, .y = -2544, .angle = 180, .type = 2015, .flags = 7 },
     {.x = 2288, .y = -2544, .angle = 180, .type = 2015, .flags = 7 },
     {.x = 2288, .y = -2320, .angle = 180, .type = 2015, .flags = 7 },
     {.x = 1712, .y = -2144, .angle = 180, .type = 2015, .flags = 7 },
     {.x = 2240, .y = -2256, .angle = 180, .type = 2015, .flags = 7 },
     {.x = 3264, .y = -3856, .angle = 270, .type = 2049, .flags = 7 },
     {.x = 3264, .y = -3936, .angle = 270, .type = 2001, .flags = 7 },
     {.x = 3232, .y = -3808, .angle = 270, .type = 2012, .flags = 7 },
     {.x = 3216, .y = -3888, .angle = 0, .type = 9, .flags = 12 },
     {.x = -160, .y = -3232, .angle = 0, .type = 9, .flags = 12 },
     {.x = 240, .y = -3376, .angle = 135, .type = 9, .flags = 12 },
     {.x = 240, .y = -3088, .angle = 225, .type = 9, .flags = 12 },
     {.x = 1696, .y = -2688, .angle = 90, .type = 9, .flags = 12 },
     {.x = 1920, .y = -2176, .angle = 270, .type = 9, .flags = 12 },
     {.x = 2976, .y = -3264, .angle = 90, .type = 9, .flags = 12 },
     {.x = 3008, .y = -3520, .angle = 90, .type = 9, .flags = 12 },
     {.x = 3008, .y = -3072, .angle = 90, .type = 9, .flags = 12 },
     {.x = 2816, .y = -2688, .angle = 0, .type = 9, .flags = 12 },
     {.x = 2496, .y = -3968, .angle = 0, .type = 9, .flags = 12 },
     {.x = 2256, .y = -4064, .angle = 0, .type = 9, .flags = 4 },
     {.x = 3072, .y = -4256, .angle = 90, .type = 9, .flags = 12 },
     {.x = -192, .y = -3296, .angle = 0, .type = 9, .flags = 12 },
     {.x = 1024, .y = -3264, .angle = 270, .type = 10, .flags = 7 },
     {.x = 1088, .y = -2944, .angle = 270, .type = 15, .flags = 7 },
     {.x = 448, .y = -3328, .angle = 270, .type = 24, .flags = 7 },
     {.x = -96, .y = -3296, .angle = 270, .type = 10, .flags = 7 },
     {.x = 1504, .y = -2496, .angle = 270, .type = 15, .flags = 7 },
     {.x = 2240, .y = -2480, .angle = 270, .type = 15, .flags = 7 },
     {.x = 2112, .y = -2688, .angle = 270, .type = 24, .flags = 7 },
     {.x = 1792, .y = -2144, .angle = 270, .type = 24, .flags = 7 },
     {.x = 2368, .y = -2336, .angle = 270, .type = 24, .flags = 7 },
     {.x = 2800, .y = -2992, .angle = 270, .type = 15, .flags = 7 },
     {.x = 3072, .y = -3488, .angle = 270, .type = 12, .flags = 7 },
     {.x = 2944, .y = -4256, .angle = 270, .type = 12, .flags = 7 },
     {.x = 2976, .y = -3840, .angle = 270, .type = 24, .flags = 7 },
     {.x = 3264, .y = -4512, .angle = 270, .type = 24, .flags = 7 },
     {.x = 2272, .y = -4000, .angle = 270, .type = 24, .flags = 7 },
     {.x = 3648, .y = -3648, .angle = 0, .type = 2049, .flags = 7 },
     {.x = 3616, .y = -3392, .angle = 0, .type = 2015, .flags = 7 },
     {.x = 3712, .y = -3264, .angle = 0, .type = 2015, .flags = 7 },
     {.x = 3472, .y = -2960, .angle = 0, .type = 2015, .flags = 7 },
     {.x = 3376, .y = -3024, .angle = 0, .type = 2015, .flags = 7 },
     {.x = 3568, .y = -2992, .angle = 0, .type = 2015, .flags = 7 },
     {.x = 3616, .y = -3088, .angle = 0, .type = 2015, .flags = 7 },
     {.x = 3664, .y = -3168, .angle = 0, .type = 2015, .flags = 7 },
     {.x = 3648, .y = -3840, .angle = 0, .type = 2015, .flags = 7 },
     {.x = 256, .y = -3424, .angle = 0, .type = 2023, .flags = 19 },
     {.x = 800, .y = -2960, .angle = 90, .type = 2035, .flags = 23 },
     {.x = 768, .y = -3008, .angle = 90, .type = 2035, .flags = 23 },
     {.x = 720, .y = -3040, .angle = 90, .type = 2035, .flags = 23 },
     {.x = 736, .y = -2976, .angle = 90, .type = 2001, .flags = 23 },
 };

static const linedef linedefs[486] = {
    {
                .v1 = 0, .v2 = 1, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 0, .left_sidedef = -1},
     {
                .v1 = 1, .v2 = 2, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 1, .left_sidedef = -1},
     {
                .v1 = 3, .v2 = 0, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 2, .left_sidedef = -1},
     {
                .v1 = 4, .v2 = 3, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 3, .left_sidedef = -1},
     {
                .v1 = 2, .v2 = 5, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 4, .left_sidedef = -1},
     {
                .v1 = 6, .v2 = 4, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 5, .left_sidedef = -1},
     {
                .v1 = 5, .v2 = 7, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 6, .left_sidedef = -1},
     {
                .v1 = 8, .v2 = 6, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 7, .left_sidedef = -1},
     {
                .v1 = 7, .v2 = 9, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 8, .left_sidedef = -1},
     {
                .v1 = 10, .v2 = 11, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 9, .left_sidedef = -1},
     {
                .v1 = 11, .v2 = 12, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 10, .left_sidedef = -1},
     {
                .v1 = 12, .v2 = 13, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 11, .left_sidedef = -1},
     {
                .v1 = 13, .v2 = 10, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 12, .left_sidedef = -1},
     {
                .v1 = 14, .v2 = 15, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 13, .left_sidedef = -1},
     {
                .v1 = 15, .v2 = 16, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 14, .left_sidedef = -1},
     {
                .v1 = 16, .v2 = 17, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 15, .left_sidedef = -1},
     {
                .v1 = 17, .v2 = 14, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 16, .left_sidedef = -1},
     {
                .v1 = 18, .v2 = 19, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 17, .left_sidedef = -1},
     {
                .v1 = 19, .v2 = 20, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 18, .left_sidedef = -1},
     {
                .v1 = 20, .v2 = 21, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 19, .left_sidedef = -1},
     {
                .v1 = 21, .v2 = 18, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 20, .left_sidedef = -1},
     {
                .v1 = 22, .v2 = 23, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 21, .left_sidedef = -1},
     {
                .v1 = 23, .v2 = 24, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 22, .left_sidedef = -1},
     {
                .v1 = 24, .v2 = 25, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 23, .left_sidedef = -1},
     {
                .v1 = 25, .v2 = 22, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 24, .left_sidedef = -1},
     {
                .v1 = 26, .v2 = 27, 
                .flags = 29, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 25, .left_sidedef = 26},
     {
                .v1 = 28, .v2 = 26, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 27, .left_sidedef = -1},
     {
                .v1 = 29, .v2 = 30, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 28, .left_sidedef = -1},
     {
                .v1 = 30, .v2 = 28, 
                .flags = 29, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 29, .left_sidedef = 30},
     {
                .v1 = 31, .v2 = 32, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 31, .left_sidedef = 32},
     {
                .v1 = 33, .v2 = 34, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 33, .left_sidedef = 34},
     {
                .v1 = 26, .v2 = 34, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 35, .left_sidedef = -1},
     {
                .v1 = 33, .v2 = 27, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 36, .left_sidedef = -1},
     {
                .v1 = 31, .v2 = 28, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 37, .left_sidedef = -1},
     {
                .v1 = 30, .v2 = 32, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 38, .left_sidedef = -1},
     {
                .v1 = 34, .v2 = 31, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 39, .left_sidedef = -1},
     {
                .v1 = 32, .v2 = 35, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 40, .left_sidedef = -1},
     {
                .v1 = 14, .v2 = 11, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 41, .left_sidedef = 42},
     {
                .v1 = 27, .v2 = 15, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 43, .left_sidedef = 44},
     {
                .v1 = 22, .v2 = 30, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 45, .left_sidedef = 46},
     {
                .v1 = 36, .v2 = 37, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 47, .left_sidedef = -1},
     {
                .v1 = 37, .v2 = 38, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 48, .left_sidedef = -1},
     {
                .v1 = 38, .v2 = 39, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 49, .left_sidedef = -1},
     {
                .v1 = 35, .v2 = 40, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 50, .left_sidedef = -1},
     {
                .v1 = 41, .v2 = 42, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 51, .left_sidedef = -1},
     {
                .v1 = 43, .v2 = 41, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 52, .left_sidedef = -1},
     {
                .v1 = 3, .v2 = 2, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 53, .left_sidedef = 54},
     {
                .v1 = 20, .v2 = 24, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 55, .left_sidedef = 56},
     {
                .v1 = 17, .v2 = 12, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 57, .left_sidedef = 58},
     {
                .v1 = 12, .v2 = 20, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 59, .left_sidedef = 60},
     {
                .v1 = 13, .v2 = 19, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 61, .left_sidedef = 62},
     {
                .v1 = 21, .v2 = 23, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 63, .left_sidedef = 64},
     {
                .v1 = 9, .v2 = 44, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 65, .left_sidedef = -1},
     {
                .v1 = 45, .v2 = 36, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 66, .left_sidedef = -1},
     {
                .v1 = 45, .v2 = 44, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 67, .left_sidedef = 68},
     {
                .v1 = 46, .v2 = 47, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 69, .left_sidedef = -1},
     {
                .v1 = 48, .v2 = 49, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 70, .left_sidedef = -1},
     {
                .v1 = 49, .v2 = 50, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 71, .left_sidedef = -1},
     {
                .v1 = 50, .v2 = 45, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 72, .left_sidedef = -1},
     {
                .v1 = 44, .v2 = 51, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 73, .left_sidedef = -1},
     {
                .v1 = 51, .v2 = 46, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 74, .left_sidedef = -1},
     {
                .v1 = 52, .v2 = 53, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 75, .left_sidedef = 76},
     {
                .v1 = 47, .v2 = 53, 
                .flags = 17, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 77, .left_sidedef = -1},
     {
                .v1 = 52, .v2 = 48, 
                .flags = 17, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 78, .left_sidedef = -1},
     {
                .v1 = 53, .v2 = 54, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 79, .left_sidedef = -1},
     {
                .v1 = 54, .v2 = 55, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 80, .left_sidedef = -1},
     {
                .v1 = 55, .v2 = 56, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 81, .left_sidedef = -1},
     {
                .v1 = 57, .v2 = 58, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 82, .left_sidedef = -1},
     {
                .v1 = 59, .v2 = 52, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 83, .left_sidedef = -1},
     {
                .v1 = 58, .v2 = 59, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 84, .left_sidedef = -1},
     {
                .v1 = 60, .v2 = 61, 
                .flags = 20, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 85, .left_sidedef = 86},
     {
                .v1 = 61, .v2 = 62, 
                .flags = 20, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 87, .left_sidedef = 88},
     {
                .v1 = 62, .v2 = 63, 
                .flags = 20, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 89, .left_sidedef = 90},
     {
                .v1 = 63, .v2 = 64, 
                .flags = 20, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 91, .left_sidedef = 92},
     {
                .v1 = 64, .v2 = 65, 
                .flags = 20, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 93, .left_sidedef = 94},
     {
                .v1 = 65, .v2 = 66, 
                .flags = 20, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 95, .left_sidedef = 96},
     {
                .v1 = 66, .v2 = 67, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 97, .left_sidedef = 98},
     {
                .v1 = 67, .v2 = 68, 
                .flags = 20, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 99, .left_sidedef = 100},
     {
                .v1 = 68, .v2 = 69, 
                .flags = 20, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 101, .left_sidedef = 102},
     {
                .v1 = 69, .v2 = 70, 
                .flags = 20, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 103, .left_sidedef = 104},
     {
                .v1 = 70, .v2 = 71, 
                .flags = 20, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 105, .left_sidedef = 106},
     {
                .v1 = 71, .v2 = 72, 
                .flags = 20, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 107, .left_sidedef = 108},
     {
                .v1 = 72, .v2 = 73, 
                .flags = 20, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 109, .left_sidedef = 110},
     {
                .v1 = 61, .v2 = 72, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 111, .left_sidedef = 112},
     {
                .v1 = 62, .v2 = 71, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 113, .left_sidedef = 114},
     {
                .v1 = 63, .v2 = 70, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 115, .left_sidedef = 116},
     {
                .v1 = 64, .v2 = 69, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 117, .left_sidedef = 118},
     {
                .v1 = 65, .v2 = 68, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 119, .left_sidedef = 120},
     {
                .v1 = 60, .v2 = 73, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 121, .left_sidedef = 122},
     {
                .v1 = 73, .v2 = 74, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 123, .left_sidedef = 124},
     {
                .v1 = 74, .v2 = 57, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 125, .left_sidedef = -1},
     {
                .v1 = 56, .v2 = 75, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 126, .left_sidedef = -1},
     {
                .v1 = 75, .v2 = 60, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 127, .left_sidedef = 128},
     {
                .v1 = 75, .v2 = 76, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 129, .left_sidedef = -1},
     {
                .v1 = 76, .v2 = 77, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 130, .left_sidedef = -1},
     {
                .v1 = 78, .v2 = 79, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 131, .left_sidedef = -1},
     {
                .v1 = 79, .v2 = 74, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 132, .left_sidedef = -1},
     {
                .v1 = 80, .v2 = 81, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 133, .left_sidedef = -1},
     {
                .v1 = 82, .v2 = 83, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 134, .left_sidedef = -1},
     {
                .v1 = 84, .v2 = 85, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 135, .left_sidedef = -1},
     {
                .v1 = 83, .v2 = 86, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 136, .left_sidedef = -1},
     {
                .v1 = 87, .v2 = 80, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 137, .left_sidedef = -1},
     {
                .v1 = 88, .v2 = 89, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 138, .left_sidedef = -1},
     {
                .v1 = 81, .v2 = 90, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 139, .left_sidedef = -1},
     {
                .v1 = 91, .v2 = 82, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 140, .left_sidedef = -1},
     {
                .v1 = 86, .v2 = 92, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 141, .left_sidedef = -1},
     {
                .v1 = 92, .v2 = 91, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 142, .left_sidedef = -1},
     {
                .v1 = 90, .v2 = 93, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 143, .left_sidedef = -1},
     {
                .v1 = 93, .v2 = 87, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 144, .left_sidedef = -1},
     {
                .v1 = 89, .v2 = 94, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 145, .left_sidedef = -1},
     {
                .v1 = 94, .v2 = 95, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 146, .left_sidedef = -1},
     {
                .v1 = 96, .v2 = 84, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 147, .left_sidedef = -1},
     {
                .v1 = 97, .v2 = 96, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 148, .left_sidedef = -1},
     {
                .v1 = 78, .v2 = 77, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 149, .left_sidedef = 150},
     {
                .v1 = 88, .v2 = 80, 
                .flags = 29, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 151, .left_sidedef = 152},
     {
                .v1 = 87, .v2 = 89, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 153, .left_sidedef = 154},
     {
                .v1 = 83, .v2 = 85, 
                .flags = 29, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 155, .left_sidedef = 156},
     {
                .v1 = 84, .v2 = 86, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 157, .left_sidedef = 158},
     {
                .v1 = 81, .v2 = 82, 
                .flags = 29, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 159, .left_sidedef = 160},
     {
                .v1 = 91, .v2 = 90, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 161, .left_sidedef = 162},
     {
                .v1 = 98, .v2 = 97, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 163, .left_sidedef = -1},
     {
                .v1 = 95, .v2 = 99, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 164, .left_sidedef = -1},
     {
                .v1 = 99, .v2 = 100, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 165, .left_sidedef = -1},
     {
                .v1 = 100, .v2 = 101, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 166, .left_sidedef = -1},
     {
                .v1 = 101, .v2 = 102, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 167, .left_sidedef = -1},
     {
                .v1 = 102, .v2 = 103, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 168, .left_sidedef = -1},
     {
                .v1 = 103, .v2 = 98, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 169, .left_sidedef = -1},
     {
                .v1 = 99, .v2 = 104, 
                .flags = 156, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 170, .left_sidedef = 171},
     {
                .v1 = 104, .v2 = 105, 
                .flags = 156, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 172, .left_sidedef = 173},
     {
                .v1 = 105, .v2 = 98, 
                .flags = 156, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 174, .left_sidedef = 175},
     {
                .v1 = 85, .v2 = 106, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 176, .left_sidedef = -1},
     {
                .v1 = 106, .v2 = 78, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 177, .left_sidedef = -1},
     {
                .v1 = 77, .v2 = 107, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 178, .left_sidedef = -1},
     {
                .v1 = 107, .v2 = 88, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 179, .left_sidedef = -1},
     {
                .v1 = 108, .v2 = 109, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 180, .left_sidedef = 181},
     {
                .v1 = 109, .v2 = 110, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 182, .left_sidedef = 183},
     {
                .v1 = 110, .v2 = 111, 
                .flags = 28, .line_type = 63, .sector_tag = 3, 
                .right_sidedef = 184, .left_sidedef = 185},
     {
                .v1 = 111, .v2 = 108, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 186, .left_sidedef = 187},
     {
                .v1 = 112, .v2 = 113, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 188, .left_sidedef = 189},
     {
                .v1 = 113, .v2 = 114, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 190, .left_sidedef = 191},
     {
                .v1 = 114, .v2 = 115, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 192, .left_sidedef = 193},
     {
                .v1 = 115, .v2 = 112, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 194, .left_sidedef = 195},
     {
                .v1 = 39, .v2 = 116, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 196, .left_sidedef = -1},
     {
                .v1 = 117, .v2 = 29, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 197, .left_sidedef = -1},
     {
                .v1 = 118, .v2 = 117, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 198, .left_sidedef = -1},
     {
                .v1 = 116, .v2 = 119, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 199, .left_sidedef = -1},
     {
                .v1 = 29, .v2 = 39, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 200, .left_sidedef = 201},
     {
                .v1 = 119, .v2 = 118, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 202, .left_sidedef = 203},
     {
                .v1 = 120, .v2 = 121, 
                .flags = 4, .line_type = 1, .sector_tag = 0, 
                .right_sidedef = 204, .left_sidedef = 205},
     {
                .v1 = 122, .v2 = 123, 
                .flags = 4, .line_type = 1, .sector_tag = 0, 
                .right_sidedef = 206, .left_sidedef = 207},
     {
                .v1 = 121, .v2 = 118, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 208, .left_sidedef = -1},
     {
                .v1 = 119, .v2 = 120, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 209, .left_sidedef = -1},
     {
                .v1 = 120, .v2 = 123, 
                .flags = 17, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 210, .left_sidedef = -1},
     {
                .v1 = 122, .v2 = 121, 
                .flags = 17, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 211, .left_sidedef = -1},
     {
                .v1 = 124, .v2 = 122, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 212, .left_sidedef = -1},
     {
                .v1 = 123, .v2 = 125, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 213, .left_sidedef = -1},
     {
                .v1 = 126, .v2 = 127, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 214, .left_sidedef = -1},
     {
                .v1 = 127, .v2 = 128, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 215, .left_sidedef = -1},
     {
                .v1 = 129, .v2 = 43, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 216, .left_sidedef = -1},
     {
                .v1 = 130, .v2 = 131, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 217, .left_sidedef = 218},
     {
                .v1 = 132, .v2 = 133, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 219, .left_sidedef = 220},
     {
                .v1 = 133, .v2 = 134, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 221, .left_sidedef = 222},
     {
                .v1 = 134, .v2 = 135, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 223, .left_sidedef = 224},
     {
                .v1 = 135, .v2 = 136, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 225, .left_sidedef = 226},
     {
                .v1 = 136, .v2 = 137, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 227, .left_sidedef = 228},
     {
                .v1 = 137, .v2 = 138, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 229, .left_sidedef = 230},
     {
                .v1 = 138, .v2 = 139, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 231, .left_sidedef = 232},
     {
                .v1 = 139, .v2 = 132, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 233, .left_sidedef = 234},
     {
                .v1 = 140, .v2 = 141, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 235, .left_sidedef = -1},
     {
                .v1 = 142, .v2 = 143, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 236, .left_sidedef = -1},
     {
                .v1 = 143, .v2 = 144, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 237, .left_sidedef = -1},
     {
                .v1 = 145, .v2 = 146, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 238, .left_sidedef = -1},
     {
                .v1 = 147, .v2 = 148, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 239, .left_sidedef = -1},
     {
                .v1 = 149, .v2 = 150, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 240, .left_sidedef = -1},
     {
                .v1 = 151, .v2 = 152, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 241, .left_sidedef = -1},
     {
                .v1 = 153, .v2 = 126, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 242, .left_sidedef = -1},
     {
                .v1 = 152, .v2 = 153, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 243, .left_sidedef = -1},
     {
                .v1 = 148, .v2 = 154, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 244, .left_sidedef = -1},
     {
                .v1 = 155, .v2 = 154, 
                .flags = 6, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 245, .left_sidedef = 246},
     {
                .v1 = 156, .v2 = 157, 
                .flags = 6, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 247, .left_sidedef = 248},
     {
                .v1 = 158, .v2 = 156, 
                .flags = 6, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 249, .left_sidedef = 250},
     {
                .v1 = 159, .v2 = 160, 
                .flags = 6, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 251, .left_sidedef = 252},
     {
                .v1 = 161, .v2 = 158, 
                .flags = 6, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 253, .left_sidedef = 254},
     {
                .v1 = 160, .v2 = 155, 
                .flags = 6, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 255, .left_sidedef = 256},
     {
                .v1 = 162, .v2 = 163, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 257, .left_sidedef = -1},
     {
                .v1 = 164, .v2 = 151, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 258, .left_sidedef = -1},
     {
                .v1 = 163, .v2 = 165, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 259, .left_sidedef = -1},
     {
                .v1 = 166, .v2 = 164, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 260, .left_sidedef = -1},
     {
                .v1 = 140, .v2 = 144, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 261, .left_sidedef = 262},
     {
                .v1 = 152, .v2 = 157, 
                .flags = 132, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 263, .left_sidedef = 264},
     {
                .v1 = 157, .v2 = 163, 
                .flags = 132, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 265, .left_sidedef = 266},
     {
                .v1 = 149, .v2 = 147, 
                .flags = 132, .line_type = 88, .sector_tag = 2, 
                .right_sidedef = 267, .left_sidedef = 268},
     {
                .v1 = 157, .v2 = 151, 
                .flags = 6, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 269, .left_sidedef = 270},
     {
                .v1 = 167, .v2 = 159, 
                .flags = 6, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 271, .left_sidedef = 272},
     {
                .v1 = 162, .v2 = 167, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 273, .left_sidedef = 274},
     {
                .v1 = 168, .v2 = 162, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 275, .left_sidedef = -1},
     {
                .v1 = 169, .v2 = 170, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 276, .left_sidedef = 277},
     {
                .v1 = 170, .v2 = 171, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 278, .left_sidedef = 279},
     {
                .v1 = 171, .v2 = 168, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 280, .left_sidedef = 281},
     {
                .v1 = 128, .v2 = 172, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 282, .left_sidedef = -1},
     {
                .v1 = 172, .v2 = 173, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 283, .left_sidedef = -1},
     {
                .v1 = 174, .v2 = 129, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 284, .left_sidedef = -1},
     {
                .v1 = 175, .v2 = 174, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 285, .left_sidedef = -1},
     {
                .v1 = 176, .v2 = 177, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 286, .left_sidedef = -1},
     {
                .v1 = 177, .v2 = 178, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 287, .left_sidedef = -1},
     {
                .v1 = 179, .v2 = 180, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 288, .left_sidedef = -1},
     {
                .v1 = 181, .v2 = 179, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 289, .left_sidedef = -1},
     {
                .v1 = 182, .v2 = 181, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 290, .left_sidedef = -1},
     {
                .v1 = 183, .v2 = 182, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 291, .left_sidedef = -1},
     {
                .v1 = 178, .v2 = 184, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 292, .left_sidedef = -1},
     {
                .v1 = 184, .v2 = 185, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 293, .left_sidedef = -1},
     {
                .v1 = 186, .v2 = 187, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 294, .left_sidedef = -1},
     {
                .v1 = 173, .v2 = 175, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 295, .left_sidedef = 296},
     {
                .v1 = 180, .v2 = 176, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 297, .left_sidedef = 298},
     {
                .v1 = 179, .v2 = 177, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 299, .left_sidedef = 300},
     {
                .v1 = 181, .v2 = 178, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 301, .left_sidedef = 302},
     {
                .v1 = 182, .v2 = 184, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 303, .left_sidedef = 304},
     {
                .v1 = 183, .v2 = 185, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 305, .left_sidedef = 306},
     {
                .v1 = 186, .v2 = 188, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 307, .left_sidedef = 308},
     {
                .v1 = 189, .v2 = 190, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 309, .left_sidedef = 310},
     {
                .v1 = 191, .v2 = 192, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 311, .left_sidedef = 312},
     {
                .v1 = 193, .v2 = 194, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 313, .left_sidedef = 314},
     {
                .v1 = 195, .v2 = 196, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 315, .left_sidedef = 316},
     {
                .v1 = 197, .v2 = 198, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 317, .left_sidedef = 318},
     {
                .v1 = 199, .v2 = 200, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 319, .left_sidedef = 320},
     {
                .v1 = 201, .v2 = 202, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 321, .left_sidedef = 322},
     {
                .v1 = 203, .v2 = 204, 
                .flags = 14, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 323, .left_sidedef = 324},
     {
                .v1 = 203, .v2 = 201, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 325, .left_sidedef = -1},
     {
                .v1 = 202, .v2 = 204, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 326, .left_sidedef = -1},
     {
                .v1 = 201, .v2 = 199, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 327, .left_sidedef = -1},
     {
                .v1 = 199, .v2 = 197, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 328, .left_sidedef = -1},
     {
                .v1 = 197, .v2 = 195, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 329, .left_sidedef = -1},
     {
                .v1 = 195, .v2 = 193, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 330, .left_sidedef = -1},
     {
                .v1 = 193, .v2 = 191, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 331, .left_sidedef = -1},
     {
                .v1 = 192, .v2 = 194, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 332, .left_sidedef = -1},
     {
                .v1 = 194, .v2 = 196, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 333, .left_sidedef = -1},
     {
                .v1 = 196, .v2 = 198, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 334, .left_sidedef = -1},
     {
                .v1 = 198, .v2 = 200, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 335, .left_sidedef = -1},
     {
                .v1 = 200, .v2 = 202, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 336, .left_sidedef = -1},
     {
                .v1 = 191, .v2 = 205, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 337, .left_sidedef = -1},
     {
                .v1 = 206, .v2 = 192, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 338, .left_sidedef = -1},
     {
                .v1 = 207, .v2 = 206, 
                .flags = 17, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 339, .left_sidedef = -1},
     {
                .v1 = 205, .v2 = 166, 
                .flags = 17, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 340, .left_sidedef = -1},
     {
                .v1 = 207, .v2 = 166, 
                .flags = 36, .line_type = 1, .sector_tag = 0, 
                .right_sidedef = 341, .left_sidedef = 342},
     {
                .v1 = 205, .v2 = 206, 
                .flags = 4, .line_type = 1, .sector_tag = 0, 
                .right_sidedef = 343, .left_sidedef = 344},
     {
                .v1 = 208, .v2 = 207, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 345, .left_sidedef = -1},
     {
                .v1 = 126, .v2 = 209, 
                .flags = 156, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 346, .left_sidedef = 347},
     {
                .v1 = 209, .v2 = 210, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 348, .left_sidedef = 349},
     {
                .v1 = 210, .v2 = 42, 
                .flags = 156, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 350, .left_sidedef = 351},
     {
                .v1 = 210, .v2 = 129, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 352, .left_sidedef = 353},
     {
                .v1 = 128, .v2 = 209, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 354, .left_sidedef = 355},
     {
                .v1 = 204, .v2 = 211, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 356, .left_sidedef = -1},
     {
                .v1 = 211, .v2 = 190, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 357, .left_sidedef = -1},
     {
                .v1 = 189, .v2 = 212, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 358, .left_sidedef = -1},
     {
                .v1 = 212, .v2 = 203, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 359, .left_sidedef = -1},
     {
                .v1 = 185, .v2 = 213, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 360, .left_sidedef = -1},
     {
                .v1 = 213, .v2 = 188, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 361, .left_sidedef = -1},
     {
                .v1 = 187, .v2 = 214, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 362, .left_sidedef = -1},
     {
                .v1 = 214, .v2 = 183, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 363, .left_sidedef = -1},
     {
                .v1 = 149, .v2 = 161, 
                .flags = 6, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 364, .left_sidedef = 365},
     {
                .v1 = 146, .v2 = 215, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 366, .left_sidedef = -1},
     {
                .v1 = 215, .v2 = 140, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 367, .left_sidedef = -1},
     {
                .v1 = 144, .v2 = 216, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 368, .left_sidedef = -1},
     {
                .v1 = 216, .v2 = 147, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 369, .left_sidedef = -1},
     {
                .v1 = 217, .v2 = 169, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 370, .left_sidedef = -1},
     {
                .v1 = 169, .v2 = 218, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 371, .left_sidedef = -1},
     {
                .v1 = 218, .v2 = 168, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 372, .left_sidedef = 373},
     {
                .v1 = 219, .v2 = 220, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 374, .left_sidedef = -1},
     {
                .v1 = 220, .v2 = 221, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 375, .left_sidedef = -1},
     {
                .v1 = 221, .v2 = 222, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 376, .left_sidedef = -1},
     {
                .v1 = 222, .v2 = 223, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 377, .left_sidedef = -1},
     {
                .v1 = 152, .v2 = 223, 
                .flags = 29, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 378, .left_sidedef = 379},
     {
                .v1 = 222, .v2 = 153, 
                .flags = 29, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 380, .left_sidedef = 381},
     {
                .v1 = 223, .v2 = 149, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 382, .left_sidedef = -1},
     {
                .v1 = 224, .v2 = 225, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 383, .left_sidedef = -1},
     {
                .v1 = 226, .v2 = 227, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 384, .left_sidedef = -1},
     {
                .v1 = 227, .v2 = 228, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 385, .left_sidedef = -1},
     {
                .v1 = 229, .v2 = 230, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 386, .left_sidedef = -1},
     {
                .v1 = 231, .v2 = 232, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 387, .left_sidedef = -1},
     {
                .v1 = 233, .v2 = 224, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 388, .left_sidedef = -1},
     {
                .v1 = 234, .v2 = 235, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 389, .left_sidedef = -1},
     {
                .v1 = 235, .v2 = 236, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 390, .left_sidedef = -1},
     {
                .v1 = 236, .v2 = 237, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 391, .left_sidedef = -1},
     {
                .v1 = 238, .v2 = 239, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 392, .left_sidedef = -1},
     {
                .v1 = 239, .v2 = 240, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 393, .left_sidedef = -1},
     {
                .v1 = 240, .v2 = 241, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 394, .left_sidedef = -1},
     {
                .v1 = 228, .v2 = 242, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 395, .left_sidedef = -1},
     {
                .v1 = 242, .v2 = 229, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 396, .left_sidedef = -1},
     {
                .v1 = 237, .v2 = 243, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 397, .left_sidedef = -1},
     {
                .v1 = 243, .v2 = 234, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 398, .left_sidedef = -1},
     {
                .v1 = 232, .v2 = 244, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 399, .left_sidedef = -1},
     {
                .v1 = 244, .v2 = 233, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 400, .left_sidedef = -1},
     {
                .v1 = 241, .v2 = 245, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 401, .left_sidedef = -1},
     {
                .v1 = 245, .v2 = 238, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 402, .left_sidedef = -1},
     {
                .v1 = 245, .v2 = 246, 
                .flags = 5, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 403, .left_sidedef = 404},
     {
                .v1 = 246, .v2 = 247, 
                .flags = 5, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 405, .left_sidedef = 406},
     {
                .v1 = 247, .v2 = 242, 
                .flags = 5, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 407, .left_sidedef = 408},
     {
                .v1 = 248, .v2 = 243, 
                .flags = 5, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 409, .left_sidedef = 410},
     {
                .v1 = 244, .v2 = 249, 
                .flags = 5, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 411, .left_sidedef = 412},
     {
                .v1 = 249, .v2 = 248, 
                .flags = 5, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 413, .left_sidedef = 414},
     {
                .v1 = 250, .v2 = 251, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 415, .left_sidedef = -1},
     {
                .v1 = 252, .v2 = 253, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 416, .left_sidedef = -1},
     {
                .v1 = 251, .v2 = 254, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 417, .left_sidedef = -1},
     {
                .v1 = 255, .v2 = 252, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 418, .left_sidedef = -1},
     {
                .v1 = 229, .v2 = 232, 
                .flags = 12, .line_type = 36, .sector_tag = 1, 
                .right_sidedef = 419, .left_sidedef = 420},
     {
                .v1 = 234, .v2 = 241, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 421, .left_sidedef = 422},
     {
                .v1 = 165, .v2 = 208, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 423, .left_sidedef = 424},
     {
                .v1 = 253, .v2 = 226, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 425, .left_sidedef = -1},
     {
                .v1 = 225, .v2 = 250, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 426, .left_sidedef = -1},
     {
                .v1 = 238, .v2 = 228, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 427, .left_sidedef = 428},
     {
                .v1 = 233, .v2 = 237, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 429, .left_sidedef = 430},
     {
                .v1 = 252, .v2 = 251, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 431, .left_sidedef = 432},
     {
                .v1 = 254, .v2 = 256, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 433, .left_sidedef = -1},
     {
                .v1 = 257, .v2 = 255, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 434, .left_sidedef = -1},
     {
                .v1 = 258, .v2 = 257, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 435, .left_sidedef = -1},
     {
                .v1 = 256, .v2 = 259, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 436, .left_sidedef = -1},
     {
                .v1 = 259, .v2 = 260, 
                .flags = 17, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 437, .left_sidedef = -1},
     {
                .v1 = 260, .v2 = 261, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 438, .left_sidedef = -1},
     {
                .v1 = 262, .v2 = 263, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 439, .left_sidedef = -1},
     {
                .v1 = 263, .v2 = 258, 
                .flags = 17, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 440, .left_sidedef = -1},
     {
                .v1 = 263, .v2 = 260, 
                .flags = 4, .line_type = 1, .sector_tag = 0, 
                .right_sidedef = 441, .left_sidedef = 442},
     {
                .v1 = 259, .v2 = 258, 
                .flags = 4, .line_type = 1, .sector_tag = 0, 
                .right_sidedef = 443, .left_sidedef = 444},
     {
                .v1 = 261, .v2 = 264, 
                .flags = 17, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 445, .left_sidedef = -1},
     {
                .v1 = 265, .v2 = 262, 
                .flags = 17, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 446, .left_sidedef = -1},
     {
                .v1 = 264, .v2 = 266, 
                .flags = 17, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 447, .left_sidedef = -1},
     {
                .v1 = 267, .v2 = 268, 
                .flags = 17, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 448, .left_sidedef = -1},
     {
                .v1 = 268, .v2 = 269, 
                .flags = 17, .line_type = 11, .sector_tag = 0, 
                .right_sidedef = 449, .left_sidedef = -1},
     {
                .v1 = 269, .v2 = 265, 
                .flags = 17, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 450, .left_sidedef = -1},
     {
                .v1 = 266, .v2 = 267, 
                .flags = 17, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 451, .left_sidedef = -1},
     {
                .v1 = 262, .v2 = 261, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 452, .left_sidedef = 453},
     {
                .v1 = 270, .v2 = 208, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 454, .left_sidedef = -1},
     {
                .v1 = 165, .v2 = 271, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 455, .left_sidedef = -1},
     {
                .v1 = 230, .v2 = 272, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 456, .left_sidedef = -1},
     {
                .v1 = 272, .v2 = 270, 
                .flags = 17, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 457, .left_sidedef = -1},
     {
                .v1 = 271, .v2 = 273, 
                .flags = 17, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 458, .left_sidedef = -1},
     {
                .v1 = 273, .v2 = 231, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 459, .left_sidedef = -1},
     {
                .v1 = 271, .v2 = 270, 
                .flags = 4, .line_type = 1, .sector_tag = 0, 
                .right_sidedef = 460, .left_sidedef = 461},
     {
                .v1 = 272, .v2 = 273, 
                .flags = 4, .line_type = 1, .sector_tag = 0, 
                .right_sidedef = 462, .left_sidedef = 463},
     {
                .v1 = 274, .v2 = 275, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 464, .left_sidedef = 465},
     {
                .v1 = 276, .v2 = 277, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 466, .left_sidedef = 467},
     {
                .v1 = 277, .v2 = 274, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 468, .left_sidedef = 469},
     {
                .v1 = 275, .v2 = 276, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 470, .left_sidedef = 471},
     {
                .v1 = 256, .v2 = 257, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 472, .left_sidedef = 473},
     {
                .v1 = 278, .v2 = 279, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 474, .left_sidedef = 475},
     {
                .v1 = 280, .v2 = 281, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 476, .left_sidedef = 477},
     {
                .v1 = 281, .v2 = 278, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 478, .left_sidedef = 479},
     {
                .v1 = 279, .v2 = 280, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 480, .left_sidedef = 481},
     {
                .v1 = 150, .v2 = 145, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 482, .left_sidedef = -1},
     {
                .v1 = 282, .v2 = 283, 
                .flags = 28, .line_type = 48, .sector_tag = 0, 
                .right_sidedef = 483, .left_sidedef = 484},
     {
                .v1 = 283, .v2 = 284, 
                .flags = 28, .line_type = 48, .sector_tag = 0, 
                .right_sidedef = 485, .left_sidedef = 486},
     {
                .v1 = 284, .v2 = 285, 
                .flags = 28, .line_type = 48, .sector_tag = 0, 
                .right_sidedef = 487, .left_sidedef = 488},
     {
                .v1 = 285, .v2 = 286, 
                .flags = 28, .line_type = 48, .sector_tag = 0, 
                .right_sidedef = 489, .left_sidedef = 490},
     {
                .v1 = 286, .v2 = 287, 
                .flags = 28, .line_type = 48, .sector_tag = 0, 
                .right_sidedef = 491, .left_sidedef = 492},
     {
                .v1 = 287, .v2 = 288, 
                .flags = 28, .line_type = 48, .sector_tag = 0, 
                .right_sidedef = 493, .left_sidedef = 494},
     {
                .v1 = 288, .v2 = 289, 
                .flags = 28, .line_type = 48, .sector_tag = 0, 
                .right_sidedef = 495, .left_sidedef = 496},
     {
                .v1 = 289, .v2 = 282, 
                .flags = 28, .line_type = 48, .sector_tag = 0, 
                .right_sidedef = 497, .left_sidedef = 498},
     {
                .v1 = 290, .v2 = 291, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 499, .left_sidedef = -1},
     {
                .v1 = 291, .v2 = 292, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 500, .left_sidedef = -1},
     {
                .v1 = 293, .v2 = 294, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 501, .left_sidedef = -1},
     {
                .v1 = 295, .v2 = 296, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 502, .left_sidedef = -1},
     {
                .v1 = 296, .v2 = 297, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 503, .left_sidedef = -1},
     {
                .v1 = 298, .v2 = 299, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 504, .left_sidedef = -1},
     {
                .v1 = 300, .v2 = 301, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 505, .left_sidedef = -1},
     {
                .v1 = 301, .v2 = 302, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 506, .left_sidedef = -1},
     {
                .v1 = 302, .v2 = 303, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 507, .left_sidedef = -1},
     {
                .v1 = 303, .v2 = 295, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 508, .left_sidedef = -1},
     {
                .v1 = 292, .v2 = 300, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 509, .left_sidedef = -1},
     {
                .v1 = 290, .v2 = 304, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 510, .left_sidedef = 511},
     {
                .v1 = 304, .v2 = 305, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 512, .left_sidedef = 513},
     {
                .v1 = 305, .v2 = 297, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 514, .left_sidedef = 515},
     {
                .v1 = 306, .v2 = 307, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 516, .left_sidedef = -1},
     {
                .v1 = 307, .v2 = 308, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 517, .left_sidedef = -1},
     {
                .v1 = 308, .v2 = 293, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 518, .left_sidedef = -1},
     {
                .v1 = 298, .v2 = 309, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 519, .left_sidedef = 520},
     {
                .v1 = 310, .v2 = 311, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 521, .left_sidedef = -1},
     {
                .v1 = 141, .v2 = 312, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 522, .left_sidedef = -1},
     {
                .v1 = 313, .v2 = 142, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 523, .left_sidedef = -1},
     {
                .v1 = 312, .v2 = 310, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 524, .left_sidedef = -1},
     {
                .v1 = 124, .v2 = 125, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 525, .left_sidedef = 526},
     {
                .v1 = 313, .v2 = 310, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 527, .left_sidedef = 528},
     {
                .v1 = 314, .v2 = 315, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 529, .left_sidedef = 530},
     {
                .v1 = 315, .v2 = 316, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 531, .left_sidedef = 532},
     {
                .v1 = 316, .v2 = 317, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 533, .left_sidedef = 534},
     {
                .v1 = 317, .v2 = 314, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 535, .left_sidedef = 536},
     {
                .v1 = 294, .v2 = 318, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 537, .left_sidedef = -1},
     {
                .v1 = 318, .v2 = 124, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 538, .left_sidedef = -1},
     {
                .v1 = 125, .v2 = 319, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 539, .left_sidedef = -1},
     {
                .v1 = 319, .v2 = 290, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 540, .left_sidedef = -1},
     {
                .v1 = 297, .v2 = 320, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 541, .left_sidedef = -1},
     {
                .v1 = 320, .v2 = 313, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 542, .left_sidedef = -1},
     {
                .v1 = 311, .v2 = 321, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 543, .left_sidedef = -1},
     {
                .v1 = 321, .v2 = 298, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 544, .left_sidedef = -1},
     {
                .v1 = 322, .v2 = 323, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 545, .left_sidedef = -1},
     {
                .v1 = 323, .v2 = 324, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 546, .left_sidedef = -1},
     {
                .v1 = 324, .v2 = 325, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 547, .left_sidedef = -1},
     {
                .v1 = 325, .v2 = 326, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 548, .left_sidedef = -1},
     {
                .v1 = 326, .v2 = 327, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 549, .left_sidedef = -1},
     {
                .v1 = 327, .v2 = 328, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 550, .left_sidedef = -1},
     {
                .v1 = 328, .v2 = 329, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 551, .left_sidedef = -1},
     {
                .v1 = 329, .v2 = 330, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 552, .left_sidedef = -1},
     {
                .v1 = 330, .v2 = 331, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 553, .left_sidedef = -1},
     {
                .v1 = 331, .v2 = 332, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 554, .left_sidedef = -1},
     {
                .v1 = 332, .v2 = 333, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 555, .left_sidedef = -1},
     {
                .v1 = 333, .v2 = 334, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 556, .left_sidedef = -1},
     {
                .v1 = 334, .v2 = 335, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 557, .left_sidedef = -1},
     {
                .v1 = 335, .v2 = 322, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 558, .left_sidedef = -1},
     {
                .v1 = 327, .v2 = 334, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 559, .left_sidedef = 560},
     {
                .v1 = 328, .v2 = 333, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 561, .left_sidedef = 562},
     {
                .v1 = 329, .v2 = 332, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 563, .left_sidedef = 564},
     {
                .v1 = 336, .v2 = 337, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 565, .left_sidedef = 566},
     {
                .v1 = 337, .v2 = 338, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 567, .left_sidedef = 568},
     {
                .v1 = 338, .v2 = 339, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 569, .left_sidedef = 570},
     {
                .v1 = 339, .v2 = 336, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 571, .left_sidedef = 572},
     {
                .v1 = 340, .v2 = 341, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 573, .left_sidedef = 574},
     {
                .v1 = 341, .v2 = 342, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 575, .left_sidedef = 576},
     {
                .v1 = 342, .v2 = 343, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 577, .left_sidedef = 578},
     {
                .v1 = 343, .v2 = 340, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 579, .left_sidedef = 580},
     {
                .v1 = 344, .v2 = 345, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 581, .left_sidedef = 582},
     {
                .v1 = 345, .v2 = 346, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 583, .left_sidedef = 584},
     {
                .v1 = 346, .v2 = 347, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 585, .left_sidedef = 586},
     {
                .v1 = 347, .v2 = 344, 
                .flags = 28, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 587, .left_sidedef = 588},
     {
                .v1 = 141, .v2 = 143, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 589, .left_sidedef = 590},
     {
                .v1 = 348, .v2 = 349, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 591, .left_sidedef = -1},
     {
                .v1 = 350, .v2 = 351, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 592, .left_sidedef = -1},
     {
                .v1 = 352, .v2 = 353, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 593, .left_sidedef = -1},
     {
                .v1 = 349, .v2 = 352, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 594, .left_sidedef = 595},
     {
                .v1 = 218, .v2 = 354, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 596, .left_sidedef = -1},
     {
                .v1 = 354, .v2 = 355, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 597, .left_sidedef = -1},
     {
                .v1 = 353, .v2 = 356, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 598, .left_sidedef = -1},
     {
                .v1 = 356, .v2 = 168, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 599, .left_sidedef = -1},
     {
                .v1 = 349, .v2 = 357, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 600, .left_sidedef = -1},
     {
                .v1 = 357, .v2 = 350, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 601, .left_sidedef = -1},
     {
                .v1 = 351, .v2 = 358, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 602, .left_sidedef = -1},
     {
                .v1 = 358, .v2 = 352, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 603, .left_sidedef = -1},
     {
                .v1 = 190, .v2 = 359, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 604, .left_sidedef = -1},
     {
                .v1 = 359, .v2 = 360, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 605, .left_sidedef = -1},
     {
                .v1 = 360, .v2 = 186, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 606, .left_sidedef = -1},
     {
                .v1 = 188, .v2 = 361, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 607, .left_sidedef = -1},
     {
                .v1 = 361, .v2 = 362, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 608, .left_sidedef = -1},
     {
                .v1 = 362, .v2 = 189, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 609, .left_sidedef = -1},
     {
                .v1 = 173, .v2 = 363, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 610, .left_sidedef = -1},
     {
                .v1 = 363, .v2 = 176, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 611, .left_sidedef = -1},
     {
                .v1 = 180, .v2 = 364, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 612, .left_sidedef = -1},
     {
                .v1 = 364, .v2 = 175, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 613, .left_sidedef = -1},
     {
                .v1 = 355, .v2 = 365, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 614, .left_sidedef = -1},
     {
                .v1 = 366, .v2 = 367, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 615, .left_sidedef = -1},
     {
                .v1 = 368, .v2 = 366, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 616, .left_sidedef = -1},
     {
                .v1 = 369, .v2 = 370, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 617, .left_sidedef = -1},
     {
                .v1 = 370, .v2 = 371, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 618, .left_sidedef = -1},
     {
                .v1 = 371, .v2 = 372, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 619, .left_sidedef = -1},
     {
                .v1 = 372, .v2 = 373, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 620, .left_sidedef = -1},
     {
                .v1 = 374, .v2 = 368, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 621, .left_sidedef = -1},
     {
                .v1 = 375, .v2 = 374, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 622, .left_sidedef = -1},
     {
                .v1 = 376, .v2 = 375, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 623, .left_sidedef = -1},
     {
                .v1 = 373, .v2 = 377, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 624, .left_sidedef = -1},
     {
                .v1 = 378, .v2 = 376, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 625, .left_sidedef = -1},
     {
                .v1 = 377, .v2 = 379, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 626, .left_sidedef = -1},
     {
                .v1 = 380, .v2 = 378, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 627, .left_sidedef = -1},
     {
                .v1 = 154, .v2 = 217, 
                .flags = 30, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 628, .left_sidedef = 629},
     {
                .v1 = 365, .v2 = 348, 
                .flags = 14, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 630, .left_sidedef = 631},
     {
                .v1 = 369, .v2 = 367, 
                .flags = 30, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 632, .left_sidedef = 633},
     {
                .v1 = 154, .v2 = 381, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 634, .left_sidedef = -1},
     {
                .v1 = 381, .v2 = 380, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 635, .left_sidedef = -1},
     {
                .v1 = 379, .v2 = 382, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 636, .left_sidedef = -1},
     {
                .v1 = 382, .v2 = 217, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 637, .left_sidedef = -1},
     {
                .v1 = 365, .v2 = 383, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 638, .left_sidedef = -1},
     {
                .v1 = 383, .v2 = 369, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 639, .left_sidedef = -1},
     {
                .v1 = 367, .v2 = 384, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 640, .left_sidedef = -1},
     {
                .v1 = 384, .v2 = 348, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 641, .left_sidedef = -1},
     {
                .v1 = 27, .v2 = 385, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 642, .left_sidedef = -1},
     {
                .v1 = 386, .v2 = 8, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 643, .left_sidedef = -1},
     {
                .v1 = 385, .v2 = 386, 
                .flags = 4, .line_type = 38, .sector_tag = 5, 
                .right_sidedef = 644, .left_sidedef = 645},
     {
                .v1 = 385, .v2 = 387, 
                .flags = 17, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 646, .left_sidedef = -1},
     {
                .v1 = 388, .v2 = 386, 
                .flags = 17, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 647, .left_sidedef = -1},
     {
                .v1 = 42, .v2 = 388, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 648, .left_sidedef = -1},
     {
                .v1 = 387, .v2 = 33, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 649, .left_sidedef = -1},
     {
                .v1 = 388, .v2 = 387, 
                .flags = 4, .line_type = 109, .sector_tag = 4, 
                .right_sidedef = 650, .left_sidedef = 651},
     {
                .v1 = 299, .v2 = 306, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 652, .left_sidedef = 653},
     {
                .v1 = 40, .v2 = 389, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 654, .left_sidedef = -1},
     {
                .v1 = 389, .v2 = 130, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 655, .left_sidedef = 656},
     {
                .v1 = 131, .v2 = 390, 
                .flags = 4, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 657, .left_sidedef = 658},
     {
                .v1 = 390, .v2 = 219, 
                .flags = 1, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 659, .left_sidedef = -1},
     {
                .v1 = 299, .v2 = 390, 
                .flags = 17, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 660, .left_sidedef = -1},
     {
                .v1 = 389, .v2 = 306, 
                .flags = 17, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 661, .left_sidedef = -1},
     {
                .v1 = 306, .v2 = 309, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 662, .left_sidedef = 663},
     {
                .v1 = 309, .v2 = 294, 
                .flags = 12, .line_type = 0, .sector_tag = 0, 
                .right_sidedef = 664, .left_sidedef = 665},
 };

static const sidedef sidedefs[666] = {
    {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOOR3",
                .sector_ref = 30},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "LITE3",
                .sector_ref = 30},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "LITE3",
                .sector_ref = 30},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 48, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 28},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 28},
     {
                .x_off = 0, .y_off = 48, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 27},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 27},
     {
                .x_off = 0, .y_off = 80, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 28},
     {
                .x_off = 0, .y_off = 48, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 28},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 28},
     {
                .x_off = 0, .y_off = 48, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 80, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 28},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 27},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 27},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STARTAN3", .middle_texture = "-",
                .sector_ref = 27},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 3},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 27},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STARTAN3", .middle_texture = "-",
                .sector_ref = 27},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 5},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STARTAN3", .middle_texture = "-",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 5},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STARTAN3", .middle_texture = "-",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 3},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORSTOP",
                .sector_ref = 3},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORSTOP",
                .sector_ref = 3},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORSTOP",
                .sector_ref = 5},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORSTOP",
                .sector_ref = 5},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "COMPUTE2", .lower_texture = "STEP6", .middle_texture = "-",
                .sector_ref = 28},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STEP6", .middle_texture = "-",
                .sector_ref = 27},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STEP6", .middle_texture = "-",
                .sector_ref = 27},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN144",
                .sector_ref = 11},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN144",
                .sector_ref = 11},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STARTAN3", .middle_texture = "-",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 30},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STEP6", .middle_texture = "-",
                .sector_ref = 27},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 28},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STEP6", .middle_texture = "-",
                .sector_ref = 27},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 28},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STEP6", .middle_texture = "-",
                .sector_ref = 27},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 28},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "COMPUTE2", .lower_texture = "STEP6", .middle_texture = "-",
                .sector_ref = 28},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "COMPUTE2", .lower_texture = "STEP6", .middle_texture = "-",
                .sector_ref = 28},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STEP6", .middle_texture = "-",
                .sector_ref = 31},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 31},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 31},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 31},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 31},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 31},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 31},
     {
                .x_off = 0, .y_off = 104, 
                .upper_texture = "STARG3", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 31},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORTRAK",
                .sector_ref = 31},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORTRAK",
                .sector_ref = 31},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARG3",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARG3",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARG3",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARG3",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARG3",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARG3",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "SLADWALL", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 48},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "SLADWALL", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 39},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "SLADWALL", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 38},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "SLADWALL", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 37},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "SLADWALL", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 34},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "SLADWALL", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 33},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STEP6", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 33},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "SLADWALL", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 33},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "SLADWALL", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 34},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "SLADWALL", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 37},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "SLADWALL", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 38},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "SLADWALL", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 39},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "SLADWALL", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 48},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STEP6", .middle_texture = "-",
                .sector_ref = 39},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 48},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STEP6", .middle_texture = "-",
                .sector_ref = 38},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 39},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STEP6", .middle_texture = "-",
                .sector_ref = 37},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 38},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STEP6", .middle_texture = "-",
                .sector_ref = 34},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 37},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STEP6", .middle_texture = "-",
                .sector_ref = 33},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 34},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARG3", .lower_texture = "STARG3", .middle_texture = "-",
                .sector_ref = 48},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 47},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARG3", .lower_texture = "STARG3", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 47},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARG3",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARG3",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARG3", .lower_texture = "STARG3", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 47},
     {
                .x_off = 4, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 47},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARG3",
                .sector_ref = 47},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARG3",
                .sector_ref = 47},
     {
                .x_off = 4, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 47},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 41},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 41},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORSTOP",
                .sector_ref = 43},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORSTOP",
                .sector_ref = 43},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORSTOP",
                .sector_ref = 46},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORSTOP",
                .sector_ref = 46},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORSTOP",
                .sector_ref = 45},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORSTOP",
                .sector_ref = 45},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 42},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 42},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 42},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 42},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 42},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 42},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 42},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 42},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARG3", .lower_texture = "STARG3", .middle_texture = "-",
                .sector_ref = 41},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 47},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STARTAN3", .middle_texture = "-",
                .sector_ref = 41},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 46},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STARTAN3", .middle_texture = "-",
                .sector_ref = 42},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 46},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STARTAN3", .middle_texture = "-",
                .sector_ref = 41},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 43},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STARTAN3", .middle_texture = "-",
                .sector_ref = 42},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 43},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STARTAN3", .middle_texture = "-",
                .sector_ref = 41},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 45},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STARTAN3", .middle_texture = "-",
                .sector_ref = 42},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 45},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 42},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 42},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN144",
                .sector_ref = 40},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN144",
                .sector_ref = 40},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN144",
                .sector_ref = 40},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN144",
                .sector_ref = 40},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN144",
                .sector_ref = 40},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARG3", .lower_texture = "STARG3", .middle_texture = "-",
                .sector_ref = 42},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 40},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARG3", .lower_texture = "STARG3", .middle_texture = "-",
                .sector_ref = 42},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 40},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARG3", .lower_texture = "STARG3", .middle_texture = "-",
                .sector_ref = 42},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 40},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 41},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 41},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 41},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 41},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "TEKWALL4", .lower_texture = "TEKWALL4", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 36},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "TEKWALL4", .lower_texture = "TEKWALL4", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 36},
     {
                .x_off = 0, .y_off = 8, 
                .upper_texture = "TEKWALL4", .lower_texture = "SW1COMP", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 36},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "TEKWALL4", .lower_texture = "TEKWALL4", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 36},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "TEKWALL4", .lower_texture = "TEKWALL4", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 35},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "TEKWALL4", .lower_texture = "TEKWALL4", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 35},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "TEKWALL4", .lower_texture = "TEKWALL4", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 35},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "TEKWALL4", .lower_texture = "TEKWALL4", .middle_texture = "-",
                .sector_ref = 32},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 35},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 24},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 24},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 24},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 24},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STEP6", .middle_texture = "-",
                .sector_ref = 24},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STEP6", .middle_texture = "-",
                .sector_ref = 24},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 25},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BIGDOOR2", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 25},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 26},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BIGDOOR2", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 22},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 26},
     {
                .x_off = 0, .y_off = 56, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 25},
     {
                .x_off = 0, .y_off = 56, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 25},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORTRAK",
                .sector_ref = 26},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORTRAK",
                .sector_ref = 26},
     {
                .x_off = 0, .y_off = 56, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARGR1",
                .sector_ref = 22},
     {
                .x_off = 0, .y_off = 56, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARGR1",
                .sector_ref = 22},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN144",
                .sector_ref = 63},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN144",
                .sector_ref = 63},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN144",
                .sector_ref = 11},
     {
                .x_off = 0, .y_off = 88, 
                .upper_texture = "STARTAN3", .lower_texture = "STARTAN3", .middle_texture = "-",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 2},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "BROWN144", .middle_texture = "-",
                .sector_ref = 0},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "BROWN144", .middle_texture = "-",
                .sector_ref = 0},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "BROWN144", .middle_texture = "-",
                .sector_ref = 0},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "BROWN144", .middle_texture = "-",
                .sector_ref = 0},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "BROWN144", .middle_texture = "-",
                .sector_ref = 0},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "BROWN144", .middle_texture = "-",
                .sector_ref = 0},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "BROWN144", .middle_texture = "-",
                .sector_ref = 0},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "BROWN144", .middle_texture = "-",
                .sector_ref = 0},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 50},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 61},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 50},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 49},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 56},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 49},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 51},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORSTOP",
                .sector_ref = 62},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 56},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "NUKE24", .middle_texture = "-",
                .sector_ref = 53},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 56},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "NUKE24", .middle_texture = "-",
                .sector_ref = 57},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 56},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "NUKE24", .middle_texture = "-",
                .sector_ref = 57},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 56},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "NUKE24", .middle_texture = "-",
                .sector_ref = 53},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 56},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "NUKE24", .middle_texture = "-",
                .sector_ref = 57},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 56},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "NUKE24", .middle_texture = "-",
                .sector_ref = 53},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 56},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 56},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 52},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 52},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 52},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWN1", .lower_texture = "BROWN1", .middle_texture = "-",
                .sector_ref = 49},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 50},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "NUKE24", .middle_texture = "-",
                .sector_ref = 51},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 57},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "NUKE24", .middle_texture = "-",
                .sector_ref = 52},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 56},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "NUKE24", .middle_texture = "-",
                .sector_ref = 56},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 49},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "NUKE24", .middle_texture = "-",
                .sector_ref = 51},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 52},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "NUKE24", .middle_texture = "-",
                .sector_ref = 53},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 56},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "NUKE24", .middle_texture = "-",
                .sector_ref = 53},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 56},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 53},
     {
                .x_off = 0, .y_off = 32, 
                .upper_texture = "BROWNGRN", .lower_texture = "BROWNGRN", .middle_texture = "-",
                .sector_ref = 53},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 55},
     {
                .x_off = 0, .y_off = 32, 
                .upper_texture = "BROWNGRN", .lower_texture = "BROWNGRN", .middle_texture = "-",
                .sector_ref = 53},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 55},
     {
                .x_off = 0, .y_off = 32, 
                .upper_texture = "BROWNGRN", .lower_texture = "BROWNGRN", .middle_texture = "-",
                .sector_ref = 53},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 55},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 12},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 12},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 12},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 12},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 10},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 9},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 10},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 9},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 8},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 7},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 8},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 7},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 6},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWN1", .lower_texture = "BROWN1", .middle_texture = "-",
                .sector_ref = 12},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 13},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWN1", .lower_texture = "BROWN1", .middle_texture = "-",
                .sector_ref = 10},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 13},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWN1", .lower_texture = "BROWN1", .middle_texture = "-",
                .sector_ref = 9},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 10},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWN1", .lower_texture = "BROWN1", .middle_texture = "-",
                .sector_ref = 8},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 9},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWN1", .lower_texture = "BROWN1", .middle_texture = "-",
                .sector_ref = 7},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 8},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWN1", .lower_texture = "BROWN1", .middle_texture = "-",
                .sector_ref = 6},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWN1", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 7},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWN1", .lower_texture = "BROWN1", .middle_texture = "-",
                .sector_ref = 79},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 6},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWN1", .lower_texture = "BROWN1", .middle_texture = "-",
                .sector_ref = 79},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 80},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWN1", .lower_texture = "BROWN1", .middle_texture = "-",
                .sector_ref = 77},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 78},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWN1", .lower_texture = "BROWN1", .middle_texture = "-",
                .sector_ref = 76},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 77},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWN1", .lower_texture = "BROWN1", .middle_texture = "-",
                .sector_ref = 75},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 76},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWN1", .lower_texture = "BROWN1", .middle_texture = "-",
                .sector_ref = 74},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 75},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWN1", .lower_texture = "BROWN1", .middle_texture = "-",
                .sector_ref = 73},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 74},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWN1", .lower_texture = "BROWN1", .middle_texture = "-",
                .sector_ref = 81},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 73},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWN1", .lower_texture = "BROWN1", .middle_texture = "-",
                .sector_ref = 80},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWN1", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 81},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 81},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 81},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 73},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 74},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 75},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 76},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 77},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 77},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 76},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 75},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 74},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 73},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 78},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 78},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORSTOP",
                .sector_ref = 83},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORSTOP",
                .sector_ref = 83},
     {
                .x_off = 0, .y_off = 72, 
                .upper_texture = "BROWN96", .lower_texture = "BROWN96", .middle_texture = "-",
                .sector_ref = 52},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 83},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWN96", .lower_texture = "BROWN96", .middle_texture = "-",
                .sector_ref = 78},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 83},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 52},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STARTAN3", .middle_texture = "-",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 63},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STARTAN3", .middle_texture = "-",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 12},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STARTAN3", .middle_texture = "-",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 11},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STARTAN3", .middle_texture = "-",
                .sector_ref = 11},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 12},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STARTAN3", .middle_texture = "-",
                .sector_ref = 63},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 12},
     {
                .x_off = 4, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 80},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 80},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 80},
     {
                .x_off = 4, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 80},
     {
                .x_off = 4, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 6},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 6},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 6},
     {
                .x_off = 4, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 6},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "NUKE24", .middle_texture = "-",
                .sector_ref = 57},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 56},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 49},
     {
                .x_off = 4, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 49},
     {
                .x_off = 4, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 49},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 49},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 53},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 55},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWNGRN", .lower_texture = "BROWNGRN", .middle_texture = "-",
                .sector_ref = 55},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "BROWNGRN", .middle_texture = "-",
                .sector_ref = 54},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORSTOP",
                .sector_ref = 62},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWNGRN", .lower_texture = "BROWNGRN", .middle_texture = "-",
                .sector_ref = 57},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 62},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN3", .lower_texture = "STARTAN3", .middle_texture = "-",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 62},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 57},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 65},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 65},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 65},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 84},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 84},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 65},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 65},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 65},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 65},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 65},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 65},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 65},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BRNBIGL",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BRNBIGR",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BRNBIGC",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BRNBIGC",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BRNBIGR",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BRNBIGL",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BRNBIGR",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BRNBIGL",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BRNBIGL",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BRNBIGR",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BRNBIGC",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BRNBIGC",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 65},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 65},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "LITE3",
                .sector_ref = 66},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "LITE3",
                .sector_ref = 66},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWNGRN", .lower_texture = "BROWNGRN", .middle_texture = "-",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 84},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWNGRN", .lower_texture = "BROWNGRN", .middle_texture = "-",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 65},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWNGRN", .lower_texture = "BROWNGRN", .middle_texture = "-",
                .sector_ref = 52},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 82},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 65},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 65},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWNGRN", .lower_texture = "BROWNGRN", .middle_texture = "-",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 65},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWNGRN", .lower_texture = "BROWNGRN", .middle_texture = "-",
                .sector_ref = 64},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 65},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWNGRN", .lower_texture = "BROWNGRN", .middle_texture = "-",
                .sector_ref = 66},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 65},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN1",
                .sector_ref = 66},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN1",
                .sector_ref = 66},
     {
                .x_off = 88, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "EXITDOOR",
                .sector_ref = 68},
     {
                .x_off = 64, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "EXITDOOR",
                .sector_ref = 68},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORTRAK",
                .sector_ref = 69},
     {
                .x_off = 88, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "EXITDOOR",
                .sector_ref = 72},
     {
                .x_off = 64, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "EXITDOOR",
                .sector_ref = 72},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORTRAK",
                .sector_ref = 69},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "EXITDOOR", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 72},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 69},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "EXITDOOR", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 68},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 69},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN1",
                .sector_ref = 70},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN1",
                .sector_ref = 70},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN1",
                .sector_ref = 70},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN1",
                .sector_ref = 70},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SW1STRTN",
                .sector_ref = 70},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN1",
                .sector_ref = 70},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN1",
                .sector_ref = 70},
     {
                .x_off = 0, .y_off = 16, 
                .upper_texture = "STARTAN1", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 70},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 72},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 82},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 82},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 84},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORTRAK",
                .sector_ref = 85},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORTRAK",
                .sector_ref = 85},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 84},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BIGDOOR4", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 82},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 85},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BIGDOOR4", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 84},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 85},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "EXITSIGN", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 66},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 67},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "EXITSIGN", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 66},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 67},
     {
                .x_off = 32, .y_off = 0, 
                .upper_texture = "EXITSIGN", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 66},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 67},
     {
                .x_off = 32, .y_off = 0, 
                .upper_texture = "EXITSIGN", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 66},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 67},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "STARTAN1", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 66},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 68},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "EXITSIGN", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 70},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 71},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "EXITSIGN", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 70},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 71},
     {
                .x_off = 32, .y_off = 0, 
                .upper_texture = "EXITSIGN", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 70},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 71},
     {
                .x_off = 32, .y_off = 0, 
                .upper_texture = "EXITSIGN", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 70},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 71},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 49},
     {
                .x_off = 8, .y_off = 0, 
                .upper_texture = "TEKWALL1", .lower_texture = "TEKWALL1", .middle_texture = "-",
                .sector_ref = 41},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 44},
     {
                .x_off = 8, .y_off = 0, 
                .upper_texture = "TEKWALL1", .lower_texture = "TEKWALL1", .middle_texture = "-",
                .sector_ref = 41},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 44},
     {
                .x_off = 8, .y_off = 0, 
                .upper_texture = "TEKWALL1", .lower_texture = "TEKWALL1", .middle_texture = "-",
                .sector_ref = 41},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 44},
     {
                .x_off = 8, .y_off = 0, 
                .upper_texture = "TEKWALL1", .lower_texture = "TEKWALL1", .middle_texture = "-",
                .sector_ref = 41},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 44},
     {
                .x_off = 8, .y_off = 0, 
                .upper_texture = "TEKWALL1", .lower_texture = "TEKWALL1", .middle_texture = "-",
                .sector_ref = 41},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 44},
     {
                .x_off = 8, .y_off = 0, 
                .upper_texture = "TEKWALL1", .lower_texture = "TEKWALL1", .middle_texture = "-",
                .sector_ref = 41},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 44},
     {
                .x_off = 8, .y_off = 0, 
                .upper_texture = "TEKWALL1", .lower_texture = "TEKWALL1", .middle_texture = "-",
                .sector_ref = 41},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 44},
     {
                .x_off = 8, .y_off = 0, 
                .upper_texture = "TEKWALL1", .lower_texture = "TEKWALL1", .middle_texture = "-",
                .sector_ref = 41},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 44},
     {
                .x_off = 0, .y_off = 32, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "LITE3",
                .sector_ref = 21},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "COMPUTE2",
                .sector_ref = 21},
     {
                .x_off = 0, .y_off = 32, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "LITE3",
                .sector_ref = 23},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "COMPUTE2",
                .sector_ref = 21},
     {
                .x_off = 0, .y_off = 32, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "LITE3",
                .sector_ref = 21},
     {
                .x_off = 0, .y_off = 32, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "LITE3",
                .sector_ref = 14},
     {
                .x_off = 0, .y_off = 32, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "LITE3",
                .sector_ref = 21},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "COMPUTE2",
                .sector_ref = 21},
     {
                .x_off = 0, .y_off = 32, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "LITE3",
                .sector_ref = 21},
     {
                .x_off = 0, .y_off = 32, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "LITE3",
                .sector_ref = 21},
     {
                .x_off = 0, .y_off = 32, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "LITE3",
                .sector_ref = 21},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "COMPTALL", .lower_texture = "COMPSPAN", .middle_texture = "-",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 21},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "COMPTALL", .lower_texture = "COMPSPAN", .middle_texture = "-",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 21},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "COMPTALL", .lower_texture = "COMPSPAN", .middle_texture = "-",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 21},
     {
                .x_off = 0, .y_off = 32, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "LITE3",
                .sector_ref = 23},
     {
                .x_off = 0, .y_off = 32, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "LITE3",
                .sector_ref = 23},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "COMPUTE2",
                .sector_ref = 23},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "COMPTALL", .lower_texture = "COMPSPAN", .middle_texture = "-",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 14},
     {
                .x_off = 0, .y_off = 32, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARGR1",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 61},
     {
                .x_off = 0, .y_off = 32, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 61},
     {
                .x_off = 0, .y_off = 32, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 61},
     {
                .x_off = 0, .y_off = 32, 
                .upper_texture = "STARGR1", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 22},
     {
                .x_off = 0, .y_off = 32, 
                .upper_texture = "STARGR1", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "BROWN1", .middle_texture = "-",
                .sector_ref = 61},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "PLANET1", .lower_texture = "STARGR1", .middle_texture = "-",
                .sector_ref = 16},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 17},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "PLANET1", .lower_texture = "STARGR1", .middle_texture = "-",
                .sector_ref = 16},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 17},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "PLANET1", .lower_texture = "STARGR1", .middle_texture = "-",
                .sector_ref = 16},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 17},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "PLANET1", .lower_texture = "STARGR1", .middle_texture = "-",
                .sector_ref = 16},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 17},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 32, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARGR1",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 32, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARGR1",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 32, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARGR1",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 32, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARGR1",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "COMPTILE",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "COMPTILE",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "COMPTILE",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "COMPTILE",
                .sector_ref = 15},
     {
                .x_off = 6, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "COMPTILE",
                .sector_ref = 20},
     {
                .x_off = 96, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "COMPTILE",
                .sector_ref = 59},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "COMPTILE",
                .sector_ref = 60},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "COMPTILE",
                .sector_ref = 60},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "COMPTILE",
                .sector_ref = 60},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "COMPTILE",
                .sector_ref = 59},
     {
                .x_off = 96, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "COMPTILE",
                .sector_ref = 20},
     {
                .x_off = 6, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "COMPTILE",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "COMPTILE", .lower_texture = "STEP1", .middle_texture = "-",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 20},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "STEP1", .middle_texture = "-",
                .sector_ref = 20},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 59},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "STEP1", .middle_texture = "-",
                .sector_ref = 59},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 60},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "PLANET1", .lower_texture = "STARGR1", .middle_texture = "-",
                .sector_ref = 18},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 19},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "PLANET1", .lower_texture = "STARGR1", .middle_texture = "-",
                .sector_ref = 18},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 19},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "PLANET1", .lower_texture = "STARGR1", .middle_texture = "-",
                .sector_ref = 18},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 19},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "PLANET1", .lower_texture = "STARGR1", .middle_texture = "-",
                .sector_ref = 18},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 19},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "PLANET1", .lower_texture = "STARGR1", .middle_texture = "-",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 18},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "PLANET1", .lower_texture = "STARGR1", .middle_texture = "-",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 18},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "PLANET1", .lower_texture = "STARGR1", .middle_texture = "-",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 18},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "PLANET1", .lower_texture = "STARGR1", .middle_texture = "-",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 18},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "PLANET1", .lower_texture = "STARGR1", .middle_texture = "-",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 16},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "PLANET1", .lower_texture = "STARGR1", .middle_texture = "-",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 16},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "PLANET1", .lower_texture = "STARGR1", .middle_texture = "-",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 16},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "PLANET1", .lower_texture = "STARGR1", .middle_texture = "-",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 16},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWN1", .lower_texture = "BROWN1", .middle_texture = "-",
                .sector_ref = 50},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 61},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 54},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 86},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 54},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWNGRN", .lower_texture = "BROWNGRN", .middle_texture = "-",
                .sector_ref = 54},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 86},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 54},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 54},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 54},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 54},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 86},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 86},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 86},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 86},
     {
                .x_off = 0, .y_off = 16, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 79},
     {
                .x_off = 0, .y_off = 16, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 79},
     {
                .x_off = 0, .y_off = 16, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 79},
     {
                .x_off = 0, .y_off = 16, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 79},
     {
                .x_off = 0, .y_off = 16, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 79},
     {
                .x_off = 0, .y_off = 16, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 79},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 13},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 13},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWN1",
                .sector_ref = 13},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 13},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 54},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 58},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 58},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 58},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 58},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 58},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 58},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 58},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 58},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 58},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 58},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 58},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 58},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 58},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWNGRN", .lower_texture = "BROWNGRN", .middle_texture = "BROWNGRN",
                .sector_ref = 53},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWNGRN", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 58},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWNGRN", .lower_texture = "BROWNGRN", .middle_texture = "-",
                .sector_ref = 54},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWNGRN", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 87},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "BROWNGRN", .lower_texture = "BROWNGRN", .middle_texture = "-",
                .sector_ref = 87},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 58},
     {
                .x_off = 4, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 58},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 58},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 58},
     {
                .x_off = 4, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 58},
     {
                .x_off = 4, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 87},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 87},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "BROWNGRN",
                .sector_ref = 87},
     {
                .x_off = 4, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "SUPPORT2",
                .sector_ref = 87},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 72, 
                .upper_texture = "STARTAN3", .lower_texture = "STARTAN3", .middle_texture = "-",
                .sector_ref = 29},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 4},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORSTOP",
                .sector_ref = 4},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORSTOP",
                .sector_ref = 4},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 88, 
                .upper_texture = "STARTAN3", .lower_texture = "STARTAN3", .middle_texture = "-",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 4},
     {
                .x_off = 0, .y_off = 88, 
                .upper_texture = "COMPUTE3", .lower_texture = "COMPUTE3", .middle_texture = "-",
                .sector_ref = 14},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "COMPUTE3", .lower_texture = "COMPUTE3", .middle_texture = "-",
                .sector_ref = 2},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 88, 
                .upper_texture = "STARTAN3", .lower_texture = "STARTAN3", .middle_texture = "-",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 2},
     {
                .x_off = 0, .y_off = 88, 
                .upper_texture = "STARTAN3", .lower_texture = "STARTAN3", .middle_texture = "-",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 2},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "STARTAN3",
                .sector_ref = 1},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORSTOP",
                .sector_ref = 2},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "DOORSTOP",
                .sector_ref = 2},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "COMPTALL", .lower_texture = "COMPSPAN", .middle_texture = "-",
                .sector_ref = 14},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 23},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "COMPTALL", .lower_texture = "COMPSPAN", .middle_texture = "-",
                .sector_ref = 15},
     {
                .x_off = 0, .y_off = 0, 
                .upper_texture = "-", .lower_texture = "-", .middle_texture = "-",
                .sector_ref = 23},
 };

static const vertex vertexes[470] = {
    {.x = 1088, .y = -3680},
     {.x = 1024, .y = -3680},
     {.x = 1024, .y = -3648},
     {.x = 1088, .y = -3648},
     {.x = 1152, .y = -3648},
     {.x = 960, .y = -3648},
     {.x = 1280, .y = -3552},
     {.x = 832, .y = -3552},
     {.x = 1344, .y = -3552},
     {.x = 704, .y = -3552},
     {.x = 896, .y = -3392},
     {.x = 928, .y = -3392},
     {.x = 928, .y = -3360},
     {.x = 896, .y = -3360},
     {.x = 1184, .y = -3392},
     {.x = 1216, .y = -3392},
     {.x = 1216, .y = -3360},
     {.x = 1184, .y = -3360},
     {.x = 896, .y = -3072},
     {.x = 896, .y = -3104},
     {.x = 928, .y = -3104},
     {.x = 928, .y = -3072},
     {.x = 1216, .y = -3072},
     {.x = 1184, .y = -3072},
     {.x = 1184, .y = -3104},
     {.x = 1216, .y = -3104},
     {.x = 1344, .y = -3264},
     {.x = 1344, .y = -3360},
     {.x = 1344, .y = -3200},
     {.x = 1344, .y = -2880},
     {.x = 1344, .y = -3104},
     {.x = 1376, .y = -3200},
     {.x = 1376, .y = -3104},
     {.x = 1376, .y = -3360},
     {.x = 1376, .y = -3264},
     {.x = 1376, .y = -2944},
     {.x = 704, .y = -2944},
     {.x = 832, .y = -2944},
     {.x = 968, .y = -2880},
     {.x = 1216, .y = -2880},
     {.x = 1472, .y = -2880},
     {.x = 1528, .y = -3680},
     {.x = 1376, .y = -3648},
     {.x = 1672, .y = -3744},
     {.x = 704, .y = -3360},
     {.x = 704, .y = -3104},
     {.x = 512, .y = -3328},
     {.x = 512, .y = -3304},
     {.x = 512, .y = -3160},
     {.x = 512, .y = -3136},
     {.x = 680, .y = -3104},
     {.x = 680, .y = -3360},
     {.x = 496, .y = -3160},
     {.x = 496, .y = -3304},
     {.x = 496, .y = -3328},
     {.x = 448, .y = -3456},
     {.x = 128, .y = -3456},
     {.x = 128, .y = -3008},
     {.x = 448, .y = -3008},
     {.x = 496, .y = -3136},
     {.x = 128, .y = -3264},
     {.x = 160, .y = -3264},
     {.x = 192, .y = -3264},
     {.x = 224, .y = -3264},
     {.x = 256, .y = -3264},
     {.x = 288, .y = -3264},
     {.x = 320, .y = -3264},
     {.x = 320, .y = -3200},
     {.x = 288, .y = -3200},
     {.x = 256, .y = -3200},
     {.x = 224, .y = -3200},
     {.x = 192, .y = -3200},
     {.x = 160, .y = -3200},
     {.x = 128, .y = -3200},
     {.x = 64, .y = -3072},
     {.x = 64, .y = -3392},
     {.x = 48, .y = -3392},
     {.x = -64, .y = -3328},
     {.x = -64, .y = -3136},
     {.x = 48, .y = -3072},
     {.x = -256, .y = -3328},
     {.x = -320, .y = -3296},
     {.x = -320, .y = -3168},
     {.x = -256, .y = -3136},
     {.x = -128, .y = -3120},
     {.x = -128, .y = -3136},
     {.x = -256, .y = -3120},
     {.x = -256, .y = -3344},
     {.x = -128, .y = -3328},
     {.x = -128, .y = -3344},
     {.x = -336, .y = -3296},
     {.x = -336, .y = -3168},
     {.x = -336, .y = -3120},
     {.x = -336, .y = -3344},
     {.x = -96, .y = -3344},
     {.x = 64, .y = -3520},
     {.x = -96, .y = -3120},
     {.x = 64, .y = -2944},
     {.x = 64, .y = -2816},
     {.x = 64, .y = -3648},
     {.x = -640, .y = -3648},
     {.x = -768, .y = -3520},
     {.x = -768, .y = -2944},
     {.x = -640, .y = -2816},
     {.x = -640, .y = -3520},
     {.x = -640, .y = -2944},
     {.x = -88, .y = -3136},
     {.x = -88, .y = -3328},
     {.x = 256, .y = -3136},
     {.x = 320, .y = -3136},
     {.x = 320, .y = -3072},
     {.x = 256, .y = -3072},
     {.x = 256, .y = -3392},
     {.x = 320, .y = -3392},
     {.x = 320, .y = -3328},
     {.x = 256, .y = -3328},
     {.x = 1248, .y = -2528},
     {.x = 1384, .y = -2592},
     {.x = 1472, .y = -2560},
     {.x = 1472, .y = -2432},
     {.x = 1536, .y = -2432},
     {.x = 1536, .y = -2560},
     {.x = 1552, .y = -2560},
     {.x = 1552, .y = -2432},
     {.x = 1664, .y = -2560},
     {.x = 1664, .y = -2432},
     {.x = 2736, .y = -3648},
     {.x = 2488, .y = -3744},
     {.x = 2240, .y = -3776},
     {.x = 1984, .y = -3776},
     {.x = 1856, .y = -2880},
     {.x = 1920, .y = -2920},
     {.x = 1520, .y = -3168},
     {.x = 1672, .y = -3104},
     {.x = 1896, .y = -3104},
     {.x = 2040, .y = -3144},
     {.x = 2128, .y = -3272},
     {.x = 2064, .y = -3408},
     {.x = 1784, .y = -3448},
     {.x = 1544, .y = -3384},
     {.x = 2752, .y = -2784},
     {.x = 2624, .y = -2784},
     {.x = 2520, .y = -2560},
     {.x = 2752, .y = -2560},
     {.x = 2944, .y = -2656},
     {.x = 2880, .y = -2912},
     {.x = 2880, .y = -2880},
     {.x = 3048, .y = -2880},
     {.x = 3048, .y = -2944},
     {.x = 2752, .y = -3048},
     {.x = 2752, .y = -2912},
     {.x = 2752, .y = -3584},
     {.x = 2752, .y = -3360},
     {.x = 2736, .y = -3360},
     {.x = 3304, .y = -3040},
     {.x = 3136, .y = -3072},
     {.x = 3112, .y = -3360},
     {.x = 2944, .y = -3536},
     {.x = 2816, .y = -3232},
     {.x = 3280, .y = -3320},
     {.x = 2984, .y = -3200},
     {.x = 2976, .y = -3072},
     {.x = 3264, .y = -3616},
     {.x = 3072, .y = -3648},
     {.x = 2944, .y = -3648},
     {.x = 3072, .y = -4000},
     {.x = 2944, .y = -3776},
     {.x = 3104, .y = -3552},
     {.x = 3352, .y = -3568},
     {.x = 3472, .y = -3432},
     {.x = 3408, .y = -3432},
     {.x = 3312, .y = -3496},
     {.x = 2208, .y = -3680},
     {.x = 2176, .y = -3680},
     {.x = 2016, .y = -3680},
     {.x = 2048, .y = -3680},
     {.x = 2176, .y = -3776},
     {.x = 2176, .y = -3808},
     {.x = 2176, .y = -3840},
     {.x = 2048, .y = -3808},
     {.x = 2048, .y = -3776},
     {.x = 2048, .y = -3840},
     {.x = 2048, .y = -3872},
     {.x = 2048, .y = -3904},
     {.x = 2176, .y = -3872},
     {.x = 2176, .y = -3904},
     {.x = 2240, .y = -4096},
     {.x = 2112, .y = -4032},
     {.x = 2240, .y = -3968},
     {.x = 2368, .y = -3968},
     {.x = 2368, .y = -4096},
     {.x = 2880, .y = -3776},
     {.x = 2880, .y = -3904},
     {.x = 2848, .y = -3776},
     {.x = 2848, .y = -3904},
     {.x = 2816, .y = -3776},
     {.x = 2816, .y = -3904},
     {.x = 2784, .y = -3776},
     {.x = 2784, .y = -3904},
     {.x = 2752, .y = -3776},
     {.x = 2752, .y = -3904},
     {.x = 2688, .y = -3776},
     {.x = 2720, .y = -3904},
     {.x = 2632, .y = -3792},
     {.x = 2688, .y = -3920},
     {.x = 2912, .y = -3776},
     {.x = 2912, .y = -3904},
     {.x = 2944, .y = -3904},
     {.x = 2944, .y = -4000},
     {.x = 2240, .y = -3648},
     {.x = 1984, .y = -3648},
     {.x = 2672, .y = -3920},
     {.x = 2616, .y = -3792},
     {.x = 2176, .y = -3920},
     {.x = 2048, .y = -3920},
     {.x = 2752, .y = -2800},
     {.x = 2960, .y = -2656},
     {.x = 3400, .y = -3152},
     {.x = 3448, .y = -3520},
     {.x = 2240, .y = -2920},
     {.x = 2272, .y = -3008},
     {.x = 2432, .y = -3112},
     {.x = 2736, .y = -3112},
     {.x = 2752, .y = -3112},
     {.x = 3200, .y = -4128},
     {.x = 3328, .y = -4128},
     {.x = 2688, .y = -4128},
     {.x = 2816, .y = -4128},
     {.x = 2856, .y = -4160},
     {.x = 2912, .y = -4160},
     {.x = 2912, .y = -4128},
     {.x = 3104, .y = -4128},
     {.x = 3104, .y = -4160},
     {.x = 3160, .y = -4160},
     {.x = 3104, .y = -4352},
     {.x = 3104, .y = -4384},
     {.x = 3160, .y = -4384},
     {.x = 3160, .y = -4352},
     {.x = 2856, .y = -4352},
     {.x = 2856, .y = -4384},
     {.x = 2912, .y = -4384},
     {.x = 2912, .y = -4352},
     {.x = 2888, .y = -4160},
     {.x = 3128, .y = -4352},
     {.x = 3128, .y = -4160},
     {.x = 2888, .y = -4352},
     {.x = 2888, .y = -4320},
     {.x = 2888, .y = -4192},
     {.x = 3128, .y = -4320},
     {.x = 3128, .y = -4192},
     {.x = 3328, .y = -4544},
     {.x = 3072, .y = -4544},
     {.x = 2944, .y = -4544},
     {.x = 2688, .y = -4544},
     {.x = 3072, .y = -4608},
     {.x = 2944, .y = -4608},
     {.x = 3040, .y = -4608},
     {.x = 2976, .y = -4608},
     {.x = 2976, .y = -4632},
     {.x = 3040, .y = -4632},
     {.x = 3040, .y = -4648},
     {.x = 3040, .y = -4672},
     {.x = 2976, .y = -4672},
     {.x = 2976, .y = -4648},
     {.x = 3104, .y = -4672},
     {.x = 2912, .y = -4672},
     {.x = 3104, .y = -4864},
     {.x = 2912, .y = -4864},
     {.x = 2912, .y = -4800},
     {.x = 2912, .y = -4736},
     {.x = 2944, .y = -4016},
     {.x = 3072, .y = -4016},
     {.x = 2944, .y = -4032},
     {.x = 3072, .y = -4032},
     {.x = 3024, .y = -4592},
     {.x = 2992, .y = -4592},
     {.x = 2992, .y = -4600},
     {.x = 3024, .y = -4600},
     {.x = 3024, .y = -4840},
     {.x = 2992, .y = -4840},
     {.x = 2992, .y = -4848},
     {.x = 3024, .y = -4848},
     {.x = -240, .y = -3264},
     {.x = -208, .y = -3264},
     {.x = -192, .y = -3248},
     {.x = -192, .y = -3216},
     {.x = -208, .y = -3200},
     {.x = -240, .y = -3200},
     {.x = -256, .y = -3216},
     {.x = -256, .y = -3248},
     {.x = 1664, .y = -2368},
     {.x = 1600, .y = -2368},
     {.x = 1600, .y = -2112},
     {.x = 1600, .y = -2624},
     {.x = 1664, .y = -2624},
     {.x = 2560, .y = -2112},
     {.x = 2560, .y = -2496},
     {.x = 2496, .y = -2496},
     {.x = 2176, .y = -2752},
     {.x = 2176, .y = -2816},
     {.x = 1600, .y = -2048},
     {.x = 1664, .y = -2048},
     {.x = 2496, .y = -2048},
     {.x = 2560, .y = -2048},
     {.x = 1664, .y = -2112},
     {.x = 2496, .y = -2112},
     {.x = 1664, .y = -2816},
     {.x = 1600, .y = -2816},
     {.x = 1600, .y = -2752},
     {.x = 1664, .y = -2752},
     {.x = 2496, .y = -2688},
     {.x = 2496, .y = -2752},
     {.x = 2520, .y = -2688},
     {.x = 2496, .y = -2560},
     {.x = 1984, .y = -2304},
     {.x = 1984, .y = -2240},
     {.x = 1792, .y = -2240},
     {.x = 1792, .y = -2304},
     {.x = 1664, .y = -2600},
     {.x = 1664, .y = -2392},
     {.x = 2496, .y = -2520},
     {.x = 2200, .y = -2752},
     {.x = 2112, .y = -2592},
     {.x = 2336, .y = -2592},
     {.x = 2336, .y = -2272},
     {.x = 2112, .y = -2272},
     {.x = 2112, .y = -2304},
     {.x = 2144, .y = -2304},
     {.x = 2176, .y = -2304},
     {.x = 2208, .y = -2304},
     {.x = 2304, .y = -2304},
     {.x = 2304, .y = -2560},
     {.x = 2208, .y = -2560},
     {.x = 2176, .y = -2560},
     {.x = 2144, .y = -2560},
     {.x = 2112, .y = -2560},
     {.x = 1984, .y = -2624},
     {.x = 1984, .y = -2560},
     {.x = 1792, .y = -2560},
     {.x = 1792, .y = -2624},
     {.x = 1992, .y = -2552},
     {.x = 1784, .y = -2552},
     {.x = 1784, .y = -2632},
     {.x = 1992, .y = -2632},
     {.x = 1784, .y = -2312},
     {.x = 1992, .y = -2312},
     {.x = 1992, .y = -2232},
     {.x = 1784, .y = -2232},
     {.x = 3520, .y = -3904},
     {.x = 3328, .y = -3968},
     {.x = 3200, .y = -3968},
     {.x = 3200, .y = -3744},
     {.x = 3328, .y = -3744},
     {.x = 3360, .y = -3648},
     {.x = 3472, .y = -3520},
     {.x = 3520, .y = -3584},
     {.x = 3352, .y = -3592},
     {.x = 3304, .y = -3968},
     {.x = 3304, .y = -3744},
     {.x = 2344, .y = -4096},
     {.x = 2264, .y = -4096},
     {.x = 2264, .y = -3968},
     {.x = 2344, .y = -3968},
     {.x = 2176, .y = -3704},
     {.x = 2048, .y = -3704},
     {.x = 3520, .y = -3840},
     {.x = 3680, .y = -3904},
     {.x = 3584, .y = -3904},
     {.x = 3744, .y = -3808},
     {.x = 3584, .y = -3840},
     {.x = 3616, .y = -3776},
     {.x = 3552, .y = -3552},
     {.x = 3552, .y = -3392},
     {.x = 3648, .y = -3264},
     {.x = 3680, .y = -3552},
     {.x = 3680, .y = -3392},
     {.x = 3808, .y = -3264},
     {.x = 3496, .y = -3032},
     {.x = 3584, .y = -2880},
     {.x = 3456, .y = -3032},
     {.x = 3360, .y = -2880},
     {.x = 3320, .y = -3040},
     {.x = 3416, .y = -3152},
     {.x = 3536, .y = -3840},
     {.x = 3536, .y = -3904},
     {.x = 1344, .y = -3392},
     {.x = 1344, .y = -3520},
     {.x = 1376, .y = -3392},
     {.x = 1376, .y = -3520},
     {.x = 1664, .y = -2880},
     {.x = 2176, .y = -2920},
     {.x = 2176, .y = -2880},
     {.x = 2176, .y = -4064},
     {.x = 2176, .y = -3648},
     {.x = 1984, .y = -3680},
     {.x = 1664, .y = -2312},
     {.x = 1664, .y = -2552},
     {.x = 1664, .y = -2232},
     {.x = 1992, .y = -2112},
     {.x = 1984, .y = -2312},
     {.x = 1984, .y = -2232},
     {.x = 1784, .y = -2240},
     {.x = 1792, .y = -2312},
     {.x = 1784, .y = -2752},
     {.x = 1992, .y = -2752},
     {.x = 1984, .y = -2552},
     {.x = 1984, .y = -2632},
     {.x = 1784, .y = -2560},
     {.x = 1792, .y = -2632},
     {.x = 1992, .y = -2560},
     {.x = 1992, .y = -2304},
     {.x = 2176, .y = -2592},
     {.x = 1992, .y = -2592},
     {.x = 2176, .y = -2112},
     {.x = 2176, .y = -2272},
     {.x = 1992, .y = -2272},
     {.x = 2176, .y = -2048},
     {.x = 1344, .y = -2486},
     {.x = 472, .y = -3392},
     {.x = 256, .y = -3456},
     {.x = 320, .y = -3008},
     {.x = 256, .y = -3008},
     {.x = 160, .y = -3456},
     {.x = 160, .y = -3008},
     {.x = -768, .y = -3496},
     {.x = -256, .y = -2874},
     {.x = -128, .y = -2851},
     {.x = -640, .y = -3296},
     {.x = -640, .y = -3168},
     {.x = -640, .y = -3120},
     {.x = -128, .y = -3613},
     {.x = -256, .y = -3589},
     {.x = -336, .y = -3575},
     {.x = 2960, .y = -2930},
     {.x = 2944, .y = -3669},
     {.x = 3242, .y = -3370},
     {.x = 3353, .y = -3601},
     {.x = 3454, .y = -3496},
     {.x = 3512, .y = -3574},
     {.x = 3047, .y = -3626},
     {.x = 2752, .y = -3296},
     {.x = 3008, .y = -2902},
     {.x = 3215, .y = -3404},
     {.x = 2887, .y = -3160},
     {.x = 3345, .y = -2939},
     {.x = 3556, .y = -3567},
     {.x = 3680, .y = -3546},
     {.x = 3738, .y = -3144},
     {.x = 3672, .y = -3032},
     {.x = 2336, .y = -2112},
     {.x = 2336, .y = -2560},
     {.x = 2336, .y = -2752},
     {.x = 2496, .y = -2665},
     {.x = 2197, .y = -3648},
     {.x = 2688, .y = -4352},
     {.x = 2688, .y = -4160},
     {.x = 3104, .y = -4544},
     {.x = 2688, .y = -4384},
     {.x = 3160, .y = -4544},
     {.x = 3328, .y = -4160},
     {.x = 3072, .y = -4600},
     {.x = 2944, .y = -4600},
     {.x = 3024, .y = -4544},
     {.x = 2944, .y = -4592},
     {.x = 3104, .y = -4840},
     {.x = 2912, .y = -4840},
     {.x = 3024, .y = -4864},
     {.x = 2912, .y = -4848},
     {.x = 2776, .y = -4128},
     {.x = 2435, .y = -3920},
 };

static const seg segs[747] = {
    {
                .begin_vert = 132, .end_vert = 133,
                .angle = 4156, .linedef = 160,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 133, .end_vert = 134,
                .angle = 0, .linedef = 161,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 134, .end_vert = 135,
                .angle = -2826, .linedef = 162,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 135, .end_vert = 136,
                .angle = -10101, .linedef = 163,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 136, .end_vert = 137,
                .angle = -20971, .linedef = 164,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 137, .end_vert = 138,
                .angle = -31287, .linedef = 165,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 138, .end_vert = 139,
                .angle = 30049, .linedef = 166,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 139, .end_vert = 132,
                .angle = 17538, .linedef = 167,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 139, .end_vert = 138,
                .angle = -2718, .linedef = 166,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 138, .end_vert = 137,
                .angle = 1480, .linedef = 165,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 137, .end_vert = 136,
                .angle = 11796, .linedef = 164,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 136, .end_vert = 135,
                .angle = 22666, .linedef = 163,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 135, .end_vert = 134,
                .angle = 29941, .linedef = 162,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 133, .end_vert = 132,
                .angle = -28611, .linedef = 160,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 31, .end_vert = 32,
                .angle = 16384, .linedef = 29,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 33, .end_vert = 34,
                .angle = 16384, .linedef = 30,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 34, .end_vert = 31,
                .angle = 16384, .linedef = 35,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 132, .end_vert = 139,
                .angle = -15229, .linedef = 167,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 387, .end_vert = 33,
                .angle = 16384, .linedef = 475,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 388, .end_vert = 387,
                .angle = 16384, .linedef = 476,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 32, .end_vert = 35,
                .angle = 16384, .linedef = 36,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 35, .end_vert = 40,
                .angle = 6133, .linedef = 43,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 130, .end_vert = 131,
                .angle = -5826, .linedef = 159,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 134, .end_vert = 133,
                .angle = -32768, .linedef = 161,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 40, .end_vert = 389,
                .angle = 0, .linedef = 478,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 389, .end_vert = 130,
                .angle = 0, .linedef = 479,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 131, .end_vert = 390,
                .angle = 0, .linedef = 480,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 131, .end_vert = 130,
                .angle = 26941, .linedef = 159,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 390, .end_vert = 131,
                .angle = -32768, .linedef = 480,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 391, .end_vert = 390,
                .angle = -16384, .linedef = 482,
                .direction = 0, .offset = 64
                },
     {
                .begin_vert = 27, .end_vert = 26,
                .angle = 16384, .linedef = 25,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 34, .end_vert = 33,
                .angle = -16384, .linedef = 30,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 26, .end_vert = 34,
                .angle = 0, .linedef = 31,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 33, .end_vert = 27,
                .angle = -32768, .linedef = 32,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 386, .end_vert = 385,
                .angle = 16384, .linedef = 471,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 385, .end_vert = 387,
                .angle = 0, .linedef = 472,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 388, .end_vert = 386,
                .angle = -32768, .linedef = 473,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 387, .end_vert = 388,
                .angle = -16384, .linedef = 476,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 28, .end_vert = 30,
                .angle = 16384, .linedef = 28,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 32, .end_vert = 31,
                .angle = -16384, .linedef = 29,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 31, .end_vert = 28,
                .angle = -32768, .linedef = 33,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 30, .end_vert = 32,
                .angle = 0, .linedef = 34,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 392, .end_vert = 187,
                .angle = 27931, .linedef = 211,
                .direction = 0, .offset = 71
                },
     {
                .begin_vert = 183, .end_vert = 185,
                .angle = 0, .linedef = 217,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 185, .end_vert = 213,
                .angle = -16384, .linedef = 255,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 187, .end_vert = 214,
                .angle = 21798, .linedef = 257,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 214, .end_vert = 183,
                .angle = 16384, .linedef = 258,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 183, .end_vert = 182,
                .angle = 16384, .linedef = 208,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 184, .end_vert = 185,
                .angle = -16384, .linedef = 210,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 182, .end_vert = 184,
                .angle = 0, .linedef = 216,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 185, .end_vert = 183,
                .angle = -32768, .linedef = 217,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 182, .end_vert = 181,
                .angle = 16384, .linedef = 207,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 178, .end_vert = 184,
                .angle = -16384, .linedef = 209,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 181, .end_vert = 178,
                .angle = 0, .linedef = 215,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 184, .end_vert = 182,
                .angle = -32768, .linedef = 216,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 177, .end_vert = 178,
                .angle = -16384, .linedef = 204,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 181, .end_vert = 179,
                .angle = 16384, .linedef = 206,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 179, .end_vert = 177,
                .angle = 0, .linedef = 214,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 178, .end_vert = 181,
                .angle = -32768, .linedef = 215,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 176, .end_vert = 177,
                .angle = -16384, .linedef = 203,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 179, .end_vert = 180,
                .angle = 16384, .linedef = 205,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 180, .end_vert = 176,
                .angle = 0, .linedef = 213,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 177, .end_vert = 179,
                .angle = -32768, .linedef = 214,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 210, .end_vert = 42,
                .angle = -32768, .linedef = 248,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 42, .end_vert = 388,
                .angle = 16384, .linedef = 474,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 41, .end_vert = 42,
                .angle = 30603, .linedef = 44,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 42, .end_vert = 210,
                .angle = 0, .linedef = 248,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 210, .end_vert = 129,
                .angle = -16384, .linedef = 249,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 43, .end_vert = 41,
                .angle = 28405, .linedef = 45,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 129, .end_vert = 43,
                .angle = 31701, .linedef = 158,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 393, .end_vert = 210,
                .angle = -32768, .linedef = 247,
                .direction = 0, .offset = 64
                },
     {
                .begin_vert = 175, .end_vert = 174,
                .angle = -32768, .linedef = 202,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 173, .end_vert = 175,
                .angle = -32768, .linedef = 212,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 210, .end_vert = 393,
                .angle = 0, .linedef = 247,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 394, .end_vert = 210,
                .angle = 16384, .linedef = 249,
                .direction = 1, .offset = 96
                },
     {
                .begin_vert = 174, .end_vert = 129,
                .angle = -19739, .linedef = 201,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 129, .end_vert = 394,
                .angle = 16384, .linedef = 249,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 175, .end_vert = 173,
                .angle = 0, .linedef = 212,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 176, .end_vert = 180,
                .angle = -32768, .linedef = 213,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 173, .end_vert = 363,
                .angle = -16384, .linedef = 440,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 363, .end_vert = 176,
                .angle = -16384, .linedef = 441,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 180, .end_vert = 364,
                .angle = 16384, .linedef = 442,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 364, .end_vert = 175,
                .angle = 16384, .linedef = 443,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 298, .end_vert = 299,
                .angle = -16384, .linedef = 361,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 309, .end_vert = 298,
                .angle = 0, .linedef = 373,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 299, .end_vert = 306,
                .angle = -32768, .linedef = 477,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 306, .end_vert = 309,
                .angle = 16384, .linedef = 484,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 306, .end_vert = 299,
                .angle = 0, .linedef = 477,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 130, .end_vert = 389,
                .angle = -32768, .linedef = 479,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 299, .end_vert = 391,
                .angle = -16384, .linedef = 482,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 389, .end_vert = 306,
                .angle = 16384, .linedef = 483,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 290, .end_vert = 395,
                .angle = 16384, .linedef = 367,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 396, .end_vert = 125,
                .angle = 16384, .linedef = 378,
                .direction = 0, .offset = 8
                },
     {
                .begin_vert = 125, .end_vert = 319,
                .angle = 16384, .linedef = 386,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 319, .end_vert = 290,
                .angle = 16384, .linedef = 387,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 340, .end_vert = 341,
                .angle = -32768, .linedef = 413,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 344, .end_vert = 345,
                .angle = 0, .linedef = 417,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 397, .end_vert = 304,
                .angle = 16384, .linedef = 367,
                .direction = 0, .offset = 136
                },
     {
                .begin_vert = 304, .end_vert = 398,
                .angle = 0, .linedef = 368,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 346, .end_vert = 347,
                .angle = -32768, .linedef = 419,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 395, .end_vert = 397,
                .angle = 16384, .linedef = 367,
                .direction = 0, .offset = 56
                },
     {
                .begin_vert = 347, .end_vert = 344,
                .angle = -16384, .linedef = 420,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 314, .end_vert = 315,
                .angle = 16384, .linedef = 380,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 345, .end_vert = 399,
                .angle = -32768, .linedef = 417,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 346, .end_vert = 345,
                .angle = -16384, .linedef = 418,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 400, .end_vert = 346,
                .angle = 0, .linedef = 419,
                .direction = 1, .offset = 200
                },
     {
                .begin_vert = 315, .end_vert = 316,
                .angle = -32768, .linedef = 381,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 347, .end_vert = 400,
                .angle = 0, .linedef = 419,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 401, .end_vert = 347,
                .angle = 16384, .linedef = 420,
                .direction = 1, .offset = 72
                },
     {
                .begin_vert = 316, .end_vert = 317,
                .angle = -16384, .linedef = 382,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 402, .end_vert = 344,
                .angle = -32768, .linedef = 417,
                .direction = 1, .offset = 200
                },
     {
                .begin_vert = 344, .end_vert = 401,
                .angle = 16384, .linedef = 420,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 317, .end_vert = 314,
                .angle = 0, .linedef = 383,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 399, .end_vert = 402,
                .angle = -32768, .linedef = 417,
                .direction = 1, .offset = 8
                },
     {
                .begin_vert = 315, .end_vert = 314,
                .angle = -16384, .linedef = 380,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 316, .end_vert = 315,
                .angle = 0, .linedef = 381,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 317, .end_vert = 316,
                .angle = 16384, .linedef = 382,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 314, .end_vert = 317,
                .angle = -32768, .linedef = 383,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 403, .end_vert = 309,
                .angle = -32768, .linedef = 373,
                .direction = 0, .offset = 392
                },
     {
                .begin_vert = 124, .end_vert = 396,
                .angle = 16384, .linedef = 378,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 294, .end_vert = 318,
                .angle = 16384, .linedef = 384,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 318, .end_vert = 124,
                .angle = 16384, .linedef = 385,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 341, .end_vert = 342,
                .angle = -16384, .linedef = 414,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 309, .end_vert = 294,
                .angle = 16384, .linedef = 485,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 404, .end_vert = 403,
                .angle = -32768, .linedef = 373,
                .direction = 0, .offset = 184
                },
     {
                .begin_vert = 342, .end_vert = 343,
                .angle = 0, .linedef = 415,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 336, .end_vert = 337,
                .angle = 16384, .linedef = 409,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 405, .end_vert = 340,
                .angle = 0, .linedef = 413,
                .direction = 1, .offset = 200
                },
     {
                .begin_vert = 343, .end_vert = 406,
                .angle = -32768, .linedef = 415,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 340, .end_vert = 343,
                .angle = -16384, .linedef = 416,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 337, .end_vert = 338,
                .angle = -32768, .linedef = 410,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 341, .end_vert = 405,
                .angle = 0, .linedef = 413,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 407, .end_vert = 341,
                .angle = 16384, .linedef = 414,
                .direction = 1, .offset = 72
                },
     {
                .begin_vert = 338, .end_vert = 339,
                .angle = -16384, .linedef = 411,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 342, .end_vert = 407,
                .angle = 16384, .linedef = 414,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 408, .end_vert = 342,
                .angle = -32768, .linedef = 415,
                .direction = 1, .offset = 200
                },
     {
                .begin_vert = 339, .end_vert = 336,
                .angle = 0, .linedef = 412,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 406, .end_vert = 408,
                .angle = -32768, .linedef = 415,
                .direction = 1, .offset = 8
                },
     {
                .begin_vert = 337, .end_vert = 336,
                .angle = -16384, .linedef = 409,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 338, .end_vert = 337,
                .angle = 0, .linedef = 410,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 339, .end_vert = 338,
                .angle = 16384, .linedef = 411,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 336, .end_vert = 339,
                .angle = -32768, .linedef = 412,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 326, .end_vert = 327,
                .angle = 0, .linedef = 396,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 334, .end_vert = 335,
                .angle = -32768, .linedef = 404,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 327, .end_vert = 334,
                .angle = -16384, .linedef = 406,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 409, .end_vert = 340,
                .angle = 16384, .linedef = 416,
                .direction = 0, .offset = 72
                },
     {
                .begin_vert = 345, .end_vert = 410,
                .angle = 16384, .linedef = 418,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 327, .end_vert = 328,
                .angle = 0, .linedef = 397,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 333, .end_vert = 334,
                .angle = -32768, .linedef = 403,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 334, .end_vert = 327,
                .angle = 16384, .linedef = 406,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 328, .end_vert = 333,
                .angle = -16384, .linedef = 407,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 298, .end_vert = 404,
                .angle = -32768, .linedef = 373,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 322, .end_vert = 411,
                .angle = 0, .linedef = 392,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 343, .end_vert = 412,
                .angle = 16384, .linedef = 416,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 335, .end_vert = 322,
                .angle = -16384, .linedef = 405,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 412, .end_vert = 409,
                .angle = 16384, .linedef = 416,
                .direction = 0, .offset = 40
                },
     {
                .begin_vert = 398, .end_vert = 413,
                .angle = 0, .linedef = 368,
                .direction = 0, .offset = 328
                },
     {
                .begin_vert = 414, .end_vert = 325,
                .angle = -32768, .linedef = 394,
                .direction = 0, .offset = 160
                },
     {
                .begin_vert = 415, .end_vert = 346,
                .angle = 16384, .linedef = 418,
                .direction = 0, .offset = 40
                },
     {
                .begin_vert = 325, .end_vert = 326,
                .angle = -16384, .linedef = 395,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 410, .end_vert = 415,
                .angle = 16384, .linedef = 418,
                .direction = 0, .offset = 8
                },
     {
                .begin_vert = 301, .end_vert = 416,
                .angle = 0, .linedef = 363,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 413, .end_vert = 304,
                .angle = -32768, .linedef = 368,
                .direction = 1, .offset = 320
                },
     {
                .begin_vert = 122, .end_vert = 123,
                .angle = 16384, .linedef = 149,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 124, .end_vert = 122,
                .angle = -32768, .linedef = 154,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 123, .end_vert = 125,
                .angle = 0, .linedef = 155,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 125, .end_vert = 124,
                .angle = -16384, .linedef = 378,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 290, .end_vert = 291,
                .angle = -32768, .linedef = 356,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 291, .end_vert = 292,
                .angle = 16384, .linedef = 357,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 300, .end_vert = 301,
                .angle = 0, .linedef = 362,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 292, .end_vert = 300,
                .angle = 16384, .linedef = 366,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 304, .end_vert = 290,
                .angle = -16384, .linedef = 367,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 293, .end_vert = 294,
                .angle = 0, .linedef = 358,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 306, .end_vert = 307,
                .angle = -32768, .linedef = 370,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 307, .end_vert = 308,
                .angle = 16384, .linedef = 371,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 308, .end_vert = 293,
                .angle = 16384, .linedef = 372,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 309, .end_vert = 306,
                .angle = -16384, .linedef = 484,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 294, .end_vert = 309,
                .angle = -16384, .linedef = 485,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 118, .end_vert = 117,
                .angle = -29130, .linedef = 144,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 417, .end_vert = 119,
                .angle = 4223, .linedef = 145,
                .direction = 0, .offset = 104
                },
     {
                .begin_vert = 119, .end_vert = 118,
                .angle = -16384, .linedef = 147,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 117, .end_vert = 29,
                .angle = -17823, .linedef = 143,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 118, .end_vert = 119,
                .angle = 16384, .linedef = 147,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 120, .end_vert = 121,
                .angle = -16384, .linedef = 148,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 121, .end_vert = 118,
                .angle = -32768, .linedef = 150,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 119, .end_vert = 120,
                .angle = 0, .linedef = 151,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 121, .end_vert = 120,
                .angle = 16384, .linedef = 148,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 123, .end_vert = 122,
                .angle = -16384, .linedef = 149,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 120, .end_vert = 123,
                .angle = 0, .linedef = 152,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 122, .end_vert = 121,
                .angle = -32768, .linedef = 153,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 16, .end_vert = 17,
                .angle = -32768, .linedef = 15,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 24, .end_vert = 25,
                .angle = 0, .linedef = 23,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 26, .end_vert = 27,
                .angle = -16384, .linedef = 25,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 28, .end_vert = 26,
                .angle = -16384, .linedef = 26,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 30, .end_vert = 28,
                .angle = -16384, .linedef = 28,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 20, .end_vert = 24,
                .angle = 0, .linedef = 47,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 17, .end_vert = 12,
                .angle = -32768, .linedef = 48,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 12, .end_vert = 20,
                .angle = 16384, .linedef = 49,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 12, .end_vert = 13,
                .angle = -32768, .linedef = 11,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 19, .end_vert = 20,
                .angle = 0, .linedef = 18,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 20, .end_vert = 12,
                .angle = -16384, .linedef = 49,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 13, .end_vert = 19,
                .angle = 16384, .linedef = 50,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 19, .end_vert = 13,
                .angle = -16384, .linedef = 50,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 44, .end_vert = 45,
                .angle = 16384, .linedef = 54,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 20, .end_vert = 21,
                .angle = 16384, .linedef = 19,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 23, .end_vert = 24,
                .angle = -16384, .linedef = 22,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 24, .end_vert = 20,
                .angle = -32768, .linedef = 47,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 21, .end_vert = 23,
                .angle = 0, .linedef = 51,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 23, .end_vert = 21,
                .angle = -32768, .linedef = 51,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 22, .end_vert = 23,
                .angle = -32768, .linedef = 21,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 25, .end_vert = 22,
                .angle = 16384, .linedef = 24,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 22, .end_vert = 30,
                .angle = -2555, .linedef = 39,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 18, .end_vert = 19,
                .angle = -16384, .linedef = 17,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 45, .end_vert = 36,
                .angle = 16384, .linedef = 53,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 21, .end_vert = 18,
                .angle = -32768, .linedef = 20,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 29, .end_vert = 30,
                .angle = -16384, .linedef = 27,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 30, .end_vert = 22,
                .angle = 30212, .linedef = 39,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 37, .end_vert = 38,
                .angle = 4587, .linedef = 41,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 38, .end_vert = 39,
                .angle = 0, .linedef = 42,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 39, .end_vert = 29,
                .angle = 0, .linedef = 146,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 36, .end_vert = 37,
                .angle = 0, .linedef = 40,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 39, .end_vert = 116,
                .angle = 15438, .linedef = 142,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 116, .end_vert = 417,
                .angle = 4223, .linedef = 145,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 29, .end_vert = 39,
                .angle = -32768, .linedef = 146,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 11, .end_vert = 12,
                .angle = 16384, .linedef = 10,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 17, .end_vert = 14,
                .angle = -16384, .linedef = 16,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 14, .end_vert = 11,
                .angle = -32768, .linedef = 37,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 12, .end_vert = 17,
                .angle = 0, .linedef = 48,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 11, .end_vert = 14,
                .angle = 0, .linedef = 37,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 7, .end_vert = 9,
                .angle = -32768, .linedef = 8,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 13, .end_vert = 10,
                .angle = -16384, .linedef = 12,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 9, .end_vert = 44,
                .angle = 16384, .linedef = 52,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 10, .end_vert = 11,
                .angle = 0, .linedef = 9,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 8, .end_vert = 6,
                .angle = -32768, .linedef = 7,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 14, .end_vert = 15,
                .angle = 0, .linedef = 13,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 386, .end_vert = 8,
                .angle = -16384, .linedef = 470,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 385, .end_vert = 386,
                .angle = -16384, .linedef = 471,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 15, .end_vert = 16,
                .angle = 16384, .linedef = 14,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 27, .end_vert = 15,
                .angle = -30212, .linedef = 38,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 15, .end_vert = 27,
                .angle = 2555, .linedef = 38,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 27, .end_vert = 385,
                .angle = -16384, .linedef = 469,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 4, .end_vert = 3,
                .angle = -32768, .linedef = 3,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 2, .end_vert = 5,
                .angle = -32768, .linedef = 4,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 6, .end_vert = 4,
                .angle = -26056, .linedef = 5,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 5, .end_vert = 7,
                .angle = 26056, .linedef = 6,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 3, .end_vert = 2,
                .angle = -32768, .linedef = 46,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 0, .end_vert = 1,
                .angle = -32768, .linedef = 0,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 1, .end_vert = 2,
                .angle = 16384, .linedef = 1,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 3, .end_vert = 0,
                .angle = -16384, .linedef = 2,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 2, .end_vert = 3,
                .angle = 0, .linedef = 46,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 45, .end_vert = 44,
                .angle = -16384, .linedef = 54,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 46, .end_vert = 47,
                .angle = 16384, .linedef = 55,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 48, .end_vert = 49,
                .angle = 16384, .linedef = 56,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 49, .end_vert = 50,
                .angle = 1963, .linedef = 57,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 50, .end_vert = 45,
                .angle = 0, .linedef = 58,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 44, .end_vert = 51,
                .angle = -32768, .linedef = 59,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 51, .end_vert = 46,
                .angle = 30804, .linedef = 60,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 53, .end_vert = 52,
                .angle = 16384, .linedef = 61,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 47, .end_vert = 53,
                .angle = -32768, .linedef = 62,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 52, .end_vert = 48,
                .angle = 0, .linedef = 63,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 52, .end_vert = 53,
                .angle = -16384, .linedef = 61,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 53, .end_vert = 54,
                .angle = -16384, .linedef = 64,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 59, .end_vert = 52,
                .angle = -16384, .linedef = 68,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 66, .end_vert = 67,
                .angle = 16384, .linedef = 76,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 64, .end_vert = 65,
                .angle = 0, .linedef = 74,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 65, .end_vert = 66,
                .angle = 0, .linedef = 75,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 114, .end_vert = 115,
                .angle = -32768, .linedef = 140,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 67, .end_vert = 68,
                .angle = -32768, .linedef = 77,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 68, .end_vert = 69,
                .angle = -32768, .linedef = 78,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 108, .end_vert = 109,
                .angle = 0, .linedef = 134,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 66, .end_vert = 65,
                .angle = -32768, .linedef = 75,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 67, .end_vert = 66,
                .angle = -16384, .linedef = 76,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 68, .end_vert = 67,
                .angle = 0, .linedef = 77,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 65, .end_vert = 68,
                .angle = 16384, .linedef = 87,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 65, .end_vert = 64,
                .angle = -32768, .linedef = 74,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 69, .end_vert = 68,
                .angle = 0, .linedef = 78,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 64, .end_vert = 69,
                .angle = 16384, .linedef = 86,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 68, .end_vert = 65,
                .angle = -16384, .linedef = 87,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 418, .end_vert = 55,
                .angle = -20126, .linedef = 65,
                .direction = 0, .offset = 68
                },
     {
                .begin_vert = 55, .end_vert = 419,
                .angle = -32768, .linedef = 66,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 112, .end_vert = 113,
                .angle = 0, .linedef = 138,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 54, .end_vert = 418,
                .angle = -20126, .linedef = 65,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 113, .end_vert = 114,
                .angle = 16384, .linedef = 139,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 113, .end_vert = 112,
                .angle = -32768, .linedef = 138,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 114, .end_vert = 113,
                .angle = -16384, .linedef = 139,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 115, .end_vert = 114,
                .angle = 0, .linedef = 140,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 112, .end_vert = 115,
                .angle = 16384, .linedef = 141,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 420, .end_vert = 58,
                .angle = 0, .linedef = 67,
                .direction = 0, .offset = 192
                },
     {
                .begin_vert = 58, .end_vert = 59,
                .angle = -12641, .linedef = 69,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 109, .end_vert = 110,
                .angle = 16384, .linedef = 135,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 421, .end_vert = 420,
                .angle = 0, .linedef = 67,
                .direction = 0, .offset = 128
                },
     {
                .begin_vert = 110, .end_vert = 111,
                .angle = -32768, .linedef = 136,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 109, .end_vert = 108,
                .angle = -32768, .linedef = 134,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 110, .end_vert = 109,
                .angle = -16384, .linedef = 135,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 111, .end_vert = 110,
                .angle = 0, .linedef = 136,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 108, .end_vert = 111,
                .angle = 16384, .linedef = 137,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 419, .end_vert = 422,
                .angle = -32768, .linedef = 66,
                .direction = 0, .offset = 192
                },
     {
                .begin_vert = 61, .end_vert = 62,
                .angle = 0, .linedef = 71,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 62, .end_vert = 63,
                .angle = 0, .linedef = 72,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 63, .end_vert = 64,
                .angle = 0, .linedef = 73,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 115, .end_vert = 112,
                .angle = -16384, .linedef = 141,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 423, .end_vert = 421,
                .angle = 0, .linedef = 67,
                .direction = 0, .offset = 32
                },
     {
                .begin_vert = 69, .end_vert = 70,
                .angle = -32768, .linedef = 79,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 70, .end_vert = 71,
                .angle = -32768, .linedef = 80,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 71, .end_vert = 72,
                .angle = -32768, .linedef = 81,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 111, .end_vert = 108,
                .angle = -16384, .linedef = 137,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 64, .end_vert = 63,
                .angle = -32768, .linedef = 73,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 70, .end_vert = 69,
                .angle = 0, .linedef = 79,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 63, .end_vert = 70,
                .angle = 16384, .linedef = 85,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 69, .end_vert = 64,
                .angle = -16384, .linedef = 86,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 63, .end_vert = 62,
                .angle = -32768, .linedef = 72,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 71, .end_vert = 70,
                .angle = 0, .linedef = 80,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 62, .end_vert = 71,
                .angle = 16384, .linedef = 84,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 70, .end_vert = 63,
                .angle = -16384, .linedef = 85,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 62, .end_vert = 61,
                .angle = -32768, .linedef = 71,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 72, .end_vert = 71,
                .angle = 0, .linedef = 81,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 61, .end_vert = 72,
                .angle = 16384, .linedef = 83,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 71, .end_vert = 62,
                .angle = -16384, .linedef = 84,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 99, .end_vert = 100,
                .angle = -32768, .linedef = 122,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 100, .end_vert = 101,
                .angle = 24576, .linedef = 123,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 101, .end_vert = 424,
                .angle = 16384, .linedef = 124,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 104, .end_vert = 99,
                .angle = -1875, .linedef = 127,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 103, .end_vert = 98,
                .angle = 0, .linedef = 126,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 98, .end_vert = 105,
                .angle = -30892, .linedef = 129,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 83, .end_vert = 85,
                .angle = 0, .linedef = 116,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 285, .end_vert = 286,
                .angle = 24576, .linedef = 351,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 84, .end_vert = 86,
                .angle = -32768, .linedef = 117,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 425, .end_vert = 426,
                .angle = 1874, .linedef = 129,
                .direction = 0, .offset = 390
                },
     {
                .begin_vert = 84, .end_vert = 85,
                .angle = -16384, .linedef = 99,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 83, .end_vert = 86,
                .angle = 16384, .linedef = 100,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 85, .end_vert = 83,
                .angle = -32768, .linedef = 116,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 86, .end_vert = 84,
                .angle = 0, .linedef = 117,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 282, .end_vert = 283,
                .angle = 0, .linedef = 348,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 283, .end_vert = 284,
                .angle = 8192, .linedef = 349,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 284, .end_vert = 285,
                .angle = 16384, .linedef = 350,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 286, .end_vert = 287,
                .angle = -32768, .linedef = 352,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 287, .end_vert = 288,
                .angle = -24576, .linedef = 353,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 289, .end_vert = 282,
                .angle = -8192, .linedef = 355,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 283, .end_vert = 282,
                .angle = -32768, .linedef = 348,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 284, .end_vert = 283,
                .angle = -24576, .linedef = 349,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 285, .end_vert = 284,
                .angle = -16384, .linedef = 350,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 286, .end_vert = 285,
                .angle = -8192, .linedef = 351,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 287, .end_vert = 286,
                .angle = 0, .linedef = 352,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 288, .end_vert = 287,
                .angle = 8192, .linedef = 353,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 289, .end_vert = 288,
                .angle = 16384, .linedef = 354,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 282, .end_vert = 289,
                .angle = 24576, .linedef = 355,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 81, .end_vert = 82,
                .angle = 16384, .linedef = 118,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 288, .end_vert = 289,
                .angle = -16384, .linedef = 354,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 91, .end_vert = 90,
                .angle = -16384, .linedef = 119,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 427, .end_vert = 428,
                .angle = 16384, .linedef = 128,
                .direction = 0, .offset = 224
                },
     {
                .begin_vert = 81, .end_vert = 90,
                .angle = -32768, .linedef = 103,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 91, .end_vert = 82,
                .angle = 0, .linedef = 104,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 82, .end_vert = 81,
                .angle = -16384, .linedef = 118,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 90, .end_vert = 91,
                .angle = 16384, .linedef = 119,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 82, .end_vert = 83,
                .angle = 4836, .linedef = 98,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 86, .end_vert = 92,
                .angle = -32768, .linedef = 105,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 429, .end_vert = 105,
                .angle = 16384, .linedef = 128,
                .direction = 0, .offset = 400
                },
     {
                .begin_vert = 105, .end_vert = 425,
                .angle = 1874, .linedef = 129,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 92, .end_vert = 91,
                .angle = -16384, .linedef = 106,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 428, .end_vert = 429,
                .angle = 16384, .linedef = 128,
                .direction = 0, .offset = 352
                },
     {
                .begin_vert = 87, .end_vert = 89,
                .angle = 0, .linedef = 115,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 430, .end_vert = 431,
                .angle = 30893, .linedef = 127,
                .direction = 0, .offset = 195
                },
     {
                .begin_vert = 88, .end_vert = 80,
                .angle = -32768, .linedef = 114,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 87, .end_vert = 80,
                .angle = 16384, .linedef = 101,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 88, .end_vert = 89,
                .angle = -16384, .linedef = 102,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 80, .end_vert = 88,
                .angle = 0, .linedef = 114,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 89, .end_vert = 87,
                .angle = -32768, .linedef = 115,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 80, .end_vert = 81,
                .angle = 27931, .linedef = 97,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 90, .end_vert = 93,
                .angle = -16384, .linedef = 107,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 432, .end_vert = 104,
                .angle = 30893, .linedef = 127,
                .direction = 0, .offset = 406
                },
     {
                .begin_vert = 104, .end_vert = 427,
                .angle = 16384, .linedef = 128,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 93, .end_vert = 87,
                .angle = 0, .linedef = 108,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 431, .end_vert = 432,
                .angle = 30893, .linedef = 127,
                .direction = 0, .offset = 325
                },
     {
                .begin_vert = 78, .end_vert = 77,
                .angle = -16384, .linedef = 113,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 75, .end_vert = 76,
                .angle = -32768, .linedef = 93,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 76, .end_vert = 77,
                .angle = 27353, .linedef = 94,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 78, .end_vert = 79,
                .angle = 5414, .linedef = 95,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 79, .end_vert = 74,
                .angle = 0, .linedef = 96,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 77, .end_vert = 78,
                .angle = 16384, .linedef = 113,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 96, .end_vert = 84,
                .angle = -32768, .linedef = 111,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 97, .end_vert = 96,
                .angle = -24079, .linedef = 112,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 98, .end_vert = 97,
                .angle = -16384, .linedef = 120,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 426, .end_vert = 98,
                .angle = 1880, .linedef = 129,
                .direction = 0, .offset = 520
                },
     {
                .begin_vert = 85, .end_vert = 106,
                .angle = 0, .linedef = 130,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 106, .end_vert = 78,
                .angle = 0, .linedef = 131,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 89, .end_vert = 94,
                .angle = 0, .linedef = 109,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 94, .end_vert = 95,
                .angle = -8688, .linedef = 110,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 95, .end_vert = 99,
                .angle = -16384, .linedef = 121,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 99, .end_vert = 430,
                .angle = 30887, .linedef = 127,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 77, .end_vert = 107,
                .angle = -32768, .linedef = 132,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 107, .end_vert = 88,
                .angle = -32768, .linedef = 133,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 424, .end_vert = 102,
                .angle = 16384, .linedef = 124,
                .direction = 0, .offset = 23
                },
     {
                .begin_vert = 102, .end_vert = 103,
                .angle = 8192, .linedef = 125,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 105, .end_vert = 104,
                .angle = -16384, .linedef = 128,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 422, .end_vert = 56,
                .angle = -32768, .linedef = 66,
                .direction = 0, .offset = 288
                },
     {
                .begin_vert = 60, .end_vert = 61,
                .angle = 0, .linedef = 70,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 57, .end_vert = 423,
                .angle = 0, .linedef = 67,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 72, .end_vert = 73,
                .angle = -32768, .linedef = 82,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 61, .end_vert = 60,
                .angle = -32768, .linedef = 70,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 73, .end_vert = 72,
                .angle = 0, .linedef = 82,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 72, .end_vert = 61,
                .angle = -16384, .linedef = 83,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 60, .end_vert = 73,
                .angle = 16384, .linedef = 88,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 73, .end_vert = 74,
                .angle = 21220, .linedef = 89,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 74, .end_vert = 57,
                .angle = 8192, .linedef = 90,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 56, .end_vert = 75,
                .angle = 24576, .linedef = 91,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 75, .end_vert = 60,
                .angle = 11547, .linedef = 92,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 73, .end_vert = 60,
                .angle = -16384, .linedef = 88,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 74, .end_vert = 73,
                .angle = -11547, .linedef = 89,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 60, .end_vert = 75,
                .angle = -21220, .linedef = 92,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 140, .end_vert = 144,
                .angle = 6133, .linedef = 188,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 147, .end_vert = 433,
                .angle = -27378, .linedef = 191,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 146, .end_vert = 215,
                .angle = 26941, .linedef = 260,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 215, .end_vert = 140,
                .angle = 16384, .linedef = 261,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 144, .end_vert = 216,
                .angle = 0, .linedef = 262,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 216, .end_vert = 147,
                .angle = -12479, .linedef = 263,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 149, .end_vert = 150,
                .angle = 16384, .linedef = 173,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 433, .end_vert = 149,
                .angle = -27385, .linedef = 191,
                .direction = 1, .offset = 101
                },
     {
                .begin_vert = 150, .end_vert = 145,
                .angle = 0, .linedef = 347,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 145, .end_vert = 146,
                .angle = 16384, .linedef = 171,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 143, .end_vert = 144,
                .angle = -4836, .linedef = 170,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 144, .end_vert = 140,
                .angle = -26634, .linedef = 188,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 151, .end_vert = 152,
                .angle = 16384, .linedef = 174,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 152, .end_vert = 157,
                .angle = -7738, .linedef = 189,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 157, .end_vert = 151,
                .angle = -30212, .linedef = 192,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 164, .end_vert = 151,
                .angle = 29412, .linedef = 185,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 151, .end_vert = 157,
                .angle = 2555, .linedef = 192,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 434, .end_vert = 164,
                .angle = 16384, .linedef = 187,
                .direction = 0, .offset = 106
                },
     {
                .begin_vert = 157, .end_vert = 163,
                .angle = -7497, .linedef = 190,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 167, .end_vert = 435,
                .angle = 9617, .linedef = 193,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 162, .end_vert = 167,
                .angle = 28799, .linedef = 194,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 168, .end_vert = 162,
                .angle = -27559, .linedef = 195,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 171, .end_vert = 168,
                .angle = -11094, .linedef = 198,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 356, .end_vert = 168,
                .angle = 16384, .linedef = 429,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 436, .end_vert = 356,
                .angle = 17863, .linedef = 428,
                .direction = 0, .offset = 47
                },
     {
                .begin_vert = 435, .end_vert = 159,
                .angle = 9605, .linedef = 193,
                .direction = 0, .offset = 228
                },
     {
                .begin_vert = 170, .end_vert = 171,
                .angle = -26634, .linedef = 197,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 171, .end_vert = 170,
                .angle = 6133, .linedef = 197,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 168, .end_vert = 171,
                .angle = 21673, .linedef = 198,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 437, .end_vert = 218,
                .angle = -19161, .linedef = 265,
                .direction = 0, .offset = 66
                },
     {
                .begin_vert = 218, .end_vert = 168,
                .angle = -27931, .linedef = 266,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 168, .end_vert = 218,
                .angle = 4836, .linedef = 266,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 218, .end_vert = 354,
                .angle = 0, .linedef = 426,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 354, .end_vert = 438,
                .angle = -9672, .linedef = 427,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 162, .end_vert = 163,
                .angle = -31045, .linedef = 184,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 163, .end_vert = 439,
                .angle = 25270, .linedef = 190,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 167, .end_vert = 162,
                .angle = -3968, .linedef = 194,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 161, .end_vert = 158,
                .angle = -24576, .linedef = 182,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 149, .end_vert = 161,
                .angle = -1113, .linedef = 259,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 440, .end_vert = 223,
                .angle = 16384, .linedef = 271,
                .direction = 0, .offset = 64
                },
     {
                .begin_vert = 223, .end_vert = 149,
                .angle = 16384, .linedef = 273,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 149, .end_vert = 441,
                .angle = 5384, .linedef = 191,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 161, .end_vert = 149,
                .angle = 31654, .linedef = 259,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 156, .end_vert = 157,
                .angle = -24333, .linedef = 179,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 158, .end_vert = 156,
                .angle = -4257, .linedef = 180,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 157, .end_vert = 152,
                .angle = 25029, .linedef = 189,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 152, .end_vert = 440,
                .angle = 16384, .linedef = 271,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 157, .end_vert = 156,
                .angle = 8434, .linedef = 179,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 439, .end_vert = 157,
                .angle = 25270, .linedef = 190,
                .direction = 1, .offset = 32
                },
     {
                .begin_vert = 442, .end_vert = 167,
                .angle = -23153, .linedef = 193,
                .direction = 1, .offset = 106
                },
     {
                .begin_vert = 159, .end_vert = 160,
                .angle = 28750, .linedef = 181,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 160, .end_vert = 155,
                .angle = 7300, .linedef = 183,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 443, .end_vert = 161,
                .angle = 8192, .linedef = 182,
                .direction = 1, .offset = 100
                },
     {
                .begin_vert = 155, .end_vert = 160,
                .angle = -25467, .linedef = 183,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 156, .end_vert = 158,
                .angle = 28510, .linedef = 180,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 160, .end_vert = 159,
                .angle = -4017, .linedef = 181,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 158, .end_vert = 443,
                .angle = 8192, .linedef = 182,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 159, .end_vert = 442,
                .angle = -23153, .linedef = 193,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 155, .end_vert = 154,
                .angle = 1963, .linedef = 178,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 169, .end_vert = 170,
                .angle = -32768, .linedef = 196,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 217, .end_vert = 169,
                .angle = -13758, .linedef = 264,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 154, .end_vert = 217,
                .angle = -8992, .linedef = 458,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 217, .end_vert = 154,
                .angle = 23775, .linedef = 458,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 170, .end_vert = 169,
                .angle = 0, .linedef = 196,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 169, .end_vert = 437,
                .angle = -19161, .linedef = 265,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 148, .end_vert = 154,
                .angle = -3742, .linedef = 177,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 154, .end_vert = 155,
                .angle = -30804, .linedef = 178,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 147, .end_vert = 148,
                .angle = -16384, .linedef = 172,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 441, .end_vert = 147,
                .angle = 5384, .linedef = 191,
                .direction = 0, .offset = 295
                },
     {
                .begin_vert = 154, .end_vert = 381,
                .angle = 0, .linedef = 461,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 381, .end_vert = 444,
                .angle = 13829, .linedef = 462,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 445, .end_vert = 371,
                .angle = 19286, .linedef = 448,
                .direction = 0, .offset = 217
                },
     {
                .begin_vert = 371, .end_vert = 372,
                .angle = 16384, .linedef = 449,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 372, .end_vert = 373,
                .angle = 9672, .linedef = 450,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 375, .end_vert = 446,
                .angle = -16384, .linedef = 452,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 376, .end_vert = 375,
                .angle = -24576, .linedef = 453,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 447, .end_vert = 376,
                .angle = -10875, .linedef = 455,
                .direction = 0, .offset = 305
                },
     {
                .begin_vert = 378, .end_vert = 448,
                .angle = -10875, .linedef = 455,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 377, .end_vert = 379,
                .angle = -32768, .linedef = 456,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 380, .end_vert = 378,
                .angle = 0, .linedef = 457,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 444, .end_vert = 380,
                .angle = 13827, .linedef = 462,
                .direction = 0, .offset = 104
                },
     {
                .begin_vert = 373, .end_vert = 377,
                .angle = 22433, .linedef = 454,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 448, .end_vert = 447,
                .angle = -10875, .linedef = 455,
                .direction = 0, .offset = 176
                },
     {
                .begin_vert = 379, .end_vert = 382,
                .angle = -19739, .linedef = 463,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 382, .end_vert = 217,
                .angle = -32768, .linedef = 464,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 449, .end_vert = 305,
                .angle = 0, .linedef = 368,
                .direction = 0, .offset = 672
                },
     {
                .begin_vert = 305, .end_vert = 297,
                .angle = -16384, .linedef = 369,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 297, .end_vert = 320,
                .angle = -16384, .linedef = 388,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 320, .end_vert = 313,
                .angle = -16384, .linedef = 389,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 450, .end_vert = 324,
                .angle = 16384, .linedef = 393,
                .direction = 0, .offset = 32
                },
     {
                .begin_vert = 413, .end_vert = 449,
                .angle = 0, .linedef = 368,
                .direction = 0, .offset = 512
                },
     {
                .begin_vert = 324, .end_vert = 414,
                .angle = -32768, .linedef = 394,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 328, .end_vert = 329,
                .angle = 0, .linedef = 398,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 332, .end_vert = 333,
                .angle = -32768, .linedef = 402,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 333, .end_vert = 328,
                .angle = 16384, .linedef = 407,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 329, .end_vert = 332,
                .angle = -16384, .linedef = 408,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 329, .end_vert = 330,
                .angle = 0, .linedef = 399,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 330, .end_vert = 331,
                .angle = -16384, .linedef = 400,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 331, .end_vert = 332,
                .angle = -32768, .linedef = 401,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 332, .end_vert = 329,
                .angle = 16384, .linedef = 408,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 310, .end_vert = 311,
                .angle = -16384, .linedef = 374,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 313, .end_vert = 310,
                .angle = -16384, .linedef = 379,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 311, .end_vert = 451,
                .angle = -32768, .linedef = 390,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 323, .end_vert = 450,
                .angle = 16384, .linedef = 393,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 451, .end_vert = 321,
                .angle = -32768, .linedef = 390,
                .direction = 0, .offset = 160
                },
     {
                .begin_vert = 321, .end_vert = 298,
                .angle = -32768, .linedef = 391,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 411, .end_vert = 323,
                .angle = 0, .linedef = 392,
                .direction = 0, .offset = 64
                },
     {
                .begin_vert = 416, .end_vert = 302,
                .angle = 0, .linedef = 363,
                .direction = 0, .offset = 512
                },
     {
                .begin_vert = 305, .end_vert = 413,
                .angle = -32768, .linedef = 368,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 140, .end_vert = 141,
                .angle = -32768, .linedef = 168,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 141, .end_vert = 143,
                .angle = 10969, .linedef = 421,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 142, .end_vert = 143,
                .angle = 0, .linedef = 169,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 141, .end_vert = 312,
                .angle = 24992, .linedef = 375,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 313, .end_vert = 142,
                .angle = 0, .linedef = 376,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 452, .end_vert = 313,
                .angle = 16384, .linedef = 379,
                .direction = 1, .offset = 22
                },
     {
                .begin_vert = 143, .end_vert = 141,
                .angle = -21798, .linedef = 421,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 312, .end_vert = 310,
                .angle = -32768, .linedef = 377,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 310, .end_vert = 452,
                .angle = 16384, .linedef = 379,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 295, .end_vert = 296,
                .angle = -16384, .linedef = 359,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 296, .end_vert = 297,
                .angle = -32768, .linedef = 360,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 302, .end_vert = 303,
                .angle = 0, .linedef = 364,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 303, .end_vert = 295,
                .angle = -16384, .linedef = 365,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 297, .end_vert = 305,
                .angle = 16384, .linedef = 369,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 219, .end_vert = 220,
                .angle = -12746, .linedef = 267,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 390, .end_vert = 219,
                .angle = 0, .linedef = 481,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 221, .end_vert = 222,
                .angle = 0, .linedef = 269,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 222, .end_vert = 153,
                .angle = -16384, .linedef = 272,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 220, .end_vert = 221,
                .angle = -6011, .linedef = 268,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 152, .end_vert = 153,
                .angle = -32768, .linedef = 176,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 222, .end_vert = 223,
                .angle = 0, .linedef = 270,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 223, .end_vert = 152,
                .angle = -16384, .linedef = 271,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 153, .end_vert = 222,
                .angle = 16384, .linedef = 272,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 153, .end_vert = 126,
                .angle = -16384, .linedef = 175,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 126, .end_vert = 209,
                .angle = -32768, .linedef = 246,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 126, .end_vert = 127,
                .angle = -28915, .linedef = 156,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 127, .end_vert = 128,
                .angle = -31429, .linedef = 157,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 209, .end_vert = 126,
                .angle = 0, .linedef = 246,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 128, .end_vert = 209,
                .angle = 16384, .linedef = 250,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 209, .end_vert = 393,
                .angle = -32768, .linedef = 247,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 128, .end_vert = 172,
                .angle = 19739, .linedef = 199,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 453, .end_vert = 209,
                .angle = 0, .linedef = 247,
                .direction = 1, .offset = 213
                },
     {
                .begin_vert = 209, .end_vert = 128,
                .angle = -16384, .linedef = 250,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 172, .end_vert = 173,
                .angle = -32768, .linedef = 200,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 393, .end_vert = 453,
                .angle = 0, .linedef = 247,
                .direction = 1, .offset = 192
                },
     {
                .begin_vert = 242, .end_vert = 229,
                .angle = 0, .linedef = 287,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 241, .end_vert = 245,
                .angle = -32768, .linedef = 292,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 245, .end_vert = 246,
                .angle = 16384, .linedef = 294,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 246, .end_vert = 247,
                .angle = 16384, .linedef = 295,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 247, .end_vert = 242,
                .angle = 16384, .linedef = 296,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 229, .end_vert = 232,
                .angle = 0, .linedef = 304,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 234, .end_vert = 241,
                .angle = -32768, .linedef = 305,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 228, .end_vert = 242,
                .angle = 0, .linedef = 286,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 245, .end_vert = 238,
                .angle = -32768, .linedef = 293,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 246, .end_vert = 245,
                .angle = -16384, .linedef = 294,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 247, .end_vert = 246,
                .angle = -16384, .linedef = 295,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 242, .end_vert = 247,
                .angle = -16384, .linedef = 296,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 238, .end_vert = 228,
                .angle = 16384, .linedef = 309,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 454, .end_vert = 455,
                .angle = 16384, .linedef = 307,
                .direction = 0, .offset = 192
                },
     {
                .begin_vert = 228, .end_vert = 238,
                .angle = -16384, .linedef = 309,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 239, .end_vert = 240,
                .angle = 0, .linedef = 284,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 456, .end_vert = 251,
                .angle = -32768, .linedef = 300,
                .direction = 0, .offset = 224
                },
     {
                .begin_vert = 252, .end_vert = 253,
                .angle = -32768, .linedef = 301,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 253, .end_vert = 457,
                .angle = 16384, .linedef = 307,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 251, .end_vert = 252,
                .angle = -32768, .linedef = 311,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 238, .end_vert = 239,
                .angle = -16384, .linedef = 283,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 457, .end_vert = 454,
                .angle = 16384, .linedef = 307,
                .direction = 0, .offset = 160
                },
     {
                .begin_vert = 234, .end_vert = 235,
                .angle = -16384, .linedef = 280,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 240, .end_vert = 241,
                .angle = 16384, .linedef = 285,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 241, .end_vert = 234,
                .angle = 0, .linedef = 305,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 236, .end_vert = 237,
                .angle = 16384, .linedef = 282,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 250, .end_vert = 458,
                .angle = -32768, .linedef = 300,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 459, .end_vert = 250,
                .angle = -16384, .linedef = 308,
                .direction = 0, .offset = 32
                },
     {
                .begin_vert = 237, .end_vert = 233,
                .angle = 16384, .linedef = 310,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 235, .end_vert = 236,
                .angle = 0, .linedef = 281,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 458, .end_vert = 456,
                .angle = -32768, .linedef = 300,
                .direction = 0, .offset = 168
                },
     {
                .begin_vert = 243, .end_vert = 234,
                .angle = -32768, .linedef = 289,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 232, .end_vert = 244,
                .angle = 0, .linedef = 290,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 248, .end_vert = 243,
                .angle = -16384, .linedef = 297,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 244, .end_vert = 249,
                .angle = -16384, .linedef = 298,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 249, .end_vert = 248,
                .angle = -16384, .linedef = 299,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 237, .end_vert = 243,
                .angle = -32768, .linedef = 288,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 244, .end_vert = 233,
                .angle = 0, .linedef = 291,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 243, .end_vert = 248,
                .angle = 16384, .linedef = 297,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 249, .end_vert = 244,
                .angle = 16384, .linedef = 298,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 248, .end_vert = 249,
                .angle = 16384, .linedef = 299,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 233, .end_vert = 237,
                .angle = -16384, .linedef = 310,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 460, .end_vert = 254,
                .angle = -16384, .linedef = 302,
                .direction = 0, .offset = 56
                },
     {
                .begin_vert = 255, .end_vert = 461,
                .angle = 16384, .linedef = 303,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 254, .end_vert = 256,
                .angle = -32768, .linedef = 312,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 257, .end_vert = 255,
                .angle = -32768, .linedef = 313,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 276, .end_vert = 277,
                .angle = 0, .linedef = 339,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 256, .end_vert = 257,
                .angle = -32768, .linedef = 342,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 251, .end_vert = 460,
                .angle = -16384, .linedef = 302,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 462, .end_vert = 251,
                .angle = 0, .linedef = 311,
                .direction = 0, .offset = 80
                },
     {
                .begin_vert = 277, .end_vert = 274,
                .angle = 16384, .linedef = 340,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 463, .end_vert = 252,
                .angle = 16384, .linedef = 303,
                .direction = 0, .offset = 16
                },
     {
                .begin_vert = 252, .end_vert = 462,
                .angle = 0, .linedef = 311,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 274, .end_vert = 275,
                .angle = -32768, .linedef = 338,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 461, .end_vert = 463,
                .angle = 16384, .linedef = 303,
                .direction = 0, .offset = 8
                },
     {
                .begin_vert = 275, .end_vert = 276,
                .angle = -16384, .linedef = 341,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 275, .end_vert = 274,
                .angle = 0, .linedef = 338,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 277, .end_vert = 276,
                .angle = -32768, .linedef = 339,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 274, .end_vert = 277,
                .angle = -16384, .linedef = 340,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 276, .end_vert = 275,
                .angle = 16384, .linedef = 341,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 258, .end_vert = 257,
                .angle = 16384, .linedef = 314,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 256, .end_vert = 259,
                .angle = -16384, .linedef = 315,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 259, .end_vert = 258,
                .angle = -32768, .linedef = 321,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 257, .end_vert = 256,
                .angle = 0, .linedef = 342,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 259, .end_vert = 260,
                .angle = -16384, .linedef = 316,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 263, .end_vert = 258,
                .angle = 16384, .linedef = 319,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 260, .end_vert = 263,
                .angle = -32768, .linedef = 320,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 258, .end_vert = 259,
                .angle = 0, .linedef = 321,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 261, .end_vert = 264,
                .angle = 0, .linedef = 322,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 265, .end_vert = 262,
                .angle = 0, .linedef = 323,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 264, .end_vert = 464,
                .angle = -16384, .linedef = 324,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 465, .end_vert = 268,
                .angle = 16384, .linedef = 325,
                .direction = 0, .offset = 24
                },
     {
                .begin_vert = 268, .end_vert = 269,
                .angle = 16384, .linedef = 326,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 269, .end_vert = 265,
                .angle = 16384, .linedef = 327,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 262, .end_vert = 261,
                .angle = 0, .linedef = 329,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 278, .end_vert = 279,
                .angle = -32768, .linedef = 343,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 464, .end_vert = 266,
                .angle = -16384, .linedef = 324,
                .direction = 0, .offset = 168
                },
     {
                .begin_vert = 266, .end_vert = 466,
                .angle = -32768, .linedef = 328,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 281, .end_vert = 278,
                .angle = 16384, .linedef = 345,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 267, .end_vert = 467,
                .angle = 16384, .linedef = 325,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 466, .end_vert = 267,
                .angle = -32768, .linedef = 328,
                .direction = 0, .offset = 80
                },
     {
                .begin_vert = 280, .end_vert = 281,
                .angle = 0, .linedef = 344,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 467, .end_vert = 465,
                .angle = 16384, .linedef = 325,
                .direction = 0, .offset = 16
                },
     {
                .begin_vert = 279, .end_vert = 280,
                .angle = -16384, .linedef = 346,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 279, .end_vert = 278,
                .angle = 0, .linedef = 343,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 281, .end_vert = 280,
                .angle = -32768, .linedef = 344,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 278, .end_vert = 281,
                .angle = -16384, .linedef = 345,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 280, .end_vert = 279,
                .angle = 16384, .linedef = 346,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 260, .end_vert = 261,
                .angle = -16384, .linedef = 317,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 262, .end_vert = 263,
                .angle = 16384, .linedef = 318,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 263, .end_vert = 260,
                .angle = 0, .linedef = 320,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 261, .end_vert = 262,
                .angle = -32768, .linedef = 329,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 199, .end_vert = 200,
                .angle = -16384, .linedef = 224,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 202, .end_vert = 201,
                .angle = 18939, .linedef = 225,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 201, .end_vert = 199,
                .angle = 0, .linedef = 229,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 200, .end_vert = 202,
                .angle = -32768, .linedef = 238,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 197, .end_vert = 198,
                .angle = -16384, .linedef = 223,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 200, .end_vert = 199,
                .angle = 16384, .linedef = 224,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 199, .end_vert = 197,
                .angle = 0, .linedef = 230,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 198, .end_vert = 200,
                .angle = -32768, .linedef = 237,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 195, .end_vert = 196,
                .angle = -16384, .linedef = 222,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 198, .end_vert = 197,
                .angle = 16384, .linedef = 223,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 197, .end_vert = 195,
                .angle = 0, .linedef = 231,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 196, .end_vert = 198,
                .angle = -32768, .linedef = 236,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 468, .end_vert = 227,
                .angle = 0, .linedef = 275,
                .direction = 0, .offset = 88
                },
     {
                .begin_vert = 193, .end_vert = 194,
                .angle = -16384, .linedef = 221,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 196, .end_vert = 195,
                .angle = 16384, .linedef = 222,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 195, .end_vert = 193,
                .angle = 0, .linedef = 232,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 194, .end_vert = 196,
                .angle = -32768, .linedef = 235,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 191, .end_vert = 192,
                .angle = -16384, .linedef = 220,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 194, .end_vert = 193,
                .angle = 16384, .linedef = 221,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 193, .end_vert = 191,
                .angle = 0, .linedef = 233,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 192, .end_vert = 194,
                .angle = -32768, .linedef = 234,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 227, .end_vert = 228,
                .angle = -7037, .linedef = 276,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 192, .end_vert = 191,
                .angle = 16384, .linedef = 220,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 191, .end_vert = 205,
                .angle = 0, .linedef = 239,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 206, .end_vert = 192,
                .angle = -32768, .linedef = 240,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 205, .end_vert = 206,
                .angle = -16384, .linedef = 244,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 186, .end_vert = 188,
                .angle = 16384, .linedef = 218,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 189, .end_vert = 190,
                .angle = -16384, .linedef = 219,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 190, .end_vert = 359,
                .angle = -32768, .linedef = 434,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 359, .end_vert = 360,
                .angle = -32768, .linedef = 435,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 360, .end_vert = 186,
                .angle = -32768, .linedef = 436,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 188, .end_vert = 361,
                .angle = 0, .linedef = 437,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 361, .end_vert = 362,
                .angle = 0, .linedef = 438,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 362, .end_vert = 189,
                .angle = 0, .linedef = 439,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 186, .end_vert = 392,
                .angle = 27931, .linedef = 211,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 188, .end_vert = 186,
                .angle = -16384, .linedef = 218,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 213, .end_vert = 188,
                .angle = -6711, .linedef = 256,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 226, .end_vert = 468,
                .angle = 0, .linedef = 275,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 455, .end_vert = 226,
                .angle = 16384, .linedef = 307,
                .direction = 0, .offset = 384
                },
     {
                .begin_vert = 203, .end_vert = 204,
                .angle = -12082, .linedef = 226,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 204, .end_vert = 211,
                .angle = -32768, .linedef = 251,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 469, .end_vert = 212,
                .angle = 6437, .linedef = 253,
                .direction = 0, .offset = 83
                },
     {
                .begin_vert = 212, .end_vert = 203,
                .angle = 0, .linedef = 254,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 190, .end_vert = 189,
                .angle = 16384, .linedef = 219,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 211, .end_vert = 190,
                .angle = -27294, .linedef = 252,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 189, .end_vert = 469,
                .angle = 6437, .linedef = 253,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 201, .end_vert = 202,
                .angle = -13828, .linedef = 225,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 204, .end_vert = 203,
                .angle = 20685, .linedef = 226,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 203, .end_vert = 201,
                .angle = 2902, .linedef = 227,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 202, .end_vert = 204,
                .angle = -27931, .linedef = 228,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 163, .end_vert = 165,
                .angle = -16384, .linedef = 186,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 166, .end_vert = 434,
                .angle = 16384, .linedef = 187,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 207, .end_vert = 166,
                .angle = 16384, .linedef = 243,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 208, .end_vert = 207,
                .angle = 16384, .linedef = 245,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 165, .end_vert = 208,
                .angle = -32768, .linedef = 306,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 208, .end_vert = 165,
                .angle = 0, .linedef = 306,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 270, .end_vert = 208,
                .angle = 16384, .linedef = 330,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 165, .end_vert = 271,
                .angle = -16384, .linedef = 331,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 271, .end_vert = 270,
                .angle = -32768, .linedef = 336,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 207, .end_vert = 206,
                .angle = -32768, .linedef = 241,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 205, .end_vert = 166,
                .angle = 0, .linedef = 242,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 166, .end_vert = 207,
                .angle = -16384, .linedef = 243,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 206, .end_vert = 205,
                .angle = 16384, .linedef = 244,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 229, .end_vert = 230,
                .angle = 16384, .linedef = 277,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 231, .end_vert = 232,
                .angle = -16384, .linedef = 278,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 232, .end_vert = 229,
                .angle = -32768, .linedef = 304,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 230, .end_vert = 272,
                .angle = 13028, .linedef = 332,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 273, .end_vert = 231,
                .angle = -13028, .linedef = 335,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 272, .end_vert = 273,
                .angle = 0, .linedef = 337,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 272, .end_vert = 270,
                .angle = 16384, .linedef = 333,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 271, .end_vert = 273,
                .angle = -16384, .linedef = 334,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 270, .end_vert = 271,
                .angle = 0, .linedef = 336,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 273, .end_vert = 272,
                .angle = -32768, .linedef = 337,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 224, .end_vert = 225,
                .angle = 0, .linedef = 274,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 233, .end_vert = 224,
                .angle = 7037, .linedef = 279,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 225, .end_vert = 459,
                .angle = -16384, .linedef = 308,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 350, .end_vert = 351,
                .angle = 16384, .linedef = 423,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 352, .end_vert = 349,
                .angle = -16384, .linedef = 425,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 349, .end_vert = 357,
                .angle = -32768, .linedef = 430,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 357, .end_vert = 350,
                .angle = -32768, .linedef = 431,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 351, .end_vert = 358,
                .angle = 0, .linedef = 432,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 358, .end_vert = 352,
                .angle = 0, .linedef = 433,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 348, .end_vert = 349,
                .angle = -29412, .linedef = 422,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 352, .end_vert = 353,
                .angle = 13028, .linedef = 424,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 349, .end_vert = 352,
                .angle = 16384, .linedef = 425,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 438, .end_vert = 355,
                .angle = -9672, .linedef = 427,
                .direction = 0, .offset = 68
                },
     {
                .begin_vert = 355, .end_vert = 365,
                .angle = -16384, .linedef = 444,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 365, .end_vert = 348,
                .angle = -16384, .linedef = 459,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 353, .end_vert = 436,
                .angle = 17864, .linedef = 428,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 348, .end_vert = 365,
                .angle = 16384, .linedef = 459,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 369, .end_vert = 367,
                .angle = -16384, .linedef = 460,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 365, .end_vert = 383,
                .angle = 0, .linedef = 465,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 383, .end_vert = 369,
                .angle = 0, .linedef = 466,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 367, .end_vert = 384,
                .angle = -32768, .linedef = 467,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 384, .end_vert = 348,
                .angle = -32768, .linedef = 468,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 366, .end_vert = 367,
                .angle = -32768, .linedef = 445,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 367, .end_vert = 369,
                .angle = 16384, .linedef = 460,
                .direction = 1, .offset = 0
                },
     {
                .begin_vert = 369, .end_vert = 370,
                .angle = 11547, .linedef = 447,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 370, .end_vert = 445,
                .angle = 19286, .linedef = 448,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 446, .end_vert = 374,
                .angle = -16384, .linedef = 452,
                .direction = 0, .offset = 154
                },
     {
                .begin_vert = 368, .end_vert = 366,
                .angle = -22517, .linedef = 446,
                .direction = 0, .offset = 0
                },
     {
                .begin_vert = 374, .end_vert = 368,
                .angle = -13828, .linedef = 451,
                .direction = 0, .offset = 0
                },
 };

static const ssector ssectors[239] = {
    {.num_segs = 8, .first_seg = 0},
     {.num_segs = 1, .first_seg = 8},
     {.num_segs = 1, .first_seg = 9},
     {.num_segs = 1, .first_seg = 10},
     {.num_segs = 1, .first_seg = 11},
     {.num_segs = 1, .first_seg = 12},
     {.num_segs = 1, .first_seg = 13},
     {.num_segs = 6, .first_seg = 14},
     {.num_segs = 6, .first_seg = 20},
     {.num_segs = 1, .first_seg = 26},
     {.num_segs = 3, .first_seg = 27},
     {.num_segs = 4, .first_seg = 30},
     {.num_segs = 4, .first_seg = 34},
     {.num_segs = 4, .first_seg = 38},
     {.num_segs = 5, .first_seg = 42},
     {.num_segs = 4, .first_seg = 47},
     {.num_segs = 4, .first_seg = 51},
     {.num_segs = 4, .first_seg = 55},
     {.num_segs = 4, .first_seg = 59},
     {.num_segs = 2, .first_seg = 63},
     {.num_segs = 3, .first_seg = 65},
     {.num_segs = 2, .first_seg = 68},
     {.num_segs = 1, .first_seg = 70},
     {.num_segs = 4, .first_seg = 71},
     {.num_segs = 2, .first_seg = 75},
     {.num_segs = 6, .first_seg = 77},
     {.num_segs = 4, .first_seg = 83},
     {.num_segs = 4, .first_seg = 87},
     {.num_segs = 6, .first_seg = 91},
     {.num_segs = 3, .first_seg = 97},
     {.num_segs = 2, .first_seg = 100},
     {.num_segs = 4, .first_seg = 102},
     {.num_segs = 3, .first_seg = 106},
     {.num_segs = 3, .first_seg = 109},
     {.num_segs = 2, .first_seg = 112},
     {.num_segs = 4, .first_seg = 114},
     {.num_segs = 6, .first_seg = 118},
     {.num_segs = 2, .first_seg = 124},
     {.num_segs = 4, .first_seg = 126},
     {.num_segs = 3, .first_seg = 130},
     {.num_segs = 3, .first_seg = 133},
     {.num_segs = 2, .first_seg = 136},
     {.num_segs = 4, .first_seg = 138},
     {.num_segs = 5, .first_seg = 142},
     {.num_segs = 4, .first_seg = 147},
     {.num_segs = 3, .first_seg = 151},
     {.num_segs = 2, .first_seg = 154},
     {.num_segs = 3, .first_seg = 156},
     {.num_segs = 2, .first_seg = 159},
     {.num_segs = 2, .first_seg = 161},
     {.num_segs = 4, .first_seg = 163},
     {.num_segs = 5, .first_seg = 167},
     {.num_segs = 6, .first_seg = 172},
     {.num_segs = 3, .first_seg = 178},
     {.num_segs = 1, .first_seg = 181},
     {.num_segs = 4, .first_seg = 182},
     {.num_segs = 4, .first_seg = 186},
     {.num_segs = 8, .first_seg = 190},
     {.num_segs = 4, .first_seg = 198},
     {.num_segs = 2, .first_seg = 202},
     {.num_segs = 4, .first_seg = 204},
     {.num_segs = 1, .first_seg = 208},
     {.num_segs = 1, .first_seg = 209},
     {.num_segs = 2, .first_seg = 210},
     {.num_segs = 2, .first_seg = 212},
     {.num_segs = 1, .first_seg = 214},
     {.num_segs = 5, .first_seg = 215},
     {.num_segs = 1, .first_seg = 220},
     {.num_segs = 3, .first_seg = 221},
     {.num_segs = 4, .first_seg = 224},
     {.num_segs = 1, .first_seg = 228},
     {.num_segs = 3, .first_seg = 229},
     {.num_segs = 1, .first_seg = 232},
     {.num_segs = 4, .first_seg = 233},
     {.num_segs = 2, .first_seg = 237},
     {.num_segs = 2, .first_seg = 239},
     {.num_segs = 5, .first_seg = 241},
     {.num_segs = 4, .first_seg = 246},
     {.num_segs = 7, .first_seg = 250},
     {.num_segs = 3, .first_seg = 257},
     {.num_segs = 4, .first_seg = 260},
     {.num_segs = 3, .first_seg = 264},
     {.num_segs = 3, .first_seg = 267},
     {.num_segs = 4, .first_seg = 270},
     {.num_segs = 4, .first_seg = 274},
     {.num_segs = 3, .first_seg = 278},
     {.num_segs = 2, .first_seg = 281},
     {.num_segs = 4, .first_seg = 283},
     {.num_segs = 3, .first_seg = 287},
     {.num_segs = 2, .first_seg = 290},
     {.num_segs = 4, .first_seg = 292},
     {.num_segs = 5, .first_seg = 296},
     {.num_segs = 5, .first_seg = 301},
     {.num_segs = 4, .first_seg = 306},
     {.num_segs = 4, .first_seg = 310},
     {.num_segs = 4, .first_seg = 314},
     {.num_segs = 4, .first_seg = 318},
     {.num_segs = 2, .first_seg = 322},
     {.num_segs = 2, .first_seg = 324},
     {.num_segs = 2, .first_seg = 326},
     {.num_segs = 4, .first_seg = 328},
     {.num_segs = 1, .first_seg = 332},
     {.num_segs = 1, .first_seg = 333},
     {.num_segs = 1, .first_seg = 334},
     {.num_segs = 1, .first_seg = 335},
     {.num_segs = 1, .first_seg = 336},
     {.num_segs = 1, .first_seg = 337},
     {.num_segs = 8, .first_seg = 338},
     {.num_segs = 2, .first_seg = 346},
     {.num_segs = 2, .first_seg = 348},
     {.num_segs = 4, .first_seg = 350},
     {.num_segs = 1, .first_seg = 354},
     {.num_segs = 3, .first_seg = 355},
     {.num_segs = 2, .first_seg = 358},
     {.num_segs = 2, .first_seg = 360},
     {.num_segs = 1, .first_seg = 362},
     {.num_segs = 4, .first_seg = 363},
     {.num_segs = 1, .first_seg = 367},
     {.num_segs = 3, .first_seg = 368},
     {.num_segs = 2, .first_seg = 371},
     {.num_segs = 1, .first_seg = 373},
     {.num_segs = 5, .first_seg = 374},
     {.num_segs = 4, .first_seg = 379},
     {.num_segs = 2, .first_seg = 383},
     {.num_segs = 4, .first_seg = 385},
     {.num_segs = 2, .first_seg = 389},
     {.num_segs = 3, .first_seg = 391},
     {.num_segs = 2, .first_seg = 394},
     {.num_segs = 2, .first_seg = 396},
     {.num_segs = 4, .first_seg = 398},
     {.num_segs = 2, .first_seg = 402},
     {.num_segs = 2, .first_seg = 404},
     {.num_segs = 3, .first_seg = 406},
     {.num_segs = 6, .first_seg = 409},
     {.num_segs = 3, .first_seg = 415},
     {.num_segs = 1, .first_seg = 418},
     {.num_segs = 2, .first_seg = 419},
     {.num_segs = 3, .first_seg = 421},
     {.num_segs = 2, .first_seg = 424},
     {.num_segs = 1, .first_seg = 426},
     {.num_segs = 1, .first_seg = 427},
     {.num_segs = 4, .first_seg = 428},
     {.num_segs = 1, .first_seg = 432},
     {.num_segs = 1, .first_seg = 433},
     {.num_segs = 2, .first_seg = 434},
     {.num_segs = 4, .first_seg = 436},
     {.num_segs = 1, .first_seg = 440},
     {.num_segs = 2, .first_seg = 441},
     {.num_segs = 3, .first_seg = 443},
     {.num_segs = 4, .first_seg = 446},
     {.num_segs = 2, .first_seg = 450},
     {.num_segs = 4, .first_seg = 452},
     {.num_segs = 3, .first_seg = 456},
     {.num_segs = 2, .first_seg = 459},
     {.num_segs = 2, .first_seg = 461},
     {.num_segs = 4, .first_seg = 463},
     {.num_segs = 4, .first_seg = 467},
     {.num_segs = 1, .first_seg = 471},
     {.num_segs = 2, .first_seg = 472},
     {.num_segs = 2, .first_seg = 474},
     {.num_segs = 2, .first_seg = 476},
     {.num_segs = 1, .first_seg = 478},
     {.num_segs = 1, .first_seg = 479},
     {.num_segs = 4, .first_seg = 480},
     {.num_segs = 2, .first_seg = 484},
     {.num_segs = 4, .first_seg = 486},
     {.num_segs = 2, .first_seg = 490},
     {.num_segs = 2, .first_seg = 492},
     {.num_segs = 5, .first_seg = 494},
     {.num_segs = 2, .first_seg = 499},
     {.num_segs = 4, .first_seg = 501},
     {.num_segs = 4, .first_seg = 505},
     {.num_segs = 4, .first_seg = 509},
     {.num_segs = 3, .first_seg = 513},
     {.num_segs = 2, .first_seg = 516},
     {.num_segs = 2, .first_seg = 518},
     {.num_segs = 5, .first_seg = 520},
     {.num_segs = 2, .first_seg = 525},
     {.num_segs = 5, .first_seg = 527},
     {.num_segs = 2, .first_seg = 532},
     {.num_segs = 2, .first_seg = 534},
     {.num_segs = 1, .first_seg = 536},
     {.num_segs = 4, .first_seg = 537},
     {.num_segs = 2, .first_seg = 541},
     {.num_segs = 4, .first_seg = 543},
     {.num_segs = 1, .first_seg = 547},
     {.num_segs = 3, .first_seg = 548},
     {.num_segs = 2, .first_seg = 551},
     {.num_segs = 7, .first_seg = 553},
     {.num_segs = 6, .first_seg = 560},
     {.num_segs = 2, .first_seg = 566},
     {.num_segs = 5, .first_seg = 568},
     {.num_segs = 2, .first_seg = 573},
     {.num_segs = 3, .first_seg = 575},
     {.num_segs = 4, .first_seg = 578},
     {.num_segs = 2, .first_seg = 582},
     {.num_segs = 5, .first_seg = 584},
     {.num_segs = 6, .first_seg = 589},
     {.num_segs = 6, .first_seg = 595},
     {.num_segs = 3, .first_seg = 601},
     {.num_segs = 3, .first_seg = 604},
     {.num_segs = 2, .first_seg = 607},
     {.num_segs = 4, .first_seg = 609},
     {.num_segs = 4, .first_seg = 613},
     {.num_segs = 4, .first_seg = 617},
     {.num_segs = 8, .first_seg = 621},
     {.num_segs = 3, .first_seg = 629},
     {.num_segs = 3, .first_seg = 632},
     {.num_segs = 2, .first_seg = 635},
     {.num_segs = 4, .first_seg = 637},
     {.num_segs = 4, .first_seg = 641},
     {.num_segs = 4, .first_seg = 645},
     {.num_segs = 4, .first_seg = 649},
     {.num_segs = 4, .first_seg = 653},
     {.num_segs = 1, .first_seg = 657},
     {.num_segs = 4, .first_seg = 658},
     {.num_segs = 4, .first_seg = 662},
     {.num_segs = 1, .first_seg = 666},
     {.num_segs = 4, .first_seg = 667},
     {.num_segs = 8, .first_seg = 671},
     {.num_segs = 3, .first_seg = 679},
     {.num_segs = 2, .first_seg = 682},
     {.num_segs = 4, .first_seg = 684},
     {.num_segs = 3, .first_seg = 688},
     {.num_segs = 4, .first_seg = 691},
     {.num_segs = 5, .first_seg = 695},
     {.num_segs = 4, .first_seg = 700},
     {.num_segs = 4, .first_seg = 704},
     {.num_segs = 6, .first_seg = 708},
     {.num_segs = 4, .first_seg = 714},
     {.num_segs = 3, .first_seg = 718},
     {.num_segs = 6, .first_seg = 721},
     {.num_segs = 6, .first_seg = 727},
     {.num_segs = 1, .first_seg = 733},
     {.num_segs = 6, .first_seg = 734},
     {.num_segs = 2, .first_seg = 740},
     {.num_segs = 1, .first_seg = 742},
     {.num_segs = 2, .first_seg = 743},
     {.num_segs = 2, .first_seg = 745},
 };

static const node nodes[238] = {
    {
            .split_x = 1784, .split_y = -3448,
            .split_dx = -240, .split_dy = 64,
            .right_box_top = -3104, .right_box_bottom = -3448,
            .right_box_left = 1520, .right_box_right = 2128,
            .left_box_top = -3384, .left_box_bottom = -3448,
            .left_box_left = 1544, .left_box_right = 1784,
            .right_child = 32768, .left_child = 32769
            },
     {
            .split_x = 2064, .split_y = -3408,
            .split_dx = -280, .split_dy = -40,
            .right_box_top = -3104, .right_box_bottom = -3448,
            .right_box_left = 1520, .right_box_right = 2128,
            .left_box_top = -3408, .left_box_bottom = -3448,
            .left_box_left = 1784, .left_box_right = 2064,
            .right_child = 0, .left_child = 32770
            },
     {
            .split_x = 2128, .split_y = -3272,
            .split_dx = -64, .split_dy = -136,
            .right_box_top = -3104, .right_box_bottom = -3448,
            .right_box_left = 1520, .right_box_right = 2128,
            .left_box_top = -3272, .left_box_bottom = -3408,
            .left_box_left = 2064, .left_box_right = 2128,
            .right_child = 1, .left_child = 32771
            },
     {
            .split_x = 2040, .split_y = -3144,
            .split_dx = 88, .split_dy = -128,
            .right_box_top = -3104, .right_box_bottom = -3448,
            .right_box_left = 1520, .right_box_right = 2128,
            .left_box_top = -3144, .left_box_bottom = -3272,
            .left_box_left = 2040, .left_box_right = 2128,
            .right_child = 2, .left_child = 32772
            },
     {
            .split_x = 1896, .split_y = -3104,
            .split_dx = 144, .split_dy = -40,
            .right_box_top = -3104, .right_box_bottom = -3448,
            .right_box_left = 1520, .right_box_right = 2128,
            .left_box_top = -3104, .left_box_bottom = -3144,
            .left_box_left = 1896, .left_box_right = 2040,
            .right_child = 3, .left_child = 32773
            },
     {
            .split_x = 1520, .split_y = -3168,
            .split_dx = 152, .split_dy = 64,
            .right_box_top = -3104, .right_box_bottom = -3448,
            .right_box_left = 1520, .right_box_right = 2128,
            .left_box_top = -3104, .left_box_bottom = -3168,
            .left_box_left = 1520, .left_box_right = 1672,
            .right_child = 4, .left_child = 32774
            },
     {
            .split_x = 1544, .split_y = -3384,
            .split_dx = -24, .split_dy = 216,
            .right_box_top = -3104, .right_box_bottom = -3448,
            .right_box_left = 1520, .right_box_right = 2128,
            .left_box_top = -3104, .left_box_bottom = -3520,
            .left_box_left = 1376, .left_box_right = 1544,
            .right_child = 5, .left_child = 32775
            },
     {
            .split_x = 1920, .split_y = -2920,
            .split_dx = 256, .split_dy = 0,
            .right_box_top = -2920, .right_box_bottom = -2920,
            .right_box_left = 1920, .right_box_right = 2176,
            .left_box_top = -2880, .left_box_bottom = -2920,
            .left_box_left = 1856, .left_box_right = 2176,
            .right_child = 32777, .left_child = 32778
            },
     {
            .split_x = 1856, .split_y = -2880,
            .split_dx = 64, .split_dy = -40,
            .right_box_top = -2880, .right_box_bottom = -3104,
            .right_box_left = 1376, .right_box_right = 1920,
            .left_box_top = -2880, .left_box_bottom = -2920,
            .left_box_left = 1856, .left_box_right = 2176,
            .right_child = 32776, .left_child = 7
            },
     {
            .split_x = 1672, .split_y = -3104,
            .split_dx = 224, .split_dy = 0,
            .right_box_top = -3104, .right_box_bottom = -3520,
            .right_box_left = 1376, .right_box_right = 2128,
            .left_box_top = -2880, .left_box_bottom = -3104,
            .left_box_left = 1376, .left_box_right = 2176,
            .right_child = 6, .left_child = 8
            },
     {
            .split_x = 1376, .split_y = -3360,
            .split_dx = -32, .split_dy = 0,
            .right_box_top = -3264, .right_box_bottom = -3360,
            .right_box_left = 1344, .right_box_right = 1376,
            .left_box_top = -3392, .left_box_bottom = -3520,
            .left_box_left = 1344, .left_box_right = 1376,
            .right_child = 32779, .left_child = 32780
            },
     {
            .split_x = 1344, .split_y = -3264,
            .split_dx = 32, .split_dy = 0,
            .right_box_top = -3264, .right_box_bottom = -3520,
            .right_box_left = 1344, .right_box_right = 1376,
            .left_box_top = -3104, .left_box_bottom = -3200,
            .left_box_left = 1344, .left_box_right = 1376,
            .right_child = 10, .left_child = 32781
            },
     {
            .split_x = 1376, .split_y = -3200,
            .split_dx = 0, .split_dy = 96,
            .right_box_top = -2880, .right_box_bottom = -3520,
            .right_box_left = 1376, .right_box_right = 2176,
            .left_box_top = -3104, .left_box_bottom = -3520,
            .left_box_left = 1344, .left_box_right = 1376,
            .right_child = 9, .left_child = 11
            },
     {
            .split_x = 2048, .split_y = -3904,
            .split_dx = 128, .split_dy = 0,
            .right_box_top = -3904, .right_box_bottom = -4064,
            .right_box_left = 2048, .right_box_right = 2176,
            .left_box_top = -3872, .left_box_bottom = -3904,
            .left_box_left = 2048, .left_box_right = 2176,
            .right_child = 32782, .left_child = 32783
            },
     {
            .split_x = 2048, .split_y = -3840,
            .split_dx = 128, .split_dy = 0,
            .right_box_top = -3840, .right_box_bottom = -3872,
            .right_box_left = 2048, .right_box_right = 2176,
            .left_box_top = -3808, .left_box_bottom = -3840,
            .left_box_left = 2048, .left_box_right = 2176,
            .right_child = 32784, .left_child = 32785
            },
     {
            .split_x = 2048, .split_y = -3808,
            .split_dx = 128, .split_dy = 0,
            .right_box_top = -3808, .right_box_bottom = -3872,
            .right_box_left = 2048, .right_box_right = 2176,
            .left_box_top = -3776, .left_box_bottom = -3808,
            .left_box_left = 2048, .left_box_right = 2176,
            .right_child = 14, .left_child = 32786
            },
     {
            .split_x = 2048, .split_y = -3872,
            .split_dx = 128, .split_dy = 0,
            .right_box_top = -3872, .right_box_bottom = -4064,
            .right_box_left = 2048, .right_box_right = 2176,
            .left_box_top = -3776, .left_box_bottom = -3872,
            .left_box_left = 2048, .left_box_right = 2176,
            .right_child = 13, .left_child = 15
            },
     {
            .split_x = 1984, .split_y = -3648,
            .split_dx = -608, .split_dy = 0,
            .right_box_top = -3520, .right_box_bottom = -3648,
            .right_box_left = 1376, .right_box_right = 1984,
            .left_box_top = -3648, .left_box_bottom = -3776,
            .left_box_left = 1376, .left_box_right = 1984,
            .right_child = 32787, .left_child = 32788
            },
     {
            .split_x = 1528, .split_y = -3680,
            .split_dx = -152, .split_dy = 32,
            .right_box_top = -3520, .right_box_bottom = -3776,
            .right_box_left = 1376, .right_box_right = 1984,
            .left_box_top = -3680, .left_box_bottom = -3776,
            .left_box_left = 1528, .left_box_right = 1984,
            .right_child = 17, .left_child = 32789
            },
     {
            .split_x = 2016, .split_y = -3680,
            .split_dx = -32, .split_dy = -96,
            .right_box_top = -3680, .right_box_bottom = -3776,
            .right_box_left = 1984, .right_box_right = 2016,
            .left_box_top = -3680, .left_box_bottom = -3776,
            .left_box_left = 2048, .left_box_right = 2176,
            .right_child = 32792, .left_child = 32793
            },
     {
            .split_x = 2048, .split_y = -3680,
            .split_dx = -32, .split_dy = 0,
            .right_box_top = -3648, .right_box_bottom = -3680,
            .right_box_left = 1984, .right_box_right = 2176,
            .left_box_top = -3680, .left_box_bottom = -3776,
            .left_box_left = 1984, .left_box_right = 2176,
            .right_child = 32791, .left_child = 19
            },
     {
            .split_x = 2176, .split_y = -3648,
            .split_dx = -192, .split_dy = 0,
            .right_box_top = -3648, .right_box_bottom = -3648,
            .right_box_left = 1984, .right_box_right = 2176,
            .left_box_top = -3648, .left_box_bottom = -3776,
            .left_box_left = 1984, .left_box_right = 2176,
            .right_child = 32790, .left_child = 20
            },
     {
            .split_x = 1984, .split_y = -3648,
            .split_dx = 0, .split_dy = -128,
            .right_box_top = -3520, .right_box_bottom = -3776,
            .right_box_left = 1376, .right_box_right = 1984,
            .left_box_top = -3648, .left_box_bottom = -3776,
            .left_box_left = 1984, .left_box_right = 2176,
            .right_child = 18, .left_child = 21
            },
     {
            .split_x = 2048, .split_y = -3776,
            .split_dx = 128, .split_dy = 0,
            .right_box_top = -3776, .right_box_bottom = -4064,
            .right_box_left = 2048, .right_box_right = 2176,
            .left_box_top = -3520, .left_box_bottom = -3776,
            .left_box_left = 1376, .left_box_right = 2176,
            .right_child = 16, .left_child = 22
            },
     {
            .split_x = 1376, .split_y = -3520,
            .split_dx = -32, .split_dy = 0,
            .right_box_top = -2880, .right_box_bottom = -3520,
            .right_box_left = 1344, .right_box_right = 2176,
            .left_box_top = -3520, .left_box_bottom = -4064,
            .left_box_left = 1376, .left_box_right = 2176,
            .right_child = 12, .left_child = 23
            },
     {
            .split_x = 2176, .split_y = -2816,
            .split_dx = -512, .split_dy = 0,
            .right_box_top = -2752, .right_box_bottom = -2816,
            .right_box_left = 1664, .right_box_right = 2176,
            .left_box_top = -2816, .left_box_bottom = -2880,
            .left_box_left = 1664, .left_box_right = 2176,
            .right_child = 32794, .left_child = 32795
            },
     {
            .split_x = 1792, .split_y = -2304,
            .split_dx = 192, .split_dy = 0,
            .right_box_top = -2304, .right_box_bottom = -2312,
            .right_box_left = 1792, .right_box_right = 1984,
            .left_box_top = -2240, .left_box_bottom = -2304,
            .left_box_left = 1792, .left_box_right = 1984,
            .right_child = 32802, .left_child = 32803
            },
     {
            .split_x = 1792, .split_y = -2240,
            .split_dx = 0, .split_dy = -64,
            .right_box_top = -2240, .right_box_bottom = -2312,
            .right_box_left = 1784, .right_box_right = 1792,
            .left_box_top = -2240, .left_box_bottom = -2312,
            .left_box_left = 1792, .left_box_right = 1984,
            .right_child = 32801, .left_child = 26
            },
     {
            .split_x = 1984, .split_y = -2240,
            .split_dx = -192, .split_dy = 0,
            .right_box_top = -2232, .right_box_bottom = -2240,
            .right_box_left = 1784, .right_box_right = 1984,
            .left_box_top = -2240, .left_box_bottom = -2312,
            .left_box_left = 1784, .left_box_right = 1984,
            .right_child = 32800, .left_child = 27
            },
     {
            .split_x = 1984, .split_y = -2304,
            .split_dx = 0, .split_dy = 64,
            .right_box_top = -2232, .right_box_bottom = -2312,
            .right_box_left = 1984, .right_box_right = 1992,
            .left_box_top = -2232, .left_box_bottom = -2312,
            .left_box_left = 1784, .left_box_right = 1984,
            .right_child = 32799, .left_child = 28
            },
     {
            .split_x = 1784, .split_y = -2232,
            .split_dx = 0, .split_dy = -80,
            .right_box_top = -2232, .right_box_bottom = -2312,
            .right_box_left = 1664, .right_box_right = 1784,
            .left_box_top = -2232, .left_box_bottom = -2312,
            .left_box_left = 1784, .left_box_right = 1992,
            .right_child = 32798, .left_child = 29
            },
     {
            .split_x = 1992, .split_y = -2232,
            .split_dx = -208, .split_dy = 0,
            .right_box_top = -2112, .right_box_bottom = -2232,
            .right_box_left = 1664, .right_box_right = 1992,
            .left_box_top = -2232, .left_box_bottom = -2312,
            .left_box_left = 1664, .left_box_right = 1992,
            .right_child = 32797, .left_child = 30
            },
     {
            .split_x = 1784, .split_y = -2312,
            .split_dx = 208, .split_dy = 0,
            .right_box_top = -2312, .right_box_bottom = -2552,
            .right_box_left = 1664, .right_box_right = 1992,
            .left_box_top = -2112, .left_box_bottom = -2312,
            .left_box_left = 1664, .left_box_right = 1992,
            .right_child = 32796, .left_child = 31
            },
     {
            .split_x = 1792, .split_y = -2624,
            .split_dx = 192, .split_dy = 0,
            .right_box_top = -2624, .right_box_bottom = -2632,
            .right_box_left = 1792, .right_box_right = 1984,
            .left_box_top = -2560, .left_box_bottom = -2624,
            .left_box_left = 1792, .left_box_right = 1984,
            .right_child = 32809, .left_child = 32810
            },
     {
            .split_x = 1792, .split_y = -2560,
            .split_dx = 0, .split_dy = -64,
            .right_box_top = -2560, .right_box_bottom = -2632,
            .right_box_left = 1784, .right_box_right = 1792,
            .left_box_top = -2560, .left_box_bottom = -2632,
            .left_box_left = 1792, .left_box_right = 1984,
            .right_child = 32808, .left_child = 33
            },
     {
            .split_x = 1984, .split_y = -2560,
            .split_dx = -192, .split_dy = 0,
            .right_box_top = -2552, .right_box_bottom = -2560,
            .right_box_left = 1784, .right_box_right = 1984,
            .left_box_top = -2560, .left_box_bottom = -2632,
            .left_box_left = 1784, .left_box_right = 1984,
            .right_child = 32807, .left_child = 34
            },
     {
            .split_x = 1984, .split_y = -2624,
            .split_dx = 0, .split_dy = 64,
            .right_box_top = -2552, .right_box_bottom = -2632,
            .right_box_left = 1984, .right_box_right = 1992,
            .left_box_top = -2552, .left_box_bottom = -2632,
            .left_box_left = 1784, .left_box_right = 1984,
            .right_child = 32806, .left_child = 35
            },
     {
            .split_x = 1784, .split_y = -2632,
            .split_dx = 208, .split_dy = 0,
            .right_box_top = -2632, .right_box_bottom = -2752,
            .right_box_left = 1784, .right_box_right = 1992,
            .left_box_top = -2552, .left_box_bottom = -2632,
            .left_box_left = 1784, .left_box_right = 1992,
            .right_child = 32805, .left_child = 36
            },
     {
            .split_x = 1784, .split_y = -2552,
            .split_dx = 0, .split_dy = -80,
            .right_box_top = -2552, .right_box_bottom = -2752,
            .right_box_left = 1664, .right_box_right = 1784,
            .left_box_top = -2552, .left_box_bottom = -2752,
            .left_box_left = 1784, .left_box_right = 1992,
            .right_child = 32804, .left_child = 37
            },
     {
            .split_x = 1992, .split_y = -2552,
            .split_dx = -208, .split_dy = 0,
            .right_box_top = -2112, .right_box_bottom = -2552,
            .right_box_left = 1664, .right_box_right = 1992,
            .left_box_top = -2552, .left_box_bottom = -2752,
            .left_box_left = 1664, .left_box_right = 1992,
            .right_child = 32, .left_child = 38
            },
     {
            .split_x = 2144, .split_y = -2304,
            .split_dx = 0, .split_dy = -256,
            .right_box_top = -2304, .right_box_bottom = -2560,
            .right_box_left = 1992, .right_box_right = 2144,
            .left_box_top = -2304, .left_box_bottom = -2560,
            .left_box_left = 2144, .left_box_right = 2176,
            .right_child = 32811, .left_child = 32812
            },
     {
            .split_x = 2112, .split_y = -2592,
            .split_dx = 64, .split_dy = 0,
            .right_box_top = -2592, .right_box_bottom = -2752,
            .right_box_left = 1992, .right_box_right = 2176,
            .left_box_top = -2560, .left_box_bottom = -2592,
            .left_box_left = 1992, .left_box_right = 2112,
            .right_child = 32813, .left_child = 32814
            },
     {
            .split_x = 2176, .split_y = -2560,
            .split_dx = -32, .split_dy = 0,
            .right_box_top = -2304, .right_box_bottom = -2560,
            .right_box_left = 1992, .right_box_right = 2176,
            .left_box_top = -2560, .left_box_bottom = -2752,
            .left_box_left = 1992, .left_box_right = 2176,
            .right_child = 40, .left_child = 41
            },
     {
            .split_x = 2176, .split_y = -2272,
            .split_dx = -64, .split_dy = 0,
            .right_box_top = -2112, .right_box_bottom = -2272,
            .right_box_left = 1992, .right_box_right = 2176,
            .left_box_top = -2272, .left_box_bottom = -2304,
            .left_box_left = 1992, .left_box_right = 2112,
            .right_child = 32815, .left_child = 32816
            },
     {
            .split_x = 2112, .split_y = -2304,
            .split_dx = 32, .split_dy = 0,
            .right_box_top = -2304, .right_box_bottom = -2752,
            .right_box_left = 1992, .right_box_right = 2176,
            .left_box_top = -2112, .left_box_bottom = -2304,
            .left_box_left = 1992, .left_box_right = 2176,
            .right_child = 42, .left_child = 43
            },
     {
            .split_x = 1992, .split_y = -2552,
            .split_dx = 0, .split_dy = -80,
            .right_box_top = -2112, .right_box_bottom = -2752,
            .right_box_left = 1664, .right_box_right = 1992,
            .left_box_top = -2112, .left_box_bottom = -2752,
            .left_box_left = 1992, .left_box_right = 2176,
            .right_child = 39, .left_child = 44
            },
     {
            .split_x = 1664, .split_y = -2112,
            .split_dx = 512, .split_dy = 0,
            .right_box_top = -2112, .right_box_bottom = -2752,
            .right_box_left = 1664, .right_box_right = 2176,
            .left_box_top = -2048, .left_box_bottom = -2112,
            .left_box_left = 1664, .left_box_right = 2176,
            .right_child = 45, .left_child = 32817
            },
     {
            .split_x = 1664, .split_y = -2752,
            .split_dx = 512, .split_dy = 0,
            .right_box_top = -2752, .right_box_bottom = -2880,
            .right_box_left = 1664, .right_box_right = 2176,
            .left_box_top = -2048, .left_box_bottom = -2752,
            .left_box_left = 1664, .left_box_right = 2176,
            .right_child = 25, .left_child = 46
            },
     {
            .split_x = 1552, .split_y = -2432,
            .split_dx = 112, .split_dy = 0,
            .right_box_top = -2432, .right_box_bottom = -2560,
            .right_box_left = 1552, .right_box_right = 1664,
            .left_box_top = -2048, .left_box_bottom = -2368,
            .left_box_left = 1600, .left_box_right = 1664,
            .right_child = 32818, .left_child = 32819
            },
     {
            .split_x = 1664, .split_y = -2560,
            .split_dx = -112, .split_dy = 0,
            .right_box_top = -2048, .right_box_bottom = -2560,
            .right_box_left = 1552, .right_box_right = 1664,
            .left_box_top = -2624, .left_box_bottom = -2816,
            .left_box_left = 1600, .left_box_right = 1664,
            .right_child = 48, .left_child = 32820
            },
     {
            .split_x = 1472, .split_y = -2560,
            .split_dx = -88, .split_dy = -32,
            .right_box_top = -2432, .right_box_bottom = -2592,
            .right_box_left = 1344, .right_box_right = 1472,
            .left_box_top = -2592, .left_box_bottom = -2880,
            .left_box_left = 1344, .left_box_right = 1384,
            .right_child = 32821, .left_child = 32822
            },
     {
            .split_x = 1536, .split_y = -2432,
            .split_dx = 0, .split_dy = -128,
            .right_box_top = -2432, .right_box_bottom = -2560,
            .right_box_left = 1472, .right_box_right = 1536,
            .left_box_top = -2432, .left_box_bottom = -2560,
            .left_box_left = 1536, .left_box_right = 1552,
            .right_child = 32823, .left_child = 32824
            },
     {
            .split_x = 1472, .split_y = -2432,
            .split_dx = 0, .split_dy = -128,
            .right_box_top = -2432, .right_box_bottom = -2880,
            .right_box_left = 1344, .right_box_right = 1472,
            .left_box_top = -2432, .left_box_bottom = -2560,
            .left_box_left = 1472, .left_box_right = 1552,
            .right_child = 50, .left_child = 51
            },
     {
            .split_x = 1552, .split_y = -2560,
            .split_dx = 0, .split_dy = 128,
            .right_box_top = -2048, .right_box_bottom = -2816,
            .right_box_left = 1552, .right_box_right = 1664,
            .left_box_top = -2432, .left_box_bottom = -2880,
            .left_box_left = 1344, .left_box_right = 1552,
            .right_child = 49, .left_child = 52
            },
     {
            .split_x = 1664, .split_y = -2600,
            .split_dx = 0, .split_dy = 40,
            .right_box_top = -2048, .right_box_bottom = -2880,
            .right_box_left = 1664, .right_box_right = 2176,
            .left_box_top = -2048, .left_box_bottom = -2880,
            .left_box_left = 1344, .left_box_right = 1664,
            .right_child = 47, .left_child = 53
            },
     {
            .split_x = 1664, .split_y = -2880,
            .split_dx = 192, .split_dy = 0,
            .right_box_top = -2880, .right_box_bottom = -4064,
            .right_box_left = 1344, .right_box_right = 2176,
            .left_box_top = -2048, .left_box_bottom = -2880,
            .left_box_left = 1344, .left_box_right = 2176,
            .right_child = 24, .left_child = 54
            },
     {
            .split_x = 896, .split_y = -3360,
            .split_dx = 0, .split_dy = 256,
            .right_box_top = -3104, .right_box_bottom = -3360,
            .right_box_left = 896, .right_box_right = 928,
            .left_box_top = -3104, .left_box_bottom = -3360,
            .left_box_left = 704, .left_box_right = 896,
            .right_child = 32826, .left_child = 32827
            },
     {
            .split_x = 928, .split_y = -3360,
            .split_dx = 0, .split_dy = 256,
            .right_box_top = -3104, .right_box_bottom = -3360,
            .right_box_left = 928, .right_box_right = 1344,
            .left_box_top = -3104, .left_box_bottom = -3360,
            .left_box_left = 704, .left_box_right = 928,
            .right_child = 32825, .left_child = 56
            },
     {
            .split_x = 928, .split_y = -3072,
            .split_dx = 256, .split_dy = 0,
            .right_box_top = -3072, .right_box_bottom = -3104,
            .right_box_left = 928, .right_box_right = 1184,
            .left_box_top = -3072, .left_box_bottom = -3072,
            .left_box_left = 928, .left_box_right = 1184,
            .right_child = 32828, .left_child = 32829
            },
     {
            .split_x = 1216, .split_y = -3072,
            .split_dx = -32, .split_dy = 0,
            .right_box_top = -3072, .right_box_bottom = -3072,
            .right_box_left = 1184, .right_box_right = 1216,
            .left_box_top = -3072, .left_box_bottom = -3104,
            .left_box_left = 1216, .left_box_right = 1344,
            .right_child = 32830, .left_child = 32831
            },
     {
            .split_x = 1184, .split_y = -3072,
            .split_dx = 0, .split_dy = -32,
            .right_box_top = -3072, .right_box_bottom = -3104,
            .right_box_left = 928, .right_box_right = 1184,
            .left_box_top = -3072, .left_box_bottom = -3104,
            .left_box_left = 1184, .left_box_right = 1344,
            .right_child = 58, .left_child = 59
            },
     {
            .split_x = 896, .split_y = -3072,
            .split_dx = 0, .split_dy = -32,
            .right_box_top = -2944, .right_box_bottom = -3104,
            .right_box_left = 704, .right_box_right = 896,
            .left_box_top = -3072, .left_box_bottom = -3072,
            .left_box_left = 896, .left_box_right = 928,
            .right_child = 32832, .left_child = 32833
            },
     {
            .split_x = 928, .split_y = -3104,
            .split_dx = 0, .split_dy = 32,
            .right_box_top = -3072, .right_box_bottom = -3104,
            .right_box_left = 928, .right_box_right = 1344,
            .left_box_top = -2944, .left_box_bottom = -3104,
            .left_box_left = 704, .left_box_right = 928,
            .right_child = 60, .left_child = 61
            },
     {
            .split_x = 832, .split_y = -2944,
            .split_dx = 136, .split_dy = 64,
            .right_box_top = -2880, .right_box_bottom = -3104,
            .right_box_left = 832, .right_box_right = 1344,
            .left_box_top = -2944, .left_box_bottom = -2944,
            .left_box_left = 704, .left_box_right = 832,
            .right_child = 32834, .left_child = 32835
            },
     {
            .split_x = 968, .split_y = -2880,
            .split_dx = 248, .split_dy = 0,
            .right_box_top = -2880, .right_box_bottom = -3104,
            .right_box_left = 704, .right_box_right = 1344,
            .left_box_top = -2486, .left_box_bottom = -2880,
            .left_box_left = 1216, .left_box_right = 1344,
            .right_child = 63, .left_child = 32836
            },
     {
            .split_x = 1216, .split_y = -3072,
            .split_dx = 128, .split_dy = -32,
            .right_box_top = -2944, .right_box_bottom = -3104,
            .right_box_left = 704, .right_box_right = 1344,
            .left_box_top = -2486, .left_box_bottom = -3104,
            .left_box_left = 704, .left_box_right = 1344,
            .right_child = 62, .left_child = 64
            },
     {
            .split_x = 896, .split_y = -3104,
            .split_dx = 32, .split_dy = 0,
            .right_box_top = -3104, .right_box_bottom = -3360,
            .right_box_left = 704, .right_box_right = 1344,
            .left_box_top = -2486, .left_box_bottom = -3104,
            .left_box_left = 704, .left_box_right = 1344,
            .right_child = 57, .left_child = 65
            },
     {
            .split_x = 1184, .split_y = -3392,
            .split_dx = -256, .split_dy = 0,
            .right_box_top = -3360, .right_box_bottom = -3392,
            .right_box_left = 928, .right_box_right = 1184,
            .left_box_top = -3392, .left_box_bottom = -3392,
            .left_box_left = 928, .left_box_right = 1184,
            .right_child = 32837, .left_child = 32838
            },
     {
            .split_x = 896, .split_y = -3360,
            .split_dx = 0, .split_dy = -32,
            .right_box_top = -3360, .right_box_bottom = -3552,
            .right_box_left = 704, .right_box_right = 896,
            .left_box_top = -3392, .left_box_bottom = -3392,
            .left_box_left = 896, .left_box_right = 928,
            .right_child = 32839, .left_child = 32840
            },
     {
            .split_x = 928, .split_y = -3392,
            .split_dx = 0, .split_dy = 32,
            .right_box_top = -3360, .right_box_bottom = -3392,
            .right_box_left = 928, .right_box_right = 1184,
            .left_box_top = -3360, .left_box_bottom = -3552,
            .left_box_left = 704, .left_box_right = 928,
            .right_child = 67, .left_child = 68
            },
     {
            .split_x = 1344, .split_y = -3360,
            .split_dx = -128, .split_dy = -32,
            .right_box_top = -3360, .right_box_bottom = -3392,
            .right_box_left = 1216, .right_box_right = 1344,
            .left_box_top = -3360, .left_box_bottom = -3392,
            .left_box_left = 1216, .left_box_right = 1344,
            .right_child = 32842, .left_child = 32843
            },
     {
            .split_x = 1184, .split_y = -3392,
            .split_dx = 32, .split_dy = 0,
            .right_box_top = -3392, .right_box_bottom = -3552,
            .right_box_left = 1184, .right_box_right = 1344,
            .left_box_top = -3360, .left_box_bottom = -3392,
            .left_box_left = 1216, .left_box_right = 1344,
            .right_child = 32841, .left_child = 70
            },
     {
            .split_x = 1184, .split_y = -3360,
            .split_dx = 0, .split_dy = -32,
            .right_box_top = -3360, .right_box_bottom = -3552,
            .right_box_left = 704, .right_box_right = 1184,
            .left_box_top = -3360, .left_box_bottom = -3552,
            .left_box_left = 1184, .left_box_right = 1344,
            .right_child = 69, .left_child = 71
            },
     {
            .split_x = 1152, .split_y = -3648,
            .split_dx = -64, .split_dy = 0,
            .right_box_top = -3552, .right_box_bottom = -3648,
            .right_box_left = 832, .right_box_right = 1280,
            .left_box_top = -3648, .left_box_bottom = -3680,
            .left_box_left = 1024, .left_box_right = 1088,
            .right_child = 32844, .left_child = 32845
            },
     {
            .split_x = 1344, .split_y = -3552,
            .split_dx = -64, .split_dy = 0,
            .right_box_top = -3360, .right_box_bottom = -3552,
            .right_box_left = 704, .right_box_right = 1344,
            .left_box_top = -3552, .left_box_bottom = -3680,
            .left_box_left = 832, .left_box_right = 1280,
            .right_child = 72, .left_child = 73
            },
     {
            .split_x = 1184, .split_y = -3360,
            .split_dx = -256, .split_dy = 0,
            .right_box_top = -2486, .right_box_bottom = -3360,
            .right_box_left = 704, .right_box_right = 1344,
            .left_box_top = -3360, .left_box_bottom = -3680,
            .left_box_left = 704, .left_box_right = 1344,
            .right_child = 66, .left_child = 74
            },
     {
            .split_x = 512, .split_y = -3328,
            .split_dx = 0, .split_dy = 24,
            .right_box_top = -3104, .right_box_bottom = -3360,
            .right_box_left = 512, .right_box_right = 704,
            .left_box_top = -3160, .left_box_bottom = -3304,
            .left_box_left = 496, .left_box_right = 512,
            .right_child = 32846, .left_child = 32847
            },
     {
            .split_x = 288, .split_y = -3264,
            .split_dx = 0, .split_dy = 64,
            .right_box_top = -3200, .right_box_bottom = -3264,
            .right_box_left = 288, .right_box_right = 320,
            .left_box_top = -3200, .left_box_bottom = -3264,
            .left_box_left = 256, .left_box_right = 288,
            .right_child = 32851, .left_child = 32852
            },
     {
            .split_x = 320, .split_y = -3200,
            .split_dx = -32, .split_dy = 0,
            .right_box_top = -3136, .right_box_bottom = -3200,
            .right_box_left = 256, .right_box_right = 320,
            .left_box_top = -3200, .left_box_bottom = -3264,
            .left_box_left = 256, .left_box_right = 320,
            .right_child = 32850, .left_child = 77
            },
     {
            .split_x = 256, .split_y = -3264,
            .split_dx = 32, .split_dy = 0,
            .right_box_top = -3264, .right_box_bottom = -3328,
            .right_box_left = 256, .right_box_right = 320,
            .left_box_top = -3136, .left_box_bottom = -3264,
            .left_box_left = 256, .left_box_right = 320,
            .right_child = 32849, .left_child = 78
            },
     {
            .split_x = 320, .split_y = -3264,
            .split_dx = 0, .split_dy = 64,
            .right_box_top = -3136, .right_box_bottom = -3328,
            .right_box_left = 320, .right_box_right = 496,
            .left_box_top = -3136, .left_box_bottom = -3328,
            .left_box_left = 256, .left_box_right = 320,
            .right_child = 32848, .left_child = 79
            },
     {
            .split_x = 320, .split_y = -3392,
            .split_dx = 0, .split_dy = 64,
            .right_box_top = -3328, .right_box_bottom = -3392,
            .right_box_left = 320, .right_box_right = 496,
            .left_box_top = -3328, .left_box_bottom = -3392,
            .left_box_left = 256, .left_box_right = 320,
            .right_child = 32854, .left_child = 32855
            },
     {
            .split_x = 256, .split_y = -3392,
            .split_dx = 64, .split_dy = 0,
            .right_box_top = -3392, .right_box_bottom = -3456,
            .right_box_left = 256, .right_box_right = 472,
            .left_box_top = -3328, .left_box_bottom = -3392,
            .left_box_left = 256, .left_box_right = 496,
            .right_child = 32853, .left_child = 81
            },
     {
            .split_x = 320, .split_y = -3328,
            .split_dx = -64, .split_dy = 0,
            .right_box_top = -3136, .right_box_bottom = -3328,
            .right_box_left = 256, .right_box_right = 496,
            .left_box_top = -3328, .left_box_bottom = -3456,
            .left_box_left = 256, .left_box_right = 496,
            .right_child = 80, .left_child = 82
            },
     {
            .split_x = 320, .split_y = -3072,
            .split_dx = -64, .split_dy = 0,
            .right_box_top = -3008, .right_box_bottom = -3072,
            .right_box_left = 256, .right_box_right = 320,
            .left_box_top = -3072, .left_box_bottom = -3136,
            .left_box_left = 256, .left_box_right = 320,
            .right_child = 32857, .left_child = 32858
            },
     {
            .split_x = 320, .split_y = -3136,
            .split_dx = 0, .split_dy = 64,
            .right_box_top = -3008, .right_box_bottom = -3136,
            .right_box_left = 320, .right_box_right = 496,
            .left_box_top = -3008, .left_box_bottom = -3136,
            .left_box_left = 256, .left_box_right = 320,
            .right_child = 32856, .left_child = 84
            },
     {
            .split_x = 256, .split_y = -3136,
            .split_dx = 64, .split_dy = 0,
            .right_box_top = -3136, .right_box_bottom = -3456,
            .right_box_left = 256, .right_box_right = 496,
            .left_box_top = -3008, .left_box_bottom = -3136,
            .left_box_left = 256, .left_box_right = 496,
            .right_child = 83, .left_child = 85
            },
     {
            .split_x = 496, .split_y = -3304,
            .split_dx = 0, .split_dy = 144,
            .right_box_top = -3104, .right_box_bottom = -3360,
            .right_box_left = 496, .right_box_right = 704,
            .left_box_top = -3008, .left_box_bottom = -3456,
            .left_box_left = 256, .left_box_right = 496,
            .right_child = 76, .left_child = 86
            },
     {
            .split_x = 704, .split_y = -3104,
            .split_dx = 0, .split_dy = 160,
            .right_box_top = -2486, .right_box_bottom = -3680,
            .right_box_left = 704, .right_box_right = 1344,
            .left_box_top = -3008, .left_box_bottom = -3456,
            .left_box_left = 256, .left_box_right = 704,
            .right_child = 75, .left_child = 87
            },
     {
            .split_x = 224, .split_y = -3264,
            .split_dx = 0, .split_dy = 64,
            .right_box_top = -3200, .right_box_bottom = -3264,
            .right_box_left = 224, .right_box_right = 256,
            .left_box_top = -3200, .left_box_bottom = -3264,
            .left_box_left = 192, .left_box_right = 224,
            .right_child = 32861, .left_child = 32862
            },
     {
            .split_x = 192, .split_y = -3264,
            .split_dx = 0, .split_dy = 64,
            .right_box_top = -3200, .right_box_bottom = -3264,
            .right_box_left = 192, .right_box_right = 256,
            .left_box_top = -3200, .left_box_bottom = -3264,
            .left_box_left = 160, .left_box_right = 192,
            .right_child = 89, .left_child = 32863
            },
     {
            .split_x = 256, .split_y = -3200,
            .split_dx = -32, .split_dy = 0,
            .right_box_top = -3008, .right_box_bottom = -3200,
            .right_box_left = 160, .right_box_right = 256,
            .left_box_top = -3200, .left_box_bottom = -3264,
            .left_box_left = 160, .left_box_right = 256,
            .right_child = 32860, .left_child = 90
            },
     {
            .split_x = 160, .split_y = -3264,
            .split_dx = 32, .split_dy = 0,
            .right_box_top = -3264, .right_box_bottom = -3456,
            .right_box_left = 160, .right_box_right = 256,
            .left_box_top = -3008, .left_box_bottom = -3264,
            .left_box_left = 160, .left_box_right = 256,
            .right_child = 32859, .left_child = 91
            },
     {
            .split_x = -128, .split_y = -3120,
            .split_dx = -128, .split_dy = 0,
            .right_box_top = -2851, .right_box_bottom = -3120,
            .right_box_left = -256, .right_box_right = -128,
            .left_box_top = -3120, .left_box_bottom = -3136,
            .left_box_left = -256, .left_box_right = -128,
            .right_child = 32867, .left_child = 32868
            },
     {
            .split_x = -256, .split_y = -3136,
            .split_dx = 128, .split_dy = 0,
            .right_box_top = -3136, .right_box_bottom = -3216,
            .right_box_left = -256, .right_box_right = -128,
            .left_box_top = -2851, .left_box_bottom = -3136,
            .left_box_left = -256, .left_box_right = -128,
            .right_child = 32866, .left_child = 93
            },
     {
            .split_x = -256, .split_y = -3248,
            .split_dx = 16, .split_dy = -16,
            .right_box_top = -3248, .right_box_bottom = -3264,
            .right_box_left = -256, .right_box_right = -240,
            .left_box_top = -3200, .left_box_bottom = -3264,
            .left_box_left = -256, .left_box_right = -192,
            .right_child = 32874, .left_child = 32875
            },
     {
            .split_x = -240, .split_y = -3200,
            .split_dx = -16, .split_dy = -16,
            .right_box_top = -3200, .right_box_bottom = -3216,
            .right_box_left = -256, .right_box_right = -240,
            .left_box_top = -3200, .left_box_bottom = -3264,
            .left_box_left = -256, .left_box_right = -192,
            .right_child = 32873, .left_child = 95
            },
     {
            .split_x = -208, .split_y = -3200,
            .split_dx = -32, .split_dy = 0,
            .right_box_top = -3200, .right_box_bottom = -3200,
            .right_box_left = -240, .right_box_right = -208,
            .left_box_top = -3200, .left_box_bottom = -3264,
            .left_box_left = -256, .left_box_right = -192,
            .right_child = 32872, .left_child = 96
            },
     {
            .split_x = -192, .split_y = -3248,
            .split_dx = 0, .split_dy = 32,
            .right_box_top = -3216, .right_box_bottom = -3248,
            .right_box_left = -192, .right_box_right = -192,
            .left_box_top = -3200, .left_box_bottom = -3264,
            .left_box_left = -256, .left_box_right = -192,
            .right_child = 32871, .left_child = 97
            },
     {
            .split_x = -208, .split_y = -3264,
            .split_dx = 16, .split_dy = 16,
            .right_box_top = -3248, .right_box_bottom = -3264,
            .right_box_left = -208, .right_box_right = -192,
            .left_box_top = -3200, .left_box_bottom = -3264,
            .left_box_left = -256, .left_box_right = -192,
            .right_child = 32870, .left_child = 98
            },
     {
            .split_x = -240, .split_y = -3264,
            .split_dx = 32, .split_dy = 0,
            .right_box_top = -3264, .right_box_bottom = -3264,
            .right_box_left = -240, .right_box_right = -208,
            .left_box_top = -3200, .left_box_bottom = -3264,
            .left_box_left = -256, .left_box_right = -192,
            .right_child = 32869, .left_child = 99
            },
     {
            .split_x = -192, .split_y = -3216,
            .split_dx = -16, .split_dy = 16,
            .right_box_top = -2851, .right_box_bottom = -3216,
            .right_box_left = -256, .right_box_right = -128,
            .left_box_top = -3200, .left_box_bottom = -3264,
            .left_box_left = -256, .left_box_right = -192,
            .right_child = 94, .left_child = 100
            },
     {
            .split_x = -336, .split_y = -3168,
            .split_dx = 0, .split_dy = -128,
            .right_box_top = -3168, .right_box_bottom = -3296,
            .right_box_left = -640, .right_box_right = -336,
            .left_box_top = -3168, .left_box_bottom = -3296,
            .left_box_left = -336, .left_box_right = -320,
            .right_child = 32877, .left_child = 32878
            },
     {
            .split_x = -320, .split_y = -3296,
            .split_dx = 0, .split_dy = 128,
            .right_box_top = -3168, .right_box_bottom = -3296,
            .right_box_left = -320, .right_box_right = -256,
            .left_box_top = -3168, .left_box_bottom = -3296,
            .left_box_left = -640, .left_box_right = -320,
            .right_child = 32876, .left_child = 102
            },
     {
            .split_x = -256, .split_y = -3120,
            .split_dx = -80, .split_dy = 0,
            .right_box_top = -2874, .right_box_bottom = -3120,
            .right_box_left = -640, .right_box_right = -256,
            .left_box_top = -3120, .left_box_bottom = -3168,
            .left_box_left = -640, .left_box_right = -336,
            .right_child = 32880, .left_child = 32881
            },
     {
            .split_x = -320, .split_y = -3168,
            .split_dx = 64, .split_dy = 32,
            .right_box_top = -3136, .right_box_bottom = -3168,
            .right_box_left = -320, .right_box_right = -256,
            .left_box_top = -2874, .left_box_bottom = -3168,
            .left_box_left = -640, .left_box_right = -256,
            .right_child = 32879, .left_child = 104
            },
     {
            .split_x = -336, .split_y = -3168,
            .split_dx = 16, .split_dy = 0,
            .right_box_top = -3168, .right_box_bottom = -3296,
            .right_box_left = -640, .right_box_right = -256,
            .left_box_top = -2874, .left_box_bottom = -3168,
            .left_box_left = -640, .left_box_right = -256,
            .right_child = 103, .left_child = 105
            },
     {
            .split_x = -256, .split_y = -3136,
            .split_dx = 0, .split_dy = 16,
            .right_box_top = -2851, .right_box_bottom = -3264,
            .right_box_left = -256, .right_box_right = -128,
            .left_box_top = -2874, .left_box_bottom = -3296,
            .left_box_left = -640, .left_box_right = -256,
            .right_child = 101, .left_child = 106
            },
     {
            .split_x = -128, .split_y = -3328,
            .split_dx = -128, .split_dy = 0,
            .right_box_top = -3328, .right_box_bottom = -3328,
            .right_box_left = -256, .right_box_right = -128,
            .left_box_top = -3328, .left_box_bottom = -3344,
            .left_box_left = -256, .left_box_right = -128,
            .right_child = 32883, .left_child = 32884
            },
     {
            .split_x = -256, .split_y = -3344,
            .split_dx = 128, .split_dy = 0,
            .right_box_top = -3344, .right_box_bottom = -3613,
            .right_box_left = -256, .right_box_right = -128,
            .left_box_top = -3328, .left_box_bottom = -3344,
            .left_box_left = -256, .left_box_right = -128,
            .right_child = 32882, .left_child = 108
            },
     {
            .split_x = -336, .split_y = -3296,
            .split_dx = 0, .split_dy = -48,
            .right_box_top = -3296, .right_box_bottom = -3575,
            .right_box_left = -640, .right_box_right = -336,
            .left_box_top = -3344, .left_box_bottom = -3589,
            .left_box_left = -336, .left_box_right = -256,
            .right_child = 32886, .left_child = 32887
            },
     {
            .split_x = -256, .split_y = -3328,
            .split_dx = -64, .split_dy = 32,
            .right_box_top = -3296, .right_box_bottom = -3328,
            .right_box_left = -320, .right_box_right = -256,
            .left_box_top = -3296, .left_box_bottom = -3589,
            .left_box_left = -640, .left_box_right = -256,
            .right_child = 32885, .left_child = 110
            },
     {
            .split_x = -256, .split_y = -3344,
            .split_dx = 0, .split_dy = 16,
            .right_box_top = -3328, .right_box_bottom = -3613,
            .right_box_left = -256, .right_box_right = -128,
            .left_box_top = -3296, .left_box_bottom = -3589,
            .left_box_left = -640, .left_box_right = -256,
            .right_child = 109, .left_child = 111
            },
     {
            .split_x = -320, .split_y = -3296,
            .split_dx = -16, .split_dy = 0,
            .right_box_top = -2851, .right_box_bottom = -3296,
            .right_box_left = -640, .right_box_right = -128,
            .left_box_top = -3296, .left_box_bottom = -3613,
            .left_box_left = -640, .left_box_right = -128,
            .right_child = 107, .left_child = 112
            },
     {
            .split_x = -64, .split_y = -3136,
            .split_dx = 0, .split_dy = -192,
            .right_box_top = -3136, .right_box_bottom = -3328,
            .right_box_left = -64, .right_box_right = -64,
            .left_box_top = -3072, .left_box_bottom = -3392,
            .left_box_left = -64, .left_box_right = 64,
            .right_child = 32888, .left_child = 32889
            },
     {
            .split_x = -96, .split_y = -3120,
            .split_dx = -32, .split_dy = 0,
            .right_box_top = -2816, .right_box_bottom = -3120,
            .right_box_left = -128, .right_box_right = 64,
            .left_box_top = -3136, .left_box_bottom = -3136,
            .left_box_left = -128, .left_box_right = -64,
            .right_child = 32890, .left_child = 32891
            },
     {
            .split_x = -64, .split_y = -3136,
            .split_dx = 112, .split_dy = 64,
            .right_box_top = -3072, .right_box_bottom = -3392,
            .right_box_left = -64, .right_box_right = 64,
            .left_box_top = -2816, .left_box_bottom = -3136,
            .left_box_left = -128, .left_box_right = 64,
            .right_child = 114, .left_child = 115
            },
     {
            .split_x = -128, .split_y = -3344,
            .split_dx = 32, .split_dy = 0,
            .right_box_top = -3344, .right_box_bottom = -3648,
            .right_box_left = -128, .right_box_right = 64,
            .left_box_top = -3328, .left_box_bottom = -3328,
            .left_box_left = -128, .left_box_right = -64,
            .right_child = 32892, .left_child = 32893
            },
     {
            .split_x = 48, .split_y = -3392,
            .split_dx = -112, .split_dy = 64,
            .right_box_top = -2816, .right_box_bottom = -3392,
            .right_box_left = -128, .right_box_right = 64,
            .left_box_top = -3328, .left_box_bottom = -3648,
            .left_box_left = -128, .left_box_right = 64,
            .right_child = 116, .left_child = 117
            },
     {
            .split_x = -128, .split_y = -3120,
            .split_dx = 0, .split_dy = -16,
            .right_box_top = -2851, .right_box_bottom = -3613,
            .right_box_left = -640, .right_box_right = -128,
            .left_box_top = -2816, .left_box_bottom = -3648,
            .left_box_left = -128, .left_box_right = 64,
            .right_child = 113, .left_child = 118
            },
     {
            .split_x = 64, .split_y = -2816,
            .split_dx = -704, .split_dy = -128,
            .right_box_top = -2816, .right_box_bottom = -2944,
            .right_box_left = -640, .right_box_right = 64,
            .left_box_top = -2816, .left_box_bottom = -3648,
            .left_box_left = -640, .left_box_right = 64,
            .right_child = 32865, .left_child = 119
            },
     {
            .split_x = -640, .split_y = -3520,
            .split_dx = 0, .split_dy = 576,
            .right_box_top = -2816, .right_box_bottom = -3648,
            .right_box_left = -640, .right_box_right = 64,
            .left_box_top = -2816, .left_box_bottom = -3520,
            .left_box_left = -768, .left_box_right = -640,
            .right_child = 120, .left_child = 32894
            },
     {
            .split_x = 160, .split_y = -3200,
            .split_dx = -32, .split_dy = 0,
            .right_box_top = -3008, .right_box_bottom = -3200,
            .right_box_left = 128, .right_box_right = 160,
            .left_box_top = -3200, .left_box_bottom = -3264,
            .left_box_left = 128, .left_box_right = 160,
            .right_child = 32896, .left_child = 32897
            },
     {
            .split_x = 128, .split_y = -3264,
            .split_dx = 32, .split_dy = 0,
            .right_box_top = -3264, .right_box_bottom = -3456,
            .right_box_left = 128, .right_box_right = 160,
            .left_box_top = -3008, .left_box_bottom = -3264,
            .left_box_left = 128, .left_box_right = 160,
            .right_child = 32895, .left_child = 122
            },
     {
            .split_x = 64, .split_y = -3392,
            .split_dx = 64, .split_dy = 128,
            .right_box_top = -3264, .right_box_bottom = -3456,
            .right_box_left = 64, .right_box_right = 128,
            .left_box_top = -3072, .left_box_bottom = -3392,
            .left_box_left = 64, .left_box_right = 128,
            .right_child = 32899, .left_child = 32900
            },
     {
            .split_x = 128, .split_y = -3200,
            .split_dx = -64, .split_dy = 128,
            .right_box_top = -3008, .right_box_bottom = -3200,
            .right_box_left = 64, .right_box_right = 128,
            .left_box_top = -3072, .left_box_bottom = -3456,
            .left_box_left = 64, .left_box_right = 128,
            .right_child = 32898, .left_child = 124
            },
     {
            .split_x = 128, .split_y = -3264,
            .split_dx = 0, .split_dy = 64,
            .right_box_top = -3008, .right_box_bottom = -3456,
            .right_box_left = 128, .right_box_right = 160,
            .left_box_top = -3008, .left_box_bottom = -3456,
            .left_box_left = 64, .left_box_right = 128,
            .right_child = 123, .left_child = 125
            },
     {
            .split_x = 64, .split_y = -3520,
            .split_dx = 0, .split_dy = -128,
            .right_box_top = -2816, .right_box_bottom = -3648,
            .right_box_left = -768, .right_box_right = 64,
            .left_box_top = -3008, .left_box_bottom = -3456,
            .left_box_left = 64, .left_box_right = 160,
            .right_child = 121, .left_child = 126
            },
     {
            .split_x = -640, .split_y = -3520,
            .split_dx = 704, .split_dy = -128,
            .right_box_top = -3496, .right_box_bottom = -3648,
            .right_box_left = -768, .right_box_right = 64,
            .left_box_top = -2816, .left_box_bottom = -3648,
            .left_box_left = -768, .left_box_right = 160,
            .right_child = 32864, .left_child = 127
            },
     {
            .split_x = 160, .split_y = -3264,
            .split_dx = 0, .split_dy = 64,
            .right_box_top = -3008, .right_box_bottom = -3456,
            .right_box_left = 160, .right_box_right = 256,
            .left_box_top = -2816, .left_box_bottom = -3648,
            .left_box_left = -768, .left_box_right = 160,
            .right_child = 92, .left_child = 128
            },
     {
            .split_x = 256, .split_y = -3264,
            .split_dx = 0, .split_dy = 64,
            .right_box_top = -2486, .right_box_bottom = -3680,
            .right_box_left = 256, .right_box_right = 1344,
            .left_box_top = -2816, .left_box_bottom = -3648,
            .left_box_left = -768, .left_box_right = 256,
            .right_child = 88, .left_child = 129
            },
     {
            .split_x = 1344, .split_y = -3200,
            .split_dx = 0, .split_dy = 96,
            .right_box_top = -2048, .right_box_bottom = -4064,
            .right_box_left = 1344, .right_box_right = 2176,
            .left_box_top = -2486, .left_box_bottom = -3680,
            .left_box_left = -768, .left_box_right = 1344,
            .right_child = 55, .left_child = 130
            },
     {
            .split_x = 2752, .split_y = -2912,
            .split_dx = 128, .split_dy = 0,
            .right_box_top = -2912, .right_box_bottom = -3048,
            .right_box_left = 2752, .right_box_right = 2960,
            .left_box_top = -2880, .left_box_bottom = -2912,
            .left_box_left = 2880, .left_box_right = 2880,
            .right_child = 32902, .left_child = 32903
            },
     {
            .split_x = 2880, .split_y = -2880,
            .split_dx = -128, .split_dy = 80,
            .right_box_top = -2656, .right_box_bottom = -2930,
            .right_box_left = 2752, .right_box_right = 3048,
            .left_box_top = -2880, .left_box_bottom = -3048,
            .left_box_left = 2752, .left_box_right = 2960,
            .right_child = 32901, .left_child = 132
            },
     {
            .split_x = 2752, .split_y = -2784,
            .split_dx = 192, .split_dy = 128,
            .right_box_top = -2656, .right_box_bottom = -3048,
            .right_box_left = 2752, .right_box_right = 3048,
            .left_box_top = -2560, .left_box_bottom = -2784,
            .left_box_left = 2752, .left_box_right = 2944,
            .right_child = 133, .left_child = 32904
            },
     {
            .split_x = 2944, .split_y = -3648,
            .split_dx = -192, .split_dy = 64,
            .right_box_top = -3536, .right_box_bottom = -3648,
            .right_box_left = 2752, .right_box_right = 2944,
            .left_box_top = -3648, .left_box_bottom = -3669,
            .left_box_left = 2944, .left_box_right = 2944,
            .right_child = 32906, .left_child = 32907
            },
     {
            .split_x = 2944, .split_y = -3536,
            .split_dx = -192, .split_dy = -48,
            .right_box_top = -3360, .right_box_bottom = -3584,
            .right_box_left = 2752, .right_box_right = 2944,
            .left_box_top = -3536, .left_box_bottom = -3669,
            .left_box_left = 2752, .left_box_right = 2944,
            .right_child = 32905, .left_child = 135
            },
     {
            .split_x = 3352, .split_y = -3592,
            .split_dx = 0, .split_dy = 24,
            .right_box_top = -3568, .right_box_bottom = -3592,
            .right_box_left = 3352, .right_box_right = 3352,
            .left_box_top = -3592, .left_box_bottom = -3601,
            .left_box_left = 3352, .left_box_right = 3353,
            .right_child = 32910, .left_child = 32911
            },
     {
            .split_x = 3352, .split_y = -3568,
            .split_dx = -88, .split_dy = -48,
            .right_box_top = -3370, .right_box_bottom = -3616,
            .right_box_left = 3104, .right_box_right = 3352,
            .left_box_top = -3568, .left_box_bottom = -3601,
            .left_box_left = 3352, .left_box_right = 3353,
            .right_child = 32909, .left_child = 137
            },
     {
            .split_x = 3448, .split_y = -3520,
            .split_dx = -96, .split_dy = -48,
            .right_box_top = -3432, .right_box_bottom = -3568,
            .right_box_left = 3312, .right_box_right = 3454,
            .left_box_top = -3520, .left_box_bottom = -3568,
            .left_box_left = 3352, .left_box_right = 3448,
            .right_child = 32913, .left_child = 32914
            },
     {
            .split_x = 3408, .split_y = -3432,
            .split_dx = -96, .split_dy = -64,
            .right_box_top = -3320, .right_box_bottom = -3496,
            .right_box_left = 3242, .right_box_right = 3408,
            .left_box_top = -3432, .left_box_bottom = -3568,
            .left_box_left = 3312, .left_box_right = 3454,
            .right_child = 32912, .left_child = 139
            },
     {
            .split_x = 3312, .split_y = -3496,
            .split_dx = 40, .split_dy = -72,
            .right_box_top = -3370, .right_box_bottom = -3616,
            .right_box_left = 3104, .right_box_right = 3353,
            .left_box_top = -3320, .left_box_bottom = -3568,
            .left_box_left = 3242, .left_box_right = 3454,
            .right_child = 138, .left_child = 140
            },
     {
            .split_x = 3454, .split_y = -3496,
            .split_dx = -6, .split_dy = -23,
            .right_box_top = -3320, .right_box_bottom = -3616,
            .right_box_left = 3104, .right_box_right = 3454,
            .left_box_top = -3520, .left_box_bottom = -3574,
            .left_box_left = 3448, .left_box_right = 3512,
            .right_child = 141, .left_child = 32915
            },
     {
            .split_x = 3264, .split_y = -3616,
            .split_dx = -160, .split_dy = 64,
            .right_box_top = -3320, .right_box_bottom = -3616,
            .right_box_left = 3104, .right_box_right = 3512,
            .left_box_top = -3552, .left_box_bottom = -3648,
            .left_box_left = 3047, .left_box_right = 3264,
            .right_child = 142, .left_child = 32916
            },
     {
            .split_x = 2752, .split_y = -3048,
            .split_dx = 224, .split_dy = -24,
            .right_box_top = -3048, .right_box_bottom = -3296,
            .right_box_left = 2752, .right_box_right = 2976,
            .left_box_top = -2902, .left_box_bottom = -3072,
            .left_box_left = 2752, .left_box_right = 3008,
            .right_child = 32917, .left_child = 32918
            },
     {
            .split_x = 3112, .split_y = -3360,
            .split_dx = -168, .split_dy = -176,
            .right_box_top = -3232, .right_box_bottom = -3536,
            .right_box_left = 2752, .right_box_right = 3112,
            .left_box_top = -3360, .left_box_bottom = -3626,
            .left_box_left = 2944, .left_box_right = 3215,
            .right_child = 32919, .left_child = 32920
            },
     {
            .split_x = 2984, .split_y = -3200,
            .split_dx = 152, .split_dy = 128,
            .right_box_top = -3072, .right_box_bottom = -3320,
            .right_box_left = 2984, .right_box_right = 3280,
            .left_box_top = -3072, .left_box_bottom = -3200,
            .left_box_left = 2887, .left_box_right = 3136,
            .right_child = 32921, .left_child = 32922
            },
     {
            .split_x = 3280, .split_y = -3320,
            .split_dx = -296, .split_dy = 120,
            .right_box_top = -3072, .right_box_bottom = -3320,
            .right_box_left = 2887, .right_box_right = 3280,
            .left_box_top = -3160, .left_box_bottom = -3404,
            .left_box_left = 2816, .left_box_right = 3280,
            .right_child = 146, .left_child = 32923
            },
     {
            .split_x = 2816, .split_y = -3232,
            .split_dx = 296, .split_dy = -128,
            .right_box_top = -3232, .right_box_bottom = -3626,
            .right_box_left = 2752, .right_box_right = 3215,
            .left_box_top = -3072, .left_box_bottom = -3404,
            .left_box_left = 2816, .left_box_right = 3280,
            .right_child = 145, .left_child = 147
            },
     {
            .split_x = 2976, .split_y = -3072,
            .split_dx = -160, .split_dy = -160,
            .right_box_top = -2902, .right_box_bottom = -3296,
            .right_box_left = 2752, .right_box_right = 3008,
            .left_box_top = -3072, .left_box_bottom = -3626,
            .left_box_left = 2752, .left_box_right = 3280,
            .right_child = 144, .left_child = 148
            },
     {
            .split_x = 3104, .split_y = -3552,
            .split_dx = 176, .split_dy = 232,
            .right_box_top = -3320, .right_box_bottom = -3648,
            .right_box_left = 3047, .right_box_right = 3512,
            .left_box_top = -2902, .left_box_bottom = -3626,
            .left_box_left = 2752, .left_box_right = 3280,
            .right_child = 143, .left_child = 149
            },
     {
            .split_x = 3304, .split_y = -3040,
            .split_dx = 96, .split_dy = -112,
            .right_box_top = -3040, .right_box_bottom = -3432,
            .right_box_left = 3136, .right_box_right = 3472,
            .left_box_top = -3040, .left_box_bottom = -3152,
            .left_box_left = 3304, .left_box_right = 3400,
            .right_child = 32924, .left_child = 32925
            },
     {
            .split_x = 3472, .split_y = -3432,
            .split_dx = -64, .split_dy = 0,
            .right_box_top = -3040, .right_box_bottom = -3432,
            .right_box_left = 3136, .right_box_right = 3472,
            .left_box_top = -3432, .left_box_bottom = -3496,
            .left_box_left = 3408, .left_box_right = 3472,
            .right_child = 151, .left_child = 32926
            },
     {
            .split_x = 3136, .split_y = -3072,
            .split_dx = 168, .split_dy = 32,
            .right_box_top = -3040, .right_box_bottom = -3496,
            .right_box_left = 3136, .right_box_right = 3472,
            .left_box_top = -2944, .left_box_bottom = -3072,
            .left_box_left = 3048, .left_box_right = 3304,
            .right_child = 152, .left_child = 32927
            },
     {
            .split_x = 3304, .split_y = -3040,
            .split_dx = 16, .split_dy = 0,
            .right_box_top = -3040, .right_box_bottom = -3040,
            .right_box_left = 3304, .right_box_right = 3320,
            .left_box_top = -2939, .left_box_bottom = -3040,
            .left_box_left = 3320, .left_box_right = 3345,
            .right_child = 32929, .left_child = 32930
            },
     {
            .split_x = 3048, .split_y = -2880,
            .split_dx = 0, .split_dy = -64,
            .right_box_top = -2880, .right_box_bottom = -2944,
            .right_box_left = 3008, .right_box_right = 3048,
            .left_box_top = -2939, .left_box_bottom = -3040,
            .left_box_left = 3304, .left_box_right = 3345,
            .right_child = 32928, .left_child = 154
            },
     {
            .split_x = 3048, .split_y = -2944,
            .split_dx = 256, .split_dy = -96,
            .right_box_top = -2944, .right_box_bottom = -3496,
            .right_box_left = 3048, .right_box_right = 3472,
            .left_box_top = -2880, .left_box_bottom = -3040,
            .left_box_left = 3008, .left_box_right = 3345,
            .right_child = 153, .left_child = 155
            },
     {
            .split_x = 3680, .split_y = -3392,
            .split_dx = 0, .split_dy = -154,
            .right_box_top = -3264, .right_box_bottom = -3567,
            .right_box_left = 3552, .right_box_right = 3680,
            .left_box_top = -3144, .left_box_bottom = -3392,
            .left_box_left = 3680, .left_box_right = 3808,
            .right_child = 32931, .left_child = 32932
            },
     {
            .split_x = 3648, .split_y = -3264,
            .split_dx = -152, .split_dy = 232,
            .right_box_top = -3032, .right_box_bottom = -3264,
            .right_box_left = 3496, .right_box_right = 3738,
            .left_box_top = -3032, .left_box_bottom = -3152,
            .left_box_left = 3400, .left_box_right = 3456,
            .right_child = 32934, .left_child = 32935
            },
     {
            .split_x = 3496, .split_y = -3032,
            .split_dx = -40, .split_dy = 0,
            .right_box_top = -2880, .right_box_bottom = -3032,
            .right_box_left = 3345, .right_box_right = 3672,
            .left_box_top = -3032, .left_box_bottom = -3264,
            .left_box_left = 3400, .left_box_right = 3738,
            .right_child = 32933, .left_child = 158
            },
     {
            .split_x = 3552, .split_y = -3392,
            .split_dx = 96, .split_dy = 128,
            .right_box_top = -3144, .right_box_bottom = -3567,
            .right_box_left = 3552, .right_box_right = 3808,
            .left_box_top = -2880, .left_box_bottom = -3264,
            .left_box_left = 3345, .left_box_right = 3738,
            .right_child = 157, .left_child = 159
            },
     {
            .split_x = 3400, .split_y = -3152,
            .split_dx = 72, .split_dy = -280,
            .right_box_top = -2880, .right_box_bottom = -3496,
            .right_box_left = 3008, .right_box_right = 3472,
            .left_box_top = -2880, .left_box_bottom = -3567,
            .left_box_left = 3345, .left_box_right = 3808,
            .right_child = 156, .left_child = 160
            },
     {
            .split_x = 3472, .split_y = -3520,
            .split_dx = 40, .split_dy = -54,
            .right_box_top = -2902, .right_box_bottom = -3648,
            .right_box_left = 2752, .right_box_right = 3512,
            .left_box_top = -2880, .left_box_bottom = -3567,
            .left_box_left = 3008, .left_box_right = 3808,
            .right_child = 150, .left_child = 161
            },
     {
            .split_x = 2944, .split_y = -3536,
            .split_dx = 128, .split_dy = -112,
            .right_box_top = -3536, .right_box_bottom = -3648,
            .right_box_left = 2944, .right_box_right = 3072,
            .left_box_top = -2880, .left_box_bottom = -3648,
            .left_box_left = 2752, .left_box_right = 3808,
            .right_child = 32908, .left_child = 162
            },
     {
            .split_x = 2752, .split_y = -3360,
            .split_dx = 192, .split_dy = -176,
            .right_box_top = -3360, .right_box_bottom = -3669,
            .right_box_left = 2752, .right_box_right = 2944,
            .left_box_top = -2880, .left_box_bottom = -3648,
            .left_box_left = 2752, .left_box_right = 3808,
            .right_child = 136, .left_child = 163
            },
     {
            .split_x = 3048, .split_y = -2880,
            .split_dx = -296, .split_dy = -168,
            .right_box_top = -2560, .right_box_bottom = -3048,
            .right_box_left = 2752, .right_box_right = 3048,
            .left_box_top = -2880, .left_box_bottom = -3669,
            .left_box_left = 2752, .left_box_right = 3808,
            .right_child = 134, .left_child = 164
            },
     {
            .split_x = 2208, .split_y = -2304,
            .split_dx = 0, .split_dy = -256,
            .right_box_top = -2304, .right_box_bottom = -2560,
            .right_box_left = 2176, .right_box_right = 2208,
            .left_box_top = -2304, .left_box_bottom = -2560,
            .left_box_left = 2208, .left_box_right = 2304,
            .right_child = 32938, .left_child = 32939
            },
     {
            .split_x = 2336, .split_y = -2272,
            .split_dx = -160, .split_dy = 0,
            .right_box_top = -2112, .right_box_bottom = -2272,
            .right_box_left = 2176, .right_box_right = 2336,
            .left_box_top = -2304, .left_box_bottom = -2560,
            .left_box_left = 2176, .left_box_right = 2304,
            .right_child = 32937, .left_child = 166
            },
     {
            .split_x = 2336, .split_y = -2560,
            .split_dx = 0, .split_dy = 288,
            .right_box_top = -2112, .right_box_bottom = -2560,
            .right_box_left = 2336, .right_box_right = 2496,
            .left_box_top = -2112, .left_box_bottom = -2560,
            .left_box_left = 2176, .left_box_right = 2336,
            .right_child = 32936, .left_child = 167
            },
     {
            .split_x = 2336, .split_y = -2592,
            .split_dx = 0, .split_dy = 32,
            .right_box_top = -2560, .right_box_bottom = -2752,
            .right_box_left = 2336, .right_box_right = 2496,
            .left_box_top = -2592, .left_box_bottom = -2752,
            .left_box_left = 2176, .left_box_right = 2336,
            .right_child = 32940, .left_child = 32941
            },
     {
            .split_x = 2304, .split_y = -2560,
            .split_dx = -96, .split_dy = 0,
            .right_box_top = -2112, .right_box_bottom = -2560,
            .right_box_left = 2176, .right_box_right = 2496,
            .left_box_top = -2560, .left_box_bottom = -2752,
            .left_box_left = 2176, .left_box_right = 2496,
            .right_child = 168, .left_child = 169
            },
     {
            .split_x = 2176, .split_y = -2112,
            .split_dx = 320, .split_dy = 0,
            .right_box_top = -2112, .right_box_bottom = -2752,
            .right_box_left = 2176, .right_box_right = 2496,
            .left_box_top = -2048, .left_box_bottom = -2112,
            .left_box_left = 2176, .left_box_right = 2496,
            .right_child = 170, .left_child = 32942
            },
     {
            .split_x = 2624, .split_y = -2784,
            .split_dx = -104, .split_dy = 96,
            .right_box_top = -2560, .right_box_bottom = -2784,
            .right_box_left = 2496, .right_box_right = 2752,
            .left_box_top = -2665, .left_box_bottom = -2688,
            .left_box_left = 2496, .left_box_right = 2520,
            .right_child = 32944, .left_child = 32945
            },
     {
            .split_x = 2624, .split_y = -2784,
            .split_dx = 128, .split_dy = 224,
            .right_box_top = -2560, .right_box_bottom = -2784,
            .right_box_left = 2624, .right_box_right = 2752,
            .left_box_top = -2560, .left_box_bottom = -2784,
            .left_box_left = 2496, .left_box_right = 2752,
            .right_child = 32943, .left_child = 172
            },
     {
            .split_x = 2520, .split_y = -2560,
            .split_dx = 232, .split_dy = 0,
            .right_box_top = -2560, .right_box_bottom = -2784,
            .right_box_left = 2496, .right_box_right = 2752,
            .left_box_top = -2048, .left_box_bottom = -2496,
            .left_box_left = 2496, .left_box_right = 2560,
            .right_child = 173, .left_child = 32946
            },
     {
            .split_x = 2496, .split_y = -2112,
            .split_dx = 0, .split_dy = -384,
            .right_box_top = -2048, .right_box_bottom = -2752,
            .right_box_left = 2176, .right_box_right = 2496,
            .left_box_top = -2048, .left_box_bottom = -2784,
            .left_box_left = 2496, .left_box_right = 2752,
            .right_child = 171, .left_child = 174
            },
     {
            .split_x = 2432, .split_y = -3112,
            .split_dx = 304, .split_dy = 0,
            .right_box_top = -3112, .right_box_bottom = -3360,
            .right_box_left = 2432, .right_box_right = 2736,
            .left_box_top = -3008, .left_box_bottom = -3112,
            .left_box_left = 2272, .left_box_right = 2432,
            .right_child = 32948, .left_child = 32949
            },
     {
            .split_x = 2240, .split_y = -2920,
            .split_dx = 32, .split_dy = -88,
            .right_box_top = -2920, .right_box_bottom = -3008,
            .right_box_left = 2176, .right_box_right = 2272,
            .left_box_top = -3008, .left_box_bottom = -3360,
            .left_box_left = 2272, .left_box_right = 2736,
            .right_child = 32947, .left_child = 176
            },
     {
            .split_x = 2736, .split_y = -3112,
            .split_dx = 0, .split_dy = -248,
            .right_box_top = -2920, .right_box_bottom = -3360,
            .right_box_left = 2176, .right_box_right = 2736,
            .left_box_top = -3112, .left_box_bottom = -3360,
            .left_box_left = 2736, .left_box_right = 2752,
            .right_child = 177, .left_child = 32950
            },
     {
            .split_x = 2736, .split_y = -3648,
            .split_dx = -496, .split_dy = 0,
            .right_box_top = -3360, .right_box_bottom = -3648,
            .right_box_left = 2240, .right_box_right = 2736,
            .left_box_top = -3648, .left_box_bottom = -3776,
            .left_box_left = 2240, .left_box_right = 2736,
            .right_child = 32951, .left_child = 32952
            },
     {
            .split_x = 2240, .split_y = -3776,
            .split_dx = -32, .split_dy = 96,
            .right_box_top = -3648, .right_box_bottom = -3776,
            .right_box_left = 2197, .right_box_right = 2240,
            .left_box_top = -3648, .left_box_bottom = -3680,
            .left_box_left = 2176, .left_box_right = 2208,
            .right_child = 32954, .left_child = 32955
            },
     {
            .split_x = 2240, .split_y = -3648,
            .split_dx = -64, .split_dy = 0,
            .right_box_top = -3648, .right_box_bottom = -3648,
            .right_box_left = 2176, .right_box_right = 2240,
            .left_box_top = -3648, .left_box_bottom = -3776,
            .left_box_left = 2176, .left_box_right = 2240,
            .right_child = 32953, .left_child = 180
            },
     {
            .split_x = 2240, .split_y = -3776,
            .split_dx = 0, .split_dy = 128,
            .right_box_top = -3360, .right_box_bottom = -3776,
            .right_box_left = 2240, .right_box_right = 2736,
            .left_box_top = -3648, .left_box_bottom = -3776,
            .left_box_left = 2176, .left_box_right = 2240,
            .right_child = 179, .left_child = 181
            },
     {
            .split_x = 2752, .split_y = -3360,
            .split_dx = -16, .split_dy = 0,
            .right_box_top = -2920, .right_box_bottom = -3360,
            .right_box_left = 2176, .right_box_right = 2752,
            .left_box_top = -3360, .left_box_bottom = -3776,
            .left_box_left = 2176, .left_box_right = 2736,
            .right_child = 178, .left_child = 182
            },
     {
            .split_x = 2752, .split_y = -2784,
            .split_dx = -128, .split_dy = 0,
            .right_box_top = -2048, .right_box_bottom = -2784,
            .right_box_left = 2176, .right_box_right = 2752,
            .left_box_top = -2920, .left_box_bottom = -3776,
            .left_box_left = 2176, .left_box_right = 2752,
            .right_child = 175, .left_child = 183
            },
     {
            .split_x = 2752, .split_y = -3584,
            .split_dx = 0, .split_dy = 224,
            .right_box_top = -2560, .right_box_bottom = -3669,
            .right_box_left = 2752, .right_box_right = 3808,
            .left_box_top = -2048, .left_box_bottom = -3776,
            .left_box_left = 2176, .left_box_right = 2752,
            .right_child = 165, .left_child = 184
            },
     {
            .split_x = 2856, .split_y = -4352,
            .split_dx = 0, .split_dy = 192,
            .right_box_top = -4160, .right_box_bottom = -4352,
            .right_box_left = 2856, .right_box_right = 2888,
            .left_box_top = -4160, .left_box_bottom = -4352,
            .left_box_left = 2688, .left_box_right = 2856,
            .right_child = 32957, .left_child = 32958
            },
     {
            .split_x = 2888, .split_y = -4352,
            .split_dx = 0, .split_dy = 32,
            .right_box_top = -4160, .right_box_bottom = -4352,
            .right_box_left = 2888, .right_box_right = 3104,
            .left_box_top = -4160, .left_box_bottom = -4352,
            .left_box_left = 2688, .left_box_right = 2888,
            .right_child = 32956, .left_child = 186
            },
     {
            .split_x = 2856, .split_y = -4352,
            .split_dx = 0, .split_dy = -32,
            .right_box_top = -4352, .right_box_bottom = -4384,
            .right_box_left = 2688, .right_box_right = 2856,
            .left_box_top = -4352, .left_box_bottom = -4384,
            .left_box_left = 2912, .left_box_right = 3104,
            .right_child = 32960, .left_child = 32961
            },
     {
            .split_x = 2856, .split_y = -4384,
            .split_dx = 56, .split_dy = 0,
            .right_box_top = -4384, .right_box_bottom = -4544,
            .right_box_left = 2688, .right_box_right = 3104,
            .left_box_top = -4352, .left_box_bottom = -4384,
            .left_box_left = 2688, .left_box_right = 3104,
            .right_child = 32959, .left_child = 188
            },
     {
            .split_x = 2912, .split_y = -4352,
            .split_dx = -24, .split_dy = 0,
            .right_box_top = -4160, .right_box_bottom = -4352,
            .right_box_left = 2688, .right_box_right = 3104,
            .left_box_top = -4352, .left_box_bottom = -4544,
            .left_box_left = 2688, .left_box_right = 3104,
            .right_child = 187, .left_child = 189
            },
     {
            .split_x = 3128, .split_y = -4320,
            .split_dx = 0, .split_dy = -32,
            .right_box_top = -4160, .right_box_bottom = -4352,
            .right_box_left = 3104, .right_box_right = 3128,
            .left_box_top = -4160, .left_box_bottom = -4352,
            .left_box_left = 3128, .left_box_right = 3160,
            .right_child = 32964, .left_child = 32965
            },
     {
            .split_x = 3104, .split_y = -4384,
            .split_dx = 56, .split_dy = 0,
            .right_box_top = -4384, .right_box_bottom = -4544,
            .right_box_left = 3104, .right_box_right = 3160,
            .left_box_top = -4160, .left_box_bottom = -4352,
            .left_box_left = 3104, .left_box_right = 3160,
            .right_child = 32963, .left_child = 191
            },
     {
            .split_x = 3160, .split_y = -4384,
            .split_dx = 0, .split_dy = 32,
            .right_box_top = -4160, .right_box_bottom = -4544,
            .right_box_left = 3160, .right_box_right = 3328,
            .left_box_top = -4160, .left_box_bottom = -4544,
            .left_box_left = 3104, .left_box_right = 3160,
            .right_child = 32962, .left_child = 192
            },
     {
            .split_x = 3104, .split_y = -4352,
            .split_dx = 0, .split_dy = -32,
            .right_box_top = -4160, .right_box_bottom = -4544,
            .right_box_left = 2688, .right_box_right = 3104,
            .left_box_top = -4160, .left_box_bottom = -4544,
            .left_box_left = 3104, .left_box_right = 3328,
            .right_child = 190, .left_child = 193
            },
     {
            .split_x = 2992, .split_y = -4592,
            .split_dx = 0, .split_dy = -8,
            .right_box_top = -4592, .right_box_bottom = -4600,
            .right_box_left = 2944, .right_box_right = 2992,
            .left_box_top = -4592, .left_box_bottom = -4600,
            .left_box_left = 2992, .left_box_right = 3024,
            .right_child = 32969, .left_child = 32970
            },
     {
            .split_x = 3024, .split_y = -4592,
            .split_dx = -32, .split_dy = 0,
            .right_box_top = -4544, .right_box_bottom = -4592,
            .right_box_left = 2944, .right_box_right = 3024,
            .left_box_top = -4592, .left_box_bottom = -4600,
            .left_box_left = 2944, .left_box_right = 3024,
            .right_child = 32968, .left_child = 195
            },
     {
            .split_x = 3024, .split_y = -4600,
            .split_dx = 0, .split_dy = 8,
            .right_box_top = -4544, .right_box_bottom = -4600,
            .right_box_left = 3024, .right_box_right = 3072,
            .left_box_top = -4544, .left_box_bottom = -4600,
            .left_box_left = 2944, .left_box_right = 3024,
            .right_child = 32967, .left_child = 196
            },
     {
            .split_x = 2992, .split_y = -4600,
            .split_dx = 32, .split_dy = 0,
            .right_box_top = -4600, .right_box_bottom = -4608,
            .right_box_left = 2944, .right_box_right = 3072,
            .left_box_top = -4544, .left_box_bottom = -4600,
            .left_box_left = 2944, .left_box_right = 3072,
            .right_child = 32966, .left_child = 197
            },
     {
            .split_x = 3040, .split_y = -4632,
            .split_dx = -64, .split_dy = 0,
            .right_box_top = -4608, .right_box_bottom = -4632,
            .right_box_left = 2976, .right_box_right = 3040,
            .left_box_top = -4632, .left_box_bottom = -4648,
            .left_box_left = 2976, .left_box_right = 3040,
            .right_child = 32971, .left_child = 32972
            },
     {
            .split_x = 3072, .split_y = -4608,
            .split_dx = -32, .split_dy = 0,
            .right_box_top = -4544, .right_box_bottom = -4608,
            .right_box_left = 2944, .right_box_right = 3072,
            .left_box_top = -4608, .left_box_bottom = -4648,
            .left_box_left = 2976, .left_box_right = 3040,
            .right_child = 198, .left_child = 199
            },
     {
            .split_x = 2992, .split_y = -4840,
            .split_dx = 0, .split_dy = -8,
            .right_box_top = -4840, .right_box_bottom = -4848,
            .right_box_left = 2912, .right_box_right = 2992,
            .left_box_top = -4840, .left_box_bottom = -4848,
            .left_box_left = 2992, .left_box_right = 3024,
            .right_child = 32976, .left_child = 32977
            },
     {
            .split_x = 2992, .split_y = -4848,
            .split_dx = 32, .split_dy = 0,
            .right_box_top = -4848, .right_box_bottom = -4864,
            .right_box_left = 2912, .right_box_right = 3024,
            .left_box_top = -4840, .left_box_bottom = -4848,
            .left_box_left = 2912, .left_box_right = 3024,
            .right_child = 32975, .left_child = 201
            },
     {
            .split_x = 3024, .split_y = -4848,
            .split_dx = 0, .split_dy = 8,
            .right_box_top = -4840, .right_box_bottom = -4864,
            .right_box_left = 3024, .right_box_right = 3104,
            .left_box_top = -4840, .left_box_bottom = -4864,
            .left_box_left = 2912, .left_box_right = 3024,
            .right_child = 32974, .left_child = 202
            },
     {
            .split_x = 3024, .split_y = -4840,
            .split_dx = -32, .split_dy = 0,
            .right_box_top = -4672, .right_box_bottom = -4840,
            .right_box_left = 2912, .right_box_right = 3104,
            .left_box_top = -4840, .left_box_bottom = -4864,
            .left_box_left = 2912, .left_box_right = 3104,
            .right_child = 32973, .left_child = 203
            },
     {
            .split_x = 3040, .split_y = -4672,
            .split_dx = 64, .split_dy = 0,
            .right_box_top = -4672, .right_box_bottom = -4864,
            .right_box_left = 2912, .right_box_right = 3104,
            .left_box_top = -4648, .left_box_bottom = -4672,
            .left_box_left = 2976, .left_box_right = 3040,
            .right_child = 204, .left_child = 32978
            },
     {
            .split_x = 3040, .split_y = -4648,
            .split_dx = -64, .split_dy = 0,
            .right_box_top = -4544, .right_box_bottom = -4648,
            .right_box_left = 2944, .right_box_right = 3072,
            .left_box_top = -4648, .left_box_bottom = -4864,
            .left_box_left = 2912, .left_box_right = 3104,
            .right_child = 200, .left_child = 205
            },
     {
            .split_x = 2944, .split_y = -4544,
            .split_dx = -256, .split_dy = 0,
            .right_box_top = -4160, .right_box_bottom = -4544,
            .right_box_left = 2688, .right_box_right = 3328,
            .left_box_top = -4544, .left_box_bottom = -4864,
            .left_box_left = 2912, .left_box_right = 3104,
            .right_child = 194, .left_child = 206
            },
     {
            .split_x = 2784, .split_y = -3776,
            .split_dx = 0, .split_dy = -128,
            .right_box_top = -3776, .right_box_bottom = -3904,
            .right_box_left = 2752, .right_box_right = 2784,
            .left_box_top = -3776, .left_box_bottom = -3904,
            .left_box_left = 2784, .left_box_right = 2816,
            .right_child = 32980, .left_child = 32981
            },
     {
            .split_x = 2816, .split_y = -3904,
            .split_dx = -32, .split_dy = 0,
            .right_box_top = -3776, .right_box_bottom = -3904,
            .right_box_left = 2752, .right_box_right = 2816,
            .left_box_top = -4128, .left_box_bottom = -4128,
            .left_box_left = 2776, .left_box_right = 2816,
            .right_child = 208, .left_child = 32982
            },
     {
            .split_x = 2752, .split_y = -3776,
            .split_dx = 0, .split_dy = -128,
            .right_box_top = -3776, .right_box_bottom = -3904,
            .right_box_left = 2688, .right_box_right = 2752,
            .left_box_top = -3776, .left_box_bottom = -4128,
            .left_box_left = 2752, .left_box_right = 2816,
            .right_child = 32979, .left_child = 209
            },
     {
            .split_x = 2848, .split_y = -3776,
            .split_dx = 0, .split_dy = -128,
            .right_box_top = -3776, .right_box_bottom = -3904,
            .right_box_left = 2816, .right_box_right = 2848,
            .left_box_top = -3776, .left_box_bottom = -3904,
            .left_box_left = 2848, .left_box_right = 2880,
            .right_child = 32983, .left_child = 32984
            },
     {
            .split_x = 2880, .split_y = -3904,
            .split_dx = -32, .split_dy = 0,
            .right_box_top = -3776, .right_box_bottom = -3904,
            .right_box_left = 2816, .right_box_right = 2880,
            .left_box_top = -4128, .left_box_bottom = -4160,
            .left_box_left = 2816, .left_box_right = 2856,
            .right_child = 211, .left_child = 32985
            },
     {
            .split_x = 2880, .split_y = -3776,
            .split_dx = 0, .split_dy = -128,
            .right_box_top = -3776, .right_box_bottom = -4160,
            .right_box_left = 2816, .right_box_right = 2880,
            .left_box_top = -3776, .left_box_bottom = -3904,
            .left_box_left = 2880, .left_box_right = 2912,
            .right_child = 212, .left_child = 32986
            },
     {
            .split_x = 2816, .split_y = -3776,
            .split_dx = 0, .split_dy = -128,
            .right_box_top = -3776, .right_box_bottom = -4128,
            .right_box_left = 2688, .right_box_right = 2816,
            .left_box_top = -3776, .left_box_bottom = -4160,
            .left_box_left = 2816, .left_box_right = 2912,
            .right_child = 210, .left_child = 213
            },
     {
            .split_x = 2240, .split_y = -4096,
            .split_dx = 0, .split_dy = 128,
            .right_box_top = -3968, .right_box_bottom = -4096,
            .right_box_left = 2240, .right_box_right = 2368,
            .left_box_top = -3920, .left_box_bottom = -4096,
            .left_box_left = 2176, .left_box_right = 2240,
            .right_child = 32987, .left_child = 32988
            },
     {
            .split_x = 2688, .split_y = -3920,
            .split_dx = -16, .split_dy = 0,
            .right_box_top = -3792, .right_box_bottom = -3920,
            .right_box_left = 2435, .right_box_right = 2688,
            .left_box_top = -3920, .left_box_bottom = -4096,
            .left_box_left = 2368, .left_box_right = 2672,
            .right_child = 32990, .left_child = 32991
            },
     {
            .split_x = 2688, .split_y = -4128,
            .split_dx = 88, .split_dy = 0,
            .right_box_top = -4128, .right_box_bottom = -4160,
            .right_box_left = 2688, .right_box_right = 2776,
            .left_box_top = -3792, .left_box_bottom = -4096,
            .left_box_left = 2368, .left_box_right = 2688,
            .right_child = 32989, .left_child = 216
            },
     {
            .split_x = 2632, .split_y = -3792,
            .split_dx = 56, .split_dy = -128,
            .right_box_top = -3792, .right_box_bottom = -4160,
            .right_box_left = 2368, .right_box_right = 2776,
            .left_box_top = -3776, .left_box_bottom = -3920,
            .left_box_left = 2632, .left_box_right = 2720,
            .right_child = 217, .left_child = 32992
            },
     {
            .split_x = 2368, .split_y = -3968,
            .split_dx = 0, .split_dy = -128,
            .right_box_top = -3920, .right_box_bottom = -4096,
            .right_box_left = 2176, .right_box_right = 2368,
            .left_box_top = -3776, .left_box_bottom = -4160,
            .left_box_left = 2368, .left_box_right = 2776,
            .right_child = 215, .left_child = 218
            },
     {
            .split_x = 2720, .split_y = -3904,
            .split_dx = -32, .split_dy = 128,
            .right_box_top = -3776, .right_box_bottom = -4160,
            .right_box_left = 2688, .right_box_right = 2912,
            .left_box_top = -3776, .left_box_bottom = -4160,
            .left_box_left = 2176, .left_box_right = 2776,
            .right_child = 214, .left_child = 219
            },
     {
            .split_x = 3072, .split_y = -4000,
            .split_dx = -128, .split_dy = 0,
            .right_box_top = -3648, .right_box_bottom = -4000,
            .right_box_left = 2944, .right_box_right = 3072,
            .left_box_top = -4000, .left_box_bottom = -4016,
            .left_box_left = 2944, .left_box_right = 3072,
            .right_child = 32993, .left_child = 32994
            },
     {
            .split_x = 2944, .split_y = -3776,
            .split_dx = 0, .split_dy = 106,
            .right_box_top = -3648, .right_box_bottom = -4016,
            .right_box_left = 2944, .right_box_right = 3072,
            .left_box_top = -3776, .left_box_bottom = -3904,
            .left_box_left = 2912, .left_box_right = 2944,
            .right_child = 221, .left_child = 32995
            },
     {
            .split_x = 2944, .split_y = -4032,
            .split_dx = 128, .split_dy = 0,
            .right_box_top = -4032, .right_box_bottom = -4160,
            .right_box_left = 2912, .right_box_right = 3104,
            .left_box_top = -4016, .left_box_bottom = -4032,
            .left_box_left = 2944, .left_box_right = 3072,
            .right_child = 32996, .left_child = 32997
            },
     {
            .split_x = 3072, .split_y = -4016,
            .split_dx = -128, .split_dy = 0,
            .right_box_top = -3648, .right_box_bottom = -4016,
            .right_box_left = 2912, .right_box_right = 3072,
            .left_box_top = -4016, .left_box_bottom = -4160,
            .left_box_left = 2912, .left_box_right = 3104,
            .right_child = 222, .left_child = 223
            },
     {
            .split_x = 3200, .split_y = -4128,
            .split_dx = 128, .split_dy = 0,
            .right_box_top = -4128, .right_box_bottom = -4160,
            .right_box_left = 3160, .right_box_right = 3328,
            .left_box_top = -3744, .left_box_bottom = -3968,
            .left_box_left = 3200, .left_box_right = 3328,
            .right_child = 32998, .left_child = 32999
            },
     {
            .split_x = 3328, .split_y = -3744,
            .split_dx = 32, .split_dy = 96,
            .right_box_top = -3574, .right_box_bottom = -3968,
            .right_box_left = 3328, .right_box_right = 3520,
            .left_box_top = -3601, .left_box_bottom = -3648,
            .left_box_left = 3353, .left_box_right = 3360,
            .right_child = 33000, .left_child = 33001
            },
     {
            .split_x = 3328, .split_y = -4128,
            .split_dx = 0, .split_dy = -32,
            .right_box_top = -3744, .right_box_bottom = -4160,
            .right_box_left = 3160, .right_box_right = 3328,
            .left_box_top = -3574, .left_box_bottom = -3968,
            .left_box_left = 3328, .left_box_right = 3520,
            .right_child = 225, .left_child = 226
            },
     {
            .split_x = 3584, .split_y = -3840,
            .split_dx = 0, .split_dy = -64,
            .right_box_top = -3840, .right_box_bottom = -3904,
            .right_box_left = 3520, .right_box_right = 3584,
            .left_box_top = -3840, .left_box_bottom = -3904,
            .left_box_left = 3584, .left_box_right = 3680,
            .right_child = 33002, .left_child = 33003
            },
     {
            .split_x = 3584, .split_y = -3840,
            .split_dx = 32, .split_dy = 64,
            .right_box_top = -3776, .right_box_bottom = -3840,
            .right_box_left = 3584, .right_box_right = 3616,
            .left_box_top = -3546, .left_box_bottom = -3776,
            .left_box_left = 3556, .left_box_right = 3680,
            .right_child = 33004, .left_child = 33005
            },
     {
            .split_x = 3520, .split_y = -3840,
            .split_dx = 16, .split_dy = 0,
            .right_box_top = -3840, .right_box_bottom = -3904,
            .right_box_left = 3520, .right_box_right = 3680,
            .left_box_top = -3546, .left_box_bottom = -3840,
            .left_box_left = 3556, .left_box_right = 3680,
            .right_child = 228, .left_child = 229
            },
     {
            .split_x = 3680, .split_y = -3546,
            .split_dx = 0, .split_dy = -5,
            .right_box_top = -3546, .right_box_bottom = -3904,
            .right_box_left = 3520, .right_box_right = 3680,
            .left_box_top = -3552, .left_box_bottom = -3904,
            .left_box_left = 3680, .left_box_right = 3744,
            .right_child = 230, .left_child = 33006
            },
     {
            .split_x = 3520, .split_y = -3584,
            .split_dx = 0, .split_dy = -256,
            .right_box_top = -3574, .right_box_bottom = -4160,
            .right_box_left = 3160, .right_box_right = 3520,
            .left_box_top = -3546, .left_box_bottom = -3904,
            .left_box_left = 3520, .left_box_right = 3744,
            .right_child = 227, .left_child = 231
            },
     {
            .split_x = 3104, .split_y = -4128,
            .split_dx = 0, .split_dy = -32,
            .right_box_top = -3648, .right_box_bottom = -4160,
            .right_box_left = 2912, .right_box_right = 3104,
            .left_box_top = -3546, .left_box_bottom = -4160,
            .left_box_left = 3160, .left_box_right = 3744,
            .right_child = 224, .left_child = 232
            },
     {
            .split_x = 2912, .split_y = -3776,
            .split_dx = 0, .split_dy = -128,
            .right_box_top = -3776, .right_box_bottom = -4160,
            .right_box_left = 2176, .right_box_right = 2912,
            .left_box_top = -3546, .left_box_bottom = -4160,
            .left_box_left = 2912, .left_box_right = 3744,
            .right_child = 220, .left_child = 233
            },
     {
            .split_x = 2888, .split_y = -4160,
            .split_dx = 24, .split_dy = 0,
            .right_box_top = -4160, .right_box_bottom = -4864,
            .right_box_left = 2688, .right_box_right = 3328,
            .left_box_top = -3546, .left_box_bottom = -4160,
            .left_box_left = 2176, .left_box_right = 3744,
            .right_child = 207, .left_child = 234
            },
     {
            .split_x = 3264, .split_y = -3616,
            .split_dx = -192, .split_dy = -32,
            .right_box_top = -2048, .right_box_bottom = -3776,
            .right_box_left = 2176, .right_box_right = 3808,
            .left_box_top = -3546, .left_box_bottom = -4864,
            .left_box_left = 2176, .left_box_right = 3744,
            .right_child = 185, .left_child = 235
            },
     {
            .split_x = 2176, .split_y = -2304,
            .split_dx = 0, .split_dy = -256,
            .right_box_top = -2048, .right_box_bottom = -4064,
            .right_box_left = -768, .right_box_right = 2176,
            .left_box_top = -2048, .left_box_bottom = -4864,
            .left_box_left = 2176, .left_box_right = 3808,
            .right_child = 131, .left_child = 236
            },
 };

static const sector sectors[88] = {
    {
            .floor_height = -80, .ceil_height = 216,
            .floor_texture = "NUKAGE3", .ceil_texture = "F_SKY1",
            .light_level = 255, .sector_special = 7,
            .sector_tag = 0
            },
     {
            .floor_height = -56, .ceil_height = 216,
            .floor_texture = "FLOOR7_1", .ceil_texture = "F_SKY1",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 0, .ceil_height = 0,
            .floor_texture = "FLOOR4_8", .ceil_texture = "CEIL5_1",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 4
            },
     {
            .floor_height = 8, .ceil_height = 192,
            .floor_texture = "FLAT5_5", .ceil_texture = "FLAT5_5",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 0, .ceil_height = 0,
            .floor_texture = "FLOOR4_8", .ceil_texture = "FLAT20",
            .light_level = 144, .sector_special = 0,
            .sector_tag = 3
            },
     {
            .floor_height = 8, .ceil_height = 192,
            .floor_texture = "FLAT5_5", .ceil_texture = "FLAT5_5",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -136, .ceil_height = -40,
            .floor_texture = "FLOOR5_1", .ceil_texture = "CEIL5_2",
            .light_level = 128, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -120, .ceil_height = 16,
            .floor_texture = "FLOOR5_1", .ceil_texture = "CEIL5_2",
            .light_level = 128, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -104, .ceil_height = 16,
            .floor_texture = "FLOOR5_1", .ceil_texture = "CEIL5_2",
            .light_level = 128, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -88, .ceil_height = 16,
            .floor_texture = "FLOOR5_1", .ceil_texture = "CEIL5_2",
            .light_level = 128, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -72, .ceil_height = 16,
            .floor_texture = "FLOOR5_1", .ceil_texture = "CEIL5_2",
            .light_level = 128, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -56, .ceil_height = 24,
            .floor_texture = "FLOOR7_1", .ceil_texture = "F_SKY1",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -56, .ceil_height = 64,
            .floor_texture = "FLOOR7_1", .ceil_texture = "F_SKY1",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -56, .ceil_height = 16,
            .floor_texture = "FLOOR7_1", .ceil_texture = "CEIL5_2",
            .light_level = 176, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 32, .ceil_height = 88,
            .floor_texture = "FLOOR4_8", .ceil_texture = "CEIL5_1",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 5
            },
     {
            .floor_height = 0, .ceil_height = 224,
            .floor_texture = "FLOOR4_8", .ceil_texture = "CEIL3_5",
            .light_level = 128, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 0, .ceil_height = 224,
            .floor_texture = "FLOOR4_8", .ceil_texture = "CEIL3_5",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 0, .ceil_height = 96,
            .floor_texture = "FLOOR4_8", .ceil_texture = "TLITE6_5",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 0, .ceil_height = 224,
            .floor_texture = "FLOOR4_8", .ceil_texture = "CEIL3_5",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 0, .ceil_height = 96,
            .floor_texture = "FLOOR4_8", .ceil_texture = "TLITE6_5",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 8, .ceil_height = 152,
            .floor_texture = "FLAT14", .ceil_texture = "TLITE6_5",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 32, .ceil_height = 88,
            .floor_texture = "FLAT18", .ceil_texture = "CEIL5_1",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 0, .ceil_height = 72,
            .floor_texture = "FLOOR4_8", .ceil_texture = "CEIL3_5",
            .light_level = 160, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 32, .ceil_height = 88,
            .floor_texture = "FLAT18", .ceil_texture = "CEIL5_1",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 0, .ceil_height = 144,
            .floor_texture = "FLOOR4_8", .ceil_texture = "CEIL3_5",
            .light_level = 144, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 0, .ceil_height = 88,
            .floor_texture = "FLOOR4_8", .ceil_texture = "TLITE6_4",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 0, .ceil_height = 0,
            .floor_texture = "FLOOR4_8", .ceil_texture = "FLAT20",
            .light_level = 208, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -16, .ceil_height = 200,
            .floor_texture = "FLAT14", .ceil_texture = "CEIL3_5",
            .light_level = 224, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -8, .ceil_height = 120,
            .floor_texture = "FLAT14", .ceil_texture = "CEIL3_5",
            .light_level = 192, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 0, .ceil_height = 72,
            .floor_texture = "FLOOR4_8", .ceil_texture = "CEIL3_5",
            .light_level = 144, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 0, .ceil_height = 72,
            .floor_texture = "FLOOR4_8", .ceil_texture = "CEIL3_5",
            .light_level = 255, .sector_special = 1,
            .sector_tag = 0
            },
     {
            .floor_height = -8, .ceil_height = 120,
            .floor_texture = "FLOOR4_8", .ceil_texture = "CEIL3_5",
            .light_level = 128, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -8, .ceil_height = 224,
            .floor_texture = "FLOOR4_8", .ceil_texture = "FLOOR7_2",
            .light_level = 128, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 8, .ceil_height = 224,
            .floor_texture = "FLOOR4_8", .ceil_texture = "FLOOR7_2",
            .light_level = 144, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 24, .ceil_height = 224,
            .floor_texture = "FLOOR4_8", .ceil_texture = "FLOOR7_2",
            .light_level = 144, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 40, .ceil_height = 184,
            .floor_texture = "STEP2", .ceil_texture = "TLITE6_1",
            .light_level = 255, .sector_special = 8,
            .sector_tag = 0
            },
     {
            .floor_height = 40, .ceil_height = 184,
            .floor_texture = "STEP2", .ceil_texture = "TLITE6_1",
            .light_level = 255, .sector_special = 8,
            .sector_tag = 0
            },
     {
            .floor_height = 40, .ceil_height = 224,
            .floor_texture = "FLOOR4_8", .ceil_texture = "FLOOR7_2",
            .light_level = 144, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 56, .ceil_height = 224,
            .floor_texture = "FLOOR4_8", .ceil_texture = "FLOOR7_2",
            .light_level = 144, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 72, .ceil_height = 224,
            .floor_texture = "FLOOR4_8", .ceil_texture = "FLOOR7_2",
            .light_level = 144, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 0, .ceil_height = 128,
            .floor_texture = "FLOOR7_1", .ceil_texture = "F_SKY1",
            .light_level = 224, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 104, .ceil_height = 264,
            .floor_texture = "FLOOR4_8", .ceil_texture = "FLOOR7_2",
            .light_level = 192, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 0, .ceil_height = 264,
            .floor_texture = "FLOOR7_1", .ceil_texture = "F_SKY1",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 136, .ceil_height = 240,
            .floor_texture = "FLAT20", .ceil_texture = "FLAT20",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 128, .ceil_height = 264,
            .floor_texture = "FLOOR1_1", .ceil_texture = "FLOOR7_2",
            .light_level = 192, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 136, .ceil_height = 240,
            .floor_texture = "FLAT20", .ceil_texture = "FLAT20",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 136, .ceil_height = 240,
            .floor_texture = "FLAT20", .ceil_texture = "FLAT20",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 104, .ceil_height = 192,
            .floor_texture = "FLOOR4_8", .ceil_texture = "FLOOR7_2",
            .light_level = 176, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 88, .ceil_height = 224,
            .floor_texture = "FLOOR4_8", .ceil_texture = "FLOOR7_2",
            .light_level = 144, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -24, .ceil_height = 176,
            .floor_texture = "FLOOR5_2", .ceil_texture = "CEIL3_5",
            .light_level = 144, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -16, .ceil_height = 72,
            .floor_texture = "FLOOR4_8", .ceil_texture = "CEIL3_5",
            .light_level = 160, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -48, .ceil_height = 176,
            .floor_texture = "NUKAGE3", .ceil_texture = "CEIL3_5",
            .light_level = 144, .sector_special = 7,
            .sector_tag = 0
            },
     {
            .floor_height = -24, .ceil_height = 176,
            .floor_texture = "FLOOR5_2", .ceil_texture = "CEIL3_5",
            .light_level = 144, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -48, .ceil_height = 176,
            .floor_texture = "NUKAGE3", .ceil_texture = "CEIL3_5",
            .light_level = 192, .sector_special = 7,
            .sector_tag = 0
            },
     {
            .floor_height = -48, .ceil_height = 104,
            .floor_texture = "FLOOR4_8", .ceil_texture = "FLOOR6_2",
            .light_level = 128, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 96, .ceil_height = 176,
            .floor_texture = "FLOOR4_8", .ceil_texture = "CEIL3_5",
            .light_level = 160, .sector_special = 0,
            .sector_tag = 1
            },
     {
            .floor_height = -24, .ceil_height = 176,
            .floor_texture = "FLOOR5_2", .ceil_texture = "CEIL3_5",
            .light_level = 192, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -48, .ceil_height = 176,
            .floor_texture = "NUKAGE3", .ceil_texture = "CEIL3_5",
            .light_level = 192, .sector_special = 7,
            .sector_tag = 0
            },
     {
            .floor_height = 104, .ceil_height = 184,
            .floor_texture = "FLOOR4_8", .ceil_texture = "FLOOR6_2",
            .light_level = 128, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 16, .ceil_height = 152,
            .floor_texture = "FLAT14", .ceil_texture = "TLITE6_5",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 24, .ceil_height = 152,
            .floor_texture = "FLAT14", .ceil_texture = "TLITE6_5",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -8, .ceil_height = 72,
            .floor_texture = "FLOOR4_8", .ceil_texture = "CEIL3_5",
            .light_level = 160, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = 0, .ceil_height = 136,
            .floor_texture = "FLAT20", .ceil_texture = "FLAT20",
            .light_level = 192, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -56, .ceil_height = 24,
            .floor_texture = "FLOOR7_1", .ceil_texture = "F_SKY1",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -24, .ceil_height = 104,
            .floor_texture = "FLOOR5_2", .ceil_texture = "TLITE6_5",
            .light_level = 255, .sector_special = 12,
            .sector_tag = 0
            },
     {
            .floor_height = -24, .ceil_height = 48,
            .floor_texture = "FLOOR5_2", .ceil_texture = "CEIL3_5",
            .light_level = 128, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -24, .ceil_height = 72,
            .floor_texture = "FLOOR5_2", .ceil_texture = "CEIL3_5",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -24, .ceil_height = 56,
            .floor_texture = "FLOOR5_2", .ceil_texture = "FLAT23",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -24, .ceil_height = 48,
            .floor_texture = "FLOOR5_2", .ceil_texture = "CEIL3_5",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -24, .ceil_height = -24,
            .floor_texture = "FLOOR5_2", .ceil_texture = "FLAT20",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -24, .ceil_height = 88,
            .floor_texture = "FLOOR5_2", .ceil_texture = "TLITE6_5",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -24, .ceil_height = 72,
            .floor_texture = "FLOOR5_2", .ceil_texture = "FLAT23",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -24, .ceil_height = 48,
            .floor_texture = "FLOOR5_2", .ceil_texture = "CEIL3_5",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -104, .ceil_height = 48,
            .floor_texture = "FLOOR5_1", .ceil_texture = "CEIL5_2",
            .light_level = 128, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -88, .ceil_height = 48,
            .floor_texture = "FLOOR5_1", .ceil_texture = "CEIL5_2",
            .light_level = 128, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -72, .ceil_height = 48,
            .floor_texture = "FLOOR5_1", .ceil_texture = "CEIL5_2",
            .light_level = 128, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -56, .ceil_height = 48,
            .floor_texture = "FLOOR5_1", .ceil_texture = "CEIL5_2",
            .light_level = 128, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -40, .ceil_height = 48,
            .floor_texture = "FLOOR5_1", .ceil_texture = "CEIL5_2",
            .light_level = 128, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -24, .ceil_height = 48,
            .floor_texture = "FLOOR5_2", .ceil_texture = "CEIL5_2",
            .light_level = 128, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -136, .ceil_height = -24,
            .floor_texture = "FLOOR5_1", .ceil_texture = "TLITE6_6",
            .light_level = 255, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -136, .ceil_height = -40,
            .floor_texture = "FLOOR5_1", .ceil_texture = "CEIL5_2",
            .light_level = 128, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -120, .ceil_height = 48,
            .floor_texture = "FLOOR5_1", .ceil_texture = "CEIL5_2",
            .light_level = 128, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -24, .ceil_height = 48,
            .floor_texture = "FLOOR5_2", .ceil_texture = "CEIL3_5",
            .light_level = 128, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -24, .ceil_height = -24,
            .floor_texture = "FLOOR5_2", .ceil_texture = "FLAT5_5",
            .light_level = 144, .sector_special = 9,
            .sector_tag = 0
            },
     {
            .floor_height = -24, .ceil_height = 48,
            .floor_texture = "FLOOR5_2", .ceil_texture = "CEIL3_5",
            .light_level = 128, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -24, .ceil_height = -24,
            .floor_texture = "FLOOR5_2", .ceil_texture = "FLAT20",
            .light_level = 128, .sector_special = 0,
            .sector_tag = 0
            },
     {
            .floor_height = -48, .ceil_height = 32,
            .floor_texture = "FLOOR4_8", .ceil_texture = "TLITE6_6",
            .light_level = 255, .sector_special = 9,
            .sector_tag = 0
            },
     {
            .floor_height = 104, .ceil_height = 184,
            .floor_texture = "FLOOR4_8", .ceil_texture = "FLOOR6_2",
            .light_level = 128, .sector_special = 9,
            .sector_tag = 2
            },
 };

static const blockmap blkmap = {.x_origin = -776, .y_origin = -4872, 
                .num_columns = 36, .num_rows = 23, .num_offsets = 828,
                .offsets_plus_table = {
828, 830, 832, 834, 836, 838, 840, 842, 844, 846, 848, 850, 852, 854, 856, 858, 860, 862, 864, 866, 868, 870, 872, 874, 876, 878, 880, 882, 884, 889, 896, 900, 902, 904, 906, 908,
910, 912, 914, 916, 918, 920, 922, 924, 926, 928, 930, 932, 934, 936, 938, 940, 942, 944, 946, 948, 950, 952, 954, 956, 958, 960, 962, 964, 966, 971, 984, 988, 990, 992, 994, 996,
998, 1000, 1002, 1004, 1006, 1008, 1010, 1012, 1014, 1016, 1018, 1020, 1022, 1024, 1026, 1028, 1030, 1032, 1034, 1036, 1038, 1040, 1042, 1044, 1046, 1048, 1050, 1052, 1056, 1059, 1073, 1079, 1082, 1086, 1088, 1090,
1092, 1094, 1096, 1098, 1100, 1102, 1104, 1106, 1108, 1110, 1112, 1114, 1116, 1118, 1120, 1122, 1124, 1126, 1128, 1130, 1132, 1134, 1136, 1138, 1140, 1142, 1144, 1146, 1149, 1154, 1156, 1161, 1163, 1166, 1168, 1170,
1172, 1174, 1176, 1178, 1180, 1182, 1184, 1186, 1188, 1190, 1192, 1194, 1196, 1198, 1200, 1202, 1204, 1206, 1208, 1210, 1212, 1214, 1216, 1218, 1220, 1222, 1224, 1226, 1229, 1239, 1242, 1252, 1254, 1257, 1259, 1261,
1263, 1265, 1267, 1269, 1271, 1273, 1275, 1277, 1279, 1281, 1283, 1285, 1287, 1289, 1291, 1293, 1295, 1297, 1299, 1301, 1303, 1305, 1307, 1309, 1311, 1313, 1315, 1317, 1321, 1333, 1336, 1347, 1351, 1355, 1357, 1359,
1361, 1363, 1365, 1367, 1369, 1371, 1373, 1375, 1377, 1379, 1381, 1383, 1385, 1387, 1389, 1391, 1393, 1395, 1397, 1399, 1401, 1403, 1405, 1409, 1415, 1421, 1424, 1427, 1429, 1432, 1441, 1450, 1452, 1454, 1456, 1458,
1460, 1462, 1464, 1466, 1468, 1470, 1472, 1474, 1476, 1478, 1480, 1482, 1484, 1486, 1488, 1490, 1492, 1494, 1496, 1498, 1500, 1502, 1504, 1512, 1523, 1529, 1532, 1537, 1548, 1559, 1564, 1567, 1572, 1577, 1583, 1589,
1592, 1594, 1596, 1598, 1600, 1602, 1604, 1606, 1608, 1610, 1612, 1614, 1616, 1618, 1620, 1622, 1624, 1626, 1628, 1631, 1635, 1638, 1643, 1652, 1664, 1667, 1672, 1679, 1688, 1699, 1704, 1707, 1712, 1717, 1723, 1729,
1733, 1736, 1740, 1743, 1746, 1750, 1754, 1759, 1761, 1763, 1765, 1767, 1769, 1772, 1776, 1784, 1788, 1793, 1797, 1802, 1805, 1808, 1815, 1822, 1832, 1835, 1839, 1843, 1849, 1852, 1857, 1862, 1867, 1871, 1875, 1879,
1882, 1886, 1890, 1893, 1896, 1899, 1901, 1905, 1907, 1909, 1911, 1913, 1917, 1921, 1924, 1926, 1929, 1938, 1940, 1942, 1944, 1946, 1948, 1950, 1952, 1954, 1956, 1958, 1964, 1968, 1974, 1978, 1984, 1992, 2001, 2005,
2007, 2010, 2013, 2015, 2019, 2024, 2031, 2038, 2042, 2048, 2052, 2055, 2061, 2063, 2073, 2077, 2086, 2097, 2099, 2103, 2106, 2110, 2113, 2117, 2119, 2121, 2123, 2125, 2133, 2136, 2140, 2145, 2148, 2153, 2160, 2165,
2168, 2171, 2174, 2176, 2183, 2195, 2203, 2206, 2217, 2228, 2234, 2239, 2242, 2244, 2248, 2250, 2252, 2259, 2262, 2265, 2267, 2269, 2271, 2275, 2277, 2279, 2281, 2283, 2287, 2291, 2295, 2298, 2302, 2305, 2308, 2312,
2316, 2319, 2322, 2324, 2332, 2342, 2353, 2357, 2368, 2379, 2385, 2390, 2396, 2398, 2406, 2409, 2416, 2427, 2431, 2434, 2438, 2442, 2445, 2449, 2451, 2454, 2458, 2461, 2468, 2471, 2476, 2479, 2481, 2487, 2490, 2494,
2497, 2500, 2503, 2505, 2507, 2509, 2512, 2519, 2523, 2529, 2533, 2535, 2538, 2540, 2546, 2549, 2556, 2560, 2562, 2564, 2566, 2568, 2570, 2572, 2576, 2579, 2581, 2583, 2589, 2593, 2598, 2603, 2609, 2615, 2620, 2623,
2625, 2629, 2634, 2637, 2640, 2643, 2646, 2651, 2653, 2655, 2657, 2659, 2663, 2667, 2671, 2674, 2679, 2686, 2690, 2693, 2698, 2702, 2706, 2709, 2715, 2717, 2719, 2721, 2726, 2732, 2738, 2741, 2743, 2747, 2750, 2754,
2756, 2759, 2763, 2766, 2769, 2772, 2775, 2780, 2782, 2784, 2786, 2788, 2790, 2792, 2794, 2796, 2799, 2802, 2804, 2809, 2817, 2821, 2825, 2829, 2837, 2840, 2845, 2850, 2856, 2859, 2862, 2864, 2866, 2868, 2870, 2872,
2874, 2876, 2878, 2880, 2882, 2884, 2886, 2888, 2890, 2892, 2894, 2896, 2898, 2900, 2902, 2904, 2907, 2911, 2914, 2918, 2924, 2930, 2936, 2940, 2943, 2947, 2953, 2956, 2960, 2964, 2970, 2972, 2974, 2976, 2978, 2980,
2982, 2984, 2986, 2988, 2990, 2992, 2994, 2996, 2998, 3000, 3002, 3004, 3006, 3008, 3010, 3012, 3016, 3019, 3025, 3032, 3037, 3043, 3049, 3055, 3062, 3067, 3076, 3081, 3086, 3088, 3090, 3092, 3094, 3096, 3098, 3100,
3102, 3104, 3106, 3108, 3110, 3112, 3114, 3116, 3118, 3120, 3122, 3124, 3126, 3128, 3130, 3132, 3134, 3136, 3141, 3150, 3158, 3160, 3162, 3165, 3169, 3173, 3176, 3179, 3181, 3183, 3185, 3187, 3189, 3191, 3193, 3195,
3197, 3199, 3201, 3203, 3205, 3207, 3209, 3211, 3213, 3215, 3217, 3219, 3221, 3223, 3225, 3227, 3229, 3231, 3233, 3236, 3239, 3247, 3255, 3262, 3270, 3276, 3279, 3282, 3284, 3286, 3288, 3290, 3292, 3294, 3296, 3298,
3300, 3302, 3304, 3306, 3308, 3310, 3312, 3314, 3316, 3318, 3320, 3322, 3324, 3326, 3328, 3330, 3332, 3334, 3336, 3340, 3344, 3347, 3350, 3353, 3356, 3359, 3363, 3367, 3369, 3371, 3373, 3375, 3377, 3379, 3381, 3383,
3385, 3387, 3389, 3391, 3393, 3395, 3397, 3399, 3401, 3403, 3405, 3407, 3409, 3411, 3413, 3415, 3417, 3419, 3421, 3425, 3429, 3432, 3435, 3438, 3441, 3444, 3448, 3452, 3454, 3456, 3458, 3460, 3462, 3464, 3466, 3468,

0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 325, 326, 328,
65535, 0, 328, 343, 344, 345, 346, 65535, 0, 324,
328, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 323,
326, 327, 65535, 0, 314, 315, 316, 317, 318, 319,
320, 321, 322, 323, 329, 65535, 0, 322, 324, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 301, 307, 65535, 0, 301,
65535, 0, 301, 303, 311, 312, 313, 314, 315, 338,
339, 340, 341, 342, 65535, 0, 300, 302, 311, 312,
65535, 0, 300, 65535, 0, 300, 308, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 307,
65535, 0, 283, 284, 285, 65535, 0, 65535, 0, 280,
281, 282, 65535, 0, 65535, 0, 308, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 307,
65535, 0, 283, 285, 292, 293, 294, 295, 305, 309,
65535, 0, 305, 65535, 0, 280, 282, 288, 289, 297,
299, 305, 310, 65535, 0, 65535, 0, 308, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535, 0,
275, 307, 65535, 0, 275, 276, 277, 286, 287, 295,
296, 304, 309, 332, 65535, 0, 304, 65535, 0, 278,
279, 290, 291, 298, 299, 304, 310, 335, 65535, 0,
274, 279, 65535, 0, 274, 308, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 65535, 0, 211, 257,
65535, 0, 211, 218, 435, 436, 65535, 0, 219, 252,
434, 435, 65535, 0, 252, 65535, 0, 252, 65535, 0,
65535, 0, 332, 65535, 0, 245, 306, 330, 332, 333,
336, 337, 65535, 0, 186, 306, 331, 334, 335, 336,
337, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 207, 208, 216,
217, 257, 258, 65535, 0, 209, 210, 216, 217, 218,
255, 256, 437, 438, 65535, 0, 219, 253, 438, 439,
65535, 0, 253, 65535, 0, 226, 251, 252, 65535, 0,
223, 224, 225, 226, 228, 236, 237, 238, 251, 65535,
0, 220, 221, 222, 234, 235, 236, 240, 241, 244,
65535, 0, 241, 243, 245, 65535, 0, 186, 65535, 0,
423, 430, 431, 65535, 0, 422, 425, 430, 65535, 0,
422, 459, 467, 468, 65535, 0, 445, 446, 460, 467,
65535, 0, 446, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 45, 65535, 0, 45, 158, 65535, 0, 158, 65535,
0, 158, 201, 249, 65535, 0, 205, 206, 207, 213,
214, 215, 442, 65535, 0, 157, 199, 203, 204, 209,
213, 214, 215, 250, 441, 65535, 0, 157, 65535, 0,
156, 157, 253, 65535, 0, 156, 226, 227, 253, 254,
65535, 0, 223, 224, 225, 227, 229, 230, 231, 65535,
0, 220, 221, 222, 231, 232, 233, 239, 242, 244,
65535, 0, 187, 242, 243, 65535, 0, 186, 65535, 0,
423, 432, 433, 65535, 0, 424, 425, 433, 65535, 0,
444, 459, 465, 466, 65535, 0, 447, 448, 460, 466,
65535, 0, 446, 451, 65535, 0, 123, 65535, 0, 122,
123, 65535, 0, 122, 65535, 0, 122, 65535, 0, 122,
127, 65535, 0, 122, 127, 65535, 0, 121, 122, 127,
65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535, 0,
65535, 0, 6, 65535, 0, 4, 6, 65535, 0, 0,
1, 2, 3, 4, 46, 65535, 0, 3, 5, 65535,
0, 44, 248, 474, 65535, 0, 44, 248, 65535, 0,
44, 45, 248, 65535, 0, 248, 65535, 0, 248, 65535,
0, 201, 202, 247, 248, 249, 65535, 0, 202, 212,
247, 442, 443, 65535, 0, 199, 200, 212, 246, 247,
250, 440, 441, 65535, 0, 246, 65535, 0, 156, 246,
65535, 0, 156, 246, 65535, 0, 156, 175, 185, 246,
65535, 0, 185, 65535, 0, 185, 187, 190, 65535, 0,
184, 186, 190, 65535, 0, 184, 194, 195, 65535, 0,
424, 428, 65535, 0, 444, 448, 65535, 0, 448, 451,
65535, 0, 451, 65535, 0, 123, 124, 65535, 0, 127,
128, 65535, 0, 127, 65535, 0, 127, 65535, 0, 127,
65535, 0, 65535, 0, 110, 121, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 8, 52, 65535, 0,
6, 8, 65535, 0, 6, 65535, 0, 65535, 0, 5,
65535, 0, 5, 7, 470, 471, 473, 474, 476, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 174, 175, 185, 192, 65535, 0, 189, 192, 65535,
0, 179, 189, 190, 192, 65535, 0, 193, 194, 65535,
0, 194, 195, 197, 198, 65535, 0, 195, 197, 198,
266, 428, 429, 65535, 0, 265, 266, 426, 427, 444,
448, 449, 65535, 0, 451, 452, 65535, 0, 65535, 0,
124, 65535, 0, 128, 65535, 0, 65535, 0, 107, 108,
65535, 0, 101, 108, 115, 65535, 0, 94, 102, 109,
110, 115, 65535, 0, 91, 92, 93, 94, 110, 65535,
0, 66, 91, 65535, 0, 66, 138, 139, 141, 65535,
0, 65, 66, 65535, 0, 60, 65535, 0, 52, 54,
59, 60, 65535, 0, 65535, 0, 9, 10, 11, 12,
37, 48, 49, 50, 65535, 0, 37, 48, 65535, 0,
13, 14, 15, 16, 37, 38, 48, 65535, 0, 25,
30, 32, 38, 469, 471, 472, 475, 476, 65535, 0,
65535, 0, 166, 167, 65535, 0, 166, 65535, 0, 165,
166, 65535, 0, 165, 65535, 0, 164, 165, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 65535, 0, 174, 175,
176, 189, 271, 272, 65535, 0, 189, 65535, 0, 179,
180, 65535, 0, 179, 180, 193, 65535, 0, 193, 65535,
0, 196, 197, 264, 65535, 0, 196, 264, 265, 449,
450, 65535, 0, 450, 452, 453, 65535, 0, 453, 65535,
0, 124, 65535, 0, 128, 65535, 0, 65535, 0, 97,
103, 107, 118, 119, 65535, 0, 97, 101, 114, 348,
349, 350, 351, 353, 354, 355, 65535, 0, 94, 102,
113, 114, 132, 133, 65535, 0, 92, 65535, 0, 70,
71, 72, 73, 83, 84, 85, 88, 92, 65535, 0,
73, 74, 75, 76, 86, 87, 139, 140, 141, 65535,
0, 61, 62, 64, 65, 65535, 0, 55, 60, 62,
65535, 0, 54, 65535, 0, 65535, 0, 49, 50, 65535,
0, 65535, 0, 65535, 0, 25, 26, 30, 31, 35,
65535, 0, 167, 65535, 0, 167, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 163, 164, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 271, 272, 65535, 0,
180, 182, 65535, 0, 180, 181, 65535, 0, 181, 65535,
0, 181, 193, 65535, 0, 264, 65535, 0, 264, 65535,
0, 450, 454, 65535, 0, 453, 455, 65535, 0, 124,
65535, 0, 128, 65535, 0, 65535, 0, 98, 104, 105,
106, 118, 119, 65535, 0, 98, 100, 105, 116, 117,
351, 352, 353, 65535, 0, 95, 99, 111, 112, 113,
116, 117, 130, 131, 65535, 0, 89, 95, 65535, 0,
79, 80, 81, 82, 83, 84, 85, 88, 89, 65535,
0, 76, 77, 78, 79, 86, 87, 134, 135, 137,
65535, 0, 61, 63, 68, 69, 65535, 0, 56, 57,
63, 65535, 0, 53, 54, 57, 58, 65535, 0, 65535,
0, 17, 18, 19, 47, 49, 50, 65535, 0, 47,
65535, 0, 22, 23, 24, 39, 47, 65535, 0, 26,
27, 28, 29, 33, 34, 35, 36, 39, 65535, 0,
160, 167, 65535, 0, 160, 65535, 0, 160, 161, 65535,
0, 161, 162, 65535, 0, 162, 65535, 0, 162, 163,
65535, 0, 65535, 0, 268, 65535, 0, 268, 269, 65535,
0, 269, 65535, 0, 269, 270, 271, 272, 273, 65535,
0, 182, 65535, 0, 181, 182, 183, 65535, 0, 183,
65535, 0, 65535, 0, 264, 458, 463, 464, 65535, 0,
454, 65535, 0, 454, 455, 65535, 0, 455, 65535, 0,
124, 65535, 0, 128, 65535, 0, 65535, 0, 65535, 0,
65535, 0, 112, 65535, 0, 89, 90, 95, 96, 112,
65535, 0, 67, 90, 65535, 0, 67, 135, 136, 137,
65535, 0, 67, 69, 65535, 0, 65535, 0, 53, 65535,
0, 65535, 0, 17, 19, 20, 51, 65535, 0, 51,
65535, 0, 21, 22, 24, 39, 51, 65535, 0, 27,
36, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 267, 268, 65535, 0, 268,
65535, 0, 65535, 0, 65535, 0, 173, 191, 259, 273,
65535, 0, 191, 259, 65535, 0, 177, 182, 259, 65535,
0, 177, 178, 183, 65535, 0, 177, 178, 458, 461,
65535, 0, 458, 461, 462, 463, 65535, 0, 454, 456,
463, 65535, 0, 455, 65535, 0, 65535, 0, 124, 125,
65535, 0, 125, 128, 129, 65535, 0, 129, 65535, 0,
129, 65535, 0, 129, 65535, 0, 129, 65535, 0, 112,
120, 129, 65535, 0, 65535, 0, 65535, 0, 65535, 0,
65535, 0, 40, 53, 65535, 0, 40, 41, 65535, 0,
41, 42, 65535, 0, 42, 65535, 0, 42, 142, 146,
65535, 0, 27, 36, 43, 143, 146, 65535, 0, 43,
478, 65535, 0, 478, 65535, 0, 478, 479, 483, 65535,
0, 159, 479, 65535, 0, 159, 480, 65535, 0, 480,
65535, 0, 267, 480, 481, 482, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 173, 260, 347, 65535, 0, 171,
191, 260, 347, 65535, 0, 172, 177, 191, 263, 65535,
0, 177, 65535, 0, 65535, 0, 457, 462, 65535, 0,
457, 65535, 0, 455, 457, 65535, 0, 65535, 0, 125,
65535, 0, 125, 126, 65535, 0, 126, 65535, 0, 126,
65535, 0, 126, 65535, 0, 126, 65535, 0, 120, 126,
129, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 142,
65535, 0, 143, 65535, 0, 65535, 0, 370, 371, 372,
65535, 0, 370, 373, 477, 483, 484, 485, 65535, 0,
373, 477, 65535, 0, 373, 477, 65535, 0, 373, 477,
65535, 0, 361, 373, 390, 391, 477, 482, 65535, 0,
390, 65535, 0, 374, 375, 390, 65535, 0, 168, 375,
421, 65535, 0, 168, 188, 260, 261, 65535, 0, 188,
65535, 0, 263, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 142, 65535, 0,
143, 144, 65535, 0, 144, 65535, 0, 358, 372, 65535,
0, 358, 384, 385, 485, 65535, 0, 411, 412, 414,
415, 65535, 0, 409, 412, 415, 416, 65535, 0, 392,
405, 65535, 0, 392, 65535, 0, 392, 393, 65535, 0,
374, 375, 377, 379, 65535, 0, 421, 65535, 0, 170,
421, 65535, 0, 170, 188, 65535, 0, 170, 188, 262,
263, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 142, 145, 65535, 0, 145,
65535, 0, 144, 145, 147, 150, 65535, 0, 148, 149,
150, 153, 154, 65535, 0, 154, 378, 385, 65535, 0,
410, 411, 413, 414, 65535, 0, 409, 410, 413, 416,
65535, 0, 403, 404, 405, 406, 65535, 0, 401, 402,
403, 407, 408, 65535, 0, 393, 400, 401, 65535, 0,
169, 360, 369, 376, 379, 388, 389, 65535, 0, 169,
359, 360, 65535, 0, 169, 170, 421, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 145,
147, 151, 65535, 0, 148, 149, 151, 152, 155, 356,
357, 65535, 0, 155, 356, 367, 378, 386, 387, 65535,
0, 65535, 0, 65535, 0, 406, 65535, 0, 407, 408,
65535, 0, 393, 400, 65535, 0, 369, 65535, 0, 359,
65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 357, 65535, 0, 367,
65535, 0, 381, 382, 383, 417, 419, 420, 65535, 0,
380, 381, 383, 417, 418, 419, 65535, 0, 394, 395,
396, 397, 406, 65535, 0, 394, 397, 398, 399, 407,
408, 65535, 0, 393, 394, 399, 400, 65535, 0, 369,
65535, 0, 359, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 357,
366, 65535, 0, 367, 368, 65535, 0, 368, 65535, 0,
368, 65535, 0, 368, 65535, 0, 368, 65535, 0, 368,
65535, 0, 368, 369, 65535, 0, 359, 365, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535, 0,
65535, 0, 65535, 0, 362, 366, 65535, 0, 362, 363,
65535, 0, 363, 65535, 0, 363, 65535, 0, 363, 65535,
0, 363, 65535, 0, 363, 65535, 0, 363, 364, 65535,
0, 364, 365, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 0, 65535, 0, 65535, 0, 65535, 0, 65535,
0, 65535, 65504, 65296, 90, 1
}};

static const render_blockmap render_blkmap = {.x_origin = -776, .y_origin = -4872, 
                .num_columns = 36, .num_rows = 23, .num_offsets = 828,
                .offsets_plus_table = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 828, 853, 894, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 911, 936, 1025, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1042, 1059, 1068, 1165, 1198, 1207, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1224, 1233, 0, 1258, 0, 1283, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1292, 1301, 1366, 1375, 0, 1440, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1449, 1466, 1547, 1556, 1629, 1646, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1663, 1680, 1713, 1746, 1755, 0, 1764, 1773, 1830, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1887, 1936, 2009, 2042, 2051, 2076, 2149, 2222, 2247, 2256, 2281, 2306, 2339, 2372,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2381, 2390, 2407, 2416, 2441, 2498, 2579, 2588, 2613, 2654, 2711, 2784, 2809, 2818, 2843, 2868, 2901, 2934,
2951, 2960, 2977, 2986, 2995, 3012, 3029, 0, 0, 0, 0, 0, 3054, 3063, 3080, 3121, 3138, 3163, 3180, 3205, 3214, 3223, 3264, 3305, 3370, 3379, 3396, 3413, 3446, 3455, 3480, 3505, 3530, 3547, 3564, 3581,
3590, 3607, 3624, 3633, 3642, 0, 3651, 0, 0, 0, 0, 3668, 3685, 3702, 0, 3711, 3720, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3777, 3810, 3827, 3860, 3877, 3910, 3959, 4016, 0,
4033, 4042, 0, 4051, 4068, 4093, 4134, 4175, 4192, 4225, 4242, 4251, 0, 4284, 4349, 4366, 4423, 0, 4496, 4513, 4522, 4539, 4548, 0, 0, 0, 0, 4565, 4614, 4623, 4640, 4665, 4674, 4699, 4740, 4765,
4774, 4783, 0, 4792, 4833, 4914, 4963, 4972, 5045, 5118, 5151, 5176, 0, 5185, 0, 0, 5202, 5243, 5252, 0, 0, 0, 5261, 0, 0, 0, 0, 5278, 5295, 5312, 5329, 5338, 5355, 5364, 5373, 5390,
5407, 5416, 0, 5425, 5474, 5539, 5612, 5629, 5702, 5775, 5808, 5833, 0, 5866, 5915, 5924, 5965, 6038, 6055, 6064, 6081, 6098, 6107, 0, 6124, 6133, 6150, 6159, 6200, 6209, 6234, 0, 6243, 6276, 6285, 6302,
6311, 6320, 0, 0, 0, 6329, 6338, 6379, 6396, 6429, 0, 6446, 0, 6455, 6488, 6497, 6538, 0, 0, 0, 0, 0, 0, 6555, 6572, 0, 0, 6581, 6614, 6631, 6656, 6681, 6714, 6747, 6772, 0,
6781, 6798, 6823, 6832, 6841, 6850, 6859, 0, 0, 0, 0, 6884, 6901, 6918, 6935, 6944, 6969, 7010, 7027, 7036, 7061, 7078, 7095, 7104, 0, 0, 0, 7137, 7162, 7195, 7228, 0, 7237, 7254, 7263, 0,
7280, 7289, 7306, 7315, 7324, 7333, 7342, 0, 0, 0, 0, 0, 0, 0, 0, 7367, 7376, 0, 7385, 7410, 7459, 7476, 7493, 7510, 7559, 7568, 7593, 7618, 7651, 7660, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7669, 7678, 7695, 7704, 7721, 7754, 7787, 7820, 7837, 7846, 7863, 7896, 7905, 7922, 7939, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7972, 7989, 7998, 8031, 8072, 8097, 8130, 8163, 8196, 8237, 8262, 8319, 8344, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8369, 8394, 8451, 0, 0, 8500, 8509, 8526, 8543, 8552, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8561, 8570, 8579, 8628, 8677, 8718, 8767, 8800, 8809, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8818, 8835, 8852, 8861, 8870, 8879, 8888, 8897, 8914, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8931, 8948, 8965, 8974, 8983, 8992, 9001, 9010, 9027, 0, 0, 0, 0, 0, 0, 0, 0, 0,

3, 40, 1314, 1068, 7765, -12971, 1072, 7765, -12800, 40,
1570, 1072, 7765, -12800, 1076, 7765, -12630, 41, 34, 1064,
8277, -12971, 1068, 7765, -12971, 5, 41, 34, 1064, 8277,
-12971, 1068, 7765, -12971, 42, 1809, 1112, 8064, -12907, 1116,
7978, -12907, 43, 17, 1120, 7978, -12928, 1124, 8064, -12928,
43, 273, 1124, 8064, -12928, 1112, 8064, -12907, 43, 529,
1116, 7978, -12907, 1120, 7978, -12928, 2, 40, 1058, 1056,
8277, -12459, 1064, 8277, -12971, 41, 34, 1064, 8277, -12971,
1068, 7765, -12971, 3, 40, 802, 1060, 7765, -12459, 1048,
7936, -12459, 40, 1570, 1072, 7765, -12800, 1076, 7765, -12630,
40, 1826, 1076, 7765, -12630, 1060, 7765, -12459, 11, 39,
546, 1032, 7936, -12352, 1028, 7936, -12288, 39, 802, 1024,
8106, -12288, 1036, 8106, -12352, 39, 1058, 1036, 8106, -12352,
1040, 8106, -12395, 39, 1314, 1040, 8106, -12395, 1044, 8106,
-12459, 39, 1570, 1048, 7936, -12459, 1052, 7936, -12395, 39,
1826, 1052, 7936, -12395, 1032, 7936, -12352, 40, 17, 1052,
7936, -12395, 1040, 8106, -12395, 40, 273, 1036, 8106, -12352,
1032, 7936, -12352, 40, 546, 1044, 8106, -12459, 1056, 8277,
-12459, 40, 802, 1060, 7765, -12459, 1048, 7936, -12459, 41,
273, 1048, 7936, -12459, 1044, 8106, -12459, 2, 40, 546,
1044, 8106, -12459, 1056, 8277, -12459, 40, 1058, 1056, 8277,
-12459, 1064, 8277, -12971, 2, 37, 1314, 1008, 7850, -12118,
1012, 7168, -12118, 38, 802, 1012, 7168, -12118, 904, 7168,
-11008, 1, 37, 1314, 1008, 7850, -12118, 1012, 7168, -12118,
12, 37, 1314, 1008, 7850, -12118, 1012, 7168, -12118, 37,
1826, 1020, 7850, -12288, 1008, 7850, -12118, 38, 1809, 1008,
7850, -12118, 1004, 8192, -12118, 39, 34, 1016, 8192, -12288,
1024, 8106, -12288, 39, 290, 1028, 7936, -12288, 1020, 7850,
-12288, 39, 546, 1032, 7936, -12352, 1028, 7936, -12288, 39,
802, 1024, 8106, -12288, 1036, 8106, -12352, 42, 529, 1096,
8064, -12246, 1100, 7978, -12246, 42, 785, 1104, 7978, -12267,
1108, 8064, -12267, 42, 1041, 1108, 8064, -12267, 1096, 8064,
-12246, 42, 1297, 1100, 7978, -12246, 1104, 7978, -12267, 42,
1553, 1024, 8106, -12288, 1028, 7936, -12288, 4, 37, 1058,
1000, 8874, -12118, 1004, 8192, -12118, 37, 1570, 1004, 8192,
-12118, 1016, 8192, -12288, 38, 1809, 1008, 7850, -12118, 1004,
8192, -12118, 39, 34, 1016, 8192, -12288, 1024, 8106, -12288,
1, 37, 1058, 1000, 8874, -12118, 1004, 8192, -12118, 2,
37, 1058, 1000, 8874, -12118, 1004, 8192, -12118, 38, 1058,
900, 8874, -11008, 1000, 8874, -12118, 1, 38, 802, 1012,
7168, -12118, 904, 7168, -11008, 3, 35, 802, 952, 7616,
-11606, 956, 7616, -11691, 35, 1058, 956, 7616, -11691, 960,
7765, -11691, 35, 1314, 960, 7765, -11691, 964, 7765, -11606,
3, 35, 34, 936, 8277, -11606, 940, 8277, -11691, 35,
290, 940, 8277, -11691, 944, 8426, -11691, 35, 546, 944,
8426, -11691, 948, 8426, -11606, 1, 38, 1058, 900, 8874,
-11008, 1000, 8874, -12118, 1, 38, 802, 1012, 7168, -12118,
904, 7168, -11008, 8, 35, 802, 952, 7616, -11606, 956,
7616, -11691, 35, 1314, 960, 7765, -11691, 964, 7765, -11606,
36, 1058, 964, 7765, -11606, 980, 7701, -11606, 36, 1314,
980, 7701, -11606, 952, 7616, -11606, 36, 1553, 980, 7701,
-11606, 984, 7701, -11520, 36, 1809, 984, 7701, -11520, 988,
7701, -11179, 38, 273, 936, 8277, -11606, 964, 7765, -11606,
38, 1297, 952, 7616, -11606, 912, 7616, -11094, 1, 38,
273, 936, 8277, -11606, 964, 7765, -11606, 8, 35, 34,
936, 8277, -11606, 940, 8277, -11691, 35, 546, 944, 8426,
-11691, 948, 8426, -11606, 36, 34, 948, 8426, -11606, 972,
8341, -11606, 36, 290, 972, 8341, -11606, 936, 8277, -11606,
37, 273, 992, 8341, -11520, 972, 8341, -11606, 37, 785,
996, 8341, -11179, 992, 8341, -11520, 38, 273, 936, 8277,
-11606, 964, 7765, -11606, 38, 1553, 932, 8426, -11094, 948,
8426, -11606, 1, 38, 1058, 900, 8874, -11008, 1000, 8874,
-12118, 2, 34, 802, 904, 7168, -11008, 908, 7509, -11008,
38, 802, 1012, 7168, -12118, 904, 7168, -11008, 10, 34,
802, 904, 7168, -11008, 908, 7509, -11008, 34, 1058, 908,
7509, -11008, 912, 7616, -11094, 34, 1314, 916, 7765, -11094,
920, 7765, -11008, 35, 1570, 912, 7616, -11094, 968, 7701,
-11094, 35, 1826, 968, 7701, -11094, 916, 7765, -11094, 36,
1809, 984, 7701, -11520, 988, 7701, -11179, 37, 17, 988,
7701, -11179, 968, 7701, -11094, 38, 17, 916, 7765, -11094,
928, 8277, -11094, 38, 1297, 952, 7616, -11606, 912, 7616,
-11094, 41, 1058, 920, 7765, -11008, 1088, 7850, -10752, 1,
38, 17, 916, 7765, -11094, 928, 8277, -11094, 9, 34,
1570, 924, 8277, -11008, 928, 8277, -11094, 34, 1826, 932,
8426, -11094, 896, 8533, -11008, 36, 546, 928, 8277, -11094,
976, 8341, -11094, 36, 802, 976, 8341, -11094, 932, 8426,
-11094, 37, 529, 976, 8341, -11094, 996, 8341, -11179, 37,
785, 996, 8341, -11179, 992, 8341, -11520, 38, 17, 916,
7765, -11094, 928, 8277, -11094, 38, 1553, 932, 8426, -11094,
948, 8426, -11606, 41, 1826, 1092, 8192, -10752, 924, 8277,
-11008, 2, 34, 546, 896, 8533, -11008, 900, 8874, -11008,
34, 1826, 932, 8426, -11094, 896, 8533, -11008, 2, 34,
546, 896, 8533, -11008, 900, 8874, -11008, 38, 1058, 900,
8874, -11008, 1000, 8874, -12118, 2, 26, 802, 744, 5973,
-10923, 748, 5632, -10752, 32, 290, 748, 5632, -10752, 856,
5461, -10454, 4, 26, 802, 744, 5973, -10923, 748, 5632,
-10752, 27, 529, 744, 5973, -10923, 752, 5973, -10582, 54,
802, 1436, 6250, -10923, 1440, 6037, -10923, 54, 1058, 1440,
6037, -10923, 744, 5973, -10923, 4, 27, 785, 756, 6314,
-10582, 760, 6314, -10923, 31, 1058, 844, 7125, -10454, 760,
6314, -10923, 54, 546, 760, 6314, -10923, 1436, 6250, -10923,
54, 802, 1436, 6250, -10923, 1440, 6037, -10923, 1, 31,
1058, 844, 7125, -10454, 760, 6314, -10923, 1, 31, 1058,
844, 7125, -10454, 760, 6314, -10923, 1, 41, 1058, 920,
7765, -11008, 1088, 7850, -10752, 7, 30, 1314, 832, 7850,
-10667, 828, 7850, -10411, 38, 529, 660, 8192, -10667, 832,
7850, -10667, 41, 546, 1080, 7850, -10710, 832, 7850, -10667,
41, 1058, 920, 7765, -11008, 1088, 7850, -10752, 41, 1314,
1088, 7850, -10752, 1080, 7850, -10710, 42, 17, 1084, 8192,
-10710, 1080, 7850, -10710, 42, 273, 1088, 7850, -10752, 1092,
8192, -10752, 7, 23, 546, 652, 8192, -9728, 660, 8192,
-10667, 38, 529, 660, 8192, -10667, 832, 7850, -10667, 41,
802, 660, 8192, -10667, 1084, 8192, -10710, 41, 1570, 1084,
8192, -10710, 1092, 8192, -10752, 41, 1826, 1092, 8192, -10752,
924, 8277, -11008, 42, 17, 1084, 8192, -10710, 1080, 7850,
-10710, 42, 273, 1088, 7850, -10752, 1092, 8192, -10752, 6,
25, 1826, 728, 5461, -10326, 724, 5461, -10240, 26, 34,
732, 5461, -10411, 728, 5461, -10326, 27, 17, 728, 5461,
-10326, 736, 5802, -10326, 27, 273, 732, 5461, -10411, 740,
5802, -10411, 32, 290, 748, 5632, -10752, 856, 5461, -10454,
32, 546, 856, 5461, -10454, 732, 5461, -10411, 9, 26,
290, 712, 5802, -10240, 736, 5802, -10326, 26, 546, 736,
5802, -10326, 740, 5802, -10411, 27, 17, 728, 5461, -10326,
736, 5802, -10326, 27, 273, 732, 5461, -10411, 740, 5802,
-10411, 27, 529, 744, 5973, -10923, 752, 5973, -10582, 31,
1826, 740, 5802, -10411, 852, 5802, -10454, 32, 34, 852,
5802, -10454, 752, 5973, -10582, 54, 1314, 752, 5973, -10582,
1444, 6037, -10582, 54, 1570, 1444, 6037, -10582, 1448, 6250,
-10582, 4, 27, 785, 756, 6314, -10582, 760, 6314, -10923,
31, 1314, 756, 6314, -10582, 848, 6976, -10112, 54, 1570,
1444, 6037, -10582, 1448, 6250, -10582, 54, 1826, 1448, 6250,
-10582, 756, 6314, -10582, 1, 31, 1314, 756, 6314, -10582,
848, 6976, -10112, 3, 28, 529, 812, 7018, -10112, 816,
7168, -10454, 31, 802, 816, 7168, -10454, 844, 7125, -10454,
31, 1058, 844, 7125, -10454, 760, 6314, -10923, 9, 27,
1809, 788, 7424, -10070, 792, 7424, -10411, 28, 17, 796,
7338, -10070, 800, 7338, -10411, 28, 273, 804, 7168, -10070,
808, 7253, -10411, 28, 529, 812, 7018, -10112, 816, 7168,
-10454, 28, 1058, 808, 7253, -10411, 816, 7168, -10454, 29,
1058, 784, 7509, -10411, 792, 7424, -10411, 29, 1314, 792,
7424, -10411, 800, 7338, -10411, 29, 1570, 800, 7338, -10411,
808, 7253, -10411, 31, 802, 816, 7168, -10454, 844, 7125,
-10454, 9, 27, 1041, 764, 7680, -10070, 768, 7680, -10411,
27, 1297, 772, 7594, -10070, 776, 7594, -10411, 27, 1553,
780, 7509, -10070, 784, 7509, -10411, 29, 546, 768, 7680,
-10411, 776, 7594, -10411, 29, 802, 776, 7594, -10411, 784,
7509, -10411, 29, 1058, 784, 7509, -10411, 792, 7424, -10411,
30, 34, 824, 7765, -10411, 768, 7680, -10411, 30, 290,
828, 7850, -10411, 824, 7765, -10411, 30, 1041, 820, 7765,
-10070, 824, 7765, -10411, 3, 30, 290, 828, 7850, -10411,
824, 7765, -10411, 30, 785, 828, 7850, -10411, 664, 7850,
-10070, 30, 1314, 832, 7850, -10667, 828, 7850, -10411, 1,
23, 546, 652, 8192, -9728, 660, 8192, -10667, 3, 52,
1826, 1400, 8533, -10582, 1404, 8533, -9984, 53, 1570, 1396,
8874, -10582, 1428, 8810, -10582, 53, 1826, 1428, 8810, -10582,
1400, 8533, -10582, 3, 52, 1570, 1392, 9386, -10411, 1396,
8874, -10582, 53, 273, 1396, 8874, -10582, 1408, 8874, -9984,
53, 1570, 1396, 8874, -10582, 1428, 8810, -10582, 4, 52,
1570, 1392, 9386, -10411, 1396, 8874, -10582, 57, 785, 1460,
9386, -10240, 1392, 9386, -10411, 58, 802, 1468, 9557, -10411,
1536, 9429, -10411, 58, 1058, 1536, 9429, -10411, 1392, 9386,
-10411, 4, 55, 1314, 1464, 9813, -10411, 1468, 9557, -10411,
55, 1570, 1472, 9984, -10155, 1464, 9813, -10411, 57, 1041,
1476, 9557, -10240, 1468, 9557, -10411, 58, 802, 1468, 9557,
-10411, 1536, 9429, -10411, 1, 55, 1570, 1472, 9984, -10155,
1464, 9813, -10411, 1, 5, 1314, 172, 4458, -9984, 164,
4074, -9814, 2, 5, 1314, 172, 4458, -9984, 164, 4074,
-9814, 19, 1570, 516, 5290, -10070, 172, 4458, -9984, 1,
19, 1570, 516, 5290, -10070, 172, 4458, -9984, 3, 19,
1570, 516, 5290, -10070, 172, 4458, -9984, 25, 290, 696,
5376, -9814, 516, 5290, -10070, 31, 273, 840, 5290, -9728,
516, 5290, -10070, 7, 25, 1314, 716, 5461, -10155, 720,
5461, -10070, 25, 1570, 724, 5461, -10240, 716, 5461, -10155,
25, 1826, 728, 5461, -10326, 724, 5461, -10240, 26, 1297,
720, 5461, -10070, 704, 5802, -10070, 26, 1553, 716, 5461,
-10155, 708, 5802, -10155, 26, 1809, 724, 5461, -10240, 712,
5802, -10240, 55, 546, 720, 5461, -10070, 1456, 5461, -9878,
10, 19, 1314, 508, 6634, -9984, 512, 5973, -10070, 24,
1826, 512, 5973, -10070, 688, 5888, -9814, 25, 802, 704,
5802, -10070, 708, 5802, -10155, 25, 1058, 708, 5802, -10155,
712, 5802, -10240, 26, 290, 712, 5802, -10240, 736, 5802,
-10326, 26, 1297, 720, 5461, -10070, 704, 5802, -10070, 26,
1553, 716, 5461, -10155, 708, 5802, -10155, 26, 1809, 724,
5461, -10240, 712, 5802, -10240, 31, 529, 512, 5973, -10070,
836, 5973, -9728, 55, 290, 1452, 5802, -9878, 704, 5802,
-10070, 1, 19, 1314, 508, 6634, -9984, 512, 5973, -10070,
3, 19, 1058, 504, 7296, -9728, 508, 6634, -9984, 19,
1314, 508, 6634, -9984, 512, 5973, -10070, 31, 1314, 756,
6314, -10582, 848, 6976, -10112, 5, 19, 1058, 504, 7296,
-9728, 508, 6634, -9984, 28, 529, 812, 7018, -10112, 816,
7168, -10454, 28, 802, 812, 7018, -10112, 804, 7168, -10070,
31, 1314, 756, 6314, -10582, 848, 6976, -10112, 31, 1570,
848, 6976, -10112, 812, 7018, -10112, 7, 27, 1809, 788,
7424, -10070, 792, 7424, -10411, 28, 17, 796, 7338, -10070,
800, 7338, -10411, 28, 273, 804, 7168, -10070, 808, 7253,
-10411, 28, 802, 812, 7018, -10112, 804, 7168, -10070, 28,
1314, 804, 7168, -10070, 796, 7338, -10070, 28, 1570, 796,
7338, -10070, 788, 7424, -10070, 28, 1826, 788, 7424, -10070,
780, 7509, -10070, 9, 27, 1041, 764, 7680, -10070, 768,
7680, -10411, 27, 1297, 772, 7594, -10070, 776, 7594, -10411,
27, 1553, 780, 7509, -10070, 784, 7509, -10411, 28, 1826,
788, 7424, -10070, 780, 7509, -10070, 29, 34, 780, 7509,
-10070, 772, 7594, -10070, 29, 290, 772, 7594, -10070, 764,
7680, -10070, 29, 1826, 764, 7680, -10070, 820, 7765, -10070,
30, 546, 820, 7765, -10070, 664, 7850, -10070, 30, 1041,
820, 7765, -10070, 824, 7765, -10411, 3, 23, 802, 664,
7850, -10070, 656, 7850, -9728, 30, 546, 820, 7765, -10070,
664, 7850, -10070, 30, 785, 828, 7850, -10411, 664, 7850,
-10070, 1, 23, 546, 652, 8192, -9728, 660, 8192, -10667,
3, 52, 1826, 1400, 8533, -10582, 1404, 8533, -9984, 54,
34, 1404, 8533, -9984, 1432, 8810, -9984, 54, 290, 1432,
8810, -9984, 1408, 8874, -9984, 3, 53, 34, 1408, 8874,
-9984, 1412, 8960, -9728, 53, 273, 1396, 8874, -10582, 1408,
8874, -9984, 54, 290, 1432, 8810, -9984, 1408, 8874, -9984,
4, 55, 1058, 1420, 9386, -9558, 1460, 9386, -10240, 57,
785, 1460, 9386, -10240, 1392, 9386, -10411, 58, 290, 1460,
9386, -10240, 1532, 9429, -10240, 58, 546, 1532, 9429, -10240,
1476, 9557, -10240, 4, 55, 1826, 1476, 9557, -10240, 1480,
9642, -10070, 56, 34, 1480, 9642, -10070, 1484, 9472, -9472,
57, 1041, 1476, 9557, -10240, 1468, 9557, -10411, 58, 546,
1532, 9429, -10240, 1476, 9557, -10240, 2, 55, 1570, 1472,
9984, -10155, 1464, 9813, -10411, 56, 802, 1496, 9813, -9472,
1472, 9984, -10155, 1, 15, 802, 400, -1707, -9728, 404,
-2048, -9387, 2, 15, 546, 396, 170, -9728, 400, -1707,
-9728, 15, 802, 400, -1707, -9728, 404, -2048, -9387, 1,
15, 546, 396, 170, -9728, 400, -1707, -9728, 1, 15,
546, 396, 170, -9728, 400, -1707, -9728, 2, 15, 546,
396, 170, -9728, 400, -1707, -9728, 15, 1809, 396, 170,
-9728, 416, -1707, -9387, 2, 15, 546, 396, 170, -9728,
400, -1707, -9728, 15, 1809, 396, 170, -9728, 416, -1707,
-9387, 3, 15, 290, 380, 170, -9387, 396, 170, -9728,
15, 546, 396, 170, -9728, 400, -1707, -9728, 15, 1809,
396, 170, -9728, 416, -1707, -9387, 1, 0, 1570, 20,
2560, -9728, 28, 2218, -9472, 2, 0, 1058, 8, 2730,
-9728, 20, 2560, -9728, 0, 1570, 20, 2560, -9728, 28,
2218, -9472, 5, 0, 290, 4, 2730, -9814, 8, 2730,
-9728, 0, 546, 12, 2901, -9728, 0, 2901, -9814, 0,
802, 16, 3072, -9728, 12, 2901, -9728, 0, 1058, 8,
2730, -9728, 20, 2560, -9728, 5, 1553, 12, 2901, -9728,
8, 2730, -9728, 2, 0, 802, 16, 3072, -9728, 12,
2901, -9728, 0, 1314, 24, 3413, -9472, 16, 3072, -9728,
3, 5, 1058, 164, 4074, -9814, 168, 3669, -9728, 31,
17, 840, 5290, -9728, 168, 3669, -9728, 59, 546, 168,
3669, -9728, 1552, 3669, -9387, 2, 5, 1058, 164, 4074,
-9814, 168, 3669, -9728, 31, 17, 840, 5290, -9728, 168,
3669, -9728, 3, 5, 1058, 164, 4074, -9814, 168, 3669,
-9728, 5, 1314, 172, 4458, -9984, 164, 4074, -9814, 31,
17, 840, 5290, -9728, 168, 3669, -9728, 1, 31, 17,
840, 5290, -9728, 168, 3669, -9728, 1, 31, 17, 840,
5290, -9728, 168, 3669, -9728, 5, 25, 290, 696, 5376,
-9814, 516, 5290, -10070, 25, 546, 700, 5461, -9814, 696,
5376, -9814, 30, 1809, 836, 5973, -9728, 840, 5290, -9728,
31, 17, 840, 5290, -9728, 168, 3669, -9728, 31, 273,
840, 5290, -9728, 516, 5290, -10070, 5, 25, 546, 700,
5461, -9814, 696, 5376, -9814, 26, 1041, 692, 5802, -9814,
700, 5461, -9814, 30, 1809, 836, 5973, -9728, 840, 5290,
-9728, 55, 546, 720, 5461, -10070, 1456, 5461, -9878, 55,
802, 1456, 5461, -9878, 700, 5461, -9814, 8, 24, 1826,
512, 5973, -10070, 688, 5888, -9814, 25, 34, 688, 5888,
-9814, 692, 5802, -9814, 26, 1041, 692, 5802, -9814, 700,
5461, -9814, 30, 1553, 504, 7296, -9728, 836, 5973, -9728,
30, 1809, 836, 5973, -9728, 840, 5290, -9728, 31, 529,
512, 5973, -10070, 836, 5973, -9728, 55, 34, 692, 5802,
-9814, 1452, 5802, -9878, 55, 290, 1452, 5802, -9878, 704,
5802, -10070, 1, 30, 1553, 504, 7296, -9728, 836, 5973,
-9728, 2, 19, 1058, 504, 7296, -9728, 508, 6634, -9984,
30, 1553, 504, 7296, -9728, 836, 5973, -9728, 2, 19,
1058, 504, 7296, -9728, 508, 6634, -9984, 30, 1553, 504,
7296, -9728, 836, 5973, -9728, 4, 19, 1058, 504, 7296,
-9728, 508, 6634, -9984, 21, 1826, 612, 7296, -8960, 504,
7296, -9728, 23, 290, 656, 7850, -9728, 604, 7338, -9558,
30, 1553, 504, 7296, -9728, 836, 5973, -9728, 1, 23,
290, 656, 7850, -9728, 604, 7338, -9558, 3, 23, 290,
656, 7850, -9728, 604, 7338, -9558, 23, 802, 664, 7850,
-10070, 656, 7850, -9728, 23, 1553, 628, 7850, -9430, 652,
8192, -9728, 3, 23, 34, 648, 8704, -9643, 652, 8192,
-9728, 23, 546, 652, 8192, -9728, 660, 8192, -10667, 23,
1553, 628, 7850, -9430, 652, 8192, -9728, 3, 23, 34,
648, 8704, -9643, 652, 8192, -9728, 24, 529, 648, 8704,
-9643, 668, 8277, -9472, 24, 802, 672, 8938, -9515, 648,
8704, -9643, 2, 53, 34, 1408, 8874, -9984, 1412, 8960,
-9728, 53, 1058, 1412, 8960, -9728, 1424, 8938, -9579, 2,
55, 1058, 1420, 9386, -9558, 1460, 9386, -10240, 56, 34,
1480, 9642, -10070, 1484, 9472, -9472, 2, 56, 34, 1480,
9642, -10070, 1484, 9472, -9472, 56, 802, 1496, 9813, -9472,
1472, 9984, -10155, 1, 56, 802, 1496, 9813, -9472, 1472,
9984, -10155, 2, 15, 802, 400, -1707, -9728, 404, -2048,
-9387, 15, 1058, 404, -2048, -9387, 408, -2048, -7851, 2,
15, 1809, 396, 170, -9728, 416, -1707, -9387, 16, 17,
416, -1707, -9387, 420, -1707, -7851, 1, 15, 1809, 396,
170, -9728, 416, -1707, -9387, 1, 15, 1809, 396, 170,
-9728, 416, -1707, -9387, 1, 15, 1809, 396, 170, -9728,
416, -1707, -9387, 2, 13, 1570, 376, -256, -8918, 380,
170, -9387, 15, 290, 380, 170, -9387, 396, 170, -9728,
2, 1, 34, 28, 2218, -9472, 36, 1877, -9472, 6,
1058, 36, 1877, -9472, 176, 1877, -8960, 2, 0, 1570,
20, 2560, -9728, 28, 2218, -9472, 1, 34, 28, 2218,
-9472, 36, 1877, -9472, 1, 0, 1570, 20, 2560, -9728,
28, 2218, -9472, 1, 0, 1314, 24, 3413, -9472, 16,
3072, -9728, 7, 0, 1314, 24, 3413, -9472, 16, 3072,
-9728, 0, 1826, 32, 3584, -9472, 24, 3413, -9472, 58,
1570, 1544, 3584, -9387, 32, 3584, -9472, 58, 1809, 1540,
3584, -9046, 1544, 3584, -9387, 59, 290, 1552, 3669, -9387,
1544, 3584, -9387, 59, 546, 168, 3669, -9728, 1552, 3669,
-9387, 59, 1041, 1552, 3669, -9387, 1548, 3669, -9046, 4,
21, 1570, 604, 7338, -9558, 608, 7338, -8960, 21, 1826,
612, 7296, -8960, 504, 7296, -9728, 23, 290, 656, 7850,
-9728, 604, 7338, -9558, 24, 17, 628, 7850, -9430, 604,
7338, -9558, 2, 23, 1297, 608, 7338, -8960, 628, 7850,
-9430, 24, 17, 628, 7850, -9430, 604, 7338, -9558, 4,
22, 785, 624, 8298, -8960, 628, 7850, -9430, 23, 1297,
608, 7338, -8960, 628, 7850, -9430, 23, 1553, 628, 7850,
-9430, 652, 8192, -9728, 24, 17, 628, 7850, -9430, 604,
7338, -9558, 2, 24, 273, 668, 8277, -9472, 636, 8746,
-8854, 24, 529, 648, 8704, -9643, 668, 8277, -9472, 4,
24, 529, 648, 8704, -9643, 668, 8277, -9472, 24, 802,
672, 8938, -9515, 648, 8704, -9643, 24, 1297, 680, 9088,
-9152, 684, 8832, -9323, 24, 1553, 684, 8832, -9323, 672,
8938, -9515, 6, 24, 802, 672, 8938, -9515, 648, 8704,
-9643, 24, 1297, 680, 9088, -9152, 684, 8832, -9323, 24,
1553, 684, 8832, -9323, 672, 8938, -9515, 33, 529, 872,
9194, -9387, 672, 8938, -9515, 53, 1058, 1412, 8960, -9728,
1424, 8938, -9579, 53, 1314, 1424, 8938, -9579, 672, 8938,
-9515, 7, 33, 290, 676, 9258, -9152, 872, 9194, -9387,
33, 529, 872, 9194, -9387, 672, 8938, -9515, 53, 546,
872, 9194, -9387, 1416, 9258, -9387, 53, 802, 1416, 9258,
-9387, 1420, 9386, -9558, 55, 1058, 1420, 9386, -9558, 1460,
9386, -10240, 56, 34, 1480, 9642, -10070, 1484, 9472, -9472,
56, 290, 1484, 9472, -9472, 1488, 9472, -9046, 2, 56,
802, 1496, 9813, -9472, 1472, 9984, -10155, 56, 1058, 1500,
9813, -9046, 1496, 9813, -9472, 1, 15, 1058, 404, -2048,
-9387, 408, -2048, -7851, 1, 16, 17, 416, -1707, -9387,
420, -1707, -7851, 2, 13, 802, 360, -896, -8790, 372,
-896, -8918, 13, 1058, 372, -896, -8918, 348, -683, -8918,
3, 12, 1314, 348, -683, -8918, 320, -683, -8875, 13,
1058, 372, -896, -8918, 348, -683, -8918, 14, 785, 348,
-683, -8918, 356, -342, -8918, 5, 11, 1570, 304, 128,
-9046, 308, -171, -8875, 12, 1570, 352, -342, -8875, 356,
-342, -8918, 13, 1314, 356, -342, -8918, 376, -256, -8918,
13, 1570, 376, -256, -8918, 380, 170, -9387, 14, 785,
348, -683, -8918, 356, -342, -8918, 5, 11, 802, 224,
341, -9216, 300, 170, -9046, 11, 1041, 300, 170, -9046,
240, 341, -8704, 11, 1314, 300, 170, -9046, 304, 128,
-9046, 11, 1570, 304, 128, -9046, 308, -171, -8875, 13,
1570, 376, -256, -8918, 380, 170, -9387, 2, 8, 546,
220, 1194, -9216, 224, 341, -9216, 11, 802, 224, 341,
-9216, 300, 170, -9046, 4, 8, 546, 220, 1194, -9216,
224, 341, -9216, 17, 529, 448, 682, -9046, 452, 853,
-9046, 17, 785, 452, 853, -9046, 456, 853, -8875, 17,
1297, 460, 682, -8875, 448, 682, -9046, 2, 8, 290,
216, 1322, -8875, 220, 1194, -9216, 8, 546, 220, 1194,
-9216, 224, 341, -9216, 1, 7, 1058, 204, 1813, -8960,
184, 1365, -8875, 4, 6, 1058, 36, 1877, -9472, 176,
1877, -8960, 6, 1553, 180, 1877, -8278, 176, 1877, -8960,
7, 802, 176, 1877, -8960, 204, 1813, -8960, 7, 1058,
204, 1813, -8960, 184, 1365, -8875, 8, 1, 290, 40,
2389, -9046, 44, 2474, -9046, 1, 546, 44, 2474, -9046,
48, 2474, -8960, 1, 802, 48, 2474, -8960, 52, 2389,
-8960, 1, 1058, 52, 2389, -8960, 40, 2389, -9046, 4,
1297, 56, 3157, -9046, 44, 2474, -9046, 6, 17, 68,
3157, -8960, 48, 2474, -8960, 6, 273, 48, 2474, -8960,
80, 2474, -8278, 6, 529, 52, 2389, -8960, 76, 2389,
-8278, 2, 4, 1297, 56, 3157, -9046, 44, 2474, -9046,
6, 17, 68, 3157, -8960, 48, 2474, -8960, 7, 1,
1314, 56, 3157, -9046, 60, 3242, -9046, 1, 1570, 60,
3242, -9046, 64, 3242, -8960, 1, 1826, 64, 3242, -8960,
68, 3157, -8960, 2, 34, 68, 3157, -8960, 56, 3157,
-9046, 4, 1297, 56, 3157, -9046, 44, 2474, -9046, 4,
1553, 108, 3584, -8960, 60, 3242, -9046, 6, 17, 68,
3157, -8960, 48, 2474, -8960, 9, 3, 273, 104, 3584,
-8704, 108, 3584, -8960, 3, 1553, 132, 3669, -8960, 136,
3669, -8704, 4, 34, 132, 3669, -8960, 108, 3584, -8960,
4, 1553, 108, 3584, -8960, 60, 3242, -9046, 58, 1314,
108, 3584, -8960, 1540, 3584, -9046, 58, 1809, 1540, 3584,
-9046, 1544, 3584, -9387, 59, 34, 1540, 3584, -9046, 1548,
3669, -9046, 59, 802, 1548, 3669, -9046, 132, 3669, -8960,
59, 1041, 1552, 3669, -9387, 1548, 3669, -9046, 2, 20,
1553, 552, 4757, -9195, 556, 4117, -9024, 20, 1809, 556,
4117, -9024, 528, 4053, -8448, 1, 20, 1553, 552, 4757,
-9195, 556, 4117, -9024, 2, 20, 1297, 548, 5504, -9088,
552, 4757, -9195, 20, 1553, 552, 4757, -9195, 556, 4117,
-9024, 1, 20, 1297, 548, 5504, -9088, 552, 4757, -9195,
2, 20, 1041, 544, 5674, -8726, 548, 5504, -9088, 20,
1297, 548, 5504, -9088, 552, 4757, -9195, 6, 21, 1570,
604, 7338, -9558, 608, 7338, -8960, 21, 1826, 612, 7296,
-8960, 504, 7296, -9728, 22, 34, 608, 7338, -8960, 612,
7296, -8960, 23, 1297, 608, 7338, -8960, 628, 7850, -9430,
33, 1809, 608, 7338, -8960, 892, 7338, -8299, 34, 17,
888, 7296, -8299, 612, 7296, -8960, 1, 23, 1297, 608,
7338, -8960, 628, 7850, -9430, 2, 22, 785, 624, 8298,
-8960, 628, 7850, -9430, 22, 1041, 632, 7509, -8619, 624,
8298, -8960, 3, 22, 785, 624, 8298, -8960, 628, 7850,
-9430, 22, 1041, 632, 7509, -8619, 624, 8298, -8960, 24,
273, 668, 8277, -9472, 636, 8746, -8854, 1, 24, 273,
668, 8277, -9472, 636, 8746, -8854, 3, 24, 1041, 676,
9258, -9152, 680, 9088, -9152, 24, 1297, 680, 9088, -9152,
684, 8832, -9323, 33, 34, 868, 9066, -8406, 676, 9258,
-9152, 5, 24, 1041, 676, 9258, -9152, 680, 9088, -9152,
33, 34, 868, 9066, -8406, 676, 9258, -9152, 33, 290,
676, 9258, -9152, 872, 9194, -9387, 56, 290, 1484, 9472,
-9472, 1488, 9472, -9046, 56, 546, 1488, 9472, -9046, 1492,
9728, -8704, 3, 56, 546, 1488, 9472, -9046, 1492, 9728,
-8704, 56, 1058, 1500, 9813, -9046, 1496, 9813, -9472, 56,
1314, 1504, 10154, -8704, 1500, 9813, -9046, 1, 56, 1314,
1504, 10154, -8704, 1500, 9813, -9046, 1, 15, 1058, 404,
-2048, -9387, 408, -2048, -7851, 1, 16, 17, 416, -1707,
-9387, 420, -1707, -7851, 5, 12, 290, 320, -683, -8875,
324, -854, -8790, 12, 1826, 324, -854, -8790, 360, -896,
-8790, 13, 802, 360, -896, -8790, 372, -896, -8918, 14,
1553, 324, -854, -8790, 328, -854, -8448, 14, 1809, 364,
-896, -8448, 360, -896, -8790, 10, 12, 290, 320, -683,
-8875, 324, -854, -8790, 12, 1314, 348, -683, -8918, 320,
-683, -8875, 14, 529, 352, -342, -8875, 320, -683, -8875,
43, 1041, 1128, -640, -8704, 1132, -555, -8704, 43, 1297,
1132, -555, -8704, 1136, -512, -8662, 43, 1553, 1136, -512,
-8662, 1140, -512, -8576, 43, 1809, 1140, -512, -8576, 1144,
-555, -8534, 44, 273, 1148, -640, -8534, 1152, -683, -8576,
44, 529, 1152, -683, -8576, 1156, -683, -8662, 44, 785,
1156, -683, -8662, 1128, -640, -8704, 6, 11, 1570, 304,
128, -9046, 308, -171, -8875, 12, 1570, 352, -342, -8875,
356, -342, -8918, 14, 273, 312, -171, -8363, 308, -171,
-8875, 14, 529, 352, -342, -8875, 320, -683, -8875, 16,
1058, 308, -171, -8875, 428, -235, -8875, 16, 1314, 428,
-235, -8875, 352, -342, -8875, 1, 11, 1041, 300, 170,
-9046, 240, 341, -8704, 9, 8, 1553, 240, 341, -8704,
244, 426, -8704, 8, 1809, 244, 426, -8704, 248, 512,
-8704, 9, 17, 248, 512, -8704, 252, 597, -8704, 9,
273, 252, 597, -8704, 256, 682, -8704, 10, 785, 244,
426, -8704, 288, 426, -8534, 10, 1041, 248, 512, -8704,
284, 512, -8534, 10, 1297, 252, 597, -8704, 280, 597,
-8534, 11, 17, 240, 341, -8704, 292, 341, -8534, 11,
1041, 300, 170, -9046, 240, 341, -8704, 9, 9, 273,
252, 597, -8704, 256, 682, -8704, 9, 529, 256, 682,
-8704, 260, 768, -8704, 9, 785, 260, 768, -8704, 264,
853, -8704, 9, 1041, 264, 853, -8704, 268, 853, -8534,
10, 1553, 256, 682, -8704, 276, 682, -8534, 10, 1809,
260, 768, -8704, 272, 768, -8534, 17, 785, 452, 853,
-9046, 456, 853, -8875, 17, 1041, 456, 853, -8875, 460,
682, -8875, 17, 1297, 460, 682, -8875, 448, 682, -9046,
4, 7, 1297, 208, 1322, -8427, 212, 1322, -8811, 7,
1570, 188, 1365, -8811, 212, 1322, -8811, 8, 34, 212,
1322, -8811, 216, 1322, -8875, 8, 290, 216, 1322, -8875,
220, 1194, -9216, 3, 6, 1826, 184, 1365, -8875, 188,
1365, -8811, 7, 1058, 204, 1813, -8960, 184, 1365, -8875,
7, 1570, 188, 1365, -8811, 212, 1322, -8811, 1, 6,
1553, 180, 1877, -8278, 176, 1877, -8960, 2, 6, 273,
48, 2474, -8960, 80, 2474, -8278, 6, 529, 52, 2389,
-8960, 76, 2389, -8278, 5, 3, 273, 104, 3584, -8704,
108, 3584, -8960, 3, 546, 112, 3584, -8534, 104, 3584,
-8704, 3, 1553, 132, 3669, -8960, 136, 3669, -8704, 3,
1826, 104, 3584, -8704, 136, 3669, -8704, 4, 802, 136,
3669, -8704, 124, 3669, -8534, 1, 20, 1809, 556, 4117,
-9024, 528, 4053, -8448, 1, 20, 1809, 556, 4117, -9024,
528, 4053, -8448, 2, 20, 785, 540, 5440, -8384, 544,
5674, -8726, 20, 1041, 544, 5674, -8726, 548, 5504, -9088,
2, 33, 1809, 608, 7338, -8960, 892, 7338, -8299, 34,
17, 888, 7296, -8299, 612, 7296, -8960, 2, 22, 1041,
632, 7509, -8619, 624, 8298, -8960, 22, 1553, 644, 7936,
-8192, 632, 7509, -8619, 2, 22, 1041, 632, 7509, -8619,
624, 8298, -8960, 22, 1297, 636, 8746, -8854, 640, 7957,
-8534, 1, 22, 1297, 636, 8746, -8854, 640, 7957, -8534,
2, 22, 1297, 636, 8746, -8854, 640, 7957, -8534, 24,
273, 668, 8277, -9472, 636, 8746, -8854, 1, 33, 34,
868, 9066, -8406, 676, 9258, -9152, 1, 33, 34, 868,
9066, -8406, 676, 9258, -9152, 2, 56, 546, 1488, 9472,
-9046, 1492, 9728, -8704, 56, 1570, 1492, 9728, -8704, 1508,
9322, -8086, 2, 56, 1314, 1504, 10154, -8704, 1500, 9813,
-9046, 56, 1826, 1512, 9557, -7680, 1504, 10154, -8704, 1,
15, 1058, 404, -2048, -9387, 408, -2048, -7851, 1, 16,
17, 416, -1707, -9387, 420, -1707, -7851, 6, 12, 546,
328, -854, -8448, 332, -683, -8363, 13, 34, 364, -896,
-8448, 328, -854, -8448, 13, 290, 344, -683, -8320, 368,
-896, -8320, 13, 546, 368, -896, -8320, 364, -896, -8448,
14, 1553, 324, -854, -8790, 328, -854, -8448, 14, 1809,
364, -896, -8448, 360, -896, -8790, 8, 12, 546, 328,
-854, -8448, 332, -683, -8363, 12, 1058, 332, -683, -8363,
344, -683, -8320, 13, 290, 344, -683, -8320, 368, -896,
-8320, 14, 1041, 332, -683, -8363, 340, -342, -8363, 14,
1297, 336, -342, -8320, 344, -683, -8320, 43, 1809, 1140,
-512, -8576, 1144, -555, -8534, 44, 17, 1144, -555, -8534,
1148, -640, -8534, 44, 273, 1148, -640, -8534, 1152, -683,
-8576, 9, 11, 1826, 312, -171, -8363, 316, 128, -8192,
12, 802, 336, -342, -8320, 340, -342, -8363, 13, 1826,
384, -256, -8320, 336, -342, -8320, 14, 34, 388, 170,
-7851, 384, -256, -8320, 14, 273, 312, -171, -8363, 308,
-171, -8875, 14, 1041, 332, -683, -8363, 340, -342, -8363,
14, 1297, 336, -342, -8320, 344, -683, -8320, 16, 546,
340, -342, -8363, 424, -235, -8363, 16, 802, 424, -235,
-8363, 312, -171, -8363, 2, 11, 273, 292, 341, -8534,
296, 170, -8192, 11, 1826, 312, -171, -8363, 316, 128,
-8192, 9, 9, 1809, 276, 682, -8534, 280, 597, -8534,
10, 17, 280, 597, -8534, 284, 512, -8534, 10, 273,
284, 512, -8534, 288, 426, -8534, 10, 529, 288, 426,
-8534, 292, 341, -8534, 10, 785, 244, 426, -8704, 288,
426, -8534, 10, 1041, 248, 512, -8704, 284, 512, -8534,
10, 1297, 252, 597, -8704, 280, 597, -8534, 11, 17,
240, 341, -8704, 292, 341, -8534, 11, 273, 292, 341,
-8534, 296, 170, -8192, 9, 9, 1041, 264, 853, -8704,
268, 853, -8534, 9, 1297, 268, 853, -8534, 272, 768,
-8534, 9, 1553, 272, 768, -8534, 276, 682, -8534, 9,
1809, 276, 682, -8534, 280, 597, -8534, 10, 1553, 256,
682, -8704, 276, 682, -8534, 10, 1809, 260, 768, -8704,
272, 768, -8534, 16, 1553, 432, 682, -8363, 436, 853,
-8363, 16, 1809, 436, 853, -8363, 440, 853, -8192, 17,
273, 444, 682, -8192, 432, 682, -8363, 4, 7, 1297,
208, 1322, -8427, 212, 1322, -8811, 7, 1826, 208, 1322,
-8427, 192, 1365, -8427, 8, 1058, 236, 1322, -8363, 208,
1322, -8427, 8, 1314, 232, 1194, -8022, 236, 1322, -8363,
3, 7, 34, 192, 1365, -8427, 196, 1365, -8363, 7,
290, 196, 1365, -8363, 200, 1813, -8278, 7, 1826, 208,
1322, -8427, 192, 1365, -8427, 4, 6, 1314, 180, 1877,
-8278, 144, 1877, -7851, 6, 1553, 180, 1877, -8278, 176,
1877, -8960, 7, 290, 196, 1365, -8363, 200, 1813, -8278,
7, 546, 200, 1813, -8278, 180, 1877, -8278, 6, 2,
290, 72, 2389, -8192, 76, 2389, -8278, 2, 546, 76,
2389, -8278, 80, 2474, -8278, 2, 802, 80, 2474, -8278,
84, 2474, -8192, 5, 1809, 80, 2474, -8278, 96, 3157,
-8278, 6, 273, 48, 2474, -8960, 80, 2474, -8278, 6,
529, 52, 2389, -8960, 76, 2389, -8278, 1, 5, 1809,
80, 2474, -8278, 96, 3157, -8278, 5, 2, 1570, 92,
3157, -8192, 96, 3157, -8278, 2, 1826, 96, 3157, -8278,
100, 3242, -8278, 3, 34, 100, 3242, -8278, 88, 3242,
-8192, 4, 1809, 88, 3242, -8192, 120, 3584, -8278, 5,
1809, 80, 2474, -8278, 96, 3157, -8278, 9, 3, 546,
112, 3584, -8534, 104, 3584, -8704, 3, 802, 116, 3584,
-7680, 120, 3584, -8278, 3, 1041, 120, 3584, -8278, 112,
3584, -8534, 3, 1297, 124, 3669, -8534, 128, 3669, -8278,
4, 290, 124, 3669, -8534, 112, 3584, -8534, 4, 546,
120, 3584, -8278, 128, 3669, -8278, 4, 802, 136, 3669,
-8704, 124, 3669, -8534, 4, 1058, 128, 3669, -8278, 140,
3669, -7851, 4, 1809, 88, 3242, -8192, 120, 3584, -8278,
2, 20, 17, 528, 4053, -8448, 532, 4458, -8278, 20,
1809, 556, 4117, -9024, 528, 4053, -8448, 1, 20, 17,
528, 4053, -8448, 532, 4458, -8278, 2, 20, 17, 528,
4053, -8448, 532, 4458, -8278, 20, 273, 532, 4458, -8278,
536, 5056, -8278, 2, 20, 273, 532, 4458, -8278, 536,
5056, -8278, 20, 529, 536, 5056, -8278, 540, 5440, -8384,
1, 20, 529, 536, 5056, -8278, 540, 5440, -8384, 2,
20, 529, 536, 5056, -8278, 540, 5440, -8384, 20, 785,
540, 5440, -8384, 544, 5674, -8726, 1, 33, 1058, 880,
6058, -8022, 884, 6485, -8299, 2, 33, 1058, 880, 6058,
-8022, 884, 6485, -8299, 33, 1314, 884, 6485, -8299, 888,
7296, -8299, 1, 33, 1314, 884, 6485, -8299, 888, 7296,
-8299, 5, 33, 1314, 884, 6485, -8299, 888, 7296, -8299,
33, 1570, 888, 7296, -8299, 892, 7338, -8299, 33, 1809,
608, 7338, -8960, 892, 7338, -8299, 34, 17, 888, 7296,
-8299, 612, 7296, -8960, 34, 290, 892, 7338, -8299, 596,
7338, -8128, 1, 22, 1553, 644, 7936, -8192, 632, 7509,
-8619, 3, 22, 1297, 636, 8746, -8854, 640, 7957, -8534,
22, 1553, 644, 7936, -8192, 632, 7509, -8619, 22, 1809,
640, 7957, -8534, 620, 8362, -8192, 1, 22, 1809, 640,
7957, -8534, 620, 8362, -8192, 4, 33, 34, 868, 9066,
-8406, 676, 9258, -9152, 57, 529, 616, 8810, -8107, 868,
9066, -8406, 57, 1826, 1516, 9216, -8086, 1528, 9109, -8406,
58, 34, 1528, 9109, -8406, 868, 9066, -8406, 1, 56,
1570, 1492, 9728, -8704, 1508, 9322, -8086, 2, 56, 1570,
1492, 9728, -8704, 1508, 9322, -8086, 56, 1826, 1512, 9557,
-7680, 1504, 10154, -8704, 1, 56, 1826, 1512, 9557, -7680,
1504, 10154, -8704, 1, 15, 1058, 404, -2048, -9387, 408,
-2048, -7851, 1, 16, 17, 416, -1707, -9387, 420, -1707,
-7851, 1, 14, 34, 388, 170, -7851, 384, -256, -8320,
5, 11, 273, 292, 341, -8534, 296, 170, -8192, 11,
546, 296, 170, -8192, 228, 341, -8022, 11, 1826, 312,
-171, -8363, 316, 128, -8192, 12, 34, 316, 128, -8192,
296, 170, -8192, 14, 34, 388, 170, -7851, 384, -256,
-8320, 2, 8, 802, 228, 341, -8022, 232, 1194, -8022,
11, 546, 296, 170, -8192, 228, 341, -8022, 4, 8,
802, 228, 341, -8022, 232, 1194, -8022, 16, 1809, 436,
853, -8363, 440, 853, -8192, 17, 17, 440, 853, -8192,
444, 682, -8192, 17, 273, 444, 682, -8192, 432, 682,
-8363, 2, 8, 802, 228, 341, -8022, 232, 1194, -8022,
8, 1314, 232, 1194, -8022, 236, 1322, -8363, 1, 6,
1314, 180, 1877, -8278, 144, 1877, -7851, 4, 2, 290,
72, 2389, -8192, 76, 2389, -8278, 2, 802, 80, 2474,
-8278, 84, 2474, -8192, 2, 1058, 84, 2474, -8192, 72,
2389, -8192, 6, 785, 84, 2474, -8192, 92, 3157, -8192,
1, 6, 785, 84, 2474, -8192, 92, 3157, -8192, 5,
2, 1314, 88, 3242, -8192, 92, 3157, -8192, 2, 1570,
92, 3157, -8192, 96, 3157, -8278, 3, 34, 100, 3242,
-8278, 88, 3242, -8192, 4, 1809, 88, 3242, -8192, 120,
3584, -8278, 6, 785, 84, 2474, -8192, 92, 3157, -8192,
2, 3, 802, 116, 3584, -7680, 120, 3584, -8278, 4,
1058, 128, 3669, -8278, 140, 3669, -7851, 2, 33, 802,
876, 5973, -7787, 880, 6058, -8022, 33, 1058, 880, 6058,
-8022, 884, 6485, -8299, 1, 33, 1058, 880, 6058, -8022,
884, 6485, -8299, 4, 21, 1314, 596, 7338, -8128, 600,
7338, -7766, 23, 1809, 596, 7338, -8128, 588, 8128, -7680,
32, 785, 596, 7338, -8128, 644, 7936, -8192, 34, 290,
892, 7338, -8299, 596, 7338, -8128, 2, 23, 1809, 596,
7338, -8128, 588, 8128, -7680, 32, 785, 596, 7338, -8128,
644, 7936, -8192, 3, 22, 290, 592, 8128, -7851, 616,
8810, -8107, 22, 1553, 644, 7936, -8192, 632, 7509, -8619,
32, 785, 596, 7338, -8128, 644, 7936, -8192, 3, 22,
290, 592, 8128, -7851, 616, 8810, -8107, 22, 529, 620,
8362, -8192, 616, 8810, -8107, 22, 1809, 640, 7957, -8534,
620, 8362, -8192, 4, 22, 290, 592, 8128, -7851, 616,
8810, -8107, 22, 529, 620, 8362, -8192, 616, 8810, -8107,
57, 529, 616, 8810, -8107, 868, 9066, -8406, 57, 1314,
616, 8810, -8107, 1524, 8853, -8107, 4, 57, 529, 616,
8810, -8107, 868, 9066, -8406, 57, 1314, 616, 8810, -8107,
1524, 8853, -8107, 57, 1570, 1524, 8853, -8107, 1520, 8960,
-7680, 57, 1826, 1516, 9216, -8086, 1528, 9109, -8406, 3,
56, 1570, 1492, 9728, -8704, 1508, 9322, -8086, 57, 34,
1508, 9322, -8086, 1516, 9216, -8086, 57, 1826, 1516, 9216,
-8086, 1528, 9109, -8406, 1, 56, 1826, 1512, 9557, -7680,
1504, 10154, -8704, 2, 15, 1058, 404, -2048, -9387, 408,
-2048, -7851, 15, 1314, 408, -2048, -7851, 412, -1707, -7510,
3, 15, 1314, 408, -2048, -7851, 412, -1707, -7510, 16,
17, 416, -1707, -9387, 420, -1707, -7851, 16, 273, 420,
-1707, -7851, 392, 170, -7510, 1, 16, 273, 420, -1707,
-7851, 392, 170, -7510, 1, 16, 273, 420, -1707, -7851,
392, 170, -7510, 1, 16, 273, 420, -1707, -7851, 392,
170, -7510, 1, 16, 273, 420, -1707, -7851, 392, 170,
-7510, 3, 14, 34, 388, 170, -7851, 384, -256, -8320,
15, 34, 392, 170, -7510, 388, 170, -7851, 16, 273,
420, -1707, -7851, 392, 170, -7510, 2, 5, 34, 144,
1877, -7851, 148, 2218, -7851, 6, 1314, 180, 1877, -8278,
144, 1877, -7851, 2, 5, 34, 144, 1877, -7851, 148,
2218, -7851, 5, 290, 148, 2218, -7851, 152, 2581, -7680,
2, 5, 290, 148, 2218, -7851, 152, 2581, -7680, 5,
546, 152, 2581, -7680, 156, 3242, -7680, 1, 5, 546,
152, 2581, -7680, 156, 3242, -7680, 3, 5, 546, 152,
2581, -7680, 156, 3242, -7680, 17, 1570, 156, 3242, -7680,
464, 3328, -6742, 18, 529, 116, 3584, -7680, 156, 3242,
-7680, 5, 3, 802, 116, 3584, -7680, 120, 3584, -8278,
4, 1058, 128, 3669, -8278, 140, 3669, -7851, 5, 802,
140, 3669, -7851, 160, 3925, -7680, 17, 1826, 468, 3690,
-6912, 116, 3584, -7680, 18, 529, 116, 3584, -7680, 156,
3242, -7680, 2, 5, 802, 140, 3669, -7851, 160, 3925,
-7680, 59, 1570, 160, 3925, -7680, 1556, 4437, -7680, 1,
59, 1570, 160, 3925, -7680, 1556, 4437, -7680, 3, 59,
1570, 160, 3925, -7680, 1556, 4437, -7680, 59, 1809, 1556,
4437, -7680, 520, 4949, -7680, 60, 802, 1556, 4437, -7680,
1224, 4437, -7510, 2, 19, 1809, 520, 4949, -7680, 524,
5120, -7787, 59, 1809, 1556, 4437, -7680, 520, 4949, -7680,
2, 19, 1809, 520, 4949, -7680, 524, 5120, -7787, 60,
17, 524, 5120, -7787, 1560, 5802, -7787, 1, 60, 17,
524, 5120, -7787, 1560, 5802, -7787, 4, 33, 802, 876,
5973, -7787, 880, 6058, -8022, 60, 17, 524, 5120, -7787,
1560, 5802, -7787, 60, 290, 1560, 5802, -7787, 876, 5973,
-7787, 60, 546, 1196, 5802, -7510, 1560, 5802, -7787, 3,
21, 1314, 596, 7338, -8128, 600, 7338, -7766, 32, 1058,
584, 7680, -7680, 860, 7338, -7467, 43, 802, 600, 7338,
-7766, 580, 7680, -7766, 4, 21, 802, 580, 7680, -7766,
584, 7680, -7680, 23, 1809, 596, 7338, -8128, 588, 8128,
-7680, 32, 1058, 584, 7680, -7680, 860, 7338, -7467, 43,
802, 600, 7338, -7766, 580, 7680, -7766, 4, 21, 1058,
588, 8128, -7680, 592, 8128, -7851, 22, 290, 592, 8128,
-7851, 616, 8810, -8107, 23, 1809, 596, 7338, -8128, 588,
8128, -7680, 32, 1826, 864, 7893, -7083, 588, 8128, -7680,
1, 22, 290, 592, 8128, -7851, 616, 8810, -8107, 2,
57, 290, 1520, 8960, -7680, 1512, 9557, -7680, 57, 1570,
1524, 8853, -8107, 1520, 8960, -7680, 1, 57, 290, 1520,
8960, -7680, 1512, 9557, -7680, 2, 56, 1826, 1512, 9557,
-7680, 1504, 10154, -8704, 57, 290, 1520, 8960, -7680, 1512,
9557, -7680, 1, 15, 1314, 408, -2048, -7851, 412, -1707,
-7510, 2, 15, 1314, 408, -2048, -7851, 412, -1707, -7510,
15, 1570, 412, -1707, -7510, 392, 170, -7510, 1, 15,
1570, 412, -1707, -7510, 392, 170, -7510, 1, 15, 1570,
412, -1707, -7510, 392, 170, -7510, 1, 15, 1570, 412,
-1707, -7510, 392, 170, -7510, 1, 15, 1570, 412, -1707,
-7510, 392, 170, -7510, 3, 15, 34, 392, 170, -7510,
388, 170, -7851, 15, 1570, 412, -1707, -7510, 392, 170,
-7510, 16, 273, 420, -1707, -7851, 392, 170, -7510, 1,
17, 1570, 156, 3242, -7680, 464, 3328, -6742, 1, 17,
1826, 468, 3690, -6912, 116, 3584, -7680, 3, 46, 546,
1224, 4437, -7510, 1228, 4266, -7510, 46, 802, 1228, 4266,
-7510, 1232, 4266, -7339, 46, 1058, 1232, 4266, -7339, 1172,
4266, -6998, 6, 46, 546, 1224, 4437, -7510, 1228, 4266,
-7510, 46, 1297, 1192, 5802, -7339, 1236, 4437, -7339, 59,
1297, 1196, 5802, -7510, 1224, 4437, -7510, 60, 802, 1556,
4437, -7680, 1224, 4437, -7510, 60, 1041, 1224, 4437, -7510,
1236, 4437, -7339, 60, 1297, 1236, 4437, -7339, 1176, 4437,
-6998, 2, 46, 1297, 1192, 5802, -7339, 1236, 4437, -7339,
59, 1297, 1196, 5802, -7510, 1224, 4437, -7510, 2, 46,
1297, 1192, 5802, -7339, 1236, 4437, -7339, 59, 1297, 1196,
5802, -7510, 1224, 4437, -7510, 2, 46, 1297, 1192, 5802,
-7339, 1236, 4437, -7339, 59, 1297, 1196, 5802, -7510, 1224,
4437, -7510, 6, 45, 290, 1192, 5802, -7339, 1196, 5802,
-7510, 46, 1297, 1192, 5802, -7339, 1236, 4437, -7339, 48,
1570, 1244, 6656, -7339, 1284, 5866, -7339, 48, 1826, 1284,
5866, -7339, 1192, 5802, -7339, 59, 1297, 1196, 5802, -7510,
1224, 4437, -7510, 60, 546, 1196, 5802, -7510, 1560, 5802,
-7787, 1, 48, 1570, 1244, 6656, -7339, 1284, 5866, -7339,
3, 46, 1570, 1240, 6656, -7168, 1244, 6656, -7339, 46,
1826, 564, 6997, -7424, 1248, 6720, -7168, 48, 1570, 1244,
6656, -7339, 1284, 5866, -7339, 3, 21, 34, 560, 7338,
-7424, 564, 6997, -7424, 46, 1826, 564, 6997, -7424, 1248,
6720, -7168, 52, 1297, 564, 6997, -7424, 572, 7338, -6827,
4, 21, 34, 560, 7338, -7424, 564, 6997, -7424, 23,
1041, 560, 7338, -7424, 576, 7850, -7083, 32, 1058, 584,
7680, -7680, 860, 7338, -7467, 32, 1314, 860, 7338, -7467,
560, 7338, -7424, 1, 23, 1041, 560, 7338, -7424, 576,
7850, -7083, 1, 32, 1826, 864, 7893, -7083, 588, 8128,
-7680, 1, 17, 1570, 156, 3242, -7680, 464, 3328, -6742,
2, 17, 1826, 468, 3690, -6912, 116, 3584, -7680, 18,
34, 472, 3925, -6827, 468, 3690, -6912, 1, 18, 34,
472, 3925, -6827, 468, 3690, -6912, 2, 44, 1570, 1172,
4266, -6998, 1176, 4437, -6998, 46, 1058, 1232, 4266, -7339,
1172, 4266, -6998, 4, 44, 1570, 1172, 4266, -6998, 1176,
4437, -6998, 48, 34, 1176, 4437, -6998, 1272, 4437, -6934,
48, 290, 1272, 4437, -6934, 496, 4437, -6827, 60, 1297,
1236, 4437, -7339, 1176, 4437, -6998, 4, 51, 785, 1352,
4778, -6827, 1356, 4778, -6998, 51, 1041, 1356, 4778, -6998,
1344, 5290, -6998, 51, 1553, 1364, 4757, -6806, 1368, 4757,
-7019, 51, 1809, 1368, 4757, -7019, 1372, 5312, -7019, 4,
51, 273, 1344, 5290, -6998, 1348, 5290, -6827, 51, 1041,
1356, 4778, -6998, 1344, 5290, -6998, 51, 1809, 1368, 4757,
-7019, 1372, 5312, -7019, 52, 17, 1372, 5312, -7019, 1360,
5312, -6806, 2, 49, 34, 1288, 5632, -6912, 1292, 6229,
-6912, 50, 1314, 1340, 5632, -6827, 1288, 5632, -6912, 1,
49, 34, 1288, 5632, -6912, 1292, 6229, -6912, 2, 49,
34, 1288, 5632, -6912, 1292, 6229, -6912, 49, 290, 1292,
6229, -6912, 1296, 6229, -6059, 4, 46, 1570, 1240, 6656,
-7168, 1244, 6656, -7339, 46, 1826, 564, 6997, -7424, 1248,
6720, -7168, 47, 290, 1248, 6720, -7168, 1240, 6656, -7168,
47, 785, 1252, 6656, -6827, 1240, 6656, -7168, 1, 52,
1297, 564, 6997, -7424, 572, 7338, -6827, 2, 21, 546,
572, 7338, -6827, 576, 7850, -7083, 52, 1297, 564, 6997,
-7424, 572, 7338, -6827, 2, 21, 546, 572, 7338, -6827,
576, 7850, -7083, 23, 1041, 560, 7338, -7424, 576, 7850,
-7083, 4, 21, 546, 572, 7338, -6827, 576, 7850, -7083,
23, 1041, 560, 7338, -7424, 576, 7850, -7083, 32, 1570,
576, 7850, -7083, 864, 7893, -7083, 32, 1826, 864, 7893,
-7083, 588, 8128, -7680, 2, 17, 1570, 156, 3242, -7680,
464, 3328, -6742, 18, 290, 464, 3328, -6742, 476, 3925,
-6486, 1, 18, 290, 464, 3328, -6742, 476, 3925, -6486,
4, 18, 34, 472, 3925, -6827, 468, 3690, -6912, 18,
290, 464, 3328, -6742, 476, 3925, -6486, 18, 785, 476,
3925, -6486, 472, 3925, -6827, 18, 1570, 484, 4096, -6827,
472, 3925, -6827, 5, 18, 1041, 480, 4096, -6486, 484,
4096, -6827, 18, 1297, 488, 4138, -6827, 492, 4138, -6486,
18, 1570, 484, 4096, -6827, 472, 3925, -6827, 19, 290,
488, 4138, -6827, 484, 4096, -6827, 19, 546, 496, 4437,
-6827, 488, 4138, -6827, 3, 19, 546, 496, 4437, -6827,
488, 4138, -6827, 47, 529, 496, 4437, -6827, 500, 4437,
-6486, 48, 290, 1272, 4437, -6934, 496, 4437, -6827, 4,
51, 529, 1348, 5290, -6827, 1352, 4778, -6827, 51, 785,
1352, 4778, -6827, 1356, 4778, -6998, 51, 1297, 1360, 5312,
-6806, 1364, 4757, -6806, 51, 1553, 1364, 4757, -6806, 1368,
4757, -7019, 4, 51, 273, 1344, 5290, -6998, 1348, 5290,
-6827, 51, 529, 1348, 5290, -6827, 1352, 4778, -6827, 51,
1297, 1360, 5312, -6806, 1364, 4757, -6806, 52, 17, 1372,
5312, -7019, 1360, 5312, -6806, 4, 50, 802, 1332, 5802,
-6827, 1336, 5717, -6827, 50, 1058, 1336, 5717, -6827, 1340,
5632, -6827, 50, 1314, 1340, 5632, -6827, 1288, 5632, -6912,
50, 1553, 1308, 5717, -6144, 1336, 5717, -6827, 5, 50,
290, 1324, 6144, -6827, 1328, 5888, -6827, 50, 546, 1328,
5888, -6827, 1332, 5802, -6827, 50, 802, 1332, 5802, -6827,
1336, 5717, -6827, 50, 1809, 1312, 5802, -6144, 1332, 5802,
-6827, 51, 17, 1316, 5888, -6144, 1328, 5888, -6827, 3,
49, 290, 1292, 6229, -6912, 1296, 6229, -6059, 50, 34,
1320, 6144, -6144, 1324, 6144, -6827, 50, 290, 1324, 6144,
-6827, 1328, 5888, -6827, 7, 21, 290, 568, 6720, -6827,
572, 7338, -6827, 45, 34, 1184, 6826, -6656, 1188, 6656,
-6656, 46, 273, 1220, 6656, -5632, 1188, 6656, -6656, 47,
34, 1252, 6656, -6827, 568, 6720, -6827, 47, 785, 1252,
6656, -6827, 1240, 6656, -7168, 48, 1058, 1188, 6656, -6656,
1280, 6656, -6720, 48, 1314, 1280, 6656, -6720, 1252, 6656,
-6827, 3, 21, 290, 568, 6720, -6827, 572, 7338, -6827,
44, 1826, 1180, 6826, -5632, 1184, 6826, -6656, 45, 34,
1184, 6826, -6656, 1188, 6656, -6656, 3, 21, 290, 568,
6720, -6827, 572, 7338, -6827, 21, 546, 572, 7338, -6827,
576, 7850, -7083, 52, 1297, 564, 6997, -7424, 572, 7338,
-6827, 3, 18, 290, 464, 3328, -6742, 476, 3925, -6486,
18, 785, 476, 3925, -6486, 472, 3925, -6827, 18, 1826,
476, 3925, -6486, 480, 4096, -6486, 7, 18, 1041, 480,
4096, -6486, 484, 4096, -6827, 18, 1297, 488, 4138, -6827,
492, 4138, -6486, 18, 1826, 476, 3925, -6486, 480, 4096,
-6486, 19, 34, 480, 4096, -6486, 492, 4138, -6486, 19,
802, 492, 4138, -6486, 500, 4437, -6486, 44, 1058, 1160,
4437, -6315, 1164, 4266, -6315, 44, 1314, 1164, 4266, -6315,
1168, 4266, -5632, 6, 19, 802, 492, 4138, -6486, 500,
4437, -6486, 44, 1058, 1160, 4437, -6315, 1164, 4266, -6315,
45, 1809, 1160, 4437, -6315, 1216, 4437, -5632, 47, 529,
496, 4437, -6827, 500, 4437, -6486, 48, 546, 500, 4437,
-6486, 1276, 4437, -6379, 48, 802, 1276, 4437, -6379, 1160,
4437, -6315, 1, 50, 1553, 1308, 5717, -6144, 1336, 5717,
-6827, 2, 50, 1809, 1312, 5802, -6144, 1332, 5802, -6827,
51, 17, 1316, 5888, -6144, 1328, 5888, -6827, 2, 49,
290, 1292, 6229, -6912, 1296, 6229, -6059, 50, 34, 1320,
6144, -6144, 1324, 6144, -6827, 1, 46, 273, 1220, 6656,
-5632, 1188, 6656, -6656, 1, 44, 1826, 1180, 6826, -5632,
1184, 6826, -6656, 1, 44, 1314, 1164, 4266, -6315, 1168,
4266, -5632, 1, 45, 1809, 1160, 4437, -6315, 1216, 4437,
-5632, 6, 47, 1297, 1260, 5290, -5974, 1264, 4778, -5974,
47, 1553, 1264, 4778, -5974, 1268, 4778, -6144, 47, 1809,
1268, 4778, -6144, 1256, 5290, -6144, 52, 273, 1376, 4757,
-6166, 1380, 5312, -6166, 52, 785, 1384, 5312, -5952, 1388,
4757, -5952, 52, 1041, 1388, 4757, -5952, 1376, 4757, -6166,
6, 47, 1041, 1256, 5290, -6144, 1260, 5290, -5974, 47,
1297, 1260, 5290, -5974, 1264, 4778, -5974, 47, 1809, 1268,
4778, -6144, 1256, 5290, -6144, 52, 273, 1376, 4757, -6166,
1380, 5312, -6166, 52, 529, 1380, 5312, -6166, 1384, 5312,
-5952, 52, 785, 1384, 5312, -5952, 1388, 4757, -5952, 5,
49, 546, 1296, 6229, -6059, 1300, 5632, -6059, 49, 802,
1300, 5632, -6059, 1304, 5632, -6144, 49, 1058, 1304, 5632,
-6144, 1308, 5717, -6144, 49, 1314, 1308, 5717, -6144, 1312,
5802, -6144, 50, 1553, 1308, 5717, -6144, 1336, 5717, -6827,
6, 49, 546, 1296, 6229, -6059, 1300, 5632, -6059, 49,
1314, 1308, 5717, -6144, 1312, 5802, -6144, 49, 1570, 1312,
5802, -6144, 1316, 5888, -6144, 49, 1826, 1316, 5888, -6144,
1320, 6144, -6144, 50, 1809, 1312, 5802, -6144, 1332, 5802,
-6827, 51, 17, 1316, 5888, -6144, 1328, 5888, -6827, 4,
49, 290, 1292, 6229, -6912, 1296, 6229, -6059, 49, 546,
1296, 6229, -6059, 1300, 5632, -6059, 49, 1826, 1316, 5888,
-6144, 1320, 6144, -6144, 50, 34, 1320, 6144, -6144, 1324,
6144, -6827, 1, 46, 273, 1220, 6656, -5632, 1188, 6656,
-6656, 1, 44, 1826, 1180, 6826, -5632, 1184, 6826, -6656,
2, 44, 1314, 1164, 4266, -6315, 1168, 4266, -5632, 45,
1570, 1168, 4266, -5632, 1200, 4266, -5462, 2, 45, 1809,
1160, 4437, -6315, 1216, 4437, -5632, 46, 17, 1216, 4437,
-5632, 1220, 6656, -5632, 1, 46, 17, 1216, 4437, -5632,
1220, 6656, -5632, 1, 46, 17, 1216, 4437, -5632, 1220,
6656, -5632, 1, 46, 17, 1216, 4437, -5632, 1220, 6656,
-5632, 1, 46, 17, 1216, 4437, -5632, 1220, 6656, -5632,
1, 46, 17, 1216, 4437, -5632, 1220, 6656, -5632, 2,
46, 17, 1216, 4437, -5632, 1220, 6656, -5632, 46, 273,
1220, 6656, -5632, 1188, 6656, -6656, 2, 44, 1826, 1180,
6826, -5632, 1184, 6826, -6656, 45, 1314, 1212, 6826, -5462,
1180, 6826, -5632, 2, 45, 546, 1200, 4266, -5462, 1204,
4437, -5462, 45, 1570, 1168, 4266, -5632, 1200, 4266, -5462,
2, 45, 546, 1200, 4266, -5462, 1204, 4437, -5462, 45,
802, 1204, 4437, -5462, 1208, 6656, -5462, 1, 45, 802,
1204, 4437, -5462, 1208, 6656, -5462, 1, 45, 802, 1204,
4437, -5462, 1208, 6656, -5462, 1, 45, 802, 1204, 4437,
-5462, 1208, 6656, -5462, 1, 45, 802, 1204, 4437, -5462,
1208, 6656, -5462, 1, 45, 802, 1204, 4437, -5462, 1208,
6656, -5462, 2, 45, 802, 1204, 4437, -5462, 1208, 6656,
-5462, 45, 1058, 1208, 6656, -5462, 1212, 6826, -5462, 2,
45, 1058, 1208, 6656, -5462, 1212, 6826, -5462, 45, 1314,
1212, 6826, -5462, 1180, 6826, -5632
}};


const level e1m1 = {

                     .num_linedefs = 486,
                     .linedefs = linedefs,
                     .nodes = nodes,
                     .sectors = sectors,
                     .num_segs = 747,
                     .segs = segs,
                     .sidedefs = sidedefs,
                     .ssectors = ssectors,
                     .things = things,
                     .num_vertexes = 470,
                     .vertexes = vertexes,
                     .blkmap = &blkmap,
                     .render_blkmap = &render_blkmap
                     };
