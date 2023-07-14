#ifndef INCLUDE_AXXEGRO_AUDIO_AUDIOSTREAM
#define INCLUDE_AXXEGRO_AUDIO_AUDIOSTREAM

#include "AudioCommon.hpp"

namespace al {

	class BaseAudioStream;
	struct AudioStreamEventSource: public EventSource {
	public:
		explicit AudioStreamEventSource(BaseAudioStream& stream) : stream(stream) {}
		[[nodiscard]] ALLEGRO_EVENT_SOURCE* ptr() const override;
	private:
		BaseAudioStream& stream;
	};

    AXXEGRO_DEFINE_DELETER(ALLEGRO_AUDIO_STREAM, al_destroy_audio_stream);

	class BaseAudioStream:
			RequiresInitializables<AudioAddon>,
			public Resource<ALLEGRO_AUDIO_STREAM>,
			public AddPlaybackParamsQuery<BaseAudioStream>,
			public AddAudioFormatQuery<BaseAudioStream> {
    public:

		explicit BaseAudioStream(
			AudioFormat format = {},
			BufferConfig bufConfig = {
				.numChunks = 2,
				.fragmentsPerChunk = 2048
			}
		)
				: Resource<ALLEGRO_AUDIO_STREAM>(nullptr),
				  bufferConfig(bufConfig),
				  evSource(new AudioStreamEventSource(*this))
		{
			if(auto* p = al_create_audio_stream(
					bufConfig.numChunks, bufConfig.fragmentsPerChunk,
					format.frequency, format.depth, format.chanConf
			)) {
				setPtr(p);
			} else {
				throw AudioError("Cannot create audio stream with %d x %d bufs and format %s", (int)bufConfig.numChunks, (int)bufConfig.fragmentsPerChunk, format.str().c_str());
			}
		}

		explicit BaseAudioStream(const std::string& filename, BufferConfig bufConfig = {
			.numChunks = 2,
			.fragmentsPerChunk = 2048
		})
			: 	Resource<ALLEGRO_AUDIO_STREAM>(nullptr),
				bufferConfig(bufConfig),
				evSource(new AudioStreamEventSource(*this))
		{
			InternalRequire<AudioCodecAddon>();
			if(auto* p = al_load_audio_stream(filename.c_str(), bufConfig.numChunks, bufConfig.fragmentsPerChunk)) {
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

		[[nodiscard]] unsigned getNumChunks() const {
			return al_get_audio_stream_fragments(ptr());
		}
		[[nodiscard]] unsigned getNumAvailableChunks() const {
			return al_get_available_audio_stream_fragments(ptr());
		}

		[[nodiscard]] void* getChunkDataPtr() const {
			return al_get_audio_stream_fragment(ptr());
		}
		bool commitChunk(void* data) {
			return al_set_audio_stream_fragment(ptr(), data);
		}

#ifdef ALLEGRO_UNSTABLE
		bool setChannelMatrixData(const float* matrix) {
			return al_set_audio_stream_channel_matrix(ptr(), matrix);
		}
#endif

	protected:
		BufferConfig bufferConfig;
		using Resource::Resource;
	private:

		std::unique_ptr<EventSource> evSource;
    };

	inline ALLEGRO_EVENT_SOURCE *al::AudioStreamEventSource::ptr() const
	{
		return al_get_audio_stream_event_source(stream.ptr());
	}



	template<ValidSampleType TSample, ALLEGRO_CHANNEL_CONF TPChanConf>
	class AudioStream : public BaseAudioStream {
	public:

		using Traits = FragmentTraits<TSample, TPChanConf>;

		explicit AudioStream(Freq freq = Hz(44100), BufferConfig bufConfig = {})
			: BaseAudioStream({
				.frequency = (unsigned)freq.getFreqHz(),
				.depth = Traits::Depth,
				.chanConf = Traits::ChanConf}, bufConfig)
		{

		}

		explicit AudioStream(const std::string& filename, BufferConfig bufConfig = {})
			: BaseAudioStream(filename, bufConfig)
		{
			if(getDepth() != Traits::Depth || getChannelConf() != Traits::ChanConf) {
				throw AudioError(
					"Audio file %s has wrong format (expected %s, got %s)",
					AudioFormat{getFrequency(), Traits::Depth, Traits::ChanConf}.str().c_str(),
					getAudioFormat().str().c_str()
				);
			}
		}

#ifdef ALLEGRO_UNSTABLE
		bool setChannelMatrix(const std::span<Vec<float, Traits::NumChannels>> matrix) {
			if constexpr(decltype(matrix)::value_type::IsContiguous) {
				return setChannelMatrixData(reinterpret_cast<float*>(matrix.data()));
			}

			static constexpr int N = Traits::NumChannels;
			std::vector<float> data(matrix.size() * N);
			for(unsigned i=0; i<matrix.size(); i++) {
				for(int j=0; j<N; j++) {
					data[i*N + j] = matrix[i][j];
				}
			}
			return setChannelMatrixData(data.data());
		}
#endif

		[[nodiscard]] GenericEventHandler createChunkEventHandler(
				std::function<void(std::span<typename Traits::FragmentType>)> filler)
		{
			using FragT = typename Traits::FragmentType;
			return [filler, this]([[maybe_unused]] const Event& event){
				if(void* vData = getChunkDataPtr()) {
					std::span<FragT> data((FragT*)vData, (FragT*)vData + bufferConfig.fragmentsPerChunk);
					filler(data);
					commitChunk(vData);
				}
			};
		}

	};

}

#endif /* INCLUDE_AXXEGRO_AUDIO_AUDIOSTREAM */
