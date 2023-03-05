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
	inline int MessageBox(
		const std::string& title,
		const std::string& heading,
		const std::string& text = "",
		const std::string& buttons = "",
		int flags = 0,
		std::optional<std::reference_wrapper<Display>> parentDisplay = std::nullopt
	)
	{
		return al_show_native_message_box(
				parentDisplay ? parentDisplay->get().ptr() : nullptr,
				title.c_str(),
				heading.c_str(),
				text.c_str(),
				buttons.size() ? buttons.c_str() : nullptr,
				flags
		);
	}

}

#endif /* INCLUDE_AXXEGRO_DIALOG_MESSAGEBOX */
