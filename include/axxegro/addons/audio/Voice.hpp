#ifndef INCLUDE_AXXEGRO_AUDIO_VOICE
#define INCLUDE_AXXEGRO_AUDIO_VOICE

#include "AudioCommon.hpp"
#include "Mixer.hpp"

namespace al {

	AXXEGRO_DEFINE_DELETER(ALLEGRO_VOICE, al_destroy_voice);

	/**
	 * @brief Represents an audio output device on the operating system.
	 * @see https://liballeg.org/a5docs/trunk/audio.html#allegro_voice
	 */
	class Voice:
			RequiresInitializables<AudioAddon>,
			public Resource<ALLEGRO_VOICE> {
	public:
		/**
		 * @brief Creates a voice. Normally you do not need to create one yourself
		 * and can use the default one (al::DefaultMixer).
		 * @throws AudioError when the voice cannot be created for any reason (e.g. no driver, bad audio format)
		 */
		explicit Voice(AudioFormat audioFormat = {.depth = ALLEGRO_AUDIO_DEPTH_INT16})
				: Resource<ALLEGRO_VOICE>(al_create_voice(audioFormat.frequency, audioFormat.depth, audioFormat.chanConf))
		{
			if(!ptr()) {
				throw AudioError("Could not create voice with format: %s", audioFormat.str().c_str());
			}
		}

		/**
		 * @brief Attaches a mixer to this voice.
		 * @return Whether the operation was successful.
		 * @see https://liballeg.org/a5docs/trunk/audio.html#al_attach_mixer_to_voice
		 */
		bool attachMixer(Mixer& mixer) {
			return al_attach_mixer_to_voice(mixer.ptr(), ptr());
		}

		/**
		 * @brief Attaches an audio stream to this voice.
		 * @return Whether the operation was successful.
		 * @see https://liballeg.org/a5docs/trunk/audio.html#al_attach_mixer_to_voice
		 */
		bool attachAudioStream(BaseAudioStream& audio) {
			return al_attach_audio_stream_to_voice(audio.ptr(), ptr());
		}

		/**
		 * @brief Attaches a sample instance to this voice.
		 * This may fail if the audio driver does not support preloading sample data, and Allegro's
		 * docs currently recommend using a mixer in between instead.
		 *
		 * @return Whether the operation was successful.
		 * @see https://liballeg.org/a5docs/trunk/audio.html#al_attach_mixer_to_voice
		 */
		bool attachSampleInstance(SampleInstance& sampleInstance) {
			return al_attach_sample_instance_to_voice(sampleInstance.ptr(), ptr());
		}

		/// @brief Set the current voice as default.
		void setAsDefault() {
			al_set_default_voice(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_get_voice_frequency
		[[nodiscard]] unsigned getFrequency() const {
			return al_get_voice_frequency(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_get_voice_channels
		[[nodiscard]] ALLEGRO_CHANNEL_CONF getChannelConf() const {
			return al_get_voice_channels(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_get_voice_depth
		[[nodiscard]] ALLEGRO_AUDIO_DEPTH getDepth() const {
			return al_get_voice_depth(ptr());
		}


		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_get_voice_playing
		[[nodiscard]] bool getPlaying() const {
			return al_get_voice_playing(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_set_voice_playing
		bool setPlaying(bool val) {
			return al_set_voice_playing(ptr(), val);
		}


		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_get_voice_position
		[[nodiscard]] unsigned getPosition() const {
			return al_get_voice_position(ptr());
		}

		/// @see https://liballeg.org/a5docs/trunk/audio.html#al_set_voice_position
		bool setPosition(unsigned val) {
			return al_set_voice_position(ptr(), val);
		}

//		[[nodiscard]] bool hasAttachments() const;
	private:
		friend class CDefaultVoice;
		using Resource::Resource;
	};

	class CDefaultVoice: public Voice {
	public:
		using Voice::Voice;
		[[nodiscard]] ALLEGRO_VOICE* getPointer() const override {
			return al_get_default_voice();
		}
	};

	/**
	 * @brief Refers to the currently set default voice.
	 */
	inline CDefaultVoice DefaultVoice {nullptr, al::ResourceModel::NonOwning};

}

#endif /* INCLUDE_AXXEGRO_AUDIO_VOICE */
