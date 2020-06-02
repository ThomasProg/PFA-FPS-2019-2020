#ifndef _AUDIO_CONTEXT_HPP_
#define _AUDIO_CONTEXT_HPP_

#include <AL/al.h>
#include <AL/alc.h>

namespace Resources
{
    class AudioContext
    {
    public:
        ALCdevice*  device  = nullptr;
        ALCcontext* context = nullptr;

    public:
        AudioContext();
        ~AudioContext();

        operator bool() const;
    };
}

#endif