#ifndef INCLUDE_AXXEGRO_EXCEPTION
#define INCLUDE_AXXEGRO_EXCEPTION

/**
 * @file Exception.hpp
 * axxegro internal exceptions
 */

#include <exception>
#include <stdexcept>
#include <string_view>
#include "format.hpp"

namespace al
{
	struct Exception: public std::runtime_error
	{
		using std::runtime_error::runtime_error;

		template<typename... Args>
		Exception(const std::string& msg, Args... args)
			: std::runtime_error(Format(msg.c_str(), args...))
		{}
	};

#define AXXEGRO_DEF_EXCEPTION(cparent, exname) struct exname: public cparent {using cparent::cparent;};

	AXXEGRO_DEF_EXCEPTION(Exception, InitJobFailed);
	AXXEGRO_DEF_EXCEPTION(Exception, ShaderError);
		AXXEGRO_DEF_EXCEPTION(ShaderError, ShaderSourceError);
		AXXEGRO_DEF_EXCEPTION(ShaderError, ShaderBuildError);
    AXXEGRO_DEF_EXCEPTION(Exception, AudioError);
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
	

#undef AXXEGRO_DEF_EXCEPTION

}

#endif /* INCLUDE_AXXEGRO_EXCEPTION */
