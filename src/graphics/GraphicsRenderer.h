#ifndef GRAPHICS_RENDERER
#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <SDL3/SDL_gpu.h>
#include <smath/smath.h>
#include "Buffers/PosColorBuffer.h"
#include "Buffers/UITextureBuffer.h"
#include "Buffers/TextBuffer.h"
#include "../Color/Color.h"

enum TextSize { TEXT_SMALL, TEXT_NORMAL, TEXT_LARGE };
enum RenderLayer 
{
	BASE_LAYER,            // 0.0+
	BASE_LAYER_2,          // 0.1+
	BASE_LAYER_3,          // 0.2+
	FREE_LAYER,            // 0.3+
	UI_POPUP_LAYER,        // 0.4+
	WINDOW_POPUP_LAYER	   // 0.5+
};

class GraphicsRenderer
{
public:
	// Constructor & Deconstructor
	GraphicsRenderer() {}
	~GraphicsRenderer() { cleanup(); }

	// Data Management Functions
	bool init(
		std::string uiTextureAtlasCoords, std::string uiTextureAtlasImage,
		std::string textTextureAtlasCoords, std::string textTextureAtlasImage,
		float uiScale);
	void cleanup();

	// Getters
	float getUIScale() const { return mUIScale; }
	float getTextSize(const TextSize& size) const;

	// Setters
	void setUIScale(const float& scale) { mUIScale = smath::max(0.25f, scale); }
	void incrementUIScale(const float& amount) { setUIScale(mUIScale + amount); }

	// Layer related functions
	RenderLayer getCurrentLayer() { return mCurrentLayer; }
	float getCurrentLayerValue() { return mCurrentLayerValue; }
	void setActiveLayer(RenderLayer layer) { mCurrentLayer = layer; mCurrentLayerValue = (float)mCurrentLayer / 10.0f; }

	// Functions for adding meshes to the PosColor mesh vector
	void addMesh(const PosColorMesh& mesh);
	void addLine(const smath::vec2& startPos, const smath::vec2& endPos, const float& layerOffset, const Color& color, const float& thickness);
	void addRectangle(const SDL_FRect& rect, const float& layerOffset, const Color& color);
	void addRectangle(const smath::vec2& position, const smath::vec2& size, const float& layerOffset, const Color& color);
	void addRectangleOutline(const smath::vec2& position, const smath::vec2& size, const float& layerOffset, const Color& color, const float& thickness);
	void addRectangleOutline(const SDL_FRect& rect, const float& layerOffset, const Color& color, const float& thickness);
	void addCircle(const smath::vec2& center, const smath::vec2& radii, const float& layerOffset, const Color& color, const int& quality = 24);
	void addCircle(const smath::vec2& center, const float& radius, const float& layerOffset, const Color& color, const int& quality = 24);
	void addCircleOutline(const smath::vec2& center, const smath::vec2& radii, const float& layerOffset, const Color& color, const float& thickness, const int& quality = 24);
	void addCircleOutline(const smath::vec2& center, const float& radius, const float& layerOffset, const Color& color, const float& thickness, const int& quality = 24);
	void addStar(const smath::vec2& center, const float& outerRadius, const float& layerOffset, const Color& color);
	void addStarOutline(const smath::vec2& center, const float& outerRadius, const float& layerOffset, const Color& color, const float& thickness);

	// Functions for adding meshes to the PosColorTex buffer
	void addTexQuad(const SDL_FRect& rectangle, const float& layerOffset, const std::string& textureKey, const Color& color = Color(1.0f), bool centered = false);
	void addTexQuad(const smath::vec2& position, const smath::vec2& size, const float& layerOffset, const std::string& textureKey, const Color& color = Color(1.0f), bool centered = false);

	// Function for adding text to the Text Buffer
	void addText(
		const std::string& text, const smath::vec2& pos, const float& layerOffset, const float& size, Color color = Color(1),
		HorizontalTextAlign horizontalAlignment = TEXT_H_LEFT, VerticalTextAlign verticalAlignment = TEXT_V_BOTTOM)
		{ mpTextBuffer->addText(text, pos, mCurrentLayerValue + layerOffset, size, color, horizontalAlignment, verticalAlignment); }
	smath::vec2 getPositionInText(
		const std::string& text, int index, const smath::vec2& pos, const float& size,
		HorizontalTextAlign horizontalAlignment, VerticalTextAlign verticalAlignment)
		{ return mpTextBuffer->getPositionInText(text, index, pos, size, horizontalAlignment, verticalAlignment); }

	// Viewport functions
	void setNewViewport(int x, int y, int w, int h);
	void setNewViewport(smath::ivec2 position, smath::ivec2 size) { setNewViewport(position.x, position.y, size.x, size.y); }
	void setNewViewport(SDL_Rect rect) { setNewViewport(rect.x, rect.y, rect.w, rect.h); }
	void setNewViewport(SDL_FRect rect) { setNewViewport((int)(rect.x), (int)(rect.y), (int)(rect.w), (int)(rect.h)); }
	SDL_Rect getViewport(int index) { return mViewports[smath::clamp(index, 0, (int)mViewports.size() - 1)]; }
	SDL_Rect* getViewportAddress(int index) { return &(mViewports[smath::clamp(index, 0, (int)mViewports.size() - 1)]); }
	int getViewportCount() { return mViewports.size(); }

	// Runtime buffer functions
	void updateDepthTexture(smath::ivec2 dimensions);
	void drawBuffers();

private:
	// Buffers used for rendering
	PosColorMeshBuffer* mpPosColorBuffer = nullptr;
	UITextureMeshBuffer* mpUITextureBuffer = nullptr;
	TextMeshBuffer* mpTextBuffer = nullptr;

	// Depth related members
	SDL_GPUTexture* mpDepthTexture = nullptr;
	smath::ivec2 mDepthTextureDimensions;

	// Viewports
	std::vector<SDL_Rect> mViewports;

	// Member variables for scaling
	float mUIScale = 1.0f;

	// Layer related members
	RenderLayer mCurrentLayer = BASE_LAYER;
	float mCurrentLayerValue = 0.0f;
};


#endif