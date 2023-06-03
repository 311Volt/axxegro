//
// Created by volt on 2023-06-03.
//

#ifndef AXXEGRO_FONTADDON_HPP
#define AXXEGRO_FONTADDON_HPP

#include "../../com/Initializable.hpp"
#include <allegro5/allegro_font.h>

namespace al {
	struct FontAddon {
		static constexpr char name[] = "Font addon";
		[[nodiscard]] static bool isInitialized() {return al_is_font_addon_initialized();}
		[[nodiscard]] static bool init() {return al_init_font_addon();}
		using DependsOn = InitDependencies<CoreAllegro>;
	};
}

#endif //AXXEGRO_FONTADDON_HPP
