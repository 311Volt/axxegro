#ifndef INCLUDE_AXXEGRO_INIT
#define INCLUDE_AXXEGRO_INIT

#include <vector>
#include <string>
#include <functional>
#include <stdexcept>

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

		void execute();
	};

	using InitJobList = std::vector<InitJob>;

	void Initialize(InitJobList list);
	InitJobList FullInitJobList();

	/**
	 * @brief Initialize Allegro and some addons (acodec, image, font, ttf, native_dialog, primitives)
	 * 
	 */
	void FullInit();
}

#define AXXEGRO_INIT_JOB(x) (al::InitJob{#x, x})


#endif /* INCLUDE_AXXEGRO_INIT */
