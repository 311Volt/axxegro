#ifndef INCLUDE_AXXEGRO_CORE_EVENT_EVENTLOOP
#define INCLUDE_AXXEGRO_CORE_EVENT_EVENTLOOP

#include "EventQueue.hpp"
#include "LegacyEventDispatcher.hpp"
#include "EventDispatcher.hpp"

#include "../../common.hpp"
#include "../display/Display.hpp"
#include "../time.hpp"
#include "../io.hpp"
#include "../time/FramerateLimiter.hpp"
#include "../time/FPSCounter.hpp"

#include <unordered_map>
#include <optional>
#include <memory>
#include <functional>


namespace al {

	struct EventLoopConfig {
		bool registerKeyboardEventSource = true;
		bool registerMouseEventSource = true;
		bool registerCurrentDisplayEventSource = true;
		bool quitOnDisplayClosed = true;
		bool quitOnEscPressed = false;
		bool autoAcknowledgeResize = true;
		FramerateLimit framerateLimit = FramerateLimiterMode::None{};
	};

	inline constexpr EventLoopConfig EmptyEventLoopConfig = {
		.registerKeyboardEventSource = false,
		.registerMouseEventSource = false,
		.registerCurrentDisplayEventSource = false,
		.quitOnDisplayClosed = false,
		.quitOnEscPressed = false,
		.autoAcknowledgeResize = false
	};

	inline constexpr EventLoopConfig BasicEventLoopConfig = {
		.registerKeyboardEventSource = true,
		.registerMouseEventSource = true,
		.registerCurrentDisplayEventSource = true,
		.quitOnDisplayClosed = true,
		.quitOnEscPressed = false,
		.autoAcknowledgeResize = true
	};

	inline constexpr EventLoopConfig DemoEventLoopConfig = {
		.registerKeyboardEventSource = true,
		.registerMouseEventSource = true,
		.registerCurrentDisplayEventSource = true,
		.quitOnDisplayClosed = true,
		.quitOnEscPressed = true,
		.autoAcknowledgeResize = true,
		.framerateLimit = FramerateLimiterMode::Auto{}
	};



	class EventLoop: RequiresInitializables<CoreAllegro> {

	public:
		~EventLoop() = default;

		explicit EventLoop(EventLoopConfig config = BasicEventLoopConfig) {

			if(config.registerKeyboardEventSource) {
				eventQueue.registerSource(GetKeyboardEventSource());
			}
			if(config.registerMouseEventSource) {
				eventQueue.registerSource(GetMouseEventSource());
			}
			if(config.registerCurrentDisplayEventSource) {
				eventQueue.registerSource(al::CurrentDisplay.eventSource());
			}

			if(config.quitOnDisplayClosed) {
				eventDispatcher.setEventHandler<DisplayEvent>(ALLEGRO_EVENT_DISPLAY_CLOSE, [this](){
					setExitFlag();
				});
			}
			if(config.quitOnEscPressed) {
				eventDispatcher.onKeyDown(ALLEGRO_KEY_ESCAPE, [this](){
					setExitFlag();
				});
			}
			if(config.autoAcknowledgeResize) {
				eventDispatcher.setEventHandler<DisplayEvent>(ALLEGRO_EVENT_DISPLAY_RESIZE, [](){
					CurrentDisplay.acknowledgeResize();
				});
			}

			setFramerateLimit(config.framerateLimit);

		}

		void setFramerateLimit(FramerateLimit limit) {
			framerateLimiter.setLimit(limit);
		}

		void setExitFlag() {
			exitFlag = true;
		}

		void run(const std::function<void(void)>& loopBody) {
			while(!exitFlag) {
				framerateLimiter.wait();
				while(!eventQueue.empty()) {
					auto event = eventQueue.pop();
					eventDispatcher.dispatch(event.get());
				}
				loopBody();
				double endTickTime = GetTime();
				if(lastTimeOfTick > 0.0) {
					lastTickTime = endTickTime - lastTimeOfTick;
				}
				lastTimeOfTick = endTickTime;
				fpsCounter.acknowledgeFrame();
				tick++;
			}
			exitFlag = false;
		}

		[[nodiscard]] int64_t getTick() const {
			return tick;
		}
		[[nodiscard]] int64_t getFPS() const {
			return fpsCounter.getFPS();
		}
		[[nodiscard]] double getLastTickTime() const {
			return lastTickTime;
		}

		EventQueue eventQueue;
		EventDispatcher eventDispatcher;
	private:
		FramerateLimiter framerateLimiter;
		FPSCounter fpsCounter;
		int64_t tick = 0;
		double lastTimeOfTick = -1.0;
		double lastTickTime = 0.01;
		bool exitFlag = false;
	};
}

#endif /* INCLUDE_AXXEGRO_CORE_EVENT_EVENTLOOP */
