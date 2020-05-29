#include "audioSource.hpp"

#include "audio.hpp"

#include <AL/al.h>
#include <AL/alc.h>

#include <cassert>

Resources::AudioSource::AudioSource()
{
    alGenSources(1, &source);  
}

Resources::AudioSource::AudioSource(const Resources::Audio& audio)
{
    alGenSources(1, &source);  
    setAudio(audio);
}

Resources::AudioSource::~AudioSource()
{
    if (source != 0)
        alDeleteSources(1, &source);
}

void Resources::AudioSource::setAudio(const Resources::Audio& audio)
{
    alSourcei(source, AL_BUFFER, audio.audioDataID);
}

void Resources::AudioSource::play()
{
    assert(source != 0);
    alSourcePlay(source);
}

void Resources::AudioSource::stop()
{
    alSourceStop(source);
}

ALfloat Resources::AudioSource::getCurrentDuration() const
{
    ALfloat time;
    alGetSourcef(source, AL_SEC_OFFSET, &time);
    return time;
}

ALint Resources::AudioSource::getState() const
{
    ALint state;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    return state;
}
