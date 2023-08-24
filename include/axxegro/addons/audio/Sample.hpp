#ifndef INCLUDE_AXXEGRO_RESOURCES_SAMPLE
#define INCLUDE_AXXEGRO_RESOURCES_SAMPLE

#include "AudioCommon.hpp"

#include <optional>

namespace al
{
	AXXEGRO_DEFINE_DELETER(ALLEGRO_SAMPLE, al_destroy_sample);

	/**
	 * @brief Identifies the internal sample instance used by Allegro to implement {@link Sample::play()}.
	 * Can be used to stop the sample.
	 */
    struct SampleID: public ALLEGRO_SAMPLE_ID {
        explicit SampleID(const ALLEGRO_SAMPLE_ID& id) {
            this->_id = id._id;
            this->_index = id._index;
        }

        void stop() {
			al_stop_sample(this);
		}
    };

	/**
	 * @brief An audio sample (e.g. loaded from an audio file)
	 */
	class Sample:
			RequiresInitializables<AudioAddon>,
			public Resource<ALLEGRO_SAMPLE>,
			public al::detail::AddAudioFormatQuery<Sample> {
	public:
		using Resource::Resource;

		/**
		 * @brief Plays the sample. Call {@link ReserveSamples} before making calls to this method.
		 * @param options Playback parameters (gain, pan, speed)
		 * @return The instance's ID or std::nullopt if the sample cannot be played.
		 *
		 * Note that std::optional has operator bool() so if you just want to check if the sample
		 * will be played, you can do it like this: {@code if(sample.play()) { \/\* ... \*\/ }}
		 *
		 * @see <a href="https://liballeg.org/a5docs/trunk/audio.html#al_play_sample">Allegro: al_play_sample</a>
		 */
        std::optional<SampleID> play(PlaybackParams options = {}) {
			ALLEGRO_SAMPLE_ID smpId;
			bool status = al_play_sample(ptr(), options.gain, options.pan, options.speed, options.loop, &smpId);
			if(!status) {
				return std::nullopt;
			}
			return SampleID(smpId);
		}

		/**
		 * @brief Save the sample to a file.
		 * @return Whether the operation was successful.
		 *
		 * @see <a href="https://liballeg.org/a5docs/trunk/audio.html#al_save_sample">Allegro: al_save_sample</a>
		 */
        bool save(const std::string& filename) {
			return al_save_sample(filename.c_str(), ptr());
		}

		/// @see <a href="https://liballeg.org/a5docs/trunk/audio.html#al_get_sample_channels">Allegro: al_get_sample_channels</a>
		[[nodiscard]] ALLEGRO_CHANNEL_CONF getChannelConf() const {
			return al_get_sample_channels(ptr());
		}

		/// @see <a href="https://liballeg.org/a5docs/trunk/audio.html#al_get_sample_depth">Allegro: al_get_sample_depth</a>
		[[nodiscard]] ALLEGRO_AUDIO_DEPTH getDepth() const {
			return al_get_sample_depth(ptr());
		}

		/// @see <a href="https://liballeg.org/a5docs/trunk/audio.html#al_get_sample_frequency">Allegro: al_get_sample_frequency</a>
		[[nodiscard]] unsigned getFrequency() const {
			return al_get_sample_frequency(ptr());
		}

	private:

	};
}

#endif /* INCLUDE_AXXEGRO_RESOURCES_SAMPLE */
