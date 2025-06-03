#include "game.h"
#include "config.h"
#include "utils.h"
#include "weapon.h"
#include "world.h"
#include "player.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <rcamera.h>
#include <raymath.h>

Game g;

static void input() {
	if (WindowShouldClose()) {
		g.running = false;
		return;
	}
	if (IsKeyPressed(KEY_F11)) {
		if (!IsWindowMaximized()) {
			SetWindowState(FLAG_WINDOW_UNDECORATED);
			MaximizeWindow();
		} else {
			ClearWindowState(FLAG_WINDOW_UNDECORATED);
			RestoreWindow();
		}
	}
	if (IsWindowResized()) {
		// const int w = GetScreenWidth();
		const int h = GetScreenHeight();
		g.zoom = floorf(h/500.0f + 0.5f)/2.0f;
		return;
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

	const World *world = g.world;

	// 3D content
	player_updateCameara(g.me, Vector3Lerp(g.me->previous_position, g.me->position, alpha));
	BeginMode3D(g.me->camera);
	DrawPlane((Vector3){0, -0.01, 0}, (Vector2){50, 50}, ColorFromHSV(200, 0.7f, 0.7f));

	// world boxes, ramps, balls
	for (int i = 0; i < world->box_count; i++) {
		const BoundingBox *box = &world->boxes[i];
		Vector3 position = Vector3Lerp(box->min, box->max, 0.5f);
		Vector3 size = Vector3Subtract(box->max, box->min);
		DrawCubeV(position, size, ColorFromHSV(360*hashV(position), 0.7f, 0.7f));
	}
	for (int i = 0; i < world->ramp_count; i++) {
		DrawRamp(&world->ramps[i], ColorFromHSV(360*hashV(world->ramps[i].max), 0.7f, 0.7f));
	}
	for (int i = 0; i < world->ball_count; i++) {
		DrawSphere(world->balls[i].center, world->balls[i].radius, ColorFromHSV(360*hashV(world->balls[i].center), 0.7f, 0.7f));
	}

	// players
	for (int i = 0; i < g.max_players; i++) {
		const Player *p = &g.players[i];
		Vector3 v = Vector3Lerp(p->previous_position, p->position, alpha); // bottom center
		v.y += PLAYER_BODY_HEIGHT/2;
		DrawCube(v, PLAYER_BODY_WIDTH, PLAYER_BODY_HEIGHT, PLAYER_BODY_WIDTH, p->team == 1 ? PINK : SKYBLUE);
		v.y += PLAYER_HEAD_OFFSET;
		DrawSphereWires(v, PLAYER_HEAD_RADIUS, 10, 10, p->team == 1 ? RED : BLUE);
	}
	EndMode3D();

	// 2D content
	const float zm = g.zoom;

	const int window_width = GetScreenWidth();
	const int window_height = GetScreenHeight();
	DrawLine(window_width/2.0f - 8*zm, window_height/2.0f, window_width/2.0f + 8*zm, window_height/2.0f, WHITE);
	DrawLine(window_width/2.0f, window_height/2.0f - 8*zm, window_width/2.0f, window_height/2.0f + 8*zm, WHITE);

	// weapons
	DrawRectangle(0, 0, 300*zm, 100*zm, GetColor(0xffffff7f));
	DrawRectangle(4*zm, 2*zm, 292*zm, 96*zm, GetColor(0x0000007f));
	DrawText(weapon_name(g.me->weapons.left.base), 9*zm, 7*zm, 24*g.zoom, WHITE);
	DrawRectangle(0, 100*zm, 300*zm, 100*zm, GetColor(0xffffff7f));
	DrawRectangle(4*zm, 102*zm, 292*zm, 96*zm, GetColor(0x0000007f));
	DrawText(weapon_name(g.me->weapons.right.base), 9*zm, 107*zm, 24*g.zoom, WHITE);
	DrawRectangle(0, 200*zm, 200*zm, 100*zm, GetColor(0xffffff7f));
	DrawRectangle(4*zm, 202*zm, 192*zm, 96*zm, GetColor(0x0000007f));
	DrawText(weapon_name(g.me->weapons.hand.base), 9*zm, 207*zm, 24*g.zoom, WHITE);

	// health
	float x = 20*zm;
	float y = window_height - 20*zm;
	y -= 40*zm;
	float w = 500*zm;
	float h = 40*zm;
	DrawRectangle(x, y, w, h, GetColor(0x0000007f));
	DrawRectangleLines(x, y, w, h, WHITE);
	float t = 2*zm;
	DrawRectangle(x + 2*t, y + 2*t, (w - 4*t)*g.me->health/PLAYER_FULL_HEALTH, h - 4*t, WHITE);

	// record
	y -= 20*zm + 24*zm;
	x += 8*zm;
	DrawRectangleGradientH(x - 8*zm, y - 6*zm, 200*zm, 36*zm, GetColor(0x0000007f), BLANK);
	char buf[16];
	snprintf(buf, 16, "%d / %d / %d", g.me->record.kill, g.me->record.death, g.me->record.assist);
	DrawText(buf, x, y, 24*g.zoom, WHITE);
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
	g.zoom = 1;

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
