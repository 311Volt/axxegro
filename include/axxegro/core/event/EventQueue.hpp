#ifndef INCLUDE_AXXEGRO_EVENT_EVENTQUEUE
#define INCLUDE_AXXEGRO_EVENT_EVENTQUEUE

#include "EventSource.hpp"
#include "../../common.hpp"

#include <optional>


namespace al {
	AXXEGRO_DEFINE_DELETER(ALLEGRO_EVENT_QUEUE, al_destroy_event_queue);
	class EventQueue: public Resource<ALLEGRO_EVENT_QUEUE> {
	public:
		EventQueue()
		: Resource(al_create_event_queue())
		{
			if(!ptr()) {
				throw EventQueueError("Failed to create Allegro event queue");
			}
		}

		/**
		 * @brief Registers an event source for this queue.
		 * 
		 * @param src A pointer to the event source.
		 */
		void registerSource(ALLEGRO_EVENT_SOURCE* src) {
			al_register_event_source(ptr(), src);
		}
		void registerSource(const EventSource& src) {
			al_register_event_source(ptr(), src.ptr());
		}

		///@brief Returns true is the queue is empty, false otherwise.
		bool empty() {
			return al_is_event_queue_empty(ptr());
		}
		
		///@brief Pops the event at the front of the queue and returns it.
		ALLEGRO_EVENT pop() {
			if(empty()) {
				throw EventQueueError("pop() called on an empty event queue. always check the queue with empty()");
			}
			ALLEGRO_EVENT ret;
			al_get_next_event(ptr(), &ret);
			return ret;
		}

		///@brief Returns the event at the front of the queue without popping it.
		ALLEGRO_EVENT peek() {
			if(empty()) {
				throw EventQueueError("peek() called on an empty event queue. always check the queue with empty()");
			}
			ALLEGRO_EVENT ret;
			al_peek_next_event(ptr(), &ret);
			return ret;
		}

		std::optional<ALLEGRO_EVENT> tryPop() { 
			if(empty()) {
				return {};
			}
			return pop();
		}
		
		/**
		 * @brief Pops the event. 
		 * Use this instead of pop() if you want to be explicit about ignoring an event.
		 * @return true on success
		 * @return false on failure (queue was already empty)
		 */
		bool drop() {
			return al_drop_next_event(ptr());
		}

		///@brief Clears the queue.
		void clear() {
			flush();
		}

		///@brief Clears the queue.
		void flush() {
			al_flush_event_queue(ptr());
		}

		/**
		 * @brief Blocks execution while the queue is empty.
		 * When an event appears, it is popped and returned.
		 * @return The event.
		 */
		ALLEGRO_EVENT wait() {
			ALLEGRO_EVENT ret;
			al_wait_for_event(ptr(), &ret);
			return ret;
		}

		/**
		 * @brief Does the same as wait() until a timeout.
		 * @return The event (on success)
		 * @return std::nullopt (on timeout)
		 */
		std::optional<ALLEGRO_EVENT> waitFor(float seconds) {
			ALLEGRO_EVENT ev;
			if(!al_wait_for_event_timed(ptr(), &ev, seconds))
				return std::nullopt;
			return {ev};
		}
		
	private:
	};
}


#endif /* INCLUDE_AXXEGRO_EVENT_EVENTQUEUE */
