#ifndef INCLUDE_AXXEGRO_DIALOG_MESSAGEBOX
#define INCLUDE_AXXEGRO_DIALOG_MESSAGEBOX

#include "axxegro/display/Display.hpp"
#include <string>
#include <optional>
#include <allegro5/allegro_native_dialog.h>

namespace al {

	/**
	 * @brief Displays a system message box. 
	 * @details Available flags:
	 * ## ALLEGRO_MESSAGEBOX_WARN
	 * The message is a warning. This may cause a different icon (or other effects).
	 * ## ALLEGRO_MESSAGEBOX_ERROR
	 * The message is an error.
	 * ## ALLEGRO_MESSAGEBOX_QUESTION
	 * The message is a question.
	 * ## ALLEGRO_MESSAGEBOX_OK_CANCEL
	 * Display a cancel button alongside the “OK” button. Ignored if `buttons` is not empty.
	 * ## ALLEGRO_MESSAGEBOX_YES_NO
	 * Display Yes/No buttons instead of the “OK” button. Ignored if `buttons` is not empty.
	 */
	int MessageBox(
		const std::string& title,
		const std::string& heading,
		const std::string& text = "",
		const std::string& buttons = "",
		int flags = 0,
		std::optional<std::reference_wrapper<Display>> parentDisplay = std::nullopt
	);

	/**
	 * @brief Displays a message box and terminates the program.
	 * @details The message box contains the current exception message if found.
	 * The function is suitable for passing to std::set_terminate.
	 */
	void Terminate();

}

#endif /* INCLUDE_AXXEGRO_DIALOG_MESSAGEBOX */
