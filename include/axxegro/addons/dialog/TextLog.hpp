#ifndef INCLUDE_AXXEGRO_DIALOG_TEXTLOG
#define INCLUDE_AXXEGRO_DIALOG_TEXTLOG

#include "axxegro/common.hpp"
#include "axxegro/core/event.hpp"
#include "NativeDialogAddon.hpp"

#include <allegro5/allegro_native_dialog.h>


namespace al {

	AXXEGRO_DEFINE_DELETER(ALLEGRO_TEXTLOG, al_close_native_text_log);

	class TextLog;
	class TextLogEventSource: public EventSource {
	public:
		explicit TextLogEventSource(TextLog& tl)
			: tl(tl)
		{}

		[[nodiscard]] ALLEGRO_EVENT_SOURCE* ptr() const override;
	private:
		const TextLog& tl;
	};


	class TextLog:
			RequiresInitializables<NativeDialogAddon>,
			public Resource<ALLEGRO_TEXTLOG> {
	public:
		explicit TextLog(const std::string& title = "Log", int flags = 0)
			: Resource<ALLEGRO_TEXTLOG>(al_open_native_text_log(title.c_str(), flags))
		{
			if(!ptr()) {
				throw al::ResourceLoadError("Cannot open native textlog (title=%s)", title.c_str());
			}
		}

		template<typename... Args>
		void writef(const char* fmt, Args... args)
		{
			al_append_native_text_log(ptr(), fmt, args...);
		}
		
		void write(const std::string& text) {
			writef("%s", text.c_str());
		}
		void writeln(const std::string& text) {
			writef("%s\n", text.c_str());
		}
	};

	inline ALLEGRO_EVENT_SOURCE* al::TextLogEventSource::ptr() const
	{
		return al_get_native_text_log_event_source(tl.ptr());
	}

}

#endif /* INCLUDE_AXXEGRO_DIALOG_TEXTLOG */
