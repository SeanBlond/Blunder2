#ifndef SHADERCROSS
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>
#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3_shadercross/SDL_shadercross.h>

// Singleton class meant to simplify the use of SDL_ShaderCross throughout the project
class ShaderCross
{
public:
    // Functions related to the instance of the singleton
    static ShaderCross* createInstance();
    static ShaderCross* getInstance();
    static void destroyInstance();

    // Initializing and Clean-up functions
    bool init();
    void cleanup();

    // Runtime Functions
    SDL_GPUShader* createShaderFromHLSL(SDL_GPUDevice* device, std::string filepath, SDL_ShaderCross_ShaderStage stage);
    SDL_GPUComputePipeline* createComputePipelineFromHLSL(SDL_GPUDevice* device, std::string filepath);

    // Disallowing the copy constructor and assignment operator
    ShaderCross(ShaderCross& instance) = delete;
    void operator=(const ShaderCross& instance) = delete;

private:
    // Instance and thread-safety member variables
    static ShaderCross* mpInstance;
    static std::mutex mThread;
    static bool mInitialized;


    // Making the constructor & deconstructor private to enfore the singleton
    ShaderCross() {}
    ~ShaderCross() { cleanup(); }
};

#endif // !SHADERCROSS