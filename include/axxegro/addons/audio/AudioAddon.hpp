//
// Created by volt on 2023-06-03.
//

#ifndef AXXEGRO_AUDIOADDON_HPP
#define AXXEGRO_AUDIOADDON_HPP

#include <allegro5/allegro_audio.h>

#include "../../com/Initializable.hpp"

namespace al {

	struct AudioAddon {
		static constexpr char name[] = "Audio addon";
		[[nodiscard]] static bool isInitialized() {return al_is_audio_installed();}
		[[nodiscard]] static bool init() {return al_install_audio();}
		using DependsOn = InitDependencies<CoreAllegro>;
	};

	struct AudioCodecAddon {
		static constexpr char name[] = "Audio codec addon";
		[[nodiscard]] static bool isInitialized() {return al_is_acodec_addon_initialized();}
		[[nodiscard]] static bool init() {return al_init_acodec_addon();}
		using DependsOn = InitDependencies<AudioAddon>;
	};

}

#endif //AXXEGRO_AUDIOADDON_HPP
