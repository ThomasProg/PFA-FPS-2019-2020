#ifndef _AUDIO_SOURCE_HPP_
#define _AUDIO_SOURCE_HPP_

#include <AL/al.h>
#include <AL/alc.h>

namespace Resources
{
    struct Audio;

    class AudioSource
    {
    private:
        // index of the gpu source's data
        ALuint source = 0; 

    public:
        AudioSource() = default;
        AudioSource(const Audio& audio);

        // We don't want to copy the data from opengl.
        AudioSource(const AudioSource&) = delete;
        AudioSource& operator=(const AudioSource&) = delete;

        AudioSource(AudioSource&&) noexcept;
        AudioSource& operator=(AudioSource&&) noexcept;
        ~AudioSource();

        // Sets the audio this source should play.
        void setAudio(const Audio& audio);
        // Play the current audio associed with this source.
        void play();
        void stop();

        // Get the duration the audio has been playing in seconds.
        ALfloat getCurrentDuration() const;
        // Get the state of the audio (on / off).
        ALint getState() const;

        // See openal documentation for more :
        // https://openal.org/documentation/OpenAL_Programmers_Guide.pdf
    };
}

#endif