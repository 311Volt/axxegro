#ifndef AA151877_661C_47B3_990A_FD6E29BC6062
#define AA151877_661C_47B3_990A_FD6E29BC6062

#include "../com/Initializable.hpp"
#include "../core/gfx/Bitmap.hpp"
#include <allegro5/allegro_image.h>

namespace al {
	struct ImageAddon {
		static constexpr char name[] = "Image addon";
		[[nodiscard]] static bool isInitialized() {return al_is_image_addon_initialized();}
		[[nodiscard]] static bool init() {return al_init_image_addon();}
		using DependsOn = InitDependencies<CoreAllegro>;
	};

	inline Bitmap LoadBitmap(const std::string& filename) {
		InternalRequire<ImageAddon>();
		if(auto* p = al_load_bitmap(filename.c_str())) {
			return Bitmap(p);
		} else {
			throw ResourceLoadError("Cannot load bitmap from file \"%s\"", filename.c_str());
		}
	}
}

#endif /* AA151877_661C_47B3_990A_FD6E29BC6062 */
