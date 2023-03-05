#ifndef INCLUDE_AXXEGRO_IO_KEYBOARD
#define INCLUDE_AXXEGRO_IO_KEYBOARD

#include <allegro5/allegro.h>
#include <string>

#include "../event/EventSource.hpp"

/**
 * @file
 * For further reference, go here:
 * https://liballeg.org/a5docs/trunk/keyboard.html
 */

namespace al {
	using KeyboardState = ALLEGRO_KEYBOARD_STATE;

	class KeyboardEventSource: public EventSource {
	public:
		[[nodiscard]] ALLEGRO_EVENT_SOURCE* ptr() const override {
			return al_get_keyboard_event_source();
		}
	};

	///@return Current keyboard state.
	inline KeyboardState GetKeyboardState() {
		ALLEGRO_KEYBOARD_STATE ret;
		al_get_keyboard_state(&ret);
		return ret;
	}

	/**
		* @brief Checks whether a key is down. If you need to check for
		* multiple keys, use GetState() in combination with this function
		* for performance.
		*/
	inline bool IsKeyDown(const KeyboardState& kbState, int keycode)
	{
		return al_key_down(&kbState, keycode);
	}

	/**
	* @brief Checks whether a key is down.
	* Keep in mind that this calls al_get_keyboard_state and is
	* slightly more inefficient than the two-argument overload.
	*
	* @param keycode Keycode.
	* @return true if the key is down
	* @return false if the key is up
	*/
	inline bool IsKeyDown(int keycode)
	{
		return IsKeyDown(GetKeyboardState(), keycode);
	}

	enum KeybLED {
		NumLock = ALLEGRO_KEYMOD_NUMLOCK,
		CapsLock = ALLEGRO_KEYMOD_CAPSLOCK,
		ScrollLock = ALLEGRO_KEYMOD_SCROLLLOCK
	};

	///@brief See: https://liballeg.org/a5docs/trunk/keyboard.html#al_set_keyboard_leds (TODO constants)
	inline bool SetKeyboardLEDs(int ledBitField) {
		return al_set_keyboard_leds(ledBitField);
	}

	///@brief See: https://liballeg.org/a5docs/trunk/keyboard.html#al_keycode_to_name
	inline std::string KeycodeToName(int keycode) {
		return {al_keycode_to_name(keycode)};
	}

	///@return The keyboard's event source.
	inline const EventSource& GetKeyboardEventSource() {
		static al::KeyboardEventSource keyboardEventSource;
		return keyboardEventSource;
	}
}

#endif /* INCLUDE_AXXEGRO_IO_KEYBOARD */
