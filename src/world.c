#include "world.h"
#include <raylib.h>
#include <stddef.h>
#include <stdlib.h>

World *world_ofDebug() {
	World *w = calloc(1, sizeof(World));

	w->box_count = 20;
	w->boxes = calloc(w->box_count, sizeof(BoundingBox));
	w->boxes[0] = (BoundingBox){{10, 0, -20}, {7, 2, -18}};
	w->boxes[1] = (BoundingBox){{}, {}};
	w->boxes[2] = (BoundingBox){{}, {}};

	w->ball_count = 2;
	w->balls = calloc(w->ball_count, sizeof(Vector3) + sizeof(float));
	w->balls[0] = (BoundingBall){{0, 1, 0}, 1.5};
	w->balls[1] = (BoundingBall){{1, 0.5, 1}, 0.5};

	return w;
}
