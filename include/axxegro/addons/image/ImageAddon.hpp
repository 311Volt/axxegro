//
// Created by volt on 2023-06-03.
//

#ifndef AXXEGRO_IMAGEADDON_HPP
#define AXXEGRO_IMAGEADDON_HPP

#include <allegro5/allegro_image.h>
#include "../../com/Initializable.hpp"

namespace al {
	struct ImageAddon {
		static constexpr char name[] = "Image addon";
		[[nodiscard]] static bool isInitialized() {return al_is_image_addon_initialized();}
		[[nodiscard]] static bool init() {return al_init_image_addon();}
		using DependsOn = InitDependencies<CoreAllegro>;
	};
}

#endif //AXXEGRO_IMAGEADDON_HPP
