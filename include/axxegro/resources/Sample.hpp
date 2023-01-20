#ifndef INCLUDE_AXXEGRO_RESOURCES_SAMPLE
#define INCLUDE_AXXEGRO_RESOURCES_SAMPLE

#include "axxegro/resources/Resource.hpp"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>

namespace al
{
	AXXEGRO_DEFINE_DELETER(ALLEGRO_SAMPLE, al_destroy_sample);

	class Sample: public Resource<ALLEGRO_SAMPLE> {
	public:
		Sample(const std::string& filename);
		~Sample();
	private:

	};
}

#endif /* INCLUDE_AXXEGRO_RESOURCES_SAMPLE */
