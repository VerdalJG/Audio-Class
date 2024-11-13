#include "AudioBuffer.h"

#include <iostream>
#include <fstream>
#include <string>

AudioBuffer::AudioBuffer(ALuint buffer, ALenum format, ALsizei size, ALsizei frequency) :
    buffer(buffer),
    format(format),
    size(size),
    frequency(frequency)
{
}

AudioBuffer::~AudioBuffer()
{
    alDeleteBuffers(1, &buffer);
}

AudioBuffer* AudioBuffer::Load(const char* filename)
{
    // Open the file as a binary stream
    std::ifstream fileStream(filename, std::ios::binary);

    if (!fileStream)
    {
        printf("Failed to open file!\n");
        return nullptr;
    }

    // Buffer to hold chunk IDs and data from the file
    char chunkBuffer[4];
    fileStream.read(chunkBuffer, 4);
    
    if (std::string(chunkBuffer, 4) != "RIFF")
    {
        printf("Not a valid RIFF file!\n");
        return nullptr;
    }

    // Skip RIFF chunk size (4 bytes)
    fileStream.ignore(4);

    // Read format id
    fileStream.read(chunkBuffer, 4);

    // Skip sub chunk ID
    fileStream.ignore(4);

    // Read format size
    fileStream.read(chunkBuffer, 4);
    int formatChunkSize = ConvertBinaryDataToInt(chunkBuffer, 4);

    // Read audio format
    fileStream.read(chunkBuffer, 2);
    int audioFormat = ConvertBinaryDataToInt(chunkBuffer, 2);

    // Read channels
    fileStream.read(chunkBuffer, 2);
    int numChannels = ConvertBinaryDataToInt(chunkBuffer, 2);

    // Read sample rate
    fileStream.read(chunkBuffer, 4);
    int sampleRate = ConvertBinaryDataToInt(chunkBuffer, 4);

    // Skip byte rate (4 bytes) and block align (2 bytes)
    fileStream.ignore(6);

    // Read bits per sample
    fileStream.read(chunkBuffer, 2);
    int bitsPerSample = ConvertBinaryDataToInt(chunkBuffer, 2);

    if (audioFormat != 1)
    {
        // Extra Params Size
        fileStream.read(chunkBuffer, 2);
        int extraParamsSize = ConvertBinaryDataToInt(chunkBuffer, 2);
        fileStream.read(chunkBuffer, extraParamsSize);
    }

    // Skip extra params if fmtChunkSize > 16
    if (formatChunkSize > 16)
    {
        fileStream.read(chunkBuffer, 2);
        int extraParamsSize = ConvertBinaryDataToInt(chunkBuffer, 2);
        fileStream.ignore(extraParamsSize);
    }

    // Search "data" chunk
    char* audioData = nullptr;
    int dataSize = 0;
    while (!fileStream.eof()) // While not at end of file
    {
        fileStream.read(chunkBuffer, 4);

        if (std::string(chunkBuffer, 4) == "data")
        {
            // Read data chunk size
            fileStream.read(chunkBuffer, 4);
            dataSize = ConvertBinaryDataToInt(chunkBuffer, 4);

            // Allocate memory and read audio data
            audioData = new char[dataSize];
            fileStream.read(audioData, dataSize);
            if (audioData == nullptr) 
            {
                printf("Failed to allocate memory for audio data.\n");
                return nullptr;
            }
            break;
        }
        else 
        {
            // Skip unknown chunk
            fileStream.read(chunkBuffer, 4);
            int chunkSize = ConvertBinaryDataToInt(chunkBuffer, 4);
            fileStream.ignore(chunkSize);
        }
    }

    // Create Buffer
    uint32_t bufferID;
    alGenBuffers(1, &bufferID);

    ALenum format;
    if (bitsPerSample == 8)
    {
        if (numChannels == 1)
        {
            format = AL_FORMAT_MONO8;
        }
        else
        {
            format = AL_FORMAT_STEREO8;
        }
    }
    if (bitsPerSample == 16)
    {
        if (numChannels == 1)
        {
            format = AL_FORMAT_MONO16;
        }
        else
        {
            format = AL_FORMAT_STEREO16;
        }
    }
    else
    {
        printf("Unsupported bits per sample: %d\n", bitsPerSample);
        delete[] audioData;
        return nullptr;
    }

    alBufferData(bufferID, format, audioData, dataSize, sampleRate);
    delete[] audioData;

    AudioBuffer* newAudioBuffer = new AudioBuffer(bufferID, format, 4, sampleRate);
    return newAudioBuffer;
}

int AudioBuffer::ConvertBinaryDataToInt(char* buffer, int length)
{
    int result = 0;

    for (int i = 0; i < length; i++)
    {
        result |= (static_cast<unsigned char>(buffer[i])) << (i * 8);
    }
    return result;
}
