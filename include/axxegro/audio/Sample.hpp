#ifndef INCLUDE_AXXEGRO_RESOURCES_SAMPLE
#define INCLUDE_AXXEGRO_RESOURCES_SAMPLE

#include "axxegro/resources/Resource.hpp"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>

#include <optional>

namespace al
{
	AXXEGRO_DEFINE_DELETER(ALLEGRO_SAMPLE, al_destroy_sample);

    struct SamplePlayParams {
        float gain = 1.0;
        float pan = 0.0;
        float speed = 1.0;
        ALLEGRO_PLAYMODE loop = ALLEGRO_PLAYMODE_ONCE;
    };

    struct SampleID: public ALLEGRO_SAMPLE_ID {
        inline SampleID(const ALLEGRO_SAMPLE_ID& id)
        {
            this->_id = id._id;
            this->_index = id._index;
        }

        void stop();
    };

	class Sample: public Resource<ALLEGRO_SAMPLE> {
	public:
		Sample(const std::string& filename);

        std::optional<SampleID> play(SamplePlayParams options = {}); //TODO return type optional<SampleID>
        bool save(const std::string& filename);


        static void ReserveSamples(int numSamples);
	private:

	};
}

#endif /* INCLUDE_AXXEGRO_RESOURCES_SAMPLE */
