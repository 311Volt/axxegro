#ifndef INCLUDE_AXXEGRO_EVENT_EVENTSOURCE
#define INCLUDE_AXXEGRO_EVENT_EVENTSOURCE


#include "../../common.hpp"

namespace al {

	class EventSource {
	public:
		virtual ALLEGRO_EVENT_SOURCE* ptr() const = 0;
	};

}

#endif /* INCLUDE_AXXEGRO_EVENT_EVENTSOURCE */
