#ifndef INCLUDE_AXXEGRO_EVENT_EVENTDISPATCHER
#define INCLUDE_AXXEGRO_EVENT_EVENTDISPATCHER

#include "EventQueue.hpp"

#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <utility>

/**
 * A high-level mechanism for event dispatching.
 * 
 * not documenting this because this is due for a complete rewrite
 */

namespace al {
	using EventHandler = std::function<void(const ALLEGRO_EVENT&)>;

	class EventDispatcher {
	public:
		using EventDiscretizerId = uint32_t;
		struct EventDiscretizer {
			ALLEGRO_EVENT_TYPE type = -1;
			std::function<int64_t(const ALLEGRO_EVENT&)> fn;
		};
		enum class DispatchLevel: uint8_t {
			Default=0,
			MatchedType=1,
			MatchedValue=2
		};

		EventDispatcher() {
			defaultHandler = [](const auto&){};
		}
		~EventDispatcher() = default;
		
		void setDefaultHandler(EventHandler handler) {
			defaultHandler = std::move(handler);
		}
		void setEventTypeHandler(ALLEGRO_EVENT_TYPE evType, EventHandler handler) {
			eventTypeHandler[evType] = std::move(handler);
		}
		void setEventValueHandler(EventDiscretizerId discrId, int64_t value, EventHandler handler) {
			if(!discretizers.count(discrId)) {
				throw EventDispatcherError("cannot set value handler: discretizer with id=%d does not exist", discrId);
			}
			eventValueHandler[discrId][value] = std::move(handler);
		}

		void deleteEventTypeHandler(ALLEGRO_EVENT_TYPE evType) {
			eventTypeHandler.erase(evType);
		}

		void deleteEventValueHandler(EventDiscretizerId discrId, int64_t value) {
			if(typeDiscretizers.count(discrId)) {
				typeDiscretizers[discrId].erase(value);
			}
		}

		EventDiscretizerId addDiscretizer(const EventDiscretizer& discretizer) {
			auto newId = createDiscretizerId();
			discretizers[newId] = discretizer;
			typeDiscretizers[discretizer.type].insert(newId);
			return newId;
		}

		void removeDiscretizer(EventDiscretizerId id) {
			typeDiscretizers[discretizers[id].type].erase(id);
			discretizers.erase(id);
			eventValueHandler.erase(id);
		}
		
		DispatchLevel checkDispatchLevel(const ALLEGRO_EVENT& event) {
			if(typeDiscretizers.count(event.type)) {
				for(const auto& discretizerId: typeDiscretizers[event.type]) {
					int64_t value = discretizers[discretizerId].fn(event);
					if(eventValueHandler.count(discretizerId)) {
						if(eventValueHandler[discretizerId].count(value)) {
							return DispatchLevel::MatchedValue;
						}
					}
				}
			}
			if(eventTypeHandler.count(event.type) && eventTypeHandler[event.type]) {
				return DispatchLevel::MatchedType;
			}
			return DispatchLevel::Default;
		}
		DispatchLevel dispatch(const ALLEGRO_EVENT& event) {
			//try level 2 (match by type and discretized value)
			if(typeDiscretizers.count(event.type)) {
				for(const auto& discretizerId: typeDiscretizers[event.type]) {
					int64_t value = discretizers[discretizerId].fn(event);
					if(eventValueHandler.count(discretizerId)) {
						if(eventValueHandler[discretizerId].count(value)) {
							eventValueHandler[discretizerId][value](event);
							return DispatchLevel::MatchedValue;
						}
					}
				}
			}

			//if no match, try level 1 (match by event type)
			if(eventTypeHandler.count(event.type) && eventTypeHandler[event.type]) {
				eventTypeHandler[event.type](event);
				return DispatchLevel::MatchedType;
			}

			//if no match, call default handler (level 0)
			defaultHandler(event);
			return DispatchLevel::Default;
		}
	private:
		static EventDiscretizerId createDiscretizerId() {
			static EventDiscretizerId counter = 1;
			return counter++;
		}

		EventHandler defaultHandler;
		std::unordered_map<ALLEGRO_EVENT_TYPE, EventHandler> eventTypeHandler;
		std::unordered_map<ALLEGRO_EVENT_TYPE, std::unordered_set<EventDiscretizerId>> typeDiscretizers;
		std::unordered_map<EventDiscretizerId, EventDiscretizer> discretizers;
		std::unordered_map<EventDiscretizerId, std::unordered_map<int64_t, EventHandler>> eventValueHandler;
	};
}

#endif /* INCLUDE_AXXEGRO_EVENT_EVENTDISPATCHER */
