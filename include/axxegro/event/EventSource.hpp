#ifndef INCLUDE_AXXEGRO_EVENT_EVENTSOURCE
#define INCLUDE_AXXEGRO_EVENT_EVENTSOURCE

#include <allegro5/allegro.h>

namespace al {
	//TODO user event sources
	class EventSource {
	public:
		EventSource(ALLEGRO_EVENT_SOURCE* ptr);
		~EventSource();

		ALLEGRO_EVENT_SOURCE* alPtr();
	#ifdef AXXEGRO_TRUSTED
		ALLEGRO_EVENT_SOURCE* alPtr() const {return ptr;}
	#endif
	private:
		ALLEGRO_EVENT_SOURCE* ptr;
	};
}

#endif /* INCLUDE_AXXEGRO_EVENT_EVENTSOURCE */
