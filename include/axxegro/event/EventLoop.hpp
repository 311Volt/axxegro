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
		
		//copy elision for Basic()
		class BasicInit{};
		EventLoop(BasicInit);
	public:

		EventLoop();
		~EventLoop();
		
		EventQueue eventQueue;
		EventDispatcher eventDispatcher;
		std::function<void(void)> loopBody;

		void enableFramerateLimit(double freq);
		void disableClock();

		void initDefaultEventQueue();
		void initDefaultDispatcher();
		void enableEscToQuit();
		void run();

		void setExitFlag();

		static EventLoop Basic();

		int64_t getTick();
		int64_t getFPS();
		double getLastTickTime();
	private:
		std::unique_ptr<Timer> clockTimer;
		EventQueue clockEventQueue;
		int64_t tick;
		int64_t fps;
		int64_t fpsCounter;
		double lastFpsUpdateTime;
		double lastTickTime;
		bool exitFlag;
	};
}

#endif /* INCLUDE_AXXEGRO_EVENT_EVENTLOOP */
