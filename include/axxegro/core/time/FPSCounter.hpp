//
// Created by volt on 2023-07-14.
//

#ifndef AXXEGRO_FPSCOUNTER_HPP
#define AXXEGRO_FPSCOUNTER_HPP

#include "Time.hpp"

namespace al {

	struct FPSCounter {

		void acknowledgeFrame() {
			auto curTime = GetTime();
			if(curTime - timeOfLastFpsUpdate > interval) {
				fps = static_cast<double>(fpsCounter) / interval;
				fpsCounter = 0;
				timeOfLastFpsUpdate = curTime;
			}
			fpsCounter++;
		}

		[[nodiscard]] double getFPS() const {
			return fps;
		}

		[[nodiscard]] double getInterval() const {
			return interval;
		}

		void setInterval(double interval) {
			if(interval < 0.1) {
				throw Exception("FPS counter interval must be positive and not less than 0.1");
			}
			this->interval = interval;
		}

	private:
		double interval = 1.0;
		int64_t fpsCounter = 0;
		double fps = 0;
		double timeOfLastFpsUpdate = -1.0;
	};

}

#endif //AXXEGRO_FPSCOUNTER_HPP
