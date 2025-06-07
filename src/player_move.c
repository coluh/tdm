#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include "physics.h"
#include "player.h"
#include "utils.h"
#include "game.h"

extern Game g;

void player_move_slide(Player *p, World *w) {

	p->velocity.y += -30 * g.delta;

	Vector3 newpos = Vector3Add(p->position, Vector3Scale(p->velocity, g.delta));
	BoundingBox bounding;
	player_getBody(p, NULL, &bounding, NULL);
	Vector3 area_min = {
		MIN(p->position.x, newpos.x) - (bounding.max.x - bounding.min.x)/2,
		MIN(p->position.y, newpos.y) - (bounding.max.y - bounding.min.y)/2,
		MIN(p->position.z, newpos.z) - (bounding.max.z - bounding.min.z)/2,
	};
	Vector3 area_max = {
		MAX(p->position.x, newpos.x) + (bounding.max.x - bounding.min.x)/2,
		MAX(p->position.y, newpos.y) + (bounding.max.y - bounding.min.y)/2,
		MAX(p->position.z, newpos.z) + (bounding.max.z - bounding.min.z)/2,
	};

	int size;
	Body *bodies = physics_allocateBodies(w, (BoundingBox){ area_min, area_max }, &size);

	float delta_time = g.delta;

	for (int loop = 0; ; loop++) {
		if (loop >= 32) {
			TraceLog(LOG_WARNING, "Too much collision in one update");
			break;
		}

		BoundingBox body;
		player_getBody(p, NULL, &body, NULL);
		Body moving = (Body){ .type = Body_BOX, .box = &body };
		float nearest_collide_time = delta_time;
		CollisionType nearest_collision = Collision_NONE;

		for (int i = 0; i < size; i++) {
			float collide_time = nearest_collide_time;
			CollisionType collision = physics_testCollision(&bodies[i], &moving, p->velocity, &collide_time);
			if (collision == Collision_NONE || collide_time > nearest_collide_time) {
				continue;
			} else {
				nearest_collide_time = collide_time;
				nearest_collision = collision;
			}
		}

		if (nearest_collision == Collision_NONE) {
			p->position = Vector3Add(p->position, Vector3Scale(p->velocity, delta_time));
			break;
		}

		if (nearest_collide_time < 0) {
			// collided before test, bug? No, this is an instant collision
			if (nearest_collision == Collision_X) {
				p->position.x += p->velocity.x * nearest_collide_time;
			} else if (nearest_collision == Collision_Y) {
				p->position.y += p->velocity.y * nearest_collide_time;
			} else if (nearest_collision == Collision_Z) {
				p->position.z += p->velocity.z * nearest_collide_time;
			}
		} else {
			p->position = Vector3Add(p->position, Vector3Scale(p->velocity, nearest_collide_time));
			delta_time -= nearest_collide_time;
		}

		if (nearest_collision == Collision_X) {
			p->velocity.x = 0;
		} else if (nearest_collision == Collision_Y) {
			if (p->velocity.y < 0) {
				p->on_ground = true;
			}
			p->velocity.y = 0;
		} else if (nearest_collision == Collision_Z) {
			p->velocity.z = 0;
		}

		if (delta_time < 0.001f) {
			break;
		}
	}

	free(bodies);
}
