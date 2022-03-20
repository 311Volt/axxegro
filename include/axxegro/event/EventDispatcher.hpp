#ifndef INCLUDE_AXXEGRO_EVENT_EVENTDISPATCHER
#define INCLUDE_AXXEGRO_EVENT_EVENTDISPATCHER

#include "EventQueue.hpp"
#include <functional>
#include <unordered_map>
#include <unordered_set>

/**
 * @file A high-level mechanism for event dispatching.
 * 
 * this code is some uncanny valley shit and in desperate need of better naming
 * and all-around consistency - do NOT consider anything in here stable at all
 */

namespace al {
	using EventHandler = std::function<void(const ALLEGRO_EVENT&)>;

	class EventDispatcher {
	public:
		using EventDiscretizerId = uint32_t;
		struct EventDiscretizer {
			ALLEGRO_EVENT_TYPE type;
			std::function<int64_t(const ALLEGRO_EVENT&)> fn;
		};
		enum class DispatchLevel: uint8_t {
			DEFAULT=0,
			MATCHED_TYPE=1,
			MATCHED_VALUE=2
		};

		EventDispatcher();
		~EventDispatcher();
		
		void setDefaultHandler(EventHandler handler);
		void setEventTypeHandler(ALLEGRO_EVENT_TYPE evType, EventHandler handler);
		void setEventValueHandler(EventDiscretizerId discrId, int64_t value, EventHandler handler);

		EventDiscretizerId addDiscretizer(EventDiscretizer discretizer);
		void removeDiscretizer(EventDiscretizerId id);
		
		DispatchLevel dispatch(const ALLEGRO_EVENT& event);
	private:
		EventDiscretizerId createDiscretizerId();

		EventHandler defaultHandler;
		std::unordered_map<ALLEGRO_EVENT_TYPE, EventHandler> eventTypeHandler;
		std::unordered_map<ALLEGRO_EVENT_TYPE, std::unordered_set<EventDiscretizerId>> typeDiscretizers;
		std::unordered_map<EventDiscretizerId, EventDiscretizer> discretizers;
		std::unordered_map<EventDiscretizerId, std::unordered_map<int64_t, EventHandler>> eventValueHandler;
	};
}

#endif /* INCLUDE_AXXEGRO_EVENT_EVENTDISPATCHER */
