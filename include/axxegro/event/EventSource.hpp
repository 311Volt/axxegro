#ifndef INCLUDE_AXXEGRO_EVENT_EVENTSOURCE
#define INCLUDE_AXXEGRO_EVENT_EVENTSOURCE

#include <allegro5/allegro.h>

namespace al {
	//TODO user event sources

	class EventSource {
	public:
		virtual ALLEGRO_EVENT_SOURCE* ptr() const = 0;
	};
	
	class UserEventSource: public EventSource {
		//TODO
	};
}

#endif /* INCLUDE_AXXEGRO_EVENT_EVENTSOURCE */
