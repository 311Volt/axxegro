#ifndef INCLUDE_AXXEGRO_AUDIO_AUDIOSTREAM
#define INCLUDE_AXXEGRO_AUDIO_AUDIOSTREAM

#include <axxegro/resources/Resource.hpp>
#include <axxegro/event/EventSource.hpp>
#include <allegro5/allegro_audio.h>

#include <span>

namespace al {

    AXXEGRO_DEFINE_DELETER(ALLEGRO_AUDIO_STREAM, al_destroy_audio_stream);

    class AudioStream: public Resource<ALLEGRO_AUDIO_STREAM> {
    public:
        //EventSource& getEventSource();

        void drain();
        bool rewind();
        unsigned getFrequency() const;
        ALLEGRO_AUDIO_DEPTH getDepth() const;
        ALLEGRO_CHANNEL_CONF getChannelConf() const;
        unsigned getLength() const;
        double getLengthSecs() const;

        float getSpeed() const;
        bool setSpeed(float val);

        float getGain() const;
        bool setGain(float val);

        float getPan() const;
        bool setPan(float val);

        bool getPlaying() const;
        bool setPlaying(bool val);

        ALLEGRO_PLAYMODE getPlayMode() const;
        bool setPlayMode(ALLEGRO_PLAYMODE val);

        bool isAttached() const;
        bool detach();

        uint64_t getNumPlayedSamples() const;
    private:
        using Resource::Resource;

    };
}

#endif /* INCLUDE_AXXEGRO_AUDIO_AUDIOSTREAM */
