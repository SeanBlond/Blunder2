#ifndef TEXT_BUFFER
#pragma once

#include <iostream>
#include <vector>
#include <regex>
#include <unordered_map>
#include <SDL3/SDL_gpu.h>
#include "Vertices.h"

enum HorizontalTextAlign { TEXT_H_LEFT, TEXT_H_CENTER, TEXT_H_RIGHT };
enum VerticalTextAlign { TEXT_V_TOP, TEXT_V_MIDDLE, TEXT_V_BOTTOM };

struct Character
{
	smath::vec4  Positions; // Corner Positon of the glyph on the bitmap UV  y---w
	smath::ivec2 Size;      // Size of the glyph                             |   |
	smath::ivec2 Bearing;   // Offset from baseline to left/top of glyph     |   |
	unsigned int Advance;   // Horizontal offset to advance to next glyph    x---z
};

class TextMeshBuffer
{
public:
	// Constructor & Deconstructor
	TextMeshBuffer() {}
	~TextMeshBuffer() { cleanup(); }

	// Data Management Functions
	bool init(std::string textureAtlasCoords, std::string textureAtlasImage);
	void cleanup();

	// Getters
	Character getCharacter(char character) { return mCharacters[(int)character - mFirstChar]; }

	// Buffer-modifying functions
	void addText(
		const std::string& text, const smath::vec2& pos, const float& zPos, const float& size, Color color, 
		HorizontalTextAlign horizontalAlignment, VerticalTextAlign verticalAlignment);
	smath::vec2 getPositionInText(
		const std::string& text, int index, const smath::vec2& pos, const float& size, 
		HorizontalTextAlign horizontalAlignment, VerticalTextAlign verticalAlignment);
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

	// Private function to add mesh data (so that only text quads can be added)
	void addMesh(const PosColorTexMesh& mesh);

	// Text Information
	int mFontSize = 0;
	int mLineHeight = 0;
	uint8_t mFirstChar = 0;
	uint8_t mLastChar = 0;
	smath::ivec2 mBitmapSize;
	std::vector<Character> mCharacters;

	// Mesh that contains all added meshes
	PosColorTexMesh mTextMesh;

	// Members for keeping track of viewports
	std::vector<int> mViewIndexOffsets;

	// Members for keeping track of the capacity of the GPU Buffers
	size_t mVertexCapacity = 16;
	size_t mIndexCapacity = 16;
	size_t mTransferCapacity = 16;
	bool mTransferBufferReallocated = false;
};

#endif