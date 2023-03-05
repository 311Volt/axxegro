#ifndef INCLUDE_AXXEGRO_AUDIO_SAMPLEINSTANCE
#define INCLUDE_AXXEGRO_AUDIO_SAMPLEINSTANCE

#include <optional>
#include <allegro5/allegro5.h>
#include <axxegro/resources/Resource.hpp>
#include <axxegro/audio/AudioCommon.hpp>
#include <axxegro/audio/Sample.hpp>
#include <axxegro/Exception.hpp>

#include <allegro5/allegro_audio.h>

namespace al 
{
    AXXEGRO_DEFINE_DELETER(ALLEGRO_SAMPLE_INSTANCE, al_destroy_sample_instance);

    class SampleInstance: public Resource<ALLEGRO_SAMPLE_INSTANCE>, public AddPlaybackParams<SampleInstance> {
    public:
        explicit SampleInstance(const Sample& sample)
				: Resource<ALLEGRO_SAMPLE_INSTANCE>(al_create_sample_instance(sample.constPtr())) {
			if(!ptr()) {
				throw ResourceLoadError("Cannot create sample instance");
			}
		}

        bool play() {
			return al_play_sample_instance(ptr());
		}
        bool stop() {
			return al_stop_sample_instance(ptr());
		}

		[[nodiscard]] ALLEGRO_CHANNEL_CONF getChannelConf() const {
			return al_get_sample_instance_channels(ptr());
		}

		[[nodiscard]] ALLEGRO_AUDIO_DEPTH getDepth() const {
			return al_get_sample_instance_depth(ptr());
		}

		[[nodiscard]] unsigned getFrequency() const {
			return al_get_sample_instance_frequency(ptr());
		}

		[[nodiscard]] float getLengthSecs() const {
			return al_get_sample_instance_time(ptr());
		}

		[[nodiscard]] unsigned int getLength() const {
			return al_get_sample_instance_length(ptr());
		}
        bool setLength(unsigned numSamples) {
			return al_set_sample_instance_length(ptr(), numSamples);
		}

		[[nodiscard]] unsigned getPosition() const {
			return al_get_sample_instance_position(ptr());
		}
        bool setPosition(unsigned val) {
			return al_set_sample_instance_position(ptr(), val);
		}

		[[nodiscard]] float getSpeed() const {
			return al_get_sample_instance_speed(ptr());
		}
        bool setSpeed(float val) {
			return al_set_sample_instance_speed(ptr(), val);
		}

		[[nodiscard]] float getGain() const {
			return al_get_sample_instance_gain(ptr());
		}
        bool setGain(float val) {
			return al_set_sample_instance_gain(ptr(), val);
		}

		[[nodiscard]] float getPan() const {
			return al_get_sample_instance_pan(ptr());
		}
        bool setPan(float val) {
			return al_set_sample_instance_pan(ptr(), val);
		}

		[[nodiscard]] ALLEGRO_PLAYMODE getPlayMode() const {
			return al_get_sample_instance_playmode(ptr());
		}
        bool setPlayMode(ALLEGRO_PLAYMODE val) {
			return al_set_sample_instance_playmode(ptr(), val);
		}

		[[nodiscard]] bool getPlaying() const {
			return al_get_sample_instance_playing(ptr());
		}
        bool setPlaying(bool val) {
			return al_set_sample_instance_playing(ptr(), val);
		}

		[[nodiscard]] bool isAttached() const {
			return al_get_sample_instance_attached(ptr());
		}
        bool detach() {
			return al_detach_sample_instance(ptr());
		}

        //Sample& getSample(); //TODO implement
        bool setSample(Sample& sample) {
			return al_set_sample(ptr(), sample.ptr());
		}

        //setChannelMatrix() //TODO

		static inline void ReserveSamples(int numSamples) {
			if(!al_reserve_samples(numSamples)) {
				throw AudioError("Cannot reserve {} samples", numSamples);
			}
		}
	};
}

#endif /* INCLUDE_AXXEGRO_AUDIO_SAMPLEINSTANCE */
