#include "event.h"
#include <raylib.h>

static EventListener listeners[Event_COUNT][MAX_EVENT_LISTENERS];
static int listener_counts[Event_COUNT];

void event_addListener(EventType type, EventListener listener) {
	if (listener_counts[type] < MAX_EVENT_LISTENERS) {
		listeners[type][listener_counts[type]++] = listener;
	} else {
		TraceLog(LOG_WARNING, "Too many listers for type %d", type);
	}
}

void event_dispatch(Event *ev) {
	for (int i = 0; i < listener_counts[ev->type]; i++) {
		listeners[ev->type][i](ev);
	}
}
