#ifndef _TDM_event_h
#define _TDM_event_h

#define MAX_EVENT_LISTENERS 32

typedef enum EventType {
	Event_KILL,

	Event_COUNT,
} EventType;

typedef struct Event {
	EventType type;
	union {
		struct KillEvent {
			int killer_id;
			int dead_id;
		} kill;
	};
} Event;

typedef void (*EventListener)(Event *event);

void event_addListener(EventType type, EventListener listener);

// ask related listeners to handle this.
// event is passed in stacks
void event_dispatch(Event *event);

#endif
