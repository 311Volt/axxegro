#ifndef INCLUDE_AXXEGRO_TIME_TIMER
#define INCLUDE_AXXEGRO_TIME_TIMER

#include <allegro5/allegro.h>
#include <memory>
#include "../resources/Resource.hpp"
#include "../event/EventSource.hpp"


namespace al {
	constexpr double PeriodToFreq(double period) {return 1.0 / period;}
	constexpr double FreqToPeriod(double freq) {return 1.0 / freq;}

	class TimerEventSource: public EventSource {
	public:
		TimerEventSource(ALLEGRO_TIMER* t)
			: t(t)
		{}

		virtual ALLEGRO_EVENT_SOURCE* ptr() const override
			{return al_get_timer_event_source(t);}
	private:
		ALLEGRO_TIMER* t;
	};

	class TimerDeleter {
	public:
		void operator()(ALLEGRO_TIMER* t) {al_destroy_timer(t);}
	};

	class Timer: public Resource<ALLEGRO_TIMER, TimerDeleter> {
	public:
		Timer(double period);

		static Timer Freq(double freq);
		static Timer Period(double period);

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
		std::unique_ptr<TimerEventSource> evSrc;
	};
}

#endif /* INCLUDE_AXXEGRO_TIME_TIMER */

