#include <axxegro/resources/Font.hpp>
#include <axxegro/UStr.hpp>
#include <allegro5/allegro_ttf.h>

al::Font::Font()
{
	ptr = al_create_builtin_font();
}
al::Font::Font(Bitmap& bmp, std::vector<CharRange> ranges)
{
	std::vector<int> rangesFlat;
	rangesFlat.reserve(ranges.size()*2);
	for(const auto& [a, b]: ranges) {
		rangesFlat.push_back(a);
		rangesFlat.push_back(b);
	}

	ptr = al_grab_font_from_bitmap(bmp.alPtr(), ranges.size(), rangesFlat.data());
}
al::Font::Font(const std::string& filename, int size)
	: Font(filename, size, 0)
{

}
al::Font::Font(const std::string& filename, int size, int flags)
{
	ptr = al_load_font(filename.c_str(), size, flags);
}

al::Font::~Font()
{
	al_destroy_font(ptr);
}

ALLEGRO_FONT* al::Font::alPtr()
{
	return ptr;
}

int al::Font::getLineHeight() const
{
	return al_get_font_line_height(ptr);
}
int al::Font::getAscent() const
{
	return al_get_font_ascent(ptr);
}
int al::Font::getDescent() const
{
	return al_get_font_descent(ptr);
}

int al::Font::getTextWidth(const std::string& text) const
{
	UStr ustr(text);
	return al_get_ustr_width(ptr, ustr.alPtr());
}
al::Rect al::Font::getTextDimensions(const std::string& text) const
{
	UStr ustr(text);
	int x,y,w,h;
	al_get_ustr_dimensions(ptr, ustr.alPtr(), &x, &y, &w, &h);
	Point pos{x,y}, size{w,h};
	return {pos, pos+size};
}

void al::Font::draw(const std::string& text, al::Color color, al::Point pos) const
{
	draw(text, color, pos, ALLEGRO_ALIGN_LEFT);
}
void al::Font::draw(const std::string& text, al::Color color, al::Point pos, int align) const
{
	UStr ustr(text);
	al_draw_ustr(ptr, color.get(), pos.x, pos.y, align | ALLEGRO_ALIGN_INTEGER, ustr.alPtr());
}

void al::Font::drawJustified(const std::string& text, al::Color color, al::Point pos, float xMax, float diffMax) const
{
	UStr ustr(text);
	al_draw_justified_ustr(ptr, color.get(), pos.x, xMax, pos.y, diffMax, ALLEGRO_ALIGN_INTEGER, ustr.alPtr());
}
