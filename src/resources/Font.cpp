#include <axxegro/resources/Font.hpp>
#include <axxegro/UStr.hpp>
#include <allegro5/allegro_ttf.h>

#include <fmt/format.h>

al::Font::Font()
{
	font = decltype(font)(al_create_builtin_font());
}
al::Font::Font(Bitmap& bmp, std::vector<CharRange> ranges)
{
	std::vector<int> rangesFlat;
	rangesFlat.reserve(ranges.size()*2);
	for(const auto& [a, b]: ranges) {
		rangesFlat.push_back(a);
		rangesFlat.push_back(b);
	}

	font = decltype(font)(al_grab_font_from_bitmap(bmp.alPtr(), ranges.size(), rangesFlat.data()));
	if(!font) {
		throw ResourceLoadError(fmt::format(
			"Error while grabbing font from a {}x{} bitmap",
			bmp.getWidth(), bmp.getHeight()
		));
	}
}
al::Font::Font(const std::string& filename, int size)
	: Font(filename, size, 0)
{

}
al::Font::Font(const std::string& filename, int size, int flags)
{
	font = decltype(font)(al_load_font(filename.c_str(), size, flags));
	if(!font) {
		throw ResourceLoadError(fmt::format(
			"Cannot load font from \"{}\" - file missing, corrupted or invalid",
			filename
		));
	}
}

ALLEGRO_FONT* al::Font::alPtr()
{
	return font.get();
}

int al::Font::getLineHeight() const
{
	return al_get_font_line_height(font.get());
}
int al::Font::getAscent() const
{
	return al_get_font_ascent(font.get());
}
int al::Font::getDescent() const
{
	return al_get_font_descent(font.get());
}

int al::Font::getTextWidth(const std::string& text) const
{
	UStr ustr(text);
	return al_get_ustr_width(font.get(), ustr.alPtr());
}
al::Rect al::Font::getTextDimensions(const std::string& text) const
{
	UStr ustr(text);
	int x,y,w,h;
	al_get_ustr_dimensions(font.get(), ustr.alPtr(), &x, &y, &w, &h);
	Point pos{x,y}, size{w,h};
	return {pos, pos+size};
}

int al::Font::getGlyphAdvance(char32_t codepoint1, char32_t codepoint2)
{
	return al_get_glyph_advance(font.get(), codepoint1, codepoint2);
}

size_t al::Font::calcCutoffPoint(std::u32string_view str, int maxWidth)
{
	int pos = 0;
	size_t ret = 0;

	for(size_t i=0; i<str.size(); i++) {
		auto cp1 = str[i];
		auto cp2 = (i+1 == str.size()) ? ALLEGRO_NO_KERNING : str[i+1];

		pos += getGlyphAdvance(cp1, cp2);
		if(pos <= maxWidth) {
			ret++;
		} else {
			break;
		}
	}
	return ret;
}

size_t al::Font::calcCutoffPoint(std::string_view str, int maxWidth)
{
	UStr ustr(str);
	auto cpOff = calcCutoffPoint(UStr::DecodeToUTF32(str), maxWidth);
	return al_ustr_offset(ustr.alPtr(), cpOff);
}

void al::Font::draw(const std::string& text, al::Color color, al::Point pos) const
{
	draw(text, color, pos, ALLEGRO_ALIGN_LEFT);
}
void al::Font::draw(const std::string& text, al::Color color, al::Point pos, int align) const
{
	UStr ustr(text);
	al_draw_ustr(font.get(), color.get(), pos.x, pos.y, align | ALLEGRO_ALIGN_INTEGER, ustr.alPtr());
}

void al::Font::drawJustified(const std::string& text, al::Color color, al::Point pos, float xMax, float diffMax) const
{
	UStr ustr(text);
	al_draw_justified_ustr(font.get(), color.get(), pos.x, xMax, pos.y, diffMax, ALLEGRO_ALIGN_INTEGER, ustr.alPtr());
}
