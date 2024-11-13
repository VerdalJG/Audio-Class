#include "Listener.h"
#include "../openal/AL/al.h"

Listener& Listener::GetInstance()
{
    static Listener instance;
    return instance;
}

void Listener::Tick(float deltaTime)
{
    float newX = positionX + velocityX * deltaTime;
    float newY = positionY + velocityY * deltaTime;
    float newZ = positionZ + velocityZ * deltaTime;

    SetListenerPosition(newX, newY, newZ);
}

void Listener::SetListenerPosition(float x, float y, float z)
{
    alListener3f(AL_POSITION, x, y, z);
    positionX = x;
    positionY = y;
    positionZ = z;
}

void Listener::SetListenerVelocity(float x, float y, float z)
{
    alListener3f(AL_VELOCITY, x, y, z);
    velocityX = x;
    velocityY = y;
    velocityZ = z;
}

void Listener::SetListenerOrientation(float x, float y, float z)
{
    alListener3f(AL_ORIENTATION, x, y, z);
    facingX = x;
    facingY = y;
    facingZ = z;
}
