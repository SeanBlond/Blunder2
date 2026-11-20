#ifndef FLOAT3_ENTRY
#pragma once

#include "../../UIElement.h"

class Float3Entry : public UIElement, public TextInput
{
public:
    Float3Entry(UIQuad quad, const std::string& label, smath::vec3* value, float speed = 0.1f, Event* clickEvent = nullptr)
        : mLabel(label), mpValue(value), mSpeed(speed), mpClickEvent(clickEvent), UIElement(quad, UI_FLOAT3_ENTRY), TextInput("") {
        generateInteractable();
    }
    ~Float3Entry();

    // Element Functions
    void setValue(smath::vec3* value) { this->mpValue = value; }
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
    smath::vec3* mpValue;
    float mSpeed;
    std::string mLabel;
    Event* mpClickEvent = nullptr;

    // Interaction Elements
    UIQuad mInteractableQuad;
    smath::ivec2 mInitialMousePos;
    float mSaveValue;
    float mAxisXPositions[3];
    uint8_t mAxisSelected = 0; // 0 = X, 1 = Y, 2 = Z, 3 = W
    bool mSlideStarted;
};

#endif // !FLOAT3_ENTRY
