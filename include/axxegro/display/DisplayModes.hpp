#ifndef INCLUDE_AXXEGRO_DISPLAY_DISPLAYMODES
#define INCLUDE_AXXEGRO_DISPLAY_DISPLAYMODES

#include <allegro5/allegro.h>
#include <vector>

namespace al {
	using DisplayMode = ALLEGRO_DISPLAY_MODE;

	std::vector<DisplayMode> GetDisplayModes();
}

#endif /* INCLUDE_AXXEGRO_DISPLAY_DISPLAYMODES */
