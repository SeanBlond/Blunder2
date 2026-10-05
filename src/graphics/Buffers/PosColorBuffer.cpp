#include "PosColorBuffer.h"
#include "../GraphicsDevice.h"
#include "../ShaderCross.h"

// * ----------------------------------------- *
// |  PosColorMeshBuffer Function Definitions  |
// * ----------------------------------------- *

// Data Management function definitions
bool PosColorMeshBuffer::init()
{
	// Createing the vertex shader
	SDL_GPUShader* vertexShader = ShaderCross::getInstance()->createShaderFromHLSL(GraphicsDevice::getInstance()->getDevice(), "resources/shaders/PosColor.vert.hlsl", SDL_SHADERCROSS_SHADERSTAGE_VERTEX);
	if (vertexShader == nullptr)
	{
		std::cout << "Failed to create vertex shader with errors: " << SDL_GetError() << std::endl;
		return false;
	}

	// Creating the fragment shader
	SDL_GPUShader* fragmentShader = ShaderCross::getInstance()->createShaderFromHLSL(GraphicsDevice::getInstance()->getDevice(), "resources/shaders/PosColor.frag.hlsl", SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT);
	if (fragmentShader == nullptr)
	{
		std::cout << "Failed to create fragment shader with errors: " << SDL_GetError() << std::endl;
		return false;
	}

	// Defining the Vertex Buffer Expectations
	SDL_GPUVertexBufferDescription vertexBufferDesc[] =
	{
		{
			.slot = 0,
			.pitch = sizeof(PosColorVert),
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
			.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
			.offset = offsetof(PosColorVert, color)
		}
	};

	// Defining the color targets
	SDL_GPUColorTargetDescription colorTargetDescriptions[] =
	{
		{
			.format = SDL_GetGPUSwapchainTextureFormat(GraphicsDevice::getInstance()->getDevice(), GraphicsDevice::getInstance()->getWindow()),
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
	SDL_SetStringProperty(pipelineProps, SDL_PROP_GPU_GRAPHICSPIPELINE_CREATE_NAME_STRING, "PosColor_Graphics_Pipeline");

	// Creating the pipeline
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


	mpPipeline = SDL_CreateGPUGraphicsPipeline(GraphicsDevice::getInstance()->getDevice(), &pipelineCreateInfo);
	if (mpPipeline == nullptr)
	{
		std::cout << "Failed to create pipeline with errors: " << SDL_GetError() << std::endl;
		return false;
	}

	// Freeing the props
	SDL_DestroyProperties(pipelineProps);

	// Releasing shaders
	SDL_ReleaseGPUShader(GraphicsDevice::getInstance()->getDevice(), vertexShader);
	SDL_ReleaseGPUShader(GraphicsDevice::getInstance()->getDevice(), fragmentShader);

	// Small amount of initial elements to set the initial capacities of the buffers
	const size_t INITIAL_ELEMENT_COUNT = 64;

	// Calculate initial memory footprints
	mVertexCapacity = INITIAL_ELEMENT_COUNT * sizeof(PosColorVert);
	mIndexCapacity = INITIAL_ELEMENT_COUNT * sizeof(int);

	// Create the initial vertex buffer
	SDL_GPUBufferCreateInfo vertexBufferInfo =
	{
		.usage = SDL_GPU_BUFFERUSAGE_VERTEX,
		.size = (uint32_t)mVertexCapacity
	};
	mpVertexBuffer = SDL_CreateGPUBuffer(GraphicsDevice::getInstance()->getDevice(), &vertexBufferInfo);

	// Checking if the buffer was created preoprly
	if (!mpVertexBuffer)
	{
		std::cout << "Failed to create vertex buffer with errors: " << SDL_GetError() << std::endl;
		return false;
	}
	SDL_SetGPUBufferName(GraphicsDevice::getInstance()->getDevice(), mpVertexBuffer, "Initial_PosColor_Vertex_Buffer");

	// Creating the initial index buffer
	SDL_GPUBufferCreateInfo indexBufferInfo =
	{
		.usage = SDL_GPU_BUFFERUSAGE_INDEX,
		.size = (uint32_t)mIndexCapacity
	};
	mpIndexBuffer = SDL_CreateGPUBuffer(GraphicsDevice::getInstance()->getDevice(), &indexBufferInfo);

	// Checking if the buffer was created preoprly
	if (!mpIndexBuffer)
	{
		std::cout << "Failed to create index buffer with errors: " << SDL_GetError() << std::endl;
		return false;
	}
	SDL_SetGPUBufferName(GraphicsDevice::getInstance()->getDevice(), mpIndexBuffer, "Initial_PosColor_Index_Buffer");

	// Creating the transfer buffer with an initial size
	size_t initialTransferSize = mVertexCapacity + mIndexCapacity;
	
	// Creating the name for the transfer buffer
	SDL_PropertiesID transferBufferProps = SDL_CreateProperties();
	SDL_SetStringProperty(transferBufferProps, SDL_PROP_GPU_TRANSFERBUFFER_CREATE_NAME_STRING, "Initial_PosColor_Transfer_Buffer");

	// Defining the transfer buffer info
	SDL_GPUTransferBufferCreateInfo transferInfo = {
		.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
		.size = (uint32_t)initialTransferSize,
		.props = transferBufferProps
	};
	mpSharedTransferBuffer = SDL_CreateGPUTransferBuffer(GraphicsDevice::getInstance()->getDevice(), &transferInfo);
	mTransferCapacity = initialTransferSize;

	// Freeing the props
	SDL_DestroyProperties(transferBufferProps);

	// Checking if the transfer buffer was created preoprly
	if (!mpSharedTransferBuffer)
	{
		std::cout << "Failed to create transfer buffer with errors: " << SDL_GetError() << std::endl;
		return false;
	}

	// Returning true for proper initialization
	return true;
}
void PosColorMeshBuffer::cleanup()
{
	// Unbinding all the dynamically created gpu stuff
	SDL_ReleaseGPUGraphicsPipeline(GraphicsDevice::getInstance()->getDevice(), mpPipeline);
	SDL_ReleaseGPUBuffer(GraphicsDevice::getInstance()->getDevice(), mpVertexBuffer);
	SDL_ReleaseGPUBuffer(GraphicsDevice::getInstance()->getDevice(), mpIndexBuffer);
	SDL_ReleaseGPUTransferBuffer(GraphicsDevice::getInstance()->getDevice(), mpSharedTransferBuffer);
}

// Buffer-modifying function definitions
void PosColorMeshBuffer::addMesh(const PosColorMesh& mesh)
{
	// If there is no viewport, don't add the mesh
	if (GraphicsDevice::getInstance()->getRenderer()->getViewportCount() == 0)
		return;

	// Adding the indices from the appended mesh
	int indiceOffset = (int)mPosColorMesh.vertices.size();
	if (indiceOffset != 0) // Checking if indice offset has to be considered
	{
		for (int i = 0; i < mesh.indices.size(); i++)
		{
			// Each indice must be added with the offset of the number of vertices already in the mesh
			mPosColorMesh.indices.push_back(mesh.indices[i] + indiceOffset);
		}
	}
	else
	{
		// If there is no indice offset, the one vector of indices can just be added to the other
		mPosColorMesh.indices.insert(mPosColorMesh.indices.end(), mesh.indices.begin(), mesh.indices.end());
	}

	// Adding the vertices of the appended mesh
	mPosColorMesh.vertices.insert(mPosColorMesh.vertices.end(), mesh.vertices.begin(), mesh.vertices.end());


	// Adding the indices to the current view offset
	mViewIndexOffsets.back() += mesh.indices.size();
}
void PosColorMeshBuffer::clearMesh()
{
	// Clearing the vertices and indices
	mPosColorMesh.vertices.clear();
	mPosColorMesh.indices.clear();

	// Clearing the viewport index offsets
	mViewIndexOffsets.clear();
}
void PosColorMeshBuffer::updateBuffers()
{
	// Getting the required sizes necessary of the buffers
	size_t requiredVertexSize = mPosColorMesh.vertices.size() * sizeof(PosColorVert);
	size_t requiredIndexSize = mPosColorMesh.indices.size() * sizeof(int);

	// Checking if the vertex buffer needs to be recreated and/or grown
	if (mpVertexBuffer == nullptr || requiredVertexSize > mVertexCapacity)
	{
		// Releasing the old buffer
		if (mpVertexBuffer != nullptr)
		{
			SDL_ReleaseGPUBuffer(GraphicsDevice::getInstance()->getDevice(), mpVertexBuffer);
		}

		// Adding padding to the capacity
		mVertexCapacity = requiredVertexSize * 2;

		// Creating the new buffer
		SDL_GPUBufferCreateInfo vertexBufferInfo =
		{
			.usage = SDL_GPU_BUFFERUSAGE_VERTEX,
			.size = (uint32_t)mVertexCapacity
		};
		mpVertexBuffer = SDL_CreateGPUBuffer(GraphicsDevice::getInstance()->getDevice(), &vertexBufferInfo);
		SDL_SetGPUBufferName(GraphicsDevice::getInstance()->getDevice(), mpVertexBuffer, "PosColor_Vertex_Buffer");
	}

	// Check if the index buffer needs to be recreated and/or grown
	if (mpIndexBuffer == nullptr || requiredIndexSize > mIndexCapacity)
	{
		if (mpIndexBuffer != nullptr)
		{
			SDL_ReleaseGPUBuffer(GraphicsDevice::getInstance()->getDevice(), mpIndexBuffer);
		}

		// Adding padding
		mIndexCapacity = requiredIndexSize * 2;

		// Creating the new buffer
		SDL_GPUBufferCreateInfo indexBufferInfo =
		{
			.usage = SDL_GPU_BUFFERUSAGE_INDEX,
			.size = (uint32_t)mIndexCapacity
		};
		mpIndexBuffer = SDL_CreateGPUBuffer(GraphicsDevice::getInstance()->getDevice(), &indexBufferInfo);
		SDL_SetGPUBufferName(GraphicsDevice::getInstance()->getDevice(), mpIndexBuffer, "PosColor_Index_Buffer");
	}

	// Check if the Shared Transfer Buffer needs to be recreated and/or grown
	size_t requiredTransferSize = requiredVertexSize + requiredIndexSize;
	if (mpSharedTransferBuffer == nullptr || requiredTransferSize > mTransferCapacity)
	{
		if (mpSharedTransferBuffer != nullptr)
		{
			SDL_ReleaseGPUTransferBuffer(GraphicsDevice::getInstance()->getDevice(), mpSharedTransferBuffer);
		}

		// Add padding
		mTransferCapacity = requiredTransferSize * 2;

		// Creating the new buffer
		SDL_GPUTransferBufferCreateInfo transferInfo =
		{
			.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
			.size = (uint32_t)mTransferCapacity
		};
		mpSharedTransferBuffer = SDL_CreateGPUTransferBuffer(GraphicsDevice::getInstance()->getDevice(), &transferInfo);

		// Updating the bool that tracks the reallocation of the transfer buffer
		mTransferBufferReallocated = true;
	}
}
void PosColorMeshBuffer::sendMeshToGPU(SDL_GPUCommandBuffer* currentCommandBuffer)
{
	// Creating sizes of the buffers
	uint32_t vertexBufferSize = mPosColorMesh.vertices.size() * sizeof(PosColorVert);
	uint32_t indexBufferSize = mPosColorMesh.indices.size() * sizeof(int);

	// Exiting out if there's no mesh data
	if (vertexBufferSize == 0 || indexBufferSize == 0)
		return;

	// Updating the buffers
	updateBuffers();

	// Calculating the offset of the index buffer
	uint32_t indexBufferOffset = (vertexBufferSize + 3) & ~3;

	// Getting the address to the GPU buffer being transferred to
	uint8_t* transferBufferMap = (uint8_t*)SDL_MapGPUTransferBuffer(GraphicsDevice::getInstance()->getDevice(), mpSharedTransferBuffer, !mTransferBufferReallocated);
	if (!transferBufferMap)
	{
		std::cout << "Failed to get transfer buffer map for vertex buffer with errors: " << SDL_GetError() << std::endl;
		return;
	}

	// Writing the vertices to the map
	memcpy(transferBufferMap, mPosColorMesh.vertices.data(), vertexBufferSize);

	// Writing the indices to the map with an offset
	memcpy(transferBufferMap + indexBufferOffset, mPosColorMesh.indices.data(), indexBufferSize);

	// Unmap the transfer buffer
	SDL_UnmapGPUTransferBuffer(GraphicsDevice::getInstance()->getDevice(), mpSharedTransferBuffer);

	// Upload the transfer data to the GPU resources
	SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(currentCommandBuffer);

	// Uploading Vertex Buffer Data
	SDL_GPUTransferBufferLocation vertexBufferLcoation =
	{
		.transfer_buffer = mpSharedTransferBuffer,
		.offset = 0
	};
	SDL_GPUBufferRegion vertexBufferRegion =
	{
		.buffer = mpVertexBuffer,
		.offset = 0,
		.size = vertexBufferSize
	};
	SDL_UploadToGPUBuffer(copyPass, &vertexBufferLcoation, &vertexBufferRegion, false);

	// Uploading Index Buffer Data
	SDL_GPUTransferBufferLocation indexBufferLcoation =
	{
		.transfer_buffer = mpSharedTransferBuffer,
		.offset = indexBufferOffset
	};
	SDL_GPUBufferRegion indexBufferRegion =
	{
		.buffer = mpIndexBuffer,
		.offset = 0,
		.size = indexBufferSize
	};
	SDL_UploadToGPUBuffer(copyPass, &indexBufferLcoation, &indexBufferRegion, true);

	// Ending the copy pass
	SDL_EndGPUCopyPass(copyPass);

	// Resetting the transfer buffer reallocation bool for the next frame
	mTransferBufferReallocated = false;
}
void PosColorMeshBuffer::drawBuffer(SDL_GPUCommandBuffer* commandBuffer, SDL_GPURenderPass* renderPass)
{
	// Exiting out if there's no mesh data
	if (mPosColorMesh.vertices.size() == 0 || mPosColorMesh.indices.size() == 0)
		return;

	// Checking if the command buffer or render pass is invalid
	if (!commandBuffer || !renderPass)
	{
		std::cout << "Failed to read command buffer or render pass" << std::endl;
		return;
	}

	// Binding the Pipeline
	SDL_BindGPUGraphicsPipeline(renderPass, mpPipeline);

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

	// Keeping track of the index offset of the different viewport renders
	int indexOffset = 0;

	for (int i = 0; i < mViewIndexOffsets.size(); i++)
	{
		// If theres no indices in this viewport, do nothing
		if (mViewIndexOffsets[i] == 0)
			continue;

		// Setting the scissor
		SDL_SetGPUScissor(renderPass, GraphicsDevice::getInstance()->getRenderer()->getViewportAddress(i));

		// Passing in the projeciton matrix
		smath::mat4 projectionMatrix = GraphicsDevice::getInstance()->getProjectionMatrix();
		SDL_PushGPUVertexUniformData(commandBuffer, 0, &projectionMatrix, (uint32_t)sizeof(smath::mat4));

		// Draw call
		SDL_DrawGPUIndexedPrimitives(renderPass, mViewIndexOffsets[i], 1, indexOffset, 0, 0);

		// Adding the index size to the offset
		indexOffset += mViewIndexOffsets[i];
	}
}
