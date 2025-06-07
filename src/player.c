#include "player.h"
#include <raylib.h>
#include <raymath.h>
#include <rcamera.h>
#include <string.h>
#include "game.h"
#include "particle.h"
#include "types.h"
#include "weapon.h"
#include "event.h"
#include "world.h"

extern Game g;

void player_init(Player *p) {
	if (p->id == 0 || p->team == 0) {
		TraceLog(LOG_ERROR, "id or team not set");
		return;
	}

	p->position = (Vector3) {p->id*2 - 10, 0.5f, p->team == 1 ? -19 : 19};
	p->rotation.pitch = 0.0f;

	p->camera.up = (Vector3) {0, 1, 0};
	p->camera.fovy = 90;
	p->camera.projection = CAMERA_PERSPECTIVE;

	p->health = PLAYER_FULL_HEALTH;
	p->weapons.left.base = Weapon_M762;
	p->weapons.hand.base = Weapon_DEAGLE;
	p->holding = &p->weapons.left;
	p->has_frag_grenade = true;
	p->has_stun_grenade = true;
}

void player_getBody(const Player *player, BoundingBall *head, BoundingBox *body, const Vector3 *pos) {
	const Vector3 *p = pos ? pos : &player->position;
	float bh = player->crouching ? PLAYER_BODY_CROUCH_HEIGHT : PLAYER_BODY_HEIGHT;

	if (head) {
		head->center = (Vector3){
			p->x,
				p->y + bh + PLAYER_HEAD_OFFSET,
				p->z,
		};
		head->radius = PLAYER_HEAD_RADIUS;
	}

	if (body) {
		body->min = (Vector3){
			p->x - PLAYER_BODY_WIDTH/2,
				p->y,
				p->z - PLAYER_BODY_WIDTH/2,
		};
		body->max = (Vector3){
			p->x + PLAYER_BODY_WIDTH/2,
				p->y + bh,
				p->z + PLAYER_BODY_WIDTH/2,
		};
	}

}

void player_fire(Player *self) {
	Vector3 dir = Vector3Subtract(self->camera.target, self->camera.position);
	dir = Vector3Normalize(dir);
	Ray bullet = {
		self->camera.position,
		dir,
	};

	if (!self->crouching) {
		self->rotation.pitch += GetRandomValue(5, 12)*2*PI/360/10;
	} else {
		self->rotation.pitch += GetRandomValue(4, 8)*2*PI/360/10;
	}
	self->rotation.yaw += GetRandomValue(-2, 2)*2*PI/360/10;

	// RayCollision nearest_collide;
	// Deleted! because I want more birds in one row!
	for (int i = 0; i < g.max_players; i++) {
		Player *p = &g.players[i];
		if (p->id == 0) continue; // player not exists
		if (p->team != self->team && !p->dead) {
			BoundingBall head;
			BoundingBox body;
			player_getBody(p, &head, &body, NULL);
			RayCollision info_head = GetRayCollisionSphere(bullet, head.center, head.radius);
			RayCollision info_body = GetRayCollisionBox(bullet, body);
			if (info_head.hit || info_body.hit) {
				float hurt = 0.48f;
				if (info_head.hit) {
					hurt *= 2.0f;
				}
				p->health -= hurt;
				if (p->health <= 0) {
					event_dispatch(&(Event){
						.type = Event_KILL,
						.kill = { self->id, p->id },
					});
				}
				Vector3 point = info_head.hit ? info_head.point : info_body.point;
				particle_spawn(point, GREEN);
				particle_spawn(point, GREEN);
				particle_spawn(point, GREEN);
				particle_spawn(point, GREEN);
				particle_spawn(point, GREEN);
			}
		}
	}
}

void player_update(Player *p, World *w) {
	p->previous_position = p->position;

	if (p->dead) {
		p->reborn_timer -= g.delta;
		if (p->reborn_timer <= 0) {
			p->dead = false;
			p->health = PLAYER_FULL_HEALTH;
		}
		return;
	}

	Vector3 input_dir = {p->input.right - p->input.left, 0, p->input.forward - p->input.back};
	Vector3 right = GetCameraRight(&p->camera);
	right.y = 0;
	right = Vector3Normalize(right);
	right = Vector3Scale(right, input_dir.x);
	Vector3 forward = GetCameraForward(&p->camera);
	forward.y = 0;
	forward = Vector3Normalize(forward);
	forward = Vector3Scale(forward, input_dir.z);
	Vector3 v = Vector3Add(right, forward);
	v = Vector3Normalize(v);
	v = Vector3Scale(v, 5.0f); // TODO: depends on player state
	p->velocity.x = v.x;
	p->velocity.z = v.z;
	if (p->input.jump) {
		if (p->on_ground) {
			p->velocity.y = 12.0f;
			p->on_ground = false;
		}
	}
	if (p->input.fire) {
		player_fire(p);
	}
	if (p->input.crouch) {
		p->crouching = !p->crouching;
	}
	if (p->input.lie) {
		p->lying = !p->lying;
	}
	p->scoping = p->input.scope;

	player_move_slide(p, w);
	// player_updateCameara(p, p->position, w);

	memset(&p->input, 0, sizeof(struct PlayerInput));
}

void player_updateCameara(Player *player, Vector3 position, const World *world) {
	Camera *cam = &player->camera;

	cam->position = Vector3Lerp(cam->position, player->target.camera_position, 0.3f);
	cam->target = Vector3Lerp(cam->target, player->target.camera_target, 0.3f);

	Vector3 c_target;
	Vector3 c_position;

	Vector3 p = position;
	Vector3 eye_offset = {0, PLAYER_BODY_HEIGHT + PLAYER_HEAD_OFFSET, 0};
	if (player->crouching) {
		eye_offset.y = PLAYER_BODY_CROUCH_HEIGHT + PLAYER_HEAD_OFFSET;
	}
	p = Vector3Add(p, eye_offset);

	Vector3 forward = (Vector3){cosf(player->rotation.yaw), 0.0f, sinf(player->rotation.yaw)};
	Vector3 up = (Vector3){0, 1, 0};
	Vector3 right = Vector3CrossProduct(forward, up);
	forward = Vector3RotateByAxisAngle(forward, right, player->rotation.pitch);

	c_target = Vector3Add(p, Vector3Scale(right, 0.4f));

	forward = Vector3Scale(forward, -1.0f);
	right = Vector3Scale(right, 0.5f);
	up = Vector3Scale(up, 0.5f);

	p = Vector3Add(p, forward);
	p = Vector3Add(p, right);
	p = Vector3Add(p, up);

	c_position = c_target;

	Vector3 eye = Vector3Subtract(p, c_target);
	eye = Vector3Normalize(eye);
	eye = Vector3Scale(eye, 0.1f);
	c_position = Vector3Add(c_position, eye);
	c_position = Vector3Add(c_position, eye);

	if (player->scoping) {
		player->target.camera_position = c_position;
		player->target.camera_target = c_target;
		return;
	}

	float d = 2.5f;
	while (!world_overlapPoint(world, c_position)) {
		c_position = Vector3Add(c_position, eye);
		d -= 0.1f;
		if (d <= 0) {
			break;
		}
	}
	c_position = Vector3Subtract(c_position, eye);

	player->target.camera_position = c_position;
	player->target.camera_target = c_target;
	// cam->target = player->target.camera_target;
	// cam->position = player->target.camera_position;
}
