#include "axxegro/Exception.hpp"
#include <axxegro/event/EventDispatcher.hpp>

#include <random>
#include <stdexcept>
#include <limits>
#include <string>

al::EventDispatcher::EventDispatcher()
{
	defaultHandler = [](const auto&){};
}
al::EventDispatcher::~EventDispatcher()
{

}

void al::EventDispatcher::setDefaultHandler(EventHandler handler)
{
	defaultHandler = handler;
}
void al::EventDispatcher::setEventTypeHandler(ALLEGRO_EVENT_TYPE evType, al::EventHandler handler)
{
	eventTypeHandler[evType] = handler;
}
void al::EventDispatcher::setEventValueHandler(al::EventDispatcher::EventDiscretizerId discrId, int64_t value, al::EventHandler handler)
{
	if(!discretizers.count(discrId)) {
		throw EventDispatcherError("cannot set value handler: discretizer with id={} does not exist", discrId);
	}
	eventValueHandler[discrId][value] = handler;
}

void al::EventDispatcher::deleteEventTypeHandler(ALLEGRO_EVENT_TYPE evType)
{
	eventTypeHandler.erase(evType);
}
void al::EventDispatcher::deleteEventValueHandler(EventDiscretizerId discrId, int64_t value)
{
	if(typeDiscretizers.count(discrId)) {
		typeDiscretizers[discrId].erase(value);
	}
}

al::EventDispatcher::EventDiscretizerId al::EventDispatcher::addDiscretizer(al::EventDispatcher::EventDiscretizer discretizer)
{
	auto newId = createDiscretizerId();
	discretizers[newId] = discretizer;
	typeDiscretizers[discretizer.type].insert(newId);
	return newId;
}
void al::EventDispatcher::removeDiscretizer(al::EventDispatcher::EventDiscretizerId id)
{
	typeDiscretizers[discretizers[id].type].erase(id);
	discretizers.erase(id);
	eventValueHandler.erase(id);
}

al::EventDispatcher::EventDiscretizerId al::EventDispatcher::createDiscretizerId()
{
	static EventDiscretizerId counter = 1;
	return counter++;
}

al::EventDispatcher::DispatchLevel al::EventDispatcher::checkDispatchLevel(const ALLEGRO_EVENT &event)
{
	if(typeDiscretizers.count(event.type)) {
		for(const auto& discretizerId: typeDiscretizers[event.type]) {
			int64_t value = discretizers[discretizerId].fn(event);
			if(eventValueHandler.count(discretizerId)) {
				if(eventValueHandler[discretizerId].count(value)) {
					return DispatchLevel::MATCHED_VALUE;
				}
			}
		}
	}
	if(eventTypeHandler.count(event.type) && eventTypeHandler[event.type]) {
		return DispatchLevel::MATCHED_TYPE;
	}
	return DispatchLevel::DEFAULT;
}

al::EventDispatcher::DispatchLevel al::EventDispatcher::dispatch(const ALLEGRO_EVENT& event)
{
	//try level 2 (match by type and discretized value)
	if(typeDiscretizers.count(event.type)) {
		for(const auto& discretizerId: typeDiscretizers[event.type]) {
			int64_t value = discretizers[discretizerId].fn(event);
			if(eventValueHandler.count(discretizerId)) {
				if(eventValueHandler[discretizerId].count(value)) {
					eventValueHandler[discretizerId][value](event);
					return DispatchLevel::MATCHED_VALUE;
				}
			}
		}
	}

	//if no match, try level 1 (match by event type)
	if(eventTypeHandler.count(event.type) && eventTypeHandler[event.type]) {
		eventTypeHandler[event.type](event);
		return DispatchLevel::MATCHED_TYPE;
	}

	//if no match, call default handler (level 0)
	defaultHandler(event);
	return DispatchLevel::DEFAULT;
}
