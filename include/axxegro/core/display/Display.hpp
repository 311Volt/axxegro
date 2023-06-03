#ifndef INCLUDE_AXXEGRO_CORE_DISPLAY_DISPLAY
#define INCLUDE_AXXEGRO_CORE_DISPLAY_DISPLAY

#include "../../common.hpp"
#include "../gfx/Bitmap.hpp"
#include "../io/Mouse.hpp"

#include "DisplayModes.hpp"

#include <string>
#include <vector>
#include <optional>

#include <allegro5/allegro.h>

#define AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC [[maybe_unused]] auto* unused_____ = ptr();

namespace al {

	AXXEGRO_DEFINE_DELETER(ALLEGRO_DISPLAY, al_destroy_display);

	class Display;

	class DisplayBackbuffer: public Bitmap {
	public:
		explicit DisplayBackbuffer(Display& disp)
			: Bitmap(nullptr), disp(disp)
		{}
		virtual ~DisplayBackbuffer() = default;
	private:
		[[nodiscard]] ALLEGRO_BITMAP* getPointer() const override;

		Display& disp;
	};

	class DisplayEventSource: public EventSource {
	public:
		explicit DisplayEventSource(Display& disp)
			: disp(disp)
		{}
		virtual ~DisplayEventSource() = default;

		[[nodiscard]] ALLEGRO_EVENT_SOURCE* ptr() const override;
	private:
		Display& disp;
	};

	class Display:
			RequiresInitializables<CoreAllegro>,
			public Resource<ALLEGRO_DISPLAY> {
	public:
		using Resource::Resource;
		struct Option {
			int option, value;
		};

		/**
		 * @brief Construct a new Display object with custom flags and options.
		 * For possible options, see: https://liballeg.org/a5docs/trunk/display.html#al_set_new_display_option
		 * 
		 * @param w Width in pixels.
		 * @param h Height in pixels.
		 * @param flags See: https://liballeg.org/a5docs/trunk/display.html#al_set_new_display_flags
		 * @param requiredOptions Will be passed with ALLEGRO_REQUIRED and throw if one of the settings can't be met.
		 * @param suggestedOptions ALLEGRO_SUGGESTED (falls back to closest possible settings)
		 * @param dontCareOptions ALLEGRO_DONTCARE 
		 */
		Display(
			int w, 
			int h, 
			int flags = 0, 
			const std::vector<Option>& requiredOptions = {},
			const std::vector<Option>& suggestedOptions = {},
			const std::vector<Option>& dontCareOptions = {}
		)
				: Resource(nullptr)
		{
			if(!al_is_system_installed()) {
				throw DisplayCreationError("cannot create display (did you forget to initialize allegro?)");
			}
			al_reset_new_display_options();
			for(const auto& [opt, val]: requiredOptions) {
				al_set_new_display_option(opt, val, ALLEGRO_REQUIRE);
			}
			for(const auto& [opt, val]: suggestedOptions) {
				al_set_new_display_option(opt, val, ALLEGRO_SUGGEST);
			}
			for(const auto& [opt, val]: dontCareOptions) {
				al_set_new_display_option(opt, val, ALLEGRO_DONTCARE);
			}
			al_set_new_display_flags(flags);

			setPtr(al_create_display(w, h));
			if(!ptr()) {
				throw DisplayCreationError("Could not create a %dx%d Allegro display.", w, h);
			}
			al_reset_new_display_options();
			initPointers();
		}



		///@return Width of the display in pixels.
		[[nodiscard]] int width() const {
			return al_get_display_width(ptr());
		}

		///@return Height of the display in pixels;
		[[nodiscard]] int height() const {
			return al_get_display_height(ptr());
		}

		///@return Refresh rate of the display in Hz. Keep in mind that THIS RETURNS 0 FOR WINDOWED DISPLAYS
		[[nodiscard]] int getRefreshRate() const {
			return al_get_display_refresh_rate(ptr());
		}

		///@return Dimensions of the display in pixels.
		[[nodiscard]] Vec2<int> size() const {
			return {width(), height()};
		}

		///@return Aspect ratio of the display (e.g. ~1.33 for 640x480)
		[[nodiscard]] double aspectRatio() const {
			return double(width()) / double(height());
		}

		///@return A rectangle starting at (0,0) and ending at (width,height).
		[[nodiscard]] Rect<int> rect() const {
			return {{0,0}, size()};
		}

		///@brief See: https://liballeg.org/a5docs/trunk/display.html#al_get_display_option
		[[nodiscard]] int getOptionValue(int option) const {
			return al_get_display_option(ptr(), option);
		}

		///@brief See: https://liballeg.org/a5docs/trunk/display.html#al_set_display_flag
		bool setFlag(int flag, bool onoff) {
			return al_set_display_flag(ptr(), flag, onoff);
		}

		/**
		 * @brief Resizes the display.
		 * 
		 * @param w New width in pixels.
		 * @param h New height in pixels.
		 * @return true on success
		 * @return false on failure
		 */
		bool resize(int w, int h) {
			return al_resize_display(ptr(), w, h);
		}

		/**
		 * @brief Should be called in response to ALLEGRO_EVENT_DISPLAY_RESIZE.
		 * See: https://liballeg.org/a5docs/trunk/display.html#al_acknowledge_resize
		 * 
		 * @return true on success
		 * @return false on failure
		 */
		bool acknowledgeResize() {
			return al_acknowledge_resize(ptr());
		}

		/**
		 * @return The position of the display relative to the desktop. 
		 */
		[[nodiscard]] Coord<int> getPosition() const {
			int x, y;
			al_get_window_position(ptr(), &x, &y);
			return {x, y};
		}

		/**
		 * @brief Changes the position of the display (window)
		 * 
		 * @param pos Position in pixels, relative to the desktop
		 */
		void setPosition(Coord<int> pos) {
			al_set_window_position(ptr(), pos.x, pos.y);
		}

		///@brief See: https://liballeg.org/a5docs/trunk/display.html#al_get_display_flags
		[[nodiscard]] int getFlags() const {
			return al_get_display_flags(ptr());
		}

		/**
		 * @brief Gets the text in the system clipboard, converted to an STL string.
		 * If the clipboard is empty, this returns an empty string.
		 * 
		 * @return Clipboard text, in the context of this display.
		 */
		[[nodiscard]] std::optional<std::string> getClipboardText() const {
			std::string ret;
			char* buf = al_get_clipboard_text(ptr());
			if(buf) {
				ret = buf;
				al_free(buf);
			} else {
				return std::nullopt;
			}
			return ret;
		}

		/**
		 * @brief Sets the system clipboard context.
		 * 
		 * @param text Text to be copied into the clipboard
		 * @return true on success
		 * @return false on failure
		 */
		bool setClipboardText(const std::string& text) {
			return al_set_clipboard_text(ptr(), text.c_str());
		}

		/**
		 * @brief Sets the display's window's title.
		 * 
		 * @param title New title.
		 */
		void setTitle(const std::string& title) {
			al_set_window_title(ptr(), title.c_str());
		}

		/**
		 * @brief Sets the mouse cursor to be shown on the display.
		 * 
		 * @param cur Cursor.
		 * @return true on success 
		 * @return false on failure
		 */
		bool setCursor(MouseCursor& cur) {
			return al_set_mouse_cursor(ptr(), cur.ptr());
		}

		/**
		 * @brief Sets the mouse cursor to one of the OS's predefined cursors.
		 * For details, see https://liballeg.org/a5docs/trunk/mouse.html#al_set_system_mouse_cursor
		 * 
		 * @param id 
		 * @return true 
		 * @return false 
		 */
		bool setSystemCursor(ALLEGRO_SYSTEM_MOUSE_CURSOR id) {
			return al_set_system_mouse_cursor(ptr(), id);
		}

		///@brief Hides the mouse cursor.
		void hideCursor() {
			al_hide_mouse_cursor(ptr());
		}

		///@brief Shows the mouse cursor.
		void showCursor() {
			al_show_mouse_cursor(ptr());
		}

		///@brief Constrains the mouse to the display.
		void grabMouse() {
			al_grab_mouse(ptr());
		}

		///@brief Reverses the effect of a grabMouse() call.
		void ungrabMouse() { AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC
			al_ungrab_mouse();
		}

		///@returns A reference to the display's backbuffer bitmap.
		[[nodiscard]] const Bitmap& backbuffer() const {
			return *ptrBackbuffer;
		}

		///@brief Returns the display's event source.
		const EventSource& eventSource() {
			return *ptrEventSource;
		}

		/**
		 * @brief Tries to find a reasonable default for a framerate limiter.
		 * @returns A number between 60 and 300. If the display is fullscreen,
		 * the display's refresh rate is returned. Otherwise, it returns
		 * the highest refresh rate of any video mode returned by GetDisplayModes().
		 * If that fails, it returns 300.
		 * The return value is clamped between 60 and 300.
		 */
		[[nodiscard]] int findFramerateCap() const {
			int ret = getRefreshRate();
			if(ret == 0) {
				auto modes = GetDisplayModes();
				ret = 0;
				for(auto& mode: modes) {
					ret = std::max(ret, mode.refresh_rate);
				}
			}
			if(ret == 0) {
				return 300;
			}
			return std::clamp(ret, 60, 300);
		}

	protected:
		void initPointers() {
			ptrBackbuffer = std::make_unique<DisplayBackbuffer>(*this);
			ptrEventSource = std::make_unique<DisplayEventSource>(*this);
		}
	private:
		std::unique_ptr<DisplayEventSource> ptrEventSource;
		std::unique_ptr<DisplayBackbuffer> ptrBackbuffer;
	};


	inline ALLEGRO_BITMAP *al::DisplayBackbuffer::getPointer() const
	{
		return al_get_backbuffer(disp.ptr());
	}

	inline ALLEGRO_EVENT_SOURCE *al::DisplayEventSource::ptr() const
	{
		return al_get_display_event_source(disp.ptr());
	}


	class TCurrentDisplay: public Display {
	public:
		TCurrentDisplay() : Display(nullptr) 
		{
			initPointers();
		}
		
		void flip() { AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC
			al_flip_display();
		}
		void flip(Rect<int> rect) { AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC
			al_update_display_region(rect.a.x, rect.a.y, rect.width(), rect.height());
		}
		bool waitForVsync() { AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC
			return al_wait_for_vsync();
		}
		void convertMemoryBitmaps() { AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC
			al_convert_memory_bitmaps();
		}

		void setTargetBitmap(Bitmap& bmp) { AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC
			al_set_target_bitmap(bmp.ptr());
		}
	private:
		[[nodiscard]] ALLEGRO_DISPLAY* getPointer() const override {
			return al_get_current_display();
		}
	};

#undef AXXEGRO_SUPPRESS_CAN_BE_MADE_STATIC

	inline TCurrentDisplay CurrentDisplay;
}

#endif /* INCLUDE_AXXEGRO_CORE_DISPLAY_DISPLAY */
