#ifndef _AUDIO_HPP_
#define _AUDIO_HPP_

namespace Resources
{
    struct AudioData;

    struct Audio
    {
    public:

    public:
        unsigned int audioDataID = 0; // index to the gpu buffer

        Audio() = default;
        Audio(const char* pathToFile);
        Audio(const AudioData& audioData);

        void sendCPUAudioToGPU(const AudioData& cpuAudioData);

        ~Audio();
    };
}

#endif