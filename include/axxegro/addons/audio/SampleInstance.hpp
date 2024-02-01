#ifndef INCLUDE_AXXEGRO_AUDIO_SAMPLEINSTANCE
#define INCLUDE_AXXEGRO_AUDIO_SAMPLEINSTANCE

#include "AudioCommon.hpp"
#include "Sample.hpp"

#include <optional>

namespace al 
{
    AXXEGRO_DEFINE_DELETER(ALLEGRO_SAMPLE_INSTANCE, al_destroy_sample_instance);

    class SampleInstance:
			RequiresInitializables<AudioAddon>,
			public Resource<ALLEGRO_SAMPLE_INSTANCE>,
			public al::detail::AddPlaybackParamsQuery<SampleInstance> {
    public:
        explicit SampleInstance(const Sample& sample)
				: Resource<ALLEGRO_SAMPLE_INSTANCE>(al_create_sample_instance(sample.ptr())) {
			if(!ptr()) {
				throw ResourceLoadError("Cannot create sample instance");
			}
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_play_sample_instance
        bool play() {
			return al_play_sample_instance(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_stop_sample_instance
        bool stop() {
			return al_stop_sample_instance(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_get_sample_instance_channels
		[[nodiscard]] ALLEGRO_CHANNEL_CONF getChannelConf() const {
			return al_get_sample_instance_channels(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_get_sample_instance_depth
		[[nodiscard]] ALLEGRO_AUDIO_DEPTH getDepth() const {
			return al_get_sample_instance_depth(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_get_sample_instance_frequency
		[[nodiscard]] unsigned getFrequency() const {
			return al_get_sample_instance_frequency(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_get_sample_instance_time
		[[nodiscard]] float getLengthSecs() const {
			return al_get_sample_instance_time(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_get_sample_instance_length
		[[nodiscard]] unsigned int getLength() const {
			return al_get_sample_instance_length(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_set_sample_instance_length
        bool setLength(unsigned numSamples) {
			return al_set_sample_instance_length(ptr(), numSamples);
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_get_sample_instance_position
		[[nodiscard]] unsigned getPosition() const {
			return al_get_sample_instance_position(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_set_sample_instance_position
        bool setPosition(unsigned val) {
			return al_set_sample_instance_position(ptr(), val);
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_get_sample_instance_speed
		[[nodiscard]] float getSpeed() const {
			return al_get_sample_instance_speed(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_set_sample_instance_speed
        bool setSpeed(float val) {
			return al_set_sample_instance_speed(ptr(), val);
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_get_sample_instance_gain
		[[nodiscard]] float getGain() const {
			return al_get_sample_instance_gain(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_set_sample_instance_gain
        bool setGain(float val) {
			return al_set_sample_instance_gain(ptr(), val);
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_get_sample_instance_pan
		[[nodiscard]] float getPan() const {
			return al_get_sample_instance_pan(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_set_sample_instance_pan
        bool setPan(float val) {
			return al_set_sample_instance_pan(ptr(), val);
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_get_sample_instance_playmode
		[[nodiscard]] ALLEGRO_PLAYMODE getPlayMode() const {
			return al_get_sample_instance_playmode(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_set_sample_instance_playmode
        bool setPlayMode(ALLEGRO_PLAYMODE val) {
			return al_set_sample_instance_playmode(ptr(), val);
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_get_sample_instance_playing
		[[nodiscard]] bool getPlaying() const {
			return al_get_sample_instance_playing(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_set_sample_instance_playing
        bool setPlaying(bool val) {
			return al_set_sample_instance_playing(ptr(), val);
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_get_sample_instance_attached
		[[nodiscard]] bool isAttached() const {
			return al_get_sample_instance_attached(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_detach_sample_instance
        bool detach() {
			return al_detach_sample_instance(ptr());
		}

        //Sample& getSample(); //TODO implement

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_set_sample
        bool setSample(Sample& sample) {
			return al_set_sample(ptr(), sample.ptr());
		}

		/// @see Sets the position to 0 seconds.
		bool rewind() {
			return setPosition(0);
		}

		/// @brief Sets the position to 0 and plays the sample instance
		bool rewindAndPlay() {
			return (rewind() && setPlaying(true));
		}

        //setChannelMatrix() //TODO

	};

	inline void ReserveSamples(int numSamples) {
		if(!al_reserve_samples(numSamples)) {
			throw AudioError("Cannot reserve %d samples", numSamples);
		}
	}
}

#endif /* INCLUDE_AXXEGRO_AUDIO_SAMPLEINSTANCE */
