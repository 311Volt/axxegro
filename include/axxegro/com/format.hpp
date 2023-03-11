#ifndef INCLUDE_AXXEGRO_FORMAT
#define INCLUDE_AXXEGRO_FORMAT

#include <cstdio>
#include <string>

namespace al {

	/* A convenient wrapper for snprintf when std::format is
	 * not available. */

	template<typename... Args>
	inline std::string Format(const char* fmt, Args... args)
	{
		int sz = std::snprintf(nullptr, 0, fmt, args...);
		if(sz < 0) {
			return {};
		}
		std::string ret(sz, '\0');
		std::snprintf(ret.data(), sz+1, fmt, args...);
		return ret;
	}

}

#endif /* INCLUDE_AXXEGRO_FORMAT */
