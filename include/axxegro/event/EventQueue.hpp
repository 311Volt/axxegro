#ifndef INCLUDE_AXXEGRO_EVENT_EVENTQUEUE
#define INCLUDE_AXXEGRO_EVENT_EVENTQUEUE

#include <allegro5/allegro.h>
#include <optional>

#include "EventSource.hpp"

namespace al {
	class EventQueue {
	public:
		EventQueue();
		~EventQueue();

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
		 * @return false on failure (queue was already emptys)
		 */
		bool drop();

		///@brief Clears the queue.
		void clear();

		///@brief Blocks execution while the queue is empty.
		ALLEGRO_EVENT wait();

		/**
		 * @brief Does the same as wait() until a timeout.
		 * @return true on success (queue non-empty)
		 * @return false on failure (timeout)
		 */
		std::optional<ALLEGRO_EVENT> waitFor(double seconds);
		
		
		ALLEGRO_EVENT_QUEUE* alPtr();
	private:
		ALLEGRO_EVENT_QUEUE* ptr;
	};
}


#endif /* INCLUDE_AXXEGRO_EVENT_EVENTQUEUE */
