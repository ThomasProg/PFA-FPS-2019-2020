#include "audio.hpp"

#include "audioData.hpp"

#include <AL/al.h>
#include <AL/alc.h>

Resources::Audio::Audio(const char* pathToFile)
{
    AudioData audioData{pathToFile};
    sendCPUAudioToGPU(audioData);
}

Resources::Audio::Audio(const Resources::AudioData& audioData)
{
    sendCPUAudioToGPU(audioData);
}

Resources::Audio::Audio(Audio&& rhs) noexcept
    : audioDataID(rhs.audioDataID) 
{
    rhs.audioDataID = 0;
}

Resources::Audio& Resources::Audio::operator=(Resources::Audio&& rhs) noexcept
{
    audioDataID = rhs.audioDataID;
    rhs.audioDataID = 0;
    return *this;
}


void Resources::Audio::sendCPUAudioToGPU(const Resources::AudioData& cpuAudioData)
{
    if (audioDataID != 0)
    {
        alDeleteBuffers(1, &audioDataID);
    }
    
    alGenBuffers(1, &audioDataID);
    alBufferData(audioDataID, cpuAudioData.getFormat(), cpuAudioData.data, cpuAudioData.dataInfo.dataSize, cpuAudioData.dataInfo.nbSamplesPerSecond);
}

Resources::Audio::~Audio() 
{
    if (audioDataID != 0)
        alDeleteBuffers(1, &audioDataID);
}