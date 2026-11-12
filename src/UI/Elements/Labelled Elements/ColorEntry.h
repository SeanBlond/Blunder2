#ifndef COLOR_ENTRY
#pragma once

#include "../../UIElement.h"

class ColorEntry : public UIElement
{
public:
    ColorEntry(UIQuad quad, const std::string& label, Color* value, Event* clickEvent = nullptr)
        : mLabel(label), mpValue(value), mpClickEvent(clickEvent), UIElement(quad, UI_COLOR_ENTRY) 
    { generateInteractable(); }
    ~ColorEntry();

    // Override functions
    void generateInteractable() override;
    void onRelease(MouseClickData clickData) override;
    void drawElement(GraphicsRenderer* renderer, float layerOffset) override;

private:
    Color* mpValue;
    Color mSaveValue;
    std::string mLabel;
    Event* mpClickEvent = nullptr;

    // Interaction Elements
    UIQuad mInteractableQuad;
};

#endif // !COLOR_ENTRY
