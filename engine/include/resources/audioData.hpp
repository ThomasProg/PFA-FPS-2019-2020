#ifndef _CPU_AUDIO_HPP_
#define _CPU_AUDIO_HPP_

#include <iostream>
#include <fstream>
#include <cstring>

namespace Resources
{
    struct AudioData
    {
    public:
        // Refer to https://fr.wikipedia.org/wiki/Waveform_Audio_File_Format for further information on the .wav format.
        struct WaveData
        {
            /* "RIFF" Chunk Descriptor */
            char         riffHeader[4];        // Magic header; value should be "RIFF", or it is an invalid header.
            uint32_t        fileSize;                  // Size of file less riff size (less 8 bytes).
            char         waveHeader[4];        // Same as riff, value shoud be "WAVE".
            /* "fmt " sub-chunk */
            // Describes audio format
            char         fmtHeader[4];   // fmt header, value should be "fmt " (notice the whitespace at the end).
            uint32_t        fmtSize;  // Size of the fmt chunk
            uint16_t        audioFormat;    // 1=PCM, 6=mulaw, 7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
            uint16_t        nbChannels;     // Number of channels 1=Mono 2=Sterio
            uint32_t        nbSamplesPerSecond;  // Sampling Frequency in Hz
            uint32_t        nbBytesPerSecond;    // bytes per second
            uint16_t        nbBytesPerChunk;     // 2=16-bit mono, 4=16-bit stereo
            uint16_t        nbBitsPerSample;  // Number of bits per sample
            // /* "data" sub-chunk */
            // Describes data in memory
            char         dataHeader[4]; // Same as riff, value should be "data".
            uint32_t        dataSize;  // Sampled data length
        };

    public: // variables
        char* data = nullptr;
        WaveData dataInfo;

    public: // constructors
        AudioData() = default;
        AudioData(const char* filename);
        ~AudioData();

    public: // functions
        bool loadWAV(const char* filename);
        unsigned int getFormat() const;
    };
}

#endif