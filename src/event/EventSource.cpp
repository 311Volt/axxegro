#include <axxegro/event/EventSource.hpp>

al::EventSource::EventSource(ALLEGRO_EVENT_SOURCE* ptr)
	: ptr(ptr)
{

}
al::EventSource::~EventSource()
{

}

ALLEGRO_EVENT_SOURCE* al::EventSource::alPtr()
{
	return ptr;
}