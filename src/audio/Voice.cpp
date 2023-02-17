#define AXXEGRO_TRUSTED

#include <axxegro/audio/Voice.hpp>

#include <axxegro/Exception.hpp>

al::Voice::Voice(AudioFormat audioFormat)
	: Resource<ALLEGRO_VOICE>(al_create_voice(audioFormat.frequency, audioFormat.depth, audioFormat.chanConf))
{
	if(!ptr()) {
		throw AudioError("Could not create voice with format: {}", audioFormat.str());
	}
}

bool al::Voice::attachMixer(al::Mixer &mixer)
{
	return al_attach_mixer_to_voice(mixer.ptr(), ptr());
}

bool al::Voice::attachAudioStream(al::AudioStream &audio)
{
	return al_attach_audio_stream_to_voice(audio.ptr(), ptr());
}

bool al::Voice::attachSampleInstance(al::SampleInstance &sampleInstance)
{
	return al_attach_sample_instance_to_voice(sampleInstance.ptr(), ptr());
}

unsigned al::Voice::getFrequency() const
{
	return al_get_voice_frequency(ptr());
}

ALLEGRO_CHANNEL_CONF al::Voice::getChannelConf() const
{
	return al_get_voice_channels(ptr());
}

ALLEGRO_AUDIO_DEPTH al::Voice::getDepth() const
{
	return al_get_voice_depth(ptr());
}

bool al::Voice::getPlaying() const
{
	return al_get_voice_playing(ptr());
}

bool al::Voice::setPlaying(bool val)
{
	return al_set_voice_playing(ptr(), val);
}

unsigned al::Voice::getPosition() const
{
	return al_get_voice_position(ptr());
}

bool al::Voice::setPosition(unsigned int val)
{
	return al_set_voice_position(ptr(), val);
}

//bool al::Voice::hasAttachments() const
//{
//	return false;
//}

bool al::Voice::setAsDefault()
{
	if(lastDefaultVoice != ptr()) {
		lastDefaultVoice = ptr();
	}
	al_set_default_voice(ptr());
	return false;
}

al::Voice::~Voice()
{
	if(lastDefaultVoice) {
		al_set_default_voice(lastDefaultVoice);
	}
}

ALLEGRO_VOICE *al::CDefaultVoice::getPointer() const
{
	return al_get_default_voice();
}

al::CDefaultVoice al::DefaultVoice {nullptr, al::ResourceModel::NonOwning};