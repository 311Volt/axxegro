#include <axxegro/time/Time.hpp>

#include <allegro5/allegro.h>

#include <memory>

#if ALLEGRO_VERSION_INT < AL_ID(5,2,8,1) && _POSIX_C_SOURCE >= 199309L
#define AXX_OVERRIDE_GET_TIME

#include <time.h>
struct AXXPOSIXTimer {
	struct timespec init;
	double t0, offset;

	AXXPOSIXTimer()
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

//#undef AXX_OVERRIDE_GET_TIME

double al::GetTime()
{
#ifdef AXX_OVERRIDE_GET_TIME
	static AXXPOSIXTimer timer;
	return timer.getTime();
#else
	return al_get_time();
#endif
}
void al::Sleep(double secs)
{
	al_rest(secs);
}