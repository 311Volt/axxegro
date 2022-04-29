#ifndef INCLUDE_AXXEGRO_IO_MOUSE
#define INCLUDE_AXXEGRO_IO_MOUSE

#include <allegro5/allegro.h>

#include "../math/math.hpp"
#include "../event/EventSource.hpp"
#include "../resources/Bitmap.hpp"

namespace al {
	namespace mouse {
		
		class CursorDeleter {
		public:
			void operator()(ALLEGRO_MOUSE_CURSOR* p){al_destroy_mouse_cursor(p);}
		};

		class Cursor: public Resource<ALLEGRO_MOUSE_CURSOR, CursorDeleter> {
		public:
			Cursor(const Bitmap& bmp, Coord<int> focus);
		};

		class MouseEventSource: public EventSource {
		public:
			ALLEGRO_EVENT_SOURCE* ptr() const override
				{return al_get_mouse_event_source();}
		};

		using State = ALLEGRO_MOUSE_STATE;

		/// @brief Mouse button numbers used by Allegro
		enum class Btn: int {
			LEFT = 1,
			RIGHT = 2,
			MIDDLE = 3
		};

		/// @returns 
		constexpr int BtnBit(Btn btn)
			{return 1<<(int(btn)-1);}
		
		unsigned GetNumAxes();
		unsigned GetNumButtons();

		State GetState();

		bool IsButtonDown(Btn btn);
		bool IsButtonDown(State state, Btn btn);

		bool SetPos(Coord<int> p);
		Coord<int> GetPos();
		Coord<int> GetDesktopPos();

		const EventSource& GetEventSource();
	}
}

#endif /* INCLUDE_AXXEGRO_IO_MOUSE */
