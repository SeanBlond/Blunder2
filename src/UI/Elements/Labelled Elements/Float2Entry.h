#ifndef FLOAT2_ENTRY
#pragma once

#include "../../UIElement.h"

class Float2Entry : public UIElement, public TextInput
{
public:
    Float2Entry(UIQuad quad, const std::string& label, smath::vec2* value, float speed = 0.1f, Event* clickEvent = nullptr)
        : mLabel(label), mpValue(value), mSpeed(speed), mpClickEvent(clickEvent), UIElement(quad, UI_FLOAT2_ENTRY), TextInput("") {
        generateInteractable();
    }
    ~Float2Entry();

    // Element Functions
    void setValue(smath::vec2* value) { this->mpValue = value; }
    uint8_t determineAxisSelected(const smath::vec2& position);
    float* getSelectedAxis();

    // Override functions
    void generateInteractable() override;
    void onClick(MouseClickData clickData) override;
    void onHold(MouseClickData clickData) override;
    void onRelease(MouseClickData clickData) override;
    void drawElement(GraphicsRenderer* renderer, float layerOffset) override;
    TextInput* getTextInput() override { return this; }
    void handleInput() override;

private:
    smath::vec2* mpValue;
    float mSpeed;
    std::string mLabel;
    Event* mpClickEvent = nullptr;

    // Interaction Elements
    UIQuad mInteractableQuad;
    smath::ivec2 mInitialMousePos;
    float mSaveValue;
    float mAxisXPositions[2];
    uint8_t mAxisSelected = 0; // 0 = X, 1 = Y, 2 = Z, 3 = W
    bool mSlideStarted;
};

#endif // !FLOAT2_ENTRY
