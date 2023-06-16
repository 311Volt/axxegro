#ifndef INCLUDE_AXXEGRO_ADDONS_FONT_FONT
#define INCLUDE_AXXEGRO_ADDONS_FONT_FONT

#include "FontAddon.hpp"
#include "TTFAddon.hpp"
#include "../../core.hpp"

#include <allegro5/allegro_font.h>

namespace al {
	AXXEGRO_DEFINE_DELETER(ALLEGRO_FONT, al_destroy_font);

	class Font:
			RequiresInitializables<FontAddon>,
			public Resource<ALLEGRO_FONT> {
		Font()
				: Resource(al_create_builtin_font())
		{

		}
	public:
		using Resource::Resource;
		struct CharRange {
			int begin, end;
		};

		[[nodiscard]] static Font CreateBuiltinFont() {
			return {};
		}

		[[nodiscard]] static Font& BuiltinFont() {
			static Font builtinFont;
			return builtinFont;
		}
		
		Font(Bitmap& bmp, const std::vector<CharRange>& ranges)
				: Resource(nullptr)
		{
			std::vector<int> rangesFlat;
			rangesFlat.reserve(ranges.size()*2);
			for(const auto& range: ranges) {
				rangesFlat.push_back(range.begin);
				rangesFlat.push_back(range.end);
			}

			setPtr(al_grab_font_from_bitmap(bmp.ptr(), (int)ranges.size(), rangesFlat.data()));
			if(!ptr()) {
				throw ResourceLoadError("Error while grabbing font from a %dx%d bitmap", bmp.width(), bmp.height());
			}
		}

		Font(const std::string& filename, int size, int flags = 0)
				: Resource(nullptr)
		{
			InternalRequire<TTFAddon>();
			if(auto* p = al_load_font(filename.c_str(), size, flags)) {
				setPtr(p);
			} else {
				throw ResourceLoadError("Cannot load font from %s - file missing, corrupted or invalid", filename.c_str());
			}
		}

		[[nodiscard]] int getLineHeight() const {
			return al_get_font_line_height(ptr());
		}
		[[nodiscard]] int getAscent() const {
			return al_get_font_ascent(ptr());
		}
		[[nodiscard]] int getDescent() const {
			return al_get_font_descent(ptr());
		}

		[[nodiscard]] int getTextWidth(const std::string& text) const {
			UStr ustr(text);
			return al_get_ustr_width(ptr(), ustr.ptr());
		}
		[[nodiscard]] Rect<int> getTextDimensions(const std::string& text) const {
			UStr ustr(text);
			int x,y,w,h;
			al_get_ustr_dimensions(ptr(), ustr.ptr(), &x, &y, &w, &h);
			Vec2<int> pos{x,y}, size{w,h};
			return {pos, pos+size};
		}

		int getGlyphAdvance(char32_t codepoint1, char32_t codepoint2) {
			return al_get_glyph_advance(ptr(), (int)codepoint1, (int)codepoint2);
		}
		size_t calcCutoffPoint(std::u32string_view str, int maxWidth) {
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
		size_t calcCutoffPoint(std::string_view str, int maxWidth) {
			UStr ustr(str);
			auto cpOff = calcCutoffPoint(ToUTF32(str), maxWidth);
			return al_ustr_offset(ustr.ptr(), (int)cpOff);
		}

		//TODO fastDraw()
		void draw(const std::string& text, Color color, Vec2<int> pos, int align = ALLEGRO_ALIGN_LEFT, bool alignInteger = true) const {
			int actualAlign = align | (alignInteger * ALLEGRO_ALIGN_INTEGER);
			al_draw_text(ptr(), color, pos.x, pos.y, actualAlign, text.c_str());
		}

		void drawJustified(const std::string& text, Color color, Vec2<int> pos, float xMax, float diffMax, bool alignInteger = true) const {
			al_draw_justified_text(ptr(), color, pos.x, xMax, pos.y, diffMax, alignInteger * ALLEGRO_ALIGN_INTEGER, text.c_str());
		}
	private:
	};

}

#endif /* INCLUDE_AXXEGRO_ADDONS_FONT_FONT */
