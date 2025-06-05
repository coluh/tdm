#ifndef _TDM_world_h
#define _TDM_world_h

#include "types.h"

typedef struct World {
	// maybe cubes?
	// and spheres, meshes, triangles, quads... because raylib has ray collision test for them
	BoundingBox *boxes;
	int box_count;
	BoundingRamp *ramps;
	int ramp_count;
	BoundingBall *balls;
	int ball_count;
} World;

World *world_ofDebug();
bool world_overlapPoint(const World *w, Vector3 v);

#endif
