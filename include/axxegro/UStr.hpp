#ifndef INCLUDE_AXXEGRO_USTR
#define INCLUDE_AXXEGRO_USTR

#include <allegro5/allegro.h>
#include <string>
#include <string_view>

/**
 * @file
 * RAII for ALLEGRO_USTR. This is not a full wrapper and is mostly intended
 * to be used by Font::draw(). It is assumed that the user will do all of the
 * text processing using C++ strings which will then only be converted to
 * ALLEGRO_USTR for drawing, with negligible cost compared to the al_draw_ustr
 * call.
 */

namespace al {
	class UStr {
	private:
		ALLEGRO_USTR* ptr;
	public:
		///@brief Initializes the string from a UTF-32 input.
		UStr(const std::u32string_view str);

		///@brief Initializes the string from a UTF-8 input.
		UStr(const std::string_view str);

		///@brief Destroys the string.
		~UStr();

		/**
		 * @brief Converts UTF-32 to UTF-8 using Allegro.
		 * 
		 * @param str UTF-32 input.
		 * @return UTF-8 output.
		 */
		static std::string EncodeToUTF8(const std::u32string_view str);

		/**
		 * @brief Converts UTF-8 to UTF-32 using Allegro.
		 * 
		 * @param str UTF-8 input
		 * @return UTF-32 output. 
		 */
		static std::u32string DecodeToUTF32(const std::string_view str);

		///@return A pointer to the underlying ALLEGRO_USTR.
		ALLEGRO_USTR* alPtr();
	};
}

#endif /* INCLUDE_AXXEGRO_USTR */
