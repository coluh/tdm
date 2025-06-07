#ifndef _TDM_utils_h
#define _TDM_utils_h

#include <raylib.h>
#include "types.h"

void DrawRamp(BoundingRamp *ramp, Color color);

// get y of ramp top plane at (x, z)
float GetRampHeight(BoundingRamp *ramp, float x, float z);

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define ABS(x) ((x) > 0 ? (x) : -(x))

#define MAX4(a, b, c, d) MAX(MAX(a, b), MAX(c, d))

#endif
