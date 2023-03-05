#ifndef INCLUDE_AXXEGRO_EVENT_EVENTSOURCE
#define INCLUDE_AXXEGRO_EVENT_EVENTSOURCE

#include <allegro5/allegro.h>

#include "../resources/Resource.hpp"
#include "UserEvent.hpp"

#include <allegro5/events.h>
#include <type_traits>

namespace al {

	class EventSource {
	public:
		virtual ALLEGRO_EVENT_SOURCE* ptr() const = 0;
	};

	class UserEventSource: public EventSource
	{
		ALLEGRO_EVENT_SOURCE evs {};
	public:
		UserEventSource(const UserEventSource&) = delete;
		UserEventSource& operator=(const UserEventSource&) = delete;

		UserEventSource() {
			al_init_user_event_source(&evs);
			al_set_event_source_data(&evs, (intptr_t)this);
		}

		~UserEventSource() {
			al_destroy_user_event_source(&evs);
		}

		[[nodiscard]] ALLEGRO_EVENT_SOURCE* ptr() const override {
			return (ALLEGRO_EVENT_SOURCE*)(&evs);
		}

		template<class UEvT> //TODO user event type constraint
		void emitEvent(const std::unique_ptr<typename UEvT::DataType> dat) {
			ALLEGRO_EVENT ev = UEvT::Create(dat);
			emitAllegroEvent<UEvT>(ev);
		}
		
		template<class UEvT>
		void emitEvent(const typename UEvT::DataType& dat) {
			ALLEGRO_EVENT ev = UEvT::Create(dat);
			emitAllegroEvent<UEvT>(ev);
		}
	private:
		template<class UEvT>
		void emitAllegroEvent(ALLEGRO_EVENT& ev)
		{
			if constexpr(UEvT::HasOptimization) {
				if constexpr(UEvT::CanSkipDtor) {
					al_emit_user_event(&evs, &ev, nullptr);
				} else {
					al_emit_user_event(&evs, &ev, OptUserEventDtor<UEvT>);
				}
			} else {
				al_emit_user_event(&evs, &ev, DefaultUserEventDtor<UEvT>);
			}
		}

	};

	inline UserEventSource& GetUserEventSource(const ALLEGRO_EVENT& ev) {
		if(ev.type < 1024) {
			throw EventSourceError("GetUserEventSource called on non-user event");
		}
		return *(UserEventSource*)al_get_event_source_data(ev.any.source);
	}
}

#endif /* INCLUDE_AXXEGRO_EVENT_EVENTSOURCE */
