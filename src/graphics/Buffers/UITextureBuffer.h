#ifndef UI_TEXTURE_BUFFER
#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <SDL3/SDL_gpu.h>
#include "../../smath/smath.h"
#include "../../Color/Color.h"
#include "Vertices.h"

// Buffer Class
class UITextureMeshBuffer
{
public:
	// Constructor & Deconstructor
	UITextureMeshBuffer() {}
	~UITextureMeshBuffer() { cleanup(); }

	// Data Management Functions
	bool init(std::string textureAtlasCoords, std::string textureAtlasImage);
	void cleanup();

	// Buffer-modifying functions
	void addMesh(const PosColorTexMesh& mesh);
	void addTexQuad(const smath::vec3& position, const smath::vec2& size, const std::string& textureKey, const Color& color = Color(1.0f), bool centered = false);
	void clearMesh();
	void startNewViewport() { mViewIndexOffsets.push_back(0); } // Creates new value to keep track of indices in the viewport
	void updateBuffers();
	void sendMeshToGPU(SDL_GPUCommandBuffer* currentCommandBuffer);
	void drawBuffer(SDL_GPUCommandBuffer* commandBuffer, SDL_GPURenderPass* renderPass);

private:
	// GPU-Related Members
	SDL_GPUGraphicsPipeline* mpPipeline = nullptr;
	SDL_GPUBuffer* mpVertexBuffer = nullptr;
	SDL_GPUBuffer* mpIndexBuffer = nullptr;
	SDL_GPUSampler* mpSampler = nullptr;
	SDL_GPUTransferBuffer* mpSharedTransferBuffer = nullptr;

	// Members for texture organization
	SDL_GPUTexture* mpAtlasTexture = nullptr;
	std::unordered_map<std::string, TextureAtlasCoord> mAtlasCoords;
	bool loadTextureAtlas(std::string atlasCoords, std::string atlasImage);
	bool createGPUTextureFromSurface(SDL_Surface* surface);

	// Members for keeping track of viewports
	std::vector<int> mViewIndexOffsets;

	// Mesh that contains all added meshes
	PosColorTexMesh mUITexMesh;

	// Members for keeping track of the capacity of the GPU Buffers
	size_t mVertexCapacity = 16;
	size_t mIndexCapacity = 16;
	size_t mTransferCapacity = 16;
	bool mTransferBufferReallocated = false;
};

#endif