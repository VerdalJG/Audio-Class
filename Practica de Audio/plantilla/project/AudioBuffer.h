#pragma once
#include "../openal/AL/al.h"
#include <cstdint>

class AudioBuffer
{
public:
	AudioBuffer(ALuint buffer, ALenum format, ALsizei size, ALsizei frequency);
	~AudioBuffer();

	static AudioBuffer* Load(const char* filename);
    static int ConvertBinaryDataToInt(char* buffer, int length);

private:
    ALuint buffer;
    ALenum format;
    ALsizei size;
    ALsizei frequency;

public:
    uint32_t GetALBuffer() const { return buffer; }
};

