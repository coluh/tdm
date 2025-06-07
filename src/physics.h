#ifndef _TDM_physics_h
#define _TDM_physics_h

#include <raylib.h>

typedef enum BodyType {
	Body_BOX,
	Body_RAMP,
	Body_BALL,
} BodyType;

typedef struct BoundingRamp BoundingRamp;
typedef struct BoundingBall BoundingBall;

typedef struct Body {
	BodyType type;
	union {
		BoundingBox *box;
		BoundingRamp *ramp;
		BoundingBall *ball;
	};
} Body;

typedef enum CollisionType {
	Collision_NONE, Collision_X, Collision_Y, Collision_Z,
} CollisionType;

typedef struct World World;
// free it
Body *physics_allocateBodies(World *w, BoundingBox area, int *size);
CollisionType physics_testCollision(Body *stati, Body *moving, Vector3 velocity, float *collide_time);

#endif
