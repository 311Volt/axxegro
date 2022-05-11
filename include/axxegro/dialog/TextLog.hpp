#ifndef INCLUDE_AXXEGRO_DIALOG_TEXTLOG
#define INCLUDE_AXXEGRO_DIALOG_TEXTLOG

#include <axxegro/resources/Resource.hpp>
#include <axxegro/event/EventSource.hpp>

#include <allegro5/allegro_native_dialog.h>


namespace al {

	AXXEGRO_DEFINE_DELETER(ALLEGRO_TEXTLOG, al_close_native_text_log);

	class TextLog;
	class TextLogEventSource: public EventSource {
	public:
		TextLogEventSource(TextLog& tl)
			: tl(tl)
		{}

		virtual ALLEGRO_EVENT_SOURCE* ptr() const override;
	private:
		const TextLog& tl;
	};


	class TextLog: public Resource<ALLEGRO_TEXTLOG> {
	public:
		TextLog(const std::string& title = "Log", int flags = 0);

		template<typename... Args>
		void writef(const char* fmt, Args... args)
		{
			al_append_native_text_log(ptr(), fmt, args...);
		}
		
		void write(const std::string& text);
		void writeln(const std::string& text);
	};
}

#endif /* INCLUDE_AXXEGRO_DIALOG_TEXTLOG */
