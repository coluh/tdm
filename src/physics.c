#include "physics.h"
#include <raylib.h>
#include <stdlib.h>
#include "world.h"
#include "utils.h"

#define INNER_MARGIN -0.5f
#define EPSILON 0.01f

Body *physics_allocateBodies(World *w, BoundingBox area, int *size) {
	Body *bodies = calloc(w->box_count + w->ramp_count + w->ball_count, sizeof(Body));
	int count = 0;

	for (int i = 0; i < w->box_count; i++) {
		BoundingBox *box = &w->boxes[i];
		if (CheckCollisionBoxes(area, *box)) {
			bodies[count++] = (Body){.type = Body_BOX, .box = box};
		}
	}
	for (int i = 0; i < w->ramp_count; i++) {
		BoundingRamp *ramp = &w->ramps[i];
		if (CheckCollisionBoxes(area, (BoundingBox){ramp->min, ramp->max})) {
			bodies[count++] = (Body){.type = Body_RAMP, .ramp = ramp};
		}
	}
	for (int i = 0; i < w->ball_count; i++) {
		BoundingBall *ball = &w->balls[i];
		if (CheckCollisionBoxSphere(area, ball->center, ball->radius)) {
			bodies[count++] = (Body){.type = Body_BALL, .ball = ball};
		}
	}

	*size = count;
	return bodies;
}

CollisionType physics_testCollision(Body *stati, Body *moving, Vector3 velocity, float *collide_time) {

	if (moving->type != Body_BOX) {
		TraceLog(LOG_WARNING, "only support box type moving body");
		return Collision_NONE;
	}

	const float total_time = *collide_time;
	Vector3 bounding;
	Vector3 sum;

	switch (stati->type) {
	case Body_BOX:
		bounding = (Vector3){
			MAX(stati->box->max.x, moving->box->max.x) - MIN(stati->box->min.x, moving->box->min.x),
			MAX(stati->box->max.y, moving->box->max.y) - MIN(stati->box->min.y, moving->box->min.y),
			MAX(stati->box->max.z, moving->box->max.z) - MIN(stati->box->min.z, moving->box->min.z),
		};
		sum = (Vector3){
			stati->box->max.x - stati->box->min.x + (moving->box->max.x - moving->box->min.x),
			stati->box->max.y - stati->box->min.y + (moving->box->max.y - moving->box->min.y),
			stati->box->max.z - stati->box->min.z + (moving->box->max.z - moving->box->min.z),
		};
		if (velocity.y) {
			const float distance = bounding.y - sum.y;
			*collide_time = distance / ABS(velocity.y);
			const float dt = MAX(*collide_time, 0);
			if (*collide_time < total_time) {
				if (((velocity.y > 0 && stati->box->min.y - moving->box->max.y > INNER_MARGIN) || (velocity.y < 0 && moving->box->min.y - stati->box->max.y > INNER_MARGIN)) &&
						(MAX(moving->box->max.x + velocity.x * dt, stati->box->max.x) - MIN(moving->box->min.x + velocity.x * dt, stati->box->min.x) < sum.x - EPSILON) &&
						(MAX(moving->box->max.z + velocity.z * dt, stati->box->max.z) - MIN(moving->box->min.z + velocity.z * dt, stati->box->min.z) < sum.z - EPSILON)) {
					return Collision_Y;
				}
			} else {
				return Collision_NONE;
			}
		}
		if (velocity.x) {
			const float distance = bounding.x - sum.x;
			*collide_time = distance / ABS(velocity.x);
			const float dt = MAX(*collide_time, 0);
			if (*collide_time < total_time) {
				if (((velocity.x > 0 && stati->box->min.x - moving->box->max.x > INNER_MARGIN) || (velocity.x < 0 && moving->box->min.x - stati->box->max.x > INNER_MARGIN)) &&
						(MAX(moving->box->max.y + velocity.y * dt, stati->box->max.y) - MIN(moving->box->min.y + velocity.y * dt, stati->box->min.y) < sum.y - EPSILON) &&
						(MAX(moving->box->max.z + velocity.z * dt, stati->box->max.z) - MIN(moving->box->min.z + velocity.z * dt, stati->box->min.z) < sum.z - EPSILON)) {
					return Collision_X;
				}
			} else {
				return Collision_NONE;
			}
		}
		if (velocity.z) {
			const float distance = bounding.z - sum.z;
			*collide_time = distance / ABS(velocity.z);
			const float dt = MAX(*collide_time, 0);
			if (*collide_time < total_time) {
				if (((velocity.z > 0 && stati->box->min.z - moving->box->max.z > INNER_MARGIN) || (velocity.z < 0 && moving->box->min.z - stati->box->max.z > INNER_MARGIN)) &&
						(MAX(moving->box->max.x + velocity.x * dt, stati->box->max.x) - MIN(moving->box->min.x + velocity.x * dt, stati->box->min.x) < sum.x - EPSILON) &&
						(MAX(moving->box->max.y + velocity.y * dt, stati->box->max.y) - MIN(moving->box->min.y + velocity.y * dt, stati->box->min.y) < sum.y - EPSILON)) {
					return Collision_Z;
				}
			} else {
				return Collision_NONE;
			}
		}
		break;
	case Body_RAMP:
		sum = (Vector3){
			stati->ramp->max.x - stati->ramp->min.x + (moving->ramp->max.x - moving->ramp->min.x),
			stati->ramp->max.y - stati->ramp->min.y + (moving->ramp->max.y - moving->ramp->min.y),
			stati->ramp->max.z - stati->ramp->min.z + (moving->ramp->max.z - moving->ramp->min.z),
		};
		if (velocity.y && (MAX(moving->box->max.x + velocity.x * total_time, stati->ramp->max.x) - MIN(moving->box->min.x + velocity.x * total_time, stati->ramp->min.x) < sum.x - EPSILON) &&
						(MAX(moving->box->max.z + velocity.z * total_time, stati->ramp->max.z) - MIN(moving->box->min.z + velocity.z * total_time, stati->ramp->min.z) < sum.z - EPSILON)) {
			float rampHeight1 = GetRampHeight(stati->ramp, moving->box->min.x, moving->box->min.z);
			float rampHeight2 = GetRampHeight(stati->ramp, moving->box->min.x, moving->box->max.z);
			float rampHeight3 = GetRampHeight(stati->ramp, moving->box->max.x, moving->box->min.z);
			float rampHeight4 = GetRampHeight(stati->ramp, moving->box->max.x, moving->box->max.z);
			float rampHeight = MAX4(rampHeight1, rampHeight2, rampHeight3, rampHeight4);
			const float distance = moving->box->min.y - rampHeight;
			*collide_time = distance / ABS(velocity.y);
			// const float dt = MAX(*collide_time, 0);
			if (velocity.y < 0 && moving->ramp->min.y - rampHeight > INNER_MARGIN) {
				return Collision_Y;
			}
		}
		break;
	case Body_BALL:
		break;
	default:
		break;
	}

	return Collision_NONE;
}
