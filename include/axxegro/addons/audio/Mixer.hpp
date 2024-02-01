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
		/**
		 * @brief Creates a mixer.
		 * @param audioFormat The audio format.
		 * @see https://liballeg.org/a5docs/trunk/audio.html#al_create_mixer
		 */
		explicit Mixer(AudioFormat audioFormat = {})
		    : Resource<ALLEGRO_MIXER>(nullptr)
		{
			if(auto* p = al_create_mixer(audioFormat.frequency, audioFormat.depth, audioFormat.chanConf)) {
				setPtr(p);
			} else {
				throw AudioError("Cannot create mixer with format: %s", audioFormat.str().c_str());
			}
		}

		/**
		 * Attaches a mixer to this mixer.
		 * @return Whether the operation was successful.
		 * @see https://liballeg.org/a5docs/trunk/audio.html#al_attach_mixer_to_mixer
		 */

		bool attachMixer(Mixer& mixer) {
			return al_attach_mixer_to_mixer(mixer.ptr(), ptr());
		}

		/**
		 * Attaches an audio stream to this mixer.
		 * @return Whether the operation was successful.
		 * @see https://liballeg.org/a5docs/trunk/audio.html#al_attach_audio_stream_to_mixer
		 */
		bool attachAudioStream(BaseAudioStream& audio) {
			return al_attach_audio_stream_to_mixer(audio.ptr(), ptr());
		}

		/**
		 * Attaches a sample instance to this mixer.
		 * @return Whether the operation was successful.
		 * @see https://liballeg.org/a5docs/trunk/audio.html#al_attach_sample_instance_to_mixer
		 */
		bool attachSampleInstance(SampleInstance& sampleInstance) {
			return al_attach_sample_instance_to_mixer(sampleInstance.ptr(), ptr());
		}

		/**
		 * Sets this mixer as the default mixer.
		 * @see https://liballeg.org/a5docs/trunk/audio.html#al_set_default_mixer
		 */
		bool setAsDefault() {
			return al_set_default_mixer(ptr());
		}

		/// Gets the mixer's frequency (Hz).
		[[nodiscard]] unsigned getFrequency() const {
			return al_get_mixer_frequency(ptr());
		}

		/// Sets the mixer's frequency (Hz).
		bool setFrequency(unsigned val) {
			return al_set_mixer_frequency(ptr(), val);
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_get_mixer_channels
		[[nodiscard]] ALLEGRO_CHANNEL_CONF getChannelConf() const {
			return al_get_mixer_channels(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_get_mixer_depth
		[[nodiscard]] ALLEGRO_AUDIO_DEPTH getDepth() const {
			return al_get_mixer_depth(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_get_mixer_gain
		[[nodiscard]] float getGain() const {
			return al_get_mixer_gain(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_set_mixer_gain
		bool setGain(float val) {
			return al_set_mixer_gain(ptr(), val);
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_get_mixer_quality
		[[nodiscard]] ALLEGRO_MIXER_QUALITY getQuality() const {
			return al_get_mixer_quality(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_set_mixer_quality
		bool setQuality(ALLEGRO_MIXER_QUALITY val) {
			return al_set_mixer_quality(ptr(), val);
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_get_mixer_playing
		[[nodiscard]] bool getPlaying() const {
			return al_get_mixer_playing(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_set_mixer_playing
		bool setPlaying(bool val) {
			return al_set_mixer_playing(ptr(), val);
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_get_mixer_attached
		[[nodiscard]] bool isAttached() const {
			return al_get_mixer_attached(ptr());
		}
//		[[nodiscard]] bool hasAttachments() const; //5.2.9

		/**
		 * @brief Detach the mixer from whatever it is attached to, if anything.
		 * @see https://liballeg.org/a5docs/trunk/audio.html#al_detach_mixer
		 */
		bool detach() {
			return al_detach_mixer(ptr());
		}

		/**
		 * @brief Provide a function to be called every time the mixer processes a buffer.
		 * A type-safe wrapper is available when using {@link UserMixer}.
		 *
		 * @see https://liballeg.org/a5docs/trunk/audio.html#al_set_mixer_postprocess_callback
		 * @param cb
		 * @param userdata A pointer that will be passed as the final parameter to every call to cb.
		 * @return
		 */
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

		/**
		 * @brief Provide a function to be called every time the mixer processes a buffer.
		 * @param callback A function that takes a span of the buffer's fragment type. For instance,
		 * a callback for UserMixer<float, al::Stereo> should take an std::span<al::Vec2<float>>.
		 * @return Whether the operator was successful.
		 */
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
