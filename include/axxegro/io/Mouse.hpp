#ifndef INCLUDE_AXXEGRO_IO_MOUSE
#define INCLUDE_AXXEGRO_IO_MOUSE

#include <allegro5/allegro.h>

#include "../math/math.hpp"
#include "../event/EventSource.hpp"
#include "../resources/Bitmap.hpp"

namespace al {
	namespace mouse {

		class Cursor {
			ALLEGRO_MOUSE_CURSOR* ptr;
		public:
			Cursor(const Bitmap& bmp, Point focus);
			~Cursor();

			ALLEGRO_MOUSE_CURSOR* alPtr() {return ptr;}

		#ifdef AXXEGRO_TRUSTED
			ALLEGRO_MOUSE_CURSOR* alPtr() const {return ptr;}
		#endif

		};

		using State = ALLEGRO_MOUSE_STATE;

		enum class Btn: int {
			LEFT = 1,
			RIGHT = 2,
			MIDDLE = 3
		};
		constexpr int BtnBit(Btn btn)
			{return 1<<(int(btn)-1);}

		unsigned GetNumAxes();
		unsigned GetNumButtons();

		State GetState();

		bool IsButtonDown(Btn btn);
		bool IsButtonDown(State state, Btn btn);

		bool SetPos(Point p);
		Point GetPos();
		Point GetDesktopPos();

		EventSource GetEventSource();
	}
}

#endif /* INCLUDE_AXXEGRO_IO_MOUSE */
