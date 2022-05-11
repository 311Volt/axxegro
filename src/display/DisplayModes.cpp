#include <axxegro/display/DisplayModes.hpp>

std::vector<al::DisplayMode> al::GetDisplayModes()
{
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