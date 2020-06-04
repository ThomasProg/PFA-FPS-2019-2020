#ifndef _AUDIO_HPP_
#define _AUDIO_HPP_

namespace Resources
{
    struct AudioData;

    struct Audio
    {
    public:

    public:
        // index to the gpu buffer
        unsigned int audioDataID = 0; 

        Audio() = default;
        Audio(const char* pathToFile);
        Audio(const AudioData& audioData);

        // We don't want to copy the pointer (audioDataID)
        Audio(const Audio&) = delete;
        Audio& operator=(const Audio&) = delete;

        Audio(Audio&& rhs) noexcept;
        Audio& operator=(Audio&&) noexcept;

        void sendCPUAudioToGPU(const AudioData& cpuAudioData);

        ~Audio();
    };
}

#endif