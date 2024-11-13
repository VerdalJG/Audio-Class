#include "AudioSource.h"
#include "AudioBuffer.h"

AudioSource::AudioSource(AudioBuffer* buffer)
{
	alGenSources(1, &source);
	SetLooping(0);
	SetPitch(1);
	SetGain(1);
	SetPosition(0, 0, 0);
	SetVelocity(0, 0, 0);

	alSourcei(source, AL_BUFFER, buffer->GetALBuffer());
}

AudioSource::~AudioSource()
{
}

void AudioSource::Tick(float deltaTime)
{
	float newX = positionX + velocityX * deltaTime;
	float newY = positionY + velocityY * deltaTime;
	float newZ = positionZ + velocityZ * deltaTime;

	SetPosition(newX, newY, newZ);
}

void AudioSource::Play()
{
	alSourcePlay(source);
	isPlaying = true;
}

void AudioSource::Stop()
{
	alSourceStop(source);
	isPlaying = false;
}

void AudioSource::Pause()
{
	alSourcePause(source);
	isPlaying = false;
}

void AudioSource::SetPitch(float pitch)
{
	this->pitch = pitch; 
	alSourcef(source, AL_PITCH, pitch);
}

void AudioSource::SetGain(float gain)
{
	this->gain = gain;
	alSourcef(source, AL_GAIN, gain);
}

void AudioSource::SetLooping(bool isLooping)
{
	this->isLooping = isLooping;
	alSourcei(source, AL_LOOPING, isLooping);
}

void AudioSource::SetPosition(float x, float y, float z)
{
	positionX = x;
	positionY = y;
	positionZ = z;
	alSource3f(source, AL_POSITION, positionX, positionY, positionZ);
}

void AudioSource::SetVelocity(float x, float y, float z)
{
	velocityX = x;
	velocityY = y;
	velocityZ = z;
	alSource3f(source, AL_VELOCITY, velocityX, velocityY, velocityZ);
}
