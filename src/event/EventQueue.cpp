#define AXXEGRO_TRUSTED

#include <axxegro/event/EventQueue.hpp>

#include <stdexcept>
#include <optional>

al::EventQueue::EventQueue()
{
	ptr = al_create_event_queue();
	if(!ptr) {
		throw std::runtime_error("Failed to create Allegro event queue");
	}
}

al::EventQueue::~EventQueue()
{
	al_destroy_event_queue(ptr);
}


void al::EventQueue::registerSource(ALLEGRO_EVENT_SOURCE* src)
{
	al_register_event_source(ptr, src);
}
void al::EventQueue::registerSource(const al::EventSource& src)
{
	al_register_event_source(ptr, src.alPtr());
}

bool al::EventQueue::empty()
{
	return al_is_event_queue_empty(ptr);
}

ALLEGRO_EVENT al::EventQueue::pop()
{
	if(empty()) {
		throw std::runtime_error("pop() called on an empty event queue. always check the queue with empty()");
	}
	ALLEGRO_EVENT ret;
	al_get_next_event(ptr, &ret);
	return ret;
}

ALLEGRO_EVENT al::EventQueue::peek()
{
	if(empty()) {
		throw std::runtime_error("peek() called on an empty event queue. always check the queue with empty()");
	}
	ALLEGRO_EVENT ret;
	al_peek_next_event(ptr, &ret);
	return ret;
}

bool al::EventQueue::drop()
{
	return al_drop_next_event(ptr);
}

void al::EventQueue::clear()
{
	al_flush_event_queue(ptr);
}

ALLEGRO_EVENT al::EventQueue::wait()
{
	ALLEGRO_EVENT ret;
	al_wait_for_event(ptr, &ret);
	return ret;
}

std::optional<ALLEGRO_EVENT> al::EventQueue::waitFor(double seconds)
{
	ALLEGRO_EVENT ev;
	if(!al_wait_for_event_timed(ptr, &ev, seconds))
		return std::nullopt;
	return {ev};
}

ALLEGRO_EVENT_QUEUE* al::EventQueue::alPtr()
{
	return ptr;
}
