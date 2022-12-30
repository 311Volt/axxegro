#include <axxegro/io/Keyboard.hpp>

ALLEGRO_KEYBOARD_STATE al::GetKeyboardState()
{
	ALLEGRO_KEYBOARD_STATE ret;
	al_get_keyboard_state(&ret);
	return ret;
}
bool al::IsKeyDown(int keycode)
{
	return IsKeyDown(GetKeyboardState(), keycode);
}
bool al::IsKeyDown(const al::KeyboardState& kbState, int keycode)
{
	return al_key_down(&kbState, keycode);
}
bool al::SetKeyboardLEDs(int flags)
{
	return al_set_keyboard_leds(flags);
}
std::string al::KeycodeToName(int keycode)
{
	return std::string(al_keycode_to_name(keycode));
}

al::KeyboardEventSource axxKeyboardEventSource;

const al::EventSource& al::GetKeyboardEventSource()
{
	return axxKeyboardEventSource;
}
