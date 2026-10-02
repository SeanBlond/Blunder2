#ifndef FLOAT_ENTRY
#pragma once

#include "../UIElement.h"

class FloatEntry : public UIElement, public TextInput
{
public:
    FloatEntry(UIQuad quad, float* value, float speed = 0.1f, Event* clickEvent = nullptr)
        : mpValue(value), mSpeed(speed), mpClickEvent(clickEvent), UIElement(quad, UI_FLOAT_ENTRY), TextInput(std::to_string(*value)) {}

    // Element Functions
    void setValue(float value) { *(this->mpValue) = value; }
    void setValue(float* value) { this->mpValue = value; }

    // Override functions
    void generateInteractable() override;
    void onClick(MouseClickData clickData) override;
    void onHold(MouseClickData clickData) override;
    void onRelease(MouseClickData clickData) override;
    void drawElement(GraphicsRenderer* renderer, float layerOffset) override;
    TextInput* getTextInput() override { return this; }
    void handleInput() override;

private:
    float* mpValue;
    float mSpeed;
    Event* mpClickEvent = nullptr;

    // Interaction Elements
    smath::ivec2 mInitialMousePos;
    bool mSlideStarted;
    float mSaveValue;
};

#endif // !FLOAT_ENTRY
