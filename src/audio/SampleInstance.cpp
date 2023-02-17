#define AXXEGRO_TRUSTED

#include <axxegro/Exception.hpp>
#include <axxegro/audio/SampleInstance.hpp>

al::SampleInstance::SampleInstance(const al::Sample &sample)
	: Resource<ALLEGRO_SAMPLE_INSTANCE>(al_create_sample_instance(sample.constPtr()))
{
	if(!ptr()) {

	}
}

bool al::SampleInstance::play()
{
	return al_play_sample_instance(ptr());
}

bool al::SampleInstance::stop()
{
	return al_stop_sample_instance(ptr());
}

ALLEGRO_CHANNEL_CONF al::SampleInstance::getChannelConf() const
{
	return al_get_sample_instance_channels(ptr());
}

ALLEGRO_AUDIO_DEPTH al::SampleInstance::getDepth() const
{
	return al_get_sample_instance_depth(ptr());
}

unsigned al::SampleInstance::getFrequency() const
{
	return al_get_sample_instance_frequency(ptr());
}

float al::SampleInstance::getLengthSecs() const
{
	return al_get_sample_instance_time(ptr());
}

unsigned int al::SampleInstance::getLength() const
{
	return al_get_sample_instance_length(ptr());
}

bool al::SampleInstance::setLength(unsigned int numSamples)
{
	return al_set_sample_instance_length(ptr(), numSamples);
}

unsigned al::SampleInstance::getPosition() const
{
	return al_get_sample_instance_position(ptr());
}

bool al::SampleInstance::setPosition(unsigned int val)
{
	return al_set_sample_instance_position(ptr(), val);
}

float al::SampleInstance::getSpeed() const
{
	return al_get_sample_instance_speed(ptr());
}

bool al::SampleInstance::setSpeed(float val)
{
	return al_set_sample_instance_speed(ptr(), val);
}

float al::SampleInstance::getGain() const
{
	return al_get_sample_instance_gain(ptr());
}

bool al::SampleInstance::setGain(float val)
{
	return al_set_sample_instance_gain(ptr(), val);
}

float al::SampleInstance::getPan() const
{
	return al_get_sample_instance_pan(ptr());
}

bool al::SampleInstance::setPan(float val)
{
	return al_set_sample_instance_pan(ptr(), val);
}

ALLEGRO_PLAYMODE al::SampleInstance::getPlayMode() const
{
	return al_get_sample_instance_playmode(ptr());
}

bool al::SampleInstance::setPlayMode(ALLEGRO_PLAYMODE val)
{
	return al_set_sample_instance_playmode(ptr(), val);
}

bool al::SampleInstance::getPlaying() const
{
	return al_get_sample_instance_playing(ptr());
}

bool al::SampleInstance::setPlaying(bool val)
{
	return al_set_sample_instance_playing(ptr(), val);
}

bool al::SampleInstance::isAttached() const
{
	return al_get_sample_instance_attached(ptr());
}

bool al::SampleInstance::detach()
{
	return al_detach_sample_instance(ptr());
}

bool al::SampleInstance::setSample(al::Sample &sample)
{
	return al_set_sample(ptr(), sample.ptr());
}

void al::SampleInstance::ReserveSamples(int numSamples)
{
    if(!al_reserve_samples(numSamples)) {
        throw AudioError("Cannot reserve {} samples", numSamples);
    }
}