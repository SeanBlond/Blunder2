#include "ShaderCross.h"

// * ---------------------------------------- *
// |  ShaderCross Class Function Definitions  |
// * ---------------------------------------- *

// Before those little function definitions, the static member variables must be initialized
ShaderCross* ShaderCross::mpInstance = nullptr;
bool ShaderCross::mInitialized = false;
std::mutex ShaderCross::mThread;

// Functions related to the instance of the singleton
ShaderCross* ShaderCross::createInstance()
{
    // If the instance already exists, destroy it so it can be recreated
    if (mpInstance)
        destroyInstance();

    // Locking the thread 
    std::lock_guard<std::mutex> lock(mThread);
    
    // Creating the instance
    mpInstance = new ShaderCross();

    // Returning the instance
    return mpInstance;
}
ShaderCross* ShaderCross::getInstance()
{
    // Creating the instance if it's not already
    if (!mpInstance)
        createInstance();

    // Returning the instance
    return mpInstance;
}
void ShaderCross::destroyInstance()
{
    // Deleting the instance and setting it to null
    delete mpInstance;
    mpInstance = nullptr;
}


// Initializing and Clean-up functions
bool ShaderCross::init()
{
    // Returning false if already initialized
    if (mInitialized)
        return false;

    // Initializing SDL_ShaderCross
    if (!SDL_ShaderCross_Init())
    {
        std::cout << "SDL_ShaderCross failed to Initialize" << std::endl;
        return false;
    }


    // Updating initialized member variable
    mInitialized = true;

    // Returning true for succesful initiailiztion
    return true;

}
void ShaderCross::cleanup()
{
    // Closing out of SDL_ShaderCross
    SDL_ShaderCross_Quit();

    // Reseting initialized bool
    mInitialized = false;
}


// Runtime Functions
SDL_GPUShader* ShaderCross::createShaderFromHLSL(SDL_GPUDevice* device, std::string filepath, SDL_ShaderCross_ShaderStage stage)
{
    // Returning out if the stage is computer
    if (stage == SDL_SHADERCROSS_SHADERSTAGE_COMPUTE)
        return nullptr;

    // Loading in the shader file
    std::ifstream shaderFile(filepath);

    // Checking if the shader file properly loaded, returning null if not
    if (!shaderFile)
    {
        std::cout << "Failed to load shader file found at path: " << filepath << std::endl;
        return nullptr;
    }

    // Loading the file into a stringstream so it can be converted to a string for the info to use
    std::stringstream shaderString;
    shaderString << shaderFile.rdbuf();
    std::string shaderSourceCode = shaderString.str();

    // Creating the information for the shader
    SDL_ShaderCross_HLSL_Info tempHLSLInfo =
    {
        .source = shaderSourceCode.c_str(),
        .entrypoint = "main",
        .include_dir = nullptr, // maybe switch in future to unified shader source folder? (prob would be smart)
        .defines = nullptr,
        .shader_stage = stage, // maybe have the system read automatically in the future?
    };

    // Converting the HLSL code (and info) into SPIRV bytecode
    size_t spirvCodeSize;
    void* spirvCode = SDL_ShaderCross_CompileSPIRVFromHLSL(&tempHLSLInfo, &spirvCodeSize);

    // Creating info for the SPIRV shader (deja vu)
    SDL_ShaderCross_SPIRV_Info tempSPIRVInfo =
    {
        .bytecode = static_cast<uint8_t*>(spirvCode),
        .bytecode_size = spirvCodeSize,
        .entrypoint = tempHLSLInfo.entrypoint,
        .shader_stage = stage,
    };

    // Creating shader resource info by reflecting the spirv bytecode
    SDL_ShaderCross_GraphicsShaderMetadata* tempMetadata = SDL_ShaderCross_ReflectGraphicsSPIRV(tempSPIRVInfo.bytecode, tempSPIRVInfo.bytecode_size, tempSPIRVInfo.props);

    // Returning null if the graphics shader could not be reflected
    if (!tempMetadata)
    {
        std::cout << "Failed to create a graphics shader from the file " << filepath << ", with errors: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    // Getting the resource info
    SDL_ShaderCross_GraphicsShaderResourceInfo tempShaderInfo = tempMetadata->resource_info;

    // Using the SPIRV info & code to create a dynamically allocated SDL_GPUShader!
    SDL_GPUShader* tempShader = SDL_ShaderCross_CompileGraphicsShaderFromSPIRV(
        device,
        &tempSPIRVInfo,
        &tempShaderInfo,
        0
    );

    // Freeing the SPIRV bytecode after use
    SDL_free(spirvCode);

    // Freeing the temporary metadata
    delete tempMetadata;
    tempMetadata = nullptr;

    // Checking if the shader was properly compiled
    if (!tempShader)
    {
        std::cout << "Failed to convert SPIRV bytecode to shader" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        return nullptr;
    }

    // Returning the properly compiled shader!
    return tempShader;
}
SDL_GPUComputePipeline* ShaderCross::createComputePipelineFromHLSL(SDL_GPUDevice* device, std::string filepath)
{
    // Loading in the shader file
    std::ifstream shaderFile(filepath);

    // Checking if the shader file properly loaded, returning null if not
    if (!shaderFile)
    {
        std::cout << "Failed to load shader file found at path: " << filepath << std::endl;
        return nullptr;
    }

    // Loading the file into a stringstream so it can be converted to a string for the info to use
    std::stringstream shaderString;
    shaderString << shaderFile.rdbuf();
    std::string shaderSourceCode = shaderString.str();

    // Creating the information for the shader
    SDL_ShaderCross_HLSL_Info tempHLSLInfo =
    {
        .source = shaderSourceCode.c_str(),
        .entrypoint = "main",
        .include_dir = nullptr, // maybe switch in future to unified shader source folder? (prob would be smart)
        .defines = nullptr,
        .shader_stage = SDL_SHADERCROSS_SHADERSTAGE_COMPUTE,
    };

    // Converting the HLSL code (and info) into SPIRV bytecode
    size_t spirvCodeSize;
    void* spirvCode = SDL_ShaderCross_CompileSPIRVFromHLSL(&tempHLSLInfo, &spirvCodeSize);

    // Creating info for the SPIRV shader (deja vu)
    SDL_ShaderCross_SPIRV_Info tempSPIRVInfo
    {
        .bytecode = static_cast<uint8_t*>(spirvCode),
        .bytecode_size = spirvCodeSize,
        .entrypoint = tempHLSLInfo.entrypoint,
        .shader_stage = SDL_SHADERCROSS_SHADERSTAGE_COMPUTE,
    };

    // Creating the pipeline info by reflecting the spirv bytecode
    SDL_ShaderCross_ComputePipelineMetadata* tempComputeMetadata = SDL_ShaderCross_ReflectComputeSPIRV(tempSPIRVInfo.bytecode, tempSPIRVInfo.bytecode_size, tempSPIRVInfo.props);

    // Returning null if the compute pipeline could not be reflected
    if (!tempComputeMetadata)
    {
        std::cout << "Failed to reflect compute metadata " << filepath << std::endl;
        return nullptr;
    }

    // Using the SPIRV info & code to create a dynamically allocated Compute Pipeline
    SDL_GPUComputePipeline* tempPipeline = SDL_ShaderCross_CompileComputePipelineFromSPIRV(device, &tempSPIRVInfo, tempComputeMetadata, 0);

    // Freeing the SPIRV bytecode after use
    SDL_free(spirvCode);

    // Freeing the temporary metadata
    delete tempComputeMetadata;
    tempComputeMetadata = nullptr;

    // Checking if the pipeline was properly compiled
    if (!tempPipeline)
    {
        std::cout << "Failed to convert SPIRV bytecode to pipeline" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        return nullptr;
    }

    // Returning the pipeline
    return tempPipeline;
}