#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>
#include "../smath/smath.h"
#include "../color/color.h"
#include "../graphics/GraphicsDevice.h"

class UIRenderer
{
public:
	// Enums
	enum TextSize { TEXT_ZERO = 0, TEXT_SMALL, TEXT_MEDIUM, TEXT_LARGE };

	// Constructor & Deconstructor
	UIRenderer(smath::vec2 dimensions, Color textColor = Color(1.0f)) :mDimensions(dimensions), mTextColor(textColor) {}
	~UIRenderer() {}

	// Getters
	//FontManager* getFontManager() { return mFontManager; }
	smath::vec2 getDimensions() { return mDimensions; }
	smath::vec2 getRelativeSize(smath::vec2 original) { return original / mDimensions; }
	float getUnitScale() { return mUnitScale; }
	float relativeScale(float value) { return value * (mDimensions.x / mUnitScale); }
	int getTextScale(TextSize size)
	{
		float textScale = 0.0f;
		switch (size)
		{
		case TEXT_ZERO:
			break;

		case TEXT_SMALL:
			textScale = relativeScale(18.0f);
			break;
		case TEXT_MEDIUM:
			textScale = relativeScale(24.0f);
			break;
		case TEXT_LARGE:
			textScale = relativeScale(36.0f);
			break;
		}
		return (int)textScale;
	}
	Color getTextColor() { return mTextColor; }

	// Setters
	void setDimensions(smath::vec2 dimensions) { this->mDimensions = dimensions; }
	void setTextColor(Color color) { this->mTextColor = color; }

private:
	smath::vec2 mDimensions;
	Color mTextColor;
	float mUnitScale = 800;
};