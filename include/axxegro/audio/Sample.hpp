#ifndef INCLUDE_AXXEGRO_RESOURCES_SAMPLE
#define INCLUDE_AXXEGRO_RESOURCES_SAMPLE

#include <axxegro/resources/Resource.hpp>
#include <axxegro/audio/AudioCommon.hpp>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>

#include <optional>

namespace al
{
	AXXEGRO_DEFINE_DELETER(ALLEGRO_SAMPLE, al_destroy_sample);

    struct SampleID: public ALLEGRO_SAMPLE_ID {
        inline SampleID(const ALLEGRO_SAMPLE_ID& id)
        {
            this->_id = id._id;
            this->_index = id._index;
        }

        void stop();
    };

	class Sample: public Resource<ALLEGRO_SAMPLE>, public AddAudioFormatQuery<Sample> {
	public:
		explicit Sample(const std::string& filename);

        std::optional<SampleID> play(PlaybackParams options = {});
        bool save(const std::string& filename);

		[[nodiscard]] ALLEGRO_CHANNEL_CONF getChannelConf() const;
		[[nodiscard]] ALLEGRO_AUDIO_DEPTH getDepth() const;
		[[nodiscard]] unsigned getFrequency() const;

	private:

	};
}

#endif /* INCLUDE_AXXEGRO_RESOURCES_SAMPLE */
