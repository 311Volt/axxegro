#ifndef INCLUDE_AXXEGRO_EXCEPTION
#define INCLUDE_AXXEGRO_EXCEPTION

/**
 * @file Exception.hpp
 * axxegro internal exceptions
 */

#include <exception>
#include <stdexcept>
#include <string_view>
#include <cstdarg>
#include "axxegro/com/util/format.hpp"

namespace al
{
	struct Exception: public std::runtime_error
	{
		using std::runtime_error::runtime_error;

		[[gnu::format(printf, 2, 3)]] explicit Exception(const char* msg, ...)
			: std::runtime_error("")
		{
			std::va_list args;
			va_start(args, msg);
			setMsg(msg, args);
			va_end(args);
		}
	protected:
		void setMsg(const char* msg, std::va_list args) {
			auto msgstr = VFormat(msg, args);
			static_cast<std::runtime_error*>(this)->operator=(std::runtime_error(msgstr));
		}
	};

#define AXXEGRO_DEF_EXCEPTION(cparent, exname) \
	struct exname: public cparent { \
		explicit exname(const std::string& msg) : cparent(msg) {} \
		[[gnu::format(printf, 2, 3)]] explicit exname(const char* msg, ...) : cparent("%s", "") {  \
			std::va_list args; \
			va_start(args, msg); \
            setMsg(msg, args); \
			va_end(args); \
		} \
	};

	AXXEGRO_DEF_EXCEPTION(Exception, InitJobFailed);
	AXXEGRO_DEF_EXCEPTION(Exception, InitializeFailed);
	AXXEGRO_DEF_EXCEPTION(Exception, ShaderError);
		AXXEGRO_DEF_EXCEPTION(ShaderError, ShaderSourceError);
		AXXEGRO_DEF_EXCEPTION(ShaderError, ShaderBuildError);
    AXXEGRO_DEF_EXCEPTION(Exception, AudioError);
	AXXEGRO_DEF_EXCEPTION(Exception, AddonError);
	AXXEGRO_DEF_EXCEPTION(Exception, DisplayError);
	AXXEGRO_DEF_EXCEPTION(Exception, DisplayCreationError);
	AXXEGRO_DEF_EXCEPTION(Exception, BitmapLockError);
	AXXEGRO_DEF_EXCEPTION(Exception, ResourceLoadError);
	AXXEGRO_DEF_EXCEPTION(Exception, InvalidTransformError);
	AXXEGRO_DEF_EXCEPTION(Exception, OutOfRangeError);
	AXXEGRO_DEF_EXCEPTION(Exception, EventDispatcherError);
	AXXEGRO_DEF_EXCEPTION(Exception, EventQueueError);
	AXXEGRO_DEF_EXCEPTION(Exception, EventSourceError);
	AXXEGRO_DEF_EXCEPTION(Exception, HardwareBufferError);
		AXXEGRO_DEF_EXCEPTION(HardwareBufferError, VertexBufferError);
		AXXEGRO_DEF_EXCEPTION(HardwareBufferError, IndexBufferError);
	AXXEGRO_DEF_EXCEPTION(Exception, ConfigError);
		AXXEGRO_DEF_EXCEPTION(ConfigError, ConfigEntryTypeError);
	

#undef AXXEGRO_DEF_EXCEPTION

}

#endif /* INCLUDE_AXXEGRO_EXCEPTION */
