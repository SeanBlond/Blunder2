#include "TextBuffer.h"
#include "../GraphicsDevice.h"
#include "../ShaderCross.h"

// * ------------------------------------- *
// |  TextMeshBuffer Function Definitions  |
// * ------------------------------------- *

// Data Management Function Definitions
bool TextMeshBuffer::init(std::string textureAtlasCoords, std::string textureAtlasImage)
{
	// Createing the vertex shader
	SDL_GPUShader* vertexShader = ShaderCross::getInstance()->createShaderFromHLSL(GraphicsDevice::getInstance()->getDevice(), "resources/shaders/PosColorTex.vert.hlsl", SDL_SHADERCROSS_SHADERSTAGE_VERTEX);
	if (vertexShader == nullptr)
	{
		std::cout << "Failed to create vertex shader with errors: " << SDL_GetError() << std::endl;
		return false;
	}

	// Creating the fragment shader
	SDL_GPUShader* fragmentShader = ShaderCross::getInstance()->createShaderFromHLSL(GraphicsDevice::getInstance()->getDevice(), "resources/shaders/PosColorTex.frag.hlsl", SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT);
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
			.pitch = sizeof(PosColorTexVert),
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
			.offset = offsetof(PosColorTexVert, color)
		},
		{
			.location = 2,
			.buffer_slot = 0,
			.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
			.offset = offsetof(PosColorTexVert, texCoord)
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
	SDL_SetStringProperty(pipelineProps, SDL_PROP_GPU_GRAPHICSPIPELINE_CREATE_NAME_STRING, "Text_Graphics_Pipeline");

	// Creating the pipeline
	SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo = {
		.vertex_shader = vertexShader,
		.fragment_shader = fragmentShader,
		.vertex_input_state = SDL_GPUVertexInputState{
			.vertex_buffer_descriptions = vertexBufferDesc,
			.num_vertex_buffers = 1,
			.vertex_attributes = vertexAttributes,
			.num_vertex_attributes = 3
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
	mVertexCapacity = INITIAL_ELEMENT_COUNT * sizeof(PosColorTexVert);
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
	SDL_SetGPUBufferName(GraphicsDevice::getInstance()->getDevice(), mpVertexBuffer, "Initial_Text_Vertex_Buffer");

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
	SDL_SetGPUBufferName(GraphicsDevice::getInstance()->getDevice(), mpIndexBuffer, "Initial_Text_Index_Buffer");

	// Creating the transfer buffer with an initial size
	size_t initialTransferSize = mVertexCapacity + mIndexCapacity;

	// Creating the name for the transfer buffer
	SDL_PropertiesID transferBufferProps = SDL_CreateProperties();
	SDL_SetStringProperty(transferBufferProps, SDL_PROP_GPU_TRANSFERBUFFER_CREATE_NAME_STRING, "Initial_Text_Transfer_Buffer");

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

	// Creating the atlas texture
	if (!loadTextureAtlas(textureAtlasCoords, textureAtlasImage))
	{
		std::cout << "Failed to load the texture atlas" << std::endl;
		return false;
	}

	// Creating the sampler
	SDL_GPUSamplerCreateInfo sampleInfo =
	{
		.min_filter = SDL_GPU_FILTER_LINEAR,
		.mag_filter = SDL_GPU_FILTER_LINEAR,
		.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
		.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
		.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
		.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
	};
	mpSampler = SDL_CreateGPUSampler(GraphicsDevice::getInstance()->getDevice(), &sampleInfo);

	if (!mpSampler)
	{
		std::cout << "Failed to create sampler for the atlas texture with errors: " << SDL_GetError() << std::endl;
		return false;
	}

	// Returning true for proper initialization
	return true;
}
void TextMeshBuffer::cleanup()
{
	// Unbinding all the dynamically created gpu stuff
	SDL_ReleaseGPUGraphicsPipeline(GraphicsDevice::getInstance()->getDevice(), mpPipeline);
	SDL_ReleaseGPUBuffer(GraphicsDevice::getInstance()->getDevice(), mpVertexBuffer);
	SDL_ReleaseGPUBuffer(GraphicsDevice::getInstance()->getDevice(), mpIndexBuffer);
	SDL_ReleaseGPUTexture(GraphicsDevice::getInstance()->getDevice(), mpAtlasTexture);
	SDL_ReleaseGPUSampler(GraphicsDevice::getInstance()->getDevice(), mpSampler);
	SDL_ReleaseGPUTransferBuffer(GraphicsDevice::getInstance()->getDevice(), mpSharedTransferBuffer);
}

// Buffer-modifying functions
void TextMeshBuffer::addMesh(const PosColorTexMesh& mesh)
{
	// If there is no viewport, don't add the mesh
	if (GraphicsDevice::getInstance()->getRenderer()->getViewportCount() == 0)
		return;

	// Adding the indices from the appended mesh
	mTextMesh.indices.insert(mTextMesh.indices.end(), mesh.indices.begin(), mesh.indices.end());

	// Adding the vertices of the appended mesh
	mTextMesh.vertices.insert(mTextMesh.vertices.end(), mesh.vertices.begin(), mesh.vertices.end());

	// Adding the indices to the current view offset
	mViewIndexOffsets.back() += mesh.indices.size();
}
void TextMeshBuffer::addText(
	const std::string& text, const smath::vec2& pos, const float& zPos, const float& size, Color color,
	HorizontalTextAlign horizontalAlignment, VerticalTextAlign verticalAlignment)
{
	// Setting x and y values
	float x = pos.x;
	float y = pos.y;

	// Calculating font scale
	float scale = size / (float)mFontSize;

	// Calculating Alignment
	float xAlignmentOffset = 0;
	if (horizontalAlignment == TEXT_H_RIGHT)
	{
		float textWidth = 0;
		for (int i = 0; i < text.size(); i++)
		{
			textWidth += (i == (text.size() - 1) ? 0 : getCharacter(text[i]).Bearing.x) + getCharacter(text[i]).Advance;
		}
		xAlignmentOffset = textWidth * scale;
	}
	else if (horizontalAlignment == TEXT_H_CENTER)
	{
		float textWidth = 0;
		for (int i = 0; i < text.size(); i++)
		{
			textWidth += (i == (text.size() - 1) ? 0 : getCharacter(text[i]).Bearing.x) + getCharacter(text[i]).Advance;
		}
		xAlignmentOffset = textWidth * scale * 0.5f;
	}

	// Offsetting X
	x -= (xAlignmentOffset);

	// Storing initial X
	float initialX = x;

	// Loop through all characters
	for (int i = 0; i < text.size(); i++)
	{
		Character ch = getCharacter(text[i]);

		// Checking for New Line
		if (text[i] == '\n')
		{
			y += mLineHeight * 1.3 * scale;
			x = initialX;
			continue;
		}
		else if (text[i] == ' ')
		{
			x += ch.Advance * scale;
			continue;
		}

		// Determing the Y Alignment Offset
		float yAlignmentOffset = 0.0f;
		if (verticalAlignment == TEXT_V_MIDDLE)
			yAlignmentOffset = mLineHeight * scale * 0.5f;
		else if (verticalAlignment == TEXT_V_BOTTOM)
			yAlignmentOffset = mLineHeight * scale;

		float xpos = x + (ch.Bearing.x * scale);
		float ypos = y + (ch.Bearing.y * scale) - yAlignmentOffset;
		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		// Setting the index offset for adding the mesh
		int indexOffset = mTextMesh.vertices.size();

		// Creating the mesh
		PosColorTexMesh tempMesh =
		{
			// Creating the vertices
			.vertices =
			{
				{ smath::vec3(xpos,     ypos,     zPos), color, smath::vec2(ch.Positions.x, ch.Positions.y) },
				{ smath::vec3(xpos,     ypos + h, zPos), color, smath::vec2(ch.Positions.x, ch.Positions.w) },
				{ smath::vec3(xpos + w, ypos + h, zPos), color, smath::vec2(ch.Positions.z, ch.Positions.w) },
				{ smath::vec3(xpos + w, ypos,     zPos), color, smath::vec2(ch.Positions.z, ch.Positions.y) }
			},

			// Creating the indices
			.indices =
			{
				indexOffset + 0, indexOffset + 1, indexOffset + 2,
				indexOffset + 0, indexOffset + 2, indexOffset + 3
			}
		};

		// Adding the mesh
		addMesh(tempMesh);

		// now advance cursors for next glyph
		x += ch.Advance * scale;
	}
}
smath::vec2 TextMeshBuffer::getPositionInText(
	const std::string& text, int index, const smath::vec2& pos, const float& size,
	HorizontalTextAlign horizontalAlignment, VerticalTextAlign verticalAlignment)
{
	// Setting x and y values
	float x = pos.x;
	float y = pos.y;

	// Clamping index
	index = smath::clamp(index, 0, (int)text.size());

	// Calculating font scale
	float scale = size / (float)mFontSize;

	// Calculating Alignment
	float xAlignmentOffset = 0;
	if (horizontalAlignment == TEXT_H_RIGHT)
	{
		float textWidth = 0;
		for (int i = 0; i < text.size(); i++)
		{
			textWidth += (i == (text.size() - 1) ? 0 : getCharacter(text[i]).Bearing.x) + getCharacter(text[i]).Advance;
		}
		xAlignmentOffset = textWidth * scale;
	}
	else if (horizontalAlignment == TEXT_H_CENTER)
	{
		float textWidth = 0;
		for (int i = 0; i < text.size(); i++)
		{
			textWidth += (i == (text.size() - 1) ? 0 : getCharacter(text[i]).Bearing.x) + getCharacter(text[i]).Advance;
		}
		xAlignmentOffset = textWidth * scale * 0.5f;
	}

	// Offsetting X
	x -= (xAlignmentOffset);

	// Storing initial X
	float initialX = x;

	// Loop through all characters
	for (int i = 0; i < index; i++)
	{
		Character ch = getCharacter(text[i]);

		// Checking for New Line
		if (text[i] == '\n')
		{
			y += mLineHeight * 1.3 * scale;
			x = initialX;
			continue;
		}
		else if (text[i] == ' ')
		{
			x += ch.Advance * scale;
			continue;
		}

		// Determing the Y Alignment Offset
		float yAlignmentOffset = 0.0f;
		if (verticalAlignment == TEXT_V_MIDDLE)
			yAlignmentOffset = mLineHeight * scale * 0.5f;
		else if (verticalAlignment == TEXT_V_BOTTOM)
			yAlignmentOffset = mLineHeight * scale;

		float xpos = x + (ch.Bearing.x * scale);
		float ypos = y + (ch.Bearing.y * scale) - yAlignmentOffset;
		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		// Setting the index offset for adding the mesh
		int indexOffset = mTextMesh.vertices.size();

		// now advance cursors for next glyph
		x += ch.Advance * scale;
	}

	// Outputting position
	return smath::vec2(x, y);
}
void TextMeshBuffer::clearMesh()
{
	// Clearing the vertices and indices
	mTextMesh.vertices.clear();
	mTextMesh.indices.clear();

	// Clearing the viewport index offsets
	mViewIndexOffsets.clear();
}
void TextMeshBuffer::updateBuffers()
{
	// Getting the required sizes necessary of the buffers
	size_t requiredVertexSize = mTextMesh.vertices.size() * sizeof(PosColorTexVert);
	size_t requiredIndexSize = mTextMesh.indices.size() * sizeof(int);

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
		SDL_SetGPUBufferName(GraphicsDevice::getInstance()->getDevice(), mpVertexBuffer, "Text_Vertex_Buffer");
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
		SDL_SetGPUBufferName(GraphicsDevice::getInstance()->getDevice(), mpIndexBuffer, "Text_Index_Buffer");
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
void TextMeshBuffer::sendMeshToGPU(SDL_GPUCommandBuffer* currentCommandBuffer)
{
	// Creating sizes of the buffers
	uint32_t vertexBufferSize = mTextMesh.vertices.size() * sizeof(PosColorTexVert);
	uint32_t indexBufferSize = mTextMesh.indices.size() * sizeof(int);

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
	memcpy(transferBufferMap, mTextMesh.vertices.data(), vertexBufferSize);

	// Writing the indices to the map with an offset
	memcpy(transferBufferMap + indexBufferOffset, mTextMesh.indices.data(), indexBufferSize);

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
void TextMeshBuffer::drawBuffer(SDL_GPUCommandBuffer* commandBuffer, SDL_GPURenderPass* renderPass)
{
	// Exiting out if there's no mesh data
	if (mTextMesh.vertices.size() == 0 || mTextMesh.indices.size() == 0)
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

	// Binding the texture
	SDL_GPUTextureSamplerBinding textureSamplingBinding =
	{
		.texture = mpAtlasTexture,
		.sampler = mpSampler
	};
	SDL_BindGPUFragmentSamplers(renderPass, 0, &textureSamplingBinding, 1);

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

// Function definition for loading in a texture atlas
bool TextMeshBuffer::loadTextureAtlas(std::string atlasCoords, std::string atlasImage)
{
	// Loading the buffer info path
	std::ifstream atlasCoordsFile(atlasCoords);

	// Exiting if the file failed to open
	if (!atlasCoordsFile)
	{
		std::cout << "Atlas coord file failed to read" << std::endl;
		return false;
	}

	// Loading in the image as a surface
	SDL_Surface* atlasImageSurface = SDL_LoadSurface(atlasImage.c_str());

	if (!atlasImageSurface)
	{
		std::cout << "Atlas image file failed to load with error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Creating GPU Texture from surface
	if (!createGPUTextureFromSurface(atlasImageSurface))
	{
		std::cout << "Failed to create GPU Texture from Surface" << std::endl;
		return false;
	}

	// Deallocating the image surface
	SDL_DestroySurface(atlasImageSurface);

	// Finding Text Size
	std::string tempString = "";
	std::string targetString = "size=";
	while (tempString.substr(0, targetString.size()) != targetString || atlasCoordsFile.eof())
	{
		atlasCoordsFile >> tempString;
	}
	if (atlasCoordsFile.eof())
	{
		std::cout << "Failed to find target in atlasCoordsFile" << std::endl;
		return false;
	}
	mFontSize = stoi(tempString.substr(targetString.size(), tempString.size() - targetString.size()));

	// Finding char range
	targetString = "charset=";
	while (tempString.substr(0, targetString.size()) != targetString || atlasCoordsFile.eof())
	{
		atlasCoordsFile >> tempString;
	}
	if (atlasCoordsFile.eof())
	{
		std::cout << "Failed to find target in atlasCoordsFile" << std::endl;
		return false;
	}
	std::string tempValueRead = tempString.substr(targetString.size() + 1, tempString.size() - targetString.size() - 2);
	std::regex charRange(R"(^(\d{1,3})\-(\d{1,3}))");
	std::smatch matches;
	std::regex_search(tempValueRead, matches, charRange);
	mFirstChar = stoi(matches[1]);
	mLastChar = stoi(matches[2]);

	// Finding line height
	targetString = "lineHeight=";
	while (tempString.substr(0, targetString.size()) != targetString && !atlasCoordsFile.eof())
	{
		atlasCoordsFile >> tempString;
	}
	if (atlasCoordsFile.eof())
	{
		std::cout << "Failed to find target in atlasCoordsFile" << std::endl;
		return false;
	}
	mLineHeight = stoi(tempString.substr(targetString.size(), tempString.size() - targetString.size()));

	// Finding bitmap size
	targetString = "scaleW=";
	while (tempString.substr(0, targetString.size()) != targetString && !atlasCoordsFile.eof())
	{
		atlasCoordsFile >> tempString;
	}
	if (atlasCoordsFile.eof())
	{
		std::cout << "Failed to find target in atlasCoordsFile" << std::endl;
		return false;
	}
	mBitmapSize.x = stoi(tempString.substr(targetString.size(), tempString.size() - targetString.size()));
	targetString = "scaleH=";
	while (tempString.substr(0, targetString.size()) != targetString && !atlasCoordsFile.eof())
	{
		atlasCoordsFile >> tempString;
	}
	if (atlasCoordsFile.eof())
	{
		std::cout << "Failed to find target in atlasCoordsFile" << std::endl;
		return false;
	}
	mBitmapSize.y = stoi(tempString.substr(targetString.size(), tempString.size() - targetString.size()));

	// Getting number of characters to read
	int characterCount = 0;
	targetString = "count=";
	while (tempString.substr(0, targetString.size()) != targetString && !atlasCoordsFile.eof())
	{
		atlasCoordsFile >> tempString;
	}
	if (atlasCoordsFile.eof())
	{
		std::cout << "Failed to find target in atlasCoordsFile" << std::endl;
		return false;
	}
	characterCount = stoi(tempString.substr(targetString.size(), tempString.size() - targetString.size()));
	//std::cout << "Read for character count: " << characterCount << std::endl;

	// Getting rid of empty line
	std::getline(atlasCoordsFile, tempString);

	// Gettind data for each character
	for (int i = 0; i < characterCount; i++)
	{
		// Getting Line Data
		std::string readLine;
		std::getline(atlasCoordsFile, readLine);
		std::istringstream lineStream(readLine);

		// Reading line data to character
		Character tempChar;

		// Finding X pos
		std::string tempCharData = "";
		std::string targetString = "x=";
		while (tempCharData.substr(0, targetString.size()) != targetString && !atlasCoordsFile.eof())
		{
			lineStream >> tempCharData;
		}
		if (atlasCoordsFile.eof())
		{
			std::cout << "Failed to find target in atlasCoordsFile" << std::endl;
			return false;
		}
		float tempFloat = stof(tempCharData.substr(targetString.size(), tempCharData.size() - targetString.size()));
		tempChar.Positions.x = tempFloat / (float)mBitmapSize.x;

		// Finding Y pos
		targetString = "y=";
		while (tempCharData.substr(0, targetString.size()) != targetString && !atlasCoordsFile.eof())
		{
			lineStream >> tempCharData;
		}
		if (atlasCoordsFile.eof())
		{
			std::cout << "Failed to find target in atlasCoordsFile" << std::endl;
			return false;
		}
		tempFloat = stof(tempCharData.substr(targetString.size(), tempCharData.size() - targetString.size()));
		tempChar.Positions.y = tempFloat / (float)mBitmapSize.y;

		// Finding Char Size
		// Finding X size
		targetString = "width=";
		while (tempCharData.substr(0, targetString.size()) != targetString && !atlasCoordsFile.eof())
		{
			lineStream >> tempCharData;
		}
		if (atlasCoordsFile.eof())
		{
			std::cout << "Failed to find target in atlasCoordsFile" << std::endl;
			return false;
		}
		tempChar.Size.x = stoi(tempCharData.substr(targetString.size(), tempCharData.size() - targetString.size()));

		// Finding Y size
		targetString = "height=";
		while (tempCharData.substr(0, targetString.size()) != targetString && !atlasCoordsFile.eof())
		{
			lineStream >> tempCharData;
		}
		if (atlasCoordsFile.eof())
		{
			std::cout << "Failed to find target in atlasCoordsFile" << std::endl;
			return false;
		}
		tempChar.Size.y = stoi(tempCharData.substr(targetString.size(), tempCharData.size() - targetString.size()));

		// Completing position values
		tempChar.Positions.z = tempChar.Positions.x + (float)tempChar.Size.x / (float)mBitmapSize.x;
		tempChar.Positions.w = tempChar.Positions.y + (float)tempChar.Size.y / (float)mBitmapSize.y;

		// Finding Bearings 
		// Finding X bearing
		targetString = "xoffset=";
		while (tempCharData.substr(0, targetString.size()) != targetString && !atlasCoordsFile.eof())
		{
			lineStream >> tempCharData;
		}
		if (atlasCoordsFile.eof())
		{
			std::cout << "Failed to find target in atlasCoordsFile" << std::endl;
			return false;
		}
		tempChar.Bearing.x = stoi(tempCharData.substr(targetString.size(), tempCharData.size() - targetString.size()));

		// Finding Y bearing
		targetString = "yoffset=";
		while (tempCharData.substr(0, targetString.size()) != targetString && !atlasCoordsFile.eof())
		{
			lineStream >> tempCharData;
		}
		if (atlasCoordsFile.eof())
		{
			std::cout << "Failed to find target in atlasCoordsFile" << std::endl;
			return false;
		}
		tempChar.Bearing.y = stoi(tempCharData.substr(targetString.size(), tempCharData.size() - targetString.size()));
		//std::cout << "Read Char Bearing: (" << tempChar.Bearing.x << ", " << tempChar.Bearing.y << ")" << std::endl;

		// Finding Advance
		targetString = "xadvance=";
		while (tempCharData.substr(0, targetString.size()) != targetString && !atlasCoordsFile.eof())
		{
			lineStream >> tempCharData;
		}
		if (atlasCoordsFile.eof())
		{
			std::cout << "Failed to find target in atlasCoordsFile" << std::endl;
			return false;
		}
		tempChar.Advance = stoi(tempCharData.substr(targetString.size(), tempCharData.size() - targetString.size()));
		//std::cout << "Read Char Advance: " << tempChar.Advance << std::endl;

		// Adding Character
		mCharacters.push_back(tempChar);
	}

	// Closing buffer info atlasCoordsFile
	atlasCoordsFile.close();

	// Return true for proper texture atlas loading
	return true;
}
bool TextMeshBuffer::createGPUTextureFromSurface(SDL_Surface* surface)
{
	// Creating a SDL_GPUTexture out of the image data
	SDL_GPUTextureCreateInfo textureInfo =
	{
		.type = SDL_GPU_TEXTURETYPE_2D,
		.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
		.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER,
		.width = (uint32_t)surface->w,
		.height = (uint32_t)surface->h,
		.layer_count_or_depth = 1,
		.num_levels = 1
	};
	mpAtlasTexture = SDL_CreateGPUTexture(GraphicsDevice::getInstance()->getDevice(), &textureInfo);

	// Checking for errors with the GPU Texture
	if (!mpAtlasTexture)
	{
		std::cout << "Failed to create GPU Texture with errors: " << SDL_GetError() << std::endl;
		return false;
	}

	// Naming the GPU Texture
	std::string textureName = "Graphics_Texture_Atlas";
	SDL_SetGPUTextureName(GraphicsDevice::getInstance()->getDevice(), mpAtlasTexture, textureName.c_str());

	// Set up texture transfer data
	SDL_GPUTransferBufferCreateInfo transferBufferTexture =
	{
		.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
		.size = (uint32_t)surface->w * (uint32_t)surface->h * 4
	};
	SDL_GPUTransferBuffer* textureTransferBuffer = SDL_CreateGPUTransferBuffer(
		GraphicsDevice::getInstance()->getDevice(),
		&transferBufferTexture
	);

	void* textureTransferPtr = SDL_MapGPUTransferBuffer(
		GraphicsDevice::getInstance()->getDevice(),
		textureTransferBuffer,
		false
	);
	SDL_memcpy(textureTransferPtr, surface->pixels, surface->w * surface->h * 4);
	SDL_UnmapGPUTransferBuffer(GraphicsDevice::getInstance()->getDevice(), textureTransferBuffer);

	// Acquiring the GPU command buffer and starting a copy pass
	SDL_GPUCommandBuffer* uploadCmdBuf = SDL_AcquireGPUCommandBuffer(GraphicsDevice::getInstance()->getDevice());
	SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(uploadCmdBuf);

	// Transfering the buffer to the GPU
	SDL_GPUTextureTransferInfo textureTransferInfo =
	{
		.transfer_buffer = textureTransferBuffer,
		.offset = 0, /* Zeros out the rest */
	};
	SDL_GPUTextureRegion textureRegion =
	{
		.texture = mpAtlasTexture,
		.w = (uint32_t)surface->w,
		.h = (uint32_t)surface->h,
		.d = 1
	};
	SDL_UploadToGPUTexture(
		copyPass,
		&textureTransferInfo,
		&textureRegion,
		false
	);

	// Ending copy pass and submitting the command buffer
	SDL_EndGPUCopyPass(copyPass);
	SDL_SubmitGPUCommandBuffer(uploadCmdBuf);

	// Releasing the transfer buffer
	SDL_ReleaseGPUTransferBuffer(GraphicsDevice::getInstance()->getDevice(), textureTransferBuffer);

	// Return true for proper creation
	return true;
}
