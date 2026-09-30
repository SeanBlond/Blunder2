#pragma once
#include "../UIElement.h"

class FloatSlider : public UIElement
{
public:
    FloatSlider(UIQuad quad, float* value, float min = 0.0f, float max = 1.0f, Event* event = nullptr)
        : mpValue(value), mMin(min), mMax(max), mDefaultValue(0.0f), mDefaultValueSet(false), mpClickEvent(event), UIElement(quad, UI_FLOAT_SLIDER) { generateInteractable(); }
    FloatSlider(UIQuad quad, float* value, float min, float max, float defaultValue, Event* event = nullptr)
        : mpValue(value), mMin(min), mMax(max), mDefaultValue(defaultValue), mDefaultValueSet(true), mpClickEvent(event), UIElement(quad, UI_FLOAT_SLIDER) { generateInteractable(); }
    ~FloatSlider() { delete mpClickEvent; mpClickEvent = nullptr; }

    // Getters
    float getPercentage() { return smath::clamp((*mpValue - mMin) / (mMax - mMin), 0.0f, 1.0f); }
    float getPercentageFromValue(float value) { return smath::clamp((value - mMin) / (mMax - mMin), 0.0f, 1.0f); }
    float getPercentageFromPosition(float xPosition) { return (xPosition - mQuad.x) / mQuad.w; }
    float getXPosFromPercentage(float percentage) { return mQuad.x + (percentage * mQuad.w); }
    float getXPosFromValue(float value) { return getXPosFromPercentage(getPercentageFromValue(value)); }

    // Setters
    void setValue(float value) { *(mpValue) = smath::clamp(value, mMin, mMax); }
    void setValue(float* value) { mpValue = value; }
    void setValueByLerp(float t) { setValue(smath::lerp(mMin, mMax, t)); }
    void setValueByLerpedPos(float xPosition) { setValueByLerp(getPercentageFromPosition(xPosition)); }

    // Functions
    void generateInteractable() override;
    void onClick(MouseClickData clickData) override;
    void onHold(MouseClickData clickData) override;
    void drawElement(GraphicsRenderer* renderer, float layerOffset) override;

private:
    float* mpValue;
    float mMin;
    float mMax;
    float mDefaultValue;
    bool mDefaultValueSet;
    Event* mpClickEvent;
};