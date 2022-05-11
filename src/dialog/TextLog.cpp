#define AXXEGRO_TRUSTED

#include <axxegro/dialog/TextLog.hpp>

#include <stdarg.h>

ALLEGRO_EVENT_SOURCE* al::TextLogEventSource::ptr() const
{
	return al_get_native_text_log_event_source(tl.ptr());
}

void al::TextLog::write(const std::string& text)
{
	writef("%s", text.c_str());
}

void al::TextLog::writeln(const std::string& text)
{
	writef("%s\n", text.c_str());
}