#ifndef INCLUDE_AXXEGRO_DISPLAY_DISPLAY
#define INCLUDE_AXXEGRO_DISPLAY_DISPLAY

#include "../math/math.hpp"
#include "../io/Mouse.hpp"
#include "../event/EventSource.hpp"

#include "../resources/Resource.hpp"
#include "../resources/Bitmap.hpp"

#include <allegro5/display.h>
#include <string>
#include <vector>
#include <optional>

#include <allegro5/allegro.h>

namespace al {
	class DisplayCreationError: public std::runtime_error {using std::runtime_error::runtime_error;};

	AXXEGRO_DEFINE_DELETER(ALLEGRO_DISPLAY, al_destroy_display);

	class Display;

	class DisplayBackbuffer: public Bitmap {
	public:
		DisplayBackbuffer(Display& disp)
			: Bitmap(nullptr), disp(disp)
		{}
	private:
		virtual ALLEGRO_BITMAP* getPointer() const override;

		Display& disp;
	};

	class DisplayEventSource: public EventSource {
	public:
		DisplayEventSource(Display& disp)
			: disp(disp)
		{}

		virtual ALLEGRO_EVENT_SOURCE* ptr() const override;
	private:
		Display& disp;
	};

	class Display: public Resource<ALLEGRO_DISPLAY> {
	public:
		using Resource::Resource;
		using Option = struct{int option, value;};

		/**
		 * @brief Construct a new Display object with default flags and options.
		 * 
		 * @param w Width in pixels.
		 * @param h Height in pixels.
		 */
		Display(int w, int h);

		/**
		 * @brief Construct a new Display object with custom flags and default options
		 * 
		 * @param w Width in pixels.
		 * @param h Height in pixels.
		 * @param flags See: https://liballeg.org/a5docs/trunk/display.html#al_set_new_display_flags
		 */
		Display(int w, int h, int flags);

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
			int flags, 
			std::vector<Option> requiredOptions, 
			std::vector<Option> suggestedOptions, 
			std::vector<Option> dontCareOptions
		);
		

		///@return Width of the display in pixels.
		int width() const;

		///@return Height of the display in pixels;
		int height() const;

		///@return Refresh rate of the display in Hz. Keep in mind that THIS RETURNS 0 FOR WINDOWED DISPLAYS
		int getRefreshRate() const;

		///@return Dimensions of the display in pixels.
		Vec2<int> size() const;

		///@return Aspect ratio of the display (e.g. ~1.33 for 640x480)
		double aspectRatio() const;

		///@return A rectangle starting at (0,0) and ending at (width,height).
		Rect<int> rect() const;

		///@brief See: https://liballeg.org/a5docs/trunk/display.html#al_get_display_option
		int getOptionValue(int option) const;

		///@brief See: https://liballeg.org/a5docs/trunk/display.html#al_set_display_flag
		bool setFlag(int flag, bool onoff);

		/**
		 * @brief Resizes the display.
		 * 
		 * @param w New width in pixels.
		 * @param h New height in pixels.
		 * @return true on success
		 * @return false on failure
		 */
		bool resize(int w, int h);

		/**
		 * @brief Should be called in response to ALLEGRO_EVENT_DISPLAY_RESIZE.
		 * See: https://liballeg.org/a5docs/trunk/display.html#al_acknowledge_resize
		 * 
		 * @return true on success
		 * @return false on failure
		 */
		bool acknowledgeResize();

		/**
		 * @return The position of the display relative to the desktop. 
		 */
		Coord<int> getPosition() const;

		/**
		 * @brief Changes the position of the display (window)
		 * 
		 * @param pos Position in pixels, relative to the desktop
		 */
		void setPosition(Coord<int> pos);

		///@brief See: https://liballeg.org/a5docs/trunk/display.html#al_get_display_flags
		int getFlags() const;

		/**
		 * @brief Gets the text in the system clipboard, converted to an STL string.
		 * If the clipboard is empty, this returns an empty string.
		 * 
		 * @return Clipboard text, in the context of this display.
		 */
		std::optional<std::string> getClipboardText() const;

		/**
		 * @brief Sets the system clipboard context.
		 * 
		 * @param text Text to be copied into the clipboard
		 * @return true on success
		 * @return false on failure
		 */
		bool setClipboardText(const std::string& text);

		/**
		 * @brief Sets the display's window's title.
		 * 
		 * @param title New title.
		 */
		void setTitle(const std::string& title);

		/**
		 * @brief Sets the mouse cursor to be shown on the display.
		 * 
		 * @param cur Cursor.
		 * @return true on success 
		 * @return false on failure
		 */
		bool setCursor(mouse::Cursor& cur);

		/**
		 * @brief Sets the mouse cursor to one of the OS's predefined cursors.
		 * For details, see https://liballeg.org/a5docs/trunk/mouse.html#al_set_system_mouse_cursor
		 * 
		 * @param id 
		 * @return true 
		 * @return false 
		 */
		bool setSystemCursor(int id);

		///@brief Hides the mouse cursor.
		void hideCursor();

		///@brief Shows the mouse cursor.
		void showCursor();

		///@brief Constrains the mouse to the display.
		void grabMouse();

		///@brief Reverses the effect of a grabMouse() call.
		void ungrabMouse();

		///@returns A reference to the display's backbuffer bitmap.
		const Bitmap& backbuffer() const;

		///@brief Returns the display's event source.
		const EventSource& eventSource();

	protected:
		void initPointers();
	private:
		std::unique_ptr<DisplayEventSource> ptrEventSource;
		std::unique_ptr<DisplayBackbuffer> ptrBackbuffer;
	};
	
	class TCurrentDisplay: public Display {
	public:
		TCurrentDisplay() : Display(nullptr) 
		{
			initPointers();
		}
		
		void flip();
		void flip(Rect<int> rect);
		bool waitForVsync();
		void convertMemoryBitmaps();

		void setTargetBitmap(Bitmap& bmp);
	private:
		virtual ALLEGRO_DISPLAY* getPointer() const override
		{
			return al_get_current_display();
		}
	};

	extern TCurrentDisplay CurrentDisplay;
}

#endif /* INCLUDE_AXXEGRO_DISPLAY_DISPLAY */
