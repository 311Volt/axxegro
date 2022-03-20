#include <axxegro/display/Display.hpp>

al::Display::Display(int w, int h)
	: Display(w, h, 0)
{

}
al::Display::Display(int w, int h, int flags)
	: Display(w, h, flags, {}, {})
{

}
al::Display::Display(int w, int h, int flags, std::vector<Option> requiredOptions, std::vector<Option> suggestedOptions)
{
	al_reset_new_display_options();
	for(const auto& [opt, val]: requiredOptions) {
		al_set_new_display_option(opt, val, ALLEGRO_REQUIRE);
	}
	for(const auto& [opt, val]: suggestedOptions) {
		al_set_new_display_option(opt, val, ALLEGRO_SUGGEST);
	}
	al_set_new_bitmap_flags(flags);
	disp = al_create_display(w, h);
	al_reset_new_display_options();
}
al::Display::~Display()
{
	al_destroy_display(disp);
}

int al::Display::width() const
{
	return al_get_display_width(disp);
}
int al::Display::height() const
{
	return al_get_display_height(disp);
}

al::Vec2 al::Display::getSize() const
{
	return {width(), height()};
}

int al::Display::getRefreshRate() const
{
	return al_get_display_refresh_rate(disp);
}

int al::Display::getOptionValue(int option) const
{
	return al_get_display_option(disp, option);
}
bool al::Display::setFlag(int flag, bool onoff)
{
	return al_set_display_flag(disp, flag, onoff);
}

bool al::Display::resize(int w, int h)
{
	return al_resize_display(disp, w, h);
}
bool al::Display::acknowledgeResize()
{
	return al_acknowledge_resize(disp);
}
al::Point al::Display::getPosition() const
{
	int x, y;
	al_get_window_position(disp, &x, &y);
	return {x, y};
}
void al::Display::setPosition(al::Point pos)
{
	al_set_window_position(disp, pos.x, pos.y);
}

int al::Display::getFlags() const
{
	return al_get_display_flags(disp);
}

std::string al::Display::getClipboardText() const
{
	char* buf = al_get_clipboard_text(disp);
	std::string ret(buf);
	al_free(buf);
	return ret;
}
bool al::Display::setClipboardText(const std::string& text)
{
	return al_set_clipboard_text(disp, text.c_str());
}

void al::Display::setTitle(const std::string& title)
{
	al_set_window_title(disp, title.c_str());
}

al::EventSource al::Display::getEventSource()
{
	return EventSource(al_get_display_event_source(disp));
}

void al::Display::Flip()
{
	al_flip_display();
}

void al::Display::Clear(al::Color color)
{
	al_clear_to_color(color.get());
}