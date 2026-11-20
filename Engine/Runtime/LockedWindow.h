#ifndef LOCKED_WINDOW
#pragma once

#include "../UI/UI.h"


class LockedWindow
{
public:
	// Data necessary for getting offset
	enum ChildWindowPosition { POS_NONE, POS_LEFT, POS_RIGHT, POS_BOTTOM, POS_TOP };

	// Constructor & Deconstructor
	LockedWindow(UIWindow* window, LockedWindow* parent, smath::vec2 dimensions, ChildWindowPosition position, float spacing) : mpWindow(window), mpParent(parent), mDimensions(dimensions), mChildPosition(position), mSpacing(spacing) {}
	~LockedWindow();

	// Getters
	UIWindow* getWindow() const { return this->mpWindow; }
	LockedWindow* getLeftWindow() const { return mpLeftWindow; };
	LockedWindow* getRightWindow() const { return mpRightWindow; };
	LockedWindow* getTopWindow() const { return mpTopWindow; };
	LockedWindow* getBottomWindow() const { return mpBottomWindow; };

	float getLeftWidth() const { return (mpLeftWindow ? mLeftWidth : 0); }
	float getRightWidth() const { return (mpRightWindow ? mRightWidth : 0); }
	float getTopHeight() const { return (mpTopWindow ? mTopHeight : 0); }
	float getBottomHeight() const { return (mpBottomWindow ? mBottomHeight : 0); }

	smath::vec2 getDimensions() const { return mDimensions; }
	smath::vec2 getMainWindowDimensions() const;
	LockedWindow* getParent() { return mpParent; }
	float getXOffset() const;
	float getYOffset() const;
	smath::vec2 getOffset() const { return smath::vec2(getXOffset(), getYOffset()); }
	smath::vec4 getScreenCorners() const { return smath::vec4(getOffset(), getOffset() + getDimensions()); }
	UIQuad getRect() const { return UIQuad(getOffset(), getMainWindowDimensions()); }

	// Setters
	void setDimensions(smath::vec2 dimensions) { this->mDimensions = dimensions; UpdateDimensions(); }
	void setDimensions(smath::vec2 dimensions, float spacing) { this->mDimensions = dimensions; UpdateDimensions(); this->mSpacing = spacing; }
	void setLeftWindow(UIWindow* window, float width = 0.25f);
	void setRightWindow(UIWindow* window, float width = 0.25f);
	void setTopWindow(UIWindow* window, float height = 0.25f);
	void setBottomWindow(UIWindow* window, float height = 0.25f);

	void setLeftWidth(float leftWidth) { this->mLeftWidth = smath::clamp(leftWidth, 0.0f, 1.0f - mPercentageUsed.x - (mSpacing * 2.0f / mDimensions.x)); UpdateDimensions(); }
	void setRightWidth(float rightWidth) { this->mRightWidth = smath::clamp(rightWidth, 0.0f, 1.0f - mPercentageUsed.x - (mSpacing * 2.0f / mDimensions.x)); UpdateDimensions(); }
	void setTopHeight(float topHeight) { this->mTopHeight = smath::clamp(topHeight, 0.0f, 1.0f - mPercentageUsed.y - (mSpacing * 2.0f / mDimensions.y)); UpdateDimensions(); }
	void setBottomHeight(float bottomHeight) { this->mBottomHeight = smath::clamp(bottomHeight, 0.0f, 1.0f - mPercentageUsed.y - (mSpacing * 2.0f / mDimensions.y)); UpdateDimensions(); }
	void setParent(LockedWindow* parent) { this->mpParent = parent; }

	// Functions
	void UpdateDimensions();
	UIWindow* checkForCollisions(smath::vec2 position);
	void DrawWindows(GraphicsRenderer* renderer);
	void ChangeWindow(UIWindow* newWindow);

private:
	// Window data
	ChildWindowPosition mChildPosition;
	LockedWindow* mpParent = nullptr;
	UIWindow* mpWindow = nullptr;
	smath::vec2 mDimensions;
	smath::vec2 mPercentageUsed; // Between 0.0 to 1.0
	float mSpacing = 0.0f;

	// Connected Windows
	LockedWindow* mpLeftWindow = nullptr;
	LockedWindow* mpRightWindow = nullptr;
	LockedWindow* mpTopWindow = nullptr;
	LockedWindow* mpBottomWindow = nullptr;
	float mLeftWidth = 0.0f;
	float mRightWidth = 0.0f;
	float mTopHeight = 0.0f;
	float mBottomHeight = 0.0f;
};

#endif