#include "config.h"
#include "game.h"
#include "weapon.h"
#include <raylib.h>
#include <stddef.h>

extern Game g;

int main() {

	init_weapons();
	config_load(NULL);

	InitWindow(1000, 700, "Team Deathmatch");
	SetTargetFPS(60);

	game_loop(0, 8);

	CloseWindow();

	return 0;
}
