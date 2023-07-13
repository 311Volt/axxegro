#ifndef INCLUDE_AXXEGRO_AUDIO_VOICE
#define INCLUDE_AXXEGRO_AUDIO_VOICE

#include "AudioCommon.hpp"
#include "Mixer.hpp"

namespace al {

	AXXEGRO_DEFINE_DELETER(ALLEGRO_VOICE, al_destroy_voice);

	class Voice:
			RequiresInitializables<AudioAddon>,
			public Resource<ALLEGRO_VOICE> {
	public:
		explicit Voice(AudioFormat audioFormat = {})
				: Resource<ALLEGRO_VOICE>(al_create_voice(audioFormat.frequency, audioFormat.depth, audioFormat.chanConf))
		{
			if(!ptr()) {
				throw AudioError("Could not create voice with format: %s", audioFormat.str().c_str());
			}
		}

		~Voice()
		{
			if(lastDefaultVoice) {
				al_set_default_voice(lastDefaultVoice);
			}
		}

		bool attachMixer(Mixer& mixer) {
			return al_attach_mixer_to_voice(mixer.ptr(), ptr());
		}
		bool attachAudioStream(BaseAudioStream& audio) {
			return al_attach_audio_stream_to_voice(audio.ptr(), ptr());
		}
		bool attachSampleInstance(SampleInstance& sampleInstance) {
			return al_attach_sample_instance_to_voice(sampleInstance.ptr(), ptr());
		}

		bool setAsDefault() {
			if(lastDefaultVoice != ptr()) {
				lastDefaultVoice = ptr();
			}
			al_set_default_voice(ptr());
			return false;
		}

		[[nodiscard]] unsigned getFrequency() const {
			return al_get_voice_frequency(ptr());
		}
		[[nodiscard]] ALLEGRO_CHANNEL_CONF getChannelConf() const {
			return al_get_voice_channels(ptr());
		}
		[[nodiscard]] ALLEGRO_AUDIO_DEPTH getDepth() const {
			return al_get_voice_depth(ptr());
		}

		[[nodiscard]] bool getPlaying() const {
			return al_get_voice_playing(ptr());
		}
		bool setPlaying(bool val) {
			return al_set_voice_playing(ptr(), val);
		}

		[[nodiscard]] unsigned getPosition() const {
			return al_get_voice_position(ptr());
		}
		bool setPosition(unsigned val) {
			return al_set_voice_position(ptr(), val);
		}

//		[[nodiscard]] bool hasAttachments() const;
	private:
		friend class CDefaultVoice;
		using Resource::Resource;
		ALLEGRO_VOICE *lastDefaultVoice = nullptr;
	};

	class CDefaultVoice: public Voice {
	public:
		using Voice::Voice;
		[[nodiscard]] ALLEGRO_VOICE* getPointer() const override {
			return al_get_default_voice();
		}
	};

	inline CDefaultVoice DefaultVoice {nullptr, al::ResourceModel::NonOwning};

}

#endif /* INCLUDE_AXXEGRO_AUDIO_VOICE */
