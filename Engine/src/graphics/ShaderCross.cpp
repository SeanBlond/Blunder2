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
HLSLCompilationResult ShaderCross::createShaderFromHLSLFile(SDL_GPUDevice* device, std::string filepath, SDL_ShaderCross_ShaderStage stage)
{
    // Loading in the shader file
    std::ifstream shaderFile(filepath);

    // Checking if the shader file properly loaded, returning null if not
    if (!shaderFile)
    {
        std::cout << "Failed to load shader file found at path: " << filepath << std::endl;
        return HLSLCompilationResult();
    }

    // Loading the file into a stringstream so it can be converted to a string for the info to use
    std::stringstream shaderString;
    shaderString << shaderFile.rdbuf();
    std::string shaderSourceCode = shaderString.str();

    // Calling creation function with the source code
    return createShaderFromHLSLCode(device, shaderSourceCode, stage);
}
HLSLCompilationResult ShaderCross::createShaderFromHLSLCode(SDL_GPUDevice* device, const std::string& shaderCode, SDL_ShaderCross_ShaderStage stage)
{
    // If the stage is compute, return null
    if (stage == SDL_SHADERCROSS_SHADERSTAGE_COMPUTE)
        return HLSLCompilationResult();

    // Creating the information for the shader
    SDL_ShaderCross_HLSL_Info tempHLSLInfo =
    {
        .source = shaderCode.c_str(),
        .entrypoint = "main",
        .include_dir = nullptr, // maybe switch in future to unified shader source folder? (prob would be smart)
        .defines = nullptr,
        .shader_stage = stage, // maybe have the system read automatically in the future?
    };

    // Converting the HLSL code (and info) into SPIRV bytecode
    size_t spirvCodeSize;
    void* spirvCode = SDL_ShaderCross_CompileSPIRVFromHLSL(&tempHLSLInfo, &spirvCodeSize);

    // Checking if the compilation went wrong
    if (!spirvCode)
    {
        // Creating output struct
        HLSLCompilationResult result =
        {
            .mpCompiledShader = nullptr,
            .mErrors = parseErrorsAfterFailure()
        };
        return result;
    }

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
        std::cout << "Failed to create a graphics shader with errors: " << SDL_GetError() << std::endl;
        return HLSLCompilationResult();
    }

    // Getting the resource info
    SDL_ShaderCross_GraphicsShaderResourceInfo tempShaderInfo = tempMetadata->resource_info;

    // Using the SPIRV info & code to create a dynamically allocated SDL_GPUShader!
    SDL_GPUShader* tempShader = SDL_ShaderCross_CompileGraphicsShaderFromSPIRV(device, &tempSPIRVInfo, &tempShaderInfo, 0);

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
        return HLSLCompilationResult();
    }

    // Creating output struct
    HLSLCompilationResult result =
    {
        .mpCompiledShader = tempShader,
    };
    return result;
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

// Private error parsing function definition
std::vector<HLSLError> ShaderCross::parseErrorsAfterFailure()
{
    // Obtainting the error message
    std::string errorMessage = SDL_GetError();

    // Checking if the message starts with "HLSL compilation failed: "
    if (errorMessage.substr(0, 25) != "HLSL compilation failed: ")
    {
        // Contains no hlsl compilation errors, return blank
        return {};
    }

    // Defining a vector of errors that will be returned
    std::vector<HLSLError> errors;

    // Creating a readble string stream from the clipped error message
    std::stringstream errorStream(errorMessage.substr(25));

    // Looping through each line of the error stream a max of 10 times
    for (int i = 0; i < 10; i++)
    {

        // Reading the line from the stream
        std::string tempString = "";
        std::getline(errorStream, tempString);

        // Checking if the line means that the loop should quit
        if (tempString == "" || tempString == "\n")
            break;

        // Checking if the line starts with the hlsl error code
        if (tempString.substr(0, 8) == "hlsl.hlsl:")
            continue;

        // Getting the line info using regex
        tempString = tempString.substr(8);
        std::regex errorPattern(R"(:([0-9]+):([0-9]+):\s*error:\s*(.*))");
        std::smatch errorMatches;

        if (std::regex_search(tempString, errorMatches, errorPattern))
        {
            errors.push_back(
            {
                .mLine = std::stoi(errorMatches[1].str()),
                .mCharacter = std::stoi(errorMatches[2].str()),
                .mErrorMessage = errorMatches[3].str()
            });
        }
    }

    // Adding an HLSLError for each error observed in the SDL_Error
    return errors;
}

// Function definition for error output handling
std::string HLSLError::outputError()
{
    std::string errorString = "ERROR AT LINE " + std::to_string(mLine) + ", CHAR " + std::to_string(mCharacter) + ": " + mErrorMessage + '\n';
    return errorString;
}

std::string HLSLCompilationResult::outputCompilationErrors()
{
    // Adding each compilation error to a string
    std::string outputString = "\n";
    for (HLSLError error : mErrors)
    {
        outputString += error.outputError();
    }
    return outputString;
}