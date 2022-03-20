#include <axxegro/time/Timer.hpp>

al::Timer::Timer(double period)
	: evSrc(nullptr)
{
	ptr = al_create_timer(period);
	evSrc = EventSource(al_get_timer_event_source(ptr));
}

al::Timer::~Timer()
{
	al_destroy_timer(ptr);
}

al::Timer al::Timer::Freq(double freq)
{
	return Timer(FreqToPeriod(freq));
}
al::Timer al::Timer::Period(double period)
{
	return Timer(period);
}

ALLEGRO_TIMER* al::Timer::alPtr()
{
	return ptr;
}

void al::Timer::start()
{
	al_start_timer(ptr);
}
void al::Timer::resume()
{
	al_resume_timer(ptr);
}
void al::Timer::stop()
{
	al_stop_timer(ptr);
}

bool al::Timer::isStarted()
{
	return al_get_timer_started(ptr);
}

const al::EventSource& al::Timer::getEventSource() const
{
	return evSrc;
}

int64_t al::Timer::getCount() const
{
	return al_get_timer_count(ptr);
}
void al::Timer::setCount(int64_t value)
{
	al_set_timer_count(ptr, value);
}

double al::Timer::getPeriod() const
{
	return al_get_timer_speed(ptr);
}
void al::Timer::setPeriod(double value)
{
	al_set_timer_speed(ptr, value);
}

double al::Timer::getFreq() const
{
	return PeriodToFreq(getPeriod());
}
void al::Timer::setFreq(double value)
{
	setPeriod(FreqToPeriod(value));
}