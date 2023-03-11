#ifndef INCLUDE_AXXEGRO_DISPLAY_DISPLAYMODES
#define INCLUDE_AXXEGRO_DISPLAY_DISPLAYMODES

#include <allegro5/allegro.h>
#include <vector>

namespace al {
	using DisplayMode = ALLEGRO_DISPLAY_MODE;

	inline std::vector<DisplayMode> GetDisplayModes() {
		std::vector<al::DisplayMode> ret;
		int num = al_get_num_display_modes();
		ret.reserve(num);

		for(int i=0; i<num; i++) {
			ALLEGRO_DISPLAY_MODE m;
			if(al_get_display_mode(i, &m)) {
				ret.push_back(m);
			}
		}

		return ret;
	}
}

#endif /* INCLUDE_AXXEGRO_DISPLAY_DISPLAYMODES */
