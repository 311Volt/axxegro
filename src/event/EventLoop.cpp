#include <axxegro/event/EventLoop.hpp>

#include <axxegro/io/Keyboard.hpp>
#include <axxegro/io/Mouse.hpp>

#include <axxegro/time/Time.hpp>
#include <axxegro/display/Display.hpp>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

al::EventLoop::EventLoop()
{
	exitFlag = false;
	tick = 0;
	loopBody = [](){};
	lastTickTime = 0.01;
}

al::EventLoop::EventLoop(al::EventLoop::BasicInit)
	: EventLoop()
{
	initDefaultEventQueue();
	initDefaultDispatcher();
}

al::EventLoop::~EventLoop()
{

}


void al::EventLoop::enableFramerateLimit(double freq)
{
	clockTimer = std::make_unique<Timer>(FreqToPeriod(freq));
	clockEventQueue.registerSource(clockTimer->getEventSource());
	clockTimer->start();
}
void al::EventLoop::disableClock()
{
	clockTimer.reset();
}

void al::EventLoop::initDefaultEventQueue()
{
	eventQueue.registerSource(GetMouseEventSource());
	eventQueue.registerSource(GetKeyboardEventSource());
	eventQueue.registerSource(al::CurrentDisplay.eventSource());
}
void al::EventLoop::initDefaultDispatcher()
{
	eventDispatcher.setEventTypeHandler(ALLEGRO_EVENT_DISPLAY_CLOSE, [this](const ALLEGRO_EVENT&){
		exitFlag = true;
	});
	
	eventDispatcher.setEventTypeHandler(ALLEGRO_EVENT_DISPLAY_RESIZE, [](const ALLEGRO_EVENT&){
		CurrentDisplay.acknowledgeResize();
	});
}

void al::EventLoop::enableEscToQuit()
{
	auto keycodeDiscretizer = eventDispatcher.addDiscretizer({ALLEGRO_EVENT_KEY_DOWN, [](const ALLEGRO_EVENT& ev){
		return ev.keyboard.keycode;
	}});

	eventDispatcher.setEventValueHandler(keycodeDiscretizer, ALLEGRO_KEY_ESCAPE, [this](const ALLEGRO_EVENT&){
		exitFlag = true;
	});
}

al::EventLoop al::EventLoop::Basic()
{
	return EventLoop(BasicInit());
}

int64_t al::EventLoop::getTick()
{
	return tick;
}

double al::EventLoop::getLastTickTime()
{
	return lastTickTime;
}

void al::EventLoop::run()
{
	while(!exitFlag) {
		double t0 = GetTime();
		while(!eventQueue.empty()) {
			ALLEGRO_EVENT ev = eventQueue.pop();
			eventDispatcher.dispatch(ev);
		}
		if(clockTimer) {
			clockEventQueue.wait();
			clockEventQueue.flush();
		}
		loopBody();
		lastTickTime = GetTime() - t0;
		tick++;
	}
	exitFlag = false;
}

void al::EventLoop::setExitFlag()
{
	exitFlag = true;
}