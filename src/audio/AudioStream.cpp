#define AXXEGRO_TRUSTED

#include <axxegro/audio/AudioStream.hpp>

//al::EventSource &al::AudioStream::getEventSource()
//{
//    ;
//}

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
