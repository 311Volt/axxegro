#ifndef INCLUDE_AXXEGRO_INIT
#define INCLUDE_AXXEGRO_INIT

#include <vector>
#include <string>
#include <functional>
#include <stdexcept>

namespace al {

	/* TODO some init jobs can be optional
	 * (e.g. al_install_audio will fail when there's no sound device) */

	class InitJob {
	public:
		std::string name;
		std::function<bool(void)> function;

		void execute();
	};

	class InitJobFailed: public std::runtime_error {
	public:
		InitJobFailed(const std::string& msg) 
			: std::runtime_error(msg){}
	};

	using InitJobList = std::vector<InitJob>;

	void Initialize(InitJobList list);
	InitJobList FullInitJobList();

	void FullInit();
}

#define AXXEGRO_INIT_JOB(x) (al::InitJob{#x, x})


#endif /* INCLUDE_AXXEGRO_INIT */
