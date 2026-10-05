#ifndef GAME_OBJECT_BUFFER
#pragma once

#include <iostream>
#include <SDL3/SDL_gpu.h>
#include <smath/smath.h>
#include "../../GameObject/GameObjectManager.h"
#include "../../GameObject/Camera.h"
#include "Vertices.h"

// Buffer Class
class GameObjectBuffer
{
public:
	// Constructor & Deconstructor
	GameObjectBuffer() {}
	~GameObjectBuffer() { cleanup(); }

	// Data Management Functions
	bool init();
	void cleanup();

	// Window drawing position setters
	void setDisplayPosition(smath::vec3 position, smath::vec2 size);
	void setDisplayPosition(SDL_FRect rect, float zPos) { setDisplayPosition(smath::vec3(rect.x, rect.y, zPos), smath::vec2(rect.w, rect.h)); }

	// Getters
	Camera* getCamera() { return &mCamera; }

	// Buffer-modifying functions
	void setGameObjectManager(GameObjectManager* manager) { this->mpGameObjectManager = manager; }
	void drawBuffer(SDL_GPUCommandBuffer* commandBuffer, SDL_GPURenderPass* renderPass);

	// Texture management functions
	void updateTextureSize(const smath::ivec2& dimensions);
	void drawToTexture();

private:
	// GPU-Related Members
	SDL_GPUBuffer* mpVertexBuffer = nullptr;
	SDL_GPUBuffer* mpIndexBuffer = nullptr;
	SDL_GPUTexture* mpTargetColorTexture = nullptr;
	SDL_GPUTexture* mpTargetDepthTexture = nullptr;
	SDL_GPUGraphicsPipeline* mpDisplayPipeline = nullptr;
	SDL_GPUSampler* mpDisplaySampler = nullptr;

	// Pipeline will eventually be changed where each game object (should it have a material) will have it's own pipeline to use
	// In the meantime, this basic one is used that simply shows the game object's UVs
	SDL_GPUGraphicsPipeline* mpTempPipeline = nullptr;

	// Pointer to the game object manager
	GameObjectManager* mpGameObjectManager = nullptr; // DOES NOT OWN, DO NOT DEALLOCATE

	// Member for keeping track of rendering texture details
	smath::ivec2 mDimensions;
	Camera mCamera;

	// Members for drawing the buffer/textrue to the actual window
	smath::vec3 mDisplayPosition;
	smath::vec2 mDisplaySize;
	int mViewportIndex = 0;
};

#endif