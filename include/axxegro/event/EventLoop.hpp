#ifndef INCLUDE_AXXEGRO_EVENT_EVENTLOOP
#define INCLUDE_AXXEGRO_EVENT_EVENTLOOP

#include "EventQueue.hpp"
#include "EventDispatcher.hpp"
#include "../time/Timer.hpp"

#include <unordered_map>
#include <optional>
#include <memory>
#include <functional>

#include <allegro5/allegro.h>

namespace al {
	class EventLoop {
	public:
		EventLoop();
		~EventLoop();
		
		EventQueue eventQueue;
		EventDispatcher eventDispatcher;
		std::function<void(void)> loopBody;

		void enableClock(double freq);
		void disableClock();

		void initDefaultEventQueue();
		void initDefaultDispatcher();
		void run();

		int64_t getTick();
	private:
		std::unique_ptr<Timer> clockTimer;
		EventQueue clockEventQueue;
		int64_t tick;
		bool exitFlag;
	};
}

#endif /* INCLUDE_AXXEGRO_EVENT_EVENTLOOP */
