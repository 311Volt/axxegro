#ifndef INCLUDE_AXXEGRO_AUDIO_MIXER
#define INCLUDE_AXXEGRO_AUDIO_MIXER

#include <axxegro/resources/Resource.hpp>
#include <axxegro/audio/SampleInstance.hpp>
#include <axxegro/audio/AudioStream.hpp>

namespace al {

	AXXEGRO_DEFINE_DELETER(ALLEGRO_MIXER, al_destroy_mixer);

	class Mixer:
			public Resource<ALLEGRO_MIXER>,
			public AddAudioFormatQuery<Mixer> {
	public:
		explicit Mixer(AudioFormat audioFormat = {});
		~Mixer();

		bool attachMixer(Mixer& mixer);
		bool attachAudioStream(AudioStream& audio);
		bool attachSampleInstance(SampleInstance& sampleInstance);

		bool setAsDefault();

		[[nodiscard]] unsigned getFrequency() const;
		bool setFrequency(unsigned val);

		[[nodiscard]] ALLEGRO_CHANNEL_CONF getChannelConf() const;

		[[nodiscard]] ALLEGRO_AUDIO_DEPTH getDepth() const;

		[[nodiscard]] float getGain() const;
		bool setGain(float val);

		[[nodiscard]] ALLEGRO_MIXER_QUALITY getQuality() const;
		bool setQuality(ALLEGRO_MIXER_QUALITY val);

		[[nodiscard]] bool getPlaying() const;
		bool setPlaying(bool val);

		[[nodiscard]] bool isAttached() const;
//		[[nodiscard]] bool hasAttachments() const; //5.2.9

		bool detach();

		bool setCStylePostprocessCallback(void (*cb)(void* buf, unsigned samples, void* userdata), void* userdata);

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
		[[nodiscard]] ALLEGRO_MIXER* getPointer() const override;
	};

	extern CDefaultMixer DefaultMixer;

}

#endif /* INCLUDE_AXXEGRO_AUDIO_MIXER */
