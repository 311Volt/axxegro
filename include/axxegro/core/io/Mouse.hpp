#ifndef INCLUDE_AXXEGRO_CORE_IO_MOUSE
#define INCLUDE_AXXEGRO_CORE_IO_MOUSE

#include "../../common.hpp"
#include "../event/EventSource.hpp"
#include "../gfx/Bitmap.hpp"

namespace al {

	AXXEGRO_DEFINE_DELETER(ALLEGRO_MOUSE_CURSOR, al_destroy_mouse_cursor);

	class MouseCursor: public Resource<ALLEGRO_MOUSE_CURSOR> {
	public:
		MouseCursor(const Bitmap& bmp, Coord<int> focus)
				: Resource(al_create_mouse_cursor(bmp.ptr(), focus.x, focus.y))
		{

		}

	};

	class MouseEventSource: public EventSource {
	public:
		[[nodiscard]] ALLEGRO_EVENT_SOURCE* ptr() const override {
			return al_get_mouse_event_source();
		}
	};

	/// @brief Mouse button numbers used by Allegro
	using MouseButton = int;
	enum {
		LMB = 1,
		RMB = 2,
		MMB = 3
	};

	struct MouseState: public ALLEGRO_MOUSE_STATE {
		bool isButtonDown(MouseButton btn) {
			return al_mouse_button_down(this, btn);
		}
	};


	/// @returns The bit corresponding to a button in the MouseState::buttons bitfield
	constexpr int MouseBtnBit(int btn) {
		return 1 << (int(btn)-1);
	}
	
	inline unsigned GetNumAxes() {
		return al_get_mouse_num_axes();
	}

	inline unsigned GetNumButtons() {
		return al_get_mouse_num_buttons();
	}

	inline MouseState GetMouseState() {
		MouseState ret;
		al_get_mouse_state(&ret);
		return ret;
	}

	inline bool IsButtonDown(MouseButton btn) {
		return GetMouseState().isButtonDown(btn);
	}

	inline bool SetMousePos(Coord<int> p) {
		return al_set_mouse_xy(al_get_current_display(), int(p.x), int(p.y));
	}
	inline Coord<int> GetMousePos() {
		MouseState st = GetMouseState();
		return {st.x, st.y};
	}
	inline Coord<int> GetMouseDesktopPos() {
		int x,y;
		al_get_mouse_cursor_position(&x, &y);
		return {x, y};
	}

	inline const EventSource& GetMouseEventSource() {
		static MouseEventSource mouseEventSource;
		return mouseEventSource;
	}
}

#endif /* INCLUDE_AXXEGRO_CORE_IO_MOUSE */
