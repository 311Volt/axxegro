//
// Created by volt on 2023-07-14.
//

#ifndef AXXEGRO_FRAMERATELIMITER_HPP
#define AXXEGRO_FRAMERATELIMITER_HPP

#include "FreqPeriod.hpp"
#include <variant>
#include "Time.hpp"
#include "Timer.hpp"
#include "../event/EventQueue.hpp"
#include "../display.hpp"


namespace al {

	namespace FramerateLimiterMode {
		struct None{};
		struct Auto{};
		struct VSync{};
	}

	using FramerateLimit = std::variant<
	    FramerateLimiterMode::None,
		FramerateLimiterMode::Auto,
		Freq
	>;

	struct FramerateLimiter {

		explicit FramerateLimiter(FramerateLimit limit = FramerateLimiterMode::None{})
			: timer(1_Hz)
		{
			queue.registerSource(timer.getEventSource());
			setLimit(limit);
		}

		void setLimit(FramerateLimit limit) {
			limitValue = limit;

			if(std::holds_alternative<FramerateLimiterMode::Auto>(limit)) {
				limitValue = al::CurrentDisplay.findGoodFramerateLimit();
			}

			if(std::holds_alternative<Freq>(limitValue)) {
				timer.setFreq(std::get<Freq>(limitValue));
				timer.start();
			} else {
				timer.stop();
			}
		}

		void wait() {
			if(std::holds_alternative<FramerateLimiterMode::None>(limitValue)) {
				return;
			} else if(std::holds_alternative<Freq>(limitValue)) {
				queue.wait();
				queue.flush();
			}
		}

	private:
		FramerateLimit limitValue;
		Timer timer;
		EventQueue queue;
	};

}


#endif //AXXEGRO_FRAMERATELIMITER_HPP
