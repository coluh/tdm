#include "utils.h"
#include <raylib.h>
#include <raymath.h>
#include "types.h"

void DrawRamp(BoundingRamp *ramp, Color color) {
	Vector3 min = ramp->min;
	Vector3 max = ramp->max;
	DrawTriangle3D((Vector3){min.x, min.y, min.z}, (Vector3){max.x, min.y, min.z}, (Vector3){max.x, min.y, max.z}, color);
	DrawTriangle3D((Vector3){min.x, min.y, min.z}, (Vector3){max.x, min.y, max.z}, (Vector3){min.x, min.y, max.z}, color);

	switch (ramp->face) {
	case Direction_XP:
		DrawTriangle3D((Vector3){min.x, min.y, min.z}, (Vector3){min.x, min.y, max.z}, (Vector3){min.x, max.y, max.z}, color);
		DrawTriangle3D((Vector3){min.x, min.y, min.z}, (Vector3){min.x, max.y, max.z}, (Vector3){min.x, max.y, min.z}, color);
		DrawTriangle3D((Vector3){min.x, min.y, min.z}, (Vector3){min.x, max.y, min.z}, (Vector3){max.x, min.y, min.z}, color);
		DrawTriangle3D((Vector3){min.x, min.y, max.z}, (Vector3){max.x, min.y, max.z}, (Vector3){min.x, max.y, max.z}, color);
		DrawTriangle3D((Vector3){min.x, max.y, min.z}, (Vector3){max.x, min.y, max.z}, (Vector3){max.x, min.y, min.z}, color);
		DrawTriangle3D((Vector3){min.x, max.y, min.z}, (Vector3){min.x, max.y, max.z}, (Vector3){max.x, min.y, max.z}, color);
		break;
	case Direction_ZP:
		DrawTriangle3D((Vector3){min.x, min.y, min.z}, (Vector3){min.x, max.y, min.z}, (Vector3){max.x, max.y, min.z}, color);
		DrawTriangle3D((Vector3){min.x, min.y, min.z}, (Vector3){max.x, max.y, min.z}, (Vector3){max.x, min.y, min.z}, color);
		DrawTriangle3D((Vector3){min.x, min.y, min.z}, (Vector3){min.x, min.y, max.z}, (Vector3){min.x, max.y, min.z}, color);
		DrawTriangle3D((Vector3){max.x, min.y, min.z}, (Vector3){max.x, max.y, min.z}, (Vector3){max.x, min.y, max.z}, color);
		DrawTriangle3D((Vector3){min.x, max.y, min.z}, (Vector3){min.x, min.y, max.z}, (Vector3){max.x, min.y, max.z}, color);
		DrawTriangle3D((Vector3){min.x, max.y, min.z}, (Vector3){max.x, min.y, max.z}, (Vector3){max.x, max.y, min.z}, color);
		break;
	case Direction_XN:
		DrawTriangle3D((Vector3){max.x, min.y, min.z}, (Vector3){max.x, max.y, min.z}, (Vector3){max.x, max.y, max.z}, color);
		DrawTriangle3D((Vector3){max.x, min.y, min.z}, (Vector3){max.x, max.y, max.z}, (Vector3){max.x, min.y, max.z}, color);
		DrawTriangle3D((Vector3){max.x, min.y, min.z}, (Vector3){min.x, min.y, min.z}, (Vector3){max.x, max.y, min.z}, color);
		DrawTriangle3D((Vector3){max.x, min.y, max.z}, (Vector3){max.x, max.y, max.z}, (Vector3){min.x, min.y, max.z}, color);
		DrawTriangle3D((Vector3){max.x, max.y, min.z}, (Vector3){min.x, min.y, min.z}, (Vector3){min.x, min.y, max.z}, color);
		DrawTriangle3D((Vector3){max.x, max.y, min.z}, (Vector3){min.x, min.y, max.z}, (Vector3){max.x, max.y, max.z}, color);
		break;
	case Direction_ZN:
		DrawTriangle3D((Vector3){min.x, min.y, max.z}, (Vector3){max.x, min.y, max.z}, (Vector3){max.x, max.y, max.z}, color);
		DrawTriangle3D((Vector3){min.x, min.y, max.z}, (Vector3){max.x, max.y, max.z}, (Vector3){min.x, max.y, max.z}, color);
		DrawTriangle3D((Vector3){min.x, min.y, min.z}, (Vector3){min.x, min.y, max.z}, (Vector3){min.x, max.y, max.z}, color);
		DrawTriangle3D((Vector3){max.x, min.y, min.z}, (Vector3){max.x, max.y, max.z}, (Vector3){max.x, min.y, max.z}, color);
		DrawTriangle3D((Vector3){min.x, min.y, min.z}, (Vector3){min.x, max.y, max.z}, (Vector3){max.x, max.y, max.z}, color);
		DrawTriangle3D((Vector3){min.x, min.y, min.z}, (Vector3){max.x, max.y, max.z}, (Vector3){max.x, min.y, min.z}, color);
		break;
	}
}

float GetRampHeight(BoundingRamp *ramp, float x, float z) {

	float p;

	switch (ramp->face) {
	case Direction_XP:
		p = (ramp->max.x - x)/(ramp->max.x - ramp->min.x);
		return Lerp(ramp->min.y, ramp->max.y, p);
	case Direction_ZP:
		p = (ramp->max.z - z)/(ramp->max.z - ramp->min.z);
		return Lerp(ramp->min.y, ramp->max.y, p);
	case Direction_XN:
		p = (x - ramp->min.x)/(ramp->max.x - ramp->min.x);
		return Lerp(ramp->min.y, ramp->max.y, p);
	case Direction_ZN:
		p = (z - ramp->min.z)/(ramp->max.z - ramp->min.z);
		return Lerp(ramp->min.y, ramp->max.y, p);
	}

	return 0;
}
