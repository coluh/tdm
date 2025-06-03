#include "game.h"
#include "config.h"
#include "utils.h"
#include "world.h"
#include "player.h"
#include <math.h>
#include <stdlib.h>
#include <raylib.h>
#include <rcamera.h>
#include <raymath.h>

Game g;

static void input() {
	if (WindowShouldClose()) {
		g.running = false;
	}

	Player * const p = g.me;
	const float da = config_get()->sensitivity.normal*GetFrameTime();
	const Vector2 mouseDelta = GetMouseDelta();
	CameraYaw(&p->camera, -mouseDelta.x*da, false);
	CameraPitch(&p->camera, -mouseDelta.y*da, true, false, false);

	// TODO: config keydown and hold
        if (IsKeyDown(KEY_W)) p->input.forward = 1;
        if (IsKeyDown(KEY_S)) p->input.back = 1;
        if (IsKeyDown(KEY_A)) p->input.left = 1;
        if (IsKeyDown(KEY_D)) p->input.right = 1;

	if (IsKeyPressed(KEY_SPACE)) p->input.jump = true;
	if (IsKeyPressed(KEY_LEFT_SHIFT)) p->input.crouch = true;
	if (IsKeyPressed(KEY_LEFT_CONTROL)) p->input.lie = true;
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) p->input.fire = true;
	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) p->input.scope = true;
}

static void update() {
	for (int i = 0; i < g.max_players; i++) {
		if (g.players[i].id == 0) {
			continue;
		}
		player_update(&g.players[i]);
	}
}

static float hashV(Vector3 v) {
	float dot = v.x * 127.1 + v.y * 311.7 + v.z * 74.7;
	dot = sinf(dot) * 43758.5453;
	return dot - floorf(dot);
}

static void draw(float alpha) {
	ClearBackground(RAYWHITE);

	const World *w = g.world;

	// 3D content
	player_updateCameara(g.me, Vector3Lerp(g.me->previous_position, g.me->position, alpha));
	BeginMode3D(g.me->camera);
	DrawPlane((Vector3){0, -0.01, 0}, (Vector2){50, 50}, ColorFromHSV(200, 0.7f, 0.7f));

	// world boxes, ramps, balls
	for (int i = 0; i < w->box_count; i++) {
		const BoundingBox *box = &w->boxes[i];
		Vector3 position = Vector3Lerp(box->min, box->max, 0.5f);
		Vector3 size = Vector3Subtract(box->max, box->min);
		DrawCubeV(position, size, ColorFromHSV(360*hashV(position), 0.7f, 0.7f));
	}
	for (int i = 0; i < w->ramp_count; i++) {
		DrawRamp(&w->ramps[i], ColorFromHSV(360*hashV(w->ramps[i].max), 0.7f, 0.7f));
	}
	for (int i = 0; i < w->ball_count; i++) {
		DrawSphere(w->balls[i].center, w->balls[i].radius, ColorFromHSV(360*hashV(w->balls[i].center), 0.7f, 0.7f));
	}

	// players
	for (int i = 0; i < g.max_players; i++) {
		const Player *p = &g.players[i];
		Vector3 v = Vector3Lerp(p->previous_position, p->position, alpha); // bottom center
		v.y += PLAYER_BODY_HEIGHT/2;
		DrawCubeWires(v, PLAYER_BODY_WIDTH, PLAYER_BODY_HEIGHT, PLAYER_BODY_WIDTH, p->team == 1 ? RED : BLUE);
		v.y += PLAYER_HEAD_OFFSET;
		DrawSphereWires(v, PLAYER_HEAD_RADIUS, 10, 10, p->team == 1 ? RED : BLUE);
	}
	EndMode3D();

	// 2D content
	const int hw = GetScreenWidth() / 2;
	const int hh = GetScreenHeight() / 2;
	DrawLine(hw - 5, hh, hw + 5, hh, WHITE);
	DrawLine(hw, hh - 5, hw, hh + 5, WHITE);
}

void game_loop(int world_index, int max_players) {

	// init
	if (world_index == 0) {
		g.world = world_ofDebug();
	} else {
		TraceLog(LOG_ERROR, "no world for id %d", world_index);
		return;
	}
	g.players = calloc(8, sizeof(Player));
	g.max_players = max_players;

	// NOTE: should be assigned by the server in the future
	// should be rewrite to support multi-player in the future!
	g.me = &g.players[0];
	for (int i = 0; i < max_players; i++) {
		g.players[i].id = i + 1;
		g.players[i].team = i < (max_players+1)/2 ? 1 : 2;
		player_init(&g.players[i]);
	}
	// player_init(&g.players[g.me]);

	DisableCursor();
	double accumulator = 0;
	g.running = true;
	g.delta = 0.05;

	while (g.running) {

		input();

		accumulator += GetFrameTime();
		while (accumulator >= g.delta) {
			update();
			accumulator -= g.delta;
		}

		BeginDrawing();
		draw(accumulator / g.delta);
		EndDrawing();
	}

	if (g.world) {
		free(g.world);
	}
	if (g.players) {
		free(g.players);
	}

	EnableCursor();
}
