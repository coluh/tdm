#include "particle.h"
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include "config.h"
#include "game.h"

extern Game g;

static struct ParticlePool {
	Particle *particles;
	int capacity;
} pool;

Particle *particle_spawn(Vector3 position, Color color) {
	for (int i = 0; i < pool.capacity; i++) {
		Particle *p = &pool.particles[i];
		if (p->alive) {
			continue;
		}
		p->alive = true;
		p->pos = position;
		p->alpha = 1.0f;
		p->size = GetRandomValue(5, 8)/10.0f;
		p->color = color;
		return p;
	}
	if (pool.capacity == 0) {
		pool.capacity = 32;
		pool.particles = calloc(pool.capacity, sizeof(Particle));
	} else {
		pool.capacity *= 2;
		pool.particles = realloc(pool.particles, pool.capacity*sizeof(Particle));
	}
	return particle_spawn(position, color);
}

void particle_update() {
	for (int i = 0; i < pool.capacity; i++) {
		Particle *p = &pool.particles[i];
		if (p->alive) {
			p->alpha *= 0.77;
			p->color.a = 256*p->alpha;
			if (p->alpha < 0.01) {
				p->alive = false;
			}
		}
	}
}

void particle_render(Camera cam, float alpha) {
	for (int i = 0; i < pool.capacity; i++) {
		Particle *p = &pool.particles[i];
		if (p->alive) {
			DrawBillboard(cam, config_get()->textures.fog, p->pos, p->size, p->color);
		}
	}
}
