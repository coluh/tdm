#include "config.h"
#include "flow.h"
#include "game.h"
#include "weapon.h"
#include <raylib.h>
#include <stddef.h>

extern Game g;

int main() {

	InitWindow(1000, 700, "Team Deathmatch");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetWindowState(FLAG_WINDOW_UNDECORATED);
	MaximizeWindow();
	SetTargetFPS(60);

	init_weapons();
	config_load(NULL);
	flow_init();

	game_loop(0, 8);

	CloseWindow();

	return 0;
}
