#ifndef INCLUDE_AXXEGRO_DISPLAY_DISPLAY
#define INCLUDE_AXXEGRO_DISPLAY_DISPLAY

#include "../math/math.hpp"
#include <string>
#include <vector>

#include <allegro5/allegro.h>

namespace al {
	class Display {
	public:
		using Option = struct{int option, value;};

		Display(int w, int h);
		Display(int w, int h, int flags);
		Display(int w, int h, int flags, std::vector<Option> requiredOptions, std::vector<Option> suggestedOptions);
		~Display();

		int width() const;
		int height() const;

		int getRefreshRate() const;
		Vec2 getSize() const;

		int getOptionValue(int option) const;
		bool setFlag(int flag, bool onoff);

		bool resize(int w, int h);
		bool acknowledgeResize();
		Point getPosition() const;
		void setPosition(Point pos);

		int getFlags() const;

		std::string getClipboardText() const;
		bool setClipboardText(const std::string& text);

		void setTitle(const std::string& title);

		ALLEGRO_DISPLAY* alPtr();
	private:
		ALLEGRO_DISPLAY* disp;
	};
}

#endif /* INCLUDE_AXXEGRO_DISPLAY_DISPLAY */
