#ifndef GRAPHICS_DEVICE
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>
#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>

#include "GraphicsRenderer.h"
#include "../smath/smath.h"

// Singleton Class that manages all stuff SDL_GPUDevice related
class GraphicsDevice
{
public:
    // Functions related to the instance of the singleton
    static GraphicsDevice* createInstance();
    static GraphicsDevice* getInstance();
    static void destroyInstance();

    // Initializing and Clean-up functions
    bool init(
        unsigned int width, unsigned int height, std::string windowName,
        std::string uiTextureAtlasCoords, std::string uiTextureAtlasImage,
        std::string textTextureAtlasCoords, std::string textTextureAtlasImage,
        float uiScale = 1.0f);
    void cleanup();

    // Getters
    SDL_GPUDevice* getDevice() { return mpDevice; }
    float getDisplayWidth() const { return mDisplayDimensions.x; }
    float getDisplayHeight() const { return mDisplayDimensions.y; }
    smath::vec2 getDisplayDimensions() const { return mDisplayDimensions; }
    SDL_Window* getWindow() const { return mpWindow; }
    GraphicsRenderer* getRenderer() const { return mpGraphicsRenderer; }
    smath::mat4 getProjectionMatrix() const;

    // Setters
    void setDisplayDimensions(const unsigned int& width, const unsigned int& height) { mDisplayDimensions = smath::vec2((float)width, (float)height); }
    void setDisplayDimensions(const float& width, const float& height) { mDisplayDimensions = smath::vec2(width, height); }
    void setDisplayDimensions(const smath::vec2& dimensions) { mDisplayDimensions = dimensions; }

    // Disallowing the copy constructor and assignment operator
    GraphicsDevice(GraphicsDevice& instance) = delete;
    void operator=(const GraphicsDevice& instance) = delete;

private:
    // Instance and thread-safety member variables
    static GraphicsDevice* mpInstance;
    static std::mutex mThread;
    static bool mInitialized;

    // Member Variables
    SDL_GPUDevice* mpDevice = nullptr;
    SDL_Window* mpWindow = nullptr;
    GraphicsRenderer* mpGraphicsRenderer = nullptr;
    smath::vec2 mDisplayDimensions;

    // Making the constructor & deconstructor private to enfore the singleton
    GraphicsDevice() {}
    ~GraphicsDevice() { cleanup(); }
};


#endif // !GRAPHICS_DEVICE
