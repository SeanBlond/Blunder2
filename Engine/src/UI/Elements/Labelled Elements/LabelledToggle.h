#ifndef FLOAT_ENTRY
#pragma once

#include "../../UIElement.h"

class LabelledToggle : public UIElement
{
public:
    LabelledToggle(UIQuad quad, const std::string& label, bool* value, Event* clickEvent = nullptr)
        : mLabel(label), mpValue(value), mpClickEvent(clickEvent), UIElement(quad, UI_INT_ENTRY) {
        setValue(*mpValue); generateInteractable();
    }
    ~LabelledToggle() {}

    // Element Functions
    void setValue(const bool& value) { (*mpValue) = value; }
    void setValue(bool* value) { this->mpValue = value; }

    // Override functions
    void generateInteractable() override;
    void onRelease(MouseClickData clickData) override;
    void drawElement(GraphicsRenderer* renderer, float layerOffset) override;

private:
    bool* mpValue;
    std::string mLabel;
    Event* mpClickEvent = nullptr;

    // Interaction Elements
    UIQuad mInteractableQuad;
};

#endif // !FLOAT_ENTRY
