#include "GraphicsDevice.h"

// * ---------------------------------------- *
// |  GraphicsDevice Class Function Definitions  |
// * ---------------------------------------- *

// Before those little function definitions, the static member variables must be initialized
GraphicsDevice* GraphicsDevice::mpInstance = nullptr;
bool GraphicsDevice::mInitialized = false;
std::mutex GraphicsDevice::mThread;

// Functions related to the instance of the singleton
GraphicsDevice* GraphicsDevice::createInstance()
{
    // If the instance already exists, destroy it so it can be recreated
    if (mpInstance)
        destroyInstance();

    // Locking the thread 
    std::lock_guard<std::mutex> lock(mThread);

    // Creating the instance
    mpInstance = new GraphicsDevice();

    // Returning the instance
    return mpInstance;
}
GraphicsDevice* GraphicsDevice::getInstance()
{
    // Creating the instance if it's not already
    if (!mpInstance)
        createInstance();

    // Returning the instance
    return mpInstance;
}
void GraphicsDevice::destroyInstance()
{
    // Deleting the instance and setting it to null
    delete mpInstance;
    mpInstance = nullptr;
}

// Getter Definitions
smath::mat4 GraphicsDevice::getProjectionMatrix() const
{
    float wS = 2.0f / mDisplayDimensions.x;
    float hS = 2.0f / mDisplayDimensions.y;
    smath::mat4 outputMatrix = smath::mat4({
        wS,   0.0f, 0.0f, -1.0f,
        0.0f, -hS,  0.0f,  1.0f,
        0.0f, 0.0f, 1.0f,  0.0f,
        0.0f, 0.0f, 0.0f,  1.0f
    });
    return outputMatrix;
}

// Initializing and Clean-up functions
bool GraphicsDevice::init(
    unsigned int width, unsigned int height, std::string windowName, 
    std::string uiTextureAtlasCoords, std::string uiTextureAtlasImage,
    std::string textTextureAtlasCoords, std::string textTextureAtlasImage,
    float uiScale)
{
    // Returning false if already initialized
    if (mInitialized)
        return false;

    //Initialize SDL Video
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "Could not initialize SDL3: " << SDL_GetError();
        return false;
    }

    // Creating the window
    mpWindow = SDL_CreateWindow(windowName.c_str(), width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    // Return false if the window is not initialized properly
    if (!mpWindow)
    {
        std::cout << "Failed to create Window with errors: " << SDL_GetError() << std::endl;
        return false;
    }

    // Creating the GPU Device dependent on the renderer
    mpDevice = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr);

    // Checking if the device was properly made
    if (!mpDevice)
    {
        std::cout << "Failed to create GPU Device with errors: " << SDL_GetError() << std::endl;
        return false;
    }

    // Claiming the window for the GPU device
    if (!SDL_ClaimWindowForGPUDevice(mpDevice, mpWindow))
    {
        std::cout << "Failed to claim the window for the GPU device with error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Setting width & height
    setDisplayDimensions(width, height);

    // Initializing the Graphics Renderer
    mpGraphicsRenderer = new GraphicsRenderer();
    if (!mpGraphicsRenderer->init(uiTextureAtlasCoords, uiTextureAtlasImage, textTextureAtlasCoords, textTextureAtlasImage, uiScale))
    {
        std::cout << "Failed to initialize the Graphics Renderer" << std::endl;
        return false;
    }

    // Updating initialized member variable
    mInitialized = true;

    // Returning true for succesful initiailiztion
    return true;

}
void GraphicsDevice::cleanup()
{
    // Deleting the Graphics Renderer
    delete mpGraphicsRenderer;
    mpGraphicsRenderer = nullptr;

    // Destroying the GPU Device and Window in proper order
    SDL_ReleaseWindowFromGPUDevice(mpDevice, mpWindow);
    SDL_DestroyWindow(mpWindow);
    SDL_DestroyGPUDevice(mpDevice);

    // Quitting SDL
    SDL_Quit();

    // Reseting initialized bool
    mInitialized = false;
}
