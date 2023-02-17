#ifndef INCLUDE_AXXEGRO_AUDIO_SAMPLEINSTANCE
#define INCLUDE_AXXEGRO_AUDIO_SAMPLEINSTANCE

#include <axxegro/resources/Resource.hpp>
#include <allegro5/allegro_audio.h>

namespace al 
{
    AXXEGRO_DEFINE_DELETER(ALLEGRO_SAMPLE_INSTANCE, al_destroy_sample_instance);

    class SampleInstance: public Resource<ALLEGRO_SAMPLE_INSTANCE> {

    };
}

#endif /* INCLUDE_AXXEGRO_AUDIO_SAMPLEINSTANCE */
