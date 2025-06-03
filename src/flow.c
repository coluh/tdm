#include "flow.h"
#include "event.h"
#include "game.h"
#include "player.h"

#include <raylib.h>

extern Game g;

void on_killed(Event *ev) {
	TraceLog(LOG_INFO, "%d killed %d!", ev->kill.killer_id, ev->kill.dead_id);
	Player *killer = getPlayer(ev->kill.killer_id);
	Player *dead = getPlayer(ev->kill.dead_id);
	killer->record.kill++;
	dead->record.death++;

	// reborn
	dead->health = PLAYER_FULL_HEALTH;
}

void flow_init() {
	event_addListener(Event_KILL, on_killed);
}
