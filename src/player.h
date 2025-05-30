#ifndef _TDM_player_h
#define _TDM_player_h

#include <raylib.h>
#include "weapon.h"

typedef struct Player {
	int id;
	int team;
	Camera camera; // also eye position
	float health;

	struct {
		Weapon left;
		Weapon right;
		Weapon hand;
	} weapons;
	Weapon *holding; // null or pointer to one of the three up
	bool has_frag_grenade;
	bool has_stun_grenade;

	struct {
		int kill;
		int death;
		int assist;
	} record;
} Player;

void player_init(Player *player);
void player_update(Player *player);

#endif
