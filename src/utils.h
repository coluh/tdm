#ifndef _TDM_utils_h
#define _TDM_utils_h

#include <raylib.h>
#include "types.h"

void DrawRamp(BoundingRamp *ramp, Color color);

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#endif
