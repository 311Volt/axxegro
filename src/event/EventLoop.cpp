#include <axxegro/event/EventLoop.hpp>

#include <axxegro/io/Keyboard.hpp>
#include <axxegro/io/Mouse.hpp>

#include <fmt/format.h>

al::EventLoop::EventLoop()
{
	exitFlag = false;
	tick = 0;
	loopBody = [](){};
}

al::EventLoop::~EventLoop()
{

}


void al::EventLoop::enableClock(double freq)
{
	clockTimer = std::make_unique<Timer>(FreqToPeriod(freq));
	clockEventQueue.registerSource(clockTimer->getEventSource());
	clockTimer->start();
}
void al::EventLoop::disableClock()
{
	clockTimer.release();
}

void al::EventLoop::initDefaultEventQueue()
{
	eventQueue.registerSource(keyb::GetEventSource());
	eventQueue.registerSource(mouse::GetEventSource());
	eventQueue.registerSource(EventSource(al_get_display_event_source(al_get_current_display())));
}
void al::EventLoop::initDefaultDispatcher()
{
	eventDispatcher.setEventTypeHandler(ALLEGRO_EVENT_DISPLAY_CLOSE, [this](const ALLEGRO_EVENT& ev){
		exitFlag = true;
	});
	auto keycodeDiscretizer = eventDispatcher.addDiscretizer({ALLEGRO_EVENT_KEY_DOWN, [this](const ALLEGRO_EVENT& ev){
		return ev.keyboard.keycode;
	}});
	
	eventDispatcher.setEventValueHandler(keycodeDiscretizer, ALLEGRO_KEY_ESCAPE, [this](const ALLEGRO_EVENT& ev){
		exitFlag = true;
	});
}

int64_t al::EventLoop::getTick()
{
	return tick;
}

void al::EventLoop::run()
{
	while(!exitFlag) {
		while(!eventQueue.empty()) {
			ALLEGRO_EVENT ev = eventQueue.pop();
			eventDispatcher.dispatch(ev);
		}
		clockEventQueue.wait();
		clockEventQueue.flush();
		loopBody();
		tick++;
	}
	exitFlag = false;
}