//
// Created by volt on 2023-06-03.
//

#ifndef AXXEGRO_VIDEOADDON_HPP
#define AXXEGRO_VIDEOADDON_HPP

#include <allegro5/allegro_video.h>
#include "../../com/Initializable.hpp"
#include "../audio/AudioAddon.hpp"

namespace al {
	struct VideoAddon {
		static constexpr char name[] = "Video addon";
		[[nodiscard]] static bool isInitialized() {return al_is_video_addon_initialized();}
		[[nodiscard]] static bool init() {return al_init_video_addon();}
		using DependsOn = InitDependencies<AudioAddon>;
	};
}

#endif //AXXEGRO_VIDEOADDON_HPP
