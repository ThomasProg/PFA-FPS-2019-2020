#include "audioData.hpp"
#include "log.hpp"

#include <AL/al.h>
#include <AL/alc.h>

Resources::AudioData::AudioData(const char* filename)
{
    loadWAV(filename);
}

Resources::AudioData::AudioData(Resources::AudioData&& rhs) noexcept
{
    data     = rhs.data;
    dataInfo = rhs.dataInfo;
    rhs.data = nullptr;
}

Resources::AudioData& Resources::AudioData::operator=(AudioData&& rhs)
{
    if (data != nullptr)
    {
        delete[] data;
    }

    data     = rhs.data;
    dataInfo = rhs.dataInfo;
    rhs.data = nullptr;
    return *this;
}

Resources::AudioData::~AudioData()
{
    if (data != nullptr)
        delete[] data;
}

bool Resources::AudioData::loadWAV(const char* filename)
{
    // Opens file
    std::ifstream in(filename, std::ios::binary);

    // Reads data informations and put them into dataInfo
    in.read((char*)&dataInfo, sizeof(dataInfo));

    // If we couldn't load the file correctly, return failure
    if (in.fail() 
        || strncmp(&dataInfo.riffHeader[0], "RIFF", 4) != 0 
        || strncmp(&dataInfo.waveHeader[0], "WAVE", 4) != 0
        || strncmp(&dataInfo.fmtHeader [0], "fmt ", 4) != 0
        || strncmp(&dataInfo.dataHeader[0], "data", 4) != 0)
    {
        Core::Debug::Log::addMessage("Invalid format for a wave file");
        return false;
    }

    // Deallocate memory if necessary
    if (data != nullptr)
        delete[] data;

    // Allocate memory and fill the data buffer with the audio data.
    data = new char[dataInfo.dataSize];
    in.read(data, dataInfo.dataSize);

    // return success
    return true;
}


unsigned int Resources::AudioData::getFormat() const
{
    if (dataInfo.nbChannels == 1)
    {
        if (dataInfo.nbBitsPerSample == 8)
        {
            return AL_FORMAT_MONO8;
        }
        else {
            return AL_FORMAT_MONO16;
        }
    }
    else {
        if (dataInfo.nbBitsPerSample == 8)
        {
            return AL_FORMAT_STEREO8;
        }
        else {
            return AL_FORMAT_STEREO16;
        }
    }
}

