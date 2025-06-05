#include "world.h"
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "types.h"

World *world_ofDebug() {
	World *w = calloc(1, sizeof(World));

	w->box_count = 20;
	w->boxes = calloc(w->box_count, sizeof(BoundingBox));
	w->boxes[0] = (BoundingBox){{10.0, 0, -20.1}, {10.1, 10, 20.1}};
	w->boxes[1] = (BoundingBox){{-10.1, 0, -20.1}, {-10.0, 10, 20.1}};
	w->boxes[2] = (BoundingBox){{-10.0, 0, -20.1}, {10.0, 10, -20.0}};
	w->boxes[3] = (BoundingBox){{-10.0, 0, 20.0}, {10.0, 10, 20.1}};
	w->boxes[4] = (BoundingBox){{4.5, 0, -20}, {10, 3, -16.5}}; // left down
	w->boxes[5] = (BoundingBox){{7.5, 0, -13}, {10, 2.5, -10}}; // left near down
	w->boxes[6] = (BoundingBox){{-4.5, 0, -20}, {0, 0.5, -14}}; // down center
	w->boxes[7] = (BoundingBox){{0, 0, -14}, {3.5, 2.5, -11}}; // down center left
	w->boxes[8] = (BoundingBox){{3.5, 0, -11.4}, {5.5, 2.5, -11}}; // center left board
	w->boxes[9] = (BoundingBox){{5.5, 0, -12.4}, {7.5, 2.5, -12}}; // left right board
	w->boxes[10] = (BoundingBox){{-4.7, 0, -14}, {-4.3, 2.4, -11}}; // down center right
	w->boxes[11] = (BoundingBox){{-4.7, 2.4, -14}, {0, 2.5, -11}}; // down center top
	w->boxes[12] = (BoundingBox){{-6.5, 0, -11.4}, {-4.7, 2.5, -11}}; // center right board
	w->boxes[13] = (BoundingBox){{-8.8, 0, -14.5}, {-7, 2.5, -13}}; // right near down
	w->boxes[14] = (BoundingBox){{-10, 0, -4}, {-6, 2.5, -2}}; // right
	w->boxes[15] = (BoundingBox){{-9, 0, -15}, {-8.8, 2.5, -10}}; // right forward board
	w->boxes[16] = (BoundingBox){{-9, 0, -10}, {-8.8, 2, -9.5}}; // right forward board
	w->boxes[17] = (BoundingBox){{-9, 0, -9.5}, {-8.8, 2.5, -4}}; // right forward board

	w->ramp_count = 6;
	w->ramps = calloc(w->ramp_count, sizeof(BoundingRamp));
	w->ramps[0] = (BoundingRamp){{0, 0, -20}, {2, 0.5, -14}, Direction_XP};
	w->ramps[1] = (BoundingRamp){{-6.5, 0, -20}, {-4.5, 0.5, -14}, Direction_XN};
	w->ramps[2] = (BoundingRamp){{-10, 0, -15}, {-9, 2.5, -4}, Direction_ZN}; // right forward slape

	w->ball_count = 2;
	w->balls = calloc(w->ball_count, sizeof(Vector3) + sizeof(float));
	w->balls[0] = (BoundingBall){{0, 1, 0}, 1.5};
	w->balls[1] = (BoundingBall){{1, 0.5, 1}, 0.5};

	return w;
}

bool world_overlapPoint(const World *w, Vector3 v) {
	for (int i = 0; i < w->box_count; i++) {
		const BoundingBox *box = &w->boxes[i];
		if (v.x >= box->min.x && v.x < box->max.x &&
				v.y >= box->min.y && v.y < box->max.y &&
				v.z >= box->min.z && v.z < box->max.z) {
			return true;
		}
	}
	for (int i = 0; i < w->ramp_count; i++) {
		;
	}
	for (int i = 0; i < w->ball_count; i++) {
		const BoundingBall *ball = &w->balls[i];
		if (Vector3DistanceSqr(v, ball->center) <= ball->radius*ball->radius) {
			return true;
		}
	}
	return false;
}
