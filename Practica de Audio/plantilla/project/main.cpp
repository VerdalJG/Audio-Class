#include "../openal/AL/al.h"
#include "../openal/AL/alc.h"



#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#define LITE_GFX_IMPLEMENTATION
#include "litegfx.h"
#include <glfw3.h>
#include <sstream>
#include <vector>

#include <fstream>     
#include <iterator>

#include "AudioBuffer.h"
#include "AudioSource.h"
#include "Listener.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PI 3.14159265358979323846f

using namespace std;

AudioSource* source = nullptr;
ALCdevice* device = nullptr;
ALCcontext* context = nullptr;
GLFWwindow* window = nullptr;
GLFWwindow* windowContext = nullptr;
bool movingRight = false;
bool movingLeft = false;
float dopplerFactor = 1;

void InitializeOpenAL()
{
    // OpenAL variables
    device = alcOpenDevice(nullptr);
    context = nullptr;

    // Set the context
    if (device)
    {
        context = alcCreateContext(device, nullptr);
        alcMakeContextCurrent(context);
    }
}

void ProcessInputs(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_UP)
        {
            dopplerFactor += 0.1f;
        }
        if (key == GLFW_KEY_DOWN)
        {
            dopplerFactor -= 0.1f;
        }
        if (key == GLFW_KEY_LEFT)
        {
            movingLeft = true;
        }
        if (key == GLFW_KEY_RIGHT)
        {
            movingRight = true;
        }
    }
    if (action == GLFW_RELEASE)
    {
        if (key == GLFW_KEY_LEFT)
        {
            movingLeft = false;
        }
        if (key == GLFW_KEY_RIGHT)
        {
            movingRight = false;
        }
    }
}

void InitializeOpenGL()
{
    glfwInit();
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Audio #2", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    windowContext = glfwGetCurrentContext();
    lgfx_setup2d(WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetKeyCallback(window, ProcessInputs);
}

int main() 
{
    InitializeOpenAL();
    Listener& listener = Listener::GetInstance();

    AudioBuffer* audioBuffer = AudioBuffer::Load("data/engine.wav");
    
    if (audioBuffer)
    {
        source = new AudioSource(audioBuffer);
    }

    source->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0);
    source->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0);
    source->Play();
    source->SetLooping(true);

    InitializeOpenGL();

    double currentFrame = glfwGetTime();
    double lastFrame = currentFrame;
    double deltaTime = 0;

    float listenerRadiusCircle = 16;
    float listenerPositionX = WINDOW_WIDTH / 2;
    float listenerPositionY = WINDOW_HEIGHT - listenerRadiusCircle * 2;
    float listenerVelocityX = 0;
    float listenerVelocityY = 0;
    listener.GetInstance().SetListenerPosition(listenerPositionX, listenerPositionY, 0);
    listener.GetInstance().SetListenerVelocity(0, 0, 0);

    float audioAngle = 0;
    float sourcePositionX = WINDOW_WIDTH / 2;
    float sourcePositionY = WINDOW_HEIGHT / 4;

    while (!glfwWindowShouldClose(windowContext))
    {
        lgfx_clearcolorbuffer(0, 0, 0);	

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        std::string windowBaseString = "Audio #2 - ";

        if (movingRight)
        {
            //source->SetVelocity(100.0f, 0, 0);
            listener.GetInstance().SetListenerVelocity(100, 0, 0);
            listenerVelocityX = 100;

            std::string windowString = windowBaseString + "Moving Right - Doppler: " + std::to_string(dopplerFactor);
            glfwSetWindowTitle(windowContext, windowString.c_str());
        }
        if (movingLeft)
        {
            //source->SetVelocity(-100.0f, 0, 0);
            listener.GetInstance().SetListenerVelocity(-100, 0, 0);
            listenerVelocityX = -100;

            std::string windowString = windowBaseString + "Moving Left - Doppler: " + std::to_string(dopplerFactor);
            glfwSetWindowTitle(windowContext, windowString.c_str());
        }
        if (!movingLeft && !movingRight || movingLeft && movingRight)
        {
            //source->SetVelocity(0.0f, 0, 0);

            listener.GetInstance().SetListenerVelocity(0, 0, 0);
            listenerVelocityX = 0;

            std::string windowString = windowBaseString + "Not moving - Doppler: " + std::to_string(dopplerFactor);
            glfwSetWindowTitle(windowContext, windowString.c_str());
        }

        listener.GetInstance().Tick(deltaTime);
        listenerPositionX += listenerVelocityX * deltaTime;
        listenerPositionY += listenerVelocityY * deltaTime;
        lgfx_drawoval(listenerPositionX, listenerPositionY, listenerRadiusCircle, listenerRadiusCircle);
        
        //Movement of audio
        audioAngle += 32 * deltaTime * (PI / 180); // Convert to radians
        if (audioAngle >= 360)
        {
            audioAngle -= 360.0f;
        }
        sourcePositionX = WINDOW_WIDTH / 2 + cos(audioAngle) * WINDOW_HEIGHT / 4;
        sourcePositionY = WINDOW_HEIGHT / 2 + sin(audioAngle) * WINDOW_HEIGHT / 4;
        lgfx_drawoval(sourcePositionX, sourcePositionY, 10, 10);
        source->SetPosition(sourcePositionX, sourcePositionY, 0);

        //source->Tick(deltaTime);

        alDopplerFactor(dopplerFactor);

        //Change back buffers
        glfwSwapBuffers(windowContext);
        //Process Events
        glfwPollEvents();
    }

    glfwTerminate();

    alcDestroyContext(context);
    alcCloseDevice(device);

    return 0;
}
