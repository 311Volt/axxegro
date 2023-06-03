#ifndef INCLUDE_AXXEGRO_AUDIO_AUDIOSTREAM
#define INCLUDE_AXXEGRO_AUDIO_AUDIOSTREAM

#include "AudioCommon.hpp"

namespace al {

	class AudioStream;
	struct AudioStreamEventSource: public EventSource {
	public:
		explicit AudioStreamEventSource(AudioStream& stream) : stream(stream) {}
		[[nodiscard]] ALLEGRO_EVENT_SOURCE* ptr() const override;
	private:
		AudioStream& stream;
	};

    AXXEGRO_DEFINE_DELETER(ALLEGRO_AUDIO_STREAM, al_destroy_audio_stream);

	class AudioStream:
			RequiresInitializables<AudioAddon>,
			public Resource<ALLEGRO_AUDIO_STREAM>,
			public AddPlaybackParams<AudioStream>,
			public AddAudioFormatQuery<AudioStream> {
    public:

		explicit AudioStream(size_t bufferCount = 4, unsigned samples = 2048, AudioFormat format = {})
				: Resource<ALLEGRO_AUDIO_STREAM>(nullptr),
				  evSource(new AudioStreamEventSource(*this))
		{
			if(auto* p = al_create_audio_stream(bufferCount, samples, format.frequency, format.depth, format.chanConf)) {
				setPtr(p);
			} else {
				throw AudioError("Cannot create audio stream with %d x %d bufs and format %s", (int)bufferCount, (int)samples, format.str().c_str());
			}
		}

		explicit AudioStream(const std::string& filename, size_t bufferCount = 4, unsigned samples = 4096)
				: Resource<ALLEGRO_AUDIO_STREAM>(nullptr),
				  evSource(new AudioStreamEventSource(*this))
		{
			Require<AudioCodecAddon>();
			if(auto* p = al_load_audio_stream(filename.c_str(), bufferCount, samples)) {
				setPtr(p);
			} else {
				throw AudioError("Cannot load audio stream from file %s", filename.c_str());
			}
		}

//		static AudioStream& Play(const std::string& filename); //5.2.8 unstable

        void drain() {
			al_drain_audio_stream(ptr());
		}

        bool rewind() {
			return al_rewind_audio_stream(ptr());
		}

        [[nodiscard]] unsigned getFrequency() const {
			return al_get_audio_stream_frequency(ptr());
		}
        [[nodiscard]] ALLEGRO_AUDIO_DEPTH getDepth() const {
			return al_get_audio_stream_depth(ptr());
		}
        [[nodiscard]] ALLEGRO_CHANNEL_CONF getChannelConf() const {
			return al_get_audio_stream_channels(ptr());
		}
        [[nodiscard]] unsigned getLength() const {
			return al_get_audio_stream_length(ptr());
		}

		[[nodiscard]] float getSpeed() const {
			return al_get_audio_stream_speed(ptr());
		}
        bool setSpeed(float val) {
			return al_set_audio_stream_speed(ptr(), val);
		}

		[[nodiscard]] float getGain() const {
			return al_get_audio_stream_gain(ptr());
		}

        bool setGain(float val) {
			return al_set_audio_stream_gain(ptr(), val);
		}

		[[nodiscard]] float getPan() const {
			return al_get_audio_stream_pan(ptr());
		}
        bool setPan(float val) {
			return al_set_audio_stream_pan(ptr(), val);
		}

		[[nodiscard]] bool getPlaying() const {
			return al_get_audio_stream_playing(ptr());
		}
        bool setPlaying(bool val) {
			return al_set_audio_stream_playing(ptr(), val);
		}
		bool play() {
			return setPlaying(true);
		}
		bool stop() {
			return setPlaying(false);
		}

		[[nodiscard]] ALLEGRO_PLAYMODE getPlayMode() const {
			return al_get_audio_stream_playmode(ptr());
		}
        bool setPlayMode(ALLEGRO_PLAYMODE val) {
			return al_set_audio_stream_playmode(ptr(), val);
		}

		[[nodiscard]] bool isAttached() const {
			return al_get_audio_stream_attached(ptr());
		}
        bool detach() {
			return al_detach_audio_stream(ptr());
		}

		[[nodiscard]] uint64_t getNumPlayedSamples() const {
			return al_get_audio_stream_played_samples(ptr());
		}

		EventSource& getEventSource() {
			return *evSource;
		}

		bool seekSecs(double time) {
			return al_seek_audio_stream_secs(ptr(), time);
		}
		[[nodiscard]] double getPositionSecs() const {
			return al_get_audio_stream_position_secs(ptr());
		}
		[[nodiscard]] double getLengthSecs() const {
			return (double)getLength() / getFrequency();
		}

		[[nodiscard]] unsigned getNumFragments() const {
			return al_get_audio_stream_fragments(ptr());
		}
		[[nodiscard]] unsigned getNumAvailableFragments() const {
			return al_get_available_audio_stream_fragments(ptr());
		}

		void* getFragmentData() {
			return al_get_audio_stream_fragment(ptr());
		}
		bool setFragmentData(void* data) {
			return al_set_audio_stream_fragment(ptr(), data);
		}

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
		std::unique_ptr<EventSource> evSource;
    };

	inline ALLEGRO_EVENT_SOURCE *al::AudioStreamEventSource::ptr() const
	{
		return al_get_audio_stream_event_source(stream.ptr());
	}
}

#endif /* INCLUDE_AXXEGRO_AUDIO_AUDIOSTREAM */
