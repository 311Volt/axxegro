#ifndef AXXEGRO_ADDONS_HPP
#define AXXEGRO_ADDONS_HPP

#include "common.hpp"

#include "addons/audio.hpp"
#include "addons/font.hpp"
#include "addons/prim.hpp"
#include "addons/dialog.hpp"
#include "addons/video.hpp"
#include "addons/color.hpp"
#include "addons/video.hpp"


namespace al {
	inline void RequireAllAddons() {
		Require<CoreAllegro,
				KeyboardDriver,
				MouseDriver,
				ImageAddon,
				AudioAddon,
				AudioCodecAddon,
				FontAddon,
				TTFAddon,
				PrimitivesAddon,
				NativeDialogAddon,
				VideoAddon
		>();
	}
}



#endif //AXXEGRO_ADDONS_HPP
