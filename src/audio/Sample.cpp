#define AXXEGRO_TRUSTED

#include <axxegro/audio/Sample.hpp>
#include <axxegro/Exception.hpp>

al::Sample::Sample(const std::string &filename)
    : Resource<ALLEGRO_SAMPLE>(al_load_sample(filename.c_str()))
{
    if(!ptr()) {
        throw ResourceLoadError("Cannot load audio file: {}", filename);
    }
}

std::optional<al::SampleID> al::Sample::play(al::PlaybackParams options)
{
    ALLEGRO_SAMPLE_ID smpId;
    bool status = al_play_sample(ptr(), options.gain, options.pan, options.speed, options.loop, &smpId);
    if(!status) {
        return std::nullopt;
    }
    return smpId;
}

bool al::Sample::save(const std::string &filename)
{
    return al_save_sample(filename.c_str(), ptr());
}

ALLEGRO_CHANNEL_CONF al::Sample::getChannelConf() const
{
	return al_get_sample_channels(ptr());
}

ALLEGRO_AUDIO_DEPTH al::Sample::getDepth() const
{
	return al_get_sample_depth(ptr());
}

unsigned al::Sample::getFrequency() const
{
	return al_get_sample_frequency(ptr());
}

void al::SampleID::stop()
{
    al_stop_sample(this);
}
