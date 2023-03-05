#ifndef INCLUDE_AXXEGRO_EVENT_EVENTLOOP
#define INCLUDE_AXXEGRO_EVENT_EVENTLOOP

#include "EventQueue.hpp"
#include "EventDispatcher.hpp"
#include "../time/Timer.hpp"

#include <axxegro/io/Keyboard.hpp>
#include <axxegro/io/Mouse.hpp>

#include <axxegro/time/Time.hpp>
#include <axxegro/display/Display.hpp>

#include <unordered_map>
#include <optional>
#include <memory>
#include <functional>

#include <allegro5/allegro.h>

namespace al {
	class EventLoop {
		
		//copy elision for Basic()
		class BasicInit{};

		explicit EventLoop(BasicInit)
				: EventLoop()
		{
			initDefaultEventQueue();
			initDefaultDispatcher();
		}

	public:

		EventLoop() = default;
		~EventLoop() = default;

		void enableFramerateLimit(double freq) {
			clockTimer = std::make_unique<Timer>(FreqToPeriod(freq));
			clockEventQueue.registerSource(clockTimer->getEventSource());
			clockTimer->start();
		}
		void disableClock() {
			clockTimer.reset();
		}

		void initDefaultEventQueue() {
			eventQueue.registerSource(GetMouseEventSource());
			eventQueue.registerSource(GetKeyboardEventSource());
			eventQueue.registerSource(al::CurrentDisplay.eventSource());
		}

		void initDefaultDispatcher() {
			eventDispatcher.setEventTypeHandler(ALLEGRO_EVENT_DISPLAY_CLOSE, [this](const ALLEGRO_EVENT&){
				exitFlag = true;
			});

			eventDispatcher.setEventTypeHandler(ALLEGRO_EVENT_DISPLAY_RESIZE, [](const ALLEGRO_EVENT&){
				CurrentDisplay.acknowledgeResize();
			});
		}
		void enableEscToQuit() {
			auto keycodeDiscretizer = eventDispatcher.addDiscretizer({ALLEGRO_EVENT_KEY_DOWN, [](const ALLEGRO_EVENT& ev){
				return ev.keyboard.keycode;
			}});

			eventDispatcher.setEventValueHandler(keycodeDiscretizer, ALLEGRO_KEY_ESCAPE, [this](const ALLEGRO_EVENT&){
				exitFlag = true;
			});
		}
		void run() {
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
				double endTickTime = GetTime();
				lastTickTime = endTickTime - t0;
				fpsCounter++;
				if(endTickTime - lastFpsUpdateTime > 1.0) {
					fps = fpsCounter;
					fpsCounter = 0;
					lastFpsUpdateTime = endTickTime;
				}
				tick++;
			}
			exitFlag = false;
		}

		void setExitFlag() {
			exitFlag = true;
		}

		static EventLoop Basic() {
			return EventLoop(BasicInit());
		}

		int64_t getTick() const {
			return tick;
		}
		int64_t getFPS() const {
			return fps;
		}
		double getLastTickTime() const {
			return lastTickTime;
		}

		EventQueue eventQueue;
		EventDispatcher eventDispatcher;
		std::function<void(void)> loopBody = [](){};
	private:
		std::unique_ptr<Timer> clockTimer;
		EventQueue clockEventQueue;
		int64_t tick = 0;
		int64_t fps = 0;
		int64_t fpsCounter = 0;
		double lastFpsUpdateTime = -1.0;
		double lastTickTime = 0.01;
		bool exitFlag = false;
	};
}

#endif /* INCLUDE_AXXEGRO_EVENT_EVENTLOOP */
