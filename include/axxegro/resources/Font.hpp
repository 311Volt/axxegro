#ifndef INCLUDE_AXXEGRO_RESOURCES_FONT
#define INCLUDE_AXXEGRO_RESOURCES_FONT

#include "Resource.hpp"
#include "../math/math.hpp"
#include "../Color.hpp"
#include "Bitmap.hpp"
#include <string>
#include <string_view>
#include <vector>
#include <utility>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

namespace al {
	class FontDeleter {
	public:
		void operator()(ALLEGRO_FONT* ptr){al_destroy_font(ptr);}
	};

	class Font: public Resource {
	public:
		struct CharRange {
			int begin, end;
		};

		Font();
		Font(Bitmap& bmp, std::vector<CharRange> ranges);
		Font(const std::string& filename, int size);
		Font(const std::string& filename, int size, int flags);

		ALLEGRO_FONT* alPtr();

		int getLineHeight() const;
		int getAscent() const;
		int getDescent() const;

		int getTextWidth(const std::string& text) const;
		Rect getTextDimensions(const std::string& text) const;

		int getGlyphAdvance(char32_t codepoint1, char32_t codepoint2);
		size_t calcCutoffPoint(std::u32string_view str, int maxWidth);
		size_t calcCutoffPoint(std::string_view str, int maxWidth);

		void draw(const std::string& text, Color color, Point pos) const;
		void draw(const std::string& text, Color color, Point pos, int align) const;

		void drawJustified(const std::string& text, Color color, Point pos, float xMax, float diffMax) const;
	private:
		std::unique_ptr<ALLEGRO_FONT, FontDeleter> font;
	};

}

#endif /* INCLUDE_AXXEGRO_RESOURCES_FONT */
