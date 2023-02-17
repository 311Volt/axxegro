#define AXXEGRO_TRUSTED

#include <axxegro/audio/AudioStream.hpp>
#include <memory>

al::AudioStream::AudioStream(size_t bufferCount, unsigned int samples, al::AudioFormat format)
	: Resource<ALLEGRO_AUDIO_STREAM>(al_create_audio_stream(bufferCount, samples, format.frequency, format.depth, format.chanConf)),
	  evSource(new AudioStreamEventSource(*this))
{

}

al::AudioStream::AudioStream(const std::string &filename, size_t bufferCount, unsigned int samples)
	: Resource<ALLEGRO_AUDIO_STREAM>(al_load_audio_stream(filename.c_str(), bufferCount, samples)),
	  evSource(new AudioStreamEventSource(*this))
{

}

void al::AudioStream::drain()
{
    al_drain_audio_stream(ptr());
}

bool al::AudioStream::rewind()
{
    return al_rewind_audio_stream(ptr());
}

unsigned al::AudioStream::getFrequency() const
{
    return al_get_audio_stream_frequency(ptr());
}

ALLEGRO_AUDIO_DEPTH al::AudioStream::getDepth() const
{
    return al_get_audio_stream_depth(ptr());
}

ALLEGRO_CHANNEL_CONF al::AudioStream::getChannelConf() const
{
    return al_get_audio_stream_channels(ptr());
}

unsigned al::AudioStream::getLength() const
{
    return al_get_audio_stream_length(ptr());
}

double al::AudioStream::getLengthSecs() const
{
    return (double)getLength() / getFrequency();
}

float al::AudioStream::getSpeed() const
{
    return al_get_audio_stream_speed(ptr());
}

bool al::AudioStream::setSpeed(float val)
{
    return al_set_audio_stream_speed(ptr(), val);
}

float al::AudioStream::getGain() const
{
    return al_get_audio_stream_gain(ptr());
}

bool al::AudioStream::setGain(float val)
{
    return al_set_audio_stream_gain(ptr(), val);
}

float al::AudioStream::getPan() const
{
    return al_get_audio_stream_pan(ptr());
}

bool al::AudioStream::setPan(float val)
{
    return al_set_audio_stream_pan(ptr(), val);
}

bool al::AudioStream::getPlaying() const
{
    return al_get_audio_stream_playing(ptr());
}

bool al::AudioStream::setPlaying(bool val)
{
    return al_set_audio_stream_playing(ptr(), val);
}

ALLEGRO_PLAYMODE al::AudioStream::getPlayMode() const
{
    return al_get_audio_stream_playmode(ptr());
}

bool al::AudioStream::setPlayMode(ALLEGRO_PLAYMODE val)
{
    return al_set_audio_stream_playmode(ptr(), val);
}

bool al::AudioStream::isAttached() const
{
    return al_get_audio_stream_attached(ptr());
}

bool al::AudioStream::detach()
{
    return al_detach_audio_stream(ptr());
}

uint64_t al::AudioStream::getNumPlayedSamples() const
{
    return al_get_audio_stream_played_samples(ptr());
}

al::EventSource &al::AudioStream::getEventSource()
{
	return *evSource;
}

bool al::AudioStream::seekSecs(double time)
{
	return al_seek_audio_stream_secs(ptr(), time);
}

double al::AudioStream::getPositionSecs() const
{
	return al_get_audio_stream_position_secs(ptr());
}

unsigned al::AudioStream::getNumFragments() const
{
	return al_get_audio_stream_fragments(ptr());
}

unsigned al::AudioStream::getNumAvailableFragments() const
{
	return al_get_available_audio_stream_fragments(ptr());
}

void *al::AudioStream::getFragmentData()
{
	return al_get_audio_stream_fragment(ptr());
}

bool al::AudioStream::setFragmentData(void *data)
{
	return al_set_audio_stream_fragment(ptr(), data);
}

//al::AudioStream &al::AudioStream::Play(const std::string &filename)
//{
//       //5.2.8 UNSTABLE
//	static constexpr int alver = ALLEGRO_VERSION_INT;
//	ALLEGRO_AUDIO_STREAM *stream = al_play_audio_stream(filename.c_str());
//	defaultAudioStream = std::make_unique<AudioStream>(stream, ResourceModel::NonOwning);
//	return *defaultAudioStream;
//}

al::AudioStreamEventSource::AudioStreamEventSource(al::AudioStream &stream)
	: stream(stream)
{

}

ALLEGRO_EVENT_SOURCE *al::AudioStreamEventSource::ptr() const
{
	return al_get_audio_stream_event_source(stream.ptr());
}
