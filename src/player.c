#include "player.h"
#include <raylib.h>

void player_init(Player *p) {
	if (p->id == 0 || p->team == 0) {
		TraceLog(LOG_ERROR, "id or team not set");
		return;
	}

	p->camera.position = (Vector3) {p->id*2 - 10, 1.7f, p->team == 1 ? -10 : 10};
	p->camera.target = (Vector3) {10.0f, 2.0f, 10.0f};
	p->camera.up = (Vector3) {0, 1, 0};
	p->camera.fovy = 90;
	p->camera.projection = CAMERA_PERSPECTIVE;
}

void player_update(Player *player) {
	;
}
