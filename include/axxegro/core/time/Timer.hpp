#ifndef INCLUDE_AXXEGRO_TIME_TIMER
#define INCLUDE_AXXEGRO_TIME_TIMER

#include "../../common.hpp"

namespace al {
	constexpr double PeriodToFreq(double period) {return 1.0 / period;}
	constexpr double FreqToPeriod(double freq) {return 1.0 / freq;}

	class TimerEventSource: public EventSource {
	public:
		explicit TimerEventSource(ALLEGRO_TIMER* t)
			: t(t)
		{}

		[[nodiscard]] ALLEGRO_EVENT_SOURCE* ptr() const override {
			return al_get_timer_event_source(t);
		}
	private:
		ALLEGRO_TIMER* t;
	};

	AXXEGRO_DEFINE_DELETER(ALLEGRO_TIMER, al_destroy_timer);

	class Timer: public Resource<ALLEGRO_TIMER> {
	public:
		explicit Timer(double period)
				: Resource(al_create_timer(period))
		{
			evSrc = std::make_unique<TimerEventSource>(ptr());
		}

		static Timer Freq(double freq) {
			return Timer(FreqToPeriod(freq));
		}

		static Timer Period(double period) {
			return Timer(period);
		};

		void start() {
			al_start_timer(ptr());
		}
		void resume() {
			al_resume_timer(ptr());
		}
		void stop() {
			al_stop_timer(ptr());
		}

		[[nodiscard]] bool isStarted() const {
			return al_get_timer_started(ptr());
		}

		[[nodiscard]] const EventSource& getEventSource() const {
			return *evSrc;
		}

		[[nodiscard]] int64_t getCount() const {
			return al_get_timer_count(ptr());
		}
		void setCount(int64_t value) {
			al_set_timer_count(ptr(), value);
		}

		[[nodiscard]] double getPeriod() const {
			return al_get_timer_speed(ptr());
		}
		void setPeriod(double value) {
			al_set_timer_speed(ptr(), value);
		}

		[[nodiscard]] double getFreq() const {
			return PeriodToFreq(getPeriod());
		}
		void setFreq(double value) {
			setPeriod(FreqToPeriod(value));
		}
	private:
		std::unique_ptr<TimerEventSource> evSrc;
	};
}

#endif /* INCLUDE_AXXEGRO_TIME_TIMER */

