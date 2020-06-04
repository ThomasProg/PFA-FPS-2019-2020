#include "audioSource.hpp"

#include "audio.hpp"

#include <AL/al.h>
#include <AL/alc.h>

#include <cassert>

Resources::AudioSource::AudioSource(const Resources::Audio& audio)
{ 
    setAudio(audio);
}

Resources::AudioSource::AudioSource(AudioSource&& rhs) noexcept
    : source(rhs.source)
{
    rhs.source = 0;
}

Resources::AudioSource& Resources::AudioSource::operator=(AudioSource&& rhs) noexcept
{   
    if (source != 0)
        alDeleteSources(1, &source);

    source = rhs.source;

    rhs.source = 0;

    return *this;
}

Resources::AudioSource::~AudioSource()
{
    if (source != 0)
        alDeleteSources(1, &source);
}

void Resources::AudioSource::setAudio(const Resources::Audio& audio)
{
    if (source == 0)
        alGenSources(1, &source);  
        
    alSourcei(source, AL_BUFFER, audio.audioDataID);
}

void Resources::AudioSource::play()
{
    if (source != 0)
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
