#ifndef INCLUDE_TIME_TIME
#define INCLUDE_TIME_TIME

namespace al {

	///@return Time (in seconds) since the program was started.
	double GetTime();

	///@brief Blocks the current thread for `secs` seconds.
	void Sleep(double secs);
}

#endif /* INCLUDE_TIME_TIME */
