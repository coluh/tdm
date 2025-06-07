#include "world.h"
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "types.h"

World *world_ofDebug() {
	World *w = calloc(1, sizeof(World));

	w->box_count = 42 + 1;
	w->boxes = calloc(w->box_count, sizeof(BoundingBox));
	w->boxes[0] = (BoundingBox){{10.0, 0, -20.5}, {10.5, 10, 20.5}};
	w->boxes[1] = (BoundingBox){{-10.5, 0, -20.5}, {-10.0, 10, 20.5}};
	w->boxes[2] = (BoundingBox){{-10.0, 0, -20.5}, {10.0, 10, -20.0}};
	w->boxes[3] = (BoundingBox){{-10.0, 0, 20.0}, {10.0, 10, 20.5}};
	w->boxes[4] = (BoundingBox){{4.5, 0, -20}, {10, 3, -16.5}}; // left down
	w->boxes[5] = (BoundingBox){{7.5, 0, -13.5}, {10, 2.5, -11}}; // left near down
	w->boxes[6] = (BoundingBox){{-4.5, 0, -20}, {0, 0.5, -14}}; // down center
	w->boxes[7] = (BoundingBox){{0, 0, -14}, {4, 2.5, -10.6}}; // down center left
	w->boxes[8] = (BoundingBox){{4, 0, -10.6}, {5.5, 2.5, -10.3}}; // center left board
	w->boxes[9] = (BoundingBox){{5.5, 0, -12.3}, {7.5, 2.5, -12}}; // left right board
	w->boxes[10] = (BoundingBox){{-4.5, 0, -14}, {-4.3, 2.4, -10.3}}; // down center right
	w->boxes[11] = (BoundingBox){{-4.5, 2.4, -14}, {0, 2.5, -10.3}}; // down center top
	w->boxes[12] = (BoundingBox){{-6.5, 0, -10.6}, {-4.5, 2.5, -10.3}}; // center right board
	w->boxes[13] = (BoundingBox){{-8.8, 0, -14.5}, {-7, 1.5, -13}}; // right near down
	w->boxes[14] = (BoundingBox){{-10, 0, -3.5}, {-6.5, 2.5, -1}}; // right
	w->boxes[15] = (BoundingBox){{-9, 0, -15}, {-8.8, 2.5, -10}}; // right forward board 1
	w->boxes[16] = (BoundingBox){{-9, 0, -10}, {-8.8, 2, -9.5}}; // right forward board 2
	w->boxes[17] = (BoundingBox){{-9, 0, -9.5}, {-8.8, 2.5, -3.5}}; // right forward board 3
	w->boxes[18] = (BoundingBox){{-6, 0, -7.5}, {-2.5, 2.5, -6}}; // center bottom right
	w->boxes[19] = (BoundingBox){{3, 0, -6.5}, {6, 2.5, -4.5}}; // center bottom left
	w->boxes[20] = (BoundingBox){{1.2, 0, -7.7}, {1.5, 2.5, -4}}; // center bottom left |
	w->boxes[21] = (BoundingBox){{1.2, 0, -8}, {3, 2.5, -7.7}}; // center bottom left -
	w->boxes[22] = (BoundingBox){{7, 0, -3.5}, {10, 2.5, 0.5}}; // left, below are up, right, down
	w->boxes[23] = (BoundingBox){{6, 0, 0.5}, {10, 2.5, 2.5}};
	w->boxes[24] = (BoundingBox){{8.7, 0, 2.5}, {8.9, 2.5, 5.3}};
	w->boxes[25] = (BoundingBox){{8.7, 0, 5.3}, {8.9, 2, 6}};
	w->boxes[26] = (BoundingBox){{8.7, 0, 6}, {8.9, 2.5, 14.5}};
	w->boxes[27] = (BoundingBox){{7, 0, 12}, {8.7, 2, 13.5}};
	w->boxes[28] = (BoundingBox){{2.5, 0, 5.5}, {6, 2.5, 7}};
	w->boxes[29] = (BoundingBox){{-3.5, 0, 5}, {-1.5, 2.5, 7}};
	w->boxes[30] = (BoundingBox){{-4.5, 0, 2}, {-2.5, 2.5, 5}};
	w->boxes[31] = (BoundingBox){{-5.5, 0, 2}, {-4.5, 2.5, 3.5}};
	w->boxes[32] = (BoundingBox){{-9, 0, 9.3}, {-5, 2.5, 9.6}};
	w->boxes[33] = (BoundingBox){{-10, 0, 6}, {-9, 3, 10}};
	w->boxes[34] = (BoundingBox){{-10, 0, -1}, {-8.5, 2.5, 6}};
	w->boxes[35] = (BoundingBox){{-10, 0, 16.5}, {-6.5, 3, 20}}; // right top
	w->boxes[36] = (BoundingBox){{4.5, 0, 10.3}, {6.5, 2.5, 10.6}}; // up center left board
	w->boxes[37] = (BoundingBox){{4.3, 0, 10.3}, {4.5, 2.4, 14}}; // left wall
	w->boxes[38] = (BoundingBox){{0, 2.4, 10.3}, {4.5, 2.5, 14}}; // top
	w->boxes[39] = (BoundingBox){{-5, 0, 12}, {0, 2.5, 14}}; // right box
	w->boxes[40] = (BoundingBox){{0, 0, 10.3}, {0.2, 2.4, 14}}; // right wall
	w->boxes[41] = (BoundingBox){{0, 0, 14}, {4.5, 0.5, 20}};
	w->boxes[42] = (BoundingBox){{-20, -1, -20}, {20, 0, 20}};

	w->ramp_count = 6;
	w->ramps = calloc(w->ramp_count, sizeof(BoundingRamp));
	w->ramps[0] = (BoundingRamp){{0, 0, -20}, {2, 0.5, -14}, Direction_XP};
	w->ramps[1] = (BoundingRamp){{-6.5, 0, -20}, {-4.5, 0.5, -14}, Direction_XN};
	w->ramps[2] = (BoundingRamp){{-10, 0, -15}, {-9, 2.5, -3.5}, Direction_ZN}; // right forward slape
	w->ramps[3] = (BoundingRamp){{8.9, 0, 2.5}, {10, 2.5, 14.5}, Direction_ZP}; // left forward slape
	w->ramps[4] = (BoundingRamp){{-2, 0, 14}, {0, 0.5, 20}, Direction_XN};
	w->ramps[5] = (BoundingRamp){{4.5, 0, 14}, {6.5, 0.5, 20}, Direction_XP};

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
