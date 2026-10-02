#ifndef UI_ELEMENTS
#pragma once

#include "UIPositioning.h"
#include "UIRenderer.h"
#include "TextInput.h"
#include "../color/color.h"
#include "../Events/EventSystem.h"

// Enum for different UI elements
enum ElementType { UI_NONE, UI_FLOAT_SLIDER, UI_TEXT, UI_BUTTON, UI_TEXT_ENTRY, UI_SCROLLER };

// Base Class for a UI Element
class UIElement
{
public:
	// Constructor & Deconstructor
	UIElement(UIQuad quad, ElementType type)
		: mQuad(quad), mType(type), mpInteractable(nullptr) {
	}
	UIElement(SDL_FRect rect, ElementType type)
		: mQuad(rect.x, rect.y, rect.w, rect.h), mType(type), mpInteractable(nullptr) {
	}
	virtual ~UIElement()
	{
		// When deleted, check if the element is highlighted and/or clicked, 
		// and fire an event so the window manager knows to reset the pointers

		if (mHighlighted || mClicked)
			EventSystem::getInstance()->fire(SoftwareRuntimeEvent(EVENT_SOFTWARE_RESET_INTERACTION_ELEMENTS));

		delete mpInteractable;
		mpInteractable = nullptr;
	}

	// Getters
	UIQuad getQuad() { return mQuad; }
	ElementType getType() { return mType; }
	bool getClicked() { return mClicked; }
	bool getHighlighted() { return mHighlighted; }
	Interactable* getInteractable() { return mpInteractable; }
	bool checkCollision(smath::vec2 position) { return (mpInteractable ? mpInteractable->checkCollision(position) : false); }

	// Setters
	void setClicked(bool clicked) { mClicked = clicked; }
	void setHighlighted(bool highlighted) { mHighlighted = highlighted; }
	void setInteractable(Interactable* interactable) { delete mpInteractable; mpInteractable = interactable; }
	void setQuad(UIQuad quad) { mQuad = quad; generateInteractable(); }

	// Virtual Functions
	virtual void generateInteractable()              {}  // Does nothing by default
	virtual void onClick(MouseClickData clickData)   {}  // Does nothing by default
	virtual void onHold(MouseClickData clickData)    {}  // Does nothing by default
	virtual void onRelease(MouseClickData clickData) {}  // Does nothing by default
	virtual void drawElement(GraphicsRenderer* renderer, float layerOffset) = 0;
	virtual TextInput* getTextInput() { return nullptr; }

protected:
	Interactable* mpInteractable = nullptr;
	UIQuad mQuad;
	ElementType mType;
	bool mClicked = false;
	bool mHighlighted = false;
};

#endif // !UI_ELEMENTS
