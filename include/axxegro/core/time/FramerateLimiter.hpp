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

	namespace FPSLimit {
		struct NoneT{};
		inline constexpr NoneT None{};

		struct AutoT{};
		inline constexpr AutoT Auto{};

		struct VSyncT{};
		inline constexpr VSyncT VSync{};
	}

	using FramerateLimit = std::variant<
	    FPSLimit::NoneT,
		FPSLimit::AutoT,
		Hz
	>;

	struct FramerateLimiter {

		explicit FramerateLimiter(FramerateLimit limit = FPSLimit::None)
			: timer(1_Hz)
		{
			queue.registerSource(timer.getEventSource());
			setLimit(limit);
		}

		void setLimit(FramerateLimit limit) {
			limitValue = limit;

			if(std::holds_alternative<FPSLimit::AutoT>(limit)) {
				limitValue = al::CurrentDisplay.findGoodFramerateLimit();
			}

			if(std::holds_alternative<Hz>(limitValue)) {
				timer.setFreq(std::get<Hz>(limitValue));
				timer.stop();
				timer.start();
			} else {
				timer.stop();
			}
		}

		void wait() {
			if(std::holds_alternative<FPSLimit::NoneT>(limitValue)) {
				return;
			} else if(std::holds_alternative<Hz>(limitValue)) {
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
