#pragma once
class Listener
{
public:
    static Listener& GetInstance();

    void Tick(float deltaTime);

    void SetListenerPosition(float x, float y, float z);
    void SetListenerVelocity(float x, float y, float z);
    void SetListenerOrientation(float x, float y, float z);

private:
    float positionX; 
    float positionY;
    float positionZ;

    float velocityX; 
    float velocityY;
    float velocityZ;

    float facingX; 
    float facingY; 
    float facingZ;
};

