#ifndef INCLUDE_AXXEGRO_INIT
#define INCLUDE_AXXEGRO_INIT

#include <vector>
#include <string>
#include <functional>
#include <stdexcept>

#include <axxegro/Exception.hpp>


#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

namespace al {

	/* TODO some init jobs can be optional
	 * (e.g. al_install_audio will fail when there's no sound device
	 * and you'd want to handle that) */

	/* TODO maybe throw away this whole thing and initialize things
	 * automatically as needed */

	class InitJob {
	public:
		std::string name;
		std::function<bool(void)> function;

		void execute() const {
			if(!function()) {
				throw InitJobFailed("cannot initialize Allegro: "+name+" failed");
			}
		}
	};

#define AXXEGRO_INIT_JOB(x) (al::InitJob{#x, x})

	using InitJobList = std::vector<InitJob>;

	inline void Initialize(InitJobList list) {
		al_init();
		for(auto& job: list) {
			job.execute();
		}
	}

	inline InitJobList FullInitJobList() {
		return {
				AXXEGRO_INIT_JOB(al_install_mouse),
				AXXEGRO_INIT_JOB(al_install_keyboard),
				AXXEGRO_INIT_JOB(al_install_audio),
				AXXEGRO_INIT_JOB(al_init_acodec_addon),
				AXXEGRO_INIT_JOB(al_init_image_addon),
				AXXEGRO_INIT_JOB(al_init_font_addon),
				AXXEGRO_INIT_JOB(al_init_ttf_addon),
				AXXEGRO_INIT_JOB(al_init_native_dialog_addon),
				AXXEGRO_INIT_JOB(al_init_primitives_addon)
		};
	}

	/**
	 * @brief Initialize Allegro and some addons (acodec, image, font, ttf, native_dialog, primitives)
	 * 
	 */
	inline void FullInit() {
		Initialize(FullInitJobList());
	}
}


#endif /* INCLUDE_AXXEGRO_INIT */
