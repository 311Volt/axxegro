#include <string>
#define AXXEGRO_TRUSTED

#include <axxegro/resources/Font.hpp>
#include <axxegro/UStr.hpp>
#include <allegro5/allegro_ttf.h>

al::Font::Font()
	: Resource(al_create_builtin_font())
{

}
al::Font::Font(Bitmap& bmp, std::vector<CharRange> ranges)
	: Resource(nullptr)
{
	std::vector<int> rangesFlat;
	rangesFlat.reserve(ranges.size()*2);
	for(const auto& [a, b]: ranges) {
		rangesFlat.push_back(a);
		rangesFlat.push_back(b);
	}

	setPtr(al_grab_font_from_bitmap(bmp.ptr(), ranges.size(), rangesFlat.data()));
	if(!ptr()) {
		throw ResourceLoadError("Error while grabbing font from a " + std::to_string(bmp.width()) + "x" + std::to_string(bmp.height()) + " bitmap");
	}
}
al::Font::Font(const std::string& filename, int size)
	: Font(filename, size, 0)
{

}
al::Font::Font(const std::string& filename, int size, int flags)
	: Resource(al_load_font(filename.c_str(), size, flags))
{
	if(!ptr()) {
		throw ResourceLoadError("Cannot load font from \"" + filename + "\" - file missing, corrupted or invalid");
	}
}

al::Font al::Font::BuiltinFont()
{
	return Font();
}

int al::Font::getLineHeight() const
{
	return al_get_font_line_height(ptr());
}
int al::Font::getAscent() const
{
	return al_get_font_ascent(ptr());
}
int al::Font::getDescent() const
{
	return al_get_font_descent(ptr());
}

int al::Font::getTextWidth(const std::string& text) const
{
	UStr ustr(text);
	return al_get_ustr_width(ptr(), ustr.ptr());
}
al::Rect<int> al::Font::getTextDimensions(const std::string& text) const
{
	UStr ustr(text);
	int x,y,w,h;
	al_get_ustr_dimensions(ptr(), ustr.ptr(), &x, &y, &w, &h);
	Coord<int> pos{x,y}, size{w,h};
	return {pos, pos+size};
}

int al::Font::getGlyphAdvance(char32_t codepoint1, char32_t codepoint2)
{
	return al_get_glyph_advance(ptr(), codepoint1, codepoint2);
}

size_t al::Font::calcCutoffPoint(std::u32string_view str, int maxWidth)
{
	int pos = 0;
	size_t ret = 0;

	for(size_t i=0; i<str.size(); i++) {
		auto cp1 = str[i];
		auto cp2 = str[i+1];
		if(i == str.size()-1) {
			cp2 = ALLEGRO_NO_KERNING;
		}

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
	auto cpOff = calcCutoffPoint(ToUTF32(str), maxWidth);
	return al_ustr_offset(ustr.ptr(), cpOff);
}

void al::Font::draw(const std::string& text, al::Color color, al::Coord<float> pos) const
{
	draw(text, color, pos, ALLEGRO_ALIGN_LEFT);
}
void al::Font::draw(const std::string& text, al::Color color, al::Coord<float> pos, int align) const
{
	al_draw_text(ptr(), color, pos.x, pos.y, align | ALLEGRO_ALIGN_INTEGER, text.c_str());
}

void al::Font::drawJustified(const std::string& text, al::Color color, al::Coord<float> pos, float xMax, float diffMax) const
{
	al_draw_justified_text(ptr(), color, pos.x, xMax, pos.y, diffMax, ALLEGRO_ALIGN_INTEGER, text.c_str());
}
