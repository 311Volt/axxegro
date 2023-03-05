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

	std::string ToUTF8(const std::u32string_view str);

	class UStr: public Resource<ALLEGRO_USTR> {
	public:
		///@brief Initializes the string from a UTF-32 input.
		UStr(const std::u32string_view str) : UStr(ToUTF8(str)) {}

		///@brief Initializes the string from a UTF-8 input.
		UStr(const std::string_view str) : Resource<ALLEGRO_USTR>(al_ustr_new_from_buffer(str.data(), str.size())) {}
	};

	

	/**
	 * @brief Converts UTF-32 to UTF-8 using Allegro.
	 * 
	 * @param str UTF-32 input.
	 * @return UTF-8 output.
	 */
	inline std::string ToUTF8(const std::u32string_view str) {
		std::string ret;
		size_t rs = 0;
		for(auto chr: str) {
			rs += al_utf8_width(chr);
		}
		ret.reserve(rs);
		char buf[8];
		for(auto chr: str) {
			size_t len = al_utf8_encode(buf, chr);
			ret.insert(ret.length(), buf, len);
		}
		return ret;
	}

	/**
	 * @brief Converts UTF-8 to UTF-32 using Allegro.
	 * 
	 * @param str UTF-8 input
	 * @return UTF-32 output. 
	 */
	inline std::u32string ToUTF32(const std::string_view str) {
		UStr ustr(str);
		size_t len = al_ustr_length(ustr.ptr());
		std::u32string ret;
		ret.reserve(len);
		for(int32_t pos=0; pos>=0;) {
			int32_t chr = al_ustr_get_next(ustr.ptr(), &pos);
			if(chr >= 0) {
				ret.push_back(chr);
			} else {
				break;
			}
		}
		return ret;
	}
}

#endif /* AXXEGRO_INCLUDE_AXXEGRO_USTR */
