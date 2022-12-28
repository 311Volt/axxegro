#ifndef INCLUDE_AXXEGRO_EVENT_EVENTQUEUE
#define INCLUDE_AXXEGRO_EVENT_EVENTQUEUE

#include <allegro5/allegro.h>
#include <allegro5/events.h>
#include <optional>

#include "EventSource.hpp"
#include "../resources/Resource.hpp"


namespace al {
	AXXEGRO_DEFINE_DELETER(ALLEGRO_EVENT_QUEUE, al_destroy_event_queue);
	class EventQueue: public Resource<ALLEGRO_EVENT_QUEUE> {
	public:
		EventQueue();

		/**
		 * @brief Registers an event source for this queue.
		 * 
		 * @param src A pointer to the event source.
		 */
		void registerSource(ALLEGRO_EVENT_SOURCE* src);
		void registerSource(const EventSource& src);

		///@brief Returns true is the queue is empty, false otherwise.
		bool empty();
		
		///@brief Pops the event at the front of the queue and returns it.
		ALLEGRO_EVENT pop();

		///@brief Returns the event at the front of the queue without popping it.
		ALLEGRO_EVENT peek();
		
		/**
		 * @brief Pops the event. 
		 * Use this instead of pop() if you want to be explicit about ignoring an event.
		 * @return true on success
		 * @return false on failure (queue was already empty)
		 */
		bool drop();

		///@brief Clears the queue.
		void clear();

		///@brief Clears the queue.
		void flush();

		/**
		 * @brief Blocks execution while the queue is empty.
		 * When an event appears, it is popped and returned.
		 * @return The event.
		 */
		ALLEGRO_EVENT wait();

		/**
		 * @brief Does the same as wait() until a timeout.
		 * @return The event (on success)
		 * @return std::nullopt (on timeout)
		 */
		std::optional<ALLEGRO_EVENT> waitFor(double seconds);
		
	private:
	};
}


#endif /* INCLUDE_AXXEGRO_EVENT_EVENTQUEUE */
