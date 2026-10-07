#include "GameObjectBuffer.h"
#include "../GraphicsDevice.h"
#include "../ShaderCross.h"

// Data Management Function definitions
bool GameObjectBuffer::init()
{
	// * ------------------------ *
	// |  Temp Graphics Pipeline  |
	// * ------------------------ *

	// Createing the vertex shader
	SDL_GPUShader* tempVertexShader = ShaderCross::getInstance()->createShaderFromHLSL(GraphicsDevice::getInstance()->getDevice(), "resources/shaders/GameObject.vert.hlsl", SDL_SHADERCROSS_SHADERSTAGE_VERTEX);
	if (!tempVertexShader)
	{
		std::cout << "Failed to create vertex shader with errors: " << SDL_GetError() << std::endl;
		return false;
	}

	// Creating the fragment shader
	SDL_GPUShader* tempFragmentShader = ShaderCross::getInstance()->createShaderFromHLSL(GraphicsDevice::getInstance()->getDevice(), "resources/shaders/GameObject.frag.hlsl", SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT);
	if (!tempFragmentShader)
	{
		std::cout << "Failed to create fragment shader with errors: " << SDL_GetError() << std::endl;
		return false;
	}

	// Createing the vertex shader
	SDL_GPUShader* vertexShader = ShaderCross::getInstance()->createShaderFromHLSL(GraphicsDevice::getInstance()->getDevice(), "resources/shaders/TextureDisplay.vert.hlsl", SDL_SHADERCROSS_SHADERSTAGE_VERTEX);
	if (!vertexShader)
	{
		std::cout << "Failed to create vertex shader with errors: " << SDL_GetError() << std::endl;
		return false;
	}

	// Creating the fragment shader
	SDL_GPUShader* fragmentShader = ShaderCross::getInstance()->createShaderFromHLSL(GraphicsDevice::getInstance()->getDevice(), "resources/shaders/TextureDisplay.frag.hlsl", SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT);
	if (!fragmentShader)
	{
		std::cout << "Failed to create fragment shader with errors: " << SDL_GetError() << std::endl;
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
	SDL_PropertiesID tempPipelineProps = SDL_CreateProperties();
	SDL_SetStringProperty(tempPipelineProps, SDL_PROP_GPU_GRAPHICSPIPELINE_CREATE_NAME_STRING, "GameObjectTEMPORARY_Graphics_Pipeline");

	// Creating the pipeline
	SDL_GPUGraphicsPipelineCreateInfo tempPipelineCreateInfo = {
		.vertex_shader = tempVertexShader,
		.fragment_shader = tempFragmentShader,
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
		.props = tempPipelineProps,
	};

	mpTempPipeline = SDL_CreateGPUGraphicsPipeline(GraphicsDevice::getInstance()->getDevice(), &tempPipelineCreateInfo);
	if (!mpTempPipeline)
	{
		std::cout << "Failed to create pipeline with errors: " << SDL_GetError() << std::endl;
		return false;
	}

	// Freeing the props
	SDL_DestroyProperties(tempPipelineProps);

	// Releasing shaders
	SDL_ReleaseGPUShader(GraphicsDevice::getInstance()->getDevice(), tempVertexShader);
	SDL_ReleaseGPUShader(GraphicsDevice::getInstance()->getDevice(), tempFragmentShader);


	// Creating the name for the graphics pipeline
	SDL_PropertiesID pipelineProps = SDL_CreateProperties();
	SDL_SetStringProperty(pipelineProps, SDL_PROP_GPU_GRAPHICSPIPELINE_CREATE_NAME_STRING, "GameObjectDisplay_Graphics_Pipeline");

	// Creating the pipeline
	colorTargetDescriptions->format = SDL_GetGPUSwapchainTextureFormat(GraphicsDevice::getInstance()->getDevice(), GraphicsDevice::getInstance()->getWindow());
	SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo = {
		.vertex_shader = vertexShader,
		.fragment_shader = fragmentShader,
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

	mpDisplayPipeline = SDL_CreateGPUGraphicsPipeline(GraphicsDevice::getInstance()->getDevice(), &pipelineCreateInfo);
	if (!mpDisplayPipeline)
	{
		std::cout << "Failed to create pipeline with errors: " << SDL_GetError() << std::endl;
		return false;
	}

	// Freeing the props
	SDL_DestroyProperties(pipelineProps);

	// Releasing shaders
	SDL_ReleaseGPUShader(GraphicsDevice::getInstance()->getDevice(), vertexShader);
	SDL_ReleaseGPUShader(GraphicsDevice::getInstance()->getDevice(), fragmentShader);

	// * --------------------------------- *
	// |  Creating Vertex & Index Buffers  |
	// * --------------------------------- *

	// Calculate size needed
	size_t vertexSize = 4 * sizeof(PosTexVert);
	size_t indexSize = 6 * sizeof(int);

	// Create the initial vertex buffer
	SDL_GPUBufferCreateInfo vertexBufferInfo =
	{
		.usage = SDL_GPU_BUFFERUSAGE_VERTEX,
		.size = (uint32_t)vertexSize
	};
	mpVertexBuffer = SDL_CreateGPUBuffer(GraphicsDevice::getInstance()->getDevice(), &vertexBufferInfo);

	// Checking if the buffer was created preoprly
	if (!mpVertexBuffer)
	{
		std::cout << "Failed to create vertex buffer with errors: " << SDL_GetError() << std::endl;
		return false;
	}
	SDL_SetGPUBufferName(GraphicsDevice::getInstance()->getDevice(), mpVertexBuffer, "GameObject_Vertex_Buffer");

	// Creating the initial index buffer
	SDL_GPUBufferCreateInfo indexBufferInfo =
	{
		.usage = SDL_GPU_BUFFERUSAGE_INDEX,
		.size = (uint32_t)indexSize
	};
	mpIndexBuffer = SDL_CreateGPUBuffer(GraphicsDevice::getInstance()->getDevice(), &indexBufferInfo);

	// Checking if the buffer was created preoprly
	if (!mpIndexBuffer)
	{
		std::cout << "Failed to create index buffer with errors: " << SDL_GetError() << std::endl;
		return false;
	}
	SDL_SetGPUBufferName(GraphicsDevice::getInstance()->getDevice(), mpIndexBuffer, "GameObject_Index_Buffer");

	// Creating the transfer buffer with an initial size
	size_t initialTransferSize = vertexSize + indexSize;

	// Creating the name for the transfer buffer
	SDL_PropertiesID transferBufferProps = SDL_CreateProperties();
	SDL_SetStringProperty(transferBufferProps, SDL_PROP_GPU_TRANSFERBUFFER_CREATE_NAME_STRING, "GameObject_Transfer_Buffer");

	// Defining the transfer buffer info
	SDL_GPUTransferBufferCreateInfo transferInfo = {
		.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
		.size = (uint32_t)initialTransferSize,
		.props = transferBufferProps
	};
	SDL_GPUTransferBuffer* sharedTransferBuffer = SDL_CreateGPUTransferBuffer(GraphicsDevice::getInstance()->getDevice(), &transferInfo);

	// Freeing the props
	SDL_DestroyProperties(transferBufferProps);

	// Checking if the transfer buffer was created preoprly
	if (!sharedTransferBuffer)
	{
		std::cout << "Failed to create transfer buffer with errors: " << SDL_GetError() << std::endl;
		return false;
	}

	// * ------------------------------- *
	// |  Uploading Vertex & Index Data  |
	// * ------------------------------- *

	// Getting the map of the buffer data
	uint8_t* transferData = (uint8_t*)SDL_MapGPUTransferBuffer(GraphicsDevice::getInstance()->getDevice(), sharedTransferBuffer, false);

	// Defining the vertex and index arrays
	PosTexVert vertices[] =
	{
		{ smath::vec3(-0.5f, -0.5f, 0.0f), smath::vec2(0, 1) },
		{ smath::vec3(-0.5f,  0.5f, 0.0f), smath::vec2(0, 0) },
		{ smath::vec3( 0.5f,  0.5f, 0.0f), smath::vec2(1, 0) },
		{ smath::vec3( 0.5f, -0.5f, 0.0f), smath::vec2(1, 1) }
	};
	int indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	memcpy(transferData, vertices, vertexSize);
	memcpy(transferData + vertexSize, indices, indexSize);

	// Unmapping the transfer buffer
	SDL_UnmapGPUTransferBuffer(GraphicsDevice::getInstance()->getDevice(), sharedTransferBuffer);

	// Upload the transfer data to the GPU resources
	SDL_GPUCommandBuffer* commandBuffer = SDL_AcquireGPUCommandBuffer(GraphicsDevice::getInstance()->getDevice());
	SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(commandBuffer);

	// Uploading Vertex Buffer Data
	SDL_GPUTransferBufferLocation transferBufferLcoation =
	{
		.transfer_buffer = sharedTransferBuffer,
		.offset = 0
	};
	SDL_GPUBufferRegion vertexBufferRegion =
	{
		.buffer = mpVertexBuffer,
		.offset = 0,
		.size = sizeof(PosTexVert) * 4
	};
	SDL_UploadToGPUBuffer(
		copyPass,
		&transferBufferLcoation,
		&vertexBufferRegion,
		false
	);

	// Uploading Index Buffer Data
	transferBufferLcoation =
	{
		.transfer_buffer = sharedTransferBuffer,
		.offset = sizeof(PosTexVert) * 4
	};
	SDL_GPUBufferRegion indexBufferRegion =
	{
		.buffer = mpIndexBuffer,
		.offset = 0,
		.size = sizeof(int) * 6
	};
	SDL_UploadToGPUBuffer(copyPass, &transferBufferLcoation, &indexBufferRegion, false);

	// Ending the copy pass
	SDL_EndGPUCopyPass(copyPass);

	// Setting up a fence to wait to the GPU is done, then removing the transfer buffer
	SDL_GPUFence* uploadFence = SDL_SubmitGPUCommandBufferAndAcquireFence(commandBuffer);
	SDL_WaitForGPUFences(GraphicsDevice::getInstance()->getDevice(), true, &uploadFence, 1);
	SDL_ReleaseGPUFence(GraphicsDevice::getInstance()->getDevice(), uploadFence);

	// Releasing the transfer buffer (the buffer data will not be changed during runtime, and thus does not need to be kept around)
	SDL_ReleaseGPUTransferBuffer(GraphicsDevice::getInstance()->getDevice(), sharedTransferBuffer);

	// * ----------------- *
	// |  Display Sampler  |
	// * ----------------- *

	// Creating the sampler
	SDL_GPUSamplerCreateInfo sampleInfo =
	{
		.min_filter = SDL_GPU_FILTER_LINEAR,
		.mag_filter = SDL_GPU_FILTER_NEAREST,
		.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
		.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
		.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
		.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
	};
	mpDisplaySampler = SDL_CreateGPUSampler(GraphicsDevice::getInstance()->getDevice(), &sampleInfo);
	if (!mpDisplaySampler)
	{
		std::cout << "Failed to create nearest sampler with errors: " << SDL_GetError() << std::endl;
		return false;
	}

	// Returning true for proper initialization
	return true;
}
void GameObjectBuffer::cleanup()
{
	SDL_ReleaseGPUBuffer(GraphicsDevice::getInstance()->getDevice(), mpVertexBuffer);
	SDL_ReleaseGPUBuffer(GraphicsDevice::getInstance()->getDevice(), mpIndexBuffer);
	SDL_ReleaseGPUTexture(GraphicsDevice::getInstance()->getDevice(), mpTargetColorTexture);
	SDL_ReleaseGPUTexture(GraphicsDevice::getInstance()->getDevice(), mpTargetDepthTexture);
	SDL_ReleaseGPUGraphicsPipeline(GraphicsDevice::getInstance()->getDevice(), mpDisplayPipeline);
	SDL_ReleaseGPUSampler(GraphicsDevice::getInstance()->getDevice(), mpDisplaySampler);

	SDL_ReleaseGPUGraphicsPipeline(GraphicsDevice::getInstance()->getDevice(), mpTempPipeline);
}

// Window drawing position setter definitions
void GameObjectBuffer::setDisplayPosition(smath::vec3 position, smath::vec2 size)
{
	// Setting the position and size of the histogram (with the size min of 0)
	mDisplayPosition = position;
	mDisplaySize = smath::max(size, smath::vec2(0));

	// Keeping track of the gpu viewport to render this under
	mViewportIndex = GraphicsDevice::getInstance()->getRenderer()->getViewportCount() - 1;
}

// Buffer-modifying function definitions
void GameObjectBuffer::drawBuffer(SDL_GPUCommandBuffer* commandBuffer, SDL_GPURenderPass* renderPass)
{
	// If either texture is invalid, exit out
	if (!mpTargetColorTexture || !mpTargetDepthTexture)
		return;

    // Checking if the manager exists
    if (!mpGameObjectManager)
        return;

	// Checking if there are any game objects to draw
	if (mpGameObjectManager->getObjectCount() <= 0)
		return;

	// Checking if the display size is actually visible
	if (mDisplaySize.x * mDisplaySize.y == 0.0f)
		return;

	// Binding the graphics pipeline
	SDL_BindGPUGraphicsPipeline(renderPass, mpDisplayPipeline);

	// Binding the texture and sampler
	SDL_GPUTextureSamplerBinding textureSamplingBinding = { .texture = mpTargetColorTexture, .sampler = mpDisplaySampler };
	SDL_BindGPUFragmentSamplers(renderPass, 0, &textureSamplingBinding, 1);

	// Setting the scissor
	SDL_SetGPUScissor(renderPass, GraphicsDevice::getInstance()->getRenderer()->getViewportAddress(mViewportIndex));

	// Setting the projection matrix uniform of the graphics pipeline
	smath::mat4 projectionMatrix = GraphicsDevice::getInstance()->getProjectionMatrix();
	projectionMatrix *= smath::translate(mDisplayPosition) * smath::scale(smath::vec3(mDisplaySize, 1.0f)) * smath::translate(smath::vec3(0.5f, 0.5f, 0.0f));
	SDL_PushGPUVertexUniformData(commandBuffer, 0, &projectionMatrix, (uint32_t)sizeof(smath::mat4));

	// Binding the vertex buffer
	SDL_GPUBufferBinding vertexBinding =
	{
		.buffer = mpVertexBuffer,
		.offset = 0
	};
	SDL_BindGPUVertexBuffers(renderPass, 0, &vertexBinding, 1);

	// Binding the index buffer
	SDL_GPUBufferBinding indexBinding =
	{
		.buffer = mpIndexBuffer,
		.offset = 0
	};
	SDL_BindGPUIndexBuffer(renderPass, &indexBinding, SDL_GPU_INDEXELEMENTSIZE_32BIT);

	// Drawing the quad
	SDL_DrawGPUIndexedPrimitives(renderPass, 6, 1, 0, 0, 0);
}

// Texture management functions
void GameObjectBuffer::updateTextureSize(const smath::ivec2& dimensions)
{
	// Ensuring the dimensions are valid
	if (dimensions.x <= 0 || dimensions.y <= 0)
		return;

	// Updating dimensions
	mDimensions = dimensions;

	// Updating camera aspect ratio
	mCamera.setAspectRatio(dimensions);

	// If there are textures already, release them
	if (mpTargetColorTexture)
		SDL_ReleaseGPUTexture(GraphicsDevice::getInstance()->getDevice(), mpTargetColorTexture);
	if (mpTargetDepthTexture)
		SDL_ReleaseGPUTexture(GraphicsDevice::getInstance()->getDevice(), mpTargetDepthTexture);

	// Creating the new depth texture
	SDL_GPUTextureCreateInfo depthTextureInfo =
	{
		.type = SDL_GPU_TEXTURETYPE_2D,
		.format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT,
		.usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET,
		.width = (uint32_t)mDimensions.x,
		.height = (uint32_t)mDimensions.y,
		.layer_count_or_depth = 1,
		.num_levels = 1,
		.sample_count = SDL_GPU_SAMPLECOUNT_1,
	};

	// Actually creating the texture
	mpTargetDepthTexture = SDL_CreateGPUTexture(GraphicsDevice::getInstance()->getDevice(), &depthTextureInfo);
	if (!mpTargetDepthTexture)
	{
		std::cout << "Failed to create depth texture with errors: " << SDL_GetError() << std::endl;
		return;
	}

	// Setting the texture's name
	SDL_SetGPUTextureName(GraphicsDevice::getInstance()->getDevice(), mpTargetDepthTexture, "GameObjectBuffer_DepthTexture");

	// Creating the color texture
	SDL_GPUTextureCreateInfo colorTextureInfo =
	{
		.type = SDL_GPU_TEXTURETYPE_2D,
		.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
		.usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER,
		.width = (uint32_t)mDimensions.x,
		.height = (uint32_t)mDimensions.y,
		.layer_count_or_depth = 1,
		.num_levels = 1
	};
	mpTargetColorTexture = SDL_CreateGPUTexture(GraphicsDevice::getInstance()->getDevice(), &colorTextureInfo);

	// Checking for errors with the GPU Texture
	if (!mpTargetColorTexture)
	{
		std::cout << "Failed to create GPU Texture with errors: " << SDL_GetError() << std::endl;
		return;
	}

	// Naming the GPU Texture
	std::string textureName = "GameObjectBuffer_ColorTexture";
	SDL_SetGPUTextureName(GraphicsDevice::getInstance()->getDevice(), mpTargetColorTexture, textureName.c_str());
}
void GameObjectBuffer::drawToTexture()
{
	// If either texture is invalid, exit out
	if (!mpTargetColorTexture || !mpTargetDepthTexture)
		return;

	// Acquiring command buffer
	SDL_GPUCommandBuffer* commandBuffer = SDL_AcquireGPUCommandBuffer(GraphicsDevice::getInstance()->getDevice());

	// Setting up target info and starting render pass
	SDL_GPUColorTargetInfo colorTargetInfo =
	{
		.texture = mpTargetColorTexture,
		.clear_color = { 0.0f, 0.0f, 0.0f, 1.0f },
		.load_op = SDL_GPU_LOADOP_CLEAR,
		.store_op = SDL_GPU_STOREOP_STORE
	};

	// Setting the depth stencil info
	SDL_GPUDepthStencilTargetInfo depthStencilInfo =
	{
		.texture = mpTargetDepthTexture,
		.clear_depth = 0.0f,
		.load_op = SDL_GPU_LOADOP_CLEAR,
		.store_op = SDL_GPU_STOREOP_STORE,
		.cycle = false,
	};

	// Starting render pass
	SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(commandBuffer, &colorTargetInfo, 1, &depthStencilInfo);
	if (!renderPass)
	{
		std::cout << "Failed to begin the render pass with errors: " << SDL_GetError() << std::endl;
		return;
	}

	// If either depth or color texture are null, return out
	if (!mpTargetColorTexture || !mpTargetDepthTexture)
		return;

	// Checking if the manager exists
	if (!mpGameObjectManager)
		return;

	// Checking if there are any game objects to draw
	if (mpGameObjectManager->getObjectCount() <= 0)
		return;

	// Binding the vertex buffer
	SDL_GPUBufferBinding vertexBinding =
	{
		.buffer = mpVertexBuffer,
		.offset = 0
	};
	SDL_BindGPUVertexBuffers(renderPass, 0, &vertexBinding, 1);

	// Binding the index buffer
	SDL_GPUBufferBinding indexBinding =
	{
		.buffer = mpIndexBuffer,
		.offset = 0
	};
	SDL_BindGPUIndexBuffer(renderPass, &indexBinding, SDL_GPU_INDEXELEMENTSIZE_32BIT);

	// Binding the graphics pipeline
	SDL_BindGPUGraphicsPipeline(renderPass, mpTempPipeline);

	// Otherwise, loop through each game object, generate it's transformation matrix, and render it
	for (int i = 0; i < mpGameObjectManager->getObjectCount(); i++)
	{
		// Setting the projection matrix uniform of the graphics pipeline
		smath::mat4 projectionMatrix = mCamera.getProjectionMatrix() * mpGameObjectManager->getObjectAtIndex(i)->getTransform()->getTransformMatrix();
		SDL_PushGPUVertexUniformData(commandBuffer, 0, &projectionMatrix, (uint32_t)sizeof(smath::mat4));

		// Drawing the primtive for the game object
		SDL_DrawGPUIndexedPrimitives(renderPass, 6, 1, 0, 0, 0);
	}

	// End render pass and submit command buffer
	SDL_EndGPURenderPass(renderPass);
	SDL_SubmitGPUCommandBuffer(commandBuffer);
}