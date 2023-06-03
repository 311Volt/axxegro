#ifndef INCLUDE_AXXEGRO_CORE_IO_MOUSE
#define INCLUDE_AXXEGRO_CORE_IO_MOUSE

#include "../../common.hpp"
#include "../event/EventSource.hpp"
#include "../gfx/Bitmap.hpp"

namespace al {

	AXXEGRO_DEFINE_DELETER(ALLEGRO_MOUSE_CURSOR, al_destroy_mouse_cursor);


	struct MouseDriver {
		static constexpr char name[] = "Mouse driver";
		[[nodiscard]] static bool init() {return al_install_mouse();}
		[[nodiscard]] static bool isInitialized() {return al_is_mouse_installed();}
		using DependsOn = InitDependencies<CoreAllegro>;
	};

	class MouseCursor:
			RequiresInitializables<MouseDriver>,
			public Resource<ALLEGRO_MOUSE_CURSOR> {
	public:
		MouseCursor(const Bitmap& bmp, Coord<int> focus)
				: Resource(al_create_mouse_cursor(bmp.ptr(), focus.x, focus.y))
		{
			if(!ptr()) {
				throw ResourceLoadError("Cannot create mouse cursor from a %dx%d bitmap", bmp.width(), bmp.height());
			}
		}

	};

	class MouseEventSource:
			RequiresInitializables<MouseDriver>,
			public EventSource {
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
		InternalRequire<MouseDriver>();
		return al_get_mouse_num_axes();
	}

	inline unsigned GetNumButtons() {
		InternalRequire<MouseDriver>();
		return al_get_mouse_num_buttons();
	}

	inline MouseState GetMouseState() {
		InternalRequire<MouseDriver>();
		MouseState ret;
		al_get_mouse_state(&ret);
		return ret;
	}

	inline bool IsButtonDown(MouseButton btn) {
		return GetMouseState().isButtonDown(btn);
	}

	inline bool SetMousePos(Coord<int> p) {
		InternalRequire<MouseDriver>();
		return al_set_mouse_xy(al_get_current_display(), int(p.x), int(p.y));
	}
	inline Coord<int> GetMousePos() {
		MouseState st = GetMouseState();
		return {st.x, st.y};
	}
	inline Coord<int> GetMouseDesktopPos() {
		InternalRequire<MouseDriver>();
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
