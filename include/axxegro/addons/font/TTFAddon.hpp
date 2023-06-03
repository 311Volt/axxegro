//
// Created by volt on 2023-06-03.
//

#ifndef AXXEGRO_TTFADDON_HPP
#define AXXEGRO_TTFADDON_HPP

#include "../../com/Initializable.hpp"
#include "FontAddon.hpp"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

namespace al {
	struct TTFAddon {
		static constexpr char name[] = "TTF addon";
		[[nodiscard]] static bool isInitialized() {return al_is_ttf_addon_initialized();}
		[[nodiscard]] static bool init() {return al_init_ttf_addon();}
		using DependsOn = InitDependencies<FontAddon>;
	};
}

#endif //AXXEGRO_TTFADDON_HPP
