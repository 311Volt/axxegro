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
		State GetState();

		bool IsKeyDown(int keycode);
		bool IsKeyDown(State& kbState, int keycode);

		bool SetLEDs(int ledBitField);

		std::string KeycodeToName(int keycode);

		EventSource GetEventSource();
	};
}

#endif /* INCLUDE_AXXEGRO_IO_KEYBOARD */
