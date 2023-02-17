#define AXXEGRO_TRUSTED

#include <axxegro/audio/Mixer.hpp>

al::Mixer::Mixer(AudioFormat audioFormat)
	: Resource<ALLEGRO_MIXER>(al_create_mixer(audioFormat.frequency, audioFormat.depth, audioFormat.chanConf))
{

}

bool al::Mixer::attachMixer(al::Mixer &mixer)
{
	return al_attach_mixer_to_mixer(mixer.ptr(), ptr());
}

bool al::Mixer::attachAudioStream(al::AudioStream &audio)
{
	return al_attach_audio_stream_to_mixer(audio.ptr(), ptr());
}

bool al::Mixer::attachSampleInstance(al::SampleInstance &sampleInstance)
{
	return al_attach_sample_instance_to_mixer(sampleInstance.ptr(), ptr());
}

unsigned al::Mixer::getFrequency() const
{
	return al_get_mixer_frequency(ptr());
}

bool al::Mixer::setFrequency(unsigned int val)
{
	return al_set_mixer_frequency(ptr(), val);
}

ALLEGRO_CHANNEL_CONF al::Mixer::getChannelConf() const
{
	return al_get_mixer_channels(ptr());
}

ALLEGRO_AUDIO_DEPTH al::Mixer::getDepth() const
{
	return al_get_mixer_depth(ptr());
}

float al::Mixer::getGain() const
{
	return al_get_mixer_gain(ptr());
}

bool al::Mixer::setGain(float val)
{
	return al_set_mixer_gain(ptr(), val);
}

ALLEGRO_MIXER_QUALITY al::Mixer::getQuality() const
{
	return al_get_mixer_quality(ptr());
}

bool al::Mixer::setQuality(ALLEGRO_MIXER_QUALITY val)
{
	return al_set_mixer_quality(ptr(), val);
}

bool al::Mixer::getPlaying() const
{
	return al_get_mixer_playing(ptr());
}

bool al::Mixer::setPlaying(bool val)
{
	return al_set_mixer_playing(ptr(), val);
}

bool al::Mixer::isAttached() const
{
	return al_get_mixer_attached(ptr());
}

//bool al::Mixer::hasAttachments() const
//{
//   //5.2.9
//}

bool al::Mixer::detach()
{
	return al_detach_mixer(ptr());
}

bool al::Mixer::setCStylePostprocessCallback(void (*cb)(void *, unsigned int, void *), void *userdata)
{
	return al_set_mixer_postprocess_callback(ptr(), cb, userdata);
}

al::Mixer::~Mixer()
{
	if(prevDefaultMixer) {
		al_set_default_mixer(prevDefaultMixer);
	}
}

bool al::Mixer::setAsDefault()
{
	if(prevDefaultMixer != ptr()) {
		prevDefaultMixer = al_get_default_mixer();
	}
	return al_set_default_mixer(ptr());
}

ALLEGRO_MIXER *al::CDefaultMixer::getPointer() const
{
	return al_get_default_mixer();
}

al::CDefaultMixer al::DefaultMixer {nullptr, al::ResourceModel::NonOwning};
