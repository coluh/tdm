#include "game.h"
#include "config.h"
#include "world.h"
#include "player.h"
#include <stdlib.h>
#include <raylib.h>
#include <rcamera.h>

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

	const float move_speed = 5.0f; // TODO: rely on player state
	const float ds = move_speed*GetFrameTime();
        if (IsKeyDown(KEY_W)) CameraMoveForward(&p->camera, ds, true);
        if (IsKeyDown(KEY_A)) CameraMoveRight(&p->camera, -ds, true);
        if (IsKeyDown(KEY_S)) CameraMoveForward(&p->camera, -ds, true);
        if (IsKeyDown(KEY_D)) CameraMoveRight(&p->camera, ds, true);
	if (IsKeyDown(KEY_SPACE)) CameraMoveUp(&p->camera, ds);
	if (IsKeyDown(KEY_LEFT_SHIFT)) CameraMoveUp(&p->camera, -ds);
}

static void update() {
	for (int i = 0; i < g.max_players; i++) {
		if (g.players[i].id == 0) {
			continue;
		}
		player_update(&g.players[i]);
	}
}

static void draw(float alpha) {
	ClearBackground(RAYWHITE);

	// const World *w = g.world;

	// 3D content
	BeginMode3D(g.me->camera);
	DrawPlane((Vector3){0, 0, 0}, (Vector2){20, 20}, LIGHTGRAY);

	for (int i = 0; i < g.max_players; i++) {
		const Player *p = &g.players[i];
		if (p == g.me) {
			continue;
		}
		Vector3 eye = p->camera.position;
		DrawSphereWires(eye, 0.25f, 10, 10, p->team == 1 ? RED : BLUE);
		eye.y -= 0.3f;
		eye.y -= 0.7f;
		DrawCubeWires(eye, 0.5f, 1.4f, 0.5f, p->team == 1 ? RED : BLUE);
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
