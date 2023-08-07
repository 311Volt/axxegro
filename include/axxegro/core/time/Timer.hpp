#ifndef INCLUDE_AXXEGRO_TIME_TIMER
#define INCLUDE_AXXEGRO_TIME_TIMER

#include "../../common.hpp"
#include "../event/EventSource.hpp"
#include "FreqPeriod.hpp"

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

	using TimerPeriod = std::chrono::duration<double, std::ratio<1, 1>>;

	class Timer: public Resource<ALLEGRO_TIMER> {
	public:

		template<FreqOrPeriod SpeedT>
		explicit Timer(SpeedT speed)
				: Resource(al_create_timer(speed.getPeriodSecs()))
		{
			evSrc = std::make_unique<TimerEventSource>(ptr());
		}

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


		[[nodiscard]] Period getPeriod() const {
			return {std::chrono::duration<double>(getPeriodSecs())};
		}
		void setPeriod(const Period& period) {
			setPeriodSecs(period.getPeriodSecs());
		}

		[[nodiscard]] Freq getFreq() const {
			return Freq::Hz(1.0 / getPeriodSecs());
		}
		void setFreq(Freq freq) {
			setPeriodSecs(freq.getPeriodSecs());
		}

		[[nodiscard]] double getPeriodSecs() const {
			return al_get_timer_speed(ptr());
		}
		void setPeriodSecs(double value) {
			al_set_timer_speed(ptr(), value);
		}

		[[nodiscard]] double getFreqHz() const {
			return getPeriod().getFreqHz();
		}
		void setFreqHz(double value) {
			setPeriodSecs(FreqToPeriod(value));
		}
	private:
		std::unique_ptr<TimerEventSource> evSrc;
	};
}

#endif /* INCLUDE_AXXEGRO_TIME_TIMER */

