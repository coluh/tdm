#ifndef _TDM_player_h
#define _TDM_player_h

#include <raylib.h>
#include "types.h"
#include "weapon.h"

// player body width
#define PLAYER_BODY_WIDTH 0.5f
// player body height
#define PLAYER_BODY_HEIGHT 1.4f
// player body height when squating
#define PLAYER_BODY_CROUCH_HEIGHT 0.8f
// player body center to head center distance
#define PLAYER_HEAD_OFFSET 1.0f
// player head radius
#define PLAYER_HEAD_RADIUS 0.25f

typedef struct Player {
	int id;
	int team;
	Vector3 position; // bottom center
	Vector3 velocity;
	Vector3 previous_position; // for render alpha
	float health;
	bool crouching;
	bool lying;

	struct PlayerInput {
		int forward, back, left, right;
		bool jump, crouch, lie;
		bool scope/*, peekl, peekr*/;
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
void player_getBody(Player *p, BoundingBall *head, BoundingBox *body);

#endif
