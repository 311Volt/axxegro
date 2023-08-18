#ifndef INCLUDE_AXXEGRO_RESOURCES_SAMPLE
#define INCLUDE_AXXEGRO_RESOURCES_SAMPLE

#include "AudioCommon.hpp"

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

	class Sample:
			RequiresInitializables<AudioAddon>,
			public Resource<ALLEGRO_SAMPLE>,
			public al::detail::AddAudioFormatQuery<Sample> {
	public:
		using Resource::Resource;

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
