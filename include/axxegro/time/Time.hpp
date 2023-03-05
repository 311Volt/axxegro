#ifndef INCLUDE_TIME_TIME
#define INCLUDE_TIME_TIME

#include <allegro5/allegro.h>
#include <memory>

namespace al {

#if ALLEGRO_VERSION_INT < AL_ID(5,2,8,1) && _POSIX_C_SOURCE >= 199309L
#define AXX_OVERRIDE_GET_TIME

	#include <time.h>
	struct POSIXTimer {
		struct timespec init;
		double t0, offset;

		POSIXTimer()
		{
			clock_gettime(CLOCK_MONOTONIC, &init);
			t0 = init.tv_sec + 0.000000001 * init.tv_nsec;
			offset = al_get_time();
		}

		double getTime()
		{
			struct timespec time;
			clock_gettime(CLOCK_MONOTONIC, &time);
			double t1 = time.tv_sec + 0.000000001 * time.tv_nsec;
			return (offset-t0) + t1;
		}
	};

#endif


	///@return Time (in seconds) since the program was started.
	inline double GetTime() {
#ifdef AXX_OVERRIDE_GET_TIME
		static POSIXTimer timer;
		return timer.getTime();
#else
		return al_get_time();
#endif
	}

	///@brief Blocks the current thread for `secs` seconds.
	inline void Sleep(double secs) {
		al_rest(secs);
	}
}

#endif /* INCLUDE_TIME_TIME */
