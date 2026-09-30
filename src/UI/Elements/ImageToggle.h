#pragma once
#include "../UIElement.h"

class ImageToggle : public UIElement
{
public:
    ImageToggle(UIQuad quad, bool* value, std::string trueImageKey, std::string falseImageKey, Event* clickEvent = nullptr)
        : mValue(value), mTrueImageKey(trueImageKey), mFalseImageKey(falseImageKey), mClickEvent(clickEvent), UIElement(quad, UI_BUTTON) {
        generateInteractable();
    }
    ~ImageToggle() { delete mClickEvent; mClickEvent = nullptr; }

    // Override functions
    void generateInteractable() override;
    void onClick(MouseClickData clickData) override;
    void drawElement(GraphicsRenderer* renderer, float layerOffset) override;

private:
    std::string mTrueImageKey;
    std::string mFalseImageKey;
    bool* mValue;
    Event* mClickEvent;
};