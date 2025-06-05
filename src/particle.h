#ifndef _ICRAFT_effects_particle_h
#define _ICRAFT_effects_particle_h

/* particles do not influence any game logic */

#include <stdbool.h>
#include <raylib.h>

typedef struct Particle {
	bool alive;
	Vector3 pos;
	// Vector3 pre_pos;
	// Vector3 v;
	float alpha;
	float size;
	Color color;
} Particle;

// return pointer to the Particle struct, but
// NOTE: this pointer can only be temporatily used to like set lifetime or size
// you should not keep the pointer because address can change when inner expand
// happens
Particle *particle_spawn(Vector3 position, Color color);

void particle_update();
void particle_render(Camera cam, float alpha);

#endif
