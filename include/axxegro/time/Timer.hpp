#ifndef INCLUDE_TIME_TIMER
#define INCLUDE_TIME_TIMER

#include <allegro5/allegro.h>
#include <memory>
#include "../event/EventSource.hpp"


namespace al {
	constexpr double PeriodToFreq(double period) {return 1.0 / period;}
	constexpr double FreqToPeriod(double freq) {return 1.0 / freq;}

	class Timer {
	public:
		Timer(double period);
		~Timer();

		static Timer Freq(double freq);
		static Timer Period(double period);

		ALLEGRO_TIMER* alPtr();

		void start();
		void resume();
		void stop();

		bool isStarted();

		const EventSource& getEventSource() const;

		int64_t getCount() const;
		void setCount(int64_t value);

		double getPeriod() const;
		void setPeriod(double value);

		double getFreq() const;
		void setFreq(double value);
	private:
		EventSource evSrc;
		ALLEGRO_TIMER* ptr;
	};
}

#endif /* INCLUDE_TIME_TIMER */

