#ifndef INCLUDE_AXXEGRO_AUDIO_VOICE
#define INCLUDE_AXXEGRO_AUDIO_VOICE

#include <axxegro/resources/Resource.hpp>
#include <axxegro/audio/AudioCommon.hpp>
#include <axxegro/audio/Mixer.hpp>

namespace al {

	AXXEGRO_DEFINE_DELETER(ALLEGRO_VOICE, al_destroy_voice);

	class Voice: public Resource<ALLEGRO_VOICE> {
	public:
		explicit Voice(AudioFormat audioFormat = {});
		~Voice();

		bool attachMixer(Mixer& mixer);
		bool attachAudioStream(AudioStream& audio);
		bool attachSampleInstance(SampleInstance& sampleInstance);

		bool setAsDefault();

		[[nodiscard]] unsigned getFrequency() const;
		[[nodiscard]] ALLEGRO_CHANNEL_CONF getChannelConf() const;
		[[nodiscard]] ALLEGRO_AUDIO_DEPTH getDepth() const;

		[[nodiscard]] bool getPlaying() const;
		bool setPlaying(bool val);

		[[nodiscard]] unsigned getPosition() const;
		bool setPosition(unsigned val);

//		[[nodiscard]] bool hasAttachments() const;
	private:
		friend class CDefaultVoice;
		using Resource::Resource;
		ALLEGRO_VOICE *lastDefaultVoice = nullptr;
	};

	class CDefaultVoice: public Voice {
	public:
		using Voice::Voice;
		ALLEGRO_VOICE* getPointer() const override;
	};

	extern CDefaultVoice DefaultVoice;

}

#endif /* INCLUDE_AXXEGRO_AUDIO_VOICE */
