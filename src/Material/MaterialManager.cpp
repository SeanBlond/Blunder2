#include "MaterialManager.h"
#include "../graphics/GraphicsDevice.h"
#include "../graphics/ShaderCross.h"

// Data management function definitions
bool MaterialManager::init(const std::string& materialsFilepath)
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

    // Return true for succesful initilization
    return true;
}
void MaterialManager::cleanup()
{
	// Saving the materials
	saveMaterialsToFile();

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
	HLSLCompilationResult vertShader = ShaderCross::getInstance()->createShaderFromHLSLCode(GraphicsDevice::getInstance()->getDevice(), Material::getBaseVertexCode(), SDL_SHADERCROSS_SHADERSTAGE_VERTEX);
	if (!(vertShader.shaderExists()))
	{
		std::cout << "Failed to create Material Manager base vertex shader with errors: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	// Creating the fragment shader
	HLSLCompilationResult fragShader = ShaderCross::getInstance()->createShaderFromHLSLCode(GraphicsDevice::getInstance()->getDevice(), FRAGMENT_SHADER_CODE, SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT);
	if (!(fragShader.shaderExists()))
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
		.vertex_shader = vertShader.mpCompiledShader,
		.fragment_shader = fragShader.mpCompiledShader,
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
	SDL_ReleaseGPUShader(GraphicsDevice::getInstance()->getDevice(), vertShader.mpCompiledShader);
	SDL_ReleaseGPUShader(GraphicsDevice::getInstance()->getDevice(), fragShader.mpCompiledShader);

	// Returning the pipeline
	return tempPipeline;
}

// Material loading & saving definitions
void MaterialManager::loadMaterialsFromFile()
{
	// Attempting to load the materials filepath
	std::ifstream materialFile(mMaterialsFilepath);

	// Checking if the material filepath exists
	if (!materialFile)
	{
		std::cout << "Materials file does not exist, could not load any :(" << std::endl;
		return;
	}

	// File succesfully loaded, looping through it to load materials
	while (!(materialFile.eof()))
	{
		// Trying to read a material key
		std::string tempString;
		materialFile >> tempString;
		uint16_t readValue;

		// Reading the int value
		try
		{
			readValue = (uint16_t)std::stoi(tempString);
		}
		catch (const std::invalid_argument& e)
		{
			std::cout << "Materials file improperly formatted, resulting in error: " << e.what() << std::endl;
			break;
		}
		catch (const std::out_of_range& e)
		{
			std::cout << "Materials file improperly formatted, resulting in error: " << e.what() << std::endl;
			break;
		}

		// Finding the material filepath
		std::string materialFilepath;
		std::getline(materialFile, materialFilepath);
		materialFilepath = materialFilepath.substr(1);

		// Checking if the key already exists
		if (mpMaterialsMap.find(readValue) != mpMaterialsMap.end())
			continue; // Key already used, skip over this material

		// Otherwise, create a new material and (should it not be null) add it to the map
		Material* newMaterial = createMaterialFromFilepath(materialFilepath);
		if (!newMaterial) { continue; }
		newMaterial->generateGraphicsPipeline();
		mpMaterialsMap.insert({ readValue, newMaterial });
	}

	// Closing the file
	materialFile.close();
}
void MaterialManager::saveMaterialsToFile()
{
	// Creating a file to save the materials & keys to
	std::ofstream outputFile(mMaterialsFilepath);

	// Looping through each material & key and saving it to the file (as well as saving the individual materials to their files)
	for (auto pair : mpMaterialsMap)
	{
		// Saving the material
		pair.second->saveMaterialToFile();

		// Storing the material map data
		outputFile << pair.first << ' ' << pair.second->getMaterialFilepath() << std::endl;
	}
}
Material* MaterialManager::createMaterialFromFilepath(const std::string& filepath)
{
	// Creating a filestream from the filepath
	std::ifstream inputFile(filepath);

	// If file is invalid, return null
	if (!inputFile)
	{
		std::cout << "Could not find filepath \"" << filepath << '\"' << std::endl;
		return nullptr;
	}

	// Otherwise, try to read name
	std::string tempString;
	inputFile >> tempString;
	if (tempString != "MATERIAL")
	{
		std::cout << "Material found at \"" << filepath << "\" is improperly formatted" << std::endl;
		inputFile.close();
		return nullptr;
	}

	// Storing material name
	std::string materialName;
	std::getline(inputFile, materialName);
	materialName = materialName.substr(1);

	// Trying to read shadercode filepath
	inputFile >> tempString;
	if (tempString != "SHADERCODE")
	{
		std::cout << "Material found at \"" << filepath << "\" is improperly formatted" << std::endl;
		inputFile.close();
		return nullptr;
	}

	// Storing material name
	std::string shadercodePath;
	std::getline(inputFile, shadercodePath);
	shadercodePath = shadercodePath.substr(1);

	// Creating a material with this basic information
	Material* newMaterial = new Material(materialName, shadercodePath, filepath);

	// Looping until the end of the file to add uniforms
	while (!(inputFile.eof()))
	{
		// Reading the start of a uniform
		inputFile >> tempString;
		if (tempString != "UNIFORM")
			continue;

		// Trying to read uniform type and name
		std::string uniformType, uniformName;
		inputFile >> uniformType;
		inputFile >> uniformName;

		// Trying to read uniform values
		if (uniformType == "INT")
		{
			// Attempting to read the integer value
			inputFile >> tempString;
			int readValue;

			try
			{
				readValue = std::stoi(tempString);
			}
			catch (const std::invalid_argument& e)
			{
				std::cout << "Uniform \"" << uniformName << "\" was not formatted properly resulting in error: " << e.what() << std::endl;
				continue;
			}
			catch (const std::out_of_range& e)
			{
				std::cout << "Uniform \"" << uniformName << "\" was not formatted properly resulting in error: " << e.what() << std::endl;
				continue;
			}
		}
		else if (uniformType.substr(0, 5) == "FLOAT")
		{
			// Trying to read the integer value of the float number
			int floatNum;
			try { floatNum = std::stoi(uniformType.substr(5, 1)); }
			catch (const std::invalid_argument& e) { continue; }
			catch (const std::out_of_range& e) { continue; }
			if (floatNum < 1 || floatNum > 4) { continue; }

			std::vector<float> floatValues;
			for (int i = 0; i < floatNum; i++)
			{
				float readValue;
				inputFile >> tempString;
				try
				{
					readValue = std::stof(tempString);
				}
				catch (const std::invalid_argument& e)
				{
					std::cout << "Uniform \"" << uniformName << "\" was not formatted properly resulting in error: " << e.what() << std::endl;
					continue;
				}
				catch (const std::out_of_range& e)
				{
					std::cout << "Uniform \"" << uniformName << "\" was not formatted properly resulting in error: " << e.what() << std::endl;
					continue;
				}
				floatValues.push_back(readValue);
			}

			// If the number of floats added doesn't equal the expected number, skip!
			if (floatValues.size() != floatNum)
				continue;

			// Otherwise, create a new uniform from the float values
			switch (floatNum)
			{
			case 1:
				newMaterial->addUniform(new FloatUniform(uniformName, floatValues[0]));
				break;
			case 2:
				newMaterial->addUniform(new Float2Uniform(uniformName, { floatValues[0], floatValues[1] }));
				break;
			case 3:
				newMaterial->addUniform(new Float3Uniform(uniformName, { floatValues[0], floatValues[1], floatValues[2] }));
				break;
			case 4:
				newMaterial->addUniform(new Float4Uniform(uniformName, { floatValues[0], floatValues[1], floatValues[2], floatValues[3] }));
				break;
			}
		}
		else if (uniformType == "COLOR")
		{
			std::vector<float> floatValues;
			for (int i = 0; i < 4; i++)
			{
				float readValue;
				try
				{
					readValue = std::stof(tempString);
				}
				catch (const std::invalid_argument& e)
				{
					std::cout << "Uniform \"" << uniformName << "\" was not formatted properly resulting in error: " << e.what() << std::endl;
					continue;
				}
				catch (const std::out_of_range& e)
				{
					std::cout << "Uniform \"" << uniformName << "\" was not formatted properly resulting in error: " << e.what() << std::endl;
					continue;
				}
				floatValues.push_back(readValue);
			}

			// If the number of floats added doesn't equal the expected number, skip!
			if (floatValues.size() != 4)
				continue;

			// Otherwise, create a color uniform from the read values
			newMaterial->addUniform(new ColorUniform(uniformName, { floatValues[0], floatValues[1], floatValues[2], floatValues[3] }));
		}
	}

	// Closing the input file
	inputFile.close();

	// Returning the new material
	return newMaterial;
}

// Materials map function definitions
void MaterialManager::addMaterial(Material* newMat)
{
    // If the material is null, return out
    if (!newMat)
        return;

    // Generating a new key for the material
	int newKey = generateNewKey();

    // If the key isn't 0, add the material
    if (newKey != 0)
        mpMaterialsMap.insert({ (uint16_t)newKey, newMat });
}
Material* MaterialManager::getMaterial(uint16_t matKey)
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
uint16_t MaterialManager::generateNewKey()
{
    // Looping until a random number that isn't already used or isn't 0
    // After 100 iterations, the function will return 0, which means a key couldn't be found
    for (int i = 0; i < 100; i++)
    {
        // Generating random number
        int potentialKey = (uint16_t)std::rand();

        // Checking random number
        if (!(mpMaterialsMap.contains(potentialKey)) && potentialKey != 0)
            return potentialKey;
    }

    // Returning 0
    return 0;
}