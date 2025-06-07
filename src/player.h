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
// player body top to head center distance
#define PLAYER_HEAD_OFFSET 0.3f
// player head radius
#define PLAYER_HEAD_RADIUS 0.25f

#define PLAYER_FULL_HEALTH 10.0f

typedef struct Player {
	int id;
	int team;
	struct {
		float yaw, pitch;
	} rotation;
	Vector3 position; // bottom center
	Vector3 velocity;
	Vector3 previous_position; // for render alpha

	struct PlayerInput {
		int forward, back, left, right;
		bool jump, crouch, lie;
		bool scope/*, peekl, peekr*/;
		bool fire;
	} input;
	Camera camera;

	float health;
	bool dead;
	float reborn_timer;
	bool crouching;
	bool lying;
	bool scoping;
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
typedef struct World World;

void player_init(Player *player);
void player_update(Player *player, World *world);

void player_updateCameara(Player *player, Vector3 position, const World *world);
void player_getBody(const Player *player, BoundingBall *head, BoundingBox *body, const Vector3 *pos);

#endif
