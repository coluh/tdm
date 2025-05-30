#ifndef _TDM_types_h
#define _TDM_types_h

#include <raylib.h>

typedef enum Direction {
	Direction_XP,
	Direction_ZP,
	Direction_XN,
	Direction_ZN,
} Direction;

// BoundingRamp
typedef struct BoundingRamp {
	Vector3 min;
	Vector3 max;
	Direction face;
} BoundingRamp;

// BoundingBall
typedef struct BoundingBall {
	Vector3 center;
	float radius;
} BoundingBall;

#endif
