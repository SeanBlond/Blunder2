#ifndef FLOAT_ENTRY
#pragma once

#include "../UIElement.h"

struct FloatClamps
{
    FloatClamps(float min, float max) : mMinValue(min), mMaxValue(max) {}

    float mMinValue;
    float mMaxValue;
};

class FloatEntry : public UIElement, public TextInput
{
public:
    FloatEntry(UIQuad quad, const std::string& label, float* value, float speed = 0.1f, Event* clickEvent = nullptr)
        : mLabel(label), mpValue(value), mSpeed(speed), mpClickEvent(clickEvent), mpClamps(nullptr), UIElement(quad, UI_FLOAT_ENTRY), TextInput(std::to_string(*value)) {}
    FloatEntry(UIQuad quad, const std::string& label, float* value, float min, float max, float speed = 0.1f, Event* clickEvent = nullptr)
        : mLabel(label), mpValue(value), mSpeed(speed), mpClickEvent(clickEvent), mpClamps(new FloatClamps(min, max)), UIElement(quad, UI_FLOAT_ENTRY), TextInput(std::to_string(*value)) {}
    ~FloatEntry() { delete mpClamps; mpClamps = nullptr; }

    // Element Functions
    void setValue(float value);
    void setValue(float* value) { this->mpValue = value; }
    float calculateSliderWidth(float originalWidth);

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
    FloatClamps* mpClamps = nullptr;
    std::string mLabel;
    Event* mpClickEvent = nullptr;

    // Interaction Elements
    UIQuad mInteractableQuad;
    smath::ivec2 mInitialMousePos;
    bool mSlideStarted;
    float mSaveValue;
};

#endif // !FLOAT_ENTRY
