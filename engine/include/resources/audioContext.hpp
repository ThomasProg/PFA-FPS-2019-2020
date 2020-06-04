#ifndef _AUDIO_CONTEXT_HPP_
#define _AUDIO_CONTEXT_HPP_

#include <AL/al.h>
#include <AL/alc.h>

namespace Resources
{
    // Should be used to use AudioSource and other classes working with the sound.
    class AudioContext
    {
    public:
        ALCdevice*  device  = nullptr;
        ALCcontext* context = nullptr;

    public:
        // Opens the AL context.
        AudioContext();

        // Destroys the AL context.
        ~AudioContext();

        operator bool() const noexcept;
    };
}

#endif