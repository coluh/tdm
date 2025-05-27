#ifndef _TDM_world_h
#define _TDM_world_h

#include <raylib.h>

typedef struct BoundingBall {
	Vector3 center;
	float radius;
} BoundingBall;

typedef struct World {
	// maybe cubes?
	// and spheres, meshes, triangles, quads... because raylib has ray collision test for them
	BoundingBox *boxes;
	int box_count;
	BoundingBall *balls;
	int ball_count;
} World;

World *world_ofDebug();

#endif
