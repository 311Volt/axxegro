#ifndef INCLUDE_AXXEGRO_RESOURCES_SAMPLE
#define INCLUDE_AXXEGRO_RESOURCES_SAMPLE

#include <axxegro/resources/Resource.hpp>
#include <axxegro/audio/AudioCommon.hpp>
#include <axxegro/Exception.hpp>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>

#include <optional>

namespace al
{
	AXXEGRO_DEFINE_DELETER(ALLEGRO_SAMPLE, al_destroy_sample);

    struct SampleID: public ALLEGRO_SAMPLE_ID {
        explicit SampleID(const ALLEGRO_SAMPLE_ID& id) {
            this->_id = id._id;
            this->_index = id._index;
        }

        void stop() {
			al_stop_sample(this);
		}
    };

	class Sample: public Resource<ALLEGRO_SAMPLE>, public AddAudioFormatQuery<Sample> {
	public:
		explicit Sample(const std::string& filename)
		   : Resource<ALLEGRO_SAMPLE>(al_load_sample(filename.c_str()))
		{
			if(!ptr()) {
				throw ResourceLoadError("Cannot load audio file: {}", filename);
			}
		}

        std::optional<SampleID> play(PlaybackParams options = {}) {
			ALLEGRO_SAMPLE_ID smpId;
			bool status = al_play_sample(ptr(), options.gain, options.pan, options.speed, options.loop, &smpId);
			if(!status) {
				return std::nullopt;
			}
			return SampleID(smpId);
		}

        bool save(const std::string& filename) {
			return al_save_sample(filename.c_str(), ptr());
		}

		[[nodiscard]] ALLEGRO_CHANNEL_CONF getChannelConf() const {
			return al_get_sample_channels(ptr());
		}
		[[nodiscard]] ALLEGRO_AUDIO_DEPTH getDepth() const {
			return al_get_sample_depth(ptr());
		}
		[[nodiscard]] unsigned getFrequency() const {
			return al_get_sample_frequency(ptr());
		}

	private:

	};
}

#endif /* INCLUDE_AXXEGRO_RESOURCES_SAMPLE */
