#ifndef _AUDIO_SOURCE_HPP_
#define _AUDIO_SOURCE_HPP_

#include <AL/al.h>
#include <AL/alc.h>

namespace Resources
{
    struct Audio;

    class AudioSource
    {
    public:
        ALuint source = 0; // index of the gpu source's data

    public:
        AudioSource();
        AudioSource(const Audio& audio);
        ~AudioSource();

        void setAudio(const Audio& audio);
        void play();
        void stop();
        ALfloat getCurrentDuration() const;
        ALint getState() const;
    };
}

#endif