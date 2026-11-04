#include "MaterialManager.h"
#include "../graphics/GraphicsDevice.h"
#include "../graphics/ShaderCross.h"

// Data management function definitions
bool MaterialManager::init(std::string materialsFilepath)
{
    // Creating a basic pipeline that can be used if materials fail to create proper pieplines
    mpBasicPipeline = generateBasicPipeline();
    
    // If the basic pipeline failed to ghenerate, return false
    if (!mpBasicPipeline)
        return false;

    // Storing the filepath
    mMaterialsFilepath = materialsFilepath;

    // Loading in the files
    loadMaterialsFromFile();

	
	// Creating a bsic material and adding some random uniforms
	Material* tempMaterial = new Material("TestMaterial", "resources/Materials/Shaders/TestMaterial.hlsl");
	tempMaterial->addUniform(new IntUniform("TestInt", 10));
	tempMaterial->addUniform(new FloatUniform("TestFloat", 3.14159f));
	tempMaterial->addUniform(new Float2Uniform("TestFloat2", smath::vec2(1.0f, 2.0f)));
	tempMaterial->loadShadercodeFile();
	tempMaterial->generateGraphicsPipeline();
	mpMaterialsMap.insert({ 1, tempMaterial });


    // Return true for succesful initilization
    return true;
}
void MaterialManager::cleanup()
{
    // Looping through all materials in the map and deallocating them
    for (auto material : mpMaterialsMap)
    {
        delete material.second;
        material.second = nullptr;
    }
    mpMaterialsMap.clear();

	// Releasing basic graphics pipeline
	SDL_ReleaseGPUGraphicsPipeline(GraphicsDevice::getInstance()->getDevice(), mpBasicPipeline);
}
SDL_GPUGraphicsPipeline* MaterialManager::generateBasicPipeline()
{
	// Creating the vertex shader
	SDL_GPUShader* vertShader = ShaderCross::getInstance()->createShaderFromHLSLCode(GraphicsDevice::getInstance()->getDevice(), Material::getBaseVertexCode(), SDL_SHADERCROSS_SHADERSTAGE_VERTEX);
	if (!vertShader)
	{
		std::cout << "Failed to create Material Manager base vertex shader with errors: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	// Creating the fragment shader
	SDL_GPUShader* fragShader = ShaderCross::getInstance()->createShaderFromHLSLCode(GraphicsDevice::getInstance()->getDevice(), FRAGMENT_SHADER_CODE, SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT);
	if (!fragShader)
	{
		std::cout << "Failed to create Material Manager base fragment shader with errors: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	// Defining the Vertex Buffer Expectations
	SDL_GPUVertexBufferDescription vertexBufferDesc[] =
	{
		{
			.slot = 0,
			.pitch = sizeof(PosTexVert),
			.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
			.instance_step_rate = 0,
		}
	};

	// Defining the vertex attributes
	SDL_GPUVertexAttribute vertexAttributes[] =
	{
		{
			.location = 0,
			.buffer_slot = 0,
			.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
			.offset = 0
		},
		{
			.location = 1,
			.buffer_slot = 0,
			.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
			.offset = offsetof(PosTexVert, texCoord)
		}
	};

	// Defining the color targets
	SDL_GPUColorTargetDescription colorTargetDescriptions[] =
	{
		{
			.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
			.blend_state =
			{
				.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
				.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
				.color_blend_op = SDL_GPU_BLENDOP_ADD,
				.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
				.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
				.alpha_blend_op = SDL_GPU_BLENDOP_ADD,
				.color_write_mask = 0xF,
				.enable_blend = true
			}
		}
	};

	// Creating the name for the graphics pipeline
	SDL_PropertiesID pipelineProps = SDL_CreateProperties();
	std::string pipelineName = "MaterialManager_Base_Graphics_Pipeline";
	SDL_SetStringProperty(pipelineProps, SDL_PROP_GPU_GRAPHICSPIPELINE_CREATE_NAME_STRING, pipelineName.c_str());

	// Creating the pipeline
	SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo = {
		.vertex_shader = vertShader,
		.fragment_shader = fragShader,
		.vertex_input_state = SDL_GPUVertexInputState{
			.vertex_buffer_descriptions = vertexBufferDesc,
			.num_vertex_buffers = 1,
			.vertex_attributes = vertexAttributes,
			.num_vertex_attributes = 2
		},
		.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
		.depth_stencil_state =
		{
			.compare_op = SDL_GPU_COMPAREOP_GREATER_OR_EQUAL,
			.enable_depth_test = true,
			.enable_depth_write = true,
		},
		.target_info =
		{
			.color_target_descriptions = colorTargetDescriptions,
			.num_color_targets = 1,
			.depth_stencil_format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT,
			.has_depth_stencil_target = true
		},
		.props = pipelineProps,
	};

	SDL_GPUGraphicsPipeline* tempPipeline = SDL_CreateGPUGraphicsPipeline(GraphicsDevice::getInstance()->getDevice(), &pipelineCreateInfo);
	if (!tempPipeline)
	{
		std::cout << "Failed to create pipeline with errors: " << SDL_GetError() << std::endl;
	}

	// Freeing the props
	SDL_DestroyProperties(pipelineProps);

	// Releasing shaders
	SDL_ReleaseGPUShader(GraphicsDevice::getInstance()->getDevice(), vertShader);
	SDL_ReleaseGPUShader(GraphicsDevice::getInstance()->getDevice(), fragShader);

	// Returning the pipeline
	return tempPipeline;
}

// Material loading & saving definitions
void MaterialManager::loadMaterialsFromFile()
{

}
void MaterialManager::saveMaterialsToFile()
{

}

// Materials map function definitions
void MaterialManager::addMaterial(Material* newMat)
{
    // If the material is null, return out
    if (!newMat)
        return;

    // Generating a new key for the material
    int newKey = generateNewKey();

    // If the key isn't -1, add the material
    if (newKey != -1)
        mpMaterialsMap.insert({ newKey, newMat });
}
Material* MaterialManager::getMaterial(int matKey)
{
    // Finding the material (if it exists)
    auto material = mpMaterialsMap.find(matKey);
    
    // If the material doesn't exist, return null
    if (material == mpMaterialsMap.end())
        return nullptr;

    // Returnning the succesfully found material
    return material->second;
}

// Function definition for generating unique keys for newly created materials
int MaterialManager::generateNewKey()
{
    // Looping until a random number that isn't already used or isn't -1
    // After 100 iterations, the function will return -1, which means a key couldn't be found
    for (int i = 0; i < 100; i++)
    {
        // Generating random number
        int potentialKey = std::rand();

        // Checking random number
        if (!(mpMaterialsMap.contains(potentialKey)) && potentialKey != -1)
            return potentialKey;
    }

    // Returning -1
    return -1;
}