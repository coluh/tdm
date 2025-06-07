#include "game.h"
#include "config.h"
#include "particle.h"
#include "types.h"
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

	if (IsKeyPressed(KEY_E)) {
		if (!g.unfocus) {
			g.unfocus = true;
			EnableCursor();
		} else {
			g.unfocus = false;
			DisableCursor();
		}
	}
	if (g.unfocus) {
		return;
	}

	Player * const p = g.me;
	const float da = config_get()->sensitivity.normal*GetFrameTime();
	const Vector2 mouseDelta = GetMouseDelta();
	p->rotation.yaw += mouseDelta.x*da;
	p->rotation.pitch -= mouseDelta.y*da;
	p->rotation.pitch = Wrap(p->rotation.pitch, -89.0f, +89.0f);

	// TODO: config keydown and hold
        if (IsKeyDown(KEY_W)) p->input.forward = 1;
        if (IsKeyDown(KEY_S)) p->input.back = 1;
        if (IsKeyDown(KEY_A)) p->input.left = 1;
        if (IsKeyDown(KEY_D)) p->input.right = 1;

	if (IsKeyPressed(KEY_SPACE)) p->input.jump = true;
	if (IsKeyPressed(KEY_LEFT_SHIFT)) p->input.crouch = true;
	if (IsKeyPressed(KEY_LEFT_CONTROL)) p->input.lie = true;
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) p->input.fire = true;
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) p->input.scope = true;

	if (IsKeyPressed(KEY_ONE)) p->holding = &p->weapons.left;
	if (IsKeyPressed(KEY_TWO)) p->holding = &p->weapons.right;
	if (IsKeyPressed(KEY_THREE)) p->holding = &p->weapons.hand;

	// debug
	if (IsKeyPressed(KEY_U)) {
		for (int i = 0; i < g.max_players; i++) {
			Player *e = &g.players[i];
			if (e->id != 0 && e->team != p->team) {
				p->position = Vector3Add(e->position, (Vector3){0, 0, -1});
			}
		}
	}
}

static void update() {
	for (int i = 0; i < g.max_players; i++) {
		if (g.players[i].id == 0) {
			continue;
		}
		player_update(&g.players[i], g.world);
	}
	particle_update();
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
	player_updateCameara(g.me, Vector3Lerp(g.me->previous_position, g.me->position, alpha), world);
	BeginMode3D(g.me->camera);
	// DrawPlane((Vector3){0, -0.01, 0}, (Vector2){50, 50}, ColorFromHSV(200, 0.7f, 0.7f));
	DrawPlane((Vector3){0, -0.01, 0}, (Vector2){50, 50}, GRAY);

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
		Vector3 pos = Vector3Lerp(p->previous_position, p->position, alpha); // bottom center
		BoundingBall head;
		BoundingBox body;
		player_getBody(p, &head, &body, &pos);
		if (!p->dead) {
			Vector3 center = Vector3Lerp(body.min, body.max, 0.5f);
			Vector3 size = Vector3Subtract(body.max, body.min);
			DrawCubeV(center, size, p->team == 1 ? PINK : SKYBLUE);
			DrawSphere(head.center, head.radius, p->team == 1 ? PINK : SKYBLUE);
			DrawLine3D(p->camera.position, p->camera.target, GREEN);
		} else {
			pos.y += PLAYER_BODY_WIDTH/2;
			DrawCube(pos, PLAYER_BODY_HEIGHT, PLAYER_BODY_WIDTH, PLAYER_BODY_WIDTH, p->team == 1 ? RED : BLUE);
			pos.x += PLAYER_BODY_HEIGHT/2 + PLAYER_HEAD_OFFSET;
			DrawSphere(pos, PLAYER_HEAD_RADIUS, p->team == 1 ? RED : BLUE);
		}
	}

	particle_render(g.me->camera, alpha);

	EndMode3D();

	// 2D content
	const float zm = g.zoom;

	const int window_width = GetScreenWidth();
	const int window_height = GetScreenHeight();

	// weapons
	DrawRectangleLinesEx((Rectangle){0, 0, 300*zm, 100*zm}, 4*zm, GetColor(0xffffff2f));
	DrawRectangle(4*zm, 4*zm, 292*zm, 92*zm, GetColor(0x000000af));
	DrawText(weapon_name(g.me->weapons.left.base), 10*zm, 10*zm, 24*g.zoom, WHITE);
	DrawRectangleLinesEx((Rectangle){0, 96*zm, 300*zm, 100*zm}, 4*zm, GetColor(0xffffff2f));
	DrawRectangle(4*zm, 100*zm, 292*zm, 92*zm, GetColor(0x000000af));
	DrawText(weapon_name(g.me->weapons.right.base), 10*zm, 106*zm, 24*g.zoom, WHITE);
	DrawRectangleLinesEx((Rectangle){0, 192*zm, 200*zm, 100*zm}, 4*zm, GetColor(0xffffff2f));
	DrawRectangle(4*zm, 196*zm, 192*zm, 92*zm, GetColor(0x000000af));
	DrawText(weapon_name(g.me->weapons.hand.base), 10*zm, 202*zm, 24*g.zoom, WHITE);
	if (g.me->holding == &g.me->weapons.left) {
		DrawRectangleLinesEx((Rectangle){0, 0, 300*zm, 100*zm}, 4*zm, YELLOW);
	} else if (g.me->holding == &g.me->weapons.right) {
		DrawRectangleLinesEx((Rectangle){0, 96*zm, 300*zm, 100*zm}, 4*zm, YELLOW);
	} else if (g.me->holding == &g.me->weapons.hand) {
		DrawRectangleLinesEx((Rectangle){0, 192*zm, 200*zm, 100*zm}, 4*zm, YELLOW);
	}

	// health
	float x = 20*zm;
	float y = window_height - 20*zm;
	y -= 40*zm;
	float w = 500*zm;
	float h = 40*zm;
	DrawRectangle(x, y, w, h, GetColor(0x000000af));
	DrawRectangleLines(x, y, w, h, WHITE);
	float t = 2*zm;
	DrawRectangle(x + 2*t, y + 2*t, (w - 4*t)*g.me->health/PLAYER_FULL_HEALTH, h - 4*t, WHITE);

	// record
	y -= 20*zm + 24*zm;
	x += 8*zm;
	DrawRectangleGradientH(x - 8*zm, y - 6*zm, 200*zm, 36*zm, GetColor(0x000000af), BLANK);
	char buf[16];
	snprintf(buf, 16, "%d / %d / %d", g.me->record.kill, g.me->record.death, g.me->record.assist);
	DrawText(buf, x, y, 24*g.zoom, WHITE);

	// scope
	if (g.me->scoping) {
		DrawCircle(window_width/2, window_height/2, 2*zm, RED);
		DrawRing((Vector2){window_width/2.0f, window_height/2.0f}, 470*zm, 520*zm, 0, 360, 50, BLACK);
		DrawRing((Vector2){window_width/2.0f, window_height/2.0f}, 450*zm, 470*zm, 0, 360, 50, DARKGRAY);
	} else {
		DrawLine(window_width/2.0f - 8*zm, window_height/2.0f, window_width/2.0f + 8*zm, window_height/2.0f, WHITE);
		DrawLine(window_width/2.0f, window_height/2.0f - 8*zm, window_width/2.0f, window_height/2.0f + 8*zm, WHITE);
	}

	y -= 48*zm;
	snprintf(buf, 4, "%d", GetFPS());
	DrawText(buf, x, y, 24*zm, WHITE);
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
		Player *p = &g.players[i];
		p->id = i + 1;
		p->team = i < (max_players+1)/2 ? 1 : 2;
		player_init(p);
		if (p->team == 1) {
			p->position = (Vector3){p->id*2 - 8, 2.5f, -19};
			p->rotation.yaw = +PI/2;
		} else {
			p->position = (Vector3){(p->id - 4)*2 - 4, 2.5f, 19};
			p->rotation.yaw = -PI/2;
		}
	}

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

Player *getPlayer(int id) {
	return &g.players[id - 1]; // TODO: improve this
}
