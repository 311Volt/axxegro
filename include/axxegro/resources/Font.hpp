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
	AXXEGRO_DEFINE_DELETER(ALLEGRO_FONT, al_destroy_font);

	class Font: public Resource<ALLEGRO_FONT> {
		Font();
	public:
		using Resource::Resource;
		struct CharRange {
			int begin, end;
		};

		static Font BuiltinFont();
		
		Font(Bitmap& bmp, std::vector<CharRange> ranges);
		Font(const std::string& filename, int size);
		Font(const std::string& filename, int size, int flags);

		int getLineHeight() const;
		int getAscent() const;
		int getDescent() const;

		int getTextWidth(const std::string& text) const;
		Rect<int> getTextDimensions(const std::string& text) const;

		int getGlyphAdvance(char32_t codepoint1, char32_t codepoint2);
		size_t calcCutoffPoint(std::u32string_view str, int maxWidth);
		size_t calcCutoffPoint(std::string_view str, int maxWidth);

		void draw(const std::string& text, Color color, Coord<float> pos) const;
		void draw(const std::string& text, Color color, Coord<float> pos, int align) const;

		void drawJustified(const std::string& text, Color color, Coord<float> pos, float xMax, float diffMax) const;
	private:
	};

}

#endif /* INCLUDE_AXXEGRO_RESOURCES_FONT */
