#include "Material.h"
#include "../graphics/ShaderCross.h"
#include "../graphics/GraphicsDevice.h"

// Constructor & Deconstructor definitions
Material::Material(std::string materialName, std::string codeFilepath)
{
	// Storing material data
	this->mMaterialName = materialName;
	this->mCodeFilepath = codeFilepath;
}
Material::~Material()
{
    // Deallocating all uniforms
    for (Uniform* uniform : mpUniforms)
    {
        delete uniform;
        uniform = nullptr;
    }
    mpUniforms.clear();

	// Deallocating the graphics pipeline
	SDL_ReleaseGPUGraphicsPipeline(GraphicsDevice::getInstance()->getDevice(), mpPipeline);
}

// Shadercode filepath functions
void Material::loadShadercodeFile()
{
	// If the code filepath can't be opened, create a new one
	std::filesystem::path tempPath(mCodeFilepath);
	if (std::filesystem::exists(tempPath))
	{
		// Shadercode file exists, return out!
		return;
	}


	// Defining a propper filepath
	std::string outputFilepath = (mCodeFilepath == "") ? ("resources/Materials/Shaders/" + mMaterialName + ".hlsl") : mCodeFilepath;

	// Shadercode file doesn't exist, start creating a new one
	std::cout << "Shadercode Filepath \"" << mCodeFilepath << "\" does not exist, creating a new one at " << outputFilepath << std::endl;
	mCodeFilepath = outputFilepath;

	// Creating the ostream file
	std::ofstream shadercodeFile(outputFilepath);

	// Outputting the blurb for the material name and available uniforms
	shadercodeFile << "// Shadercode for Material: " << mMaterialName << std::endl;
	shadercodeFile << R"(
// Input Struct contains:
// float3 input.Position, world position of the object
// float2 input.TexCoord, texture coordinate of the object
)" << std::endl;
	shadercodeFile << "// Available Uniforms:" << std::endl;
	for (Uniform* uniform : mpUniforms)
	{
		shadercodeFile << "// " << UniformTypeToString(uniform->getType()) << ' ' << uniform->getUniformName() << ";\n";
	}
	shadercodeFile << std::endl;

	shadercodeFile << "// Main Function" << std::endl <<
		"float4 mainColor(Input input) \n{\n    return float4(1.0, 1.0, 1.0, 1.0);\n};" << std::endl;
}

// Uniforms Management function definitions
void Material::addUniform(Uniform* uniform)
{
    mpUniforms.push_back(uniform);
}
void Material::replaceUniformAt(UniformType newType, int index)
{
	// Do nothing if invalid index
	if (index < 0 || index >= mpUniforms.size())
		return;

	// Removing the old one
	std::string uniformName = mpUniforms[index]->getUniformName();
	mpUniforms.erase(mpUniforms.begin() + index);

	// Adding the new uniform
    mpUniforms.insert(mpUniforms.begin() + index, newUniformFromType(newType, uniformName));
}
int Material::getUniformCount() const
{
    return mpUniforms.size();
}
Uniform* Material::getUniformAtIndex(const int& index)
{
    // Returning null if index is out of range
    if (index < 0 || index >= mpUniforms.size())
        return nullptr;

    return mpUniforms[index];
}

// Graphics Pipeline function definitions
SDL_GPUShader* Material::createFragmentShader()
{
    // Defining the stringstream that will contain the shader code
    std::stringstream shaderCodeStream;
    
    // Adding the basic input code
    shaderCodeStream <<
R"(
struct Input
{
    float4 ScreenPosition : SV_Position;
    float3 Position : POSITION0;
    float2 TexCoord : TEXCOORD0;
};
)" << std::endl;

    // If there are any uniforms, add them as a uniform buffer
    if (mpUniforms.size() != 0)
    {
        // Adding the buffer to the stream
        shaderCodeStream << "cbuffer UBO : register(b0, space3) \n{\n";

		// Creating an unordedred set to keep track of the uniforms added, so there can be no repeats
		std::unordered_set<std::string> usedUniformNames;

        // Adding each uniform
        for (Uniform* uniform : mpUniforms)
        {
			// Checking if the uniform is equivalent to one already used
			if (usedUniformNames.count(uniform->getUniformName()))
			{
				// Equuivalent uniform already used, skipping over it
				std::cout << "The uniform name \"" << uniform->getUniformName() << "\" has already been used" << std::endl;
				continue;
			}
			else 
			{
				// Uniform equivalent not used, add it to the set
				usedUniformNames.insert(uniform->getUniformName());
			}

			// Adding the uniformto the shader code
			shaderCodeStream << '\t' << UniformTypeToString(uniform->getType()) << ' ' << uniform->getUniformName() << ";\n";
		}

		// Clearing the set
		usedUniformNames.clear();

        // Closing the buffer
        shaderCodeStream << "};\n";
    }

	// Loading in the shader code file
	std::ifstream userShaderFile(mCodeFilepath);

	// Checking if the shader file properly loaded, returning null if not
	if (!userShaderFile)
	{
		std::cout << "Failed to load user shader code found at path: " << mCodeFilepath << std::endl;
		return nullptr;
	}

	// Loading the file intyo the shadercode string stream
	shaderCodeStream << userShaderFile.rdbuf();

    // Adding in the propper main function
    shaderCodeStream <<
R"(
float4 main(Input input) : SV_Target0
{
    float4 shaderOutput = mainColor(input);
    return shaderOutput;
};
)";

    // Creating a pipeline from the code
    std::string sourceCode = shaderCodeStream.str();
    SDL_GPUShader* fragShader = ShaderCross::getInstance()->createShaderFromHLSLCode(GraphicsDevice::getInstance()->getDevice(), sourceCode, SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT);

    // TODO: Add output for errors in code if there are any

    // Returning the shader (whether it be null or not)
    return fragShader;
}
bool Material::generateGraphicsPipeline()
{
	// Deallocating the current pipeline if it exists
	if (mpPipeline)
	{
		SDL_ReleaseGPUGraphicsPipeline(GraphicsDevice::getInstance()->getDevice(), mpPipeline);
		mpPipeline = nullptr;
	}

    // Creating the fragment shader
    SDL_GPUShader* fragShader = createFragmentShader();
    if (!fragShader)
    {
        return false;
    }

    // Creating the vertex shader
    SDL_GPUShader* vertShader = ShaderCross::getInstance()->createShaderFromHLSLCode(GraphicsDevice::getInstance()->getDevice(), VERTEX_SHADER_CODE, SDL_SHADERCROSS_SHADERSTAGE_VERTEX);
    if (!vertShader)
    {
        std::cout << "Failed to create vertex shader with errors: " << SDL_GetError() << std::endl;
        return false;
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
	std::string pipelineName = mMaterialName + "_Graphics_Pipeline";
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

	mpPipeline = SDL_CreateGPUGraphicsPipeline(GraphicsDevice::getInstance()->getDevice(), &pipelineCreateInfo);
	if (!mpPipeline)
	{
		std::cout << "Failed to create pipeline with errors: " << SDL_GetError() << std::endl;
	}

	// Freeing the props
	SDL_DestroyProperties(pipelineProps);

	// Releasing shaders
	SDL_ReleaseGPUShader(GraphicsDevice::getInstance()->getDevice(), vertShader);
	SDL_ReleaseGPUShader(GraphicsDevice::getInstance()->getDevice(), fragShader);

	return (mpPipeline != nullptr);
}
bool Material::bind(SDL_GPUCommandBuffer* commandBuffer, SDL_GPURenderPass* renderPass)
{
	// If the pipeline is invalid, return false for failed binding
	if (!mpPipeline)
		return false;

	// Binding the graphics pipeline
	SDL_BindGPUGraphicsPipeline(renderPass, mpPipeline);

	// Calculating the total size of all the uniforms
	uint32_t totalUniformSize = 0;
	for (Uniform* uniform : mpUniforms)
	{
		// If uniform is invalid, skip!
		if (!uniform)
			continue;

		// Adding the size
		totalUniformSize += uniform->getSize();
	}

	// Reserving the space for the uniforms
	uint8_t* uniformData = new uint8_t[totalUniformSize];

	// Writing each of the uniform's data into the memory
	uint32_t byteOffset = 0;
	for (Uniform* uniform : mpUniforms)
	{
		// If uniform is invalid, skip!
		if (!uniform)
			continue;

		// Copying the memory
		SDL_memcpy(uniformData + byteOffset, uniform->getData(), uniform->getSize());

		// Adding the byte offset
		byteOffset += uniform->getSize();
	}

	// Sending the data to the gpu
	SDL_PushGPUFragmentUniformData(commandBuffer, 0, uniformData, totalUniformSize);

	// Deallocating the uniform data
	delete[] uniformData;
	uniformData = nullptr;

	// Return true for succesful binding!
	return true;
}