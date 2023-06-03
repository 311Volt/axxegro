//
// Created by volt on 2023-06-03.
//

#ifndef AXXEGRO_NATIVEDIALOGADDON_HPP
#define AXXEGRO_NATIVEDIALOGADDON_HPP

#include "../../com/Initializable.hpp"
#include <allegro5/allegro_native_dialog.h>

namespace al {

	struct NativeDialogAddon {
		static constexpr char name[] = "Native dialog addon";
		[[nodiscard]] static bool isInitialized() {return al_is_native_dialog_addon_initialized();}
		[[nodiscard]] static bool init() {return al_init_native_dialog_addon();}
		using DependsOn = InitDependencies<CoreAllegro>;
	};

}

#endif //AXXEGRO_NATIVEDIALOGADDON_HPP
