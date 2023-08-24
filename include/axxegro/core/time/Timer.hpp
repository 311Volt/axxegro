#ifndef INCLUDE_AXXEGRO_TIME_TIMER
#define INCLUDE_AXXEGRO_TIME_TIMER

#include "../../common.hpp"
#include "../event/EventSource.hpp"
#include "FreqPeriod.hpp"

namespace al {

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

		template<FreqOrPeriod SpeedT>
		explicit Timer(SpeedT speed)
				: Resource(al_create_timer(Hz(speed).getHz()))
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


		[[nodiscard]] Seconds getPeriod() const {
			return Seconds(getPeriodSecs());
		}

		void setPeriod(const Seconds& period) {
			setPeriodSecs(period.getSeconds());
		}

		[[nodiscard]] Hz getFreq() const {
			return Hz(Seconds(getPeriodSecs()));
		}
		void setFreq(const Hz& freq) {
			setPeriod(Seconds(freq));
		}

		[[nodiscard]] double getPeriodSecs() const {
			return al_get_timer_speed(ptr());
		}
		void setPeriodSecs(double value) {
			al_set_timer_speed(ptr(), value);
		}

		[[nodiscard]] double getFreqHz() const {
			return 1.0 / getPeriodSecs();
		}
		void setFreqHz(double value) {
			setPeriodSecs(1.0 / value);
		}
	private:
		std::unique_ptr<TimerEventSource> evSrc;
	};
}

#endif /* INCLUDE_AXXEGRO_TIME_TIMER */

