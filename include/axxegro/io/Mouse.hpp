#ifndef INCLUDE_AXXEGRO_IO_MOUSE
#define INCLUDE_AXXEGRO_IO_MOUSE

#include <allegro5/allegro.h>

#include <axxegro/resources/Resource.hpp>

#include "../math/math.hpp"
#include "../event/EventSource.hpp"
#include "../resources/Bitmap.hpp"

namespace al {

	AXXEGRO_DEFINE_DELETER(ALLEGRO_MOUSE_CURSOR, al_destroy_mouse_cursor);

	class MouseCursor: public Resource<ALLEGRO_MOUSE_CURSOR> {
	public:
		MouseCursor(const Bitmap& bmp, Coord<int> focus);
	};

	class MouseEventSource: public EventSource {
	public:
		ALLEGRO_EVENT_SOURCE* ptr() const override
			{return al_get_mouse_event_source();}
	};

	/// @brief Mouse button numbers used by Allegro
	using MouseButton = int;
	enum {
		LMB = 1,
		RMB = 2,
		MMB = 3
	};

	struct MouseState: public ALLEGRO_MOUSE_STATE {
		inline bool isButtonDown(MouseButton btn)
		{
			return al_mouse_button_down(this, btn);
		}
	};


	/// @returns The bit corresponding to a button in the MouseState::buttons bitfield
	constexpr int MouseBtnBit(int btn)
	{
		return 1 << (int(btn)-1);
	}
	
	unsigned GetNumAxes();
	unsigned GetNumButtons();

	MouseState GetMouseState();
	bool IsButtonDown(MouseButton btn);

	bool SetMousePos(Coord<int> p);
	Coord<int> GetMousePos();
	Coord<int> GetMouseDesktopPos();

	const EventSource& GetMouseEventSource();
}

#endif /* INCLUDE_AXXEGRO_IO_MOUSE */
