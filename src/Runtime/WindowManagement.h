#ifndef WINDOW_MANAGEMENT
#pragma once

#include "../UI/UI.h"
#include "LockedWindow.h"
#include "../Events/EventListener.h"
#include "../Events/EventSystem.h"

class WindowManager : public EventListener
{
public:
	// Constructor & Deconstructor
	WindowManager() {}
	~WindowManager() { cleanup(); }

	// Event-related functions
	void handleEvent(const Event& event) override;

	// Management Functions
	bool init(const unsigned int& width, const unsigned int& height);
	void cleanup();

	// Getters
	LockedWindow* getRootWindow() { return mpRootWindow; }

	// Runtime Functions
	void updateWindows(smath::vec2 screenDimensions);
	void drawWindows(GraphicsRenderer* renderer);
	UIElement* checkForElementCollision(smath::vec2 pos);
	void changeRootWindow(RootWindowOptions option);

private:
	smath::vec2 mScreenDimensions;
	LockedWindow* mpRootWindow = nullptr;
	RootWindowOptions mCurrentRootWindow = WINDOW_NONE;

	// UI Element Interaction Members
	UIElement* mClickedElement = nullptr;
	UIElement* mHighlightedElement = nullptr;
	TextInput* mActiveTextInput = nullptr;

	// Useful UI Windows 
	TestWindow* mpViewport = nullptr;
	TestWindow* mpHierarchy = nullptr;
	TestWindow* mpExplorerer = nullptr;
	ComponentWindow* mpComponents = nullptr;
};

#endif // !WINDOW_MANAGEMENT
