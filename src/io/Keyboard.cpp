#include <axxegro/io/Keyboard.hpp>


ALLEGRO_KEYBOARD_STATE al::keyb::GetState()
{
	ALLEGRO_KEYBOARD_STATE ret;
	al_get_keyboard_state(&ret);
	return ret;
}
bool al::keyb::IsKeyDown(int keycode)
{
	ALLEGRO_KEYBOARD_STATE state = GetState();
	return al_key_down(&state, keycode);
}
bool al::keyb::SetLEDs(int flags)
{
	return al_set_keyboard_leds(flags);
}
std::string al::keyb::KeycodeToName(int keycode)
{
	return std::string(al_keycode_to_name(keycode));
}

al::keyb::KeyboardEventSource axxKeyboardEventSource;

const al::EventSource& al::keyb::GetEventSource()
{
	return axxKeyboardEventSource;
}