#include "audioContext.hpp"

#include <cassert>
#include <iostream>

#include "log.hpp"

Resources::AudioContext::AudioContext()
{
    device = alcOpenDevice(NULL);
    if (device == NULL)
    {
        Core::Debug::Log::addMessage("cannot open sound card");
        return;
    }
    context = alcCreateContext(device, NULL);
    if (context == NULL)
    {
        Core::Debug::Log::addMessage("cannot open context");
        return;
    }
    alcMakeContextCurrent(context);
}

Resources::AudioContext::~AudioContext()
{
    alcDestroyContext(context);
    alcCloseDevice(device);
}

Resources::AudioContext::operator bool() const noexcept
{
    assert(context == nullptr || device != nullptr);
    return context != nullptr;
} 

