#ifndef _TDM_player_h
#define _TDM_player_h

#include <raylib.h>
#include "weapon.h"

typedef struct Player {
	int id;
	int team;
	Vector3 position; // bottom center
	Vector3 velocity;
	Vector3 previous_position; // for render alpha
	float health;

	struct PlayerInput {
		int forward, back, left, right;
		bool jump, crouch, lie;
		bool scope, peekl, peekr;
		bool fire;
	} input;
	Camera camera;

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

void player_updateCameara(Player *p, Vector3 position);

#endif
