#ifndef _TDM_game_h
#define _TDM_game_h

#include <stdbool.h>
#include <raylib.h>

typedef struct World World;
typedef struct Player Player;

// struct we need in a game round
typedef struct Game {
	World *world; // map we are playing
	Player *players; // pointer to an array of players
	int max_players;

	Player *me; // pointer to one of the player in players array above

	bool running;
	double delta; // fixed timestep for updating
} Game;

// start a game round
void game_loop(int world_index, int max_players);

#endif
