#include "axxegro/display/DisplayModes.hpp"
#define AXXEGRO_TRUSTED

#include <axxegro/display/Display.hpp>
#include <stdexcept>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

al::TCurrentDisplay al::CurrentDisplay;

ALLEGRO_BITMAP* al::DisplayBackbuffer::getPointer() const
{
	return al_get_backbuffer(disp.ptr());
}

ALLEGRO_EVENT_SOURCE* al::DisplayEventSource::ptr() const
{
	return al_get_display_event_source(disp.ptr());
}

al::Display::Display(int w, int h, int flags, std::vector<Option> requiredOptions, std::vector<Option> suggestedOptions, std::vector<Option> dontCareOptions)
	: Resource(nullptr)
{
	if(!al_is_system_installed()) {
		throw DisplayCreationError("cannot create display (did you forget to initialize allegro?)");
	}
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
	al_set_new_display_flags(flags);

	setPtr(al_create_display(w, h));
	if(!ptr()) {
		throw DisplayCreationError(fmt::format("Could not create a {}x{} Allegro display.", w, h));
	}
	al_reset_new_display_options();
	initPointers();
}

void al::Display::initPointers()
{
	ptrBackbuffer = std::make_unique<DisplayBackbuffer>(*this);
	ptrEventSource = std::make_unique<DisplayEventSource>(*this);
}

int al::Display::width() const
{
	return al_get_display_width(ptr());
}
int al::Display::height() const
{
	return al_get_display_height(ptr());
}

al::Vec2<int> al::Display::size() const
{
	return {width(), height()};
}

al::Rect<int> al::Display::rect() const
{
	return {{0,0}, size()};
}

double al::Display::aspectRatio() const
{
	return double(width()) / double(height());
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
al::Coord<int> al::Display::getPosition() const
{
	int x, y;
	al_get_window_position(ptr(), &x, &y);
	return {x, y};
}
void al::Display::setPosition(al::Coord<int> pos)
{
	al_set_window_position(ptr(), pos.x, pos.y);
}

int al::Display::getFlags() const
{
	return al_get_display_flags(ptr());
}

std::optional<std::string> al::Display::getClipboardText() const
{
	std::string ret;
	char* buf = al_get_clipboard_text(ptr());
	if(buf) {
		ret = buf;
		al_free(buf);
	} else {
		return std::nullopt;
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

const al::EventSource& al::Display::eventSource()
{
	return *ptrEventSource;
}

const al::Bitmap& al::Display::backbuffer() const
{
	return *ptrBackbuffer;
}

int al::Display::findFramerateCap()
{
	int ret = getRefreshRate();
	if(ret == 0) {
		auto modes = GetDisplayModes();
		ret = 0;
		for(auto& mode: modes) {
			ret = std::max(ret, mode.refresh_rate);
		}
	}
	return std::clamp(ret, 30, 300);
}

void al::TCurrentDisplay::flip()
{
	al_flip_display();
}
void al::TCurrentDisplay::flip(Rect<int> rect)
{
	al_update_display_region(rect.a.x, rect.a.y, rect.width(), rect.height());
}

bool al::TCurrentDisplay::waitForVsync()
{
	return al_wait_for_vsync();
}
void al::TCurrentDisplay::convertMemoryBitmaps()
{
	al_convert_memory_bitmaps();
}

void al::TCurrentDisplay::setTargetBitmap(Bitmap& bmp)
{
	al_set_target_bitmap(bmp.ptr());
}
