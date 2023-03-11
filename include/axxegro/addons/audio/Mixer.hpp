#ifndef INCLUDE_AXXEGRO_AUDIO_MIXER
#define INCLUDE_AXXEGRO_AUDIO_MIXER

#include "SampleInstance.hpp"
#include "AudioStream.hpp"

namespace al {

	AXXEGRO_DEFINE_DELETER(ALLEGRO_MIXER, al_destroy_mixer);

	class Mixer:
			public Resource<ALLEGRO_MIXER>,
			public AddAudioFormatQuery<Mixer> {
	public:
		explicit Mixer(AudioFormat audioFormat = {})
		    : Resource<ALLEGRO_MIXER>(al_create_mixer(audioFormat.frequency, audioFormat.depth, audioFormat.chanConf))
		{

		}
		~Mixer()
		{
			if(prevDefaultMixer) {
				al_set_default_mixer(prevDefaultMixer);
			}
		}

		bool attachMixer(Mixer& mixer) {
			return al_attach_mixer_to_mixer(mixer.ptr(), ptr());
		}
		bool attachAudioStream(AudioStream& audio) {
			return al_attach_audio_stream_to_mixer(audio.ptr(), ptr());
		}
		bool attachSampleInstance(SampleInstance& sampleInstance) {
			return al_attach_sample_instance_to_mixer(sampleInstance.ptr(), ptr());
		}

		bool setAsDefault() {
			if(prevDefaultMixer != ptr()) {
				prevDefaultMixer = al_get_default_mixer();
			}
			return al_set_default_mixer(ptr());
		}

		[[nodiscard]] unsigned getFrequency() const {
			return al_get_mixer_frequency(ptr());
		}
		bool setFrequency(unsigned val) {
			return al_set_mixer_frequency(ptr(), val);
		}

		[[nodiscard]] ALLEGRO_CHANNEL_CONF getChannelConf() const {
			return al_get_mixer_channels(ptr());
		}

		[[nodiscard]] ALLEGRO_AUDIO_DEPTH getDepth() const {
			return al_get_mixer_depth(ptr());
		}

		[[nodiscard]] float getGain() const {
			return al_get_mixer_gain(ptr());
		}
		bool setGain(float val) {
			return al_set_mixer_gain(ptr(), val);
		}

		[[nodiscard]] ALLEGRO_MIXER_QUALITY getQuality() const {
			return al_get_mixer_quality(ptr());
		}
		bool setQuality(ALLEGRO_MIXER_QUALITY val) {
			return al_set_mixer_quality(ptr(), val);
		}

		[[nodiscard]] bool getPlaying() const {
			return al_get_mixer_playing(ptr());
		}
		bool setPlaying(bool val) {
			return al_set_mixer_playing(ptr(), val);
		}

		[[nodiscard]] bool isAttached() const {
			return al_get_mixer_attached(ptr());
		}
//		[[nodiscard]] bool hasAttachments() const; //5.2.9

		bool detach() {
			return al_detach_mixer(ptr());
		}

		bool setCStylePostprocessCallback(void (*cb)(void* buf, unsigned samples, void* userdata), void* userdata) {
			return al_set_mixer_postprocess_callback(ptr(), cb, userdata);
		}

		template<ALLEGRO_CHANNEL_CONF ChanConf, ALLEGRO_AUDIO_DEPTH AudioDepth>
		bool setPostprocessCallback(void (*callback)(std::span<typename SampleType<ChanConf, AudioDepth>::Type>))
		{
			using SmpType = typename SampleType<ChanConf, AudioDepth>::Type;
			return setCStylePostprocessCallback([callback](void* buf, unsigned samples, [[maybe_unused]] void* userdata){
				callback({static_cast<SmpType*>(buf), samples});
			}, nullptr);
		}
	private:
		friend class CDefaultMixer;
		using Resource::Resource;
		ALLEGRO_MIXER* prevDefaultMixer = nullptr;
	};

	class CDefaultMixer: public Mixer
	{
	public:
		using Mixer::Mixer;
	private:
		[[nodiscard]] ALLEGRO_MIXER* getPointer() const override {
			return al_get_default_mixer();
		}
	};

	inline CDefaultMixer DefaultMixer {nullptr, al::ResourceModel::NonOwning};

}

#endif /* INCLUDE_AXXEGRO_AUDIO_MIXER */
