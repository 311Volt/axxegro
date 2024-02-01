#ifndef INCLUDE_AXXEGRO_CORE_EVENT_EVENTLOOP
#define INCLUDE_AXXEGRO_CORE_EVENT_EVENTLOOP

#include "EventQueue.hpp"
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

	enum BuiltinEventSourceMask {
		KeyboardEventSourceBit = 1,
		MouseEventSourceBit = 2,
		CurrentDisplayEventSourceBit = 4
	};

	enum BuiltinQuitTriggersMask {
		QuitOnDisplayClosedBit = 1,
		QuitOnEscPressedBit = 2,
	};

	struct EventLoopConfig {
		uint32_t autoRegisterEvents = KeyboardEventSourceBit | MouseEventSourceBit | CurrentDisplayEventSourceBit;
		uint32_t enableQuitTriggers = QuitOnDisplayClosedBit | QuitOnEscPressedBit;
		bool autoAcknowledgeResize = true;
		FramerateLimit framerateLimit = FPSLimit::None;
	};

	inline constexpr EventLoopConfig EmptyEventLoopConfig = {
		.autoRegisterEvents = 0,
		.enableQuitTriggers = 0,
		.autoAcknowledgeResize = false
	};

	inline constexpr EventLoopConfig BasicEventLoopConfig = {
		.autoRegisterEvents = KeyboardEventSourceBit | MouseEventSourceBit | CurrentDisplayEventSourceBit,
		.enableQuitTriggers = QuitOnDisplayClosedBit,
		.autoAcknowledgeResize = true
	};

	inline constexpr EventLoopConfig DemoEventLoopConfig = {
		.autoRegisterEvents = KeyboardEventSourceBit | MouseEventSourceBit | CurrentDisplayEventSourceBit,
		.enableQuitTriggers = QuitOnDisplayClosedBit | QuitOnEscPressedBit,
		.autoAcknowledgeResize = true,
		.framerateLimit = FPSLimit::Auto
	};



	class EventLoop: RequiresInitializables<CoreAllegro> {

	public:
		~EventLoop() = default;

		explicit EventLoop(EventLoopConfig config = BasicEventLoopConfig) {

			if(config.autoRegisterEvents & KeyboardEventSourceBit) {
				eventQueue.registerSource(GetKeyboardEventSource());
			}
			if(config.autoRegisterEvents & MouseEventSourceBit) {
				eventQueue.registerSource(GetMouseEventSource());
			}
			if(config.autoRegisterEvents & CurrentDisplayEventSourceBit) {
				eventQueue.registerSource(al::CurrentDisplay.eventSource());
			}

			if(config.enableQuitTriggers & QuitOnDisplayClosedBit) {
				eventDispatcher.setEventHandler<DisplayEvent>(ALLEGRO_EVENT_DISPLAY_CLOSE, [this](){
					setExitFlag();
				});
			}
			if(config.enableQuitTriggers & QuitOnEscPressedBit) {
				eventDispatcher.onKeyDown(ALLEGRO_KEY_ESCAPE, [this](){
					setExitFlag();
				});
			}
			if(config.autoAcknowledgeResize) {
				eventDispatcher.setEventHandler<DisplayEvent>(ALLEGRO_EVENT_DISPLAY_RESIZE, [](){
					CurrentDisplay.acknowledgeResize();
				});
			}

			framerateLimiter.setLimit(config.framerateLimit);

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
		[[nodiscard]] double getFPS() const {
			return fpsCounter.getFPS();
		}
		[[nodiscard]] double getLastTickTime() const {
			return lastTickTime;
		}

		EventQueue eventQueue;
		EventDispatcher eventDispatcher;
		FramerateLimiter framerateLimiter;
		FPSCounter fpsCounter;
	private:
		int64_t tick = 0;
		double lastTimeOfTick = -1.0;
		double lastTickTime = 0.01;
		bool exitFlag = false;
	};
}

#endif /* INCLUDE_AXXEGRO_CORE_EVENT_EVENTLOOP */
