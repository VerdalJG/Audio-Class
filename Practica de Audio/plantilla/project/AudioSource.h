#pragma once
#include "../openal/AL/al.h"

class AudioBuffer;

class AudioSource
{
public:
	AudioSource(AudioBuffer* buffer);
	~AudioSource();

	void Tick(float deltaTime);

	void Play();
	void Stop();
	void Pause();

private:
	ALuint source = 0;
	float pitch = 0.0f;

	float gain = 0.0f;

	float positionX = 0.0f;
	float positionY = 0.0f;
	float positionZ = 0.0f;

	float velocityX = 0.0f;
	float velocityY = 0.0f;
	float velocityZ = 0.0f;

	bool isPlaying = false;
	bool isLooping = false;
	
public:
	float GetPitch() const { return pitch; }
	void SetPitch(float pitch);

	float GetGain() const { return gain; }
	void SetGain(float gain);

	float GetLooping() const { return isLooping; }
	void SetLooping(bool isLooping);

	float GetPositionX() const { return positionX; }
	float GetPositionY() const { return positionY; }
	float GetPositionZ() const { return positionZ; }
	void SetPosition(float x, float y, float z);

	float GetVelocityX() const { return velocityX; }
	float GetVelocityY() const { return velocityY; }
	float GetVelocityZ() const { return velocityZ; }
	void SetVelocity(float x, float y, float z);

	bool IsPlaying() { return isPlaying; }
};

