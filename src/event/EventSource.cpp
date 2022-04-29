#include <axxegro/event/EventSource.hpp>

al::UserEventSource& al::GetUserEventSource(const ALLEGRO_EVENT& ev)
{
	if(ev.type < 1024) {
		std::runtime_error("GetUserEventSource called on non-user event");
	}
	return *(UserEventSource*)al_get_event_source_data(ev.any.source);
}

