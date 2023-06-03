//
// Created by volt on 2023-06-03.
//

#ifndef AXXEGRO_PRIMITIVESADDON_HPP
#define AXXEGRO_PRIMITIVESADDON_HPP

#include <allegro5/allegro_primitives.h>
#include "../../com/Initializable.hpp"


#ifndef NDEBUG
#define AXX_CHECK_PRIM_IF_DEBUG \
	do{ \
		if(not ::al::PrimitivesAddon::isInitialized()) \
        	throw AddonError("al::Require<al::PrimitivesAddon>(); needs to be called before using any drawing routines from the primitives addon."); \
		} while(0);
#else
#define AXX_CHECK_PRIM_IF_DEBUG ;
#endif

namespace al {
	struct PrimitivesAddon {
		static constexpr char name[] = "Primitives addon";
		[[nodiscard]] static bool isInitialized() {return al_is_primitives_addon_initialized();}
		[[nodiscard]] static bool init() {return al_init_primitives_addon();}
		using DependsOn = InitDependencies<CoreAllegro>;
	};
}

#endif //AXXEGRO_PRIMITIVESADDON_HPP
