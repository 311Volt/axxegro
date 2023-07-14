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
			if(curTime - timeOfLastFpsUpdate > 1.0) {
				fps = fpsCounter;
				fpsCounter = 0;
				timeOfLastFpsUpdate = curTime;
			}
			fpsCounter++;
		}

		[[nodiscard]] int64_t getFPS() const {
			return fps;
		}

	private:
		int64_t fpsCounter = 0;
		int64_t fps = 0;
		double timeOfLastFpsUpdate = -1.0;
	};

}

#endif //AXXEGRO_FPSCOUNTER_HPP
