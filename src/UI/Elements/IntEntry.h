#ifndef FLOAT_ENTRY
#pragma once

#include "../UIElement.h"

struct IntClamps
{
    IntClamps(int min, int max) : mMinValue(min), mMaxValue(max) {}

    int mMinValue;
    int mMaxValue;
};

class IntEntry : public UIElement, public TextInput
{
public:
    IntEntry(UIQuad quad, const std::string& label, int* value, float speed = 0.1f, Event* clickEvent = nullptr)
        : mLabel(label), mpValue(value), mSpeed(speed), mpClickEvent(clickEvent), mpClamps(nullptr), UIElement(quad, UI_INT_ENTRY), TextInput(std::to_string(*value)) { setValue(*mpValue); }
    IntEntry(UIQuad quad, const std::string& label, int* value, int min, int max, float speed = 0.1f, Event* clickEvent = nullptr)
        : mLabel(label), mpValue(value), mSpeed(speed), mpClickEvent(clickEvent), mpClamps(new IntClamps(min, max)), UIElement(quad, UI_INT_ENTRY), TextInput(std::to_string(*value)) { setValue(*mpValue); }
    ~IntEntry() { delete mpClamps; mpClamps = nullptr; }

    // Element Functions
    void setValue(const int& value);
    void setValue(int* value) { this->mpValue = value; }
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
    int* mpValue;
    float mSpeed;
    IntClamps* mpClamps = nullptr;
    std::string mLabel;
    Event* mpClickEvent = nullptr;

    // Interaction Elements
    UIQuad mInteractableQuad;
    smath::ivec2 mInitialMousePos;
    bool mSlideStarted;
    int mSaveValue;
};

#endif // !FLOAT_ENTRY
