#ifndef INCLUDE_AXXEGRO_IO_KEYBOARD
#define INCLUDE_AXXEGRO_IO_KEYBOARD

#include "../../common.hpp"

/**
 * @file
 * For further reference, go here:
 * https://liballeg.org/a5docs/trunk/keyboard.html
 */

namespace al {
	using KeyboardState = ALLEGRO_KEYBOARD_STATE;

	struct KeyboardDriver {
		static constexpr char name[] = "Keyboard driver";
		[[nodiscard]] static bool init() {return al_install_keyboard();}
		[[nodiscard]] static bool isInitialized() {return al_is_keyboard_installed();}
		using DependsOn = InitDependencies<CoreAllegro>;
	};

	class KeyboardEventSource:
			RequiresInitializables<KeyboardDriver>,
			public EventSource {
	public:
		[[nodiscard]] ALLEGRO_EVENT_SOURCE* ptr() const override {
			return al_get_keyboard_event_source();
		}
	};

	///@return Current keyboard state.
	inline KeyboardState GetKeyboardState() {
		InternalRequire<KeyboardDriver>();
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

	///@brief See: https://liballeg.org/a5docs/trunk/keyboard.html#al_set_keyboard_leds
	inline bool SetKeyboardLEDs(int ledBitField) {
		InternalRequire<KeyboardDriver>();
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

	enum class Key {
		A = ALLEGRO_KEY_A,
		B = ALLEGRO_KEY_B,
		C = ALLEGRO_KEY_C,
		D = ALLEGRO_KEY_D,
		E = ALLEGRO_KEY_E,
		F = ALLEGRO_KEY_F,
		G = ALLEGRO_KEY_G,
		H = ALLEGRO_KEY_H,
		I = ALLEGRO_KEY_I,
		J = ALLEGRO_KEY_J,
		K = ALLEGRO_KEY_K,
		L = ALLEGRO_KEY_L,
		M = ALLEGRO_KEY_M,
		N = ALLEGRO_KEY_N,
		O = ALLEGRO_KEY_O,
		P = ALLEGRO_KEY_P,
		Q = ALLEGRO_KEY_Q,
		R = ALLEGRO_KEY_R,
		S = ALLEGRO_KEY_S,
		T = ALLEGRO_KEY_T,
		U = ALLEGRO_KEY_U,
		V = ALLEGRO_KEY_V,
		W = ALLEGRO_KEY_W,
		X = ALLEGRO_KEY_X,
		Y = ALLEGRO_KEY_Y,
		Z = ALLEGRO_KEY_Z,
		NumRow0 = ALLEGRO_KEY_0,
		NumRow1 = ALLEGRO_KEY_1,
		NumRow2 = ALLEGRO_KEY_2,
		NumRow3 = ALLEGRO_KEY_3,
		NumRow4 = ALLEGRO_KEY_4,
		NumRow5 = ALLEGRO_KEY_5,
		NumRow6 = ALLEGRO_KEY_6,
		NumRow7 = ALLEGRO_KEY_7,
		NumRow8 = ALLEGRO_KEY_8,
		NumRow9 = ALLEGRO_KEY_9,
		NumPad0 = ALLEGRO_KEY_PAD_0,
		NumPad1 = ALLEGRO_KEY_PAD_1,
		NumPad2 = ALLEGRO_KEY_PAD_2,
		NumPad3 = ALLEGRO_KEY_PAD_3,
		NumPad4 = ALLEGRO_KEY_PAD_4,
		NumPad5 = ALLEGRO_KEY_PAD_5,
		NumPad6 = ALLEGRO_KEY_PAD_6,
		NumPad7 = ALLEGRO_KEY_PAD_7,
		NumPad8 = ALLEGRO_KEY_PAD_8,
		NumPad9 = ALLEGRO_KEY_PAD_9,
		F1 = ALLEGRO_KEY_F1,
		F2 = ALLEGRO_KEY_F2,
		F3 = ALLEGRO_KEY_F3,
		F4 = ALLEGRO_KEY_F4,
		F5 = ALLEGRO_KEY_F5,
		F6 = ALLEGRO_KEY_F6,
		F7 = ALLEGRO_KEY_F7,
		F8 = ALLEGRO_KEY_F8,
		F9 = ALLEGRO_KEY_F9,
		F10 = ALLEGRO_KEY_F10,
		F11 = ALLEGRO_KEY_F11,
		F12 = ALLEGRO_KEY_F12,
		Esc = ALLEGRO_KEY_ESCAPE,
		Tilde = ALLEGRO_KEY_TILDE,
		Minus = ALLEGRO_KEY_MINUS,
		Equals = ALLEGRO_KEY_EQUALS,
		Backspace = ALLEGRO_KEY_BACKSPACE,
		Tab = ALLEGRO_KEY_TAB,
		OpenBrace = ALLEGRO_KEY_OPENBRACE,
		CloseBrace = ALLEGRO_KEY_CLOSEBRACE,
		Enter = ALLEGRO_KEY_ENTER,
		Semicolon = ALLEGRO_KEY_SEMICOLON,
		Quote = ALLEGRO_KEY_QUOTE,
		Backslash = ALLEGRO_KEY_BACKSLASH,
		Backslash2 = ALLEGRO_KEY_BACKSLASH2,
		Comma = ALLEGRO_KEY_COMMA,
		FullStop = ALLEGRO_KEY_FULLSTOP,
		Slash = ALLEGRO_KEY_SLASH,
		Space = ALLEGRO_KEY_SPACE,
		Insert = ALLEGRO_KEY_INSERT,
		Delete = ALLEGRO_KEY_DELETE,
		Home = ALLEGRO_KEY_HOME,
		End = ALLEGRO_KEY_END,
		PgUp = ALLEGRO_KEY_PGUP,
		PgDn = ALLEGRO_KEY_PGDN,
		Left = ALLEGRO_KEY_LEFT,
		Right = ALLEGRO_KEY_RIGHT,
		Up = ALLEGRO_KEY_UP,
		Down = ALLEGRO_KEY_DOWN,
		PadSlash = ALLEGRO_KEY_PAD_SLASH,
		PadAsterisk = ALLEGRO_KEY_PAD_ASTERISK,
		PadMinus = ALLEGRO_KEY_PAD_MINUS,
		PadPlus = ALLEGRO_KEY_PAD_PLUS,
		PadDelete = ALLEGRO_KEY_PAD_DELETE,
		PadEnter = ALLEGRO_KEY_PAD_ENTER,
		PrtSc = ALLEGRO_KEY_PRINTSCREEN,
		Pause = ALLEGRO_KEY_PAUSE,
		AbntC1 = ALLEGRO_KEY_ABNT_C1,
		Yen = ALLEGRO_KEY_YEN,
		Kana = ALLEGRO_KEY_KANA,
		Convert = ALLEGRO_KEY_CONVERT,
		NoConvert = ALLEGRO_KEY_NOCONVERT,
		At = ALLEGRO_KEY_AT,
		Circumflex = ALLEGRO_KEY_CIRCUMFLEX,
		Colon2 = ALLEGRO_KEY_COLON2,
		Kanji = ALLEGRO_KEY_KANJI,
		LShift = ALLEGRO_KEY_LSHIFT,
		RShift = ALLEGRO_KEY_RSHIFT,
		LCtrl = ALLEGRO_KEY_LCTRL,
		RCtrl = ALLEGRO_KEY_RCTRL,
		Alt = ALLEGRO_KEY_ALT,
		AltGr = ALLEGRO_KEY_ALTGR,
		LWin = ALLEGRO_KEY_LWIN,
		RWin = ALLEGRO_KEY_RWIN,
		Menu = ALLEGRO_KEY_MENU,
		ScrollLock = ALLEGRO_KEY_SCROLLLOCK,
		NumLock = ALLEGRO_KEY_NUMLOCK,
		CapsLock = ALLEGRO_KEY_CAPSLOCK,
		PadEquals = ALLEGRO_KEY_PAD_EQUALS,
		BackQuote = ALLEGRO_KEY_BACKQUOTE,
		Semicolon2 = ALLEGRO_KEY_SEMICOLON2,
		Command = ALLEGRO_KEY_COMMAND,

		/* Since: 5.1.1 */
		/* Android only for now */
		Back = ALLEGRO_KEY_BACK,

		/* Since: 5.1.2 */
		/* Android only for now */
		VolUp = ALLEGRO_KEY_VOLUME_UP,
		VolDown = ALLEGRO_KEY_VOLUME_DOWN,

		/* Since: 5.1.6 */
		/* Android only for now */
		Search = ALLEGRO_KEY_SEARCH,
		DPadCenter = ALLEGRO_KEY_DPAD_CENTER,
		ButtonX = ALLEGRO_KEY_BUTTON_X,
		ButtonY = ALLEGRO_KEY_BUTTON_Y,
		DPadUp = ALLEGRO_KEY_DPAD_UP,
		DPadDown = ALLEGRO_KEY_DPAD_DOWN,
		DPadLeft = ALLEGRO_KEY_DPAD_LEFT,
		DPadRight = ALLEGRO_KEY_DPAD_RIGHT,
		Select = ALLEGRO_KEY_SELECT,
		Start = ALLEGRO_KEY_START
	};
}

#endif /* INCLUDE_AXXEGRO_IO_KEYBOARD */
