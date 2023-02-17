#ifndef INCLUDE_AXXEGRO_AUDIO_SAMPLEINSTANCE
#define INCLUDE_AXXEGRO_AUDIO_SAMPLEINSTANCE

#include <optional>
#include <allegro5/allegro5.h>
#include <axxegro/resources/Resource.hpp>
#include <axxegro/audio/AudioCommon.hpp>
#include <axxegro/audio/Sample.hpp>

#include <allegro5/allegro_audio.h>

namespace al 
{
    AXXEGRO_DEFINE_DELETER(ALLEGRO_SAMPLE_INSTANCE, al_destroy_sample_instance);

    class SampleInstance: public Resource<ALLEGRO_SAMPLE_INSTANCE>, public AddPlaybackParams<SampleInstance> {
    public:
        explicit SampleInstance(const Sample& sample);

        bool play();
        bool stop();

		[[nodiscard]] ALLEGRO_CHANNEL_CONF getChannelConf() const;
		[[nodiscard]] ALLEGRO_AUDIO_DEPTH getDepth() const;
		[[nodiscard]] unsigned getFrequency() const;

		[[nodiscard]] float getLengthSecs() const;

		[[nodiscard]] unsigned int getLength() const;
        bool setLength(unsigned numSamples);

		[[nodiscard]] unsigned getPosition() const;
        bool setPosition(unsigned val);

		[[nodiscard]] float getSpeed() const;
        bool setSpeed(float val);

		[[nodiscard]] float getGain() const;
        bool setGain(float val);

		[[nodiscard]] float getPan() const;
        bool setPan(float val);

		[[nodiscard]] ALLEGRO_PLAYMODE getPlayMode() const;
        bool setPlayMode(ALLEGRO_PLAYMODE val);

		[[nodiscard]] bool getPlaying() const;
        bool setPlaying(bool val);

		[[nodiscard]] bool isAttached() const;
        bool detach();

        //Sample& getSample(); //TODO implement
        bool setSample(Sample& sample);

        //setChannelMatrix() //TODO

		static void ReserveSamples(int numSamples);
	};
}

#endif /* INCLUDE_AXXEGRO_AUDIO_SAMPLEINSTANCE */
