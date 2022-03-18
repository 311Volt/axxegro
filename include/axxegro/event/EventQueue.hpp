#ifndef INCLUDE_AXXEGRO_EVENT_EVENTQUEUE
#define INCLUDE_AXXEGRO_EVENT_EVENTQUEUE

#include <allegro5/allegro.h>

namespace al {
	class EventQueue {
	public:
		EventQueue();
		~EventQueue();
		
		void registerSource(ALLEGRO_EVENT_SOURCE* src);
		bool empty();
		
		ALLEGRO_EVENT pop();
		ALLEGRO_EVENT peek();
		
		bool drop();
		void clear();
		
		ALLEGRO_EVENT_QUEUE* get();
	private:
		ALLEGRO_EVENT_QUEUE* alPtr;
	};
}


#endif /* INCLUDE_AXXEGRO_EVENT_EVENTQUEUE */
