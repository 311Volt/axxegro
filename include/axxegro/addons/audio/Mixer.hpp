#ifndef INCLUDE_AXXEGRO_AUDIO_MIXER
#define INCLUDE_AXXEGRO_AUDIO_MIXER

#include "SampleInstance.hpp"
#include "AudioStream.hpp"
#include "AudioCommon.hpp"

namespace al {

	AXXEGRO_DEFINE_DELETER(ALLEGRO_MIXER, al_destroy_mixer);

	class Mixer:
			RequiresInitializables<AudioAddon>,
			public Resource<ALLEGRO_MIXER>,
			public al::detail::AddAudioFormatQuery<Mixer> {
	public:
		explicit Mixer(AudioFormat audioFormat = {})
		    : Resource<ALLEGRO_MIXER>(nullptr)
		{
			if(auto* p = al_create_mixer(audioFormat.frequency, audioFormat.depth, audioFormat.chanConf)) {
				setPtr(p);
			} else {
				throw AudioError("Cannot create mixer with format: %s", audioFormat.str().c_str());
			}
		}

		bool attachMixer(Mixer& mixer) {
			return al_attach_mixer_to_mixer(mixer.ptr(), ptr());
		}
		bool attachAudioStream(BaseAudioStream& audio) {
			return al_attach_audio_stream_to_mixer(audio.ptr(), ptr());
		}
		bool attachSampleInstance(SampleInstance& sampleInstance) {
			return al_attach_sample_instance_to_mixer(sampleInstance.ptr(), ptr());
		}

		bool setAsDefault() {
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

		bool setPostprocessCallbackFnPtr(void (*cb)(void* buf, unsigned samples, void* userdata), void* userdata) {
			return al_set_mixer_postprocess_callback(ptr(), cb, userdata);
		}

	private:
		using Resource::Resource;
	};

	namespace detail {
		class CDefaultMixer: public Mixer
		{
		public:
			using Mixer::Mixer;

		private:
			[[nodiscard]] ALLEGRO_MIXER* getPointer() const override {
				if(!al_get_default_mixer()) {
					al_restore_default_mixer();
				}
				return al_get_default_mixer();
			}
		};
	}

	inline detail::CDefaultMixer DefaultMixer {nullptr, al::ResourceModel::NonOwning};

	template<detail::ValidSampleType TSample, ALLEGRO_CHANNEL_CONF TPChanConf>
	class UserMixer: public Mixer {
	public:

		using Traits = detail::FragmentTraits<TSample, TPChanConf>;

		using CallbackT = std::function<void(std::span<typename Traits::FragmentType>)>;

		explicit UserMixer(Hz freq = Hz(44100))
			: Mixer({.frequency = unsigned(freq.getHz()), .depth = Traits::Depth, .chanConf = TPChanConf})
		{

		}

		bool setPostprocessCallback(std::function<void(std::span<typename Traits::FragmentType>)> callback) {
			postprocessCallback = std::move(callback);

			return setPostprocessCallbackFnPtr([](void* buf, unsigned samples, void* userdata) {
				using FragT = typename Traits::FragmentType;
				CallbackT& ppcb = *static_cast<CallbackT*>(userdata);
				ppcb(std::span<FragT>((FragT*)buf, (FragT*)buf + samples));

			}, &postprocessCallback);
		}

	private:
		CallbackT postprocessCallback;
	};

}

#endif /* INCLUDE_AXXEGRO_AUDIO_MIXER */
