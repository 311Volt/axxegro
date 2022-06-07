#ifndef AXXEGRO_INCLUDE_AXXEGRO_USTR
#define AXXEGRO_INCLUDE_AXXEGRO_USTR

#include <allegro5/allegro.h>
#include <string>
#include <string_view>

#include <axxegro/resources/Resource.hpp>

/**
 * @file
 * RAII for ALLEGRO_USTR (incomplete)
 */

namespace al {
	
	AXXEGRO_DEFINE_DELETER(ALLEGRO_USTR, al_ustr_free);

	class UStr: public Resource<ALLEGRO_USTR> {
	public:
		///@brief Initializes the string from a UTF-32 input.
		UStr(const std::u32string_view str);

		///@brief Initializes the string from a UTF-8 input.
		UStr(const std::string_view str);
	};

	

	/**
	 * @brief Converts UTF-32 to UTF-8 using Allegro.
	 * 
	 * @param str UTF-32 input.
	 * @return UTF-8 output.
	 */
	std::string ToUTF8(const std::u32string_view str);

	/**
	 * @brief Converts UTF-8 to UTF-32 using Allegro.
	 * 
	 * @param str UTF-8 input
	 * @return UTF-32 output. 
	 */
	std::u32string ToUTF32(const std::string_view str);
}

#endif /* AXXEGRO_INCLUDE_AXXEGRO_USTR */
