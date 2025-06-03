#include "player.h"
#include <raylib.h>
#include <raymath.h>
#include <rcamera.h>
#include <string.h>
#include "game.h"
#include "types.h"

extern Game g;

void player_init(Player *p) {
	if (p->id == 0 || p->team == 0) {
		TraceLog(LOG_ERROR, "id or team not set");
		return;
	}

	p->position = (Vector3) {p->id*2 - 10, 0.5f, p->team == 1 ? -19 : 19};

	p->camera.position = (Vector3) {p->id*2 - 10, 2.2f, p->team == 1 ? -19 : 19};
	p->camera.target = (Vector3) {0.0f, 2.0f, 0.0f};
	p->camera.up = (Vector3) {0, 1, 0};
	p->camera.fovy = 90;
	p->camera.projection = CAMERA_PERSPECTIVE;
	player_updateCameara(p, p->position);
	p->camera.target = (Vector3) {0.0f, 2.0f, 0.0f};
}

static void move_slide(Player *p) {
	p->velocity.y += -30 * g.delta;

	p->position.x += p->velocity.x * g.delta;
	p->position.y += p->velocity.y * g.delta;
	p->position.z += p->velocity.z * g.delta;
	if (p->position.y < 0.01) {
		p->position.y = 0.01;
		p->velocity.y = 0;
	}
}

void player_getBody(Player *p, BoundingBall *head, BoundingBox *body) {
	head->center = (Vector3){
		p->position.x,
		p->position.y + PLAYER_BODY_HEIGHT/2 + PLAYER_HEAD_OFFSET,
		p->position.z,
	};
	head->radius = PLAYER_HEAD_RADIUS;
	body->min = (Vector3){
		p->position.x - PLAYER_BODY_WIDTH/2,
		p->position.y,
		p->position.z - PLAYER_BODY_WIDTH/2,
	};
	body->max = (Vector3){
		p->position.x + PLAYER_BODY_WIDTH/2,
		p->position.y + PLAYER_BODY_HEIGHT,
		p->position.z + PLAYER_BODY_WIDTH/2,
	};
}

void player_fire(Player *self) {
	Vector3 dir = Vector3Subtract(self->camera.target, self->camera.position);
	dir = Vector3Normalize(dir);
	Ray bullet = {
		self->camera.position,
		dir,
	};

	// RayCollision nearest_collide;
	// Deleted! because I want more birds in one row!
	for (int i = 0; i < g.max_players; i++) {
		Player *p = &g.players[i];
		if (p->id == 0) continue; // player not exists
		if (p->team != self->team) {
			BoundingBall head;
			BoundingBox body;
			player_getBody(p, &head, &body);
			RayCollision info_head = GetRayCollisionSphere(bullet, head.center, head.radius);
			RayCollision info_body = GetRayCollisionBox(bullet, body);
			if (info_head.hit || info_body.hit) {
				TraceLog(LOG_INFO, "player %d shoot player %d!", self->id, p->id);
			}
		}
	}
}

void player_update(Player *p) {
	p->previous_position = p->position;

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
		p->velocity.y = 10.0f;
	}
	if (p->input.fire) {
		player_fire(p);
	}

	move_slide(p);

	memset(&p->input, 0, sizeof(struct PlayerInput));
}

void player_updateCameara(Player *player, Vector3 position) {
	Camera *cam = &player->camera;

	Vector3 p = position;
	Vector3 eye_offset = {0, PLAYER_BODY_HEIGHT/2 + PLAYER_HEAD_OFFSET, 0};
	Vector3 forward = GetCameraForward(cam);
	Vector3 right = GetCameraRight(cam);
	Vector3 up = GetCameraUp(cam);
	forward = Vector3Scale(forward, -1.0f);
	right = Vector3Scale(right, 0.5f);
	up = Vector3Scale(up, 0.5f);
	p = Vector3Add(p, eye_offset);
	p = Vector3Add(p, forward);
	p = Vector3Add(p, right);
	p = Vector3Add(p, up);

	Vector3 dp = Vector3Subtract(p, cam->position);

	cam->position = Vector3Add(cam->position, dp);
	cam->target = Vector3Add(cam->target, dp);
}
