#define AXXEGRO_TRUSTED

#include <axxegro/display/Display.hpp>
#include <stdexcept>

al::Display::Display(int w, int h)
	: Display(w, h, 0)
{

}
al::Display::Display(int w, int h, int flags)
	: Display(w, h, flags, {}, {}, {})
{

}
al::Display::Display(int w, int h, int flags, std::vector<Option> requiredOptions, std::vector<Option> suggestedOptions, std::vector<Option> dontCareOptions)
	: Resource(nullptr)
{
	al_reset_new_display_options();
	for(const auto& [opt, val]: requiredOptions) {
		al_set_new_display_option(opt, val, ALLEGRO_REQUIRE);
	}
	for(const auto& [opt, val]: suggestedOptions) {
		al_set_new_display_option(opt, val, ALLEGRO_SUGGEST);
	}
	for(const auto& [opt, val]: dontCareOptions) {
		al_set_new_display_option(opt, val, ALLEGRO_DONTCARE);
	}
	al_set_new_bitmap_flags(flags);

	setPtr(al_create_display(w, h));
	if(!ptr()) {
		throw std::runtime_error("Could not create an Allegro display.");
	}
	al_reset_new_display_options();

	ptrBackbuffer = std::make_unique<DisplayBackbuffer>(ptr());
}

int al::Display::width() const
{
	return al_get_display_width(ptr());
}
int al::Display::height() const
{
	return al_get_display_height(ptr());
}

al::Vec2 al::Display::getSize() const
{
	return {width(), height()};
}

int al::Display::getRefreshRate() const
{
	return al_get_display_refresh_rate(ptr());
}

int al::Display::getOptionValue(int option) const
{
	return al_get_display_option(ptr(), option);
}
bool al::Display::setFlag(int flag, bool onoff)
{
	return al_set_display_flag(ptr(), flag, onoff);
}

bool al::Display::resize(int w, int h)
{
	return al_resize_display(ptr(), w, h);
}
bool al::Display::acknowledgeResize()
{
	return al_acknowledge_resize(ptr());
}
al::Point al::Display::getPosition() const
{
	int x, y;
	al_get_window_position(ptr(), &x, &y);
	return {x, y};
}
void al::Display::setPosition(al::Point pos)
{
	al_set_window_position(ptr(), pos.x, pos.y);
}

int al::Display::getFlags() const
{
	return al_get_display_flags(ptr());
}

std::string al::Display::getClipboardText() const
{
	std::string ret;
	char* buf = al_get_clipboard_text(ptr());
	if(buf) {
		ret = {buf};
		al_free(buf);
	} else {
		ret = "";
	}
	return ret;
}
bool al::Display::setClipboardText(const std::string& text)
{
	return al_set_clipboard_text(ptr(), text.c_str());
}

void al::Display::setTitle(const std::string& title)
{
	al_set_window_title(ptr(), title.c_str());
}

al::EventSource al::Display::getEventSource()
{
	return EventSource(al_get_display_event_source(ptr()));
}

const al::Bitmap& al::Display::backbuffer() const
{
	return *ptrBackbuffer;
}


void al::CCurrentDisplay::flip()
{
	al_flip_display();
}
void al::CCurrentDisplay::flip(Rect rect)
{
	al_update_display_region(rect.a.x, rect.a.y, rect.getWidth(), rect.getHeight());
}
void al::CCurrentDisplay::clearToColor(Color color)
{
	al_clear_to_color(color.get());
}
bool al::CCurrentDisplay::waitForVsync()
{
	return al_wait_for_vsync();
}
void al::CCurrentDisplay::convertMemoryBitmaps()
{
	al_convert_memory_bitmaps();
}

void al::CCurrentDisplay::setTargetBitmap(Bitmap& bmp)
{
	al_set_target_bitmap(bmp.ptr());
}

al::CCurrentDisplay axxCurrentDisplay;
al::CCurrentDisplay& al::CurrentDisplay()
{
	return axxCurrentDisplay;
}