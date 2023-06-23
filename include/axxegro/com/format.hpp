#ifndef INCLUDE_AXXEGRO_FORMAT
#define INCLUDE_AXXEGRO_FORMAT

#include <cstdio>
#include <string>
#include <cstdarg>

namespace al {

	/* A convenient wrapper for snprintf for when std::format is
	 * not available. (e.g. GCC before version 13) */

	inline std::string VFormat(const char* fmt, std::va_list srcArgs)
	{
		std::va_list args;

		va_copy(args, srcArgs);
		int sz = std::vsnprintf(nullptr, 0, fmt, args);
		va_end(args);

		if(sz < 0) {
			return {};
		}

		std::string ret(sz, '\0');
		va_copy(args, srcArgs);
		std::vsnprintf(ret.data(), sz+1, fmt, args);
		va_end(args);

		return ret;
	}

	[[gnu::format(printf, 1, 2)]] inline std::string Format(const char* fmt, ...)
	{
		std::va_list args;

		va_start(args, fmt);
		auto ret = VFormat(fmt, args);
		va_end(args);

		return ret;
	}

}

#endif /* INCLUDE_AXXEGRO_FORMAT */
