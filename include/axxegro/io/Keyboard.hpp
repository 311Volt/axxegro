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
	namespace keyb {
		using State = ALLEGRO_KEYBOARD_STATE;

		class KeyboardEventSource: public EventSource {
		public:
			ALLEGRO_EVENT_SOURCE* ptr() const override
				{return al_get_keyboard_event_source();}
		};

		///@return Current keyboard state.
		State GetState();

		/**
		 * @brief Checks whether a key is down.
		 * Keep in mind that this calls al_get_keyboard_state and is
		 * inefficient.
		 * 
		 * @param keycode Keycode.
		 * @return true if the key is down
		 * @return false if the key is up
		 */
		bool IsKeyDown(int keycode);

		/**
		 * @brief Checks whether a key is down. If you need to check for
		 * multiple keys, use GetState() in combination with this function
		 * for performance.
		 */
		bool IsKeyDown(State& kbState, int keycode);

		///@brief See: https://liballeg.org/a5docs/trunk/keyboard.html#al_set_keyboard_leds
		bool SetLEDs(int ledBitField);

		///@brief See: https://liballeg.org/a5docs/trunk/keyboard.html#al_keycode_to_name
		std::string KeycodeToName(int keycode);

		///@return The keyboard's event source.
		const EventSource& GetEventSource();
	}
}

#endif /* INCLUDE_AXXEGRO_IO_KEYBOARD */
