#include "audioContext.hpp"

#include <cassert>
#include <iostream>

Resources::AudioContext::AudioContext()
{
    device = alcOpenDevice(NULL);
    if (device == NULL)
    {
        std::cout << "cannot open sound card" << std::endl;
        return;
    }
    context = alcCreateContext(device, NULL);
    if (context == NULL)
    {
        std::cout << "cannot open context" << std::endl;
        return;
    }
    alcMakeContextCurrent(context);
}

Resources::AudioContext::~AudioContext()
{
    alcDestroyContext(context);
    alcCloseDevice(device);
}

Resources::AudioContext::operator bool() const
{
    // Either context is invalid (and so device is too),
    // Either
    assert(context == nullptr || device != nullptr);
    return context != nullptr;
} 

