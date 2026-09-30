#ifndef POS_COLOR_BUFFER
#pragma once

#include <iostream>
#include <vector>
#include <SDL3/SDL_gpu.h>
#include "../../smath/smath.h"
#include "../../Color/Color.h"
#include "Vertices.h"

// Buffer Class
class PosColorMeshBuffer
{
public:
	// Constructor & Deconstructor
	PosColorMeshBuffer() {}
	~PosColorMeshBuffer() { cleanup(); }

	// Data Management Functions
	bool init();
	void cleanup();

	// Buffer-modifying functions
	void addMesh(const PosColorMesh& mesh);
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
	SDL_GPUTransferBuffer* mpSharedTransferBuffer = nullptr;

	// Mesh that contains all added meshes
	PosColorMesh mPosColorMesh;

	// Members for keeping track of viewports
	std::vector<int> mViewIndexOffsets;

	// Members for keeping track of the capacity of the GPU Buffers
	size_t mVertexCapacity = 16;
	size_t mIndexCapacity = 16;
	size_t mTransferCapacity = 16;
	bool mTransferBufferReallocated = false;
};

#endif // !POS_COLOR_BUFFER
