#ifndef INCLUDE_AXXEGRO_RESOURCES_FONT
#define INCLUDE_AXXEGRO_RESOURCES_FONT

#include "Resource.hpp"
#include "ResourceHandle.hpp"
#include "../math/math.hpp"
#include "../Color.hpp"
#include "Bitmap.hpp"
#include <string>
#include <vector>
#include <utility>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

namespace al {
	class Font: public Resource {
	public:
		struct CharRange {int begin, end;};

		Font();
		Font(Bitmap& bmp, std::vector<CharRange> ranges);
		Font(const std::string& filename, int size);
		Font(const std::string& filename, int size, int flags);
		~Font();

		ALLEGRO_FONT* alPtr();

		int getLineHeight() const;
		int getAscent() const;
		int getDescent() const;

		int getTextWidth(const std::string& text) const;
		Rect getTextDimensions(const std::string& text) const;

		void draw(const std::string& text, Color color, Point pos) const;
		void draw(const std::string& text, Color color, Point pos, int align) const;

		void drawJustified(const std::string& text, Color color, Point pos, float xMax, float diffMax) const;
	private:
		ALLEGRO_FONT* ptr;
	};

	class FontHandleFile : public ResourceHandle<Font> {
	public:
		FontHandleFile(const std::string& filename, int size);
		virtual Font* loader() override;
	private:
		std::string filename;
		int size;
	};
};

#endif /* INCLUDE_AXXEGRO_RESOURCES_FONT */
