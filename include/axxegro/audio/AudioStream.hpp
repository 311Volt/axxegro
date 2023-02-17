#ifndef INCLUDE_AXXEGRO_AUDIO_AUDIOSTREAM
#define INCLUDE_AXXEGRO_AUDIO_AUDIOSTREAM

#include <axxegro/resources/Resource.hpp>
#include <axxegro/audio/AudioCommon.hpp>
#include <axxegro/event/EventSource.hpp>
#include <allegro5/allegro_audio.h>

#include <span>

namespace al {

    AXXEGRO_DEFINE_DELETER(ALLEGRO_AUDIO_STREAM, al_destroy_audio_stream);


	class AudioStream;

	struct AudioStreamEventSource: public EventSource {
	public:
		explicit AudioStreamEventSource(AudioStream& stream);
		[[nodiscard]] ALLEGRO_EVENT_SOURCE* ptr() const override;
	private:
		AudioStream& stream;
	};

	class AudioStream:
			public Resource<ALLEGRO_AUDIO_STREAM>,
			public AddPlaybackParams<AudioStream>,
			public AddAudioFormatQuery<AudioStream> {
    public:
		explicit AudioStream(size_t bufferCount = 4, unsigned samples = 2048, AudioFormat format = {});
		explicit AudioStream(const std::string& filename, size_t bufferCount = 4, unsigned samples = 4096);

//		static AudioStream& Play(const std::string& filename); //5.2.8 unstable

        void drain();
        bool rewind();
        [[nodiscard]] unsigned getFrequency() const;
        [[nodiscard]] ALLEGRO_AUDIO_DEPTH getDepth() const;
        [[nodiscard]] ALLEGRO_CHANNEL_CONF getChannelConf() const;
        [[nodiscard]] unsigned getLength() const;

		[[nodiscard]] float getSpeed() const;
        bool setSpeed(float val);

		[[nodiscard]] float getGain() const;
        bool setGain(float val);

		[[nodiscard]] float getPan() const;
        bool setPan(float val);

		[[nodiscard]] bool getPlaying() const;
        bool setPlaying(bool val);

		[[nodiscard]] ALLEGRO_PLAYMODE getPlayMode() const;
        bool setPlayMode(ALLEGRO_PLAYMODE val);

		[[nodiscard]] bool isAttached() const;
        bool detach();

		[[nodiscard]] uint64_t getNumPlayedSamples() const;

		EventSource& getEventSource();

		bool seekSecs(double time);
		[[nodiscard]] double getPositionSecs() const;
		[[nodiscard]] double getLengthSecs() const;

		[[nodiscard]] unsigned getNumFragments() const;
		[[nodiscard]] unsigned getNumAvailableFragments() const;

		void* getFragmentData();
		bool setFragmentData(void* data);

		template<ALLEGRO_CHANNEL_CONF ChanConf, ALLEGRO_AUDIO_DEPTH AudioDepth>
		bool setFragment(void (*filler)(std::span<typename SampleType<ChanConf, AudioDepth>::Type>))
		{
			using SmpType = typename SampleType<ChanConf, AudioDepth>::Type;
			void* fragData = getFragmentData();
			if(getFragmentData()) {
				std::span<SmpType> typedFragData {static_cast<SmpType*>(fragData), getLength()};
				filler(typedFragData);
				setFragmentData(fragData);
				return true;
			}

			return false;
		}

		//TODO setChannelMatrix

	private:
		using Resource::Resource;

//		static std::unique_ptr<AudioStream> defaultAudioStream;
		std::unique_ptr<AudioStreamEventSource> evSource;
    };

}

#endif /* INCLUDE_AXXEGRO_AUDIO_AUDIOSTREAM */
